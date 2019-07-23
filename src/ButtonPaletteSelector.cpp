#include "ButtonPaletteSelector.h"

namespace ofxInterface
{
    void ButtonPaletteSelector::setThisPaletteType(int palette)
    {
        thisPaletteType = palette;

        ofLogNotice("ButtonPaletteSelector") << "thisPaletteType:" << thisPaletteType;
    }

    void ButtonPaletteSelector::set_SELECTED_palette(int &palette)
    {
        SELECTED_palette = &palette;

        ofLogNotice("ButtonPaletteSelector") << "SELECTED_palette:" << SELECTED_palette;
    }

    void ButtonPaletteSelector::setup(const string &_label)
    {
        label = _label;
        setSize(label.length()*8 + 10, 20);
        setName(label);

        bDrawBorder = true;
        bDrawBackground = true;

        borderColor = ofColor(0);
        bgColor = ofColor(255);
        labelColor = ofColor(0);

        ofAddListener(eventTouchDown, this, &ButtonPaletteSelector::onTouchDown);
        ofAddListener(eventTouchUp, this, &ButtonPaletteSelector::onTouchUp);
        ofAddListener(eventTouchMove, this, &ButtonPaletteSelector::onTouchMove);
    }

    void ButtonPaletteSelector::draw()
    {
        if (bDrawBackground) {
            ofSetColor(bgColor * (isTouched()?0.5:1));
            ofFill();
            ofDrawRectangle(0, 0, getWidth(), getHeight());
        }

        ofSetColor(labelColor);
        ofDrawBitmapString(label, 5, getHeight()-5);

        if (bDrawBorder) {
            ofSetColor(borderColor);
            ofNoFill();
            ofSetLineWidth(1);
            ofDrawRectangle(0, 0, getWidth(), getHeight());
        }
    }

    void ButtonPaletteSelector::onTouchDown(ofxInterface::TouchEvent &event)
    {
        ofVec2f local = toLocal(event.position);

        SELECTED_palette = &thisPaletteType;
        ofLogNotice("ButtonPaletteSelector") << "onTouchDown: " << thisPaletteType;
    }

    void ButtonPaletteSelector::onTouchUp(ofxInterface::TouchEvent &event)
    {
        ofVec2f local = toLocal(event.position);
//        ofLogNotice("ButtonPaletteSelector") << "onTouchUp: " << thisPaletteType;
    }

    void ButtonPaletteSelector::onTouchMove(ofxInterface::TouchEvent &event)
    {

    }

} 	// namespace
