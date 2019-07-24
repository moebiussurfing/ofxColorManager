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

    // COLOR BROWSER

    ColorBrowser.setBoxSize(25);
    ColorBrowser.setRowsSize(10);
    ColorBrowser.setup_colorBACK(color_BACK);
    ColorBrowser.setPosition(glm::vec2(925, 40));
    ColorBrowser.setup();

    //-

    color_picked.addListener(this, &ofxColorManager::Changed_color_picked);
    color_clicked_param.addListener(this, &ofxColorManager::Changed_color_clicked);

    //-

    dt = 1/30.0f;//TODO: should be setted externally

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

    params_control.setName("CONTROL");
    params_control.add(color_picked);
    params_control.add(bRandomColor);
    params_control.add(bAddColor);
    params_control.add(bRemoveColor);
    params_control.add(bClearPalette);

    ofAddListener(params_control.parameterChangedE(), this, &ofxColorManager::Changed_control);

    //-

    // ALGORITHMIC PALETTE

    MODE_Palette.set("MODE COLOR/SLIDERS", false);
    BRIGHTNESS.set("BRIGHTNESS", 128, 0, 255 );
    SATURATION.set("SATURATION", 128, 0, 255 );
    bRandomPalette.set("RANDOM PALETTE", false);

    params_palette.setName("ALGORITHMIC PALETTE");
    params_palette.add(MODE_Palette);
    params_palette.add(BRIGHTNESS);
    params_palette.add(SATURATION);
    params_palette.add(bRandomPalette);

    ofAddListener(params_palette.parameterChangedE(), this, &ofxColorManager::Changed_control);

    //-

    // INTERFACE

    interface_setup();

    //-

    // PALETTES

    random.generateRandom();
    palettes_update();
    palettes_setup();

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
    imGui_theme();

    //--

    // LISTENERS

    addKeysListeners();
    addMouseListeners();

    //--

    // STARTUP SETTINGS

    XML_params.setName("ofxColorManager");
    XML_params.add(color_backGround);
    XML_params.add(color_picked);
    XML_params.add(color_HUE);
    XML_params.add(color_SAT);
    XML_params.add(color_BRG);
    XML_params.add(MODE_Palette);//algorithmic palette
    XML_params.add(BRIGHTNESS);
    XML_params.add(SATURATION);
    XML_params.add(gradient_hard);//gradient
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
void ofxColorManager::setup_Gui_layout()
{
    // LAYOUT

    box_size = 40;

    // global mini pad between panels/objects
    pad = 2;

    // gui
    gui_x = 10;
    gui_y = 10;
    gui_w = 200;
    gui_h = 420;//estimate or measure high of the panel on window resize

    // user palette
    palette_x = gui_x;
    palette_y = gui_y + gui_h + pad;

    // algorithmic palettes
    palettes_x = gui_x;
    palettes_y = 460;

    // curve tool pos (anchor for others)
    curveTool_x = 525;
    curveTool_y = 25;
    curveTool_w = 256;
    curveTool_h = 256;

    // gradient curved image LUT [referenced to curve pos (vertical)]
    image_curvedGradient_x = amount + pad;//curveTool +
    image_curvedGradient_y = 0;//curveTool +
    image_curvedGradient_w = box_size;
    image_curvedGradient_h = amount;

    // slider
    slider_x = curveTool_x + pad + amount + pad + image_curvedGradient_w;
    slider_y = curveTool_y;
    slider_w = box_size / 2;
    slider_h = amount;

    // gradient pre curve (bad sorted to the left but anchored to curve..)
    grad_w = box_size;
    grad_x = curveTool_x - (grad_w + pad);
    grad_y = curveTool_y;
    grad_h = amount;

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
void ofxColorManager::imGui_theme()
{
    // must be done after setup the gui

    //-

    // widgets color
    ofColor myColor1;
    int grayDark = 0;
    myColor1 = ofColor(grayDark, grayDark, grayDark, 255);//black
    ofColor myColor2;
    int gray = 24;
    myColor2 = ofColor(gray, gray, gray, 255);//gray1
    ofColor myColor3;
    int gray2 = 16;
    myColor3 = ofColor(gray2, gray2, gray2, 255);//gray2

    ImGuiStyle *style = &ImGui::GetStyle();
    style->WindowRounding = (3.0f);
    style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(myColor1, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(myColor2, 0.21f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(myColor1, 0.5f);
    style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(myColor2, 1.00f);
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
    style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(myColor2, 0.78f);//hover
    style->Colors[ImGuiCol_FrameBgActive] = ImVec4(myColor1, 1.00f);
    style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(myColor2, 0.7f);
}

//--------------------------------------------------------------
void ofxColorManager::interface_addColor(ofColor c)
{
    palette_x = gui_x;
    int perRow = 5;
    int i = btns_palette.size();
    palette_x += i * ( box_size + pad );

    ButtonExample *btn = new ButtonExample();
    btn->setup(palette_x, palette_y, box_size, box_size);
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
bool ofxColorManager::imGui()
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
                ofxImGui::AddParameter(this->color_picked, true);
                ofxImGui::AddParameter(this->color_HUE);
                ofxImGui::AddParameter(this->color_SAT);
                ofxImGui::AddParameter(this->color_BRG);
                ofxImGui::AddParameter(this->bRandomColor);
                ofxImGui::AddParameter(this->bAddColor);
                ofxImGui::AddParameter(this->bRemoveColor);
                ofxImGui::AddParameter(this->bClearPalette);
                ofxImGui::EndTree(mainSettings);
            }

            // ALGORITHMIC PALETTE
            if (ofxImGui::BeginTree(this->params_palette, mainSettings))
            {
                ofxImGui::AddParameter(this->MODE_Palette);
                if (MODE_Palette) {
                    ofxImGui::AddParameter(this->SATURATION);
                    ofxImGui::AddParameter(this->BRIGHTNESS);
                }
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
    if (SELECTED_palette != SELECTED_palette_PRE)
    {
        ofLogNotice("ofxColorManager::update") << "-> CHANGED SELECTED_palette: " << SELECTED_palette;
        palettes_recall(SELECTED_palette);

        SELECTED_palette_PRE = SELECTED_palette = -1;//bug if not if pressed same button
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
        ofLogNotice("ofxColorManager") << "-> CHANGED color_clicked";
        color_picked.set(color_clicked);
    }
}

//#pragma mark curveTool

//--------------------------------------------------------------
void ofxColorManager::curveTool_setup()
{
    img_gradient.allocate(image_curvedGradient_w, image_curvedGradient_h, OF_IMAGE_COLOR);

    curvesTool.setup(amount);
    curvesTool.load("curves.yml"); //needed because it fills polyline

    curve_pos.set("CURVE POS", 0., 0., 1.);// pct value 0. to 1.
    bResetCurve.set("RESET CURVE", false);
    params_curve.add(curve_pos);
    params_curve.add(bResetCurve);
    ofAddListener(params_curve.parameterChangedE(), this, &ofxColorManager::Changed_control);

    //-

    // slider
    curveSlider.setup(slider_x, slider_y, slider_w, slider_h, 0., 1., 0, true, true);
    curveSlider.setLabelString("");
}

//--------------------------------------------------------------
void ofxColorManager::curveTool_update()
{
    // TODO: add parameter to improve backwards mirroring
    // slider // TODO: mirror to gui param
    curve_pos = curveSlider.getValue();

    //-

    // vertical and rectangle

    bool bInvert = false; // flip gradient direction
    float outLUT; // output will be clamped to: [ 0 - 255]
    float outLUT_normalized; // output [ 0.0 - 1.0 ]

    for (int inLUT = 0; inLUT < amount; inLUT++)
    {
        float y = ofMap( inLUT, 0, amount-1, 0, image_curvedGradient_h );

        outLUT = curvesTool[inLUT]; // get value for curve at inLUT (0-255) position
        if (!bInvert)
            outLUT_normalized = ofMap( outLUT, 0, amount-1, 0., 1. );
        else
            outLUT_normalized = ofMap( outLUT, 0, amount-1, 1., 0. );
        ofColor c = gradient.getColorAtPercent( outLUT_normalized );

        for (int x = 0; x < image_curvedGradient_w; x++)
        {
//            img_gradient.setColor(x, y, c);
            img_gradient.setColor(x, inLUT, c);
        }
    }
    img_gradient.update();

    //-

    // TODO:
//    cnt = ofMap( curve_pos.get(), 0., 1., 0., amount-1 );
    cnt = ofMap( curve_pos.get(), 0., 0., 1., amount-1 );
}

//--------------------------------------------------------------
void ofxColorManager::curveTool_draw() {

    if (curveShow)
    {
        ofPushMatrix();
        ofPushStyle();

        //-

        // GRADIENT

        // 1. un-curved gradient rectangle (left positioned)

        gradient.drawDebug(grad_x, grad_y, grad_w, grad_h);

        //-

        // 2. current box color at point (right positioned)

        ofRectangle r;
        bool bInvert = true;
        float inLUT;
        float outLUT;
        float outLUT_normalized;
        r = ofRectangle( currColor_x, currColor_y, box_size/2, slider_h );
        inLUT = ofMap( curve_pos, 0., 1., 0, amount-1 );
        outLUT = curvesTool[inLUT];

        if (!bInvert)
            outLUT_normalized = ofMap( outLUT, 0, amount-1, 0., 1. );
        else
            outLUT_normalized = ofMap( outLUT, 0, amount-1, 1., 0. );
        ofColor c = gradient.getColorAtPercent( outLUT_normalized );

        ofPushStyle();
        ofFill();
        ofSetColor(c);
        ofDrawRectangle(r);
        ofPopStyle();

        //--

        // 3. curve tool

        ofTranslate(curveTool_x, curveTool_y);

        //-

        // image box gradient LUT
        img_gradient.draw(image_curvedGradient_x, image_curvedGradient_y);

        //-

        // curve editor

        ofSetColor(255);
        curvesTool.draw(0, 0, cnt);

        // horizontal line
        ofSetColor(ofColor::white);

        float y =  amount - curvesTool[cnt];
        ofDrawLine(0, y, amount, y);
//    ofDrawLine(amount + slider_w, y, amount + slider_w + pad + image_curvedGradient_w + 100, y);

        // current circle point
        ofSetColor(25);
        ofDrawCircle(cnt, y, 3);

        //-

        ofPopMatrix();
        ofPopStyle();
    }
}

//--------------------------------------------------------------
void ofxColorManager::palettes_setup() {
    int x0 = palettes_x;
    int y0 = palettes_y;//to recall at end
    int h0 = box_size + pad;

    // 1. triad
    palettes_x = x0;
    palettes_y += h0;
    for (int i = 0; i < triad.size(); i++) {
        ButtonExample *btn = new ButtonExample();
        btn->setup(palettes_x, palettes_y, box_size, box_size);
        btn->setup_colorBACK(color_clicked);
        btn->setLocked(true);
        btn->setName("triad" + ofToString(i));
        btn->setColor(triad[i]);
        scene->addChild(btn);
        btns_plt_Triad.push_back(btn);
        palettes_x += h0;
    }

    // 2. complement triad
    palettes_x = x0;
    palettes_y += h0;
    for (int i = 0; i < complementTriad.size(); i++) {
        ButtonExample *btn = new ButtonExample();
        btn->setup(palettes_x, palettes_y, box_size, box_size);
        btn->setup_colorBACK(color_clicked);
        btn->setLocked(true);
        btn->setName("compTriad" + ofToString(i));
        btn->setColor(complementTriad[i]);
        scene->addChild(btn);
        btns_plt_ComplTriad.push_back(btn);
        palettes_x += h0;
    }

    // 3. complement sat
    palettes_x = x0;
    palettes_y += h0;
    for (int i = 0; i < complement.size(); i++) {
        ButtonExample *btn = new ButtonExample();
        btn->setup(palettes_x, palettes_y, box_size, box_size);
        btn->setup_colorBACK(color_clicked);
        btn->setLocked(true);
        btn->setName("compSat" + ofToString(i));
        btn->setColor(complement[i]);
        scene->addChild(btn);
        btns_plt_CompSat.push_back(btn);
        palettes_x += h0;
    }

    // 4. complement brgt
    palettes_x = x0;
    palettes_y += h0;
    for (int i = 0; i < complementBrightness.size(); i++) {
        ButtonExample *btn = new ButtonExample();
        btn->setup(palettes_x, palettes_y, box_size, box_size);
        btn->setup_colorBACK(color_clicked);
        btn->setLocked(true);
        btn->setName("compBrgt" + ofToString(i));
        btn->setColor(complementBrightness[i]);
        scene->addChild(btn);
        btns_plt_ComplBrgt.push_back(btn);
        palettes_x += h0;
    }

    // 5. mono sat
    palettes_x = x0;
    palettes_y += h0;
    for (int i = 0; i < monochrome.size(); i++) {
        ButtonExample *btn = new ButtonExample();
        btn->setup(palettes_x, palettes_y, box_size, box_size);
        btn->setup_colorBACK(color_clicked);
        btn->setLocked(true);
        btn->setName("monoSat" + ofToString(i));
        btn->setColor(monochrome[i]);
        scene->addChild(btn);
        btns_plt_MonoSat.push_back(btn);
        palettes_x += h0;
    }

    // 6. mono brgt
    palettes_x = x0;
    palettes_y += h0;
    for (int i = 0; i < monochromeBrightness.size(); i++) {
        ButtonExample *btn = new ButtonExample();
        btn->setup(palettes_x, palettes_y, box_size, box_size);
        btn->setup_colorBACK(color_clicked);
        btn->setLocked(true);
        btn->setName("monoBrgt" + ofToString(i));
        btn->setColor(monochromeBrightness[i]);
        scene->addChild(btn);
        btns_plt_MonoBrgt.push_back(btn);
        palettes_x += h0;
    }

    // 7. analogue
    palettes_x = x0;
    palettes_y += h0;
    for (int i = 0; i < analogue.size(); i++) {
        ButtonExample *btn = new ButtonExample();
        btn->setup(palettes_x, palettes_y, box_size, box_size);
        btn->setup_colorBACK(color_clicked);
        btn->setLocked(true);
        btn->setName("analogue" + ofToString(i));
        btn->setColor(analogue[i]);
        scene->addChild(btn);
        btns_plt_Analog.push_back(btn);
        palettes_x += h0;
    }

    // 8. random
    palettes_x = x0;
    palettes_y += h0;
    for (int i = 0; i < random.size(); i++) {
        ButtonExample *btn = new ButtonExample();
        btn->setup(palettes_x, palettes_y, box_size, box_size);
        btn->setup_colorBACK(color_clicked);
        btn->setLocked(true);
        btn->setName("random" + ofToString(i));
        btn->setColor(random[i]);
        scene->addChild(btn);
        btns_plt_Random.push_back(btn);
        palettes_x += h0;
    }

    //--

    // LABELS PICKER : TODO: maybe can use simpler thing than ofxInterface for this

    // palettes labels

    ofColor btn_plt_c;
    btn_plt_c.set(ofColor(32));
    int btn_plt_h;
    int btn_pad_w;
    btn_pad_w = 230;
    btn_plt_h = h0;
    palettes_x = x0;
    palettes_y = y0 + btn_plt_h + 10;

    //-

    int NUM_PALETTES = 8;
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
        btn->setPosition(palettes_x + btn_pad_w, palettes_y + p*btn_plt_h);
        btn->setBGColor(btn_plt_c);
        btn->setLabelColor(ofColor::white);
        btn->setBorder(false);
        scene->addChild(btn);
        btns_plt_Selector.push_back(btn);
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
    brightness = BRIGHTNESS;
    saturation = SATURATION;
    ofColor base;

    //-

    if (MODE_Palette)
    {
        // using hue only from picked color and sat/(brg from sliders
        base = ofColor::fromHsb(ofMap(color_picked.get().getHue(), 0., 1., 0, 255), saturation, brightness);
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

    // COLOR BOX PICKER (CURRENT)

    ofPushStyle();
    ofFill();
    ofSetColor( ofColor( color_picked.get() ) );
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

    // COLOR BROWSER
    ColorBrowser.draw();

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

//    // DEBUG ALGORTIHMIC PALETTE
//
//    int x, y, w, h;
//    h = (box_size+pad);
//
//    ofPushMatrix();
//    ofPushStyle();
//    ofSetColor(ofColor::white);
//    ofTranslate(palettes_x + 80, palettes_y + h/2 + 12);
//
//    ofTranslate(0, h);
//    ofDrawBitmapString("Triad", 0, 0);
//    ofTranslate(0, h);
//    ofDrawBitmapString("Complement Triad", 0, 0);
//    ofTranslate(0, h);
//    ofDrawBitmapString("Complement (Saturation)", 0, 0);
//    ofTranslate(0, h);
//    ofDrawBitmapString("Complement (Brightness)", 0, 0);
//    ofTranslate(0, h);
//    ofDrawBitmapString("Monochrome (Saturation)", 0, 0);
//    ofTranslate(0, h);
//    ofDrawBitmapString("Monochrome (Brightness)", 0, 0);
//    ofTranslate(0, h);
//    ofDrawBitmapString("Analogue", 0, 0);
//    ofTranslate(0, h);
//    ofDrawBitmapString("Random", 0, 0);
//
//    ofPopMatrix();
//    ofPopStyle();

    //--
}

//--------------------------------------------------------------
void ofxColorManager::palette_addColor(ofColor c)
{
    ofLogNotice("ofxColorManager") << "add color " << " (" << ofToString(c) << ") to palette";
    palette.push_back( c );
    gradient.addColor( c );
    interface_addColor(c);
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
        auto a = scene->getChildWithName(n, 100);
        auto b = a->getName();
        scene->removeChild(a, false);
        ofLogVerbose("ofxColorManager") << "removed children: " << b;
    }
    btns_palette.clear();
}

//--------------------------------------------------------------
void ofxColorManager::Changed_control(ofAbstractParameter &e) {
    string name = e.getName();

    //TODO: reduce callbacks..
    if (name != "CURVE POS" && name != "COLOR")
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
//        color_clicked.set(color_picked);//TODO: ?
    }
    else if (name == "BRIGHTNESS")
    {
        ofColor c;
        c.set(ofColor( color_picked.get() ));
        c.setBrightness(color_BRG);
        color_picked.set(c);
//        color_clicked.set(color_picked);//TODO: ?
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

    if (key == 'r')
    {
        random.generateRandom();

        //TODO: bug because some trigs are flags. we need direct functions
//        bRandomColor = true;
        color_picked = ofFloatColor(ofRandom(0., 1.), ofRandom(0., 1.), ofRandom(0., 1.));

//        palettes_recall((int)ofRandom(7));
        palettes_recall(6);//analog palette
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

//    ofAddListener( ofEvents().mouseDragged, this, &ofxColorsBrowser::mouseDragged );
//    ofAddListener( ofEvents().mousePressed, this, &ofxColorsBrowser::mousePressed );
//    ofAddListener( ofEvents().mouseReleased, this, &ofxColorsBrowser::mouseReleased );
}

//--------------------------------------------------------------
void ofxColorManager::removeMouseListeners()
{
    ofRemoveListener( ofEvents().mouseDragged, this, &ofxColorManager::mouseDragged );
    ofRemoveListener( ofEvents().mousePressed, this, &ofxColorManager::mousePressed );
    ofRemoveListener( ofEvents().mouseReleased, this, &ofxColorManager::mouseReleased );

//    ofRemoveListener( ofEvents().mouseDragged, this, &ofxColorsBrowser::mouseDragged );
//    ofRemoveListener( ofEvents().mousePressed, this, &ofxColorsBrowser::mousePressed );
//    ofRemoveListener( ofEvents().mouseReleased, this, &ofxColorsBrowser::mouseReleased );
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
}

//--------------------------------------------------------------
void ofxColorManager::Changed_color_clicked(ofFloatColor &color)
{
//    ofLogNotice("ofxColorManager") << "Changed_color_clicked " << ofToString(color);
//    color_picked.set(color);
}