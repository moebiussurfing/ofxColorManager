# ofxColorManager

This **openFrameworks** add-on is a **kit of Engines and Tools** to design presets of color palettes for your scenes.  

## Screenshot
![image](/readme_images/Capture.JPG?raw=true "image")

## Engines:  
  1. 15 Algorithmic **Theory** colors from a base color.
  2. 12 **Range Spaces** between two colors.
  3. **Colour Lovers** online **API** to search keywords, handle favorites and history search.
  4. **Extract** the dominant colors **from image files**.

## Tools:  
* **Gradient** designer with an extra pickeable color.
* **Kit Browser**: 
  * Receive palette presets from then engines and save. 
  * Name, delete or duplicate presets from your kit.
  * Preview all your kit on a scrollable panel.

## Modes:
  1. **Powered Standalone Mode**:  
    Use this add-on inside your **OF_App** when you want to use all the tools bundled.  
    Requires all the dependencies. (Similar to the inclued example)
  2. **Linked Mode**:  
    Link the **ofxColorManager** container **MASTER OF_App** to your **CLIENT OF_App** with the client add-on inside.  
    Allows all the tools, but a more handy setup using two separated apps.
  3. **Minimal Standalone Mode**:  
    Using the **client add-on** only.  
    The more **Lightweight** version that allows you to browse your local preset files.  
    Bundled with a minimal set of tools: kit broser, palette and gradient previews...

## ofxColorClient 
  * A Lightweight version of the add-on just to use your presets Kit, or: 
  * To work linked to this **MASTER OF_App** addon but as a minimal version into your own **CLIENT OF_App**. 
  * Or just as standalone mode to browse and use your kit with preset files.

## Other Features
* Demo Scenes for fast preview. Colored SVG and bubble scenes.
* ImGui based GUI. Docked and customizable.
* Linkable with **ofxColorClient** trough TCP or Watching files.
* Hue Wheel and rectangle color pickers with HSB sliders.
* **Pantone** library with **2000+** named colors.
* Edit, add, delete, sort, shift, and drag your palette colors. 
* Kit exporter of all **JSON** preset files to your project data path.

## Usage
That's similar to **1-example_Basic**.  
This is recommended to de **Power Standalone Mode**.  
For the other two Modes using the **CLIENT**, you can use the **1-example_Basic** as-is.
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
	// Use the palette colors to draw your Scene. Nothing more!
}
```

## Dependencies
* [ofxColorClient](https://github.com/moebiussurfing/ofxColorClient)
* [ofxImGui](https://github.com/moebiussurfing/ofxImGui)  Fork from @Daandelange Thanks **Daan**!
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)  
* [ofxIO](https://github.com/bakercp/ofxIO)
* ofxNetwork [**OF**]
* ofxPoco [**OF**]
* ofxGui [**OF**]
* ofxOpenCv [**OF**]
* ofxXmlSettings [**OF**]
* [ofxSCENE-SVG](https://github.com/moebiussurfing/ofxSCENE-SVG)  Only for an example 
* [ofxScaleDragRect](https://github.com/moebiussurfing/ofxScaleDragRect)

Already packed into ```OF_ADDON/libs```. No need to add them manually with the ```OF Project Generator```:  
*Thanks a lot to all these ofxAddons coders. Look into each folder for authoring credits, original forks, and license info.*  
* [ofxColorQuantizerHelper](https://github.com/moebiussurfing/ofxColorQuantizerHelper)
* [ofxColorsBrowser](https://github.com/moebiussurfing/ofxColorsBrowser)
* [ofxColourLoversHelper](https://github.com/moebiussurfing/ofxColourLoversHelper)  

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