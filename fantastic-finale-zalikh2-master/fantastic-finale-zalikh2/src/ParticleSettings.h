#pragma once
#include "ofMain.h"
#include "Shape.h"

#include <vector>

struct ParticleSettings {
    ofPoint center;
	std::vector<ofPoint> locations;
	double radius;
    bool is_toggled;
    double x_rotation_speed;
    double y_rotation_speed;
    double z_rotation_speed;
    ofColor color_one;
    ofColor color_two;
    double color_interval;
    double max_size;
    int num_particles;
    Shape shape;
    int num_bands;
    int starting_band;
};