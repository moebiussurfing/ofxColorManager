#pragma once

#include "ofMain.h"

class TextBox {

public:

    TextBox();
    ~TextBox();

    void setup();
    void update();
    void draw();//draw text block
    void exit();

    void setPosition(glm::vec2 p);
    glm::vec2 getPosition();
    void setSize(glm::vec2 s);
    glm::vec2 getSize();

    void setFontSize(int _fontSize);
    void setFont(ofTrueTypeFont font);
    void setFontPath(std::string fontPath);
    void setText(std::string text);
    void setColorText(ofColor c);
    void setColorBack(ofColor c);

    void drawText(glm::vec2 pos, std::string text, bool withShadow);
    //draw one standalone line
    void drawText(glm::vec2 _pos, std::string _text, ofColor c);

    void setName(std::string name);
    std::string getName();

    void setVisible(bool visible);
    bool isVisible();

private:

    std::string textBox_name;
    glm::vec2 pos;
    glm::vec2 size;
    std::string fontPath;
    ofTrueTypeFont font;
    int fontSize;
    std::string text;
    ofColor colorText;
    ofColor colorBack;
    bool bUseShadow;
    int shadowPad;
    bool bVisible;

};