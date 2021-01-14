#pragma once
#include "ofMain.h"

namespace ofxColorClient
{
	class PalettePreviewMini
	{
		ofTrueTypeFont font;

	public:
		PalettePreviewMini()
		{
			setup();
		};

		~PalettePreviewMini() {};

		void setup() {
			font.load("assets/fonts/telegrama_render.otf", 11, true, true, true);
		}

		void draw() {};

		//--------------------------------------------------------------
		inline void draw_MiniPreview(const std::string PRESET_Name, vector<ofColor> &palette, const ofColor color_BackGround)
			//inline void draw_MiniPreview(ofTrueTypeFont &font, const std::string PRESET_Name, vector<ofColor> &palette, const ofColor color_BackGround)
		{
			ofPushMatrix();
			ofPushStyle();
			{
				int _x = 30;
				int _y = 30;
				glm::vec2 _pos;
				int _sz = 60;//size boxes
				int _p = 3;//pad between colors
				int _sz2 = _sz + _p;
				int _pad = 35;
				int _pad2 = 35;
				int _padlabel = 10;
				int _hBg = 15;
				float _round2 = 3;
				float _round = 4;
				ofRectangle _rBg;
				ofColor colorBackground{ 0, 225 };//bg box
				ofColor _cb = ofColor(ofColor::black, 64);//border color black
				//ofColor _cb = ofColor(ofColor::white, 64);//border color white

				//-

				// 1. left top corner
				_pos = glm::vec2(_x, _y);
				//_pos = glm::vec2(_x, _y + _pad2);
				//// 2. right top corner
				//_pos = glm::vec2(ofGetWidth() - palette.size()*_sz2, 2*_p);

				ofTranslate(_pos);

				//-

				//prepare bg box
				ofRectangle _r{
					0, 0,
					float(palette.size() * _sz2 - _p), float(_pad2 + _sz2 + _hBg) };
				_r.setHeight(_r.getHeight() + _pad);
				_r.setX(_r.getPosition().x - _pad / 2.);
				_r.setY(_r.getPosition().y - _pad / 2.);

				//-

				// preset name

				//label + name
				std::string s = PRESET_Name;
				ofColor font0_Color{ 255, 255 };
				float _ww2 = font.getStringBoundingBox(s, 0, 0).getWidth();

				//draw bg box
				_r.setWidth(MAX(_r.getWidth() + _pad, _ww2 + _pad));
				ofSetColor(colorBackground);
				ofFill();
				ofDrawRectRounded(_r, _round);

				//draw text
				ofSetColor(font0_Color);
				font.drawString(s, _padlabel, 0 + _pad * 0.5);

				//-

				ofTranslate(0, 35);

				//3. palette colors
				for (int col = 0; col < palette.size(); col++)
				{
					_r = ofRectangle(col * _sz2, 0, _sz, _sz);
					ofFill();
					ofSetColor(palette[col]);
					ofDrawRectRounded(_r, _round2);
					//ofDrawRectangle(_r);
					ofNoFill();
					ofSetColor(_cb);
					ofDrawRectRounded(_r, _round2);
					//ofDrawRectangle(_r);
				}

				ofTranslate(0, _sz + _p);

				//1. background color box
				_rBg = ofRectangle(0, 0, palette.size() * _sz2 - _p, _hBg);
				ofFill();
				ofSetColor(color_BackGround);
				//ofSetColor(color_BackGround.get());
				ofDrawRectRounded(_rBg, _round2);
				//ofDrawRectangle(_rBg);
				ofNoFill();
				ofSetColor(_cb);
				ofDrawRectRounded(_rBg, _round2);
				//ofDrawRectangle(_rBg);
			}
			ofPopStyle();
			ofPopMatrix();
		}

	};
};

