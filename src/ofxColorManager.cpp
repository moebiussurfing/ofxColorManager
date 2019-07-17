#include "ofxColorManager.h"

ofxColorManager::ofxColorManager()
{
}

ofxColorManager::~ofxColorManager()
{
}

void ofxColorManager::setup()
{
    // Gui
    this->gui.setup();
    this->guiVisible = true;

    color_backGround.set (ofColor::white);

    mode = ofxColorPalette::BRIGHTNESS;
    brightness = 200;
    saturation = 200;

    BRIGHTNESS.set("BRIGHTNESS", brightness, 0, 255 );
    SATURATION.set("SATURATION", saturation, 0, 255 );

    MODE.set("MODE", false);

}

//--------------------------------------------------------------
bool ofxColorManager::imGui()
{
    auto mainSettings = ofxImGui::Settings();

    this->gui.begin();
    {
        if (ofxImGui::BeginWindow("Helpers", mainSettings, false))
        {
            ofxImGui::AddParameter(this->myColor, true);
            ofxImGui::AddParameter(this->MODE);
            ofxImGui::AddParameter(this->SATURATION);
            ofxImGui::AddParameter(this->BRIGHTNESS);

        }
        ofxImGui::EndWindow(mainSettings);
    }
    this->gui.end();

    return mainSettings.mouseOverGui;
}

void ofxColorManager::update()
{
    if (MODE.get())
    {
        mode = ofxColorPalette::SATURATION;
    }
    else
    {
        mode = ofxColorPalette::BRIGHTNESS;
    }

    update_color(BRIGHTNESS, SATURATION);
}

void ofxColorManager::update_color(int brg, int sat)
{
    switch (mode) {
        case ofxColorPalette::BRIGHTNESS:
//            brightness = ofMap(ofGetMouseY(), 0, ofGetHeight(), 0, 255);
            brightness = brg;
            break;
        case ofxColorPalette::SATURATION:
//            saturation = ofMap(ofGetMouseY(), 0, ofGetHeight(), 0, 255);
            saturation = sat;
            break;
        default:
            break;
    };

//    ofColor base = ofColor::fromHsb(ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 255), saturation, brightness);
//    ofColor c = (ofColor) (myColor.get());
    ofColor base = ofColor::fromHsb(ofMap(myColor.get().getHue(), 0., 1., 0, 255), saturation, brightness);

    complement.setBaseColor(base);
    complementBrightness.setBaseColor(base);
    triad.setBaseColor(base);
    complementTriad.setBaseColor(base);
    monochrome.setBaseColor(base);
    monochromeBrightness.setBaseColor(base);
    analogue.setBaseColor(base);
    random.setBaseColor(base);


    complement.generateComplementary();
    complementBrightness.generateComplementary(ofxColorPalette::BRIGHTNESS);
    triad.generateTriad();
    complementTriad.generateComplementaryTriad();
    monochrome.generateMonoChromatic();
    monochromeBrightness.generateMonoChromatic(ofxColorPalette::BRIGHTNESS);
    analogue.generateAnalogous();
}

