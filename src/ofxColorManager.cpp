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

    //---

    // DEMO 1

    ofSetCircleResolution(36);

    ofxLoadCamera(cam, "DEMOcam");

    //-

    // MOUSE DEBUGGER

    mouseRuler.setup();
    mouseRuler.toggleVisibility();

    //-

    dt = 1/30.0f;//TODO: should be setted externally

    //-

    // COLOUR LOVERS

    // set positions and panel sizes
    glm::vec2 sizeGui(150, 375);
    glm::vec2 sizeGrid(150, ofGetHeight());
    glm::vec2 posGui(ofGetWidth()-(sizeGui.x+sizeGrid.x+4), 0);
    glm::vec2 posGrid(posGui.x+sizeGui.x+2, 0);

    // must be called before setup() to overwrite default settings
    ColourLoversHelper.setGrid(posGrid, sizeGrid);
    ColourLoversHelper.setup(posGui, sizeGui);

    //-

    // receivers pointers
    ColourLoversHelper.setColor_BACK(myColor);
    ColourLoversHelper.setPalette_BACK(myPalette);
    ColourLoversHelper.setPalette_Name_BACK(myPalette_Name);
    ColourLoversHelper.setPalette_bUpdated_Palette_BACK(bUpdated_Palette_BACK);
    ColourLoversHelper.setPalette_bUpdated_Color_BACK(bUpdated_Color_BACK);

    // some initiation values..
    myColor = ofColor::white;
    myPalette.resize(2);//pointer setter whill clear/resize. nevermind the vector size here
    myPalette[0] = ofColor::white;
    myPalette[0] = ofColor::white;
    myPalette_Name = "NOT LOADED";

    //-

    // LAYOUT

    gui_setup_layout();

    //-

    // COLOR BROWSER

    ColorBrowser.setBoxSize(7);
    ColorBrowser.setRowsSize(7*17);//for Pantone palette
    ColorBrowser.setup_colorBACK(color_BACK);
    ColorBrowser.setPosition(colorBrowserPos);
    ColorBrowser.setup();
    ColorBrowser_palette = ColorBrowser.getPalette();

    //-

    // GENERAL DATA

    setBackground_ENABLE(true);
    color_backGround.set("BACKGROUND", ofFloatColor::white);
    color_backGround_SET.set("SET FROM COLOR", false);
    color_backGround_SETAUTO.set("AUTO SET", false);
    params_data.setName("GENERAL");
    params_data.add(color_backGround);
    params_data.add(color_backGround_SET);
    params_data.add(color_backGround_SETAUTO);

    //--

    // COLOR

    color_picked.set("COLOR", ofFloatColor::red);
    color_HUE.set("H", 0, 0, 255);
    color_SAT.set("S", 0, 0, 255);
    color_BRG.set("V", 0, 0, 255);

    params_color.setName("COLOR");
    params_color.add(color_HUE);
    params_color.add(color_SAT);
    params_color.add(color_BRG);

    ofAddListener(params_color.parameterChangedE(), this, &ofxColorManager::Changed_CONTROL);

    //-

    // ALGORITHMIC PALETTE

    MODE_Palette.set("FROM COLOR", false);
    SATURATION.set("SATURATION", 200, 0, 255 );
    BRIGHTNESS.set("BRIGHTNESS", 200, 0, 255 );
    bRandomPalette.set("RANDOM PALETTE", false);
    bAuto_palette_recall.set("AUTO CREATE!", false);
    bLock_palette.set("LOCK PALETTES", false);

    NUM_ALGO_PALETTES.set("SIZE COLORS", 6, 2, 8);
    params_palette.setName("ALGORITHMIC PALETTES");
    params_palette.add(MODE_Palette);
    params_palette.add(SATURATION);
    params_palette.add(BRIGHTNESS);
    params_palette.add(bRandomPalette);
    params_palette.add(NUM_ALGO_PALETTES);
    params_palette.add(bAuto_palette_recall);
    params_palette.add(bLock_palette);

    ofAddListener(params_palette.parameterChangedE(), this, &ofxColorManager::Changed_CONTROL);

    //-

    // INTERFACE

    interface_setup();

    //-

    // ALGORITHMIC PALETTES

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

    //-

    // CONTROL

    bRandomColor.set("RANDOM COLOR", false);
    bAddColor.set("ADD COLOR", false);
    bPaletteEdit.set("EDIT COLOR", false);
    bRemoveColor.set("REMOVE COLOR", false);
    bClearPalette.set("CLEAR PALETTE", false);
    params_control.setName("COLOR EDITOR");
//    params_control.add(color_picked);
    params_control.add(bRandomColor);
    params_control.add(bPaletteEdit);
    params_control.add(bAddColor);
    params_control.add(bRemoveColor);
    params_control.add(bClearPalette);

    //-

    // CONTROL WINDOWS

    SHOW_ColourLovers.set("SHOW COLOUR LOVERS", true);
    SHOW_AlgoPalettes.set("SHOW PALETTES", true);
    SHOW_BrowserColors.set("SHOW BROWSER COLORS", true);
    SHOW_Gradient.set("SHOW GRADIENT", true);
    SHOW_Curve.set("SHOW CURVE TOOL", true);

    params_control.add(SHOW_ColourLovers);
    params_control.add(SHOW_AlgoPalettes);
    params_control.add(SHOW_BrowserColors);
    params_control.add(SHOW_Gradient);
    params_control.add(SHOW_Curve);

    SHOW_ALL_GUI = true;
    SHOW_debugText = false;
    SHOW_GUI_MINI = false;

    //-

    // BACKGROUND

    params_control.add(color_backGround_SET);
    params_control.add(color_backGround_SETAUTO);

    //-

    ofAddListener(params_control.parameterChangedE(), this, &ofxColorManager::Changed_CONTROL);

    //--

    // GUI

    //-

    // font

    ImGuiIO& io = ImGui::GetIO();
    string inputPath;
    inputPath = ofFilePath::getAbsolutePath("assets/fonts/PragmataProR_0822.ttf");
    const char* myPath = inputPath.c_str();
    ImFontConfig config;
//    config.OversampleH = 3;
//    config.OversampleV = 1;
//    config.GlyphExtraSpacing.x = 1.0f;
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

    color_picked.addListener(this, &ofxColorManager::Changed_color_picked);
    color_clicked_param.addListener(this, &ofxColorManager::Changed_color_clicked);

    //--

    // STARTUP SETTINGS

    XML_params.setName("ofxColorManager");

    XML_params.add(SHOW_ColourLovers);
    XML_params.add(SHOW_AlgoPalettes);
    XML_params.add(SHOW_Gradient);
    XML_params.add(SHOW_Curve);
    XML_params.add(SHOW_BrowserColors);

    XML_params.add(color_picked);

    XML_params.add(color_backGround);
    XML_params.add(color_backGround_SETAUTO);

    XML_params.add(bPaletteEdit);//user palette
    XML_params.add(MODE_Palette);//algorithmic palette
    XML_params.add(NUM_ALGO_PALETTES);
    XML_params.add(BRIGHTNESS);
    XML_params.add(SATURATION);
    XML_params.add(bAuto_palette_recall);
    XML_params.add(gradient_hard);//gradient
    XML_params.add(SHOW_TEST_Curve);//curve tool
//    XML_params.add(TEST_MODE);

//    XML_params.add(bAuto_palette_recall);
    XML_params.add(color_backGround_SETAUTO);
