#pragma once
#include "ofMain.h"

/*

*/

#include "ofxSvgLoader.h"
#include "ofxPSBlend.h"

class DEMO_svg
{

public:
	DEMO_svg()
	{
		setup();
	};
	~DEMO_svg() {};

	void setup();
	void update();

	void draw();
	void draw(glm::vec2 _pos);
	//void draw(glm::vec2 _pos, glm::vec2 _shape);

	void keyPressed(int key);

	float getWidth() {
		return shape.x;
	}
	float getHeight() {
		return shape.y;
	}
	void setPaletteColors(vector<ofColor> &palette);

private:
	ofxSvgLoader svg;
	ofImage img;

	ofxPSBlend psBlend;

	int blendMode;

	std::string path;
	vector<ofColor> paletteSvg;
	int maxNumColors = 7;//colors of the used moebius svg file

	//void initializeColors();

	glm::vec2 pos{ 0,0 };
	glm::vec2 shape{ 838, 1080 };
};