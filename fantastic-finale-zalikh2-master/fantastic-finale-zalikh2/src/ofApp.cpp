#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	//setup background
    ofBackground(ofColor::purple);
    bg_color_1 = ofColor::red;
    bg_color_2 = ofColor::blue;

	//create sound players
    ofSoundPlayer symphony;
    symphony.load("symphony.mp3");
    symphony.setLoop(true);

    ofSoundPlayer rock;
    rock.load("rock.mp3");
    rock.setLoop(true);

    ofSoundPlayer rap;
    rap.load("rap.mp3");
    rap.setLoop(true);

    songs = {symphony, rock, rap};
    std::vector<std::string> song_names = {"symphony", "rock", "rap"};
    current_song = symphony;

	//initalize sound data
    fftSmooth.fill(0);
    beat_magnitude = 0;
    max_magnitude = 0;
    smoothing_factor = kDefaultSmoothingFactor;
    use_scaling = false;

    // set up particle GUIS
    for (int i = 0; i < kNumParticles; i++) {
        particle_guis.push_back(new ofxDatGui());
        ParticleSettings settings;
        particle_settings.push_back(settings);
        frequency_sums.push_back(0);
        previous_sums.push_back(0);
        max_sums.push_back(0);
    }

    // set default state of settings and GUI
    SetDefaultParticleGUIs();
    SetDefaultParticleSettings();

    // set up general GUI
    main_gui = new ofxDatGui(ofxDatGuiAnchor::TOP_RIGHT);
    main_gui->addColorPicker("Color 1", ofColor::red);
    main_gui->addColorPicker("Color 2", ofColor::blue);
    main_gui->addButton("Reset");
    main_gui->addSlider("Smoothing factor", kMinSmoothingFactor, kMaxSmoothingFactor, 
		kDefaultSmoothingFactor);
    main_gui->addDropdown("Song", song_names);
    main_gui->addToggle("Relative Scaling", true);
    main_gui->onColorPickerEvent(this, &ofApp::OnBackgroundColorChanged);
    main_gui->onSliderEvent(this, &ofApp::OnSmoothingSliderChanged);
    main_gui->onButtonEvent(this, &ofApp::OnResetButtonClicked);
    main_gui->onDropdownEvent(this, &ofApp::OnSongChanged);
    main_gui->onToggleEvent(this, &ofApp::OnScalingToggled);
}

