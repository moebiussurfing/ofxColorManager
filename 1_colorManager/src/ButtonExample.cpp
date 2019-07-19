//
//  ButtonExample.cpp
//  example-basic
//
//  Created by Gal Sasson on 2/4/15.
//
//

#include "ButtonExample.h"

void ButtonExample::setup_colorBACK(ofFloatColor &c)
{
    colorBACK = &c;
}

void ButtonExample::setColor(ofColor c)
{
    color_picked = c;
}

ofColor ButtonExample::getColor()
{
    return color_picked;
}

void ButtonExample::setup(float x, float y, float w, float h)
{
	/*****
	 * you get these from ofxInterface::Node
	 */
	setPosition(x, y);
	setSize(w, h);

    color_picked = ofColor(ofColor::red);
    color_over = ofColor(100);//default
    color_down = ofColor(ofColor::green);

	bTouched = false;

	/*****
	 * register for touch events
	 *
	 * TouchManager will notify these automatically
	 */
	ofAddListener(eventTouchDown, this, &ButtonExample::onTouchDown);
	ofAddListener(eventTouchUp, this, &ButtonExample::onTouchUp);
	ofAddListener(eventTouchMove, this, &ButtonExample::onTouchMove);
}

void ButtonExample::update()
{
}

void ButtonExample::draw()
{
	/*****
	 * always draw in local space
	 */

	// draw the frame
//	ofSetColor(ofColor::black);
//    ofNoFill();

    ofSetColor(color_picked);
    ofFill();

    ofDrawRectangle(0, 0, getWidth(), getHeight());

	// draw the touch anchor
	if (bTouched) {
		ofSetColor(color_over);
		ofFill();
		ofDrawEllipse(touchAnchor, 10, 10);
	}
}

void ButtonExample::setLocked(bool b){
    bLocked = b;
}

void ButtonExample::onTouchDown(ofxInterface::TouchEvent &event)
{
	/*****
	 * event.position is always in global space, use toLocal
	 */

	ofVec2f local = toLocal(event.position);

	color = color_down;
	bTouched = true;

	touchAnchor = local;

    colorBACK->set(ofFloatColor(color_picked));
}

void ButtonExample::onTouchUp(ofxInterface::TouchEvent &event)
{
	ofVec2f local = toLocal(event.position);

	color = color_picked;
	bTouched = false;
}

void ButtonExample::onTouchMove(ofxInterface::TouchEvent &event)
{
    if (!bLocked)
    {
        ofVec2f parentPos = ((Node*)parent)->toLocal(event.position);
        setPosition(parentPos - touchAnchor);
    }
}