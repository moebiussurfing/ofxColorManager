#pragma once

#include "ofMain.h"

class CurveLut {
public:
	CurveLut();
	void setup(int n = 256);
	void add(ofVec2f controlPoint);
	void set(int i, ofVec2f controlPoint);
	float getLut(int i);
	void remove(int i);
	void update();
	float operator[](int i) const;
	int size() const;
	int getStart() const;
	int getStop() const;
	bool isLutNew();
	ofVec2f getPoint(int i);
	float getPointSize();
	vector<ofVec2f> getControlPoints();

protected:
	vector<float> getSecondDerivative();
	int n;
	vector<float> lut;
	vector<ofVec2f> controlPoints;
	bool lutNew;
};