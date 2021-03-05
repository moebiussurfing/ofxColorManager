# ofxColorManager

## Description
This **openFrameworks** add-on is a palette and **kit of tools** to create color presets for your scenes.  

## Screenshot
![image](/readme_images/Capture.JPG?raw=true "image")

## Included Tools and Engines:  
* Algorithmic **Theory** colors from a base color.
* **Range Spaces** between two colors.
* **Colour Lovers** online **API** to search keywords, handle favorites and history search.
* **Extract** the dominant colors **from images**.
* **Gradient** designer with an extra picked color.
* **Kit Presets Browser**: 
  * Receive from all engines and save. 
  * Name, delete or duplicate presets from your kit.
  * Preview all your kit on a scrollable panel.

## ofxColorClient 
  * A Lightweight version of the add-on just to use presets. 
  * To work linked to this **MASTER** addon but as a minimal **CLIENT** into your own **OF App**. 
  * Standalone mode to browse and use your kit with preset files.

## Other Features
* Demo Scenes for fast preview.
* ImGui based GUI. Docked and customizable.
* Linkable with **ofxColorClient** trough TCP or Watching files.
* Wheel and rectangle color pickers.
* **Pantone** library with **2000+** named colors.
* Edit, rearrenge, delete and drag colors. 
* Kit exporter of all **JSON** preset files to your project data path.

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
	// Draw your Scene using the colors. Nothing more!
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

## File format
The **JSON** file format of a preset it's simple. This is a 3 colors palette file content as an example:
```.json
[
    {
        "a": 255,
        "b": 206,
        "g": 69,
        "r": 4
    },
    {
        "a": 255,
        "b": 165,
        "g": 103,
        "r": 3
    },
    {
        "a": 255,
        "b": 125,
        "g": 137,
        "r": 3
    }
]
```

## Tested systems
- **Windows 10** / **VS 2017** / **OF ~0.11**

## TODO

## Author
An addon by **@moebiusSurfing**  
*(ManuMolina). 2020.*

## License
*MIT License.*