//    XML_params.add(SHOW_ColourLovers);


    load_group_XML(XML_params, XML_path);
    palette_load("myPalette");

    //-

    palettes_setVisible(SHOW_AlgoPalettes);
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
    // LAYOUT DEFAULT
    // TODO:: add layout json saver like ofxGuiPanels

    // global mini pad between panels/objects
    pad = 2;
    guiWidth = 250;
    box_size = 40;

    // gui 1 ImGui
    gui_x = 0;
    gui_y = 0;
    gui_w = guiWidth;
    gui_h = 200;

    // gui 2 ImGui
    gui2_x = guiWidth-3;
    gui2_y = 0;
    gui2_w = guiWidth;
    gui2_h = 100;

    //-

    // curve tool pos (anchor for others)
    curveTool_x = 650;//distance required to not be over the colorpicker
    curveTool_y = 15;
    curveTool_w = curveTool_amount;//TODO: should can resize curve tool editor box besides amount
    curveTool_h = curveTool_amount;

    // gradient-pre curve (bad sorted to the left but anchored to curve..)
    grad_w = box_size;
    grad_x = curveTool_x-(grad_w + pad);
    grad_y = curveTool_y;
    grad_h = curveTool_h;

    // curve mod slider
    sliderMod_x = curveTool_x+curveTool_w+pad;
    sliderMod_y = curveTool_y;
    sliderMod_w = box_size/2;
    sliderMod_h = curveTool_h;

    // gradient-curved image LUT [referenced to curve pos (vertical)]
    image_curvedGradient_x = curveTool_w+pad+sliderMod_w+pad;//curveTool +
    image_curvedGradient_y = 0;//curveTool +
    image_curvedGradient_w = box_size;
    image_curvedGradient_h = curveTool_h;

    // testing curve widgets
    // curve out slider
    slider_x = sliderMod_x+pad+image_curvedGradient_w+box_size/2;
    slider_y = curveTool_y;
    slider_w = box_size/2;
    slider_h = curveTool_h;

    // current color box/bar (the one affected by slider position. just for testing gradient purpose)
    currColor_x = slider_x+2*(slider_w+pad);
    currColor_y = curveTool_y;

    //-

    // user palette (pos related to gradient-pre curve)
    palette_x = grad_x-(grad_w+pad);
    palette_y = curveTool_y;

    // user color box monitor picked (same that color picker gui)
    // bar mode
    color_w = (2*box_size);
    color_x = palette_x-(color_w+pad+box_size/2+pad);
    color_y = curveTool_y;
    color_h = curveTool_h;
    r_color_picked = ofRectangle( color_x, color_y, color_w, color_h );

    //-

    // all 8 algorithmic palettes
    palettes_x = 500;
    palettes_y = 320;

    // color clicked box on palettes(hidden)
    colorPick_x = 390;
    colorPick_y = color_h + 30;
    colorPick_w = colorPick_h = 2*box_size;
    r_color_clicked = ofRectangle( colorPick_x, colorPick_y, colorPick_w, colorPick_h );

    // browser colors palettes
    colorBrowserPos = glm::vec2(270, 675);
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
            ofLogNotice("ofxColorManager") << "color_picked" << i << ": " << ofToString(c);
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
        ofLogNotice("ofxColorManager") << "color_picked" << i << " " << ofToString(data.palette[i]);
    }

    ofFile file(path, ofFile::WriteOnly);
    jsonout jo(file);
    jo << data;
}



//--------------------------------------------------------------
void ofxColorManager::gui_imGui_theme()
{
    // must be done after setup the gui

    ImGuiStyle *style = &ImGui::GetStyle();

    // my dark theme
    //    ImVec4* colors = ImGui::GetStyle().Colors;
    style->Colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style->Colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style->Colors[ImGuiCol_WindowBg]               = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    style->Colors[ImGuiCol_ChildBg]                = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
    style->Colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    style->Colors[ImGuiCol_Border]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
    style->Colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_FrameBg]                = ImVec4(0.02f, 0.02f, 0.02f, 0.54f);
    style->Colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.26f, 0.26f, 0.26f, 0.68f);
    style->Colors[ImGuiCol_FrameBgActive]          = ImVec4(0.25f, 0.25f, 0.25f, 0.67f);
    style->Colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    style->Colors[ImGuiCol_TitleBgActive]          = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    style->Colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    style->Colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.12f, 0.11f, 0.11f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
    style->Colors[ImGuiCol_CheckMark]              = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style->Colors[ImGuiCol_SliderGrab]             = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    style->Colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.74f, 0.75f, 0.77f, 0.79f);
    style->Colors[ImGuiCol_Button]                 = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
    style->Colors[ImGuiCol_ButtonHovered]          = ImVec4(0.19f, 0.19f, 0.19f, 0.79f);
    style->Colors[ImGuiCol_ButtonActive]           = ImVec4(0.00f, 0.00f, 0.00f, 0.70f);
    style->Colors[ImGuiCol_Header]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.31f);
    style->Colors[ImGuiCol_HeaderHovered]          = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_HeaderActive]           = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
    style->Colors[ImGuiCol_Separator]              = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    style->Colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.29f, 0.29f, 0.29f, 0.78f);
    style->Colors[ImGuiCol_SeparatorActive]        = ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip]             = ImVec4(0.44f, 0.44f, 0.44f, 0.25f);
    style->Colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.68f, 0.68f, 0.68f, 0.67f);
    style->Colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.68f, 0.68f, 0.68f, 0.95f);
    style->Colors[ImGuiCol_PlotLines]              = ImVec4(0.81f, 0.79f, 0.79f, 1.00f);
    style->Colors[ImGuiCol_PlotLinesHovered]       = ImVec4(0.58f, 0.58f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogram]          = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.00f, 0.00f, 0.00f, 0.35f);
    style->Colors[ImGuiCol_DragDropTarget]         = ImVec4(0.50f, 0.50f, 0.50f, 0.90f);
    style->Colors[ImGuiCol_NavHighlight]           = ImVec4(0.79f, 0.79f, 0.79f, 1.00f);
    style->Colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    style->Colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    style->Colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

//    // 2. defaut dark theme exported
//    style->WindowMinSize = ImVec2(160, 65);
//    style->FramePadding = ImVec2(4, 2);
//    style->ItemSpacing = ImVec2(6, 2);
//    style->ItemInnerSpacing = ImVec2(6, 4);
//    style->Alpha = 1.0f;
//    style->WindowRounding = 0.0f;
//    style->FrameRounding = 0.0f;
//    style->IndentSpacing = 6.0f;
//    style->ItemInnerSpacing = ImVec2(2, 4);
//    style->ColumnsMinSpacing = 50.0f;
//    style->GrabMinSize = 14.0f;
//    style->GrabRounding = 0.0f;
//    style->ScrollbarSize = 12.0f;
//    style->ScrollbarRounding = 0.0f;
}


//--------------------------------------------------------------
void ofxColorManager::palette_rearrenge()
{
    ofLogNotice("ofxColorManager:palette_rearrenge");
    ofLogNotice("ofxColorManager:palette_rearrenge") << "btns_palette.size():" << btns_palette.size();

    // re-arrenge all resized boxes

    std::string name;
    int x, y, w, h, hTotal;
    x = palette_x;
    w = box_size;
    int numBtns = btns_palette.size();
    hTotal = curveTool_h;
    h = hTotal/numBtns;
    bool flipBtn = true;

    for (int i=0; i<numBtns; i++)
    {
        name = "btn"+ofToString(i);
        cout << name << endl;
        auto a = scene->getChildWithName(name, 1000);

        if (flipBtn)
            y = palette_y + (numBtns-i-1)*h;
        else
            y = palette_y + i*h;

        a->setPosition(x, y);
        a->setSize(w, h);
    }

    ofLogNotice("ofxColorManager:palette_rearrenge") <<"btns_palette.size():"<<btns_palette.size();
}


//--------------------------------------------------------------
void ofxColorManager::palette_addColor_toInterface(ofColor c)
{
    // vertical palette with resize boxes size to fit gradient bar height
    // add the new color (current color_clicked) to the user palette
    int i = btns_palette.size();

    ButtonExample *btn = new ButtonExample();
    btn->setup(0, 0, 10, 10);//temp
    btn->setColor(c);
    btn->setup_colorBACK( color_clicked );
    btn->setSelectable(false);//to enable border draw
    btn->setLocked(true);//avoid dragging
    btn->setName("btn"+ofToString(i));

    //temp positions
    btn->setPosition(palette_x, palette_y+box_size+i*(box_size+pad));
    btn->setSize(box_size, box_size);

    scene->addChild(btn);
    btns_palette.push_back(btn);

    ofLogNotice("ofxColorManager:palette_addColor_toInterface:")<<"added btn";
    ofLogNotice("ofxColorManager:palette_addColor_toInterface:")<<"btns_palette.size():["<<btns_palette.size()<<"]";

    //-

    // make positions & resizes to fill bar
    palette_rearrenge();
}


