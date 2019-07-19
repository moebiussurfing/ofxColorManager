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
    mouseRuler.setup();
    mouseRuler.toggleVisibility();

    //-

    // LAYOUT

    // global mini pad between panels/objects
    pad = 5;

    // gui
    gui_x = 10;
    gui_y = 10;
    gui_w = 200;
    gui_h = 330;

    // user palette
    palette_x = gui_x;
    palette_y = gui_y + gui_h;
    color_size = 40;

    // algorithmic palettes
    box_size = color_size;
    palettes_x = gui_x;
    palettes_y = palette_y + 2 * box_size;//2 rows

    // curve tool pos
    amount = 256;
    pos_curve_x = 525;
    pos_curve_y = 75;

    // slider
    slider_x = pos_curve_x + amount + pad;
    slider_y = pos_curve_y;
    slider_w = box_size / 2;
    slider_h = amount;

    // squared LUT referenced to curve pos (vertical)
    pos_curve_prev_x = amount + pad + slider_w + pad;
    pos_curve_prev_y = 0;
    pos_curve_prev_w = box_size;
    pos_curve_prev_h = amount;

    // gradient pre curve
    grad_w = box_size;
    grad_x = pos_curve_x - (grad_w + pad);
    grad_y = pos_curve_y;
    grad_h = amount;

    currColor_x = pos_curve_x + pos_curve_prev_x + pos_curve_prev_w + pad;
    currColor_y = pos_curve_y;

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

    random.generateRandom();
    update_palettes();
    setup_palettes();

    //-

    // CURVE

    setup_curveTool();

    //-

    // GRADIENT

    gradient.reset();
    gradient_hard.set("GRADIENT HARD", false);
    gradient.setHardMode(gradient_hard);
    gradient.setDrawVertical(true);
    gradient.setDrawDirFlip(true);

//    params_curve.add(gradient_hard);
//    params_curve.add(bResetCurve);

    //-

    // GUI

    this->gui.setup();
    this->guiVisible = true;
    imGui_theme();

    //-

    // LISTENERS

    addKeysListeners();
    addMouseListeners();

    //--

    // SETTINGS

    XML_params.setName("ofxColorManager");
    XML_params.add(color_backGround);
    XML_params.add(myColor);
    XML_params.add(BRIGHTNESS);
    XML_params.add(SATURATION);
    XML_params.add(gradient_hard);
    load_group_XML(XML_params, XML_path);

    //-
}

