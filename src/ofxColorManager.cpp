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
    //-

    mouseRuler.setup();
    mouseRuler.toggleVisibility();

    //-

    color_picked.addListener(this, &ofxColorManager::Changed_color_picked);
    color_clicked_param.addListener(this, &ofxColorManager::Changed_color_clicked);

    //-

    dt = 1/30.0f;//TODO: should be setted externally

    //-

    // LAYOUT

    gui_setup_layout();

    //-

    // COLOR BROWSER

    ColorBrowser.setBoxSize(25);
    ColorBrowser.setRowsSize(10);
    ColorBrowser.setup_colorBACK(color_BACK);
    ColorBrowser.setPosition(colorBrowserPos);
    ColorBrowser.setup();

    //-

    // DATA
    setBackground_ENABLE(true);
    color_backGround.set("BACKGROUND", ofFloatColor::white);

    params_data.setName("GENERAL");
    params_data.add(color_backGround);

    //--

    // COLOR

    color_picked.set("COLOR", ofFloatColor::red);
    color_HUE.set("HUE", 0, 0, 255);
    color_SAT.set("SATURATION", 0, 0, 255);
    color_BRG.set("BRIGHTNESS", 0, 0, 255);

    params_color.setName("COLOR");
    params_color.add(color_HUE);
    params_color.add(color_SAT);
    params_color.add(color_BRG);

    ofAddListener(params_color.parameterChangedE(), this, &ofxColorManager::Changed_control);

    //-

    // CONTROL

    bRandomColor.set("RANDOM COLOR", false);
    bAddColor.set("ADD COLOR", false);
    bRemoveColor.set("REMOVE COLOR", false);
    bClearPalette.set("CLEAR PALETTE", false);

    params_control.setName("COLOR EDITOR");
    params_control.add(color_picked);
    params_control.add(bRandomColor);
    params_control.add(bAddColor);
    params_control.add(bRemoveColor);
    params_control.add(bClearPalette);

    ofAddListener(params_control.parameterChangedE(), this, &ofxColorManager::Changed_control);

    //-

    // ALGORITHMIC PALETTE

    MODE_Palette.set("FROM COLOR", false);
    SATURATION.set("SATURATION", 200, 0, 255 );
    BRIGHTNESS.set("BRIGHTNESS", 200, 0, 255 );
    bRandomPalette.set("RANDOM PALETTE", false);
    bAutoTrigPalette.set("AUTO TRIG", false);
    NUM_ALGO_PALETTES.set("SIZE", 6, 2, 8);
    params_palette.setName("ALGORITHMIC PALETTE");
    params_palette.add(MODE_Palette);
    params_palette.add(SATURATION);
    params_palette.add(BRIGHTNESS);
    params_palette.add(bRandomPalette);
    params_palette.add(NUM_ALGO_PALETTES);
    params_palette.add(bAutoTrigPalette);

    ofAddListener(params_palette.parameterChangedE(), this, &ofxColorManager::Changed_control);

    //-

    // INTERFACE

    interface_setup();

    //-

    // PALETTES

    random.generateRandom(NUM_ALGO_PALETTES);
    palettes_update();
    palettes_setup();
    palettes_setup_labels();

    //-

    // GRADIENT

    gradient.reset();
    gradient_hard.set("GRADIENT HARD", false);
    gradient.setHardMode(gradient_hard);
    gradient.setDrawVertical(true);
    gradient.setDrawDirFlip(true);

    params_curve.setName("GRADIENT CURVE");
    params_curve.add(gradient_hard);

    //-

    // CURVE TOOL

    curveTool_setup();

    //--

    // GUI

    // font
    ImGuiIO& io = ImGui::GetIO();
    string inputPath;
    inputPath = ofFilePath::getAbsolutePath("assets/fonts/PragmataProR_0822.ttf");
    const char* myPath = inputPath.c_str();
    ImFontConfig config;
    config.OversampleH = 3;
    config.OversampleV = 1;
    config.GlyphExtraSpacing.x = 1.0f;
    io.Fonts->AddFontFromFileTTF(myPath, 13.0f, &config);

    // create
    this->gui.setup();
    this->guiVisible = true;

    // theme colors
    gui_imGui_theme();

    //--

    // LISTENERS

    addKeysListeners();
    addMouseListeners();

    //--

    // STARTUP SETTINGS

    XML_params.setName("ofxColorManager");
    XML_params.add(color_backGround);
    XML_params.add(color_picked);
//    XML_params.add(color_HUE);
//    XML_params.add(color_SAT);
//    XML_params.add(color_BRG);
    XML_params.add(MODE_Palette);//algorithmic palette
    XML_params.add(BRIGHTNESS);
    XML_params.add(SATURATION);
    XML_params.add(gradient_hard);//gradient
    XML_params.add(NUM_ALGO_PALETTES);
    XML_params.add(bAutoTrigPalette);
    load_group_XML(XML_params, XML_path);

    palette_load("myPalette");

    //-

//    color_picked.addListener(this, &ofxColorManager::Changed_color_picked);
//    color_clicked_param.addListener(this, &ofxColorManager::Changed_color_clicked);
}

//--------------------------------------------------------------
void ofxColorManager::interface_setup()
{
    scene = new Node();
    scene->setSize(ofGetWidth(), ofGetHeight());
    scene->setName("Scene");
    TouchManager::one().setup(scene);
}