//--------------------------------------------------------------
void ofxColorManager::interface_update(){

    TouchManager::one().update();

    // 1. EACH COLOR OF CURRENT USER PALETTE
    for (int i=0; i<btns_palette.size(); i++)
    {
        btns_palette[i]->update();
    }

    // 2. ALGORITHMIC PALETTES SELECTORS
    for (int i=0; i<btns_plt_Selector.size(); i++)
    {
        btns_plt_Selector[i]->update(dt);
    }

    //-

    // 3. ALGORITHMIC PALETTES COLORS

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

    float widgetFactor = 0.9;

    auto mainSettings = ofxImGui::Settings();
    this->gui.begin();

    //-

    // 1ST WINDOW

    // COLOR PICKER CUSTOM

    auto COLOR_PICKER_Settings = ofxImGui::Settings();
    COLOR_PICKER_Settings.windowPos = ofVec2f(gui_x, gui_y);
    COLOR_PICKER_Settings.windowSize = ofVec2f(guiWidth, 200);
//    COLOR_PICKER_Settings.lockPosition = true;

    if (ofxImGui::BeginWindow("COLOR PICKER", COLOR_PICKER_Settings, false))
    {
        //--

        // GET COLOR FROM OUTSIDE COLOR PICKED

        // TEST
        LISTEN_isEnabled = false;//maybe required because get() causes callbacks too (?)

        // TEST
        static ImVec4 color;
        color.x = color_picked.get().r;
        color.y = color_picked.get().g;
        color.z = color_picked.get().b;
        color.w = color_picked.get().a;

        // TEST
        LISTEN_isEnabled = true;

        //--

        // 0. CUSTOM BUTTON COLOR BIG

        ImGui::PushItemWidth(guiWidth * widgetFactor);

        int colorW = ImGui::GetWindowWidth() * widgetFactor;
        int colorH = 50;
        int misc_flags = ImGuiColorEditFlags_NoOptions|ImGuiColorEditFlags_NoTooltip;
        ImGui::ColorButton("MyColor##3", *(ImVec4 *) &color, misc_flags, ImVec2(colorW, colorH));

        ImGui::PopItemWidth();

        //-

        // 1. USER PALETTE

        if (ofxImGui::BeginTree("USER PALETTE", COLOR_PICKER_Settings)){
            ofxImGui::AddParameter(this->bPaletteEdit);
            ofxImGui::AddParameter(this->bAddColor);
            ImGui::SameLine();
            ofxImGui::AddParameter(this->bRemoveColor);
            ofxImGui::AddParameter(this->bClearPalette);
            ofxImGui::EndTree(COLOR_PICKER_Settings);
        }

        //-

        // 2. COLOR HSB

        if (ofxImGui::BeginTree("COLOR", COLOR_PICKER_Settings))
        {
            ImGui::PushItemWidth(guiWidth * 0.77);

            // TODO: SHOULD APPLY HSV HERE, NOT INTO CALLBACK, BECAUSE IT WILL TRIG
            // THE COLOR PICKED UPDATING CALLBACK!!!

            // TEST
            LISTEN_isEnabled = false;

            if (ofxImGui::AddParameter(this->color_HUE))
            {
                ofLogNotice("ofxColorManager") << "ImGui: HUE MOVED !" << endl;
                ofColor c;
                c.set(color_picked.get());
                c.setHue(color_HUE);
                color_picked.set(c);
                Update_color_picked_CHANGES();
            }
            if (ofxImGui::AddParameter(this->color_SAT))
            {
                ofLogNotice("ofxColorManager") << "ImGui: SAT MOVED !" << endl;
                ofColor c;
                c.set(color_picked.get());
                c.setSaturation(color_SAT);
                color_picked.set(c);
                Update_color_picked_CHANGES();
            }
            if (ofxImGui::AddParameter(this->color_BRG))
            {
                ofLogNotice("ofxColorManager") << "ImGui: BRG MOVED !" << endl;
                ofColor c;
                c.set(color_picked.get());
                c.setBrightness(color_BRG);
                color_picked.set(c);
                Update_color_picked_CHANGES();
            }

            // TEST
            LISTEN_isEnabled = true;

            //-

            ofxImGui::AddParameter(this->bRandomColor);

            ImGui::PopItemWidth();
            ofxImGui::EndTree(COLOR_PICKER_Settings);
        }

        //-

//        // get color from outside color picked
//
//        // TEST
//        LISTEN_isEnabled = false;//maybe required because get() causes callbacks too (?)
//
//        // TEST
//        static ImVec4 color;
//        color.x = color_picked.get().r;
//        color.y = color_picked.get().g;
//        color.z = color_picked.get().b;
//        color.w = color_picked.get().a;
//
//        // TEST
//        LISTEN_isEnabled = true;

        //-

        if (ofxImGui::BeginTree("COLOR WHEEL", mainSettings))//grouped folder
        {
            // 0. custom button color big

            ImGui::PushItemWidth(guiWidth * widgetFactor);

//            int colorW = ImGui::GetWindowWidth() * widgetFactor;
//            int colorH = 30;
//            int misc_flags = ImGuiColorEditFlags_NoOptions|ImGuiColorEditFlags_NoTooltip;
//            ImGui::ColorButton("MyColor##3", *(ImVec4 *) &color, misc_flags, ImVec2(colorW, colorH));

            //--

            // 1. color picker

            // 1.1 circled & triangle
            ImGuiColorEditFlags colorEdiFlags =
                    ImGuiColorEditFlags_NoSmallPreview |
                            ImGuiColorEditFlags_NoTooltip |
                            ImGuiColorEditFlags_NoLabel |
                            ImGuiColorEditFlags_NoSidePreview |
                            ImGuiColorEditFlags_HSV |
                            ImGuiColorEditFlags_RGB |
                            ImGuiColorEditFlags_NoInputs |
                            ImGuiColorEditFlags_NoAlpha |
                            ImGuiColorEditFlags_PickerHueWheel;
            if (ImGui::ColorPicker4("MyColor##4", (float *) &color, colorEdiFlags))
            {
                // TEST
//                LISTEN_isEnabled = false;

                cout << "PICKER 1 MOVED !" << endl;
                color_picked = color;
//                color_picked_Update_To_HSV();

                // TEST
//                LISTEN_isEnabled = true;
            }

            //-

            // 1.2 squared box
            colorEdiFlags =
                    ImGuiColorEditFlags_NoSmallPreview |
                            ImGuiColorEditFlags_NoTooltip |
                            ImGuiColorEditFlags_NoLabel |
                            ImGuiColorEditFlags_NoSidePreview |
                            ImGuiColorEditFlags_HSV |
                            ImGuiColorEditFlags_RGB |
                            ImGuiColorEditFlags_HDR |
                            ImGuiColorEditFlags_NoAlpha |
                            ImGuiColorEditFlags_PickerHueBar;
            if (ImGui::ColorPicker4("MyColor##5", (float *) &color, colorEdiFlags))
            {
                // TEST
//                LISTEN_isEnabled = false;

                cout << "PICKER 2 MOVED !" << endl;
                color_picked = color;
//                color_picked_Update_To_HSV();

//                LISTEN_isEnabled = true;
            }

            //-

            ImGui::PopItemWidth();
            ofxImGui::EndTree(mainSettings);
        }

//        //--
//
//        // 2 load/create palete from colorBrowser
//
//
//        static bool saved_palette_inited = false;
//        //const int PaletteSIZE = ColorBrowser_palette.size();//error
//
////        static ImVec4 saved_palette[130];//same than openColor palettes
//        static ImVec4 saved_palette[2310];//same than Pantone palette
//
//        if (!saved_palette_inited)
//            for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
//            {
//                ofFloatColor c = ofColor(ColorBrowser_palette[n]);
//                saved_palette[n].x = c.r;
//                saved_palette[n].y = c.g;
//                saved_palette[n].z = c.b;
//                saved_palette[n].w = 1.0f;//alpha
//            }
//        saved_palette_inited = true;
//
//        //-
//
//        // 2.2 draw palette
//
//        if (ImGui::CollapsingHeader("PALETTE"))
//        {
//            for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
//            {
////                ImGui::PushItemWidth(guiWidth * 0.2);
//                ImGui::PushID(n);
//
////                // open colors
////                if ((n % 10) != 0) {//10 colors per row Open Color
////                    ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);
////                }
////
////                if (ImGui::ColorButton("##palette", saved_palette[n],
////                        ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip,
////                        ImVec2(20, 20)))
////                {
////                    color = ImVec4(saved_palette[n].x, saved_palette[n].y, saved_palette[n].z, color.w); // Preserve alpha!
//
//
//// pantone
//                if ((n%7) != 0) {//7 colors per row Pantone
//                    ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);
//                }
//
//                if (ImGui::ColorButton("##palette", saved_palette[n],
//                        ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip,
//                        ImVec2(20, 20)))
//                {
//                    color = ImVec4(saved_palette[n].x, saved_palette[n].y, saved_palette[n].z, color.w); // Preserve alpha!
//
//
//                            //-
//
//                    // TEST
////                    LISTEN_isEnabled = false;
//                    ofLogNotice("ofxColorManager") << "ImGui: PALETTE PICKED !" << endl;
//                    color_picked = color;
////                    LISTEN_isEnabled = true;
//
//                    //-
//                }
//
////                ImGui::PopItemWidth();
//                ImGui::PopID();
//            }
//        }

    }
    ofxImGui::EndWindow(COLOR_PICKER_Settings);

    //-------------------------------------------------------------------

    // 2ND WINDOW

    auto COLOR_MANAGER_Settings = ofxImGui::Settings();
    COLOR_MANAGER_Settings.windowPos = ofVec2f(gui2_x, gui2_y);
    COLOR_MANAGER_Settings.windowSize = ofVec2f(gui2_w, gui2_h);

    if (ofxImGui::BeginWindow("COLOR MANAGER", COLOR_MANAGER_Settings, false))
    {
        // GENERAL DATA

        if (ofxImGui::BeginTree("BACKGROUND", mainSettings))
        {
            //--TEST. can't do with pointers to ofParameter...
            ImGui::PushItemWidth(80);
            static ImVec4 color;
            color.x = color_backGround.get().r;
            color.y = color_backGround.get().g;
            color.z = color_backGround.get().b;
            color.w = color_backGround.get().a;

            // squared box
            ImGuiColorEditFlags colorEdiFlags =
                    ImGuiColorEditFlags_NoSmallPreview |
                            ImGuiColorEditFlags_NoTooltip |
                            ImGuiColorEditFlags_NoLabel |
                            ImGuiColorEditFlags_NoSidePreview |
                            ImGuiColorEditFlags_NoInputs |
                            ImGuiColorEditFlags_NoAlpha |
                            ImGuiColorEditFlags_PickerHueBar;

            ImGui::ColorPicker4("Background Color", (float *) &color, colorEdiFlags);
            color_backGround = color;

            ImGui::PopItemWidth();

            //-

            ImGui::PushItemWidth(guiWidth * 0.5);
            ofxImGui::AddParameter(this->color_backGround_SET);
            ofxImGui::AddParameter(this->color_backGround_SETAUTO);
            ofxImGui::EndTree(mainSettings);
            ImGui::PopItemWidth();
        }

        if (ofxImGui::BeginTree("PANELS", mainSettings))
        {
            ofxImGui::AddParameter(this->SHOW_Gradient);
            ofxImGui::AddParameter(this->SHOW_Curve);
            ofxImGui::AddParameter(this->SHOW_TEST_Curve);
            ofxImGui::AddParameter(this->SHOW_ColourLovers);
            ofxImGui::AddParameter(this->SHOW_AlgoPalettes);
            ofxImGui::AddParameter(this->SHOW_BrowserColors);
            ofxImGui::EndTree(mainSettings);
        }

        //--

        // CURVE TOOL

        if (ImGui::CollapsingHeader("CURVE TOOL")) {
            ImGui::PushItemWidth(guiWidth * 0.5);
            ofxImGui::AddParameter(this->gradient_hard);
            if (ofxImGui::AddParameter(this->curve_pos))
            {
                curve_pos_slider.setPercent(curve_pos.get());
            }
            ofxImGui::AddParameter(this->curve_pos_out);
            ofxImGui::AddParameter(this->bResetCurve);
            // ofxImGui::AddParameter(this->curveMod);
            ImGui::PopItemWidth();
        }

        //--

        // TEST CURVE

        if (ImGui::CollapsingHeader("TEST CURVE")) {
            ImGui::PushItemWidth(guiWidth * 0.5);
            ImGui::Checkbox("ENABLE TEST", &TEST_MODE);
            ImGui::Checkbox("CYCLE", &TEST_CycleMODE);
            ImGui::Checkbox("TO BACKGROUND", &TEST_toBackground);
            ImGui::SliderFloat("SPEED", &TEST_Speed, 0.0f, 1.0f);
            ImGui::PopItemWidth();
        }

        //--

        // ALGORITHMIC PALETTES

        if (ImGui::CollapsingHeader("ALGORITHMIC PALETTES")) {
            ImGui::PushItemWidth(guiWidth * 0.5);
            ofxImGui::AddParameter(this->MODE_Palette);
            if (!MODE_Palette) {
                ofxImGui::AddParameter(this->SATURATION);
                ofxImGui::AddParameter(this->BRIGHTNESS);
            }
            ofxImGui::AddParameter(this->NUM_ALGO_PALETTES);
            ofxImGui::AddParameter(this->bRandomPalette);
            ofxImGui::AddParameter(this->bLock_palette);
            ofxImGui::AddParameter(this->bAuto_palette_recall);
            ImGui::PopItemWidth();
        }
    }
    ofxImGui::EndWindow(COLOR_MANAGER_Settings);

    //-

//    this->gui.end();
//    return mainSettings.mouseOverGui;

    //-

    // NOTE:
    // I wrongly supposed that the begin() and end() methods of OfxImGui were related with ImGui::Begin
    // and ImGui::End methods which is not the case…
    // The right calling sequence is
    // im_gui.begin();
    // Addon begin ImGui::SetNextWindowPos(ImVec2(0,0));
    // ImGui::SetNextWindowSize(ImVec2(1200,800),
    // ImGuiSetCond_Once);
    // ImGui::Begin("Funky Window"); /* Here your ImGui stuff
    // */ ImGui::End();
    // im_gui.end(); // Addon end




    //-------------------------------------------------------------------

    // 3rd WINDOW

    auto palette_Settings = ofxImGui::Settings();
    palette_Settings.windowPos = ofVec2f(300, 300);
    palette_Settings.windowSize = ofVec2f(gui2_w, 300);

    if (ofxImGui::BeginWindow("PALETTE", palette_Settings, false))
    {
        //--

        // GET COLOR FROM OUTSIDE COLOR PICKED

        // TEST
        LISTEN_isEnabled = false;//maybe required because get() causes callbacks too (?)

        // TEST
        static ImVec4 color;
        color.x = color_picked.get().r;
        color.y = color_picked.get().g;
        color.z = color_picked.get().b;
        color.w = color_picked.get().a;

        // TEST
        LISTEN_isEnabled = true;

        //--
        //--

        // 2 load/create palete from colorBrowser


        static bool saved_palette_inited = false;
        //const int PaletteSIZE = ColorBrowser_palette.size();//error

//        static ImVec4 saved_palette[130];//same than openColor palettes
        static ImVec4 saved_palette[2310];//same than Pantone palette

        if (!saved_palette_inited)
            for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
            {
                ofFloatColor c = ofColor(ColorBrowser_palette[n]);
                saved_palette[n].x = c.r;
                saved_palette[n].y = c.g;
                saved_palette[n].z = c.b;
                saved_palette[n].w = 1.0f;//alpha
            }
        saved_palette_inited = true;

        //-

        // 2.2 draw palette

        if (ImGui::CollapsingHeader("PALETTE"))
        {
            for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
            {
//                ImGui::PushItemWidth(guiWidth * 0.2);
                ImGui::PushID(n);

//                // open colors
//                if ((n % 10) != 0) {//10 colors per row Open Color
//                    ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);
//                }
//
//                if (ImGui::ColorButton("##palette", saved_palette[n],
//                        ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip,
//                        ImVec2(20, 20)))
//                {
//                    color = ImVec4(saved_palette[n].x, saved_palette[n].y, saved_palette[n].z, color.w); // Preserve alpha!


// pantone
                if ((n%7) != 0) {//7 colors per row Pantone
                    ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);
                }

                if (ImGui::ColorButton("##palette", saved_palette[n],
                        ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip,
                        ImVec2(20, 20)))
                {
                    color = ImVec4(saved_palette[n].x, saved_palette[n].y, saved_palette[n].z, color.w); // Preserve alpha!


                    //-

                    // TEST
//                    LISTEN_isEnabled = false;
                    ofLogNotice("ofxColorManager") << "ImGui: PALETTE PICKED !" << endl;
                    color_picked = color;
//                    LISTEN_isEnabled = true;

                    //-
                }

//                ImGui::PopItemWidth();
                ImGui::PopID();
            }
        }
    }
    ofxImGui::EndWindow(palette_Settings);




    this->gui.end();
    return mainSettings.mouseOverGui;
}


