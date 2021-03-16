//
// Created by moebiussurfing on 8/6/19.
//

#ifndef INC_4_COLORMANAGER_DEMO_PALETTE_H
#define INC_4_COLORMANAGER_DEMO_PALETTE_H

#include "ofMain.h";

#include "ofxCameraSaveLoad.h"

class DEMO_Scene {

private:
	std::string _name = "DEMO_Cam";

public:
	~DEMO_Scene() 
	{
		ofLogNotice(__FUNCTION__);
		ofxSaveCamera(cam, path + _name);
	}
	DEMO_Scene()
	{
		ofLogNotice(__FUNCTION__);
	}

	//-

public:
	void setup();
	void update();
	void draw(float alpha = 1.0f);

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

	void setEnableMouse(bool b) {
		ofLogNotice(__FUNCTION__);
	}

	void toggleMouseCamera() {
		ofLogNotice(__FUNCTION__);
		if (!cam.getMouseInputEnabled()) cam.enableMouseInput();
		else cam.disableMouseInput();
	}

private:
	
	string path = "ofxColorManager/demo/";
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

#endif //INC_4_COLORMANAGER_DEMO_PALETTE_H
