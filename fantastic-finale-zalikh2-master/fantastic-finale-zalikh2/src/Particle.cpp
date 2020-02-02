#include "Particle.h"
#include "Drawer.h"

float Particle::GetSumFrequencies(ParticleSettings settings,
                                  float* frequencies) {
	//check for invalid data
    if (settings.starting_band < 0 || settings.num_bands <= 0) {
        return 0;
	}
	//ptherwise, sum up the frequency numbers given
    float sum = 0;
    for (int i = settings.starting_band;
         i < settings.starting_band + settings.num_bands; i++) {
        sum += frequencies[i];
    }
    return sum;
}

void Particle::DrawParticle(ParticleSettings settings, double size) {
	//only draw if toggled
    if (settings.is_toggled) {
		//for each particle location, draw a particle there
        for (ofPoint loc : settings.locations) {
            ofPushMatrix();
			//translate the particle and rotate as necessary
            ofTranslate(loc.x, loc.y);
            ofRotateXDeg(ofGetElapsedTimef() * settings.x_rotation_speed);
            ofRotateYDeg(ofGetElapsedTimef() * settings.y_rotation_speed);
            ofRotateZDeg(ofGetElapsedTimef() * settings.z_rotation_speed);
			//calculate which color should be drawn
            double interval = fmod(ofGetElapsedTimef(), settings.color_interval);
            double pct = interval / settings.color_interval;
            if (pct == 0) {
                ofColor temp = settings.color_one;
                settings.color_one = settings.color_two;
                settings.color_two = temp;
            }
            ofColor color = Drawer::GetColorFromPercentage(
                pct, settings.color_one, settings.color_two);
            ofSetColor(color);
			//draw the actual shape
            Drawer::DrawShape(size, settings.shape);
            ofPopMatrix();
		} 
    }
}

std::vector<ofPoint> Particle::GeneratePointsAroundLocation(ofPoint center,
                                                            int num_points,
                                                            double radius) {
	//check for invalid data
    if (num_points <= 0 || radius < 0) {
        return std::vector<ofPoint>();
    } else if (num_points == 1) {
		//if there's only one point, draw it at the center
        std::vector<ofPoint> center_point = {center};
        return center_point;
    }
    // generate points around the center using trig
    std::vector<ofPoint> points = {};
    for (int i = 0; i < num_points; i++) {
        double angle_degrees = i * (360.0 / num_points);
        double angle_radians = angle_degrees * PI / 180.0;
        double x = center.x + radius * sin(angle_radians);
        double y = center.y + radius * cos(angle_radians);
        points.push_back(ofPoint(x, y));
    }
    return points;
}