//--------------------------------------------------------------
void ofxColorManager::palette_load_ColourLovers()
{
    ofLogNotice("ofxColorManager") << "palette_load_ColourLovers";

    // 1. erase user palette and fill a new one with just update/received colour lovers
    palette_clear();
    ofColor c;
    for (int i = 0; i< myPalette.size(); i++)
    {
        c = myPalette[i];
        ofLogNotice("ofxColorManager") << "color [" << i << "] " << ofToString(c);
        palette_addColor(c);
    }

//    // 2. get color from colour lovers
//    color_clicked = ofColor(myColor);

    //-

    // workflow: set background color from first/last palette color
    if (color_backGround_SETAUTO)
    {
        if (palette.size() >0)
        {
            int size = palette.size();
            ofColor c;
            c = palette[0];//get first color
            color_backGround.set( c );
        }
    }

    //-
}


//--------------------------------------------------------------
void ofxColorManager::update()
{
    ofSetWindowTitle(ofToString(ofGetFrameRate()));

    //---

    // DEMO 1

    if (bDEMO1_clear)
//    if (bDEMO1_clear || locations.size()>10)
    {
        bDEMO1_clear = false;
        locations.clear();
        velocities.clear();
        colors.clear();

    }
    {
//        int bloquer = locations.size()>10
        ofColor color;

        if (ofRandom(100) < 10) {
//        if (ofRandom(100) < 50) {//prob speed?
//        if (ofRandom(100) < 80) {//prob speed?

            this->locations.push_back(glm::vec2());
            this->velocities.push_back(glm::normalize(glm::vec2(ofRandom(-1, 1), ofRandom(-1, 1))) * 2);

//        color.setHsb(ofRandom(255), 255, 255);

            float RandomNorm = ofRandom(0., 1.);
            color.set(getColorAtPercent(RandomNorm));
            this->colors.push_back(color);
        }

        for (int i = this->locations.size() - 1; i > -1; i--) {

            this->locations[i] += this->velocities[i];

            if (glm::length(this->locations[i]) > 720) {

                this->locations.erase(this->locations.begin() + i);
                this->velocities.erase(this->velocities.begin() + i);
                this->colors.erase(this->colors.begin() + i);
            }
        }
    }

    //---

    // TEST CURVE

    if (TEST_MODE) {
        int frameBuffer = (int)ofMap(TEST_Speed, 0., 1., TEST_maxFrames, 30);
        int frameCurrent = ofGetFrameNum()%frameBuffer;//0 to maxFrames
        framePrc = ofMap(frameCurrent, 0, frameBuffer, 0., 1.);
        float control;
        if (!TEST_CycleMODE)
            control = ofClamp(framePrc, 0., 1.);
        else{
            float mySin = std::sin(PI * framePrc);
            control = ofClamp(mySin, 0., 1.);
        }
        if (TEST_toBackground)
            color_backGround.set( getColorAtPercent(control) );
        setControl(control);
    }

    //--

    // COLOUR LOVERS

    ColourLoversHelper.update();

    if (bUpdated_Palette_BACK)
    {
        bUpdated_Palette_BACK = false;
        ofLogWarning("ofxColorManager") << "update:bUpdated_Palette_BACK: " << bUpdated_Palette_BACK;

        // TODO: workflow: if mode is palette&color should load first palette color
        // and forget the clicked color

        // 1. get palette colors from colour lovers
        palette_load_ColourLovers();

        //-

        //TODO
        // workflow: when loading a colour lover palette we disable auto create from algo palettes
        if (bAuto_palette_recall)
        {
            bAuto_palette_recall = false;
        }


        // clear DEMO1 objects
        bDEMO1_clear = true;
    }

    if (bUpdated_Color_BACK)
    {
        bUpdated_Color_BACK = false;
        ofLogWarning("ofxColorManager") << "update:bUpdated_Color_BACK: " << bUpdated_Color_BACK;

        // workflow: TODO: disable to avoid overwrite the selected color into the palette just created
        if (ColourLoversHelper.MODE_PickPalette_BACK &&
                ColourLoversHelper.MODE_PickColor_BACK)
        {
            if (bPaletteEdit) {
                bPaletteEdit = false;
            }
        }

        // 2. get color from colour lovers
        color_clicked = ofColor(myColor);

        // 3. auto create user palette from algo palette from colour lover picked color
        if (!ColourLoversHelper.MODE_PickPalette_BACK && ColourLoversHelper.MODE_PickColor_BACK)
        {
            if (bAuto_palette_recall)
            {
                palette_clear();
                palette_recallFromPalettes(SELECTED_palette_LAST);//trig last choice
            }
        }


        // clear DEMO1 objects
        bDEMO1_clear = true;
    }

    //--

    // handle last selected algorithmic palette
    if (SELECTED_palette != SELECTED_palette_PRE)
    {
        ofLogNotice("ofxColorManager") << "update:CHANGED SELECTED_palette: " << SELECTED_palette;

        // when a label palette is clicked, will always trig and load the palette
        // TODO: BUG should add this to avoid auto load to user palette
        palette_recallFromPalettes(SELECTED_palette);

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
    curveTool_update();
    ColorBrowser.update();

    //-

    if (color_clicked != color_clicked_PRE )
    {
        color_clicked_PRE = color_clicked;
        ofLogNotice("ofxColorManager") << "update:CHANGED color_clicked";
        color_picked.set(color_clicked);
    }

    //-

    // set the local scope color pointer that is into ofxColorBrowser that whill be used as color picked too
    if (color_BACK != color_BACK_PRE )
    {
        color_BACK_PRE = color_BACK;
        ofLogNotice("ofxColorManager") << "update:CHANGED color_BACK pointer";
        color_picked.set(color_BACK);

        if (!bPaletteEdit)
        {
            if (bAuto_palette_recall)
            {
                palettes_update();
                palette_recallFromPalettes(SELECTED_palette_LAST);//trig last choice
            }
        }

        //-
    }
}


//--------------------------------------------------------------
void ofxColorManager::curveTool_setup()
{
    //gradient here too
    curve_img_gradient.allocate(image_curvedGradient_w, image_curvedGradient_h, OF_IMAGE_COLOR);

    curvesTool.setup(curveTool_amount);
    //TEMP
    curvesTool.load("curves.yml"); //needed because it fills polyline

    curve_pos.set("INPUT", 0., 0., 1.);
    curve_pos_out.set("OUTPUT", 0., 0., 1.);
    bResetCurve.set("RESET CURVE", false);
    SHOW_TEST_Curve.set("SHOW TEST CURVE", false);
    params_curve.add(curve_pos);
    params_curve.add(curve_pos_out);
    params_curve.add(bResetCurve);
    params_curve.add(SHOW_TEST_Curve);
    curveMod.set("CURVE MOD", 0.5, 0., 1.);
    ofAddListener(params_curve.parameterChangedE(), this, &ofxColorManager::Changed_CONTROL);

    //-

    // slider live test color out for this input
    curve_pos_slider.setup(slider_x+(slider_w+pad), slider_y, slider_w, slider_h, 0., 1., 0, true, false);
    curve_pos_slider.setVisible(SHOW_TEST_Curve);

    // curve mod
    curveMod = 0.5;
    curveMod_Slider.setup(sliderMod_x, sliderMod_y, sliderMod_w, sliderMod_h, 0., 1., 0, true, false);
    curveMod_Slider.setPercent(curveMod);
    curveMod_Slider.setVisible(SHOW_TEST_Curve);
//    curve_pos_slider.setLabelString("input");
}


//--------------------------------------------------------------
void ofxColorManager::curveTool_update()
{
    // update values
    curve_pos = curve_pos_slider.getValue();
    curve_pos_LUT = (int) ofMap( curve_pos.get(), 0., 1., 0, curveTool_amount-1 );

    // curve modifier
    curveMod = curveMod_Slider.getValue();
    int pointsSize = curvesTool.getPointSize();
    int pointToModify;//default 1 if size is 3 points
    int pointY;
    if (pointsSize%2!=0 && pointsSize>=3)//impar
        pointToModify = pointsSize/2;
    else if (pointsSize%2==0 && pointsSize>=3)
        pointToModify = pointsSize/2-1;
    pointY = (curvesTool.getPoint(pointToModify)).x;
    curvesTool.set(pointToModify, ofVec2f(pointY, ofMap(curveMod,0.,1.,0,curveTool_amount)));

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
}


//--------------------------------------------------------------
void ofxColorManager::gradient_draw() {

    // GRADIENT
    // 1. un-curved gradient rectangle (left positioned)
    gradient.drawDebug(grad_x, grad_y, grad_w, grad_h);
}


//--------------------------------------------------------------
void ofxColorManager::curveTool_draw() {

    ofPushMatrix();
    ofPushStyle();

    // NOTE: sometimes we need some tricky hacking to avoid rare fliping from gradients
    curve_pos_out = ofMap( curvesTool.getAtPercent(curve_pos), 0, curveTool_amount-1, 0., 1.);

    // 3. curve tool
    ofTranslate(curveTool_x, curveTool_y);

    // 3.1 image box gradient LUT
    curve_img_gradient.draw(image_curvedGradient_x, image_curvedGradient_y);

    // 3.2 curve splines editor
    ofSetColor(255);
    curvesTool.draw(0, 0, curve_pos_LUT);

    if (SHOW_TEST_Curve)
    {
        // 3.3 horizontal line
        ofSetColor(ofColor::white);
        float y = curveTool_amount - curvesTool[curve_pos_LUT];
        ofDrawLine(0, y, curveTool_amount, y);

//        // 3.4 current pos circle point
//        ofSetColor(25);
//        ofDrawCircle(curve_pos_LUT, y, 3);
    }

    //-

    ofPopMatrix();
    ofPopStyle();

    //-

//    // debug curve values
//    int posx, posy;
//    posx = 425;
//    posy = 20;
//    ofDrawBitmapStringHighlight("in : "+ofToString(curve_pos), glm::vec2(posx, posy));
//    ofDrawBitmapStringHighlight("out: "+ofToString(curve_pos_out), glm::vec2(posx, posy + 20));
//    ofDrawBitmapStringHighlight("lut: "+ofToString(curve_pos_LUT), glm::vec2(posx, posy + 40));
//    }

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
void ofxColorManager::palette_recallFromPalettes(int p)
{
    ofLogNotice("ofxColorManager") << "palette_recallFromPalettes: " << p;

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

    //-

    // workflow: set background color from first/last palette color
    if (color_backGround_SETAUTO)
    {
        int size = palette.size();
        ofColor c;
        c = palette[0];//get first color
        color_backGround.set( c );
    }

    //-

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

    //-

    if (bAuto_palette_recall)
    {
        palette_clear();
        palette_recallFromPalettes(SELECTED_palette_LAST);//trig last choice
    }
}

//--------------------------------------------------------------
void ofxColorManager::palettes_setVisible(bool b)
{
    // hide and disable touchs for buttons
    //-

    // 1. ALGORITMIC PALETTES

    for (auto& btn : btns_plt_Triad)
    {
        btn->setVisible(b);
        btn->setEnabled(b);
    }
    for (auto& btn : btns_plt_ComplTriad)
    {
        btn->setVisible(b);
        btn->setEnabled(b);
    }
    for (auto& btn : btns_plt_CompSat)
    {
        btn->setVisible(b);
        btn->setEnabled(b);
    }
    for (auto& btn : btns_plt_ComplBrgt)
    {
        btn->setVisible(b);
        btn->setEnabled(b);
    }
    for (auto& btn : btns_plt_MonoSat)
    {
        btn->setVisible(b);
        btn->setEnabled(b);
    }
    for (auto& btn : btns_plt_MonoBrgt)
    {
        btn->setVisible(b);
        btn->setEnabled(b);
    }
    for (auto& btn : btns_plt_Analog)
    {
        btn->setVisible(b);
        btn->setEnabled(b);
    }
    for (auto& btn : btns_plt_Random)
    {
        btn->setVisible(b);
        btn->setEnabled(b);
    }

    //-

    // 2. LABEL BUTTONS

    for (auto& btn : btns_plt_Selector)
    {
        btn->setVisible(b);
    }
}

//--------------------------------------------------------------
void ofxColorManager::draw()
{
    // BACKGROUND

    if (backgroundENABLE)
    {
        ofClear(ofColor(color_backGround.get()));
    }

    //--

    // DEMO 1

    if (ENABLE_DEMO1) {
        ofPushMatrix();
        ofPushStyle();
//    ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

        ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
//    ofTranslate(ofGetMouseX(), ofGetMouseY());

        for (int i = 0; i < this->locations.size(); i++) {

            ofSetColor(this->colors[i]);

//        float radius = (2 * PI * glm::length(this->locations[i])) / 360 * 10;
//        float radius = (2 * PI * glm::length(this->locations[i])) / 360 * 20;
            float radius = (2 * PI * glm::length(this->locations[i])) / 360 * 40;


            ofDrawCircle(this->locations[i], radius);
        }

//    ofDisableAlphaBlending();
        ofPopStyle();
        ofPopMatrix();
    }

    //--

    // DEMO 2

    if (ENABLE_DEMO2) {
        this->cam.begin();

        ofPushMatrix();
        ofPushStyle();
//    ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

        float radius = 25;
//    float radius = 300;
//    float radius = 150;

        int iDeg = 25;
//    int iDeg = 360./palette.size();

        for (int deg = 0; deg < 360; deg += iDeg) {
            float x = radius * cos(deg * DEG_TO_RAD);
            float y = radius * sin(deg * DEG_TO_RAD);

            ofColor c;
//        c.setHsb(deg / 360.f * 255, 255, 255);
//        float cnt = deg / 360.f;
            float cnt = ofMap(deg, 0, 360, 0.f, 1.f);
            c = getColorAtPercent(cnt);
            ofSetColor(c);

            ofRotateZ(ofGetFrameNum() * 0.25);

            ofPushMatrix();
            ofTranslate(ofVec3f(x, y, 0));
            ofRotateX(90);
            ofRotateY(deg + 90);

//        ofRect(ofVec3f(0, 0, 0), 60, 150);
//        ofRect(ofVec3f(0, 0, 0), 200, 200);
            ofRect(ofVec3f(0, 0, 0), 1000, 1000);

            ofPopMatrix();
        }

//    ofDisableAlphaBlending();
        ofPopStyle();
        ofPopMatrix();

        this->cam.end();
    }

    //--

    if (SHOW_ALL_GUI) {

        ofPushMatrix();
        ofPushStyle();

        //-

        // COLOR BOX PICKER (CURRENT)

        ofPushStyle();
        ofFill();
        ofSetColor(ofColor( color_picked.get() ));
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
        if (SHOW_Gradient) {
            gradient_draw();
        }

        //-

        // TEST CURVE

        // COLOR MONITORING SLIDER POSITION ON CURVED GRADIENT
        if (SHOW_TEST_Curve) {

            //gradiented color
            ofRectangle r(currColor_x, currColor_y, box_size / 2, slider_h);
            // 2. current box color at input curve point (right positioned)
            float out = ofMap(curvesTool.getAtPercent(1.0 - curve_pos), 0, curveTool_amount - 1, 1., 0.);
            ofColor c = gradient.getColorAtPercent(out);

            ofRectangle recBar(0,ofGetHeight()-5,framePrc*ofGetWidth(), 5);

            ofPushStyle();
            ofFill();
            ofSetColor(c);
            ofDrawRectangle(r);
            ofSetColor(ofColor(ofColor::black, 200));
//            ofSetColor(ofColor::black);
            ofDrawRectangle(recBar);
            ofPopStyle();
        }

        //-

        // CURVE
        if (SHOW_Curve) {
            curveTool_draw();
        }

        // COLORS BROWSER
        if (SHOW_BrowserColors)
        {
            ColorBrowser.draw();
        }

        // COLOUR LOVERS
        if (SHOW_ColourLovers && false)
        {
            draw_ColourLovers();
        }

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

        ofPopMatrix();
        ofPopStyle();
    }

    //-

    if (SHOW_GUI_MINI)
    {
        draw_PaleteMINI();
    }
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
    ofLogNotice("ofxColorManager:palette_addColor") << ofToString(c);
    palette.push_back( c );
    gradient.addColor( c );
    palette_addColor_toInterface( c );

    // TODO: BUG
    if (palette_colorSelected>-1 && bPaletteEdit)
    {
        palette_colorSelected = palette.size()-1;//select last one, just created now
    }
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

    palette_rearrenge();
}


//--------------------------------------------------------------
void ofxColorManager::palette_touched(string name)
{
    // handle de-select all buttons and select last clicked
    for (int i=0; i<btns_palette.size(); i++)
    {
        if (btns_palette[i]->getName() != name)
        {
            btns_palette[i]->setSelected(false);
        }
        else
        {
            btns_palette[i]->setSelected(true);
            palette_colorSelected = i;
            ofLogNotice("ofxColorManager") << "selected color: " << i;
        }
    }

    // workflow: auto set edit mode
    if (!bPaletteEdit)
    {
        bPaletteEdit = true;
    }
}


//--------------------------------------------------------------
void ofxColorManager::palette_clear()
{
    // remove all colors of user palette

    ofLogNotice("ofxColorManager") << "palette_clear";

    palette.clear();
    gradient.reset();

//    ofLogNotice("ofxColorManager") << "palette_clear::getNumChildren: " << scene->getNumChildren();

    for (int i=0; i< btns_palette.size(); i++)
    {
        std::string n = ("btn" + ofToString(i));
        auto a = scene->getChildWithName(n, 1000);
        auto b = a->getName();
        scene->removeChild(a, false);
        ofLogNotice("ofxColorManager") << "removed children: " << b;
    }
    btns_palette.clear();



    // clear DEMO1 objects
    bDEMO1_clear = true;
}

//--------------------------------------------------------------
void ofxColorManager::color_picked_Update_To_HSV(){
    color_HUE = 255 * color_picked.get().getHue();
    color_SAT = 255 * color_picked.get().getSaturation();
    color_BRG = 255 * color_picked.get().getBrightness();
}

//--------------------------------------------------------------
void ofxColorManager::Changed_CONTROL(ofAbstractParameter &e) {

    string name = e.getName();

    // TODO: should reduce callbacks in output..
//    if (name != "INPUT" && name != "OUTPUT" &&
//            name!="H" && name!="S" && name!="V")

    if (name!="INPUT" && name!="OUTPUT")
    {
        ofLogNotice("ofxColorManager") << "Changed_CONTROL: " << name << ": " << e;
    }

    //--

    // CONTROL WINDOWS PANELS

    if (name == "SHOW COLOUR LOVERS") {
        ColourLoversHelper.setVisible(SHOW_ColourLovers);
    }
    else if (name == "SHOW PALETTES") {
        palettes_setVisible(SHOW_AlgoPalettes);
    }
    else if (name == "SHOW BROWSER COLORS") {
        ColorBrowser.setVisible(SHOW_BrowserColors);
    }
    else if (name == "SHOW CURVE TOOL") {
        curveMod_Slider.setVisible(SHOW_Curve);
    }
    else if (name == "SHOW TEST CURVE") {
        curve_pos_slider.setVisible(SHOW_TEST_Curve);
    }

    //--

    // COLOR

    // TODO: should reduce callbacks
    // TEST
    if (LISTEN_isEnabled == true)
    {
        if (name=="H" && name=="S" && name=="V")
        {
            ofLogNotice("ofxColorManager") << "Changed_CONTROL:LISTEN_isEnabled: " << name << ":" << e;
        }

        if (name == "H") {
            ofColor c;
            c.set(color_picked.get());
            c.setHue(color_HUE);
            color_picked.set(c);
        }
        else if (name == "S") {
            ofColor c;
            c.set(color_picked.get());
            c.setSaturation(color_SAT);
            color_picked.set(c);
        }
        else if (name == "V") {
            ofColor c;
            c.set(color_picked.get());
            c.setBrightness(color_BRG);
            color_picked.set(c);
        }
    }
    // TEST
    // LISTEN_isEnabled

    //--

    // PALLETE

    if (name == "RANDOM COLOR") {
        if (bRandomColor) {
            bRandomColor = false;
            color_picked = ofFloatColor(ofRandom(0., 1.), ofRandom(0., 1.), ofRandom(0., 1.));
        }
    } else if (name == "ADD COLOR") {
        if (bAddColor) {
            bAddColor = false;
            palette_addColor(ofColor(color_picked.get()));
        }
    } else if (name == "EDIT COLOR") {
        for (int i = 0; i < btns_palette.size(); i++) {
            if (!bPaletteEdit) {
                btns_palette[i]->setSelectable(false);
                btns_palette[i]->setSelected(false);//deselect each
                palette_colorSelected = -1;
            } else if (bPaletteEdit) {
                btns_palette[i]->setSelectable(true);
                btns_palette[i]->setSelected(false);
            }
        }
        if (bPaletteEdit && btns_palette.size() > 0) {
            palette_colorSelected = 0;
            btns_palette[0]->setSelected(true);
        }
    } else if (name == "REMOVE COLOR") {
        if (bRemoveColor) {
            bRemoveColor = false;
            palette_removeColorLast();
        }
    } else if (name == "CLEAR PALETTE") {
        if (bClearPalette) {
            bClearPalette = false;
            palette_clear();
        }
    }

        //-

        // ALGORITHMIC PALETTES

    else if (name == "RANDOM PALETTE") {
        if (bRandomPalette) {
            bRandomPalette = false;

            random.generateRandom(NUM_ALGO_PALETTES);
            palettes_update();
            if (bAuto_palette_recall) {
                //set random palette to user palette
                palette_recallFromPalettes(7);
            }
        }
    } else if (name == "SIZE COLORS") {
        palettes_resize();
    } else if (name == "AUTO CREATE!") {
        if (bAuto_palette_recall)
            bLock_palette = false;
    } else if (name == "LOCK PALETTES") {
        if (bLock_palette)
            bAuto_palette_recall = false;
    } else if (name == "BRIGHTNESS" || name == "SATURATION") {
        palettes_update();
//        cout << name << endl;
    }

        //--

        // CURVE

//    else if (name == "INPUT") {
//    }

    else if (name == "RESET CURVE")
    {
        if (bResetCurve)
        {
            bResetCurve = false;
            curvesTool.clear();
            curvesTool.add(ofVec2f(0, 0));
            curvesTool.add(ofVec2f(127, 127));
            curvesTool.add(ofVec2f(255, 255));
            curveMod = 0.5;
            curveMod_Slider.setPercent(curveMod);
        }
    }
    else if (name == "GRADIENT HARD") {
        gradient.setHardMode(gradient_hard);
    }

        //-

        // BACKGROUND
    else if (name == "SET FROM COLOR") {
        if (color_backGround_SET) {
            color_backGround_SET = false;
            color_backGround.set(ofColor(color_picked.get()));
        }
    } else if (name == "AUTO SET") {

    }
}


//--------------------------------------------------------------
void ofxColorManager::keyPressed( ofKeyEventArgs& eventArgs )
{
    const int & key = eventArgs.key;
//    ofLogNotice("ofxColorManager") << "key: " << key;

    //-

    if (key == 'g')
    {
        SHOW_ALL_GUI = !SHOW_ALL_GUI;
        setVisible(SHOW_ALL_GUI);
    }
    else if (key == 'e')
    {
        bPaletteEdit = !bPaletteEdit;
    }
//    else if (key == 'l')
//    {
//        bLock_palette = !bLock_palette;
//    }

    else if (key == 'm')
    {
        mouseRuler.toggleVisibility();
    }

    else if (key == 'd')
    {
        bShowDebug = !bShowDebug;
    }

        //-

        // randomize color and build palette if enabled

    else if (key == 'o')
    {
        //TODO
        // workflow: when loading a colour lover palette we disable auto create from algo palettes
        if (!bAuto_palette_recall)
        {
            bAuto_palette_recall = true;
        }

        //TODO: bug because some trigs are flags. we need direct functions
//        bRandomColor = true;
        color_picked = ofFloatColor(ofRandom(0., 1.), ofRandom(0., 1.), ofRandom(0., 1.));

        if (bAuto_palette_recall) {
            palettes_update();
            palette_recallFromPalettes(SELECTED_palette_LAST);//trig last choice
        }

        // undo
        color_Undo = color_picked.get();
        color_Undo.store();


        // clear DEMO1 objects
        bDEMO1_clear = true;
    }

        //--

        // trig get algorithmic random palette

    else if (key == 'p')
    {
        //TODO
        // workflow: when loading a colour lover palette we disable auto create from algo palettes
        if (!bAuto_palette_recall)
        {
            bAuto_palette_recall = true;
        }

        random.generateRandom(NUM_ALGO_PALETTES);
        palettes_update();
        if (bAuto_palette_recall) {
            //set random palette to user palette
            palette_recallFromPalettes(7);
        }

        // clear DEMO1 objects
        bDEMO1_clear = true;
    }

        //-

        // randomly get a palete from colour lovers

    else if (key == 'v') {
        ColourLoversHelper.randomPalette();
        // clear DEMO1 objects
        bDEMO1_clear = true;
    }

        //--

        // UNDO

    else if (key == 'z')
    {
        color_Undo.undo();
        color_picked = color_Undo;

    }
    else if (key == 'y')
    {
        color_Undo.redo();
        color_picked = color_Undo;
    }
//    else if()
//        color_Undo.clearRedo();

        //--

        // PRESET CLASS

//    else if (key == 'z') {
//        preset_save(PRESET_name);
//    }
    else if (key == 'l') {
        preset_load(PRESET_name);
    }

        // SAVE
    else if (key == 's') {
        myPresetPalette.setName(PRESET_name);
        myPresetPalette.setCurveName(PRESET_curveName);
//        myPresetPalette.setBackgroundColor(color_backGround);//error ofParameter
        myPresetPalette.setPalette(palette);
        myPresetPalette.preset_save(PRESET_name);
    }

//--

//    if (key == 's') {
//        curvesTool.save("curves  );
//    }
//    if (key == 'l') {
//        curvesTool.load("curves.yml");
//    }

//    if (key == 's')
//    {
//        palette_save("myPalette");
//    }
//    if (key == 'l')
//    {
//        palette_load("myPalette");
//    }

    else if (key == 'c')
    {
        palette_clear();
    }
    else if (key == 'x')
    {
        palette_removeColorLast();
    }
    else if (key == 'a')
    {
        color_picked = ofFloatColor(ofRandom(0., 1.), ofRandom(0., 1.), ofRandom(0., 1.));
        palette_addColor(ofColor(color_picked.get()));
    }

        //-

        // COLOR BROWSER

////    ColorBrowser.keyPressed( eventArgs );
//
//    else if (key == ' ')
//        ColorBrowser.switch_palette_Type();
//
//    else if (key == OF_KEY_RETURN)
//        ColorBrowser.switch_sorted_Type();

        //-

        // COLOUR LOVERS

    else if (key == OF_KEY_DOWN)
    {
        ColourLoversHelper.nextPalette();
    }
    else if (key == OF_KEY_UP)
    {
        ColourLoversHelper.prevPalette();
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
//    ofLogNotice("ofxColorManager") << "mouseDragged " <<  x << ", " << y << ", " << button;

    TouchManager::one().touchMove(button, ofVec2f(x, y));

    //-
}


//--------------------------------------------------------------
void ofxColorManager::mousePressed(ofMouseEventArgs& eventArgs){
    const int & x = eventArgs.x;
    const int & y = eventArgs.y;
    const int & button = eventArgs.button;
//    ofLogNotice("ofxColorManager") << "mousePressed " <<  x << ", " << y << ", " << button;

    TouchManager::one().touchDown(button, ofVec2f(x, y));

    //-

//    if (bPaletteEdit)
//    {
    // filter touch if its an user palette button only
    auto a = TouchManager::one().getComponentUnder( ofVec2f(x, y) );
    auto b = a->getName();
    ofLogVerbose("ofxColorManager") << "touched: " << b;
    auto str = ofSplitString(b, "btn");
    if (str.size() > 1)//check if "btn" is present
    {
        ofLogVerbose("ofxColorManager") << "detected palette touch: " << b;
        palette_touched(b);
    }
    else
    {
        //ofLogVerbose("ofxColorManager") << "ignored touch: " << b;
    }
//    }

    //-
}


//--------------------------------------------------------------
void ofxColorManager::mouseReleased(ofMouseEventArgs& eventArgs){
    const int & x = eventArgs.x;
    const int & y = eventArgs.y;
    const int & button = eventArgs.button;
//    ofLogNotice("ofxColorManager") << "mouseReleased " <<  x << ", " << y << ", " << button;

    TouchManager::one().touchUp(button, ofVec2f(x, y));

    //-

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
void ofxColorManager::Update_color_picked_CHANGES()
{
    // executes workflow app

    ofLogNotice("ofxColorManager") << "Update_color_picked_CHANGES: " << ofToString(color_picked);

//    // TEST
//    color_picked_Update_To_HSV();//redundant...

    color_clicked.set(color_picked);//TODO: mirror clicked/picked colors

    //--

    // 1. autosave edited color

    if (bPaletteEdit && palette_colorSelected != -1 && btns_palette.size() > palette_colorSelected) {
        // update user palette color with recently picked color
        btns_palette[palette_colorSelected]->setColor(color_clicked);

        // update gradient for selected index color
        if (gradient.getNumColors() > palette_colorSelected) {
            gradient.replaceColorAtIndex(palette_colorSelected, btns_palette[palette_colorSelected]->getColor());
        }
    }

    // 2. when color picked changes, auto trig and put generated palette to user palette

    if (bAuto_palette_recall) {
        palettes_update();
        palette_recallFromPalettes(SELECTED_palette_LAST);//trig last choiced algorithmic palette
    }

    // 3. recreate algorithmic palettes if locking is disable

    if (!bLock_palette) {
        palettes_update();
    }
}

//--------------------------------------------------------------
void ofxColorManager::Changed_color_picked(ofFloatColor &_c)
{
    // TEST
    if (LISTEN_isEnabled == true)
    {
        Update_color_picked_CHANGES();

        // TEST
        color_picked_Update_To_HSV();//redundant...
    }
}


//--------------------------------------------------------------
void ofxColorManager::Changed_color_clicked(ofFloatColor &color)
{
    //DISABLED
    ofLogNotice("ofxColorManager") << "Changed_color_clicked " << ofToString(color);
//    color_picked.set(color);
}

//----


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
    // TODO should split all hide or remember states...

    if (!SHOW_ALL_GUI)
    {
        SHOW_BrowserColors = SHOW_ALL_GUI;
        SHOW_Gradient = SHOW_ALL_GUI;
        SHOW_Curve = SHOW_ALL_GUI;
        SHOW_AlgoPalettes = SHOW_ALL_GUI;
        SHOW_TEST_Curve = SHOW_ALL_GUI;
        SHOW_ColourLovers = SHOW_ALL_GUI;
    }

    this->guiVisible = SHOW_ALL_GUI;
//    curve_pos_slider.setVisible(b);
//    curveMod_Slider.setVisible(b);
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

//----


//--------------------------------------------------------------
void ofxColorManager::draw_ColourLovers(){

    // preview receivers

    if (SHOW_ColourLovers)
    {
        int x, y, w, h, pad, lineH;
        x = 320;
        y = ofGetHeight() - 200;
        w = h = 40;
        pad = 3;
        lineH = 20;

        ofPushStyle();
        ofFill();

        ofDrawBitmapStringHighlight("RECEIVERS IN ofApp", x, y, ofColor::black, ofColor::white);
        y += 3*pad+lineH;

        ofDrawBitmapStringHighlight("myColor:", x, y, ofColor::black, ofColor::white);
        y += 3*pad;

        ofSetColor(myColor);
        ofDrawRectangle(ofRectangle(x, y, w, h));
        y += (h+pad);

        y += (lineH);
        ofDrawBitmapStringHighlight("myPalette:", x, y, ofColor::black, ofColor::white);
        y += 3*pad;

        for (int i = 0; i < myPalette.size(); i++) {
            ofSetColor(myPalette[i]);
            ofDrawRectangle(ofRectangle(x + i * (w + pad), y, w, h));
        }
        y += (h+pad);

        y += (lineH);
        ofDrawBitmapStringHighlight("myPalette_Name:", x, y, ofColor::black, ofColor::white);
        y += (lineH);
        ofDrawBitmapStringHighlight(myPalette_Name, x, y, ofColor::black, ofColor::white);

        ofPopStyle();
    }
}


//--------------------------------------------------------------
void ofxColorManager::windowResized(int w, int h)
{
    // COLOUR LOVERS

    // set positions and panel sizes
    glm::vec2 sizeGui(150, 375);
    glm::vec2 sizeGrid(150, ofGetHeight());
    glm::vec2 posGui(ofGetWidth()-(sizeGui.x+sizeGrid.x+4), 0);
    glm::vec2 posGrid(posGui.x+sizeGui.x+2, 0);

    //must be called before setup() to overwrite default settings
    ColourLoversHelper.setGrid(posGrid, sizeGrid);
    ColourLoversHelper.setup(posGui, sizeGui);
    ColourLoversHelper.windowResized(w, h);
}


//--------------------------------------------------------------
void ofxColorManager::exit()
{
    palette_save("myPalette");
    save_group_XML(XML_params, XML_path);

    ColorBrowser.exit();
    ColourLoversHelper.exit();

    removeKeysListeners();
    removeMouseListeners();
    ofRemoveListener(params_control.parameterChangedE(), this, &ofxColorManager::Changed_CONTROL);
    ofRemoveListener(params_color.parameterChangedE(), this, &ofxColorManager::Changed_CONTROL);
    ofRemoveListener(params_palette.parameterChangedE(), this, &ofxColorManager::Changed_CONTROL);
    ofRemoveListener(params_curve.parameterChangedE(), this, &ofxColorManager::Changed_CONTROL);

    ofxSaveCamera(cam, "DEMOCam");

//    bool ofxSaveCamera(const ofNode &node, string savePath);
//    Where you pass the camera or ofNode object that you want to save, and the directory where it is going to be saved
//
//    Loading is quite much the same.


}

//
////--------------------------------------------------------------
//void ofxColorManager::palette_load(string p)
//{
//    ofLogNotice("ofxColorManager") << "palette_load: " << p;
//
//    string path = path_palettes + p + ".json";
//    ofFile file(path);
//    if (file.exists())
//    {
//        jsonin ji(file);
//        ji >> data;
//
//        ofLogNotice("ofxColorManager") << "palette name: " << data.name;
//        palette_clear();
//        ofColor c;
//        for (int i = 0; i< data.palette.size(); i++)
//        {
//            c = data.palette[i];
//            ofLogNotice("ofxColorManager") << "color_picked" << i << ": " << ofToString(c);
//            palette_addColor(c);
//        }
//    }
//    else
//    {
//        ofLogNotice("ofxColorManager") << "FILE '" << path << "' NOT FOUND";
//    }
//}
//
//
////--------------------------------------------------------------
//void ofxColorManager::palette_save(string p)
//{
//    ofLogNotice("ofxColorManager") << "palette_save: " << p;
//
//    string path = path_palettes + p + ".json";
//
//    data.name = "myPalette";
//    ofLogNotice("ofxColorManager") << "palette name: " << data.name;
//
//    data.palette.resize(palette.size());
//    for (int i = 0; i< palette.size(); i++)
//    {
//        data.palette[i] = palette[i];
//        ofLogNotice("ofxColorManager") << "color_picked" << i << " " << ofToString(data.palette[i]);
//    }
//
//    ofFile file(path, ofFile::WriteOnly);
//    jsonout jo(file);
//    jo << data;
//}



//--------------------------------------------------------------
void ofxColorManager::preset_load(string p)
{
    ofLogNotice("ofxColorManager:preset_load") << p;
    string path = preset_path+p+".json";

    ofFile file(path);
    if (file.exists())
    {
        jsonin ji(file);
        ji >> presetData;

        ofLogNotice("ofxColorManager:preset_load") << "presetData.name      : " << presetData.name;
        ofLogNotice("ofxColorManager:preset_load") << "presetData.curveName : " << presetData.curveName;
        ofLogNotice("ofxColorManager:preset_load") << "presetData.background: " << presetData.background;

        curvesTool.load(preset_path + presetData.curveName + ".yml");

        if (color_backGround_SETAUTO)
        {
            color_backGround = presetData.background;
        }

        ofLogNotice("ofxColorManager:preset_load") << "presetData.palette.size(): " << presetData.palette.size();

        palette_clear();

        for (int i=0; i<presetData.palette.size(); i++)
        {
            ofColor c;
            c = presetData.palette[i];
            ofLogNotice("ofxColorManager:preset_load") << "addColor:" << ofToString(c) <<" ["<<i<<"]";
            palette_addColor(c);
        }
        ofLogNotice("ofxColorManager:preset_load") << "DONE! preset_load  : " << p;
        ofLogNotice("ofxColorManager:preset_load") << "palette.size()     :" << palette.size()<<endl;
        ofLogNotice("ofxColorManager:preset_load") << "btns_palette.size():" << btns_palette.size()<<endl;
    }
    else
    {
        ofLogNotice("ofxColorManager:preset_load") << "FILE '" << path << "' NOT FOUND";
    }
}


//--------------------------------------------------------------
void ofxColorManager::preset_save(string p)
{
    ofLogNotice("ofxColorManager:preset_save") << "preset_save: " << p;
    string path = preset_path+p+".json";

    presetData.name = PRESET_name;//TODO:
    presetData.curveName = "curve01";//TODO:
    presetData.background = color_backGround.get();

    ofLogNotice("ofxColorManager:preset_save") << "presetData.name      : " << presetData.name;
    ofLogNotice("ofxColorManager:preset_save") << "presetData.curveName : " << presetData.curveName;
    ofLogNotice("ofxColorManager:preset_save") << "presetData.background: " << presetData.background;

    curvesTool.save(preset_path+presetData.curveName+".yml");

    presetData.palette.resize(palette.size());
    for (int i = 0; i< palette.size(); i++)
    {
        presetData.palette[i] = palette[i];
        ofLogNotice("ofxColorManager:preset_save") << "presetData.palette[" << i << "]: " << ofToString(presetData.palette[i]);
    }

    ofFile file(path, ofFile::WriteOnly);
    jsonout jo(file);
    jo << presetData;

    ofLogNotice("ofxColorManager:preset_save") << "DONE! preset_save: " << p;
}
