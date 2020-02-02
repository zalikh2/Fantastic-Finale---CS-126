#include "Drawer.h"


ofColor Drawer::GetColorFromPercentage(double pct, ofColor c1, ofColor c2) {
	//clamp the values to valid numbers
    if (pct < 0) {
        pct = 0;
    } else if (pct > 1) {
        pct = 1;
    }
	//take a percentage of both color 1 and 2 and return it
    double r, g, b;
    r = c1.r * pct + (1 - pct) * c2.r;
    g = c1.g * pct + (1 - pct) * c2.g;
    b = c1.b * pct + (1 - pct) * c2.b;
    return ofColor(r, g, b, 255);
}

void Drawer::DrawShape(double size, Shape shape_to_draw) {
	//draw the appropriate shape based on the enum value and size
    switch (shape_to_draw) {
        case Triangle:
            DrawTriangle(size);
            break;
        case Square:
            DrawSquare(size);
            break;
        case Star:
            DrawStar(size);
            break;
        case Circle:
            DrawCircle(size);
            break;
        default:
            break;
    }
}

void Drawer::DrawSquare(double size) {
	//draw a rectangle with side lengths both equalling the size
    ofDrawRectangle(0, 0, size, size);
}

void Drawer::DrawTriangle(double size) {
    //draw a triangle with side lengths equalling the size
    ofDrawTriangle(-1 * size / 2, 0, 0, size, size / 2, 0);
}

void Drawer::DrawCircle(double size) {
	//draw a circle with a diameter equalling size
    ofDrawCircle(0, 0, size/2);
}

void Drawer::DrawStar(double size) {
    // drawing taken from openFrameworks PolygonExample
    ofPushMatrix();
    int nTips = 5;
    int nStarPts = nTips * 2;
    float innerRadius = size / 2;
    float outerRadius = size;
    float angleChangePerPt = TWO_PI / (float)nStarPts;
    float angle = 0;

    ofBeginShape();
    for (int i = 0; i < nStarPts; i++) {
        if (i % 2 == 0) {
            // inside point:
            float x = innerRadius * cos(angle);
            float y = innerRadius * sin(angle);
            ofVertex(x, y);
        } else {
            // outside point
            float x = outerRadius * cos(angle);
            float y = outerRadius * sin(angle);
            ofVertex(x, y);
        }
        angle += angleChangePerPt;
    }
    ofEndShape();
    ofPopMatrix();
}
