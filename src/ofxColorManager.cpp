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
void ofxColorManager::setup_Interface()
{
    scene = new Node();
    scene->setSize(ofGetWidth(), ofGetHeight());
    scene->setName("Scene");
    TouchManager::one().setup(scene);
}

//--------------------------------------------------------------
void ofxColorManager::setup()
{
    //-

    // DATA
    color_backGround.set("BACKGROUND", ofFloatColor::white);
    params_data.setName("DATA");
    params_data.add(color_backGround);

    //-

    // COLOR
    myColor.set("COLOR", ofFloatColor::black);
    params_color.setName("COLOR");
    params_color.add(myColor);

    //-

    // ALGORITHMIC PALETTE
//    brightness = 128;
//    saturation = 128;

    BRIGHTNESS.set("BRIGHTNESS", 128, 0, 255 );
    SATURATION.set("SATURATION", 128, 0, 255 );
    bRandomPalette.set("RANDOMIZE", false);

    params_palette.setName("ALGORITHMIC PALETTE");
    params_palette.add(BRIGHTNESS);
    params_palette.add(SATURATION);
    params_palette.add(bRandomPalette);

    //-

    // INTERFACE

    setup_Interface();

    //-

    // PALETTES

//    setup_palettes();
    update_palettes();
    setup_palettes();

    //-

    // GRADIENT

    gradient.reset();

    //-

    // CURVE

    setup_curveTool();

    //-

    // GUI

    this->gui.setup();
    this->guiVisible = true;

    //-

    addKeysListeners();
    addMouseListeners();
}

