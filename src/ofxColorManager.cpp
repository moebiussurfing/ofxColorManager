#include "ofxColorManager.h"

//--------------------------------------------------------------
ofxColorManager::ofxColorManager()
{
}

//--------------------------------------------------------------
ofxColorManager::~ofxColorManager()
{
}

//--------------------------------------------------------------
void ofxColorManager::setup_UI()
{
/******
     * setup the scene with width and height
     */
    scene = new Node();
    scene->setSize(ofGetWidth(), ofGetHeight());
    scene->setName("Scene");

/******
 * this is the touch manager, give him the scene to set it up
 */
    TouchManager::one().setup(scene);
}

//--------------------------------------------------------------
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

    MODE.set("SWITCH MODE", false);

    //-

    setup_UI();

    gradient.reset();

    //-

    addKeysListeners();
    addMouseListeners();
}

//--------------------------------------------------------------
void ofxColorManager::addColorUI(ofColor c)
{
    float size = 40;
    float pad = 40;
    int perRow = 10;

    int i = buttons.size();

//    for (int i=0; i<perRow*4; i++) {

    // setup them in a grid
    float x = 150 + (i%perRow)*(size+pad);
    float y = 200 + (i/perRow)*(size+pad);

    // create a ButtonExample node
    ButtonExample *btn = new ButtonExample();
    btn->setup(x, y, size, size);
    btn->setColor(c);

    btn->setName("btn" + ofToString(i));

    // add it to the scene
    scene->addChild(btn);

    if (i%perRow>0) {
        // this can be called to place nodes next to each other
        btn->placeNextTo(*buttons[i-1], Node::RIGHT);
    }

    // keep reference (we need it to update the nodes)
    buttons.push_back(btn);
//    }


}

//--------------------------------------------------------------
void ofxColorManager::update_UI(){

    /******
     * update the touch manager,
     */
    TouchManager::one().update();


    /******
     * update the nodes (optional, only if you have an update)
     */
    for (int i=0; i<buttons.size(); i++) {
        buttons[i]->update();
    }
}

//--------------------------------------------------------------
void ofxColorManager::draw_UI(){
    /******
     * this will render the scene
     * 1. this function takes all the visible nodes
     * 2. sort them by 'plane' float
     * 3. transform into the local space of each node
     * 4. calls the 'draw' function on each node
     */
    scene->render();

    /******
     * if you want debug rendering
     */
    if (bShowDebug) {
        scene->renderDebug();
    }


    ofSetColor(255);
    ofDrawBitmapString("hit 'd' to toggle debug rendering", 5, ofGetHeight()-8);
}

//--------------------------------------------------------------
bool ofxColorManager::imGui()
{
    auto mainSettings = ofxImGui::Settings();

    this->gui.begin();
    {
        if (ofxImGui::BeginWindow("COLOR MANAGER", mainSettings, false))
        {
            ofxImGui::AddParameter(this->myColor, true);
            ofxImGui::AddParameter(this->MODE);

            if ( MODE )
            {
                ImGui::Text( "MODE SATURATION" );
                ofxImGui::AddParameter(this->SATURATION);
            }
            else
            {
                ImGui::Text( "MODE BRIGHTNESS" );
                ofxImGui::AddParameter(this->BRIGHTNESS);
            }

            if (ImGui::Button("ADD COLOR"))
            {
                add_color(ofColor(myColor.get()));
            }

        }
        ofxImGui::EndWindow(mainSettings);
    }
    this->gui.end();

    return mainSettings.mouseOverGui;
}

//--------------------------------------------------------------
void ofxColorManager::add_color(ofColor c)
{
    palette.push_back( c );
    ofLogNotice("ofxColorManager") << "added color " << ofToString(palette.size()) << " (" << ofToString(c) << ") to palette";

    addColorUI(c);

    gradient.addColor( c );
}

//--------------------------------------------------------------
void ofxColorManager::update()
{
    update_UI();

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

//--------------------------------------------------------------
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

//--------------------------------------------------------------
void ofxColorManager::draw()
{
    // We have to use ofParameter::get() since this we are using an ofFloatColor.
//    ofClear(this->myColor.get());
    ofClear(color_backGround);

    //-

    draw_UI();

    gradient.drawDebug(50, 700, 200, 40);

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

//--------------------------------------------------------------
void ofxColorManager::exit()
{
    removeKeysListeners();
    removeMouseListeners();
}

//--------------------------------------------------------------
void ofxColorManager::keyPressed( ofKeyEventArgs& eventArgs )
{
    const int & key = eventArgs.key;
    cout << "key: " << key << endl;

    if(key == 'd')
    {
        bShowDebug = !bShowDebug;
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

//--------------------------------------------------------------
void ofxColorManager::keyReleased( ofKeyEventArgs& eventArgs )
{
    if( eventArgs.key == ' ')
    {
    }
}

//--------------------------------------------------------------
void ofxColorManager::addKeysListeners()
{
    ofAddListener( ofEvents().keyPressed, this, &ofxColorManager::keyPressed );
}

//--------------------------------------------------------------
void ofxColorManager::removeKeysListeners()
{
    ofRemoveListener( ofEvents().keyPressed, this, &ofxColorManager::keyPressed );
}

//--------------------------------------------------------------
void ofxColorManager::mouseDragged(ofMouseEventArgs& eventArgs){
    const int & x = eventArgs.x;
    const int & y = eventArgs.y;
    const int & button = eventArgs.button;
    ofLogNotice("ofxColorManager") << "mouseDragged " <<  x << ", " << y << ", " << button;

    /******
     * pass touch/mouse events to the touch manager
     */
    TouchManager::one().touchMove(button, ofVec2f(x, y));
}

//--------------------------------------------------------------
void ofxColorManager::mousePressed(ofMouseEventArgs& eventArgs){
    const int & x = eventArgs.x;
    const int & y = eventArgs.y;
    const int & button = eventArgs.button;
    ofLogNotice("ofxColorManager") << "mousePressed " <<  x << ", " << y << ", " << button;

    /******
     * pass touch/mouse events to the touch manager
     */
    TouchManager::one().touchDown(button, ofVec2f(x, y));
}

//--------------------------------------------------------------
void ofxColorManager::mouseReleased(ofMouseEventArgs& eventArgs){
    const int & x = eventArgs.x;
    const int & y = eventArgs.y;
    const int & button = eventArgs.button;
    ofLogNotice("ofxColorManager") << "mouseReleased " <<  x << ", " << y << ", " << button;

    /******
     * pass touch/mouse events to the touch manager
     */
    TouchManager::one().touchUp(button, ofVec2f(x, y));
}

//--------------------------------------------------------------
void ofxColorManager::addMouseListeners()
{
    ofAddListener( ofEvents().mouseDragged, this, &ofxColorManager::mouseDragged );
    ofAddListener( ofEvents().mousePressed, this, &ofxColorManager::mousePressed );
    ofAddListener( ofEvents().mouseReleased, this, &ofxColorManager::mouseReleased );

}

//--------------------------------------------------------------
void ofxColorManager::removeMouseListeners()
{
    ofRemoveListener( ofEvents().keyPressed, this, &ofxColorManager::keyPressed );
}