//--------------------------------------------------------------
void ofxColorManager::gui_setup_layout()
{
    // LAYOUT

    box_size = 40;

    // global mini pad between panels/objects
    pad = 2;

    // gui ImGui panel
    gui_x = 10;
    gui_y = 10;
    gui_w = 200;
    gui_h = 470;//estimate (should measure) height of the panel on window resize

    // algorithmic palettes
    palettes_x = gui_x + 10;
    palettes_y = (gui_y + gui_h + pad) + box_size + 10;

    // curve tool pos (anchor for others)
    curveTool_x = 525;
    curveTool_y = 25;
    curveTool_w = 256;
    curveTool_h = 256;

    // gradient curved image LUT [referenced to curve pos (vertical)]
    image_curvedGradient_x = curveTool_amount + pad;//curveTool +
    image_curvedGradient_y = 0;//curveTool +
    image_curvedGradient_w = box_size;
    image_curvedGradient_h = curveTool_amount;

    // slider
    slider_x = curveTool_x + pad + curveTool_amount + pad + image_curvedGradient_w;
    slider_y = curveTool_y;
    slider_w = box_size / 2;
    slider_h = curveTool_amount;

    // gradient-pre curve (bad sorted to the left but anchored to curve..)
    grad_w = box_size;
    grad_x = curveTool_x - (grad_w + pad);
    grad_y = curveTool_y;
    grad_h = curveTool_amount;

    // user palette (related gradient-pre curve)
    palette_x = grad_x - (grad_w + pad);
    palette_y = curveTool_y + grad_h;

    // current color bar
    currColor_x = slider_x + slider_w + pad;
    currColor_y = curveTool_y;

    // color box picker
    color_x = 335;
    color_y = 20;
    color_w = color_h = 2*box_size;
    r_color_picked = ofRectangle( color_x, color_y, color_w, color_h );

    // color box clicked
    colorPick_x = 335;
    colorPick_y = color_h + 30;
    colorPick_w = colorPick_h = 2*box_size;
    r_color_clicked = ofRectangle( colorPick_x, colorPick_y, colorPick_w, colorPick_h );

    // color browser
    colorBrowserPos = glm::vec2(910, 35);
}

//--------------------------------------------------------------
void ofxColorManager::palette_load(string p)
{
    ofLogNotice("ofxColorManager") << "palette_load: " << p;

    string path = path_palettes + p + ".json";
    ofFile file(path);
    if (file.exists())
    {
        jsonin ji(file);
        ji >> data;

        ofLogNotice("ofxColorManager") << "palette name: " << data.name;
        palette_clear();
        ofColor c;
        for (int i = 0; i< data.palette.size(); i++)
        {
            c = data.palette[i];
            ofLogNotice("ofxColorManager") << "palette " << i << ": " << ofToString(c);
            palette_addColor(c);
        }
    }
    else
    {
        ofLogNotice("ofxColorManager") << "FILE '" << path << "' NOT FOUND";
    }
}

//--------------------------------------------------------------
void ofxColorManager::palette_save(string p)
{
    ofLogNotice("ofxColorManager") << "palette_save: " << p;

    string path = path_palettes + p + ".json";

    data.name = "myPalette";
    ofLogNotice("ofxColorManager") << "palette name: " << data.name;

    data.palette.resize(palette.size());
    for (int i = 0; i< palette.size(); i++)
    {
        data.palette[i] = palette[i];
        ofLogNotice("ofxColorManager") << "palette_" << i << " " << ofToString(data.palette[i]);
    }

    ofFile file(path, ofFile::WriteOnly);
    jsonout jo(file);
    jo << data;
}

