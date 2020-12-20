//
// Copyright (c) 2009 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofMain.h"


class ofxRectangle: public ofRectangle
{
public:

//    bool isClickable;
//    setClickable(bool b)
//    {
//        isClickable = b;
//    }

    ofxRectangle()
    {
        ofColor c;
        string name;
        int position_inVector;

        ofRectangle rect;
        init(rect, c);
    }

    ofxRectangle(const ofRectangle& rect,
            const ofColor& _color)
    {
        init(rect,_color);
    }

    void init(const ofRectangle& rect,
            const ofColor& _color)
    {
        x      = rect.x;
        y      = rect.y;
        width  = rect.width;
        height = rect.height;

        isOver     = false;
        isSelected = false;
        color      = _color;

        hAlign = OF_ALIGN_HORZ_LEFT;
        vAlign = OF_ALIGN_VERT_CENTER;

//        isClickable = true;
    }

//    void updateScaleHandles() {
//        for(int i = 0; i)
//    }

    void update()
    {
    }

    void draw(std::size_t index, std::size_t selectionIndex)
    {
        if (bShowdebug) {

            int bmLineHeight = 12;

            stringstream ss;
            ss << " idx:[" + ofToString(index) + "]" << endl;
            ss << "sidx:[" + ofToString(selectionIndex) + "]" << endl;
            ss << " x/y:[" + ofToString(getLeft()) + "," + ofToString(getTop()) + "]" << endl;
            ss << "peri:[" + ofToString(getPerimeter()) + "]" << endl;
            ss << "area:[" + ofToString(getArea()) + "]" << endl;
            ss << "size:[" + ofToString(width, 0) + "x" + ofToString(height, 0) + "]" << endl;

            ofFill();
            ofSetColor(255, 127);
            ofDrawBitmapString(ss.str(), getLeft() + 2, getTop() + 12);
        }

        ofPoint rectCenter = getCenter();

        // big rectangle ?
        ofFill();
//        ofSetColor(color,isSelected ? 110 : 80);
        ofSetColor(color,isSelected ? 255 : 255);
        ofDrawRectangle(*this);

        if (isSelected)
        {
            if (bShowdebug)
            {
                // center circle
                ofSetColor(color,255);
//            ofSetColor(ofColor::black,255);
                ofNoFill();
                ofRectangle r;
                r.setFromCenter(rectCenter,5,5);
                ofDrawRectangle(r);

                ofFill();
                int cornerW = 3;
                int cornerH = 3;
                ofRectangle corner;
                corner.setFromCenter(getTopLeft(), cornerW, cornerH);
                ofDrawRectangle(corner);
                corner.setFromCenter(getTopRight(), cornerW, cornerH);
                ofDrawRectangle(corner);
                corner.setFromCenter(getBottomRight(), cornerW, cornerH);
                ofDrawRectangle(corner);
                corner.setFromCenter(getBottomLeft(), cornerW, cornerH);
                ofDrawRectangle(corner);
            }
        }

        // borders debugger
        if (bShowdebug)
        {
            ofNoFill();
//        ofSetColor(color,isOver ? 255 : 100);
            ofSetColor(ofColor::black, isOver ? 255 : 200);

            ofDrawRectangle(*this);
        }

        // anchors and extra lines
        if (isOver && !isSelected && bShowdebug)
        {
            int offset = 3;
            int xLeft   = rectCenter.x - offset;
            int xRight  = rectCenter.x + offset;
            int yTop    = rectCenter.y - offset;
            int yBottom = rectCenter.y + offset;

            ofPoint topLeft(xLeft,yTop);
            ofPoint topRight(xRight,yTop);
            ofPoint bottomLeft(xLeft,yBottom);
            ofPoint bottomRight(xRight,yBottom);

            ofDrawLine(topLeft,bottomRight);
            ofDrawLine(topRight,bottomLeft);
        }

        float hAnchor = -1;
        float vAnchor = -1;

        switch (hAlign) {
            case OF_ALIGN_HORZ_IGNORE:
                hAnchor = -1;
                break;
            case OF_ALIGN_HORZ_LEFT:
                hAnchor = getMinX();
                break;
            case OF_ALIGN_HORZ_RIGHT:
                hAnchor = getMaxX();
                break;
            case OF_ALIGN_HORZ_CENTER:
                hAnchor = rectCenter.x;
                break;
        }

        switch (vAlign) {
            case OF_ALIGN_VERT_IGNORE:
                vAnchor = -1;
                break;
            case OF_ALIGN_VERT_TOP:
                vAnchor = getMinY();
                break;
            case OF_ALIGN_VERT_BOTTOM:
                vAnchor = getMaxY();
                break;
            case OF_ALIGN_VERT_CENTER:
                vAnchor = rectCenter.y;
                break;
        }

        if (bShowdebug) {
            if (hAnchor != -1) {
                ofSetColor(255, 80);
                ofDrawLine(hAnchor, getMinY() - 13, hAnchor, getMinY() - 3);
                ofDrawLine(hAnchor, getMaxY() + 13, hAnchor, getMaxY() + 3);
            } else {

            }

            if (vAnchor != -1) {
                ofSetColor(255, 80);
                ofDrawLine(getMinX() - 13, vAnchor, getMinX() - 3, vAnchor);
                ofDrawLine(getMaxX() + 13, vAnchor, getMaxX() + 3, vAnchor);
            } else {
            }
        }
    }

    ofRectangle scaleHandles[8];

    ofColor color;

    //-
    // TEST
    string name;
    void setName(string n)
    {
        name = n;
    }

    int position_inVector;
    void setPosition_inVector(int i)
    {
        position_inVector = i;
    }
    //-

    bool isOver;
    bool isPressed;
    bool isSelected;

    ofAlignHorz hAlign;
    ofAlignVert vAlign;

    glm::vec2 dragOffset;

    bool bShowdebug = false;
    bool bDraggable = false;
    void setDebug(const bool b)
    {
        bShowdebug = b;
    }
};
