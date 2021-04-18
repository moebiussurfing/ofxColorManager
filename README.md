# ofxColorManager
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
![Stage: alpha](https://img.shields.io/badge/-alpha-red)

This **openFrameworks** add-on is a **bundle of Tools and Engines** to create and organize **Color Palettes** for your scenes.  

## Screenshot
![image](/doc/readme_images/Capture.JPG?raw=true "image")

## Videos

**ofxColorManager**:  

[![VIDEO](http://img.youtube.com/vi/oSvGwpbWEuc/0.jpg)](http://www.youtube.com/watch?v=oSvGwpbWEuc "VIDEO")

**ofxColorClient**:  

[![VIDEO](http://img.youtube.com/vi/oSvGwpbWEuc/0.jpg)](http://www.youtube.com/watch?v=oSvGwpbWEuc "VIDEO")

<br/>

## Engines  

_Send palettes to the Editor._  

  1. Algorithmic **Theory Schemes** from a base color.  
  *Analogous, Compound, Flipped Compound, Monochrome, Tetrad, Triad, Complementary, Split and Flipped.*  

![GIF](https://media.giphy.com/media/AbfmncgOsqcNqtPxup/giphy.gif)

  2. **Range Spaces** between two colors.  
  *RGB, HSL, HSV, HSB, LUV, LAB, HLAB, LCH, CMY, CMYK, YXY, and XYZ.*  

![GIF](https://media.giphy.com/media/mOTlQQ5ILnmnTVrLpE/giphy.gif)

  3. [**Colour Lovers API**](https://www.colourlovers.com/) to search keywords online, handle favorites, and history search.  
 
 ![GIF](https://media.giphy.com/media/L07R0ixmQrAvh9zfui/giphy.gif)

  4. **Extract** the dominant colors **from image files**.  
  *Sorting by Dominance, Hue, Saturation or Brightness*.  

![GIF](https://media.giphy.com/media/jMgk4fCbaeJG5pqjtv/giphy.gif)

## Tools  

_Palettes ​Management._

* **Editor**: 
  * Receive from the Engines and organize the palette colors.
  * Edit using the color pickers and the _Pantone_ colors library.
  * Add, remove, sort, shift, and drag colors.
  * [Coolors.co](https://coolors.co/palettes/trending) URL import/export.

* **Kit of Presets Manager**: 
  * Receive palettes, name and save to presets kit. 
  * Tweak, delete or duplicate presets from your kit.
  * Fast preview all your presets on a scrollable clickable panel.
  * Your user kit is ready to use (also in other apps using the **ofxColorClient** player.)

* **Gradient Designer**:
  * Auto-generates a gradient for each palette.
  * Exposes the all the gradient and another extra pickable color.
  * Tweackeable curve to deform spread distribution.
  
## Three Workflow Setups

#### 1. **Powered Standalone**:  
**One Single app**:  
Use the add-on inside your _Scene Render App_ when you want to use all the bundled Tools.  
( _Requires all the dependencies._ )  

#### 2. **Minimal Standalone**:
**One single App**:  
A **Lightweight** Mode into your _Scene Render App_.  
Only using the **ofxColorClient** add-on.  
( Without the bigger **ofxColorManager** )  
Just as a player or palettes browser, but without all the Engines and Tools.  
Only included some minimal edit tools. Fewer dependencies.  

#### 3. **Dual Linked**:  
**Two Apps**:  
All the Engines and Tools, but with a more handy setup.  
Links a **MASTER OF_App** (with the **ofxColorManager**) to your **CLIENT OF_App** (with the **ofxColorClient**).  
 
<br/>

## ofxColorClient: The Player add-on 
  * Also included into **ofxColorManager**. 
  * A Lightweight version of the add-on.
  * Works in the **Standalone Mode** too. 
  * Browse and use your **Presets Kit** files.
  * A minimal set of tools: 
  Kit browser, palette and gradient previews, and tweakeable tweened transitions.  
  
<br/>

## Ready-To-Use Binaries

Ready to use pre-compiled versions for fast testing, or to have executable apps with for the most common uses/learning.  
*Windows* / *macOS*

### 1. **PALETTO v1.0rc** 
**Master App**  
Works Standalone or Linked to a **CLIENT App** : **_SceneTEST_COLORiZED_**  
( _Based on _1-example_Basic_ from **ofxColorManager**_ )  
**Download**:  
<a href="https://moebiussurfing.itch.io/paletto"><img src="/docs/itch.io/Paletto.ico" align="left" height="96" width="96" ></a>  
<br></br>

### 2. **Scene_COLORiZED v1.0**
**Client App**  
Works Standalone or Linked to a **MASTER App** : **_Paletto v1.0_**  
( _Based on _3-example_Scenes_ from **ofxColorClient**_ )  
**Download**:  
<a href="https://moebiussurfing.itch.io/paletto"><img src="/docs/itch.io/Scene_COLORiZED.ico" align="left" height="96" width="96" ></a>  
<br></br>

## Extra Info

<details>
  <summary>HowTo / Guide</summary>
  <p>

**THREE DIFFERENT WORKFLOWS**

1. Open the **MASTER App** alone to create, manage and browse palettes. Explore the *GUI*.  

2. Open the **CLIENT App** alone and browse the bundled kit of presets files.  

3. To play on **Linking Mode**:  
  Open both **MASTER** and **CLIENT** Apps, and start creating and browsing presets on **MASTER** App,  
  and look how **CLIENT** App is linked and coloring your scene.  

* NOTES / HELP
  * When **TCP Linking**, you should open the **Master App** at first.  
  * Sometimes you need to toggle _Off/On_ the **TCP Link** toggle. 
  </p>
</details>

<details>
  <summary>Other Features</summary>
  <p>

  * Import and export [Coolors.co](https://coolors.co/palettes/trending) URL format using clipboard copy/paste.  	
  * **Demo Scenes** for fast palette preview. Colored SVG with blending and Bubble scenes.
  * **ImGui** based GUI. Docked and customizable with layouts presets management.
  * Hue wheel and rectangle **Color Pickers** with **HSV** sliders and clipboard copy/paste codes.
  * **Pantone** library with **2000+ colors**.
  * Kit exporter of all **JSON** preset files to your project data path. (Can be used on the Standalone Client App.)
  </p>
</details>

<details>
  <summary>Usage</summary>
  <p>

**ofApp.h**
```.c++
#include "ofxColorManager.h"

ofxColorManager colorManager;
vector<ofColor> palette; // colors to use
```
**ofApp.cpp**
```.c++
void ofApp::setup()
{
  colorManager.setLinkPalette(palette); // subscribe local palette
  colorManager.setup();
}

void ofApp::draw()
{
  /*
  
  Use the colors
  Nothing more!
  
  */
}
```
  </p>
</details>

<details>
  <summary>Required Dependencies</summary>
  <p>

Clone these add-ons and include into the **OF Project Generator** to allow compile your projects or the examples:
* [ofxColorClient](https://github.com/moebiussurfing/ofxColorClient)
* [ofxImGui](https://github.com/Daandelange/ofxImGui/tree/jvcleave)  [ Fork from @Daandelange. Thanks **Daan**! ]
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)  
* [ofxScaleDragRect](https://github.com/moebiussurfing/ofxScaleDragRect)
* [ofxIO](https://github.com/bakercp/ofxIO)
* [ofxKuNetwork](https://github.com/moebiussurfing/ofxKuNetwork)  [ Fork from @kuflex ]
* ofxNetwork  [ **OF** ]
* ofxPoco  [ **OF** ]
* ofxGui  [ **OF** ]
* ofxOpenCv  [ **OF** ]
* ofxXmlSettings [ **OF** ]
* [ofxSCENE-SVG](https://github.com/moebiussurfing/ofxSCENE-SVG)  [ Only for the example ]
* [ofxWindowApp](https://github.com/moebiussurfing/ofxWindowApp)  [ Only for the example ]
  </p>
</details>

<details>
  <summary>Included Dependencies</summary>
  <p>

The above add-ons already packed somewhere into **OF_ADDON/libs**.  
No need to add them manually with the **OF Project Generator**:  
* [ofxColorQuantizerHelper](https://github.com/moebiussurfing/ofxColorQuantizerHelper)
* [ofxColorsBrowser](https://github.com/moebiussurfing/ofxColorsBrowser)
* [ofxColourLoversHelper](https://github.com/moebiussurfing/ofxColourLoversHelper)  
* [ofxMouseRuler2](https://github.com/moebiussurfing/ofxMouseRuler2)  
* [ofxCameraSaveLoad](https://github.com/roymacdonald/ofxCameraSaveLoad)  
* [ofxColorMorph](https://github.com/makobouzu/ofxColorMorph)  
* [ofxColorPalette](https://github.com/aspeteRakete/ofxColorPalette)  
* [ofxColorTheory](https://github.com/kasparsj/ofxColorTheory)  
* [ofxColourLovers](https://github.com/borg/ofxColourLovers)  
* [ofxColorQuantizer](https://github.com/mantissa/ofxColorQuantizer)  
* [ofxHttpUtils](https://github.com/arturoc/ofxHttpUtils)  
* [ofxSimpleTimer](https://github.com/HeliosInteractive/ofxSimpleTimer)  

For **ofxColorClient**:  
* [ofxAnimatable](https://github.com/armadillu/ofxAnimatable)  
* [ofxColorGradient](https://github.com/armadillu/ofxColorGradient)  
* [ofxCurvesTool](https://github.com/kylemcdonald/ofxCurvesTool)  
* [ofxSerializer](https://github.com/bakercp/ofxSerializer)  
* [ofxSimpleSlider](https://github.com/danomatika/ofxPd/blob/master/pitchShifter/src/ofxSimpleSlider.h)  

*Thanks a lot to all these ofxAddons coders. Look into each folder for authoring CREDITS, original forks, and license info.*  
  </p>
</details>

<details>
  <summary>Preset File format</summary>
  <p>

The **JSON** file format of a palette preset quite simple.  
This is an example of a _3 colors palette_ file content:
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
  </p>
</details>

<details>
  <summary>Tested Systems</summary>
  <p>

  - **Windows 10** / **VS 2017** / **OF ~0.11**
  - **macOS**. **High Sierra** / **Xcode9** & **Xcode10** / **OF ~0.11**
  </p>
</details>

<details>
  <summary>Credits</summary>
  <p>

*List of GitHub names of all coders of used ofxAddons:*  
@jvcleave, @Daandelange, @roymacdonald, @bakercp, @kuflex, @borg, @mantissa, @makobouzu, @aspeteRakete, @kasparsj, @arturoc, @HeliosInteractive, @frauzufall, @armadillu, @kylemcdonald, @golanlevin.
  </p>
</details>

<details>
  <summary>IDEAS / TODO</summary>
  <p>

+ Global Saturation / Brightness modifiers to all the palette colors. _(?)_  
+ Create an App using all the power but more user-friendly and a very simplified GUI. _(?)_
  + Export Adobe .ASE, .ACO, ...etc _(?)_
+ Think about other creative code tools client/add-on: *Processing* / *Unity3D* / *UE4* clients plug-ins. _(?)_
+ Tween transitions to presets also on master app. _(?)_
+ Undo engine. _(?)_
+ Improve Gradient Engine adding cosine/shifting algorithms. 
  + Add an example and improve gradient exposing and background tool. 
  + [ofxCosineGradient](https://github.com/rystylee/ofxCosineGradient)
  + [DearWidgets](https://github.com/soufianekhiat/DearWidgets)
  </p>
</details>

<br/>

## Author
An addon by **@moebiusSurfing**  
*( ManuMolina ) 2019-2021*  

<br/>

## License
[**MIT License**](https://github.com/moebiussurfing/ofxColorManager/blob/b29c56f7b0e374b6a6fe2406e45fbfaaf2726112/LICENSE)

<br/>

<h1>📫 Contact / Follow</h1>

<p>
<a href="https://twitter.com/moebiusSurfing/" rel="nofollow">Twitter</a> | 
<a href="https://www.youtube.com/moebiusSurfing" rel="nofollow">YouTube</a> | 
<a href="https://www.instagram.com/moebiusSurfing/" rel="nofollow">Instagram</a> | 
<a href="https://github.com/moebiussurfing" target="_blank">GitHub</a> | 
<a href="mailto:moebiussurfing@gmail.com" target="_blank">Email</a>
</p>

<br/>

### 🔭 Comming Soon...

<br/>

<div align="left">
<!--
<img src="./banner.gif">
-->
<img src="https://github.com/moebiussurfing/ofxColorManager/blob/develop/docs/itch.io/Paletto_Thumbnail.png">
</div>
