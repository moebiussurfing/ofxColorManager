#include "ofxControlPointManager.h"

#include <string>
#include "ofEventUtils.h"
#include "ofAppRunner.h"
#include "ofMath.h"
#include "ofGraphics.h"
#include "ofLog.h"


// ofxControlPoint
//--------------------------------------------------------------
ofxControlPoint::ofxControlPoint(float _x, float _y)
    : ofVec2f(_x, _y), bHover(false), bSelected(false)
{
}

ofxControlPoint::ofxControlPoint(ofVec2f _vec)
    : ofxControlPoint(_vec.x, _vec.y)
{
}

// setHover
//--------------------------------------------------------------
void ofxControlPoint::setHover(bool _b)
{
    bHover = _b;
}

// isHover
//--------------------------------------------------------------
bool ofxControlPoint::isHover()
{
    return bHover;
}

// select
//--------------------------------------------------------------
void ofxControlPoint::select(bool _b)
{
    bSelected = _b;
}

// isSelected
//--------------------------------------------------------------
bool ofxControlPoint::isSelected()
{
    return bSelected;
}


// ofxControlPointManager
//--------------------------------------------------------------
ofxControlPointManager::ofxControlPointManager()
    : pointSize(8), pointColor(ofColor::fromHex(0x5980f7)), limitArea(0, 0, ofGetWidth(), ofGetHeight())
{
    enableKeyEvents();
    enableMouseEvents();
}

// setPointSize
//--------------------------------------------------------------
void ofxControlPointManager::setPointSize(float _r)
{
    ofxControlPointManager::pointSize = _r;
}

// getPointSize
//--------------------------------------------------------------
float ofxControlPointManager::getPointSize()
{
    return pointSize;
}

// setPointColor
//--------------------------------------------------------------
void ofxControlPointManager::setPointColor(ofColor _c)
{
    pointColor = _c;
}

// getPointColor
//--------------------------------------------------------------
ofColor ofxControlPointManager::getPointColor(ofColor _c)
{
    return ofxControlPointManager::pointColor;
}

// setLimitArea
//--------------------------------------------------------------
void ofxControlPointManager::setLimitArea(ofRectangle _limitArea)
{
    limitArea = _limitArea;
}

// addPoint
//--------------------------------------------------------------
void ofxControlPointManager::addPoint(float _x, float _y)
{
    controlPoints.emplace_back(_x, _y);
}

// addPoint
//--------------------------------------------------------------
void ofxControlPointManager::addPoint(ofVec2f _p)
{
    addPoint(_p.x, _p.y);
}

// clear
//--------------------------------------------------------------
void ofxControlPointManager::clear()
{
    controlPoints.clear();
}

// getPoints
//--------------------------------------------------------------
std::vector<ofVec2f> ofxControlPointManager::getPoints()
{
    std::vector<ofVec2f> points;
    std::transform(controlPoints.begin(), controlPoints.end(), std::back_inserter(points), [](ofxControlPoint &_cp)
    {
        return static_cast< ofVec2f >( _cp );
    });
    return points;
}

// clampMouse
//--------------------------------------------------------------
ofVec2f ofxControlPointManager::clampMouse(float _x, float _y)
{
    float x = ofClamp(_x, limitArea.getLeft(), limitArea.getRight());
    float y = ofClamp(_y, limitArea.getTop(), limitArea.getBottom());
    return ofVec2f(x, y);
}

// keyPressed
//--------------------------------------------------------------
void ofxControlPointManager::keyPressed(ofKeyEventArgs &_data)
{
    int key = _data.key;

    if (key == OF_KEY_COMMAND || key == OF_KEY_CONTROL) bCtrlPressed = true;
    if (key == OF_KEY_SHIFT) bShiftPressed = true;

    if (bCtrlPressed && (key == 'a' || key == 'A'))
    {
        for (auto &cp : controlPoints) cp.select(true);
    }

    std::vector<ofVec2f> moveDirections = {
        {0, -1},
        {1, 0},
        {0, 1},
        {-1, 0}
    };

    int i = 0;
    for (int k : {OF_KEY_UP, OF_KEY_RIGHT, OF_KEY_DOWN, OF_KEY_LEFT})
    {
        if (key == k)
        {
            for (auto &cp : controlPoints)
            {
                if (cp.isSelected())
                {
                    cp.x = ofClamp(cp.x + moveDirections.at(i).x, limitArea.getLeft(), limitArea.getRight());
                    cp.y = ofClamp(cp.y + moveDirections.at(i).y, limitArea.getTop(), limitArea.getBottom());

                    bChanged = true;
                }
            }
        }
        ++i;
    }
}

// keyReleased
//--------------------------------------------------------------
void ofxControlPointManager::keyReleased(ofKeyEventArgs &_data)
{
    int key = _data.key;

    if (key == OF_KEY_COMMAND || key == OF_KEY_CONTROL) bCtrlPressed = false;
    if (key == OF_KEY_SHIFT) bShiftPressed = false;
}

