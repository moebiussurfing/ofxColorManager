#include "ButtonExample.h"

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
    setPosition(x, y);
    setSize(w, h);
    color_picked = ofColor(ofColor::red);
    color_over = ofColor(0);//default
    color_down = ofColor(ofColor::green);
    bTouched = false;

    ofAddListener(eventTouchDown, this, &ButtonExample::onTouchDown);
    ofAddListener(eventTouchUp, this, &ButtonExample::onTouchUp);
    ofAddListener(eventTouchMove, this, &ButtonExample::onTouchMove);
}

void ButtonExample::update()
{
}

void ButtonExample::draw()
{
    // filled color box
    ofSetColor(color_picked);
    ofFill();
    ofDrawRectangle(0, 0, getWidth(), getHeight());

//	// draw the touch anchor
//	if (bTouched) {
//		ofSetColor(color_over);
//		ofFill();
//		ofDrawEllipse(touchAnchor, 10, 10);
//	}

    // draw border
    if (bIsSelected)
    {
        ofNoFill();
        ofSetColor(ofColor(ofColor::black, 128));//with alpha
        ofSetLineWidth(1.);
        ofDrawRectangle(1., 1., getWidth()-2., getHeight()-2.);
    }
}

void ButtonExample::setSelectable(bool b)
{
    bIsSelectable = b;
}

void ButtonExample::setSelected(bool b)
{
    bIsSelected = b;
}

void ButtonExample::setLocked(bool b){
    bLocked = b;
}

// pointer back
void ButtonExample::setup_colorBACK(ofFloatColor &c)
{
    colorBACK = &c;
}

void ButtonExample::onTouchDown(ofxInterface::TouchEvent &event)
{
    ofVec2f local = toLocal(event.position);
    color = color_down;
    bTouched = true;
    touchAnchor = local;

    colorBACK->set(ofFloatColor(color_picked));

    if (bIsSelectable)
    {
        if (!bIsSelected)
            bIsSelected = true;
//        else
//            bIsSelected = false;
    }
}

void ButtonExample::onTouchUp(ofxInterface::TouchEvent &event)
{
    ofVec2f local = toLocal(event.position);
    color = color_picked;
    bTouched = false;

//    if (bIsSelectable && bIsSelected)
//    {
//        bIsSelected = false;
//    }
}

void ButtonExample::onTouchMove(ofxInterface::TouchEvent &event)
{
    if (!bLocked)
    {
        ofVec2f parentPos = ((Node*)parent)->toLocal(event.position);
        setPosition(parentPos - touchAnchor);
    }
}