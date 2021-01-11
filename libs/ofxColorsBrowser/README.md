ofxColorsBrowser
=============================

# Overview
**ofxColorsBrowser** is an **openFrameworks** addon to browse color libraries.

## Screenshot
![image](/readme_images/Capture.PNG?raw=true "image")

## Features
* Three included libraries.
  * **Pantone** (+2000 colors from a JSON file).
  * **Open Color**.
  * **OF Native** named colors
* **GUI** with clickable colored and named boxes. 
  * Can be completely disabled to simplify. (But still having access to the colors)
* Key commands and *ofxGui* panel.
* Basic parameters and app modes.
* Store/Recall settings.
* Technically updatable with other libraries (JSON, XML, or other libs/addons).

## Usage
 
### ofApp.h
```.cpp
#include "ofxColorsBrowser.h"

ofxColorsBrowser colorBrowser;
ofFloatColor color_BACK;
```

### ofApp.cpp
```.cpp
ofApp::setup(){
    colorBrowser.setup_colorBACK(color_BACK);// set local target color auto-receiver
    colorBrowser.setup();

ofApp::update(){
	ofxColorsBrowser.update();
}

ofApp::draw(){
    ofBackground( ofColor( color_BACK ));// auto updated when changes
	colorBrowser.draw();
}
```

## Dependencies
* ofxGui
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)  

*Not required when interface is disabled:*  
* [ofxRectangleUtils](https://github.com/bakercp/ofxRectangleUtils) 
* [ofxFontStash](https://github.com/armadillu/ofxFontStash)  

Already included into ```/libs```:  
* ofxOpenColor (https://github.com/pd-andy/ofxOpenColor)  
 
## Tested systems
- **Windows10** / **VS2017** / **OF ~0.11**
- **macOS High Sierra** / **Xcode 9/10** / **OF ~0.11**

## Author
Addon by **@moebiusSurfing**  
*(ManuMolina). 2020.*  

Thanks to *@bakercp* adn *@pd-andy* for their repositories.  

## License
*MIT License.*