// mouseMoved
//--------------------------------------------------------------
void ofxControlPointManager::mouseMoved(ofMouseEventArgs &_data)
{
    int x = _data.x;
    int y = _data.y;

    ofVec2f mouse = clampMouse(_data.x, _data.y);
    for (auto &cp : controlPoints) cp.setHover(false);

    auto found = std::find_if(controlPoints.begin(), controlPoints.end(), [mouse, this](ofxControlPoint &_cp)
    {
        float dist = (mouse - ofVec2f(_cp.x, _cp.y)).length();
        bool bHover = (dist <= pointSize);
        return bHover;
    });
    if (found != controlPoints.end()) found->setHover(true);
}

// mousePressed
//--------------------------------------------------------------
void ofxControlPointManager::mousePressed(ofMouseEventArgs &_data)
{
    int x = _data.x;
    int y = _data.y;

    mouseMoved(_data);
    clickedPos = clampMouse(x, y);

    auto found = std::find_if(controlPoints.begin(), controlPoints.end(), [this](ofxControlPoint &_cp)
    {
        float dist = (clickedPos - ofVec2f(_cp.x, _cp.y)).length();
        bool bHover = (dist <= pointSize);
        return bHover;
    });
    if (found != controlPoints.end())
    {
        if (found->isSelected())
        {
            bMoving = true;
        }
    }
}

// mouseDragged
//--------------------------------------------------------------
void ofxControlPointManager::mouseDragged(ofMouseEventArgs &_data)
{
    int x = _data.x;
    int y = _data.y;

    ofVec2f mouse = clampMouse(x, y);
    ofVec2f diff = mouse - clickedPos;

    if (bMoving)
    {
        // first time
        if (lastDragPos == ofVec2f(0, 0)) lastDragPos = clickedPos;

        ofVec2f moveDiff = mouse - lastDragPos;

        int left = limitArea.getLeft();
        int right = limitArea.getRight();
        int top = limitArea.getTop();
        int bottom = limitArea.getBottom();

        auto found = std::find_if(controlPoints.begin(), controlPoints.end(), [moveDiff, left, right, top, bottom](ofxControlPoint &_cp)
        {
            if (!_cp.isSelected()) return false;

            int x = ofClamp(_cp.x + moveDiff.x, left, right);
            int y = ofClamp(_cp.y + moveDiff.y, top, bottom);
            return (x == left || x == right || y == top || y == bottom);
        });

        bool bStopOnWall = (found == controlPoints.end());
        if (bStopOnWall)
        {
            for (auto &cp : controlPoints)
            {
                if (cp.isSelected())
                {
                    cp.x = ofClamp(cp.x + moveDiff.x, left, right);
                    cp.y = ofClamp(cp.y + moveDiff.y, top, bottom);

                    if (cp.x == left || cp.y == right || cp.y == top || cp.y == bottom) break;
                }
            }
        }

        lastDragPos = mouse;

        bChanged = true;
    }
    else
    {
        bDragging = true;
        dragArea.set(clickedPos.x, clickedPos.y, diff.x, diff.y);
    }
}

// mouseReleased
//--------------------------------------------------------------
void ofxControlPointManager::mouseReleased(ofMouseEventArgs &_data)
{
    int x = _data.x;
    int y = _data.y;

    ofVec2f mouse = clampMouse(x, y);

    // just click, not drag
    if (mouse == clickedPos)
    {
        for (auto &cp : controlPoints)
        {
            if (bCtrlPressed || bShiftPressed)
            {
                if (cp.isHover()) cp.select(!cp.isSelected());
            }
            else
            {
                cp.select(cp.isHover());
            }
        }
    }
    else if (bMoving)
    {
        lastDragPos.set(0, 0);
        bMoving = false;

        bChanged = true;
    }
        // dragging
    else
    {
        ofVec2f diff = mouse - clickedPos;
        dragArea.set(clickedPos.x, clickedPos.y, diff.x, diff.y);

        for (auto &cp : controlPoints)
        {
            bool isInside = dragArea.inside(cp.x, cp.y);

            if (bCtrlPressed || bShiftPressed)
            {
                if (isInside) cp.select(!cp.isSelected());
            }
            else
            {
                cp.select(isInside);
            }
        }

        dragArea.width = 0;
        dragArea.height = 0;
        bDragging = false;
    }
}

// enableKeyEvents
//--------------------------------------------------------------
void ofxControlPointManager::enableKeyEvents()
{
    ofAddListener(ofEvents().keyPressed, this, &ofxControlPointManager::keyPressed);
    ofAddListener(ofEvents().keyReleased, this, &ofxControlPointManager::keyReleased);
}

