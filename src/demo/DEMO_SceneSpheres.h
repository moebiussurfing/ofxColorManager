//
// Created by moebiussurfing.

// scene coded by junkiyoshi.com  
// https://junkiyoshi.com/openframeworks20210316/

#pragma once

#include "ofMain.h";

#include "ofxCameraSaveLoad.h"

class DEMO_SceneSpheres {

private:
	std::string _name = "DEMO_SceneSpheres_Cam";

public:
	~DEMO_SceneSpheres() 
	{
		ofLogNotice(__FUNCTION__);
		ofxSaveCamera(cam, path + _name);
	}
	DEMO_SceneSpheres()
	{
		ofLogNotice(__FUNCTION__);
	}

	ofParameter<float> DEMO5_Speed{ "Speed", 0.5, 0.001 , 1 };
	ofParameter<float> DEMO5_Zoom{ "Zoom", 0.5, 0, 1 };
	ofParameter<float> DEMO5_Alpha{ "Alpha Spheres", 0.8, 0, 1 };
	ofParameter<bool> DEMO5_Cam{ "Edit Camera Spheres", false };

	//-

public:
	void setup();
	void update();
	void draw();
	//void draw(float _alpha = 1.0f);


	void setPalette(vector<ofColor> &_palette)
	{
		ofLogNotice(__FUNCTION__);
		palette = &_palette;
	}

	void resetCamera() {
		cam.reset();
		cam.setDistance(750);
	}

	void setEnableMouseCamera(bool b) {
		ofLogNotice(__FUNCTION__);
		if (b) {
			cam.enableMouseInput();
			cam.enableMouseMiddleButton();
			//cam.enableInertia();
			//cam.setenableInertia();
		}
		else {
			cam.disableMouseInput();
		}
	}

	//void setEnableMouse(bool b) {
	//	ofLogNotice(__FUNCTION__);
	//}

	void toggleMouseCamera() {
		ofLogNotice(__FUNCTION__);
		if (!cam.getMouseInputEnabled()) cam.enableMouseInput();
		else cam.disableMouseInput();
	}

	void start() {
		tweenD = 1;
	};

private:

	float alpha;
	
	string path = "ofxColorManager/demo/";
	ofEasyCam cam;
	vector<ofColor> *palette;

	ofMesh face, frame;

	float tweenD = 1;

};
