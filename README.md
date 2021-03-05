# ofxColorManager

# Description
This **openFrameworks** add-on is a palette and **kit of tools** to create color presets for your scenes.  

## Screenshot
![image](/readme_images/Capture.JPG?raw=true "image")

## Overview
Included Tools and Engines:  
* Algorithmic **Theory** colors.
* **Range spaces** of two colors.
* **Colour Lovers** online **API** to search keywords, handle favorites and history search.
* **Extract** the dominant colors **from images**.
* **Gradient** designer with an extra picked color.
* User **Kit Presets Browser**: Receive from all engines, duplicate presets and preview all your kit.
* ```ofxColorClient```: 
  * A Lightweight version of the add-on. 
  * To work linked to this **MASTER** addon but as a minimal **CLIENT** into your own **OF App**. 
  * Standalone mode to browse and use your kit with preset files.

## Features
* Demo Scenes for fast preview.
* ImGui based GUI. Docked and customizable.
* Linkable with ```ofxColorClient``` trough TCP or Watching files.
* Kit exporter of all ```JSON``` preset files to your project data path.
* Wheel and rectangle color pickers.
* **Pantone** library with 2000+ colors.
* Edit, rearrenge, delete and drag colors. 

## Usage
### ofApp.h
```.cpp
#include "ofxColorManager.h"

ofxColorManager colorManager;

vector<ofColor> palette;
```
### ofApp.cpp
```.cpp
void ofApp::setup()
{
	colorManager.setLinkPalette(palette); // subscribe palette

	colorManager.setup();
}

void ofApp::draw()
{
	// Draw your Scene using the colors
	// Nothing more!
}
```

## Dependencies
* [ofxColorClient](https://github.com/moebiussurfing/ofxColorClient)
* [ofxImGui](https://github.com/moebiussurfing/ofxImGui)  
  Fork from @Daandelange
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)  
* [ofxIO](https://github.com/bakercp/ofxIO)
* ofxNetwork
* ofxPoco
* ofxGui
* ofxOpenCv
* ofxXmlSettings
* [ofxSCENE-SVG](https://github.com/moebiussurfing/ofxSCENE-SVG)  
  for some example only 

Already packed into ```OF_ADDON/libs```. No need to add them manually with the **OF Project Generator**:  
* [ofxColorQuantizerHelper](https://github.com/moebiussurfing/ofxColorQuantizerHelper)
* [ofxColorsBrowser](https://github.com/moebiussurfing/ofxColorsBrowser)
* [ofxColourLoversHelper](https://github.com/moebiussurfing/ofxColourLoversHelper)
*Thanks a lot to all these ofxAddons coders. 
Look into each folder for authoring credits, original forks, and license info.*  

## Tested systems
- **Windows 10** / **VS 2017** / **OF ~0.11**

## TODO

## Author
An addon by **@moebiusSurfing**  
*(ManuMolina). 2020.*

## License
*MIT License.*