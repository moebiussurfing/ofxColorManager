ofxColorManager 
-------------------

# Overview

**ofxColorManager** is an **openFrameworks** addon as palettes manager with many algorithmic palettes, curved gradients and user presets kit handler.

## Screenshot
![image](/readme_images/Capture.PNG?raw=true "image")

## Features
- Customize gui.
- Basic parameters and app modes.
- Store/Recall settings.
- 


## Usage
 
### ofApp.h
```.cpp
#include "ofxAddonTemplateCustomApp.h"
ofxAddonTemplateCustomApp myAddon;
```

### ofApp.cpp
```.cpp
ofApp::setup(){
	ofxAddonTemplateCustomApp.setup();
}

ofApp::update(){
	ofxAddonTemplateCustomApp.update();
}

ofApp::draw(){
	ofxAddonTemplateCustomApp.draw();
	ofxAddonTemplateCustomApp.drawGui();
}
```

## Dependencies
* ofxGui
* ofxOpenCv
* ofxPoco
* ofxXmlSettings
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)  

Already packed into ```/libs```. No need to add:  
* [ofxColorQuantizerHelper](https://github.com/moebiussurfing/ofxColorQuantizerHelper)
* [ofxColorsBrowser](https://github.com/moebiussurfing/ofxColorsBrowser)
* [ofxColourLoversHelper](https://github.com/moebiussurfing/ofxColourLoversHelper)


## Tested systems
- **Windows10** / **VS2017** / **OF ~0.11**
- **macOS High Sierra** / **Xcode 9/10** / **OF ~0.11**

### TODO/IDEAS
* 

### Notes
*

## Author
Addon by **@moebiusSurfing**  
*(ManuMolina). 2020.*

## License
*MIT License.*