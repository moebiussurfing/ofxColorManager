
#include "ColorRectangle.h"

    void ColorRectangle::setup(const string &_label)
    {
        label = _label;
//        setSize(label.length()*8 + 10, 20);
//        setName(label);

        bDrawBorder = true;
        bDrawBackground = true;

        borderColor = ofColor(0);
        bgColor = ofColor(255);
        labelColor = ofColor(0);

        setLabelVisible(false);
        setFontSize(12);
//        setHeight(10);//hack to prevent mouse block on overlap over other components
        setInset(1, -1);

        // ofAddListener(eventTouchDown, this, &ColorRectangle::onTouchDown);
        // ofAddListener(eventTouchUp, this, &ColorRectangle::onTouchUp);
        // ofAddListener(eventTouchMove, this, &ColorRectangle::onTouchMove);
    }

    void ColorRectangle::draw()
    {
        ofPushStyle();

        if (bDrawBackground) {
//            ofSetColor(bgColor * (isTouched()?0.5:1));
            ofFill();
//            ofDrawRectangle(0, 0, getWidth(), getHeight());
        }

        if (bLabel)
        {
            ofSetColor(labelColor);

            if(font.isLoaded()){
                font.draw(label,fontSize,_x,_y);
            }else{
//                ofDrawBitmapString(label,_x,_y);
//                ofDrawBitmapString(label, _x, getHeight() + _y);
            }

            if (bDrawBorder) {
                ofSetColor(borderColor);
                ofNoFill();
                ofSetLineWidth(1);
//                ofDrawRectangle(0, 0, getWidth(), getHeight());
            }
        }

        ofPopStyle();
    }

    void ColorRectangle::setLabelVisible(bool b){
        bLabel = b;
    }

    void ColorRectangle::setInset(float x, float y){
        _x = x;
        _y = y;
    };

    void ColorRectangle::setFontSize(float s){
        fontSize = s;
        font.setSize(fontSize);
    };

    bool ColorRectangle::loadFont(string file){
        bool s = font.setup(file, 1.0, 1024, false, 8, 1.5);
        font.setCharacterSpacing(0);
        return s;
    };

//     void ColorRectangle::onTouchDown(ofxInterface::TouchEvent &event)
//     {
//         ofVec2f local = toLocal(event.position);
//         colorBACK->set(ofFloatColor(bgColor));

// //        color = ofColor(255);
// //        bTouched = true;
// //
// //        touchAnchor = local;
//     }

//     void ColorRectangle::onTouchUp(ofxInterface::TouchEvent &event)
//     {
// //        ofVec2f local = toLocal(event.position);
// //
// //        color = ofColor(100);
// //        bTouched = false;
//     }

//     void ColorRectangle::onTouchMove(ofxInterface::TouchEvent &event)
//     {
// //        ofVec2f parentPos = ((Node*)parent)->toLocal(event.position);
// //
// //        setPosition(parentPos - touchAnchor);
//     }

//     void ColorRectangle::setup_colorBACK(ofFloatColor &c)
//     {
//         colorBack_DEFINED = true;
//         colorBACK = &c;
//     }


