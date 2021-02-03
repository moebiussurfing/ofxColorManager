#include "DEMO_Svg.h"

//--------------------------------------------------------------
void DEMO_Svg::setup() {
	ofLogNotice(__FUNCTION__);

	path = "svg/";
	img.load(path + "moebius-lines.jpg");
	svg.load(path + "moebius-giraud.svg");

	ofLogNotice(__FUNCTION__) << svg.toString();

	rSvgBounds = svg.getBounds();

	//TODO:
	//scale
	//svg.getTransformFromSvgMatrix
	//getTransformFromSvgMatrix(string matrix, ofVec2f& apos, float & scaleX, float & scaleY, float & arotation)
	//vector< shared_ptr<ofxSvgImage> > trees = svg.getElementsForType< ofxSvgImage>("trees");

	shape = glm::vec2(rSvgBounds.getWidth(), rSvgBounds.getHeight());
	//ratio = shape.x / shape.y;

	img.resize(rSvgBounds.getWidth(), rSvgBounds.getHeight());

	psBlend.setup(rSvgBounds.getWidth(), rSvgBounds.getHeight());
	//psBlend.setup(img.getWidth(), img.getHeight());

	blendMode = 1;//multiply

	paletteSvg.clear();
	paletteSvg.resize(maxNumColors);

	//initializeColors();

	// draggeble rect
	rectDgSvg.setRect(rSvgBounds.getX(), rSvgBounds.getY(), rSvgBounds.getWidth(), rSvgBounds.getHeight());//default init
	rectDgSvg.setLockResize(false);
	//rectDgSvg.setLockResize(true);

	// b. load settings
	//rectDgSvg.loadSettings();
	path_Layout = path;
	rectDgSvg.loadSettings(path_Name, path_Layout, false);
	rectDgSvg.disableEdit();
	//rectDgSvg.enableEdit();

	//_xx = rectDgSvg.getX() + _pad1;
	//_yy = rectDgSvg.getY() + _pad1;
}

//--------------------------------------------------------------
void DEMO_Svg::setPaletteColors(vector<ofColor> &_palette) {
	ofLogNotice(__FUNCTION__);

	if (_palette.size() > 0)
	{
		int _max = MAX(paletteSvg.size(), _palette.size());

		//TODO:

		if (paletteSvg.size() < _palette.size())
		{
			for (int i = 0; i < _max; i++)
			{
				int i2 = i % paletteSvg.size();
				paletteSvg[i] = _palette[i2];

			}
		}
		else if (_palette.size() < paletteSvg.size())
		{
			for (int i = 0; i < _max; i++)
			{
				int i2 = i % _palette.size();
				paletteSvg[i] = _palette[i2];
			}
		}
	}
}

//--------------------------------------------------------------
void DEMO_Svg::update() {
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
void DEMO_Svg::draw(glm::vec2 _pos) //force pos
{
	pos = _pos;
	rectDgSvg.setPosition(pos.x, pos.y);
	draw();
}

//--------------------------------------------------------------
void DEMO_Svg::draw()
{
	//blend types
	//string str;
	//str += "Press UP/DOWN key\n";
	//str += "Blend Mode: [" + ofToString(blendMode) + "] " + psBlend.getBlendMode(blendMode);
	//ofDrawBitmapStringHighlight(str, 20, 30);

	ofPushStyle();

	ofPushMatrix();
	ofTranslate(rectDgSvg.getX(), rectDgSvg.getY());
	
	//ofPushMatrix();
	//ofTranslate(rectDgSvg.getWidth() / 2.f, rectDgSvg.getHeight()/2.0f);
	ofScale(scale);

	psBlend.draw(img.getTextureReference(), blendMode);
	
	ofPopMatrix();
	//ofPopMatrix();

	if (rectDgSvg.isEditing())
	{
		////highlight
		//ofFill();
		//int fr = ofGetFrameNum() % 60;
		//ofSetColor(fr < 30 ? ofColor(255, 16) : (ofColor(0, 16)));
		//ofDrawRectangle(rectDgSvg);

		//-

		rectDgSvg.setWidth(rSvgBounds.getWidth() * scale);
		rectDgSvg.setHeight(rSvgBounds.getHeight() * scale);

		//float _ratio = rectDgSvg.getHeight() / rectDgSvg.getHeight();
		//scale = rectDgSvg.getY() / rSvgBounds.getY();
	}

	ofPopStyle();
}

//--------------------------------------------------------------
void DEMO_Svg::keyPressed(int key)
{
	ofLogNotice(__FUNCTION__) << key;

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