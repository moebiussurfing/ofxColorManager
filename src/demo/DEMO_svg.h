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
	void mouseScrolled(ofMouseEventArgs &eventArgs);

public:
	std::string path_Name = "_DEMO_Svg";
	ofxInteractiveRect rectDgSvg = { "_DEMO_Svg" };
	std::string path_Layout;

	ofParameter<bool> DEMO2_Test{ "Enable DEMO2", false };
	ofParameter<bool> DEMO2_Edit{ "Edit DEMO2", false };
	ofParameter<float> DEMO2_Scale{ "Scale", 0.2, 0, 1.0f };
	ofParameter<float> DEMO2_Alpha{ "Alpha", 1.0f, 0, 1.0f };

	ofParameterGroup params{ "DEMO_Svg" };
	void Changed_Controls(ofAbstractParameter &e);

	void setEdit(bool b) 
	{
		if (b) rectDgSvg.enableEdit();
		else rectDgSvg.disableEdit();
	}
	
private:
	float scale;
	float alpha;
	float ratio;
	ofRectangle rSvgBounds;
	
public:
	void setAlpha(float f) 
	{
		alpha = f;
	}
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
		ofRemoveListener(params.parameterChangedE(), this, &DEMO_Svg::Changed_Controls);

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
	glm::vec2 shape;

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