//--------------------------------------------------------------
void ofxColorManager::gui_imGui_theme()
{
    // must be done after setup the gui

    //-

    // widgets color
    ofColor myColor1;
    ofColor myColor2;
    ofColor myColor3;
    ofColor myColor4;
    int grayDark = 0;
    int gray = 24;
    int gray2 = 16;
    int gray4 = 128;
    myColor1 = ofColor(grayDark, grayDark, grayDark, 255);//black
    myColor2 = ofColor(gray, gray, gray, 255);//gray light
    myColor3 = ofColor(gray2, gray2, gray2, 255);//gray darke
    myColor4 = ofColor(gray4, gray4, gray4, 255);//white

    ImGuiStyle *style = &ImGui::GetStyle();
    style->WindowRounding = (3.0f);
//    style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(myColor1, 1.00f);
//    style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(myColor2, 0.21f);
//    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(myColor1, 0.5f);
//    style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(myColor2, 1.00f);
    style->Colors[ImGuiCol_CheckMark] = ImVec4(ofColor(255), 0.80f);
    style->Colors[ImGuiCol_Button] = ImVec4(myColor3, 0.8f);
    style->Colors[ImGuiCol_ButtonHovered] = ImVec4(myColor1, 0.86f);
    style->Colors[ImGuiCol_ButtonActive] = ImVec4(myColor1, 1.00f);
    style->Colors[ImGuiCol_TitleBg] = ImVec4(myColor1, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(myColor1, 0.75f);
    style->Colors[ImGuiCol_TitleBgActive] = ImVec4(myColor1, 1.00f);
    style->Colors[ImGuiCol_Header] = ImVec4(myColor1, 0.76f);
    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(myColor2, 0.86f);
    style->Colors[ImGuiCol_HeaderActive] = ImVec4(myColor1, 1.00f);
    style->Colors[ImGuiCol_FrameBg] = ImVec4(myColor1, 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(myColor2, 0.40f);//hover
    style->Colors[ImGuiCol_FrameBgActive] = ImVec4(myColor1, 1.00f);
    style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(myColor4, 1.f);
    style->Colors[ImGuiCol_SliderGrab] = ImVec4(myColor4, 0.4f);
}

//--------------------------------------------------------------
void ofxColorManager::palette_addColor_toInterface(ofColor c)
{
////     horizontal palette
//    palette_x = gui_x;
//    int perRow = 5;
//    int i = btns_palette.size();
//    palette_x += i * ( box_size + pad );
//
//    ButtonExample *btn = new ButtonExample();
//    btn->setup(palette_x, palette_y, box_size, box_size);
//    btn->setColor(c);
//    btn->setup_colorBACK( color_clicked );
//    btn->setLocked(true);
//    btn->setName("btn" + ofToString(i));
//    scene->addChild(btn);
//    btns_palette.push_back(btn);

    // vertical palette
    int i = btns_palette.size();
    float y = palette_y - box_size;
    y = y - i * (box_size + pad);

    ButtonExample *btn = new ButtonExample();
    btn->setup(palette_x, y, box_size, box_size);
    btn->setColor(c);
    btn->setup_colorBACK( color_clicked );
    btn->setLocked(true);
    btn->setName("btn" + ofToString(i));
    scene->addChild(btn);
    btns_palette.push_back(btn);
}

//--------------------------------------------------------------
void ofxColorManager::interface_update(){

    TouchManager::one().update();

    // EACH COLOR OF CURRENT PALETTE
    for (int i=0; i<btns_palette.size(); i++)
    {
        btns_palette[i]->update();
    }

    // ALGORITHMIC PALETTES SELECTORS
    for (int i=0; i<btns_plt_Selector.size(); i++)
    {
        btns_plt_Selector[i]->update(dt);
    }

    //-

    // ALGORITHMIC PALETTES COLORS

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
void ofxColorManager::interface_draw(){
    scene->render();

    if (bShowDebug)
    {
        scene->renderDebug();
    }
}

//--------------------------------------------------------------
bool ofxColorManager::gui_imGui()
{
    auto mainSettings = ofxImGui::Settings();
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
                ofxImGui::AddParameter(this->bRandomColor);
                ofxImGui::AddParameter(this->color_picked, true);
                ofxImGui::AddParameter(this->color_HUE);
                ofxImGui::AddParameter(this->color_SAT);
                ofxImGui::AddParameter(this->color_BRG);

                ImGui::Text("PALETTE MANAGER");
                ofxImGui::AddParameter(this->bAddColor);
                ofxImGui::AddParameter(this->bRemoveColor);
                ofxImGui::AddParameter(this->bClearPalette);
                ofxImGui::EndTree(mainSettings);
            }

            // ALGORITHMIC PALETTE
            if (ofxImGui::BeginTree(this->params_palette, mainSettings))
            {
                ofxImGui::AddParameter(this->bRandomPalette);
                ofxImGui::AddParameter(this->MODE_Palette);
                if (!MODE_Palette) {
                    ofxImGui::AddParameter(this->SATURATION);
                    ofxImGui::AddParameter(this->BRIGHTNESS);
                }
                ofxImGui::AddParameter(this->bAutoTrigPalette);
                ofxImGui::AddParameter(this->NUM_ALGO_PALETTES);
                ofxImGui::EndTree(mainSettings);
            }

            // CURVE
            if (ofxImGui::BeginTree(this->params_curve, mainSettings))
            {
                ofxImGui::AddParameter(this->bResetCurve);
//                ofxImGui::AddParameter(this->curve_pos);
                if (ofxImGui::AddParameter(this->curve_pos))
                {
                    curve_pos_slider.setPercent(curve_pos.get());
                }
                ofxImGui::AddParameter(this->curve_pos_out);
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
    //--

    // handle last selected algorithmic palette
    if (SELECTED_palette != SELECTED_palette_PRE)
    {
        ofLogNotice("ofxColorManager::update") << "CHANGED SELECTED_palette: " << SELECTED_palette;
        palettes_recall(SELECTED_palette);
        SELECTED_palette_LAST = SELECTED_palette;

        SELECTED_palette_PRE = SELECTED_palette = -1;//bug if not if pressed same button

        // hide all borders
        for (int p = 0; p< btns_plt_Selector.size() && p< NUM_PALETTES; p++)
        {
            btns_plt_Selector[p]->setBorder(false);
        }

        // set to draw border to selected palette
        if (btns_plt_Selector.size()>0)
        {
            btns_plt_Selector[SELECTED_palette_LAST]->setBorder(true);
        }
    }

    //-

    interface_update();
    palettes_update();//TODO: should reduce calls on update()
    curveTool_update();
    ColorBrowser.update();

    //-

    if (color_clicked != color_clicked_PRE )
    {
        color_clicked_PRE = color_clicked;
        ofLogNotice("ofxColorManager") << "CHANGED color_clicked";
        color_picked.set(color_clicked);
    }

    //-

    // set the local scope color pointer that is into ofxColorBrowser that whill be used as color picked too
    if (color_BACK != color_BACK_PRE )
    {
        color_BACK_PRE = color_BACK;
        ofLogNotice("ofxColorManager") << "CHANGED color_BACK";
        color_picked.set(color_BACK);

        if (bAutoTrigPalette)
        {
            palettes_update();
            palettes_recall(SELECTED_palette_LAST);//trig last choice
        }
    }
}

//#pragma mark curveTool

//--------------------------------------------------------------
void ofxColorManager::curveTool_setup()
{
    curve_img_gradient.allocate(image_curvedGradient_w, image_curvedGradient_h, OF_IMAGE_COLOR);

    curvesTool.setup(curveTool_amount);
    curvesTool.load("curves.yml"); //needed because it fills polyline

    curve_pos.set("INPUT", 0., 0., 1.);
    curve_pos_out.set("OUTPUT", 0., 0., 1.);
    bResetCurve.set("RESET CURVE", false);
    params_curve.add(curve_pos);
    params_curve.add(curve_pos_out);
    params_curve.add(bResetCurve);
    ofAddListener(params_curve.parameterChangedE(), this, &ofxColorManager::Changed_control);

    //-

    // slider
    curve_pos_slider.setup(slider_x, slider_y, slider_w, slider_h, 0., 1., 0, true, false);
//    curve_pos_slider.setLabelString("input");
}

//--------------------------------------------------------------
void ofxColorManager::curveTool_update()
{
    // TODO: add parameter to improve backwards mirroring
    // TODO: mirror to gui param

    // update values
    curve_pos = curve_pos_slider.getValue();
    curve_pos_LUT = (int) ofMap( curve_pos.get(), 0., 1., 0, curveTool_amount-1 );

    //--

    // vertical palette clors gradient rectangle modified by curveTool

    // every y point has different color
    for (int y=0; y<image_curvedGradient_h; y++)
    {
        float input_LUT = ofMap( y, 0, image_curvedGradient_h, 0, curveTool_amount-1 );
        float output_LUT = curvesTool[input_LUT];
        float output;
        output = ofMap( output_LUT, 0, curveTool_amount-1, 1., 0.) ;
        ofColor c = gradient.getColorAtPercent( output );

        // all x point has the same color
        for (int x = 0; x < image_curvedGradient_w; x++)
        {
            curve_img_gradient.setColor(x, y, c);
        }
    }
    curve_img_gradient.update();

    //-

    // UPDATE TRAGET COLOR POINTER IN ofApp

    if (color_TARGET != nullptr)//only if pointer is setted
    {
        float out = ofMap( curvesTool.getAtPercent(1.0-curve_pos), 0, curveTool_amount-1, 1., 0.) ;
        ofColor c = gradient.getColorAtPercent( out );
        color_TARGET->set(c);//TODO: should reduce calls
    }

    //-
}

//--------------------------------------------------------------
void ofxColorManager::curveTool_draw() {

    if (curveShow)
    {
        ofPushMatrix();
        ofPushStyle();

        //-

        ofRectangle r;
        r = ofRectangle( currColor_x, currColor_y, box_size, slider_h );

        //-

        // GRADIENT

        // 1. un-curved gradient rectangle (left positioned)
        gradient.drawDebug(grad_x, grad_y, grad_w, grad_h);

        // 2. current box color at input curve point (right positioned)
        float out = ofMap( curvesTool.getAtPercent(1.0-curve_pos), 0, curveTool_amount-1, 1., 0.) ;
        ofColor c = gradient.getColorAtPercent( out );

        ofPushStyle();
        ofFill();
        ofSetColor(c);
        ofDrawRectangle(r);
        ofPopStyle();

        // NOTE: sometimes we need some tricky hacking to avoid rare fliping from gradients
        curve_pos_out = ofMap( curvesTool.getAtPercent(curve_pos), 0, curveTool_amount-1, 0., 1.);

        // 3. curve tool
        ofTranslate(curveTool_x, curveTool_y);

        // 3.1 image box gradient LUT
        curve_img_gradient.draw(image_curvedGradient_x, image_curvedGradient_y);

        // 3.2 curve splines editor
//        ofPushMatrix();
        ofPushStyle();

        ofSetColor(255);
        curvesTool.draw(0, 0, curve_pos_LUT);

        // 3.3 horizontal line
        ofSetColor(ofColor::white);
        float y =  curveTool_amount - curvesTool[curve_pos_LUT];
        ofDrawLine(0, y, curveTool_amount, y);

        // 3.4 current circle point
        ofSetColor(25);
        ofDrawCircle(curve_pos_LUT, y, 3);

//        ofPopMatrix();
        ofPopStyle();
        //-

        ofPopMatrix();
        ofPopStyle();

//    // debug curve values
//    int posx, posy;
//    posx = 425;
//    posy = 20;
//    ofDrawBitmapStringHighlight("in : "+ofToString(curve_pos), glm::vec2(posx, posy));
//    ofDrawBitmapStringHighlight("out: "+ofToString(curve_pos_out), glm::vec2(posx, posy + 20));
//    ofDrawBitmapStringHighlight("lut: "+ofToString(curve_pos_LUT), glm::vec2(posx, posy + 40));
    }

    //-
}

//--------------------------------------------------------------
void ofxColorManager::palettes_setup() {
    int btn_pad_w;
    btn_pad_w = 210;//padding to place boxes right to the labels
    int x0;
    x0 = palettes_x + btn_pad_w;
    int y0 = palettes_y;
    int h0 = box_size + pad;

    // 1. triad
    for (int i = 0; i < triad.size(); i++) {
        ButtonExample *btn = new ButtonExample();
        btn->setup(x0, y0, box_size, box_size);
        btn->setup_colorBACK(color_clicked);
        btn->setLocked(true);
        btn->setName("triad" + ofToString(i));
        btn->setColor(triad[i]);
        scene->addChild(btn);
        btns_plt_Triad.push_back(btn);
        x0 += h0;
    }

    // 2. complement triad
    x0 = palettes_x + btn_pad_w;
    y0 += h0;
    for (int i = 0; i < complementTriad.size(); i++) {
        ButtonExample *btn = new ButtonExample();
        btn->setup(x0, y0, box_size, box_size);
        btn->setup_colorBACK(color_clicked);
        btn->setLocked(true);
        btn->setName("compTriad" + ofToString(i));
        btn->setColor(complementTriad[i]);
        scene->addChild(btn);
        btns_plt_ComplTriad.push_back(btn);
        x0 += h0;
    }

    // 3. complement sat
    x0 = palettes_x + btn_pad_w;
    y0 += h0;
    for (int i = 0; i < complement.size(); i++) {
        ButtonExample *btn = new ButtonExample();
        btn->setup(x0, y0, box_size, box_size);
        btn->setup_colorBACK(color_clicked);
        btn->setLocked(true);
        btn->setName("compSat" + ofToString(i));
        btn->setColor(complement[i]);
        scene->addChild(btn);
        btns_plt_CompSat.push_back(btn);
        x0 += h0;
    }

    // 4. complement brgt
    x0 = palettes_x + btn_pad_w;
    y0 += h0;
    for (int i = 0; i < complementBrightness.size(); i++) {
        ButtonExample *btn = new ButtonExample();
        btn->setup(x0, y0, box_size, box_size);
        btn->setup_colorBACK(color_clicked);
        btn->setLocked(true);
        btn->setName("compBrgt" + ofToString(i));
        btn->setColor(complementBrightness[i]);
        scene->addChild(btn);
        btns_plt_ComplBrgt.push_back(btn);
        x0 += h0;
    }

    // 5. mono sat
    x0 = palettes_x + btn_pad_w;
    y0 += h0;
    for (int i = 0; i < monochrome.size(); i++) {
        ButtonExample *btn = new ButtonExample();
        btn->setup(x0, y0, box_size, box_size);
        btn->setup_colorBACK(color_clicked);
        btn->setLocked(true);
        btn->setName("monoSat" + ofToString(i));
        btn->setColor(monochrome[i]);
        scene->addChild(btn);
        btns_plt_MonoSat.push_back(btn);
        x0 += h0;
    }

    // 6. mono brgt
    x0 = palettes_x + btn_pad_w;
    y0 += h0;
    for (int i = 0; i < monochromeBrightness.size(); i++) {
        ButtonExample *btn = new ButtonExample();
        btn->setup(x0, y0, box_size, box_size);
        btn->setup_colorBACK(color_clicked);
        btn->setLocked(true);
        btn->setName("monoBrgt" + ofToString(i));
        btn->setColor(monochromeBrightness[i]);
        scene->addChild(btn);
        btns_plt_MonoBrgt.push_back(btn);
        x0 += h0;
    }

    // 7. analogue
    x0 = palettes_x + btn_pad_w;
    y0 += h0;
    for (int i = 0; i < analogue.size(); i++) {
        ButtonExample *btn = new ButtonExample();
        btn->setup(x0, y0, box_size, box_size);
        btn->setup_colorBACK(color_clicked);
        btn->setLocked(true);
        btn->setName("analogue" + ofToString(i));
        btn->setColor(analogue[i]);
        scene->addChild(btn);
        btns_plt_Analog.push_back(btn);
        x0 += h0;
    }

    // 8. random
    x0 = palettes_x + btn_pad_w;
    y0 += h0;
    for (int i = 0; i < random.size(); i++) {
        ButtonExample *btn = new ButtonExample();
        btn->setup(x0, y0, box_size, box_size);
        btn->setup_colorBACK(color_clicked);
        btn->setLocked(true);
        btn->setName("random" + ofToString(i));
        btn->setColor(random[i]);
        scene->addChild(btn);
        btns_plt_Random.push_back(btn);
        x0 += h0;
    }
}

//--------------------------------------------------------------
void ofxColorManager::palettes_setup_labels()
{
    int x0 = palettes_x;
    int padH = 11;
    int y0 = palettes_y + padH;
    int h0 = box_size + pad;

    //--

    // LABELS PICKER : TODO: maybe can use simpler thing than ofxInterface for this

    // palettes labels

    ofColor btn_plt_c;
    btn_plt_c.set(ofColor(32));
    int btn_plt_h;
    btn_plt_h = h0;
    x0 = palettes_x;

    //-

    //TODO: BUG
    btns_plt_Selector.clear();

    for (int p = 0; p< NUM_PALETTES; p++)
    {
        ButtonPaletteSelector *btn = new ButtonPaletteSelector();
        switch (p)
        {
            case 0:
                btn->setup("TRIAD");
                btn->setThisPaletteType(0);
                btn->set_SELECTED_palette(SELECTED_palette);
                break;
            case 1:
                btn->setup("COMPLEMENT TRIAD");
                btn->setThisPaletteType(1);
                btn->set_SELECTED_palette(SELECTED_palette);
                break;
            case 2:
                btn->setup("COMPLEMENT (SATURATION)");
                btn->setThisPaletteType(2);
                btn->set_SELECTED_palette(SELECTED_palette);
                break;
            case 3:
                btn->setup("COMPLEMENT (BRIGHTNESS)");
                btn->setThisPaletteType(3);
                btn->set_SELECTED_palette(SELECTED_palette);
                break;
            case 4:
                btn->setup("MONOCHROME (SATURATION)");
                btn->setThisPaletteType(4);
                btn->set_SELECTED_palette(SELECTED_palette);
                break;
            case 5:
                btn->setup("MONOCHROME (BRIGHTNESS)");
                btn->setThisPaletteType(5);
                btn->set_SELECTED_palette(SELECTED_palette);
                break;
            case 6:
                btn->setup("ANALOGUE");
                btn->setThisPaletteType(6);
                btn->set_SELECTED_palette(SELECTED_palette);
                break;
            case 7:
                btn->setup("RANDOM");
                btn->setThisPaletteType(7);
                btn->set_SELECTED_palette(SELECTED_palette);
                break;
        }

        btn->setPosition(x0, y0 + p*btn_plt_h);
        btn->setBGColor(btn_plt_c);
        btn->setLabelColor(ofColor::white);
        btn->setBorderColor(ofColor::white);
        btn->setBorder(false);
        scene->addChild(btn);
        btns_plt_Selector.push_back(btn);
    }

    // set to draw border to selected palette
    if ((btns_plt_Selector.size()>0) && (btns_plt_Selector.size()>SELECTED_palette_LAST))
    {
        btns_plt_Selector[SELECTED_palette_LAST]->setBorder(true);
    }
}

//--------------------------------------------------------------
void ofxColorManager::palettes_recall(int p)
{
    palette_clear();
    ofColor color;

    switch (p)
    {
        case 0:
            for (int i = 0; i < btns_plt_Triad.size(); i++) {
                color = btns_plt_Triad[i]->getColor();
                palette_addColor(color);
            }
            break;
        case 1:
            for (int i = 0; i < btns_plt_ComplTriad.size(); i++) {
                color = btns_plt_ComplTriad[i]->getColor();
                palette_addColor(color);
            }
            break;
        case 2:
            for (int i = 0; i < btns_plt_CompSat.size(); i++) {
                color = btns_plt_CompSat[i]->getColor();
                palette_addColor(color);
            }
            break;
        case 3:
            for (int i = 0; i < btns_plt_ComplBrgt.size(); i++) {
                color = btns_plt_ComplBrgt[i]->getColor();
                palette_addColor(color);
            }
            break;
        case 4:
            for (int i = 0; i < btns_plt_MonoSat.size(); i++) {
                color = btns_plt_MonoSat[i]->getColor();
                palette_addColor(color);
            }
            break;
        case 5:
            for (int i = 0; i < btns_plt_MonoBrgt.size(); i++) {
                color = btns_plt_MonoBrgt[i]->getColor();
                palette_addColor(color);
            }
            break;
        case 6:
            for (int i = 0; i < btns_plt_Analog.size(); i++) {
                color = btns_plt_Analog[i]->getColor();
                palette_addColor(color);
            }
            break;
        case 7:
            for (int i = 0; i < btns_plt_Random.size(); i++) {
                color = btns_plt_Random[i]->getColor();
                palette_addColor(color);
            }
            break;
    }
}

//--------------------------------------------------------------
void ofxColorManager::palettes_update()
{
    ofColor base;
    if (!MODE_Palette)
    {
        // using hue only from picked color and sat/(brg from sliders
        base = ofColor::fromHsb(ofMap(color_picked.get().getHue(), 0., 1., 0, 255), SATURATION, BRIGHTNESS);
    }
    else
    {
        // check using hue + sat/brg from color
        base = ofColor::fromHsb(ofMap(color_picked.get().getHue(), 0., 1., 0, 255), color_SAT, color_BRG);
    }

    complement.setBaseColor(base);
    complementBrightness.setBaseColor(base);
    triad.setBaseColor(base);
    complementTriad.setBaseColor(base);
    monochrome.setBaseColor(base);
    monochromeBrightness.setBaseColor(base);
    analogue.setBaseColor(base);
    random.setBaseColor(base);

    complement.generateComplementary(ofxColorPalette::SATURATION, NUM_ALGO_PALETTES);
    complementBrightness.generateComplementary(ofxColorPalette::BRIGHTNESS, NUM_ALGO_PALETTES);
    triad.generateTriad();
    complementTriad.generateComplementaryTriad();
    monochrome.generateMonoChromatic(ofxColorPalette::SATURATION, NUM_ALGO_PALETTES);
    monochromeBrightness.generateMonoChromatic(ofxColorPalette::BRIGHTNESS, NUM_ALGO_PALETTES);
    analogue.generateAnalogous(NUM_ALGO_PALETTES, 0.2);

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
void ofxColorManager::palettes_resize()
{
    //-

    // USER PALETTE

//    palette.clear();//TODO:
    palette_clear();

    //-

    // ALGORITMIC PALETTE

    for (int i=0; i< btns_plt_Triad.size(); i++)
    {
        std::string n = ("triad" + ofToString(i));
        auto a = scene->getChildWithName(n, 1000);
        auto b = a->getName();
        scene->removeChild(a, false);
        ofLogVerbose("ofxColorManager") << "removed children: " << b;
    }
    btns_plt_Triad.clear();

    for (int i=0; i< btns_plt_ComplTriad.size(); i++)
    {
        std::string n = ("compTriad" + ofToString(i));
        auto a = scene->getChildWithName(n, 1000);
        auto b = a->getName();
        scene->removeChild(a, false);
        ofLogVerbose("ofxColorManager") << "removed children: " << b;
    }
    btns_plt_ComplTriad.clear();

    for (int i=0; i< btns_plt_CompSat.size(); i++)
    {
        std::string n = ("compSat" + ofToString(i));
        auto a = scene->getChildWithName(n, 1000);
        auto b = a->getName();
        scene->removeChild(a, false);
        ofLogVerbose("ofxColorManager") << "removed children: " << b;
    }
    btns_plt_CompSat.clear();

    for (int i=0; i< btns_plt_ComplBrgt.size(); i++)
    {
        std::string n = ("compBrgt" + ofToString(i));
        auto a = scene->getChildWithName(n, 1000);
        auto b = a->getName();
        scene->removeChild(a, false);
        ofLogVerbose("ofxColorManager") << "removed children: " << b;
    }
    btns_plt_ComplBrgt.clear();

    for (int i=0; i< btns_plt_MonoSat.size(); i++)
    {
        std::string n = ("monoSat" + ofToString(i));
        auto a = scene->getChildWithName(n, 1000);
        auto b = a->getName();
        scene->removeChild(a, false);
        ofLogVerbose("ofxColorManager") << "removed children: " << b;
    }
    btns_plt_MonoSat.clear();

    for (int i=0; i< btns_plt_MonoBrgt.size(); i++)
    {
        std::string n = ("monoBrgt" + ofToString(i));
        auto a = scene->getChildWithName(n, 1000);
        auto b = a->getName();
        scene->removeChild(a, false);
        ofLogVerbose("ofxColorManager") << "removed children: " << b;
    }
    btns_plt_MonoBrgt.clear();

    for (int i=0; i< btns_plt_Analog.size(); i++)
    {
        std::string n = ("analogue" + ofToString(i));
        auto a = scene->getChildWithName(n, 1000);
        auto b = a->getName();
        scene->removeChild(a, false);
        ofLogVerbose("ofxColorManager") << "removed children: " << b;
    }
    btns_plt_Analog.clear();

    for (int i=0; i< btns_plt_Random.size(); i++)
    {
        std::string n = ("random" + ofToString(i));
        auto a = scene->getChildWithName(n, 1000);
        auto b = a->getName();
        scene->removeChild(a, false);
        ofLogVerbose("ofxColorManager") << "removed children: " << b;
    }
    btns_plt_Random.clear();

    //-

    random.generateRandom(NUM_ALGO_PALETTES);
    palettes_update();
    palettes_setup();
//    palettes_recall(SELECTED_palette);

    //-

    // TODO: BUG
//    if (bAutoTrigPalette)
//    {
//        palettes_update();
        palettes_recall(SELECTED_palette_LAST);//trig last choice
//    }
}

//--------------------------------------------------------------
void ofxColorManager::draw()
{
    ofPushMatrix();
    ofPushStyle();

    if (SHOW_ALL_GUI) {

        //-

        // BACKGROUND

        if (backgroundENABLE)
        {
            ofClear(ofColor(color_backGround.get()));
        }

        //-

        // COLOR BOX PICKER (CURRENT)

        ofPushStyle();
        ofFill();
        ofSetColor(ofColor(color_picked.get()));
        ofDrawRectangle(r_color_picked);
        ofPopStyle();

        //-

        // COLOR BOX CLICKED

//    ofPushStyle();
//    ofFill();
//
//    ofSetColor( ofColor( color_clicked ) );
////    ofSetColor( ofColor( color_clicked_param.get() ) );
//
//    ofDrawRectangle(r_color_clicked);
//    ofPopStyle();

        //-

        // INTERFACE
        interface_draw();

        // CURVE
        curveTool_draw();

        // COLORS BROWSER
        ColorBrowser.draw();

        //-

        // GUI

        this->mouseOverGui = false;
        if (this->guiVisible) {
            this->mouseOverGui = this->gui_imGui();
        }
        if (this->mouseOverGui) {
        } else {
        }

        //--
    }

    if (SHOW_GUI_MINI)
    {
        draw_PaleteMINI();
    }

    ofPopMatrix();
    ofPopStyle();
}

//--------------------------------------------------------------
void ofxColorManager::draw_PaleteMINI()
{
    int boxW = 40;
    int boxPad = 1;
    int boxSize = boxW+boxPad;
    glm::vec2 palettePos = glm::vec2(ofGetWidth()-palette.size()*boxSize, 2*boxPad);
    ofRectangle r;

    ofPushMatrix();
    ofPushStyle();
    ofTranslate(palettePos);

    for (int col=0; col<palette.size(); col++)
    {
        ofTranslate ( boxSize, 0);
        r = ofRectangle(0, 0, boxW, boxW);
        ofFill();
        ofSetColor(palette[col]);
        ofDrawRectangle(r);
    }

    ofPopStyle();
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofxColorManager::palette_addColor(ofColor c)
{
    ofLogNotice("ofxColorManager") << "add color " << " (" << ofToString(c) << ") to palette";
    palette.push_back( c );
    gradient.addColor( c );
    palette_addColor_toInterface(c);
}

//--------------------------------------------------------------
void ofxColorManager::palette_removeColorLast()
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
            ofLogVerbose("ofxColorManager") << "removed children: " << b;
        }
        btns_palette.pop_back();
    }
}

//--------------------------------------------------------------
void ofxColorManager::palette_clear()
{
    ofLogNotice("ofxColorManager") << "palette_clear";

    palette.clear();
    gradient.reset();

    ofLogNotice("ofxColorManager") << "getNumChildren: " << scene->getNumChildren();

    for (int i=0; i< btns_palette.size(); i++)
    {
        std::string n = ("btn" + ofToString(i));
        auto a = scene->getChildWithName(n, 1000);
        auto b = a->getName();
        scene->removeChild(a, false);
        ofLogVerbose("ofxColorManager") << "removed children: " << b;
    }
    btns_palette.clear();
}

//--------------------------------------------------------------
void ofxColorManager::Changed_control(ofAbstractParameter &e) {
    string name = e.getName();

    //TODO: should reduce callbacks in output..
    if (name!="INPUT" && name!="COLOR" && name!="OUTPUT")
        ofLogNotice("ofxColorManager") << "Changed_control: " << name << ":" << e;

    // COLOR
    if (name == "COLOR") // color picked
    {
        color_HUE = 255 * color_picked.get().getHue();
        color_SAT = 255 * color_picked.get().getSaturation();
        color_BRG = 255 * color_picked.get().getBrightness();
    }
    else if (name == "HUE")
    {
        ofColor c;
        c.set(ofColor( color_picked.get() ));
        c.setHue(color_HUE);
        color_picked.set(c);

    }
    else if (name == "SATURATION")
    {
        ofColor c;
        c.set(ofColor( color_picked.get() ));
        c.setSaturation(color_SAT);
        color_picked.set(c);
    }
    else if (name == "BRIGHTNESS")
    {
        ofColor c;
        c.set(ofColor( color_picked.get() ));
        c.setBrightness(color_BRG);
        color_picked.set(c);
    }

        // PALLETE
    else if (name == "RANDOM COLOR")
    {
        if (bRandomColor)
        {
            bRandomColor = false;
            color_picked = ofFloatColor(ofRandom(0., 1.), ofRandom(0., 1.), ofRandom(0., 1.));
        }
    }
    else if (name == "ADD COLOR")
    {
        if (bAddColor)
        {
            bAddColor= false;
            palette_addColor(ofColor(color_picked.get()));
        }
    }
    else if (name == "REMOVE COLOR")
    {
        if (bRemoveColor)
        {
            bRemoveColor= false;
            palette_removeColorLast();
        }
    }
    else if (name == "CLEAR PALETTE")
    {
        if (bClearPalette)
        {
            bClearPalette = false;
            palette_clear();
        }
    }
    else if (name == "RANDOM PALETTE")
    {
        if (bRandomPalette)
        {
            bRandomPalette = false;
            random.generateRandom(NUM_ALGO_PALETTES);
        }
    }
    else if (name == "SIZE")
    {
        palettes_resize();
    }

        //--

        // CURVE
    else if (name == "INPUT")
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
//    ofLogNotice("ofxColorManager") << "key: " << key;

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

    if (key == 'o')
    {
        //TODO: bug because some trigs are flags. we need direct functions
//        bRandomColor = true;

        color_picked = ofFloatColor(ofRandom(0., 1.), ofRandom(0., 1.), ofRandom(0., 1.));
        palettes_update();
        palettes_recall(SELECTED_palette_LAST);//trig last choice
    }

    if (key == 'p')
    {
        random.generateRandom(NUM_ALGO_PALETTES);
        palettes_update();
        //set random palette to user palette
        palettes_recall(7);
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
        palette_save("myPalette");
    }
    if (key == 'l')
    {
        palette_load("myPalette");
    }
    if (key == 'c')
    {
        palette_clear();
    }
    if (key == 'x')
    {
        palette_removeColorLast();
    }

    if (key == 'a')
    {
        color_picked = ofFloatColor(ofRandom(0., 1.), ofRandom(0., 1.), ofRandom(0., 1.));
        palette_addColor(ofColor(color_picked.get()));
    }

    //-

    // COLOR BROWSER

//    ColorBrowser.keyPressed( eventArgs );

    if (key == ' ')
        ColorBrowser.switch_palette_Type();

    if (key == OF_KEY_RETURN)
        ColorBrowser.switch_sorted_Type();
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
//    ofLogNotice("ofxColorManager") << "mouseDragged " <<  x << ", " << y << ", " << button;

    TouchManager::one().touchMove(button, ofVec2f(x, y));

    //-

//    ColorBrowser.mouseDragged(eventArgs);
//    ColorBrowser.TouchManager::one().touchMove(button, ofVec2f(x, y));
}

//--------------------------------------------------------------
void ofxColorManager::mousePressed(ofMouseEventArgs& eventArgs){
    const int & x = eventArgs.x;
    const int & y = eventArgs.y;
    const int & button = eventArgs.button;
//    ofLogNotice("ofxColorManager") << "mousePressed " <<  x << ", " << y << ", " << button;

    TouchManager::one().touchDown(button, ofVec2f(x, y));

    //-

//    ColorBrowser.mousePressed(eventArgs);
//    ColorBrowser.TouchManager::one().touchDown(button, ofVec2f(x, y));
}

//--------------------------------------------------------------
void ofxColorManager::mouseReleased(ofMouseEventArgs& eventArgs){
    const int & x = eventArgs.x;
    const int & y = eventArgs.y;
    const int & button = eventArgs.button;
//    ofLogNotice("ofxColorManager") << "mouseReleased " <<  x << ", " << y << ", " << button;

    TouchManager::one().touchUp(button, ofVec2f(x, y));

    //-

//    ColorBrowser.mouseReleased(eventArgs);
//    ColorBrowser.TouchManager::one().touchUp(button, ofVec2f(x, y));
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
    ofRemoveListener( ofEvents().mouseDragged, this, &ofxColorManager::mouseDragged );
    ofRemoveListener( ofEvents().mousePressed, this, &ofxColorManager::mousePressed );
    ofRemoveListener( ofEvents().mouseReleased, this, &ofxColorManager::mouseReleased );
}

//--------------------------------------------------------------
void ofxColorManager::disableListeners()
{
    addMouseListeners();
    addKeysListeners();
}
//--------------------------------------------------------------
void ofxColorManager::enableListeners()
{
    removeMouseListeners();
    removeKeysListeners();
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
    palette_save("myPalette");
    save_group_XML(XML_params, XML_path);

    ColorBrowser.exit();

    removeKeysListeners();
    removeMouseListeners();
    ofRemoveListener(params_control.parameterChangedE(), this, &ofxColorManager::Changed_control);
    ofRemoveListener(params_color.parameterChangedE(), this, &ofxColorManager::Changed_control);
    ofRemoveListener(params_palette.parameterChangedE(), this, &ofxColorManager::Changed_control);
    ofRemoveListener(params_curve.parameterChangedE(), this, &ofxColorManager::Changed_control);
}

//--------------------------------------------------------------
void ofxColorManager::Changed_color_picked(ofFloatColor &color)
{
    ofLogNotice("ofxColorManager") << "Changed_color_picked " << ofToString(color);
    color_clicked.set(color);
//    color_clicked_param.set(color);

//    // TODO: BUG
//    // TODO: recreate palettes
//    // TODO: could auto create
//    if (bAutoTrigPalette)
//    {
//        palettes_update();
//        palettes_recall(SELECTED_palette_LAST);//trig last choice
//    }
}

//--------------------------------------------------------------
void ofxColorManager::Changed_color_clicked(ofFloatColor &color)
{
    //DISABLED
//    ofLogNotice("ofxColorManager") << "Changed_color_clicked " << ofToString(color);
//    color_picked.set(color);
}

//-

// API

//--------------------------------------------------------------
void ofxColorManager::setColor_TARGET(ofColor &c)
{
    color_TARGET = &c;
}

//--------------------------------------------------------------
void ofxColorManager::setControl(float control)
{
    curve_pos = control;
    curve_pos_slider.setPercent(control);
}

//--------------------------------------------------------------
vector<ofColor> ofxColorManager::getPalette()
{
    return palette;
}

//--------------------------------------------------------------
void ofxColorManager::setVisible(bool b)
{
    SHOW_ALL_GUI = b;
    curve_pos_slider.setVisible(b);
}

//--------------------------------------------------------------
ofColor ofxColorManager::getColorAtPercent(float control)
{
    float out = ofMap( curvesTool.getAtPercent(1.0-control), 0, curveTool_amount-1, 1., 0.) ;
    ofColor c = gradient.getColorAtPercent( out );
    return c;
}

//--------------------------------------------------------------
void ofxColorManager::setVisible_GUI_MINI(bool b)
{
    SHOW_GUI_MINI = b;
}

//--------------------------------------------------------------
void ofxColorManager::setBackground_ENABLE(bool b)
{
    backgroundENABLE = b;
}

//--------------------------------------------------------------
void ofxColorManager::draw_previewGradient(glm::vec2 pos, bool horizontal = true) {

    if (horizontal)
    {
        curve_img_gradient.draw(pos);
    }
    //TODO: how to draw rotated... can image.rotate the image in some trig...
//    else
//    {
//        ofPushMatrix();
//        ofTranslate(pos);
//        ofRotateDeg(90, 1, 0, 0);
//        curve_img_gradient.draw(0, 0);
//        ofPopMatrix();
//    }
}

//--------------------------------------------------------------
void ofxColorManager::setVisible_debugText(bool b)
{
    SHOW_debugText = b;
}