void ofxColorManager::draw()
{
    // We have to use ofParameter::get() since this we are using an ofFloatColor.
//    ofClear(this->myColor.get());
    ofClear(color_backGround);

    //-

    // Gui
    this->mouseOverGui = false;
    if (this->guiVisible)
    {
        this->mouseOverGui = this->imGui();
    }
    if (this->mouseOverGui)
    {
    }
    else
    {
    }

    //--

    ofPushMatrix();
    ofTranslate(600, 400);
    int padTxt = 200;

    //--begin
    ofPushMatrix();
    for (int i = 0 ; i < triad.size(); i++) {
        ofSetColor(triad[i]);
        ofRect(0, 0, RECT_SIZE, RECT_SIZE);
        ofTranslate(RECT_SIZE, 0);
    }
    ofPopMatrix();
    ofPushMatrix();
    ofTranslate(padTxt, RECT_SIZE /2);
    ofSetColor(0);
    ofDrawBitmapString("Triad", 0, 0);
    ofPopMatrix();
    ofTranslate(0, RECT_SIZE + PADDING);
    // ----
    ofPushMatrix();
    for (int i = 0 ; i < complementTriad.size(); i++) {
        ofSetColor(complementTriad[i]);
        ofRect(0, 0, RECT_SIZE, RECT_SIZE);
        ofTranslate(RECT_SIZE, 0);
    }
    ofPopMatrix();
    ofPushMatrix();
    ofTranslate(padTxt, RECT_SIZE /2);
    ofSetColor(0);
    ofDrawBitmapString("Complement Triad", 0, 0);
    ofPopMatrix();
    ofTranslate(0, RECT_SIZE + PADDING);
    // ----
    ofPushMatrix();
    for (int i = 0 ; i < complement.size(); i++) {
        ofSetColor(complement[i]);
        ofRect(0, 0, RECT_SIZE, RECT_SIZE);
        ofTranslate(RECT_SIZE, 0);
    }
    ofPopMatrix();
    ofPushMatrix();
    ofTranslate(padTxt, RECT_SIZE /2);
    ofSetColor(0);
    ofDrawBitmapString("Complement (Saturation)", 0, 0);
    ofPopMatrix();
    ofTranslate(0, RECT_SIZE + PADDING);
    // ----
    ofPushMatrix();
    for (int i = 0 ; i < complementBrightness.size(); i++) {
        ofSetColor(complementBrightness[i]);
        ofRect(0, 0, RECT_SIZE, RECT_SIZE);
        ofTranslate(RECT_SIZE, 0);
    }
    ofPopMatrix();
    ofPushMatrix();
    ofTranslate(padTxt, RECT_SIZE /2);
    ofSetColor(0);
    ofDrawBitmapString("Complement (Brightness)", 0, 0);
    ofPopMatrix();
    ofTranslate(0, RECT_SIZE + PADDING);
    // ----
    ofPushMatrix();
    for (int i = 0 ; i < monochrome.size(); i++) {
        ofSetColor(monochrome[i]);
        ofRect(0, 0, RECT_SIZE, RECT_SIZE);
        ofTranslate(RECT_SIZE, 0);
    }
    ofPopMatrix();
    ofPushMatrix();
    ofTranslate(padTxt, RECT_SIZE /2);
    ofSetColor(0);
    ofDrawBitmapString("Monochrome (Saturation)", 0, 0);
    ofPopMatrix();
    ofTranslate(0, RECT_SIZE + PADDING);
    // ----
    ofPushMatrix();
    for (int i = 0 ; i < monochromeBrightness.size(); i++) {
        ofSetColor(monochromeBrightness[i]);
        ofRect(0, 0, RECT_SIZE, RECT_SIZE);
        ofTranslate(RECT_SIZE, 0);
    }
    ofPopMatrix();
    ofPushMatrix();
    ofTranslate(padTxt, RECT_SIZE /2);
    ofSetColor(0);
    ofDrawBitmapString("Monochrome (Brightness)", 0, 0);
    ofPopMatrix();
    ofTranslate(0, RECT_SIZE + PADDING);
    // ----
    ofPushMatrix();
    for (int i = 0 ; i < analogue.size(); i++) {
        ofSetColor(analogue[i]);
        ofRect(0, 0, RECT_SIZE, RECT_SIZE);
        ofTranslate(RECT_SIZE, 0);
    }
    ofPopMatrix();
    ofPushMatrix();
    ofTranslate(padTxt, RECT_SIZE /2);
    ofSetColor(0);
    ofDrawBitmapString("Analogue", 0, 0);
    ofPopMatrix();
    ofTranslate(0, RECT_SIZE + PADDING);
    // ----
    ofPushMatrix();
    for (int i = 0 ; i < random.size(); i++) {
        ofSetColor(random[i]);
        ofRect(0, 0, RECT_SIZE, RECT_SIZE);
        ofTranslate(RECT_SIZE, 0);
    }
    ofPopMatrix();
    ofPushMatrix();
    ofTranslate(padTxt, RECT_SIZE /2);
    ofSetColor(0);
    ofDrawBitmapString("Random (Click to regenerate)", 0, 0);
    ofPopMatrix();

    ofTranslate(10, RECT_SIZE+PADDING);
    string info = MODE.get() ? "saturation" : "brightness";
    info += "\nhue:" + ofToString(complement[0].getHue()) + "\n";
    string modeString;
    float modeValue;
    if(mode == ofxColorPalette::SATURATION){
        modeString = " (saturation)";
        modeValue = saturation;
    }else {
        modeString = " (brightness)";
        modeValue = brightness;
    }
    info += " " + modeString + ofToString(modeValue) +  "\n";
    ofDrawBitmapString(info,0,0);

    ofPopMatrix();
    //--
}

void ofxColorManager::exit()
{
}

void ofxColorManager::keyPressed( ofKeyEventArgs& eventArgs ) 
{    
	const int & key = eventArgs.key;	
    cout << "key: " << key << endl;

    if(key == ' ')
	{
	}

    if(key == 'g')
    {
        this->guiVisible = ! this->guiVisible;
    }

    //--

    if (key == 's')
    {
        mode = ofxColorPalette::SATURATION;
    }
    else if (key == 'b')
    {
        mode = ofxColorPalette::BRIGHTNESS;
    }
    else if (key == 'r') {
        random.generateRandom();
    }

    //-
}

void ofxColorManager::keyReleased( ofKeyEventArgs& eventArgs ) 
{
    if( eventArgs.key == ' ') 
    {
	}
}

void ofxColorManager::mousePressed( int x, int y ) 
{
}