//--------------------------------------------------------------
void ofxColorManager::imGui_theme()
{
    ofColor myColor;
    int grayDark = 0;
    myColor = ofColor(grayDark, grayDark, grayDark, 255);//black
    ofColor myColor2;
    int gray = 24;
    myColor2 = ofColor(gray, gray, gray, 255);//gray1
    ofColor myColor3;
    int gray2 = 16;
    myColor3 = ofColor(gray2, gray2, gray2, 255);//gray2

    ImGuiStyle *style = &ImGui::GetStyle();
//    style->WindowRounding(2.);
    style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(myColor, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(myColor2, 0.21f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(myColor, 0.78f);
    style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(myColor2, 1.00f);
    style->Colors[ImGuiCol_CheckMark] = ImVec4(ofColor(255), 0.80f);
    style->Colors[ImGuiCol_Button] = ImVec4(myColor3, 0.8f);
    style->Colors[ImGuiCol_ButtonHovered] = ImVec4(myColor, 0.86f);
    style->Colors[ImGuiCol_ButtonActive] = ImVec4(myColor, 1.00f);
    style->Colors[ImGuiCol_TitleBg] = ImVec4(myColor, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(myColor, 0.75f);
    style->Colors[ImGuiCol_TitleBgActive] = ImVec4(myColor, 1.00f);
    style->Colors[ImGuiCol_Header] = ImVec4(myColor, 0.76f);
    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(myColor2, 0.86f);
    style->Colors[ImGuiCol_HeaderActive] = ImVec4(myColor, 1.00f);
//    style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(myColor, 1.00f);
//    style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(myColor, 1.00f);
//    style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(myColor, 0.78f);
//    style->Colors[ImGuiCol_ColumnHovered] = ImVec4(myColor, 0.78f);
//    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(myColor, 0.86f);
//    style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(myColor, 1.00f);
    style->Colors[ImGuiCol_FrameBg] = ImVec4(myColor, 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(myColor2, 0.78f);//hover
    style->Colors[ImGuiCol_FrameBgActive] = ImVec4(myColor, 1.00f);
}

//--------------------------------------------------------------
void ofxColorManager::add_color_Interface(ofColor c)
{
    palette_x = gui_x;
    int miniPad = 4;
    int perRow = 5;

    int i = buttons.size();

    // setup them in a grid
//    palette_x += (i%perRow)*(color_size+miniPad);
//    palette_y += (i/perRow)*(color_size+miniPad);

    palette_x += ( i * ( color_size + miniPad ) );
//    palette_y += (i/perRow)*(color_size+pad);

    // create a ButtonExample node
    ButtonExample *btn = new ButtonExample();
    btn->setup(palette_x, palette_y, color_size, color_size);
    btn->setColor(c);
    btn->setup_colorBACK( color_p );
    btn->setLocked(true);
    btn->setName("btn" + ofToString(i));

    // add it to the scene
    scene->addChild(btn);

//    if (i%perRow>0)
//    {
//        // this can be called to place nodes next to each other
//        btn->placeNextTo(*buttons[i-1], Node::RIGHT);
//    }

    // keep reference (we need it to update the nodes)
    buttons.push_back(btn);
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

    //TODO: move ALL listeners outside gui

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
                if (bRandomPalette)
                {
                    cout << "RANDOMIZE: " << bRandomPalette << endl;
                    random.generateRandom();
                    bRandomPalette = false;
                }

                ofxImGui::EndTree(mainSettings);
            }

            // CURVE
            if (ofxImGui::BeginTree(this->params_curve, mainSettings))
            {
                ofxImGui::AddParameter(this->bResetCurve);
                ofxImGui::AddParameter(this->curve_pos);
                ofxImGui::AddParameter(this->gradient_hard);

                if (bResetCurve)
                {
                    bResetCurve = false;
                    curvesTool.clear();
                    curvesTool.add(ofVec2f(0, 0));
                    curvesTool.add(ofVec2f(255, 255));
                }
                else if (true)
                {
                    gradient.setHardMode(gradient_hard);
//                    cout << "gradient_hard: " << gradient_hard << endl;
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
    show = true;

    //    amount = 256;
    cnt = 0;

    curvesTool.setup(amount);
    curvesTool.load("curves.yml"); //needed because it fills polyline

    img.allocate(pos_curve_prev_w, pos_curve_prev_h, OF_IMAGE_COLOR);

    curve_pos.set("CURVE POS", 0., 0., 1.);// pct value 0. to 1.
    bResetCurve.set("RESET CURVE", false);
    params_curve.setName("CURVE");
    params_curve.add(curve_pos);
    params_curve.add(bResetCurve);

    //-

    // pct slider
    curveSlider.setup(slider_x, slider_y, slider_w, slider_h, 0., 1., 0, true, true);
    curveSlider.setLabelString("value");

    //-
}

//--------------------------------------------------------------
void ofxColorManager::update_curveTool()
{
    // slider // TODO: mirror to gui param
    curve_pos = curveSlider.getValue();

    //-

    // vertical and rectangle

    bool bInvert = true;// flip gradient direction
    float pct;
    float curveVal;

    for(int y = 0; y < pos_curve_prev_h; y++)
    {
        curveVal = curvesTool[y];
        if (!bInvert)
        {
            pct = ofMap( curveVal, 0, amount-1, 0., 1. );
        }
        else{
            pct = ofMap( curveVal, 0, amount-1, 1., 0. );
        }
        ofColor c = gradient.getColorAtPercent(pct);

        for(int x = 0; x < pos_curve_prev_w; x++)
        {
            img.setColor(x, y, c);
        }
    }
    img.update();

    cnt = ofMap( curve_pos.get(), 0., 1., 0, amount-1 );
}

//--------------------------------------------------------------
void ofxColorManager::draw_curveTool() {

    ofPushMatrix();
    ofPushStyle();

    ofTranslate(pos_curve_x, pos_curve_y);

    if (show)
    {
        ofSetColor(255);
        // curve editor
        curvesTool.draw(0,0,cnt);

        // box LUT gradiant
        img.draw(pos_curve_prev_x, pos_curve_prev_y);
    }

    // red line
    ofSetColor(ofColor::red);
    float y =  amount-curvesTool[cnt];
//    float y = amount-curvesTool.getAt(cnt);
    ofDrawLine(0, y, amount, y);
//    ofDrawLine(amount + slider_w, y, amount + slider_w + pad + pos_curve_prev_w + 100, y);

    // red current point
    ofDrawCircle(cnt, y, 3);

//    float lerp_amt = ofMap(cnt,0,amount-1,0,1);
//    ofNoFill();
//    ofSetColor(255);

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

////--------------------------------------------------------------
//void ofxColorManager::add_color_Palette(int i)
//{
//}

//--------------------------------------------------------------
void ofxColorManager::setup_palettes()
{
    int x0 = palettes_x;
    int y0 = palettes_y;//to recall at end

    // 1. triad
    palettes_x = x0;
    palettes_y += (box_size+pad);
    for (int i = 0 ; i < triad.size(); i++) {
        ButtonExample *btn = new ButtonExample();
        btn->setup(palettes_x, palettes_y, box_size, box_size);
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
        palettes_x += (box_size+pad);
    }

    // 2. complement triad
    palettes_x = x0;
    palettes_y += (box_size+pad);
    for (int i = 0 ; i < complementTriad.size(); i++) {
        ButtonExample *btn = new ButtonExample();
        btn->setup(palettes_x, palettes_y, box_size, box_size);
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
        palettes_x += (box_size+pad);
    }

    // 3. complement sat
    palettes_x = x0;
    palettes_y += (box_size+pad);
    for (int i = 0 ; i < complement.size(); i++) {
        ButtonExample *btn = new ButtonExample();
        btn->setup(palettes_x, palettes_y, box_size, box_size);
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
        palettes_x += (box_size+pad);
    }

    // 4. complement brgt
    palettes_x = x0;
    palettes_y += (box_size+pad);
    for (int i = 0 ; i < complementBrightness.size(); i++) {
        ButtonExample *btn = new ButtonExample();
        btn->setup(palettes_x, palettes_y, box_size, box_size);
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
        palettes_x += (box_size+pad);
    }

    // 5. mono sat
    palettes_x = x0;
    palettes_y += (box_size+pad);
    for (int i = 0 ; i < monochrome.size(); i++) {
        ButtonExample *btn = new ButtonExample();
        btn->setup(palettes_x, palettes_y, box_size, box_size);
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
        palettes_x += (box_size+pad);
    }

    // 6. mono brgt
    palettes_x = x0;
    palettes_y += (box_size+pad);
    for (int i = 0 ; i < monochromeBrightness.size(); i++) {
        ButtonExample *btn = new ButtonExample();
        btn->setup(palettes_x, palettes_y, box_size, box_size);
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
        palettes_x += (box_size+pad);
    }

    // 7. analogue
    palettes_x = x0;
    palettes_y += (box_size+pad);
    for (int i=0; i<analogue.size(); i++)
    {
        ButtonExample *btn = new ButtonExample();
        btn->setup(palettes_x, palettes_y, box_size, box_size);
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
        palettes_x += (box_size+pad);
    }

    // 8. random
    palettes_x = x0;
    palettes_y += (box_size+pad);
    for (int i = 0 ; i < random.size(); i++) {
        ButtonExample *btn = new ButtonExample();
        btn->setup(palettes_x, palettes_y, box_size, box_size);
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
        palettes_x += (box_size+pad);
    }

    palettes_y = y0; //back
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

    // this will not runned on setup() because vectors are empty yet

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

    // COLOR BOX

    int cx, cy, cw, ch;
    cx = 350;
    cy = 20;
    cw = ch = 100;
    ofRectangle rColor = ofRectangle( cx, cy, cw, ch );
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

    // full rectangle
    gradient.drawDebug(grad_x, grad_y, grad_w, grad_h);

    // current box color at point

    ofRectangle r = ofRectangle( currColor_x, currColor_y, box_size/2, slider_h );
    ofPushStyle();
    ofFill();
    ofSetColor( gradient.getColorAtPercent( curve_pos ) );
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

    // ALGORTIHMIC PALETTE

    int x, y, w, h;
    h = (box_size+pad);

    ofPushMatrix();
    ofPushStyle();
    ofSetColor(ofColor::white);
    ofTranslate(palettes_x + 80, palettes_y + h/2 + 12);

    ofTranslate(0, h);
    ofDrawBitmapString("Triad", 0, 0);
    ofTranslate(0, h);
    ofDrawBitmapString("Complement Triad", 0, 0);
    ofTranslate(0, h);
    ofDrawBitmapString("Complement (Saturation)", 0, 0);
    ofTranslate(0, h);
    ofDrawBitmapString("Complement (Brightness)", 0, 0);
    ofTranslate(0, h);
    ofDrawBitmapString("Monochrome (Saturation)", 0, 0);
    ofTranslate(0, h);
    ofDrawBitmapString("Monochrome (Brightness)", 0, 0);
    ofTranslate(0, h);
    ofDrawBitmapString("Analogue", 0, 0);
    ofTranslate(0, h);
    ofDrawBitmapString("Random", 0, 0);

    ofPopMatrix();
    ofPopStyle();

    //--
}

//--------------------------------------------------------------
void ofxColorManager::exit()
{
    removeKeysListeners();
    removeMouseListeners();

    save_group_XML(XML_params, XML_path);
}

//--------------------------------------------------------------
void ofxColorManager::keyPressed( ofKeyEventArgs& eventArgs )
{
    const int & key = eventArgs.key;
    cout << "key: " << key << endl;

    //-

    if (key == 'm')
    {
        mouseRuler.toggleVisibility();
    }

    if (key == ' ')
    {
        myColor = ofFloatColor(ofRandom(0., 1.), ofRandom(0., 1.), ofRandom(0., 1.));
        add_color(ofColor(myColor.get()));
    }

    if (key == 'd')
    {
        bShowDebug = !bShowDebug;
    }

    if (key == 'g')
    {
        this->guiVisible = ! this->guiVisible;
    }

    //-

    if (key == 'r')
    {
        random.generateRandom();
//        update_palettes();
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

    TouchManager::one().touchMove(button, ofVec2f(x, y));
}

//--------------------------------------------------------------
void ofxColorManager::mousePressed(ofMouseEventArgs& eventArgs){
    const int & x = eventArgs.x;
    const int & y = eventArgs.y;
    const int & button = eventArgs.button;
    ofLogNotice("ofxColorManager") << "mousePressed " <<  x << ", " << y << ", " << button;

    TouchManager::one().touchDown(button, ofVec2f(x, y));
}

//--------------------------------------------------------------
void ofxColorManager::mouseReleased(ofMouseEventArgs& eventArgs){
    const int & x = eventArgs.x;
    const int & y = eventArgs.y;
    const int & button = eventArgs.button;
    ofLogNotice("ofxColorManager") << "mouseReleased " <<  x << ", " << y << ", " << button;

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

//--------------------------------------------------------------
void ofxColorManager::load_group_XML(ofParameterGroup &g, string path)
{
    ofLogNotice("ofxColorManager") << "load_group_XML " << path;
    ofXml settings;
    settings.load(path);
    ofDeserialize(settings, g);
}

//--------------------------------------------------------------
void ofxColorManager::save_group_XML(ofParameterGroup &g, string path)
{
    ofLogNotice("ofxColorManager") << "save_group_XML " << path;
    ofXml settings;
    ofSerialize(settings, g);
    settings.save(path);
}