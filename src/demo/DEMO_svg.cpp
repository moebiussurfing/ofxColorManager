#include "DEMO_svg.h"

//--------------------------------------------------------------
void DEMO_svg::setup() {

	path = "";
	img.load(path + "moebius-lines.jpg");
	svg.load(path + "moebius-giraud.svg");

	ofLogNotice(__FUNCTION__) << svg.toString();

	ofRectangle r = svg.getBounds();

	shape = glm::vec2(r.getWidth(), r.getHeight());

	img.resize(r.getWidth(), r.getHeight());

	psBlend.setup(r.getWidth(), r.getHeight());
	//psBlend.setup(img.getWidth(), img.getHeight());

	blendMode = 1;//multiply

	paletteSvg.clear();
	paletteSvg.resize(maxNumColors);

	//initializeColors();
}

//--------------------------------------------------------------
void DEMO_svg::setPaletteColors(vector<ofColor> &palette) {

	int _max = MAX(paletteSvg.size(), palette.size());

	//TODO:

	if (paletteSvg.size() < palette.size())
	{
		for (int i = 0; i < _max; i++)
		{
			int i2 = i % paletteSvg.size();
			paletteSvg[i] = palette[i2];

		}
	}
	else if (palette.size() < paletteSvg.size())
	{
		for (int i = 0; i < _max; i++)
		{
			int i2 = i % palette.size();
			paletteSvg[i] = palette[i2];
		}
	}

	//-

	//int isvg;
	//if (paletteSvg.size() > palette.size())
	//{
	//	isvg = iplt % palette.size();
	//}
	//else if (paletteSvg.size() <= palette.size())
	//{
	//	isvg = iplt;
	//}

	//paletteSvg[isvg] = palette[iplt];
	////TODO: fix ranges...
	//if (paletteSvg.size() < palette.size())
	//{
	//	for (int isvg = palette.size(); isvg < palette.size(); isvg++)
	//	{
	//		int iplt = isvg % palette.size();
	//		paletteSvg[isvg] = palette[iplt];
	//	}
	//}

	////swap all original file colors
	//paletteSvg[0] = ofColor::red;
	//paletteSvg[1] = ofColor::yellow;
	//paletteSvg[2] = ofColor::orange;
	//paletteSvg[3] = ofColor::blueSteel;
	//paletteSvg[4] = ofColor::darkSlateGrey;
	//paletteSvg[5] = ofColor::darkOliveGreen;
	//paletteSvg[6] = ofColor::paleVioletRed;
	//paletteSvg[7] = ofColor::deepSkyBlue;
}

//--------------------------------------------------------------
void DEMO_svg::update() {
	//float val = (1 + sin(2 * 3.1416 * ofGetElapsedTimef())) * 0.5;
	//ofLogNotice(__FUNCTION__) << val;

	for (int i = 0; i < maxNumColors; i++)
	{
		std::string name = "group" + ofToString(i);

		ofColor c = paletteSvg[i];

		//if (i == 0) c.setHue(128 + val * 128);
		//else if (i % 2 == 0) c.setBrightness(128 + val * 128.0);
		//else if (i % 2 != 0) c.setSaturation(val * 255.0);

		shared_ptr<ofxSvgGroup> bGroup = svg.get< ofxSvgGroup>(name.c_str());

		if (bGroup)
		{
			for (int e = 0; e < bGroup->getElements().size(); e++)
			{
				shared_ptr< ofxSvgElement > te = dynamic_pointer_cast<ofxSvgElement>(bGroup->getElements()[e]);

				te->path.setFillColor(c);
			}
		}
	}

	//-

	psBlend.begin();
	svg.draw();
	psBlend.end();
}

//--------------------------------------------------------------
//void DEMO_svg::draw(glm::vec2 _pos, glm::vec2 _shape = glm::vec2(838, 1080)) {
void DEMO_svg::draw(glm::vec2 _pos) {
	//ofClear(255);
	//update();

	pos = _pos;

	ofPushMatrix();
	ofPushStyle();

	//ofSetColor(255, 255);
	ofTranslate(pos.x, pos.y);
	psBlend.draw(img.getTextureReference(), blendMode);

	ofPopStyle();
	ofPopMatrix();
}

//--------------------------------------------------------------
void DEMO_svg::draw() {
	//ofClear(255);
	//update();

	psBlend.draw(img.getTextureReference(), blendMode);

	//string str;
	//str += "Press UP/DOWN key\n";
	//str += "Blend Mode: [" + ofToString(blendMode) + "] " + psBlend.getBlendMode(blendMode);
	//ofDrawBitmapStringHighlight(str, 20, 30);
}

//--------------------------------------------------------------
void DEMO_svg::keyPressed(int key) {
	if (key == OF_KEY_UP)
	{
		if (blendMode >= 24)
		{
			blendMode = 0;
		}
		else
		{
			blendMode++;
		}
	}
	if (key == OF_KEY_DOWN || key == ' ')
	{
		if (blendMode <= 0)
		{
			blendMode = 24;
		}
		else
		{
			blendMode--;
		}
	}
}