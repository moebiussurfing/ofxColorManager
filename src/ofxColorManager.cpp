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
void ofxColorManager::setup()
{
    mouseRuler.setup();
    mouseRuler.toggleVisibility();

    //-

    // LAYOUT

    setup_Gui_layout();

    //-

    // DATA

    color_backGround.set("BACKGROUND", ofFloatColor::white);
    params_data.setName("DATA");
    params_data.add(color_backGround);

    //--

    // COLOR

    myColor.set("COLOR", ofFloatColor::black);
    params_color.setName("COLOR");
    params_color.add(myColor);

//    bClearPalette.set("RESET PALETTE", false);
//    params_color.add(bClearPalette);

    //-

    // CONTROL

    bRandomColor.set("RANDOM COLOR", false);
    bAddColor.set("ADD COLOR", false);
    bRemoveColor.set("REMOVE COLOR", false);
    bClearPalette.set("CLEAR PALETTE", false);

    params_control.setName("CONTROL");
    params_control.add(myColor);
    params_control.add(bRandomColor);
    params_control.add(bAddColor);
    params_control.add(bRemoveColor);
    params_control.add(bClearPalette);

    ofAddListener(params_control.parameterChangedE(), this, &ofxColorManager::Changed_control);

    //-

    // ALGORITHMIC PALETTE

    BRIGHTNESS.set("BRIGHTNESS", 128, 0, 255 );
    SATURATION.set("SATURATION", 128, 0, 255 );
    bRandomPalette.set("RANDOMIZE", false);

    params_palette.setName("ALGORITHMIC PALETTE");
    params_palette.add(BRIGHTNESS);
    params_palette.add(SATURATION);
    params_palette.add(bRandomPalette);

    ofAddListener(params_palette.parameterChangedE(), this, &ofxColorManager::Changed_control);

    //-

    // INTERFACE

    setup_Interface();

    //-

    // PALETTES

    random.generateRandom();
    update_palettes();
    setup_palettes();

    //-

    // GRADIENT

    gradient.reset();
    gradient_hard.set("GRADIENT HARD", false);
    gradient.setHardMode(gradient_hard);
    gradient.setDrawVertical(true);
    gradient.setDrawDirFlip(true);

    params_curve.setName("CURVE");
    params_curve.add(gradient_hard);

    //-

    // CURVE TOOL

    setup_curveTool();

    //--

    // GUI

    this->gui.setup();
    this->guiVisible = true;
    imGui_theme();

    //--

    // LISTENERS

    addKeysListeners();
    addMouseListeners();

    //--

    // STARTUP SETTINGS

    XML_params.setName("ofxColorManager");
    XML_params.add(color_backGround);
    XML_params.add(myColor);
    XML_params.add(BRIGHTNESS);
    XML_params.add(SATURATION);
    XML_params.add(gradient_hard);
    load_group_XML(XML_params, XML_path);

    loadPalette("myPalette");

    //-
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
void ofxColorManager::setup_Gui_layout()
{
    // LAYOUT

    // global mini pad between panels/objects
    pad = 5;

    // gui
    gui_x = 10;
    gui_y = 10;
    gui_w = 200;
    gui_h = 350;

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

    // squared LUT referenced to curve pos (vertical)
    pos_curve_prev_x = amount + pad;
    pos_curve_prev_y = 0;
    pos_curve_prev_w = box_size;
    pos_curve_prev_h = amount;

    // slider
    slider_x = pos_curve_x + pad + amount + pad + pos_curve_prev_w;
    slider_y = pos_curve_y;
    slider_w = box_size / 2;
    slider_h = amount;

    // gradient pre curve (bad sorted to the left..)
    grad_w = box_size;
    grad_x = pos_curve_x - (grad_w + pad);
    grad_y = pos_curve_y;
    grad_h = amount;

    // current color bar
    currColor_x = slider_x + slider_w + pad;
    currColor_y = pos_curve_y;
}

//--------------------------------------------------------------
void ofxColorManager::loadPalette(string p)
{
    cout << "loadPalette: " << p << endl;

    string path = path_palettes + p + ".json";
    ofFile file(path);
    if (file.exists())
    {
        jsonin ji(file);
        ji >> data;

        cout << "palette name: " << data.name << endl;
        clearPalette();
        ofColor c;
        for (int i = 0; i< data.palette.size(); i++)
        {
            c = data.palette[i];
            cout << "palette " << i << ": " << ofToString(c) << endl;
            add_color(c);
        }
    }
}

//--------------------------------------------------------------
void ofxColorManager::savePalette(string p)
{
    cout << "savePalette: " << p << endl;

    string path = path_palettes + p + ".json";

    data.name = "myPalette";
    cout << "palette name: " << data.name << endl;

    data.palette.resize(palette.size());
    for (int i = 0; i< palette.size(); i++)
    {
        data.palette[i] = palette[i];
        cout << "palette_" << i << " " << ofToString(data.palette[i]) << endl;
    }

    ofFile file(path, ofFile::WriteOnly);
    jsonout jo(file);
    jo << data;
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

    int i = btns_palette.size();

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
//        btn->placeNextTo(*btns_palette[i-1], Node::RIGHT);
//    }

    // keep reference (we need it to update the nodes)
    btns_palette.push_back(btn);
}

//--------------------------------------------------------------
void ofxColorManager::update_Interface(){

    TouchManager::one().update();

    // COLORS OF CURRENT PALETTE
    for (int i=0; i<btns_palette.size(); i++)
    {
        btns_palette[i]->update();
    }

//    // PALETTES
//    for (int i=0; i<btns_plt_Analog.size(); i++)
//    {
//        btns_plt_Analog[i]->update();
//    }
//    for (int i=0; i<btns_plt_CompSat.size(); i++)
//    {
//        btns_plt_CompSat[i]->update();
//    }

    //-

    // PALETTES

    // 1. triad
    for (int i = 0 ; i < btns_plt_Triad.size(); i++)
    {
        btns_plt_Triad[i]->update();
    }

    // 2. complement triad
    for (int i = 0 ; i < btns_plt_ComplTriad.size(); i++)
    {
        btns_plt_ComplTriad[i]->update();
    }

    // 3. complement sat
    for (int i = 0 ; i < btns_plt_CompSat.size(); i++)
    {
        btns_plt_CompSat[i]->update();
    }

    // 4. complement brgt
    for (int i = 0 ; i < btns_plt_ComplBrgt.size(); i++)
    {
        btns_plt_ComplBrgt[i]->update();
    }

    // 5. mono sat
    for (int i = 0 ; i < btns_plt_MonoSat.size(); i++)
    {
        btns_plt_MonoSat[i]->update();
    }

    // 6. mono brgt
    for (int i = 0 ; i < btns_plt_MonoBrgt.size(); i++)
    {
        btns_plt_MonoBrgt[i]->update();
    }

    // 7. analogue
    for (int i=0; i<btns_plt_Analog.size(); i++)
    {
        btns_plt_Analog[i]->update();
    }

    // 8. random
    for (int i = 0 ; i < btns_plt_Random.size(); i++)
    {
        btns_plt_Random[i]->update();
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
            // GENERAL DATA
            if (ofxImGui::BeginTree(this->params_data, mainSettings))
            {
                ofxImGui::AddParameter(this->color_backGround, true);

                ofxImGui::EndTree(mainSettings);
            }

            // COLOR
            if (ofxImGui::BeginTree(this->params_control, mainSettings))
            {
                ofxImGui::AddParameter(this->myColor, true);
                ofxImGui::AddParameter(this->bRandomColor);
                ofxImGui::AddParameter(this->bAddColor);
                ofxImGui::AddParameter(this->bRemoveColor);
                ofxImGui::AddParameter(this->bClearPalette);

                ofxImGui::EndTree(mainSettings);
            }

            // ALGORITHMIC PALETTE
            if (ofxImGui::BeginTree(this->params_palette, mainSettings))
            {
                ofxImGui::AddParameter(this->SATURATION);
                ofxImGui::AddParameter(this->BRIGHTNESS);
                ofxImGui::AddParameter(this->bRandomPalette);

                ofxImGui::EndTree(mainSettings);
            }

            // CURVE
            if (ofxImGui::BeginTree(this->params_curve, mainSettings))
            {
                ofxImGui::AddParameter(this->bResetCurve);
                ofxImGui::AddParameter(this->curve_pos);
                ofxImGui::AddParameter(this->gradient_hard);

                ofxImGui::EndTree(mainSettings);
            }
        }
        ofxImGui::EndWindow(mainSettings);
    }
    this->gui.end();

    return mainSettings.mouseOverGui;
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
    curveShow = true;
    cnt = 0;
    // amount = 256;

    curvesTool.setup(amount);
    curvesTool.load("curves.yml"); //needed because it fills polyline

    img.allocate(pos_curve_prev_w, pos_curve_prev_h, OF_IMAGE_COLOR);

    curve_pos.set("CURVE POS", 0., 0., 1.);// pct value 0. to 1.
    bResetCurve.set("RESET CURVE", false);

    params_curve.add(curve_pos);
    params_curve.add(bResetCurve);

    ofAddListener(params_curve.parameterChangedE(), this, &ofxColorManager::Changed_control);

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

    if (curveShow)
    {
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

        //--

        // CURVE TOOL

        ofPushMatrix();
        ofPushStyle();

        ofTranslate(pos_curve_x, pos_curve_y);

        ofSetColor(255);

        // curve editor
        curvesTool.draw(0, 0, cnt);

        // box LUT gradiant
        img.draw(pos_curve_prev_x, pos_curve_prev_y);

        float y =  amount-curvesTool[cnt];
//    float y = amount-curvesTool.getAt(cnt);

        // horizontal line
//        ofSetColor(ofColor::red);
        ofSetColor(ofColor::white);

        ofDrawLine(0, y, amount, y);
//    ofDrawLine(amount + slider_w, y, amount + slider_w + pad + pos_curve_prev_w + 100, y);

//        // current circle point
//        ofSetColor(25);
//        ofDrawCircle(cnt, y, 3);

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
            btn->placeNextTo(*btns_plt_Triad[i-1], Node::RIGHT);
        }
        btns_plt_Triad.push_back(btn);
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
            btn->placeNextTo(*btns_plt_ComplTriad[i-1], Node::RIGHT);
        }
        btns_plt_ComplTriad.push_back(btn);
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
            btn->placeNextTo(*btns_plt_CompSat[i-1], Node::RIGHT);
        }
        btns_plt_CompSat.push_back(btn);
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
            btn->placeNextTo(*btns_plt_ComplBrgt[i-1], Node::RIGHT);
        }
        btns_plt_ComplBrgt.push_back(btn);
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
            btn->placeNextTo(*btns_plt_MonoSat[i-1], Node::RIGHT);
        }
        btns_plt_MonoSat.push_back(btn);
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
            btn->placeNextTo(*btns_plt_MonoBrgt[i-1], Node::RIGHT);
        }
        btns_plt_MonoBrgt.push_back(btn);
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
            btn->placeNextTo(*btns_plt_Analog[i-1], Node::RIGHT);
        }
        btns_plt_Analog.push_back(btn);
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
            btn->placeNextTo(*btns_plt_Random[i-1], Node::RIGHT);
        }
        btns_plt_Random.push_back(btn);
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
    for (int i = 0 ; i < btns_plt_Triad.size(); i++)
    {
        btns_plt_Triad[i]->setColor(triad[i]);
    }

    // 2. complement triad
    for (int i = 0 ; i < btns_plt_ComplTriad.size(); i++)
    {
        btns_plt_ComplTriad[i]->setColor(complementTriad[i]);
    }

    // 3. complement sat
    for (int i = 0 ; i < btns_plt_CompSat.size(); i++)
    {
        btns_plt_CompSat[i]->setColor(complement[i]);
    }

    // 4. complement brgt
    for (int i = 0 ; i < btns_plt_ComplBrgt.size(); i++)
    {
        btns_plt_ComplBrgt[i]->setColor(complementBrightness[i]);
    }

    // 5. mono sat
    for (int i = 0 ; i < btns_plt_MonoSat.size(); i++)
    {
        btns_plt_MonoSat[i]->setColor(monochrome[i]);
    }

    // 6. mono brgt
    for (int i = 0 ; i < btns_plt_MonoBrgt.size(); i++)
    {
        btns_plt_MonoBrgt[i]->setColor(monochromeBrightness[i]);
    }

    // 7. analogue
    for (int i=0; i<btns_plt_Analog.size(); i++)
    {
        btns_plt_Analog[i]->setColor(analogue[i]);
    }

    // 8. random
    for (int i = 0 ; i < btns_plt_Random.size(); i++)
    {
        btns_plt_Random[i]->setColor(random[i]);
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
void ofxColorManager::add_color(ofColor c)
{
    ofLogNotice("ofxColorManager") << "add color " << " (" << ofToString(c) << ") to palette";
    palette.push_back( c );
    gradient.addColor( c );
    add_color_Interface(c);
}

//--------------------------------------------------------------
void ofxColorManager::remove_colorLast()
{
    if (palette.size() > 0) {
        palette.pop_back();
        gradient.removeColorLast();

        if (btns_palette.size() > 0)
        {
            std::string n = ("btn" + ofToString(btns_palette.size()-1));
            auto a = scene->getChildWithName(n, 100);
            auto b = a->getName();
            scene->removeChild(a, false);
            cout << "removed children: " << b << endl;
        }
        btns_palette.pop_back();
    }
}

//--------------------------------------------------------------
void ofxColorManager::clearPalette()
{
    cout << endl;
    cout << "clearPalette" << endl;

    palette.clear();
    gradient.reset();

    cout << "getNumChildren: " << scene->getNumChildren() << endl;

    for (int i=0; i< btns_palette.size(); i++)
    {
        std::string n = ("btn" + ofToString(i));
        auto a = scene->getChildWithName(n, 100);
        auto b = a->getName();
        scene->removeChild(a, false);
        cout << "removed children: " << b << endl;
    }
    btns_palette.clear();

    cout << endl;
}

//--------------------------------------------------------------
void ofxColorManager::Changed_control(ofAbstractParameter &e) {
    string name = e.getName();

    //TODO: reduce callbacks..
    if (name != "CURVE POS")
        ofLogNotice() << "Changed_control: " << name << ":" << e;

    // PALLETE
    if (name == "RANDOM COLOR")
    {
        if (bRandomColor)
        {
            bRandomColor= false;
            myColor = ofFloatColor(ofRandom(0., 1.), ofRandom(0., 1.), ofRandom(0., 1.));
        }
    }
    else if (name == "ADD COLOR")
    {
        if (bAddColor)
        {
            bAddColor= false;
            add_color(ofColor(myColor.get()));
        }
    }
    else if (name == "REMOVE COLOR")
    {
        if (bRemoveColor)
        {
            bRemoveColor= false;
            remove_colorLast();
        }
    }
    else if (name == "CLEAR PALETTE")
    {
        if (bClearPalette)
        {
            bClearPalette= false;
            clearPalette();
        }
    }
    else if (name == "RANDOMIZE")
    {
        if (bRandomPalette)
        {
            bRandomPalette = false;
            random.generateRandom();
        }
    }

    // CURVE
    else if (name == "CURVE POS")
    {
//        if ( )
//        {
//        }
    }
    else if (name == "RESET CURVE")
    {
        if (bResetCurve)
        {
            bResetCurve = false;
            curvesTool.clear();
            curvesTool.add(ofVec2f(0, 0));
            curvesTool.add(ofVec2f(255, 255));
        }
    }
    else if (name == "GRADIENT HARD")
    {
            gradient.setHardMode(gradient_hard);
    }
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

//    if (key == 'c') {
//        curveShow = !curveShow;
//    }
//    if (key == 's') {
//        curvesTool.save("curves.yml");
//    }
//    if (key == 'l') {
//        curvesTool.load("curves.yml");
//    }

    if (key == 's')
    {
        savePalette("myPalette");
    }
    if (key == 'l')
    {
        loadPalette("myPalette");
    }
    if (key == 'c')
    {
        clearPalette();
    }
    if (key == 'x')
    {
        remove_colorLast();
    }

    if (key == ' ')
    {
        myColor = ofFloatColor(ofRandom(0., 1.), ofRandom(0., 1.), ofRandom(0., 1.));
        add_color(ofColor(myColor.get()));
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

//--------------------------------------------------------------
void ofxColorManager::exit()
{
    savePalette("myPalette");
    save_group_XML(XML_params, XML_path);

    removeKeysListeners();
    removeMouseListeners();
    ofRemoveListener(params_control.parameterChangedE(), this, &ofxColorManager::Changed_control);
    ofRemoveListener(params_palette.parameterChangedE(), this, &ofxColorManager::Changed_control);
    ofRemoveListener(params_curve.parameterChangedE(), this, &ofxColorManager::Changed_control);

}