# ofxOpenColor

openFrameworks port of Open color.

> Open color is an 
open-source color scheme optimized for UI like font, background, border, etc.

Open color goals:

- All colors shall have adequate use
- Provide general color for UI design
- All colors will be beautiful in itself and harmonious
- At the same brightness level, the perceived brightness will be constant

## Available colors

![available colors](https://yeun.github.io/open-color/asset/images/open-color.svg)

## Usage

Include ```ofxOpenColor.h``` wherever you would like to use the Open color palette, typically ```ofApp.h```. ofxOpenColor follows the naming and usage patterns of the original naming convention as closely as possible.

```c++
// oc_(color)_(number)
// For example:
ofBackground(oc_gray_9);
```
Colors are also accessible as arrays, although the naming is likely to change in the future.

```c++
// oc_(color)_[number]
// For example:
ofSetColor(oc_blue_[6]);
```
