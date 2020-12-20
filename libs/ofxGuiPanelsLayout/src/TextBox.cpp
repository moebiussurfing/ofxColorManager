#include "TextBox.h"

TextBox::TextBox()
{
    fontPath = "fonts/mono.ttf";
    fontSize = 10;
    pos = glm::vec2 ( 10, 10);
    size = glm::vec2 ( 200, 300);
    text = "";
    colorText = ofColor::white;
    colorBack = ofColor::black;
    bUseShadow = true;
    shadowPad = -1;
    textBox_name = "";
    bVisible = true;
}

TextBox::~TextBox()
{
}

void TextBox::setup()
{
    font.load(fontPath, fontSize, true, true);
}

void TextBox::update()
{
}

void TextBox::draw()
{
    if (bVisible)
    {
        ofPushStyle();
        if (bUseShadow)
        {
            ofSetColor(colorBack);
            font.drawString(text, pos.x + shadowPad, pos.y + fontSize);
        }
        ofSetColor(colorText);
        font.drawString(text, pos.x, pos.y + fontSize);
        ofPopStyle();
    }
}

//custom color without shadow
void TextBox::drawText(glm::vec2 _pos, std::string _text, bool withShadow)
{
    ofPushStyle();
    if (withShadow)
    {
        ofSetColor(colorBack);
        font.drawString(_text, _pos.x + shadowPad, _pos.y + fontSize);
    }
    ofSetColor(colorText);
    font.drawString(_text, _pos.x, _pos.y + fontSize);
    ofPopStyle();
}

void TextBox::drawText(glm::vec2 _pos, std::string _text, ofColor c)
{
    ofPushStyle();
    ofSetColor(c);
    font.drawString(_text, _pos.x, _pos.y + fontSize);
    ofPopStyle();
}

void TextBox::setPosition(glm::vec2 p)
{
    pos = p;
}

void TextBox::setSize(glm::vec2 s)
{
    size = s;
}

glm::vec2 TextBox::getPosition()
{
    return pos;
}

glm::vec2 TextBox::getSize()
{
    return size;
}

void TextBox::setFontSize(int _fontSize)
{
    fontSize = _fontSize;
    font.load(fontPath, fontSize, true, true);
}

void TextBox::setFontPath(std::string _fontPath)
{
    fontPath = _fontPath;
    font.load(fontPath, fontSize, true, true);
}

void TextBox::setFont(ofTrueTypeFont _font)
{
    font = _font;
}

void TextBox::setText(std::string _text)
{
    text = _text;
}

void TextBox::setColorText(ofColor c)
{
    colorText = c;
}

void TextBox::setColorBack(ofColor c)
{
    colorBack = c;
}

void TextBox::setName(std::string name)
{
    textBox_name = name;
}

std::string TextBox::getName()
{
    return textBox_name;
}

void TextBox::setVisible(bool visible)
{
    bVisible = visible;
}

bool TextBox::isVisible()
{
    return bVisible;
}

void TextBox::exit()
{
}