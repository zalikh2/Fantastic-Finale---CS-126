#pragma once

#include "Shape.h"
#include "ofMain.h"

class Drawer {
   public:
    static void DrawShape(double size, Shape shape_to_draw);

	// This returns a color that is a weighted average of two different colors.
    // c1 and c2 are the two colors to choose from and pct represents the
    // percentage
    //     of c1 to use in the color.
    static ofColor GetColorFromPercentage(double pct, ofColor c1, ofColor c2);

    

	private:
	//These methods all contain implementations of drawing a specific shape.
    static void DrawSquare(double size);
    static void DrawTriangle(double size);
    static void DrawCircle(double size);
    static void DrawStar(double size);




};
