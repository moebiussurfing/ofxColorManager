//
// Created by moebiussurfing on 8/6/19.
//

#include "DEMO_Scene.h"

//TODO
// should check if pointer and palettes size is not null


//--------------------------------------------------------------
void DEMO_Scene::setup() {
	ofLogNotice(__FUNCTION__);

	// DEMO 1

	//ofSetCircleResolution(75);

	//-

	// DEMO 2

	ofxLoadCamera(cam, path + _name);
	//cam.reset();

	//cam.disableMouseInput();
	//cam.enableOrtho();
}

//--------------------------------------------------------------
void DEMO_Scene::update() {

	// DEMO 1 - CIRCLES

	if (bDEMO1_clear)
	{
		bDEMO1_clear = false;
		locations.clear();
		velocities.clear();
		colors.clear();
	}

	ofColor color;

	// 1. create circle every x frames
	int frameCurrent = ofGetFrameNum() % 5;//speed retrig
	if (frameCurrent == 0 && !pauseCreate) {

		// // 2. create circle with randomly frequency
		// if ((ofRandom(100) < 50) && !pauseCreate) {

		glm::vec2 startPos;
		// glm::vec2 mousePos = glm::vec2(ofGetMouseX(), ofGetMouseY()) - glm::vec2(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
		// startPos = mousePos;
		startPos = glm::vec2(0, 0);

		locations.push_back(startPos);

		// randomize each circle/color speed
		velocities.push_back(glm::normalize(glm::vec2(ofRandom(-1, 1), ofRandom(-1, 1))) * 2);

		if (iColor == palette->size() - 1)
			pauseCreate = true;

		// // 1. gradient: get random color for all gradient
		// float RandomNorm = ofRandom(0., 1.);
		// color.set(getColorAtPercent(RandomNorm));

		// 2. palette color: get one of the palette colors
		// color.set(palette[iColor]);//TODO

		//avoid crash when palette pointer not linked
		if ((palette != nullptr) && 
			((*palette).size() > 0) && 
			(iColor < (*palette).size())) 
{
			color.set((*palette)[iColor]);
		}
		else {
			color.set(ofColor(ofRandom(255)));
		}

		iColor++;
		int palSize = palette->size();
		if (palSize > 0) iColor = iColor % palSize;

		if (palette != nullptr) {
			this->colors.push_back(color);
		}
		else {
			this->colors.push_back(ofColor(ofRandom(255)));
		}

		// sort each circle/color speed
		// this->velocities.push_back(glm::normalize( glm::vec2(palSize/(float)iColor * ofRandom(-1, 1), ofRandom(-1, 1)) ));
	}

	//update circles speeds
	for (int i = this->locations.size() - 1; i > -1; i--) {

		this->locations[i] += this->velocities[i];

		// distance to erase circles outside screen
		int maxCirclesDist = 1440;
		if (glm::length(this->locations[i]) > maxCirclesDist) {

			this->locations.erase(this->locations.begin() + i);
			this->velocities.erase(this->velocities.begin() + i);
			this->colors.erase(this->colors.begin() + i);
		}
	}
}

//--------------------------------------------------------------
void DEMO_Scene::draw(ofRectangle viewport, float alpha)
{
	//TODO: workaround to avoid crash
if (viewport == ofRectangle(0, 0, 0, 0)) {
	viewport = ofRectangle(100, 100, 100, 100);
}

	{
		// DEMO 1 - CIRCLES

		if (ENABLE_DEMO1 && colors.size() != 0) 
		{
			ofPushMatrix();
			ofPushStyle();
			ofTranslate(viewport.getCenter());
			//ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

			for (int i = 0; i < locations.size(); i++) {
				ofSetColor(colors[i], 255 * alpha);
				float radius = 1.5 * (2 * PI * glm::length(this->locations[i])) / 360 * 20;
				ofDrawCircle(this->locations[i], radius);
			}

			ofPopStyle();
			ofPopMatrix();
		}

		//--

		// DEMO 2 - ROTATING RECTANGLES

		if (ENABLE_DEMO2 && (*palette).size() != 0)
		{
			cam.begin(viewport);

			ofPushMatrix();
			ofPushStyle();
			{
				float scaleRects = 2.f;
				float radius = 300;
				int iDeg = 36;

				//int iDeg = 360/ (2*palette.size());
				int iCol = 0;

				for (int deg = 0; deg < 360; deg += iDeg)
				{
					float x = radius * cos(deg * DEG_TO_RAD);
					float y = radius * sin(deg * DEG_TO_RAD);
					ofColor c;

					//-

		//            // 1. color from gradient
		//            float cnt = ofMap(deg, 0, 360, 0.f, 1.f);
		//            c = getColorAtPercent(cnt);//gradient not in the class...
		//			  // 2. color from palette//TODO
		//            c.set((palette[iCol]);

					if (palette != nullptr && palette->size() > 0)
					{
						c.set((*palette)[iCol]);

						iCol++;
						if (palette->size() > 0)
							iCol = iCol % palette->size();
					}

					else
					{
						c.set(ofColor(ofRandom(255)));
						iCol++;
					}

					//--

					ofSetColor(c, 255 * alpha);

					ofRotateZ(ofGetFrameNum() * 0.01);
					//ofRotateZ(ofGetFrameNum() * 0.25);

					ofPushMatrix();
					ofTranslate(ofVec3f(x, y, 0));
					ofRotateX(90);
					ofRotateY(deg + 90);

					ofRect(ofVec3f(0, 0, 0), scaleRects * 1000, scaleRects * 1000);

					ofPopMatrix();
				}
			}
			ofPopStyle();
			ofPopMatrix();

			cam.end();
		}
	}
}
