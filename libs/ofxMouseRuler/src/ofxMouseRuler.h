#pragma once
#include "ofMain.h"

class ofxMouseRuler {

public:
	ofxMouseRuler();
	~ofxMouseRuler();

	void setup();
	void draw(ofEventArgs&);

	void hide();
	void show();
	void toggleVisibility();

	bool mousePressed(ofMouseEventArgs& args);
	bool mouseReleased(ofMouseEventArgs& args);
	void mouseMoved(ofMouseEventArgs&){}
	void mouseDragged(ofMouseEventArgs&){}
	void mouseScrolled(ofMouseEventArgs&){}
	void mouseEntered(ofMouseEventArgs&){}
	void mouseExited(ofMouseEventArgs&){}

	void setOrig(int x, int y);

    void setAxisVisible(bool b)
    {
        bShowAvis = b;
    }

private:

	bool setupDone;
	bool visible;
	ofPoint dragStart;
	bool dragging;
	ofBitmapFont font;

	int orig_x, orig_y;
	int pad_x, pad_y;

    bool bShowAvis = true;

};

