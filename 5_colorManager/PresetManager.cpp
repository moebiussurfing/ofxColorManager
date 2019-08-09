//
// Created by moebiussurfing on 8/9/19.
//

#include "PresetManager.h"



//--------------------------------------------------------------
void PresetManager::setup()
{

}


//--------------------------------------------------------------
void PresetManager::update() {

}


//--------------------------------------------------------------
void PresetManager::draw()
{

}


//--------------------------------------------------------------
void PresetManager::mouseDragged(ofMouseEventArgs& eventArgs){
    const int & x = eventArgs.x;
    const int & y = eventArgs.y;
    const int & button = eventArgs.button;
    //    ofLogNotice("PresetManager") << "mouseDragged " <<  x << ", " << y << ", " << button;
}


//--------------------------------------------------------------
void PresetManager::mousePressed(ofMouseEventArgs& eventArgs){
    const int & x = eventArgs.x;
    const int & y = eventArgs.y;
    const int & button = eventArgs.button;
    //    ofLogNotice("PresetManager") << "mousePressed " <<  x << ", " << y << ", " << button;
}


//--------------------------------------------------------------
void PresetManager::mouseReleased(ofMouseEventArgs& eventArgs){
    const int & x = eventArgs.x;
    const int & y = eventArgs.y;
    const int & button = eventArgs.button;
    //    ofLogNotice("PresetManager") << "mouseReleased " <<  x << ", " << y << ", " << button;
}


//--------------------------------------------------------------
void PresetManager::addMouseListeners()
{
    ofAddListener( ofEvents().mouseDragged, this, &PresetManager::mouseDragged );
    ofAddListener( ofEvents().mousePressed, this, &PresetManager::mousePressed );
    ofAddListener( ofEvents().mouseReleased, this, &PresetManager::mouseReleased );
}


//--------------------------------------------------------------
void PresetManager::removeMouseListeners()
{
    ofRemoveListener( ofEvents().mouseDragged, this, &PresetManager::mouseDragged );
    ofRemoveListener( ofEvents().mousePressed, this, &PresetManager::mousePressed );
    ofRemoveListener( ofEvents().mouseReleased, this, &PresetManager::mouseReleased );
}