// disableKeyEvents
//--------------------------------------------------------------
void ofxControlPointManager::disableKeyEvents()
{
    ofRemoveListener(ofEvents().keyPressed, this, &ofxControlPointManager::keyPressed);
    ofRemoveListener(ofEvents().keyReleased, this, &ofxControlPointManager::keyReleased);
}

// enableMouseEvents
//--------------------------------------------------------------
void ofxControlPointManager::enableMouseEvents()
{
    ofAddListener(ofEvents().mouseMoved, this, &ofxControlPointManager::mouseMoved);
    ofAddListener(ofEvents().mouseDragged, this, &ofxControlPointManager::mouseDragged);
    ofAddListener(ofEvents().mousePressed, this, &ofxControlPointManager::mousePressed);
    ofAddListener(ofEvents().mouseReleased, this, &ofxControlPointManager::mouseReleased);
}

// disableMouseEvents
//--------------------------------------------------------------
void ofxControlPointManager::disableMouseEvents()
{
    ofRemoveListener(ofEvents().mouseMoved, this, &ofxControlPointManager::mouseMoved);
    ofRemoveListener(ofEvents().mouseDragged, this, &ofxControlPointManager::mouseDragged);
    ofRemoveListener(ofEvents().mousePressed, this, &ofxControlPointManager::mousePressed);
    ofRemoveListener(ofEvents().mouseReleased, this, &ofxControlPointManager::mouseReleased);
}

// check changed
//--------------------------------------------------------------
bool ofxControlPointManager::hasChanged()
{
    if (bChanged)
    {
        bChanged = false;
        return true;
    }
    else
    {
        return false;
    }
}

//get a point
//--------------------------------------------------------------
ofVec2f ofxControlPointManager::getPointPosition(int i)
{
    if (i < controlPoints.size())
    {
        float x = controlPoints[i].x;
        float y = controlPoints[i].y;

        ofVec2f p{x, y};
        return p;
    }
    else
    {
        ofLogError("ofxControlPointManager") << "getPointPosition: point " << i << " not found";
    }
}

// draw
//--------------------------------------------------------------
void ofxControlPointManager::draw()
{
    ofPushStyle();

    if (bChanged)
    {
        ofDrawBitmapStringHighlight("CHANGED", 0, 10);
        bChanged = false;
    }

    for (auto &cp : controlPoints)
    {
        bool bHover = cp.isHover();
        float size = pointSize * ((bHover) ? 2 : 1);
        ofFill();
        ofSetColor((cp.isSelected()) ? pointColor : ofColor(255));
        ofDrawRectangle(static_cast< ofVec2f >( cp ) - size / 2, size, size);

        ofNoFill();
        ofSetColor(pointColor);
        ofDrawRectangle(static_cast< ofVec2f >( cp ) - size / 2, size, size);
    }

    drawInfo();

    if (bDragging)
    {
        ofNoFill();
        ofSetColor(255, 128);
        ofDrawRectangle(dragArea);
    }

    ofSetColor(255, 128);
    ofDrawRectangle(limitArea);

    ofPopStyle();
}

// drawInfo
//--------------------------------------------------------------
void ofxControlPointManager::drawInfo()
{
    int i = 0;
    int padX = -65;
    int padY = 20;
    for (auto &cp : controlPoints)
    {
        if (cp.isHover())
        {
            std::string label = "i: " + ofToString(i) + "\nX: " + ofToString((int) cp.x) + "\nY: " + ofToString((int) cp.y);
            ofDrawBitmapStringHighlight(label, cp.x + padX, cp.y + padY);
        }
        ++i;
    }
}

// saveToCsv
//--------------------------------------------------------------
void ofxControlPointManager::saveToCsv(std::string _fileName)
{
    ofFile csv(_fileName, ofFile::WriteOnly);

    bool bFirst = true;
    for (auto &cp : controlPoints)
    {
        if (!bFirst) csv << "\n";
        csv << cp;
        if (bFirst) bFirst = false;
    }
}

// loadFromCsv
//--------------------------------------------------------------
void ofxControlPointManager::loadFromCsv(std::string _fileName)
{
    ofFile csv(_fileName, ofFile::ReadOnly);

    if (csv.exists())
    {
        clear();

        ofBuffer buf = ofBufferFromFile(_fileName);

        int i = 0;
        for (auto &line : buf.getLines())
        {
            std::vector<std::string> splitStr = ofSplitString(line, ",", true, true);
            if (splitStr.size() == 2)
            {
                int x = ofToInt(splitStr.at(0));
                int y = ofToInt(splitStr.at(1));
                addPoint(x, y);
            }
            else
            {
                ofLogWarning() << "[ofxControlPointManager::loadFromCsv] line: " << i << " element size is not 2.";
            }
            ++i;
        }
    }
    else
    {
        ofLogWarning() << "[ofxControlPointManager::loadFromCsv] " << _fileName << " not exists.";
    }
}
