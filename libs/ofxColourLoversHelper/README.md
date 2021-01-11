ofxColourLoversHelper
------------------------------------

# Overview
**ofxColourLoversHelper** is an **openFrameworks** helper addon to handle **ColourLovers** palettes.
 
The core of this addons is https://github.com/borg/ofxColourLovers.  
Thanks to **@borg**!  
I just added some methods to help integrate into my projects/addons, and also switched from *ofxUI* to *ofxImGui* user control.

## Screenshots
![image](/readme_images/Capture_ofxImGui.PNG?raw=true "image")  

## Features
- Implemented GUI with **ImGui**.  
- Alternative GUI using the old **ofxUI**  


## Usage
 - Look the examples.

## Dependencies
Add to your project using ```Project Generator```:  
* [ofxImGui](https://github.com/moebiussurfing/ofxImGui_v5)
* ofxPoco  
* ofxXmlSettings
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)  
* ofxUI [ not required but implemented as GUI alternative to *ImGui* ]  

Already included into ```OF_ADDON/libs/```. No need to add:  
* [ofxColourLovers](https://github.com/borg/ofxColourLovers)  
* [ofxHttpUtils](https://github.com/arturoc/ofxHttpUtils)

## Tested systems
- **Windows10** / **VS2017** / **OF ~0.11**
- **macOS High Sierra** / **Xcode 9/10** / **OF ~0.11**

## Author
Addon by **@moebiusSurfing**  
*(ManuMolina). 2020.*  
Thanks to **borg**, **arturoc** and **MacFurax** for the repositories.  

## License
*MIT License.*