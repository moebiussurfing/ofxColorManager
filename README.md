# Overview
This **openFrameworks** add-on is a palette and **colors manager** to create color presets for your scenes.  

## Screenshot
![image](/readme_images/Capture.jpg?raw=true "image")

## Highlights
Included tools and engines:  
* Algorithmic **theory** color.
* **Range spaces** of two colors.
* **Colour Lovers** online **API** to search keywords, handle favorites and history search.
* **Extract** or quantize dominant colors **from images**.
* Curved Gradients for an extra picked color or **gradient design**.
* User **Kit Presets browser**: edit, rearrenge, delete colors, duplicate presets, preview your kit.
* ofxColorClient: A twin add-on to work linked to this **MASTER** addon or standalone but as minimal **CLIENT** into your own **OF App**. 

## Features
* Demo Scenes for fast preview.
* ImGui based GUI.
* Linkable with ```ofxColorClient``` trough TCP or Watching files.
* Kit exporter of all ```JSON``` preset files.
* Color pickers.
* Pantone library with 2000+ colors.
* Draggable color boxes.

## Usage
### ofApp.h
```.cpp
#include "ofxColorManager.h"

ofxColorManager colorManager;

vector<ofColor> palette;
ofColor colorPick;
```
### ofApp.cpp
```.cpp
void ofApp::setup()
{
	colorManager.setLinkPalette(palette);     // subscribe palette
	colorManager.setLinkColorPick(colorPick); // subscribe picked color

	colorManager.setup();
}

void ofApp::draw()
{
	// Draw your Scene using the palette colors
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