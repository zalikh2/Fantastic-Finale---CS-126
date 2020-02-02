#pragma once

#include <vector>

#include "Drawer.h"
#include "Particle.h"
#include "ofxBeat.h"
#include "ofxDatGui.h"

class ofApp : public ofBaseApp {
   private:
    // constants for the class methods to use
    const std::vector<std::string> shapes = {"Triangle", "Square", "Circle",
                                             "Star"};
    static const int kASCIIDigitOffset = 48;
    static const int kInvalidIndex = -1;
    const int kNumAudioBands = 64;
    const int kNumParticles = 4;
    const int kNumBandsPerParticle = 2;

	//constants for the particle GUIs
	const int kDefaultRotationSpeed = 0;
    const int kMaxRotationSpeed = 300;
    const int kMinRadius = 0;
    const int kDefaultRadius = 100;
    const int kMaxRadius = 1000;
    const int kMinDrawingSize = 10;
    const int kDefaultSize = 100;
    const int kMaxDrawingSize = 1000;
    const int kDefaultNumParticles = 1;
    const int kMaxNumParticles = 20;
    const double kMinColorInterval = 0.1;
    const double kDefaultColorInterval = 1.0;
    const double kMaxColorInterval = 10.0;

	// constants for the main settings GUI
    const double kMinSmoothingFactor = 0;
    const double kDefaultSmoothingFactor = 0.96;
    const double kMaxSmoothingFactor = 0.99;

    std::vector<ofxDatGui*> particle_guis;
    ofxDatGui* main_gui;

    ofColor bg_color_1;
    ofColor bg_color_2;

	std::vector<ofSoundPlayer> songs;
    ofSoundPlayer current_song;
    std::array<double, 64> fftSmooth;
    ofxBeat beat;
    double smoothing_factor;
    double beat_magnitude;
    double max_magnitude;
    bool use_scaling;

    std::vector<ParticleSettings> particle_settings;
    std::vector<double> frequency_sums;
    std::vector<double> previous_sums;
    std::vector<double> max_sums;

   public:

    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
	//This method handles what happens when you use the 2D pad to change the position of the particle.
    void OnParticlePostionChanged(ofxDatGui2dPadEvent event);

	//This method handles what happens when you change one of the sliders for rotation, color
	//interval, or radius.
    void OnParticleSliderChanged(ofxDatGuiSliderEvent event);

	//This method handles what happens when you change one of the particle's colors.
    void OnParticleColorChanged(ofxDatGuiColorPickerEvent event);\

	//This method handles what happens when you change the shape of a particle.
    void OnParticleShapeChanged(ofxDatGuiDropdownEvent event);

	//This method handles what happens when you toggle a particle on and off.
    void OnParticleToggled(ofxDatGuiToggleEvent event);

	//This method handles what happens when you change the number of particles to be drawn.
    void OnNumParticlesChanged(ofxDatGuiMatrixEvent event);

	//This method handles what happens when you change one of the background colors.
    void OnBackgroundColorChanged(ofxDatGuiColorPickerEvent event);

	//This method handles what happens when you change the smoothing factor.
    void OnSmoothingSliderChanged(ofxDatGuiSliderEvent event);

	//This method handles what happens when you reset the particle settings and GUIS.
    void OnResetButtonClicked(ofxDatGuiButtonEvent event);

	//This method handles what happens when you choose a new song.
    void OnSongChanged(ofxDatGuiDropdownEvent event);

	//This method handles what happens when you turn scaling on/off.
    void OnScalingToggled(ofxDatGuiToggleEvent event);

    // This method sets one of the particle GUIs visible and all of the
    // others invisible.
    static void SetGUIVisible(int gui_index, std::vector<ofxDatGui*> guis);

    // Gets the Shape to draw based on the name of the Shape.
    static Shape GetShapeFromName(std::string shape_name);

    // gets the index of the particle GUI from the labels of the GUI elements.
    static int GetGUIIndexFromLabel(std::string label);

	//reset all of the ParticleSettings that we have stored to their default states.
    void SetDefaultParticleSettings();

	//reset all of the particle GUIs to their default states.
    void SetDefaultParticleGUIs();
};
