#include "DEMO_Svg.h"

//--------------------------------------------------------------
void DEMO_Svg::setup() {
	ofLogNotice(__FUNCTION__);

	path = "svg/";

	//-

	params.add(DEMO2_Test);
	params.add(DEMO2_Edit);
	params.add(DEMO2_Scale);
	params.add(DEMO2_Alpha);

	ofAddListener(params.parameterChangedE(), this, &DEMO_Svg::Changed_Controls);

	//--

	//img.load(path + "moebius/moebius-lines.jpg");
	//svg.load(path + "moebius/moebius-giraud.svg");
	//shape = glm::vec2(838, 1080);

	img.load(path + "nike/nike.jpg");
	svg.load(path + "nike/nike.svg");
	shape = glm::vec2(1024, 666);

	//--

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

	// draggable rect
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
void DEMO_Svg::mouseScrolled(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const float &scrollX = eventArgs.scrollX;
	const float &scrollY = eventArgs.scrollY;
	ofLogNotice(__FUNCTION__) << "scrollX: " << scrollX << "  scrollY: " << scrollY;

	if (DEMO2_Edit)
	{
		if (rectDgSvg.inside(glm::vec2(x, y)))//zoom 
			if (scrollY == 1) DEMO2_Scale += 0.025f;
			else if (scrollY == -1) DEMO2_Scale -= 0.025f;
	}
}

//--------------------------------------------------------------
void DEMO_Svg::Changed_Controls(ofAbstractParameter &e)
{
	std::string name = e.getName();

	ofLogNotice(__FUNCTION__) << name << " : " << e;

	//----

	if (false) {}

	//----

	// demo2 svg
	else if (name == DEMO2_Edit.getName())
	{
		setEdit(DEMO2_Edit);
	}
	else if (name == DEMO2_Scale.getName())
	{
		setScale(DEMO2_Scale);
	}
	else if (name == DEMO2_Alpha.getName())
	{
		setAlpha(DEMO2_Alpha);
	}
}

//--------------------------------------------------------------
void DEMO_Svg::update() {
	//float val = (1 + sin(2 * 3.1416 * ofGetElapsedTimef())) * 0.5;
	//ofLogNotice(__FUNCTION__) << val;

	// DEMO2
	//if (DEMO2_Test) update();

	//--

	for (int i = 0; i < maxNumColors; i++)
	{
		std::string name = "group" + ofToString(i);

		ofColor c = ofColor(paletteSvg[i], alpha * 255.0);

		//if (i < paletteSvg.size())
		//{
		//	ofColor c = ofColor(paletteSvg[i], alpha * 255.0);
		//}
		//else
		//{
		//	ofColor c = ofColor(paletteSvg[i % paletteSvg.size()], alpha * 255.0);
		//}

		//ofColor c = paletteSvg[i];

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

	if (DEMO2_Test)
	{
		update();

		//-

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