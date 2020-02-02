#include "../src/ofApp.h"
#include "ofMain.h"
#include <vector>

#include "catch.hpp"

TEST_CASE("Calculating interpolated colors correctly") { 
	SECTION("A valid percentage between two colors gets the right color") {
        ofColor skewed_color =
            Drawer::GetColorFromPercentage(0.7, ofColor::red, ofColor::green);
            REQUIRE(skewed_color == ofColor(178, 76, 0));
	}
	SECTION("An invalid percentage will be made valid") {
            ofColor gray = Drawer::GetColorFromPercentage(-1, ofColor::white,
                                                         ofColor::black);
            REQUIRE(gray == ofColor::black);
	}
}

TEST_CASE("Getting Shapes from a string") {
    SECTION("Getting the correct shape from a valid string") {
        REQUIRE(ofApp::GetShapeFromName("Triangle") == Shape::Triangle);
	}
    SECTION("An invalid string returns no shape") {
        REQUIRE(ofApp::GetShapeFromName("Circle ") == Shape::None);
	}
}

TEST_CASE("Getting GUI indexes from a label") {
    SECTION("Passing in a valid label gives the proper index") {
        REQUIRE(ofApp::GetGUIIndexFromLabel("Label 3") == 2);
	}
	SECTION("Passing in an invalid label gives an invalid index") {
        REQUIRE(ofApp::GetGUIIndexFromLabel("GUI") == -1);
	}
}

TEST_CASE("Getting the sums of the audio freqeuncy data"){
    ParticleSettings settings;
	SECTION("Settings with a negative number of bands to sum up will give a sum of 0") {
        settings.starting_band = 0;
        settings.num_bands = -2;
        float array[] = {1.0, 2.0};
        float sum = Particle::GetSumFrequencies(settings, array);
        REQUIRE(sum == 0.0);
	}
	SECTION("Settings with a negative starting band will give a sum of 0") {
        settings.starting_band = -2;
        settings.num_bands = 2;
        float array[] = {1.0, 2.0};
        float sum = Particle::GetSumFrequencies(settings, array);
        REQUIRE(sum == 0.0);
	}
	SECTION("Proper settings will lead to a proper sum") {
        settings.starting_band = 0;
        settings.num_bands = 2;
        float array[] = {1.0, 2.0};
        float sum = Particle::GetSumFrequencies(settings, array);
        REQUIRE(sum == 3.0);
	}
}

TEST_CASE("Generating locations based on a center point and radius") {
    std::vector<ofPoint> locations = {};
    SECTION("A 0 number of particles would return 0 locations") {
        locations = Particle::GeneratePointsAroundLocation(ofPoint(0, 0), 0, 100);
        REQUIRE(locations.empty());
	}
	SECTION("A negative radius would not return back any locations") {
        locations = Particle::GeneratePointsAroundLocation(ofPoint(0, 0), 2, -100);
        REQUIRE(locations.empty());
	}
	SECTION("A radius of 0 would return the same points") {
        locations = Particle::GeneratePointsAroundLocation(ofPoint(100, 250), 3, 0);
        for (ofPoint point : locations) {
            REQUIRE(point == ofPoint(100, 250));
		}
	}
	SECTION("Properly generate points around a given center and radius") {
            locations =
                Particle::GeneratePointsAroundLocation(ofPoint(100, 250), 4, 100);
            std::vector<ofPoint> expected_points = {
                ofPoint(100, 350),
                ofPoint(200, 250),
                ofPoint(100, 150),
                ofPoint(0, 250)
            };
            REQUIRE(locations == expected_points);
	}
}