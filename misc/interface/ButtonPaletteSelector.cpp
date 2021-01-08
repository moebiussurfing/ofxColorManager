#include "ButtonPaletteSelector.h"

namespace ofxInterface
{
    void ButtonPaletteSelector::setup(const string &_label)
    {
        label = _label;
        setSize(label.length()*8 + 10, 20);
        setName(label);

        bDrawBorder = true;
        bDrawBackground = true;

        borderColor = ofColor(ofColor::white, 255);
        bgColor = ofColor(255);
        labelColor = ofColor(0);

        ofAddListener(eventTouchDown, this, &ButtonPaletteSelector::onTouchDown);
        ofAddListener(eventTouchUp, this, &ButtonPaletteSelector::onTouchUp);
        ofAddListener(eventTouchMove, this, &ButtonPaletteSelector::onTouchMove);

        loadFont("assets/fonts/PragmataProR_0822.ttf");
        setFontSize(14);
    }

    void ButtonPaletteSelector::draw()
    {
        if (bDrawBackground) {
            ofSetColor(bgColor * (isTouched()?0.5:1));
            ofFill();
            ofDrawRectangle(0, 0, getWidth(), getHeight());
        }

        //-

        ofSetColor(labelColor);
        if(font.isLoaded())
        {
            font.draw(label,fontSize, 5, getHeight()-5);
        }
        else
        {
            ofDrawBitmapString(label, 5, getHeight()-5);
        }

        //-

        if (bDrawBorder) {
            ofSetColor(borderColor);
            ofNoFill();
            ofSetLineWidth(1);
            ofDrawRectangle(0, 0, getWidth(), getHeight());
        }
    }

    // pointer back link the outside (ofApp) variable
    void ButtonPaletteSelector::set_SELECTED_palette(int &palette)
    {
        SELECTED_palette_pointer = &palette;

        ofLogNotice("ButtonPaletteSelector") << "set_SELECTED_palette:" << palette;
    }

    void ButtonPaletteSelector::onTouchDown(ofxInterface::TouchEvent &event)
    {
        if (!isLocked)
        {
        ofVec2f local = toLocal(event.position);
        ofLogNotice("ButtonPaletteSelector") << "onTouchDown: thisPaletteType: " << thisPaletteType;

//        & : reference operator : gives the memory address of variable
//        * : deference operator : gives the value of the variable (pointed to by the pointer). Dereferences a reference.
//        -> : used to access a vaiable within a pointer (equivilant to (*p).x)

//        if (SELECTED_palette_pointer != nullptr)
//        {
            // pointer back link the outside (ofApp) variable
            (*SELECTED_palette_pointer) = thisPaletteType;
//        }
        }
    }

    void ButtonPaletteSelector::onTouchUp(ofxInterface::TouchEvent &event) {
        if (!isLocked) {
            ofVec2f local = toLocal(event.position);
//        ofLogNotice("ButtonPaletteSelector") << "onTouchUp: ";
        }
    }

    void ButtonPaletteSelector::onTouchMove(ofxInterface::TouchEvent &event)
    {
    }

    void ButtonPaletteSelector::setThisPaletteType(int palette)
    {
        thisPaletteType = palette;
        ofLogVerbose("ButtonPaletteSelector") << "thisPaletteType:" << thisPaletteType;
    }

//-

    void ButtonPaletteSelector::setInset(float x, float y){
        _x = x;
        _y = y;
    };

    void ButtonPaletteSelector::setFontSize(float s){
        fontSize = s;
        font.setSize(fontSize);
    };

    bool ButtonPaletteSelector::loadFont(string file){
        bool s = font.setup(file, 1.0, 1024, false, 8, 1.5);
        font.setCharacterSpacing(0);
        return s;
    };


} 	// namespace
