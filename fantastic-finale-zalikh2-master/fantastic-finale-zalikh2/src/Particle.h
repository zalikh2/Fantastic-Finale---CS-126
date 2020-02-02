#include "ParticleSettings.h"

#include <vector>

class Particle {
   public:
	//This method takes in an array of frequency values, and will get the sum of the 
	//values based on the ParticleSettings passed to it. It will hold both the starting band
	//and number of freqeuncy bands to sum up.
    static float GetSumFrequencies(ParticleSettings settings, float* frequencies);

	//This method will draw a Particle on the screen given a ParticleSettings struct
	//with data such as locations, size, Shape, rotations, and more.
	static void DrawParticle(ParticleSettings settings, double size);

	//This method will generate a given number of points a given distance away from
	//a given center point.
	static std::vector<ofPoint> GeneratePointsAroundLocation(ofPoint center,
                                                                 int num_points,
                                                                 double radius);
	
};