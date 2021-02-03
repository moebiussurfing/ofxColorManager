#pragma once
#include "ofMain.h"

/*

*/

#include "ofxSvgLoader.h"
#include "ofxPSBlend.h"
#include "ofxInteractiveRect.h" 

class DEMO_Svg
{

public:
	std::string path_Name = "_DEMO_Svg";
	ofxInteractiveRect rectDgSvg = { "_DEMO_Svg" };
	std::string path_Layout;
	
	void setEdit(bool b) 
	{
		if (b) rectDgSvg.enableEdit();
		else rectDgSvg.disableEdit();
	}
	
	float scale;
	float ratio;
	ofRectangle rSvgBounds;
	
	void setScale(float f) 
	{
		scale = f;

		rectDgSvg.setWidth(rSvgBounds.getWidth() * scale);
		rectDgSvg.setHeight(rSvgBounds.getHeight() * scale);
	}
	
	//--

public:
	DEMO_Svg()
	{
		setup();
	};

	~DEMO_Svg()
	{
		rectDgSvg.saveSettings(path_Name, path_Layout, false);
	};
	
	//--

	void setup();
	void update();

	void draw();//use draggable rectangle
	void draw(glm::vec2 _pos);//force pos

	void keyPressed(int key);

	//--

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

	//--------------------------------------------------------------
	void refresh_Gui_Layout()
	{
		int _pad1 = 20;
		int _pad2 = 3;
		int _xx, _yy, _ww, _hh;
		_xx = rectDgSvg.getX() + _pad1;
		_yy = rectDgSvg.getY() + _pad1;

		rectDgSvg.setWidth(_ww);
		rectDgSvg.setHeight(_hh);
	}
};