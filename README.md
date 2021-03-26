# ofxColorManager

This **openFrameworks** add-on is a **kit of Engines and Tools** to design and organize color palettes for your scenes.  

## Screenshot
![image](/readme_images/Capture.JPG?raw=true "image")

## Videos
**ofxColorManager**:  

[![VIDEO](http://img.youtube.com/vi/oSvGwpbWEuc/0.jpg)](http://www.youtube.com/watch?v=oSvGwpbWEuc "VIDEO")

**ofxColorClient**:  

[![VIDEO](http://img.youtube.com/vi/oSvGwpbWEuc/0.jpg)](http://www.youtube.com/watch?v=oSvGwpbWEuc "VIDEO")

## Engines:  
  1. **10+** Algorithmic **Theory Schemes** from a base color.  
  *Complementary, Compound, Monochrome, Analogous, Triad...*  
  2. **10+** **Range Spaces** between two colors.  
  *RGB, LAB, CMYK, XYZ...*  
  3. [**Colour Lovers API**](https://www.colourlovers.com/) to search keywords online, handle favorites, and history search.
  4. **Extract** the dominant colors **from image files**.  
  *Sorting by Dominance, Hue, Saturation or Brightness*.

## Tools  
* **Editor**: 
  * Organize the colors.
  * Edit using the color pickers and the Pantone colors library.
* **Kit of Presets Manager**: 
  * Receive palettes from all the engines and save presets. 
  * Name, modify, delete or duplicate presets from your kit.
  * Preview all your kit of preset on a scrollable clickable panel.
* **Gradient Designer**:
  * Auto-generates a gradient from each palette.
  * Exposes an available extra pickable color.

## Modes

#### 1. **Powered Standalone Mode**:  
Use this add-on inside your **OF_App** (Scene Renderer) when you want to use all the tools bundled into one single app.  
Requires all the **ofxColorManager** dependencies. (Similar to the included **1-example_Basic**)  

#### 2. **Linked Mode**:  
Link the **ofxColorManager** container **MASTER OF_App** to your **CLIENT OF_App** with the **ofxColorClient** add-on inside.  
Allows all the tools, but a more handy setup using two separated apps.  

#### 3. **Minimal Standalone Mode**:
The more **Lightweight** Mode that allows you to use your local preset files.  
Using the **ofxColorClient add-on** only, without all the bigger Engines and Tools.  

( More performant in most scenarios and using fewer dependencies than in the other two modes that require **ofxColorManager**. )
 
## ofxColorClient: The player addon 
  * A Lightweight version of the add-on just to use your **Presets Kit**. 
  * Bundled with a minimal set of tools: Kit browser, palette and gradient previews, and tweened transitions.  
  * To work linked to a another **OF_App** (**MASTER**) using the **ofxColorManger** addon, but inside a minimal version into your own **OF_App** (**CLIENT**). 
  * Works too in the Standalone Mode to browse and use your kit with preset files.

## Other Features
* **Demo Scenes** for fast palette preview. Colored SVG and bubble scenes.
* **ImGui** based GUI. Docked and customizable.
* **Linkable** with **ofxColorClient** trough TCP or Watching files.
* Hue Wheel and rectangle **Color Pickers** with **HSV** sliders.
* **Pantone** library with **2000+** named **colors**.
* Manage: Edit, add, remove, sort, shift, and drag your palette colors. 
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
  // Use the palette colors to draw your Scene. 
  // Nothing more!
}
```

## Dependencies
* [ofxColorClient](https://github.com/moebiussurfing/ofxColorClient)
* [ofxImGui](https://github.com/Daandelange/ofxImGui/tree/jvcleave)  [ Fork from @Daandelange Thanks **Daan**! ]
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)  
* [ofxIO](https://github.com/bakercp/ofxIO)
* ofxNetwork  [ **OF** ]
* ofxPoco  [ **OF** ]
* ofxGui  [ **OF** ]
* ofxOpenCv  [ **OF** ]
* ofxXmlSettings [ **OF** ]
* [ofxSCENE-SVG](https://github.com/moebiussurfing/ofxSCENE-SVG)  [ Only for the example ]
* [ofxScaleDragRect](https://github.com/moebiussurfing/ofxScaleDragRect)

Above addons already packed into ```OF_ADDON/libs```. No need to add them manually with the ```OF Project Generator```:  
* [ofxColorQuantizerHelper](https://github.com/moebiussurfing/ofxColorQuantizerHelper)
* [ofxColorsBrowser](https://github.com/moebiussurfing/ofxColorsBrowser)
* [ofxColourLoversHelper](https://github.com/moebiussurfing/ofxColourLoversHelper)  

*Thanks a lot to all these ofxAddons coders. Look into each folder for authoring credits, original forks, and license info.*  

## File format
The **JSON** file format of a palette preset it's simple. This is an example of a 3 colors palette file content:
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
- **macOS**. **High Sierra** / **Xcode9** & **Xcode10** / **OF ~0.11**

## Notes / TODO
+ Processing / Unity3D / UE4 clients plugins. _(?)_
+ Export Adobe ASE. _(?)_
+ Tween transitions to presets.
+ Undo engine.
+ Create an App more user-friendly and very simplified GUI.

## Author
An addon by **@moebiusSurfing**  
*( ManuMolina ) 2019-2021*

## License
*MIT License*
