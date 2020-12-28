//
// Created by moebiussurfing on 8/6/19.
//

#ifndef INC_4_COLORMANAGER_DEMO_PALETTE_H
#define INC_4_COLORMANAGER_DEMO_PALETTE_H

#include "ofMain.h";
#include "ofxCameraSaveLoad.h"

class DEMO_palette {

private:
	std::string _name = "DEMO_Cam";
public:
	~DEMO_palette() 
	{
		ofxSaveCamera(cam, path + _name);
	}
	DEMO_palette()
	{
		ofxLoadCamera(cam, path + _name);
	}

	//-

public:
	void setup();
	void update();
	void draw();

	void clear() {
		//bDEMO1_clear = true;
		//pauseCreate = false;
		locations.clear();
		velocities.clear();
		colors.clear();
	}

	void reStart() {
		//bDEMO1_clear = true;
		clear();
		pauseCreate = false;
	}

	void start() {
		pauseCreate = false;
	}

	void setClear(bool b) {
		bDEMO1_clear = true;
	}

	void setPalette(vector<ofColor> &_palette)
	{
		palette = &_palette;
	}

	void setEnableMouseCamera(bool b) {
		if (b) cam.enableMouseInput();
		else cam.disableMouseInput();
	}

	void toggleMouseCamera() {
		if (!cam.getMouseInputEnabled())
			cam.enableMouseInput();
		else
			cam.disableMouseInput();
	}

private:

	string path = "settings/";
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