//--------------------------------------------------------------
void ofApp::update() {
    // get the sound data at this frame
    ofSoundUpdate();
    float* spectrum = ofSoundGetSpectrum(kNumAudioBands);
    for (int i = 0; i < kNumAudioBands; i++) {
        fftSmooth[i] = beat.getBand(i);
    }

    // smooth out the audio frequency numbers
    for (int i = 0; i < kNumParticles; i++) {
        previous_sums[i] = frequency_sums[i];
        frequency_sums[i] =
            Particle::GetSumFrequencies(particle_settings[i], spectrum);
        if (frequency_sums[i] < smoothing_factor * previous_sums[i]) {
            frequency_sums[i] = smoothing_factor * previous_sums[i];
        }
        if (frequency_sums[i] > max_sums[i]) {
            max_sums[i] = frequency_sums[i];
        }
        cout << i << ": " << max_sums[i] << endl;
    }

    // track the beat magnitude numbers using the audio
    beat.update(ofGetElapsedTimeMillis());

    beat.audioReceived(spectrum, beat.getBufferSize(), 2);

    // calculate the maximum magnitude so far in the audio
    beat_magnitude = beat.getMagnitude();
    if (beat_magnitude > max_magnitude) {
        max_magnitude = beat_magnitude;
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(Drawer::GetColorFromPercentage(beat_magnitude / max_magnitude,
                                                bg_color_1, bg_color_2));
    // only draw the particle if a song is playing
    if (current_song.isPlaying()) {
        for (int i = 0; i < kNumParticles; i++) {
            // smooth out the value of the frequencies and draw the particle
			// based on if the user toggled scaling or not
            double size;
            if (use_scaling) {
                size = particle_settings[i].max_size * frequency_sums[i] /
                       max_sums[i];
                cout << i << ": " << size << endl;
            } else {
                size = ofMap(frequency_sums[i], 0, 0.05, kMinDrawingSize,
                             particle_settings[i].max_size, true);
            }

            Particle::DrawParticle(particle_settings[i], size);
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    // if the key pressed is a number, toggle the particle GUI state
    // appropiately
    if (isdigit(key)) {
        int particle_index = key - 1 - kASCIIDigitOffset;
        if (particle_index >= 0 || particle_index < particle_guis.size()) {
            if (particle_guis[particle_index]->getVisible()) {
                particle_guis[particle_index]->setVisible(false);
            } else {
                SetGUIVisible(particle_index, particle_guis);
            }
        }
    }

    // check for other key options
    switch (key) {
            // toggle the settings menu
        case 's':
            main_gui->setVisible(!main_gui->getVisible());
            break;
        case 'p':
            // reset magnitude and switch the state of the playing song
            if (current_song.isPlaying()) {
                current_song.stop();
                max_magnitude = 0;
            } else {
                current_song.play();
            }
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {}

void ofApp::OnParticlePostionChanged(ofxDatGui2dPadEvent event) {
    int index = GetGUIIndexFromLabel(event.target->getLabel());
    if (index != kInvalidIndex) {
        // change particle position and update locations
        particle_settings[index].center = event.target->getPoint();
        particle_settings[index].locations =
            Particle::GeneratePointsAroundLocation(
                particle_settings[index].center,
                particle_settings[index].num_particles,
                particle_settings[index].radius);
    }
}

void ofApp::OnParticleSliderChanged(ofxDatGuiSliderEvent event) {
    int index = GetGUIIndexFromLabel(event.target->getLabel());
    if (index != kInvalidIndex) {
        // first check if one of the sliders was for the rotation
        char axis_rotation = event.target->getLabel().front();
        if (axis_rotation == 'X') {
            particle_settings[index].x_rotation_speed =
                event.target->getValue();
        } else if (axis_rotation == 'Y') {
            particle_settings[index].y_rotation_speed =
                event.target->getValue();
        } else if (axis_rotation == 'Z') {
            particle_settings[index].z_rotation_speed =
                event.target->getValue();
            // also check for the color interval size and radius.
        } else if (event.target->getLabel().rfind("Color Interval", 0) == 0) {
            particle_settings[index].color_interval = event.target->getValue();
        } else if (event.target->getLabel().rfind("Max Size", 0) == 0) {
            particle_settings[index].max_size = event.target->getValue();
        } else if (event.target->getLabel().rfind("Radius", 0) == 0) {
            // if the radius is changed we also need to update the locations
            particle_settings[index].radius = event.target->getValue();
            particle_settings[index].locations =
                Particle::GeneratePointsAroundLocation(
                    particle_settings[index].center,
                    particle_settings[index].num_particles,
                    particle_settings[index].radius);
        }
    }
}

void ofApp::OnBackgroundColorChanged(ofxDatGuiColorPickerEvent event) {
    // check if one the the background colors was changed and update the values
    // properly.
    if (event.target->is("Color 1")) {
        bg_color_1 = event.target->getColor();
    } else if (event.target->is("Color 2")) {
        bg_color_2 = event.target->getColor();
    }
}

void ofApp::OnSmoothingSliderChanged(ofxDatGuiSliderEvent event) {
    smoothing_factor = event.target->getValue();
}

void ofApp::OnResetButtonClicked(ofxDatGuiButtonEvent event) {
    SetDefaultParticleSettings();
    SetDefaultParticleGUIs();
}

void ofApp::OnSongChanged(ofxDatGuiDropdownEvent event) {
    std::string song_name = event.target->getSelected()->getName();
    current_song.stop();
    // no easy way to get name of the song, so hardcoded checks are required
    if (song_name == "symphony") {
        current_song = songs[0];
    } else if (song_name == "rock") {
        current_song = songs[1];
    } else if (song_name == "rap") {
        current_song = songs[2];
    }
}

void ofApp::OnScalingToggled(ofxDatGuiToggleEvent event) {
    if (event.target->getName() == "Relative Scaling") {
        use_scaling = event.checked;
    }
}

void ofApp::OnParticleColorChanged(ofxDatGuiColorPickerEvent event) {
    int gui_index = GetGUIIndexFromLabel(event.target->getLabel());
    if (gui_index != kInvalidIndex) {
        if (event.target->getLabel().rfind("Color 1", 0) == 0) {
            particle_settings[gui_index].color_one = event.color;
        } else if (event.target->getLabel().rfind("Color 2", 0) == 0) {
            particle_settings[gui_index].color_two = event.color;
        }
    }
}

void ofApp::OnParticleShapeChanged(ofxDatGuiDropdownEvent event) {
    int index = kInvalidIndex;
    for (int i = 0; i < kNumParticles; i++) {
        if (particle_guis[i]->getVisible()) {
            index = i;
        }
    }
    if (index != kInvalidIndex) {
        // get the shape and update the settings
        particle_settings[index].shape =
            GetShapeFromName(event.target->getSelected()->getName());
    }
}
void ofApp::OnParticleToggled(ofxDatGuiToggleEvent event) {
    // change the settings for whether the particle is toggled or not
    int index = GetGUIIndexFromLabel(event.target->getLabel());
    if (index != kInvalidIndex) {
        particle_settings[index].is_toggled = event.checked;
    }
}

void ofApp::OnNumParticlesChanged(ofxDatGuiMatrixEvent event) {
    int index = GetGUIIndexFromLabel(event.target->getLabel());
    if (index != kInvalidIndex) {
        // get the number selected and update the settings
        particle_settings[index].num_particles =
            event.target->getSelected()[0] + 1;
        particle_settings[index].locations =
            Particle::GeneratePointsAroundLocation(
                particle_settings[index].center,
                particle_settings[index].num_particles,
                particle_settings[index].radius);
        // reset the state of the button
        event.target->clear();
    }
}

void ofApp::SetGUIVisible(int gui_index, std::vector<ofxDatGui*> guis) {
    for (int i = 0; i < guis.size(); i++) {
        if (i == gui_index) {
            // if this is the GUI we want to show, set it visible
            guis[i]->setVisible(true);
        } else {
            // otherwise, set the other particle GUIs invisible
            guis[i]->setVisible(false);
        }
    }
}

Shape ofApp::GetShapeFromName(std::string shape_name) {
    // get the shape name based on the string value
    if (shape_name == "Triangle") {
        return Shape::Triangle;
    } else if (shape_name == "Square") {
        return Shape::Square;
    } else if (shape_name == "Circle") {
        return Shape::Circle;
    } else if (shape_name == "Star") {
        return Shape::Star;
    } else {
        return Shape::None;
    }
}

int ofApp::GetGUIIndexFromLabel(std::string label) {
    if (isdigit(label.back())) {
        int index = label.back() - 1 - kASCIIDigitOffset;
        return index;
    } else {
        return kInvalidIndex;
    }
}

void ofApp::SetDefaultParticleSettings() {
    // set all of the settings structs to their default values
    for (int i = 0; i < particle_settings.size(); i++) {
        std::vector<ofPoint> locations = {
            ofPoint(ofGetWidth() / 2, ofGetHeight() / 2)};
        particle_settings[i].center =
            ofPoint(ofGetWidth() / 2, ofGetHeight() / 2);
        particle_settings[i].locations = locations;
        particle_settings[i].radius = kDefaultRadius;
        particle_settings[i].is_toggled = true;
        particle_settings[i].x_rotation_speed = kDefaultRotationSpeed;
        particle_settings[i].y_rotation_speed = kDefaultRotationSpeed;
        particle_settings[i].z_rotation_speed = kDefaultRotationSpeed;
        particle_settings[i].color_one = ofColor::green;
        particle_settings[i].color_two = ofColor::purple;
        particle_settings[i].color_interval = kDefaultColorInterval;
        particle_settings[i].num_particles = kDefaultNumParticles;
        particle_settings[i].max_size = kDefaultSize;
        particle_settings[i].shape = Shape::Square;
        particle_settings[i].num_bands = kNumBandsPerParticle;
        particle_settings[i].starting_band = kNumBandsPerParticle * i;
    }
}

void ofApp::SetDefaultParticleGUIs() {
    for (int i = 0; i < kNumParticles; i++) {
        // destroy currently stored GUI
        particle_guis[i]->~ofxDatGui();

        // create new default GUI
        std::string gui_label = std::to_string(i + 1);
        ofxDatGui* gui = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
        gui->add2dPad("Position Particle " + gui_label);
        gui->addToggle("Toggle Particle " + gui_label, true);
        gui->addSlider("X Rotation " + gui_label, -kMaxRotationSpeed, 
			kMaxRotationSpeed, kDefaultRotationSpeed);
        gui->addSlider("Y Rotation " + gui_label, -kMaxRotationSpeed,
                       kMaxRotationSpeed, kDefaultRotationSpeed);
        gui->addSlider("Z Rotation " + gui_label, -kMaxRotationSpeed,
                       kMaxRotationSpeed, kDefaultRotationSpeed);
        gui->addColorPicker("Color 1: " + gui_label, ofColor::green);
        gui->addColorPicker("Color 2: " + gui_label, ofColor::purple);
        gui->addSlider("Color Interval: " + gui_label, kMinColorInterval, kMaxColorInterval,
			kDefaultColorInterval);
        gui->addMatrix("Number: " + gui_label, kMaxNumParticles, true);
        gui->addSlider("Radius: " + gui_label, kMinRadius, kMaxRadius, kDefaultRadius);
        gui->addDropdown("Particle Shape: " + gui_label, shapes);
        gui->addSlider("Max Size: " + gui_label, kMinDrawingSize, kMaxDrawingSize, 
			kDefaultSize);
        gui->setVisible(false);

        gui->on2dPadEvent(this, &ofApp::OnParticlePostionChanged);
        gui->onSliderEvent(this, &ofApp::OnParticleSliderChanged);
        gui->onColorPickerEvent(this, &ofApp::OnParticleColorChanged);
        gui->onDropdownEvent(this, &ofApp::OnParticleShapeChanged);
        gui->onToggleEvent(this, &ofApp::OnParticleToggled);
        gui->onMatrixEvent(this, &ofApp::OnNumParticlesChanged);

        // store GUI in vector
        particle_guis[i] = gui;
    }
}