//--------------------------------------------------------------
void ofxColorManager::add_color_Interface(ofColor c)
{
    float x = 150;
    float y = 400;

    float size = 40;
    float pad = 40;
    int perRow = 10;

    int i = buttons.size();

//    for (int i=0; i<perRow*4; i++) {

    // setup them in a grid
    x += (i%perRow)*(size+pad);
    y += (i/perRow)*(size+pad);

    // create a ButtonExample node
    ButtonExample *btn = new ButtonExample();
    btn->setup(x, y, size, size);
    btn->setColor(c);
    btn->setup_colorBACK( color_p );
    btn->setLocked(true);
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
void ofxColorManager::update_Interface(){

    TouchManager::one().update();

    // COLORS OF CURRENT PALETTE
    for (int i=0; i<buttons.size(); i++)
    {
        buttons[i]->update();
    }

//    // PALETTES
//    for (int i=0; i<buttons_palette_Analog.size(); i++)
//    {
//        buttons_palette_Analog[i]->update();
//    }
//    for (int i=0; i<buttons_palette_CompSat.size(); i++)
//    {
//        buttons_palette_CompSat[i]->update();
//    }

    //-

    // PALETTES

    // 1. triad
    for (int i = 0 ; i < buttons_palette_Triad.size(); i++)
    {
        buttons_palette_Triad[i]->update();
    }

    // 2. complement triad
    for (int i = 0 ; i < buttons_palette_ComplTriad.size(); i++)
    {
        buttons_palette_ComplTriad[i]->update();
    }

    // 3. complement sat
    for (int i = 0 ; i < buttons_palette_CompSat.size(); i++)
    {
        buttons_palette_CompSat[i]->update();
    }

    // 4. complement brgt
    for (int i = 0 ; i < buttons_palette_ComplBrgt.size(); i++)
    {
        buttons_palette_ComplBrgt[i]->update();
    }

    // 5. mono sat
    for (int i = 0 ; i < buttons_palette_MonoSat.size(); i++)
    {
        buttons_palette_MonoSat[i]->update();
    }

    // 6. mono brgt
    for (int i = 0 ; i < buttons_palette_MonoBrgt.size(); i++)
    {
        buttons_palette_MonoBrgt[i]->update();
    }

    // 7. analogue
    for (int i=0; i<buttons_palette_Analog.size(); i++)
    {
        buttons_palette_Analog[i]->update();
    }

    // 8. random
    for (int i = 0 ; i < buttons_palette_Random.size(); i++)
    {
        buttons_palette_Random[i]->update();
    }
}

//--------------------------------------------------------------
void ofxColorManager::draw_Interface(){
    scene->render();

    if (bShowDebug)
    {
        scene->renderDebug();
    }
}

//--------------------------------------------------------------
bool ofxColorManager::imGui()
{
    auto mainSettings = ofxImGui::Settings();

    this->gui.begin();
    {
        if (ofxImGui::BeginWindow("COLOR MANAGER", mainSettings, false))
        {
            // DATA
            if (ofxImGui::BeginTree(this->params_data, mainSettings))
            {
                ofxImGui::AddParameter(this->color_backGround, true);

                ofxImGui::EndTree(mainSettings);
            }

            // COLOR
            if (ofxImGui::BeginTree(this->params_color, mainSettings))
            {
                ofxImGui::AddParameter(this->myColor, true);

                if (ImGui::Button("RANDOM COLOR")) {
                    myColor = ofFloatColor(ofRandom(0., 1.), ofRandom(0., 1.), ofRandom(0., 1.));
                }
                if (ImGui::Button("ADD COLOR")) {
                    add_color(ofColor(myColor.get()));
                }
                if (ImGui::Button("REMOVE COLOR")) {
                    remove_colorLast();
                }

                ofxImGui::EndTree(mainSettings);
            }

            // ALGORITHMIC PALETTE
            if (ofxImGui::BeginTree(this->params_palette, mainSettings))
            {
                ofxImGui::AddParameter(this->SATURATION);
                ofxImGui::AddParameter(this->BRIGHTNESS);

                ofxImGui::AddParameter(this->bRandomPalette);
                if (ImGui::Button("RANDOMIZE"))
                {
                    cout << "RANDOMIZE: " << bRandomPalette << endl;

                    if (bRandomPalette)
                    {
                        bRandomPalette = false;
                        random.generateRandom();
                    }
                }

                ofxImGui::EndTree(mainSettings);
            }

            // CURVE
            if (ofxImGui::BeginTree(this->params_curve, mainSettings))
            {
                ofxImGui::AddParameter(this->bResetCurve);
                ofxImGui::AddParameter(this->curve_pos);

                if (bResetCurve)
                {
                    bResetCurve = false;
                    curvesTool.clear();
                    curvesTool.add(ofVec2f(0, 0));
                    curvesTool.add(ofVec2f(255, 255));
                }

                ofxImGui::EndTree(mainSettings);
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

    add_color_Interface(c);

    gradient.addColor( c );
}

//--------------------------------------------------------------
void ofxColorManager::remove_colorLast()
{
//    if (!palette.empty())
    {
        palette.pop_back();
    }

    if (true)
    {
        gradient.removeColorLast();
    }

//    if (!buttons.empty())
    {
        buttons.pop_back();
        int iSize = scene->getNumChildren();
        scene->removeChild(iSize);
    }

}

//--------------------------------------------------------------
void ofxColorManager::update()
{
    update_Interface();

    //-

    update_palettes();

    //-

    update_curveTool();

    //-
}


//--------------------------------------------------------------
void ofxColorManager::setup_curveTool()
{
    amount = 256;
//    amount = 100;
    curvesTool.setup(amount);
    curvesTool.load("curves.yml"); //needed because it fills polyline

    img.allocate(amount, amount, OF_IMAGE_COLOR);
    show = true;

    curve_pos.set("CURVE POS", 0., 0., 1.);
    bResetCurve.set("RESET CURVE", false);
    params_curve.setName("CURVE");
    params_curve.add(curve_pos);
    params_curve.add(bResetCurve);

    cnt = 0;
}

//--------------------------------------------------------------
void ofxColorManager::update_curveTool()
{
    for(int x = 0; x < amount; x++)
    {
        for(int y = 0; y < amount; y++)
        {
//            img.setColor(x, y, ofColor(curvesTool[x]));
            float f = ofMap( curvesTool[x], 0, 255, 0., 1. );
            ofColor c = gradient.getColorAtPercent(f);

            img.setColor(x, y, c);

        }
    }
    img.update();

    cnt = ofMap( curve_pos.get(), 0., 1., 0, amount );
}

//--------------------------------------------------------------
void ofxColorManager::draw_curveTool() {
    int pos_curve_x;
    int pos_curve_y;
    pos_curve_x = 700;
    pos_curve_y = 100;

    //-

    ofPushMatrix();
    ofPushStyle();

    ofTranslate(pos_curve_x, pos_curve_y);

    if(show)
    {
        ofSetColor(255);
//        curvesTool.draw();
        curvesTool.draw(0,0,cnt);
        img.draw(amount, 0);
    }

    ofSetColor(255,0,0);
//    ofDrawLine(0, amount-curvesTool[cnt], ofGetWidth(),amount-curvesTool[cnt]);
    ofDrawLine(0, amount-curvesTool[cnt], amount, amount-curvesTool[cnt]);

    ofDrawCircle(cnt,amount-curvesTool[cnt],3);

    float lerp_amt = ofMap(cnt,0,amount-1,0,1);
    ofNoFill();
    ofSetColor(255);

//    //--result based on interpolation
//    ofPushMatrix();
//    ofTranslate(0, amount + 10 );
//    float vv = curvesTool.getAt(cnt);
//    ofDrawRectangle(0,0,vv,50);
//    ofDrawBitmapString(ofToString(vv,2),0,10);
//    ofPopMatrix();
//
//    //----result based on lut[index]
//    ofPushMatrix();
//    ofTranslate(0, amount + 60 );
//    ofDrawRectangle(0,0,curvesTool[cnt],50);
//    ofDrawBitmapString(ofToString(curvesTool[cnt]),0,10);
//    ofPopMatrix();

    ofPopMatrix();
    ofPopStyle();
}

//--------------------------------------------------------------
void ofxColorManager::add_color_Palette(int i)
{
}

//--------------------------------------------------------------
void ofxColorManager::setup_palettes()
{
    int x = 50;
    int y = 500;
    int size = 40;
    int pad = 5;
    int x0 = x;

    // 1. triad
    x = x0;
    y += (size+pad);
    for (int i = 0 ; i < triad.size(); i++) {
        x += (size+pad);
        ButtonExample *btn = new ButtonExample();
        btn->setup(x, y, size, size);
        btn->setup_colorBACK( color_p );
        btn->setLocked(true);
        btn->setName("triad" + ofToString(i));
        btn->setColor(triad[i]);
        scene->addChild(btn);
        if (i>0)
        {
            btn->placeNextTo(*buttons_palette_Triad[i-1], Node::RIGHT);
        }
        buttons_palette_Triad.push_back(btn);
    }

    // 2. complement triad
    x = x0;
    y += (size+pad);
    for (int i = 0 ; i < complementTriad.size(); i++) {
        x += (size+pad);
        ButtonExample *btn = new ButtonExample();
        btn->setup(x, y, size, size);
        btn->setup_colorBACK( color_p );
        btn->setLocked(true);
        btn->setName("compTriad" + ofToString(i));
        btn->setColor(complementTriad[i]);
        scene->addChild(btn);
        if (i>0)
        {
            btn->placeNextTo(*buttons_palette_ComplTriad[i-1], Node::RIGHT);
        }
        buttons_palette_ComplTriad.push_back(btn);
    }

    // 3. complement sat
    x = x0;
    y += (size+pad);
    for (int i = 0 ; i < complement.size(); i++) {
        x += (size+pad);
        ButtonExample *btn = new ButtonExample();
        btn->setup(x, y, size, size);
        btn->setup_colorBACK( color_p );
        btn->setLocked(true);
        btn->setName("compSat" + ofToString(i));
        btn->setColor(complement[i]);
        scene->addChild(btn);
        if (i>0)
        {
            btn->placeNextTo(*buttons_palette_CompSat[i-1], Node::RIGHT);
        }
        buttons_palette_CompSat.push_back(btn);
    }

    // 4. complement brgt
    x = x0;
    y += (size+pad);
    for (int i = 0 ; i < complementBrightness.size(); i++) {
        x += (size+pad);
        ButtonExample *btn = new ButtonExample();
        btn->setup(x, y, size, size);
        btn->setup_colorBACK( color_p );
        btn->setLocked(true);
        btn->setName("compBrgt" + ofToString(i));
        btn->setColor(complementBrightness[i]);
        scene->addChild(btn);
        if (i>0)
        {
            btn->placeNextTo(*buttons_palette_ComplBrgt[i-1], Node::RIGHT);
        }
        buttons_palette_ComplBrgt.push_back(btn);
    }

    // 5. mono sat
    x = x0;
    y += (size+pad);
    for (int i = 0 ; i < monochrome.size(); i++) {
        x += (size+pad);
        ButtonExample *btn = new ButtonExample();
        btn->setup(x, y, size, size);
        btn->setup_colorBACK( color_p );
        btn->setLocked(true);
        btn->setName("monoSat" + ofToString(i));
        btn->setColor(monochrome[i]);
        scene->addChild(btn);
        if (i>0)
        {
            btn->placeNextTo(*buttons_palette_MonoSat[i-1], Node::RIGHT);
        }
        buttons_palette_MonoSat.push_back(btn);
    }

    // 6. mono brgt
    x = x0;
    y += (size+pad);
    for (int i = 0 ; i < monochromeBrightness.size(); i++) {
        x += (size+pad);
        ButtonExample *btn = new ButtonExample();
        btn->setup(x, y, size, size);
        btn->setup_colorBACK( color_p );
        btn->setLocked(true);
        btn->setName("monoBrgt" + ofToString(i));
        btn->setColor(monochromeBrightness[i]);
        scene->addChild(btn);
        if (i>0)
        {
            btn->placeNextTo(*buttons_palette_MonoBrgt[i-1], Node::RIGHT);
        }
        buttons_palette_MonoBrgt.push_back(btn);
    }

    // 7. analogue
    for (int i=0; i<analogue.size(); i++)
    {
        x += (size+pad);
        ButtonExample *btn = new ButtonExample();
        btn->setup(x, y, size, size);
        btn->setup_colorBACK( color_p );
        btn->setLocked(true);
        btn->setName("analogue" + ofToString(i));
        btn->setColor(analogue[i]);
        scene->addChild(btn);
        if (i>0)
        {
            btn->placeNextTo(*buttons_palette_Analog[i-1], Node::RIGHT);
        }
        buttons_palette_Analog.push_back(btn);
    }

    // 8. random
    x = x0;
    y += (size+pad);
    for (int i = 0 ; i < random.size(); i++) {
        x += (size+pad);
        ButtonExample *btn = new ButtonExample();
        btn->setup(x, y, size, size);
        btn->setup_colorBACK( color_p );
        btn->setLocked(true);
        btn->setName("random" + ofToString(i));
        btn->setColor(random[i]);
        scene->addChild(btn);
        if (i>0)
        {
            btn->placeNextTo(*buttons_palette_Random[i-1], Node::RIGHT);
        }
        buttons_palette_Random.push_back(btn);
    }
}

//--------------------------------------------------------------
void ofxColorManager::update_palettes()
{
    brightness = BRIGHTNESS;
    saturation = SATURATION;

    //-

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

    //-

    // PALETTES

    // 1. triad
    for (int i = 0 ; i < buttons_palette_Triad.size(); i++)
    {
        buttons_palette_Triad[i]->setColor(triad[i]);
    }

    // 2. complement triad
    for (int i = 0 ; i < buttons_palette_ComplTriad.size(); i++)
    {
        buttons_palette_ComplTriad[i]->setColor(complementTriad[i]);
    }

    // 3. complement sat
    for (int i = 0 ; i < buttons_palette_CompSat.size(); i++)
    {
        buttons_palette_CompSat[i]->setColor(complement[i]);
    }

    // 4. complement brgt
    for (int i = 0 ; i < buttons_palette_ComplBrgt.size(); i++)
    {
        buttons_palette_ComplBrgt[i]->setColor(complementBrightness[i]);
    }

    // 5. mono sat
    for (int i = 0 ; i < buttons_palette_MonoSat.size(); i++)
    {
        buttons_palette_MonoSat[i]->setColor(monochrome[i]);
    }

    // 6. mono brgt
    for (int i = 0 ; i < buttons_palette_MonoBrgt.size(); i++)
    {
        buttons_palette_MonoBrgt[i]->setColor(monochromeBrightness[i]);
    }

    // 7. analogue
    for (int i=0; i<buttons_palette_Analog.size(); i++)
    {
        buttons_palette_Analog[i]->setColor(analogue[i]);
    }

    // 8. random
    for (int i = 0 ; i < buttons_palette_Random.size(); i++)
    {
        buttons_palette_Random[i]->setColor(random[i]);
    }

}

//--------------------------------------------------------------
void ofxColorManager::draw()
{
    //-

    // BACKGROUND

    ofClear(ofColor( color_backGround.get() ));

    //-

    // COLOR

    int x, y, w, h;
    x = 400;
    y = 20;
    w = h = 100;
    ofRectangle rColor = ofRectangle( x, y, w, h );
    ofPushStyle();
    ofFill();
//    ofSetColor( ofColor( myColor.get() ) );//TODO: mirror with params pointer ?
    ofSetColor( ofColor( color_p ) );
    ofDrawRectangle(rColor);
    ofPopStyle();

    //-

    // INTERFACE

    draw_Interface();

    //-

    // GRADIENT

    int grad_x;
    int grad_y;
    int grad_w;
    int grad_h;
    grad_x = 50;
    grad_y = 700;
    grad_w = 200;
    grad_h = 40;

    gradient.drawDebug(grad_x, grad_y, grad_w, grad_h);

    int rSize = 50;
    ofRectangle r = ofRectangle( grad_x, grad_y + rSize + 2, rSize, rSize );
    ofPushStyle();
    ofFill();
    ofSetColor(gradient.getColorAtPercent(curve_pos));
    ofDrawRectangle(r);
    ofPopStyle();

    //-

    // CURVE

    draw_curveTool();

    //-

    // GUI

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

//    // ALGORTIHMIC PALETTE
//
//    ofPushMatrix();
//    ofTranslate(600, 400);
//    int padTxt = 200;
//
//    //--begin
//    ofPushMatrix();
//    for (int i = 0 ; i < triad.size(); i++) {
//        ofSetColor(triad[i]);
//        ofRect(0, 0, RECT_SIZE, RECT_SIZE);
//        ofTranslate(RECT_SIZE, 0);
//    }
//    ofPopMatrix();
//    ofPushMatrix();
//    ofTranslate(padTxt, RECT_SIZE /2);
//    ofSetColor(0);
//    ofDrawBitmapString("Triad", 0, 0);
//    ofPopMatrix();
//    ofTranslate(0, RECT_SIZE + PADDING);
//    // ----
//    ofPushMatrix();
//    for (int i = 0 ; i < complementTriad.size(); i++) {
//        ofSetColor(complementTriad[i]);
//        ofRect(0, 0, RECT_SIZE, RECT_SIZE);
//        ofTranslate(RECT_SIZE, 0);
//    }
//    ofPopMatrix();
//    ofPushMatrix();
//    ofTranslate(padTxt, RECT_SIZE /2);
//    ofSetColor(0);
//    ofDrawBitmapString("Complement Triad", 0, 0);
//    ofPopMatrix();
//    ofTranslate(0, RECT_SIZE + PADDING);
//    // ----
//    ofPushMatrix();
//    for (int i = 0 ; i < complement.size(); i++) {
//        ofSetColor(complement[i]);
//        ofRect(0, 0, RECT_SIZE, RECT_SIZE);
//        ofTranslate(RECT_SIZE, 0);
//    }
//    ofPopMatrix();
//    ofPushMatrix();
//    ofTranslate(padTxt, RECT_SIZE /2);
//    ofSetColor(0);
//    ofDrawBitmapString("Complement (Saturation)", 0, 0);
//    ofPopMatrix();
//    ofTranslate(0, RECT_SIZE + PADDING);
//    // ----
//    ofPushMatrix();
//    for (int i = 0 ; i < complementBrightness.size(); i++) {
//        ofSetColor(complementBrightness[i]);
//        ofRect(0, 0, RECT_SIZE, RECT_SIZE);
//        ofTranslate(RECT_SIZE, 0);
//    }
//    ofPopMatrix();
//    ofPushMatrix();
//    ofTranslate(padTxt, RECT_SIZE /2);
//    ofSetColor(0);
//    ofDrawBitmapString("Complement (Brightness)", 0, 0);
//    ofPopMatrix();
//    ofTranslate(0, RECT_SIZE + PADDING);
//    // ----
//    ofPushMatrix();
//    for (int i = 0 ; i < monochrome.size(); i++) {
//        ofSetColor(monochrome[i]);
//        ofRect(0, 0, RECT_SIZE, RECT_SIZE);
//        ofTranslate(RECT_SIZE, 0);
//    }
//    ofPopMatrix();
//    ofPushMatrix();
//    ofTranslate(padTxt, RECT_SIZE /2);
//    ofSetColor(0);
//    ofDrawBitmapString("Monochrome (Saturation)", 0, 0);
//    ofPopMatrix();
//    ofTranslate(0, RECT_SIZE + PADDING);
//    // ----
//    ofPushMatrix();
//    for (int i = 0 ; i < monochromeBrightness.size(); i++) {
//        ofSetColor(monochromeBrightness[i]);
//        ofRect(0, 0, RECT_SIZE, RECT_SIZE);
//        ofTranslate(RECT_SIZE, 0);
//    }
//    ofPopMatrix();
//    ofPushMatrix();
//    ofTranslate(padTxt, RECT_SIZE /2);
//    ofSetColor(0);
//    ofDrawBitmapString("Monochrome (Brightness)", 0, 0);
//    ofPopMatrix();
//    ofTranslate(0, RECT_SIZE + PADDING);
//    // ----
//    ofPushMatrix();
//    for (int i = 0 ; i < analogue.size(); i++) {
//        ofSetColor(analogue[i]);
//        ofRect(0, 0, RECT_SIZE, RECT_SIZE);
//        ofTranslate(RECT_SIZE, 0);
//    }
//    ofPopMatrix();
//    ofPushMatrix();
//    ofTranslate(padTxt, RECT_SIZE /2);
//    ofSetColor(0);
//    ofDrawBitmapString("Analogue", 0, 0);
//    ofPopMatrix();
//    ofTranslate(0, RECT_SIZE + PADDING);
//    // ----
//    ofPushMatrix();
//    for (int i = 0 ; i < random.size(); i++) {
//        ofSetColor(random[i]);
//        ofRect(0, 0, RECT_SIZE, RECT_SIZE);
//        ofTranslate(RECT_SIZE, 0);
//    }
//    ofPopMatrix();
//    ofPushMatrix();
//    ofTranslate(padTxt, RECT_SIZE /2);
//    ofSetColor(0);
//    ofDrawBitmapString("Random (Click to regenerate)", 0, 0);
//    ofPopMatrix();
//
//    ofPopMatrix();
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

    //-

    if(key == ' ')
    {
        cout << "setup_palettes" << endl;
        setup_palettes();
    }

    if(key == 'd')
    {
        bShowDebug = !bShowDebug;
    }

    if(key == 'g')
    {
        this->guiVisible = ! this->guiVisible;
    }

    //-

    if (key == 'r') {
        random.generateRandom();
    }

    //-

    if (key == 'c') {
        show = !show;
    }
    if (key == 's') {
        curvesTool.save("curves.yml");
    }
    if (key == 'l') {
        curvesTool.load("curves.yml");
    }
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