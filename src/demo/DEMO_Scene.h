//
// Created by moebiussurfing on 8/6/19.
//
#pragma once

#include "ofMain.h";

#include "ofxCameraSaveLoad.h"

class DEMO_Scene {

private:
	std::string _name = "DEMO_Bubbles_Cam";
	
public:
	~DEMO_Scene()
	{
		ofLogNotice(__FUNCTION__);
		ofxSaveCamera(cam, path + _name);
	}
	DEMO_Scene()
	{
		ofLogNotice(__FUNCTION__);
		//fix init
		setEnableMouseCamera(true);
		resetCamera();
	}

	//-

public:
	void setup();
	void update();
	void draw(ofRectangle viewport, float alpha = 1.0f);

	void clear() {
		ofLogNotice(__FUNCTION__);
		//bDEMO1_clear = true;
		//pauseCreate = false;
		locations.clear();
		velocities.clear();
		colors.clear();
	}

	void reStart() {
		ofLogVerbose(__FUNCTION__);
		//bDEMO1_clear = true;
		clear();
		pauseCreate = false;
	}

	void start() {
		ofLogVerbose(__FUNCTION__);
		pauseCreate = false;
	}

	void setClear(bool b) {
		ofLogNotice(__FUNCTION__);
		bDEMO1_clear = true;
	}

	void setPalette(vector<ofColor> &_palette)
	{
		ofLogNotice(__FUNCTION__);
		palette = &_palette;
	}

	void resetCamera() {
		cam.reset();
		cam.setupPerspective();
		//float d = cam.getDistance();
		//cam.setDistance( d * 0.8);
		cam.setDistance(500);
	}

	void setEnableMouseCamera(bool b) {
		ofLogNotice(__FUNCTION__);
		if (b) {
			cam.enableMouseInput();
			//cam.enableMouseMiddleButton();
			////cam.enableInertia();
			////cam.setenableInertia();
		}
		else {
			cam.disableMouseInput();
		}
	}

	void setEnableMouse(bool b) {
		ofLogNotice(__FUNCTION__);
	}

	void toggleMouseCamera() {
		ofLogNotice(__FUNCTION__);
		if (!cam.getMouseInputEnabled()) cam.enableMouseInput();
		else cam.disableMouseInput();
	}

public:
	void save() {
		ofLogNotice(__FUNCTION__);
		ofxSaveCamera(cam, path + _name);
	};
	void load() {
		ofLogNotice(__FUNCTION__);
		ofxLoadCamera(cam, path + _name);
	};

private:

	string path = "ofxColorManager/demo/";//TODO: hardcoded..
	ofEasyCam cam;
	vector<ofColor> *palette;

	// DEMO 1
	bool ENABLE_DEMO1 = true;
	vector<glm::vec2> locations;
	vector<glm::vec2> velocities;
	vector<ofColor> colors;
	bool bDEMO1_clear = false;
	bool bDEMO1_stop = false;
	int iColor = 0;
	bool pauseCreate = false;

	// DEMO 2
	bool ENABLE_DEMO2 = true;
};
