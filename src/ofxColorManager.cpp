#include "ofxColorManager.h"

#pragma mark - OF APP
//--------------------------------------------------------------
ofxColorManager::ofxColorManager()
{
	ofAddListener(ofEvents().update, this, &ofxColorManager::update);
	ofAddListener(ofEvents().draw, this, &ofxColorManager::draw);

	//default
	fps = 60.0f;
	dt = 1.f / fps;
}

//--------------------------------------------------------------
void ofxColorManager::setup()
{
	//ofSetLogLevel("ofxColorManager", OF_LOG_NOTICE);

	//-

	////COSINE GRADIENT
	////mCosineGradient.init(100, 25);
	//mCosineGradient.init(ofGetWidth(), ofGetHeight());
	////mGui.setup();
	//
	//mBias.set("bias", glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
	//mAmplitude.set("amplitude", glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
	//mFrequency.set("frequency", glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
	//mPhase.set("phase", glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
	//cosineGradient_params.setName("COSINE GRADIENT");
	//cosineGradient_params.add(mBias);
	//cosineGradient_params.add(mAmplitude);
	//cosineGradient_params.add(mFrequency);
	//cosineGradient_params.add(mPhase);

	//-

	//ofAddListener(ofxMacMouseEvent, this, &ofxColorManager::mouseEvent);

	//--

	// colorQuantizer
#ifdef QUANT_IMA
	colorQuantizer.setup();
	colorQuantizer.setBottomMode(true);
	// ignore y position and put at the window bottom
	colorQuantizer.setPosition(glm::vec2(0, 400));

	// receivers pointers
	colorQuantizer.setPalette_BACK(myPalette);
	colorQuantizer.setPalette_bUpdated_Palette_BACK(bUpdated_Palette_BACK);

	colorQuantizer.setPalette_Name_BACK(myPalette_Name);
	//colorQuantizer.setPalette_Name_BACK(textInput_temp);
	//colorQuantizer.setPalette_Name_BACK(textInput_New);

	//colorQuantizer.setColor_BACK(myColor);
	//colorQuantizer.setPalette_bUpdated_Color_BACK(bUpdated_Color_BACK);
#endif

	//--

	// ColorWheel

	palettes_colorTheory_setup();

	//--

	//color range

	//col1 = ofColor(0);
	//col2 = ofColor(255);
	col1.set("Color 1", ofColor(0), ofColor(0), ofColor(255));
	col2.set("Color 2", ofColor(0), ofColor(0), ofColor(255));

	//guiCol1 = ofFloatColor(0);
	//guiCol2 = ofFloatColor(1);

	guiCol1 = col1.get();
	guiCol2 = col2.get();

	generateRange(guiCol1, guiCol2);

	types = { "RGB ", "HSL ", "HSV ", "HSB ", "LUV ", "LAB ", "HLAB", "LCH ", "CMY ", "CMYK", "YXY ", "XYZ " };//12 types
	bRefreshMorph = false;

	morphAutoUpdate.set("AUTO UPDATE", true);
	color1FromPicker.set("AUTO PICK C1", false);
	color2FromPicker.set("AUTO PICK C2", false);
	bGetPaletteFromrange.set("GET PALETTE", false);
	numCcolorsRange.set("Amount Colors", 11, 4, 20);

	params_rangTypes.setName("rangTypes");
	for (int i = 0; i < types.size(); i++) //12
	{
		rangTypes[i].set(ofToString(types.at(i)), false);
		params_rangTypes.add(rangTypes[i]);
	}
	params_rangTypes.add(numCcolorsRange);

	ofAddListener(params_rangTypes.parameterChangedE(), this, &ofxColorManager::Changed_Range);

	//--

	myPresetManager.setup();

	//--

	// DEMO 1

	myDEMO_palette.setup();
	myDEMO_palette.setPalette(palette);

	//--

	// MOUSE DEBUGGER

	mouseRuler.setup();
	mouseRuler.toggleVisibility();

	//-

	// COLOUR LOVERS

	// set positions and panel sizes
	glm::vec2 sizeGui(150, 375);
	glm::vec2 sizeGrid(150, ofGetHeight());
	glm::vec2 posGui(ofGetWidth() - (sizeGui.x + sizeGrid.x + 4), 0);
	glm::vec2 posGrid(posGui.x + sizeGui.x + 2, 0);

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
	myPalette.resize(2);//pointer setter will clear/resize. nevermind the vector size here
	myPalette[0] = ofColor::white;
	myPalette[0] = ofColor::white;
	myPalette_Name = "NOT LOADED";

	//-

	// LAYOUT

	gui_setup_layout();

	//-

	// COLOR BROWSER

	//ColorBrowser.setBoxSize(7);
	//ColorBrowser.setRowsSize(7 * 17);//for Pantone palette
	ColorBrowser.setup_colorBACK(color_BACK);
	ColorBrowser.setPosition(colorBrowserPos);
	ColorBrowser.setup();
	ColorBrowser_palette = ColorBrowser.getPalette();

	//-

	// GENERAL DATA

	setBackground_ENABLE(true);
	color_backGround.set("BACKGROUND", ofColor(8));
	color_backGround_SET.set("SET FROM COLOR", false);
	backgroundDarkness.set("DARKNESS", 0.5, 0., 1.);
	color_backGround_Darker.set("DARKER", false);
	color_backGround_SETAUTO.set("AUTO SET", true);
	params_data.setName("GENERAL");
	params_data.add(color_backGround);
	params_data.add(color_backGround_SET);
	params_data.add(color_backGround_Darker);
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

	ofAddListener(params_color.parameterChangedE(), this, &ofxColorManager::Changed_Controls);

	//-

	// ALGORITHMIC PALETTE

	MODE_Palette.set("LOCK FROM COLOR", false);
	SATURATION.set("SATURATION", 200, 0, 255);
	BRIGHTNESS.set("BRIGHTNESS", 200, 0, 255);
	//bRandomPalette.set("RANDOM PALETTE", false);
	bAuto_palette_recall.set("AUTO LIVE MODE", false);
	bLock_palette.set("LOCK PALETTES", false);
	NUM_COLORS_ALGO_PALETTES.set("SIZE COLORS", 6, 2, 10);
	params_palette.setName("ALGORITHMIC PALETTES");
	params_palette.add(MODE_Palette);
	params_palette.add(SATURATION);
	params_palette.add(BRIGHTNESS);
	//params_palette.add(bRandomPalette);
	params_palette.add(NUM_COLORS_ALGO_PALETTES);
	params_palette.add(bAuto_palette_recall);
	params_palette.add(bLock_palette);

	ofAddListener(params_palette.parameterChangedE(), this, &ofxColorManager::Changed_Controls);

	//TODO: add ColorWheelSchemes alternative

	//-

	// INTERFACE

	interface_setup();

	//-

	// ALGORITHMIC PALETTES

	//random.generateRandom(NUM_COLORS_ALGO_PALETTES);
	palettes_update();
	palettes_setup();
	palettes_setup_labels();

	//-

	// GRADIENT

	gradient.reset();
	//gradient.setupAsTurbo(10); //with 10 samples
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

	//SHOW_PaletteCustom.set("SHOW PALETTE LIB", false);
	bRandomColor.set("RANDOM COLOR", false);
	bAddColor.set("ADD COLOR", false);
	bPaletteEdit.set("EDIT COLOR", false);
	bRemoveColor.set("REMOVE COLOR", false);
	bClearPalette.set("CLEAR PALETTE", false);
	params_control.setName("COLOR EDITOR");
	//params_control.add(color_picked);
	params_control.add(bRandomColor);
	params_control.add(bPaletteEdit);
	params_control.add(bAddColor);
	params_control.add(bRemoveColor);
	params_control.add(bClearPalette);
	//params_control.add(SHOW_PaletteCustom);
	//params_control.add(color_backGround);

	//-

	// CONTROL WINDOWS

	SHOW_ColourLovers.set("SHOW COLOUR LOVERS", true);
	SHOW_ColourLovers_searcher.set("SHOW COLOUR LOVERS SEARCH", true);
	SHOW_AlgoPalettes.set("SHOW PALETTES", true);
	SHOW_BrowserColors.set("SHOW BROWSER COLORS", true);
	SHOW_Gradient.set("SHOW GRADIENT", true);
	//SHOW_CosineGradient.set("SHOW COSINE GRADIENT", true);
	SHOW_Curve.set("SHOW CURVE TOOL", true);

	SHOW_PresetManager.set("SHOW PRESET MANAGER", true);
	SHOW_ColorManager.set("SHOW COLOR MANAGER", true);
	SHOW_ColorPicker.set("SHOW COLOR PICKER", true);
	SHOW_ColorRange.set("SHOW COLOR RANGE", true);
	SHOW_PanelsManager.set("SHOW PANELS MANAGER", true);

	params_control.add(SHOW_ColourLovers_searcher);
	params_control.add(SHOW_ColourLovers);
	params_control.add(SHOW_AlgoPalettes);
	params_control.add(SHOW_BrowserColors);
	params_control.add(SHOW_Gradient);
	params_control.add(SHOW_Curve);

	params_control.add(SHOW_PresetManager);
	params_control.add(SHOW_ColorManager);
	params_control.add(SHOW_ColorPicker);
	params_control.add(SHOW_ColorRange);
	params_control.add(SHOW_PanelsManager);
	params_control.add(SHOW_UserPalette);
	params_control.add(SHOW_ColorQuantizer);
	//params_control.add(SHOW_CosineGradient);

	//-

	SHOW_ALL_GUI = true;
	SHOW_debugText = false;
	SHOW_GUI_MINI = false;

	//-

	// BACKGROUND

	params_control.add(color_backGround_SET);
	params_control.add(color_backGround_SETAUTO);
	params_control.add(color_backGround_Darker);
	params_control.add(backgroundDarkness);

	//-

	ofAddListener(params_control.parameterChangedE(), this, &ofxColorManager::Changed_Controls);

	//--

	// GUI

	//-

	// font

	ImGuiIO &io = ImGui::GetIO();
	string inputPath;

	//macOS
	//inputPath = ofFilePath::getAbsolutePath("assets/fonts/PragmataProR_0822.ttf");
	//Windows
	//inputPath = "assets/fonts/PragmataProR_0822.ttf";

	const char *myPath = inputPath.c_str();
	ImFontConfig config;
	//config.OversampleH = 3;
	//config.OversampleV = 1;
	//config.GlyphExtraSpacing.x = 1.0f;

	//macOS
	//io.Fonts->AddFontFromFileTTF(myPath, 13.0f, &config);
	//Windows
	//io.Fonts->AddFontFromFileTTF(myPath, 13.0f, &config);

#ifdef INCLUDE_IMGUI_CUSTOM_THEME_AND_FONT
	ofxSurfingHelpers::ImGui_FontCustom();
#endif

	// create
	gui.setup();

	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// theme customize
#ifdef INCLUDE_IMGUI_CUSTOM_THEME_AND_FONT
	ofxSurfingHelpers::ImGui_ThemeMoebiusSurfing();
	//ofxSurfingHelpers::ImGui_ThemeModernDark();
#endif

	//-

	guiVisible = true;

	//// theme colors
	//gui_imGui_Theme();

	//--

	// LISTENERS

	addKeysListeners();
	addMouseListeners();

	color_picked.addListener(this, &ofxColorManager::Changed_ColorPicked);
	color_clicked_param.addListener(this, &ofxColorManager::Changed_ColorClicked);

	//------------------------------------------------------

	// STARTUP SETTINGS

	XML_params.setName(__FUNCTION__);

	XML_params.add(SHOW_PresetManager);
	XML_params.add(SHOW_ColorManager);
	XML_params.add(SHOW_ColorPicker);
	XML_params.add(SHOW_ColorRange);
	XML_params.add(SHOW_PanelsManager);
	XML_params.add(SHOW_UserPalette);
	XML_params.add(SHOW_ColorQuantizer);

	XML_params.add(SHOW_ColourLovers_searcher);
	XML_params.add(SHOW_ColourLovers);
	XML_params.add(SHOW_AlgoPalettes);
	XML_params.add(SHOW_Gradient);
	//XML_params.add(SHOW_CosineGradient);
	XML_params.add(SHOW_Curve);
	XML_params.add(SHOW_TEST_Curve);//curve tool
	XML_params.add(SHOW_BrowserColors);

	XML_params.add(color_picked);
	XML_params.add(color_backGround);
	XML_params.add(backgroundDarkness);
	XML_params.add(color_backGround_Darker);
	XML_params.add(color_backGround_SETAUTO);

	XML_params.add(bPaletteEdit);//user palette
	XML_params.add(MODE_Palette);//algorithmic palette
	XML_params.add(NUM_COLORS_ALGO_PALETTES);
	XML_params.add(BRIGHTNESS);
	XML_params.add(SATURATION);
	XML_params.add(gradient_hard);//gradient
	XML_params.add(bAuto_palette_recall);
	XML_params.add(SHOW_Layout_Gui);
	XML_params.add(paletteLibPage_param);
	//XML_params.add(TEST_MODE);

	loadAppSettings(XML_params, XML_path);

	//------------------------------------------------

	// STARTUP

	//--

	// PRESET MANAGER

	preset_filesRefresh();

	// TODO
	preset_load(PRESET_name);

	//-

	palettes_setVisible(SHOW_AlgoPalettes);

	//--

	// ofxGuiPanelsLayout

	SHOW_ALL_GUI.setName("SHOW_ALL_GUI");
	SHOW_UserPalette.setName("SHOW USER PALETTE");
	SHOW_GUI_MINI.setName("SHOW_GUI_MINI");
	SHOW_debugText.setName("SHOW debug");
	SHOW_TEST_Curve.setName("SHOW TEST CURVE");
	SHOW_ColorQuantizer.setName("SHOW COLOR PICTURE");

	// ofxGuiPanelsLayout

	// 3. toggles

	p_TOGGLES.setName("TOGGLES PREVIEW");
	p_TOGGLES.add(SHOW_PanelsManager);
	p_TOGGLES.add(SHOW_ColourLovers);
	p_TOGGLES.add(SHOW_ColourLovers_searcher);
	p_TOGGLES.add(SHOW_AlgoPalettes);
	p_TOGGLES.add(SHOW_BrowserColors);
	p_TOGGLES.add(SHOW_Gradient);
	p_TOGGLES.add(SHOW_Curve);
	p_TOGGLES.add(SHOW_TEST_Curve);
	p_TOGGLES.add(SHOW_ImGui);
	p_TOGGLES.add(SHOW_ColorManager);
	p_TOGGLES.add(SHOW_ColorPicker);
	p_TOGGLES.add(SHOW_UserPalette);
	p_TOGGLES.add(SHOW_ColorQuantizer);
	p_TOGGLES.add(SHOW_ColorRange);
	p_TOGGLES.add(SHOW_PresetManager);
	//p_TOGGLES.add(SHOW_CosineGradient);
	p_TOGGLES.add(SHOW_ALL_GUI);
	p_TOGGLES.add(SHOW_GUI_MINI);
	p_TOGGLES.add(TEST_DEMO);
	p_TOGGLES.add(SHOW_debugText);

	//-

	// add panels to manager
#ifdef INCL_LAYOUT
	panels.addToggle(&SHOW_ColourLovers);
	panels.addToggle(&SHOW_AlgoPalettes);
	panels.addToggle(&SHOW_BrowserColors);
	panels.addToggle(&SHOW_Gradient);
	panels.addToggle(&SHOW_Curve);
	panels.addToggle(&SHOW_ALL_GUI);
	panels.addToggle(&SHOW_GUI_MINI);
	panels.addToggle(&SHOW_debugText);
	panels.addToggle(&SHOW_TEST_Curve);
	panels.addToggle(&SHOW_ImGui);
	panels.addToggle(&SHOW_PresetManager);
	panels.addToggle(&SHOW_ColorManager);
	panels.addToggle(&SHOW_ColorPicker);
	panels.addToggle(&SHOW_ColorRange);
	panels.addToggle(&SHOW_PanelsManager);
	panels.addToggle(&SHOW_ColourLovers_searcher);
	panels.addToggle(&SHOW_UserPalette);
	panels.addToggle(&SHOW_ColorQuantizer);
	//panels.addToggle(&SHOW_CosineGradient);
	panels.addToggle(&TEST_DEMO);

	//call after add the panels
	panels.setup();

	panels.group_Selected = 1;
#endif

	//--
}

//--------------------------------------------------------------
void ofxColorManager::update(ofEventArgs & args)
{

	////COSINE GRADIENT
	//cosineGradient_update();

	//-

	// WINDOW TITLE
#ifdef INCL_LAYOUT
	string str;
	str += ("[PAGE " + ofToString(panels.group_Selected) + "] ");
	//str += ofToString((int)ofGetFrameRate()) + "FPS";
	ofSetWindowTitle(str);
#endif

	//-

	// PRESET MANAGER

	myPresetManager.update();

	//--

	// ofxGuiPanelsLayout
	//#define QUANT_IMA
#ifdef INCL_LAYOUT
	panels.update();
#endif

	//--

	// DEMO

	if (TEST_DEMO)
		myDEMO_palette.update();

	//---

	// TEST CURVE

	if (TEST_MODE)
	{
		int frameBuffer = (int)ofMap(TEST_Speed, 0., 1., TEST_maxFrames, 30);
		int frameCurrent = ofGetFrameNum() % frameBuffer;//0 to maxFrames

		if (!bTEST_pause)
		{
			framePrc = ofMap(frameCurrent, 0, frameBuffer, 0., 1.);
			float control;
			if (!TEST_CycleMODE)
				control = ofClamp(framePrc, 0., 1.);
			else
			{
				float mySin = std::sin(PI * framePrc);
				control = ofClamp(mySin, 0., 1.);
			}
			if (TEST_toBackground)
				color_backGround.set(getColorAtPercent(control));
			setControl(control);
		}

		if (frameCurrent == frameBuffer - 1)
		{
			bTEST_pause = !bTEST_pause;

			//round end position to clamp
			float control;
			if (!TEST_CycleMODE)
			{
				control = 1.;
				framePrc = 1.;
			}
			else
			{
				control = 0.;
				framePrc = 1.;
			}
			if (TEST_toBackground)
				color_backGround.set(getColorAtPercent(control));
			setControl(control);
		}
	}

	//--

	// COLOUR LOVERS

	ColourLoversHelper.update();

	// 1. colour lover palette has been clicked/changed selected

	if (bUpdated_Palette_BACK)
	{
		bUpdated_Palette_BACK = false;
		ofLogWarning(__FUNCTION__) << "bUpdated_Palette_BACK: " << bUpdated_Palette_BACK;

		// TODO: WORKFLOW: if mode is palette&color should load first palette color
		// and forget the clicked color

		// 1. get palette colors from colour lovers
		palette_load_ColourLovers();

		//-

		// WORKFLOW: when loading a color lover palette we disable auto create from algo palettes
		if (bAuto_palette_recall)
		{
			bAuto_palette_recall = false;
		}

		//-

		// PRESET MANAGER
		if (!MODE_newPreset)
			MODE_newPreset = true;
		textInput_New = myPalette_Name;

		//-

		// DEMO
		if (TEST_DEMO)
			myDEMO_palette.reStart();

		//-

		//WORKFLOW
		if (bAutoExportPreset)
		{
			ofLogNotice(__FUNCTION__) << "bAutoExportPreset: " << bAutoExportPreset;
			//cout << "EXPORT" << endl;
			saveColors();
		}
	}

	//-

	// 2. color pick from palette clicked

	if (bUpdated_Color_BACK)
	{
		bUpdated_Color_BACK = false;
		ofLogWarning(__FUNCTION__) << "bUpdated_Color_BACK: " << bUpdated_Color_BACK;

		// WORKFLOW: TODO: disable to avoid overwrite the selected color into the palette just created
		if (ColourLoversHelper.MODE_PickPalette_BACK &&
			ColourLoversHelper.MODE_PickColor_BACK)
		{
			// WORKFLOW: auto disable edit mode
			if (bPaletteEdit)
			{
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

		if (!MODE_newPreset)
			MODE_newPreset = true;

		// DEMO
		if (TEST_DEMO)
			myDEMO_palette.reStart();
	}

	//--

	// 3. CHANGED ALGORIHTMIC PALETTES

	// handle last selected algorithmic palette

	if (SELECTED_palette != SELECTED_palette_PRE)
	{
		ofLogNotice(__FUNCTION__) << "CHANGED SELECTED_palette: " << SELECTED_palette;

		// WORKFLOW: when a label palette is clicked, will always trig
		// and load the palette into the user palette
		// TODO: BUG should add this to avoid auto load to user palette
		palette_recallFromPalettes(SELECTED_palette);

		SELECTED_palette_LAST = SELECTED_palette;//memorize what was the last trigged palette
		SELECTED_palette_PRE = SELECTED_palette = -1;//disable to stop check..
		//bug if not if pressed same button

		//-

		// 1. hide all borders
		// from OFX-COLOR-PALETTE & OFX-COLOUR-THEORY
		for (int p = 0; p < btns_plt_Selector.size() && p < NUM_TOTAL_PALETTES; p++)
		{
			btns_plt_Selector[p]->setBorder(false);
		}

		//-

		// 2. set to draw border to the selected palette
		if (btns_plt_Selector.size() > 0)
		{
			btns_plt_Selector[SELECTED_palette_LAST]->setBorder(true);
		}

		//-

		// PRESET MANAGER
		if (!MODE_newPreset)
			MODE_newPreset = true;
		textInput_New = btns_plt_Selector[SELECTED_palette_LAST]->getName();

		// DEMO
		if (TEST_DEMO)
			myDEMO_palette.reStart();
	}

	//-

	//TODO
	//not used and hidden
	// 3. color clicked
	if (color_clicked != color_clicked_PRE)
	{
		color_clicked_PRE = color_clicked;
		ofLogNotice(__FUNCTION__) << "CHANGED color_clicked";
		color_picked.set(color_clicked);
	}

	//-

	// 4. set the local scope color pointer that is into ofxColorBrowser that whill be used as color picked too

	if (color_BACK != color_BACK_PRE)
	{
		color_BACK_PRE = color_BACK;
		ofLogNotice(__FUNCTION__) << "CHANGED color_BACK pointer";
		color_picked.set(color_BACK);

		if (!bPaletteEdit)
		{
			if (bAuto_palette_recall)
			{
				palettes_update();
				palette_recallFromPalettes(SELECTED_palette_LAST);//trig last choice
			}
		}
	}

	//-

	interface_update();

	//TODO:
	//Windows
	curveTool_update();
	ColorBrowser.update();
}

//--------------------------------------------------------------
void ofxColorManager::draw(ofEventArgs & args)
{
	// BACKGROUND

	if (backgroundENABLE)
		ofClear(ofColor(color_backGround.get()));

	//--

	////COSINE GRADIENT
	//if (SHOW_CosineGradient)
	//{
	//    mCosineGradient.render(glm::vec2(0), ofGetWidth(), ofGetHeight());
	//}

	//--

	////TODO: BUG: startup..
	////if (ENABLE_keys)
	//if (this->mouseOverGui)
	//{
	//    ofClear(ofColor::red);
	//}
	//else
	//{
	//    ofClear(ofColor::blue);
	//}

	//--

	// PRESET MANAGER

	if (SHOW_PresetManager)
	{
		//myPresetPalette.draw();
		myPresetManager.draw();
	}

	//--

	// DEMO

	if (TEST_DEMO)
		myDEMO_palette.draw();

	//--

	//if (SHOW_ALL_GUI)
	//{
	//    ofPushMatrix();
	//    ofPushStyle();

	//-

	// COLOR BOX PICKER (CURRENT)

	//        ofPushStyle();
	//        ofFill();
	//        ofSetColor(ofColor( color_picked.get() ));
	//        ofDrawRectangle(r_color_picked);
	//        ofPopStyle();

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

	//-

	// CURVE

	////COSINE GRADIENT
	//if (SHOW_Gradient)
	//{
	//    gradient_draw();
	//}

	//-

	// TEST CURVE

	// COLOR MONITORING SLIDER POSITION ON CURVED GRADIENT

	if (SHOW_TEST_Curve)
	{
		//gradiented color
		ofRectangle r(currColor_x, currColor_y, box_user_size / 2.0, slider_h);

		// 2. current box color at input curve point (right positioned)
		float out = ofMap(curvesTool.getAtPercent(1.0 - curve_pos), 0, curveTool_amount - 1, 1., 0.);
		ofColor c = gradient.getColorAtPercent(out);

		// 3. rectangle progress bar
		ofRectangle recBar(0, ofGetHeight() - 15, framePrc * ofGetWidth(), 5);

		ofPushStyle();
		ofFill();
		ofSetColor(c);
		ofDrawRectangle(r);
		ofSetColor(ofColor(ofColor::black, 200));
		//ofSetColor(ofColor::black);
		ofDrawRectangle(recBar);
		ofPopStyle();
	}

	//-

	// CURVE
	if (SHOW_Curve)
	{
		curveTool_draw();
	}

	// COLORS BROWSER
	if (SHOW_BrowserColors)
	{
		ColorBrowser.draw();
	}

	// COLOUR LOVERS
	// debug
	//        if (SHOW_ColourLovers) {
	//            colourLovers_drawPreview();
	//        }

	//ofPopMatrix();
	//ofPopStyle();
	//}

	//--

	// GUI

	if (SHOW_ImGui)
	{
		bool ENABLE_keys_PRE = ENABLE_keys;

		if (mouseOverGui)
		{
			ENABLE_keys = false;
		}
		else
		{
			ENABLE_keys = true;
		}

		mouseOverGui = false;
		if (guiVisible)
		{
			mouseOverGui = gui_imGui_Draw();

			////TODO
			//gui_imGui_Draw();
			//mouseOverGui = ofxImGui::IsMouseOverGui();
		}
		//disable things to avoid move by mouse when editing gui

		//// TODO: BUG: startup disabled keys..
		//if (ofGetFrameNum() == 60)
		//{
		//    ENABLE_keys = true;
		//}
		//else
		//{
		//    if (this->mouseOverGui)
		//    {
		//        ENABLE_keys = false;
		//    }
		//    else
		//    {
		//        ENABLE_keys = true;
		//    }
		//}

		////TODO: disables keys when using colour lovers
		//if (SHOW_ColourLovers || SHOW_ColourLovers_searcher)
		//{
		//    ENABLE_keys = false;
		//}

		//TODO: BUG: solve startup bug that disables keys
		if (ENABLE_keys != ENABLE_keys_PRE)
		{
#ifdef QUANT_IMA
			if (SHOW_ColorQuantizer && !ENABLE_keys)
			{
				colorQuantizer.setActive(false);
			}
			else if (SHOW_ColorQuantizer && ENABLE_keys)
			{
				colorQuantizer.setActive(true);
			}
#endif

			//TODO
			//if (SHOW_ColourLovers || SHOW_ColourLovers_searcher)
			//    ColourLoversHelper.setKeysEnabled(false);
		}

		//--
	}

	//--

	if (SHOW_GUI_MINI)
	{
		palette_drawMINI();
	}

	//--

	// ofxGuiPanelsLayout
	if (SHOW_Layout_Gui)
	{
#ifdef INCL_LAYOUT
		panels.draw();
#endif
	}

	//--
#ifdef QUANT_IMA
	if (SHOW_ColorQuantizer)
		colorQuantizer.draw();
#endif

	//--

	////TODO:
	//string strKeys = "COLOUR LOVERS KEYS: ";
	//strKeys += (ColourLoversHelper.ENABLER_Keys ? "ENTER/UNFOCUS":"FOCUS/LOAD");
	//string strKeys2 = "MOUSE OVER GUI: ";
	//strKeys2 += (ENABLE_keys ? "FALSE":"TRUE");
	//ofDrawBitmapStringHighlight( strKeys, glm::vec2(500,ofGetHeight()-40) );
	//ofDrawBitmapStringHighlight( strKeys2, glm::vec2(500,ofGetHeight()-20) );

}


//--------------------------------------------------------------
ofxColorManager::~ofxColorManager()
{
	exit();
}

//--------------------------------------------------------------
void ofxColorManager::exit()
{
	//palette_save("myPalette");

	saveAppSettings(XML_params, XML_path);

	ColorBrowser.exit();
	ColourLoversHelper.exit();
#ifdef QUANT_IMA
	colorQuantizer.exit();
#endif

	removeKeysListeners();
	removeMouseListeners();

	ofRemoveListener(params_control.parameterChangedE(), this, &ofxColorManager::Changed_Controls);
	ofRemoveListener(params_color.parameterChangedE(), this, &ofxColorManager::Changed_Controls);
	ofRemoveListener(params_palette.parameterChangedE(), this, &ofxColorManager::Changed_Controls);
	ofRemoveListener(params_curve.parameterChangedE(), this, &ofxColorManager::Changed_Controls);

	ofRemoveListener(params_rangTypes.parameterChangedE(), this, &ofxColorManager::Changed_Range);

	//--

	// ofxGuiPanelsLayout
#ifdef INCL_LAYOUT
	panels.exit();
#endif

	//--

	ofRemoveListener(ofEvents().update, this, &ofxColorManager::update);
	ofRemoveListener(ofEvents().draw, this, &ofxColorManager::draw);
}


#pragma mark - INTERFACE

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
	// TODO:: add layout json saver like ofxGuiPanels to live editing!

	// global mini pad between panels/objects
	pad = 0;//between box colors in palettes
	int padPanels = 2;//between panels mainly in curve tool sections

	guiWidth = 200;
	//    box_size = 40;
	box_size = 30;
	box_user_size = 50;//user palette colors

	// gui 1 COLOR PANEL
	gui_x = 5;
	gui_y = 5;
	gui_w = guiWidth;
	gui_h = 200;

	// gui 2 COLOR MANAGER
	gui2_x = 225;
	gui2_y = 5;
	gui2_w = guiWidth;
	gui2_h = 100;

	// gui 3 PRESET MANAGER
	gui3_x = 700;
	gui3_y = 5;
	gui3_w = guiWidth;
	gui3_h = 200;

	// gui 4 PANELS MANAGER
	gui4_x = gui2_x;
	gui4_y = 510;
	gui4_w = guiWidth;
	gui4_h = 200;

	//-

	// curve tool pos (anchor for others)
	curveTool_x = 650;//distance required to not be over the colorpicker
	curveTool_y = 15;
	curveTool_w = curveTool_amount;//TODO: should can resize curve tool editor box besides amount
	curveTool_h = curveTool_amount;

	// gradient-pre curve (bad sorted to the left but anchored to curve..)
	grad_w = box_user_size;
	grad_x = curveTool_x - (grad_w + padPanels);
	grad_y = curveTool_y;
	grad_h = curveTool_h;

	// curve mod slider
	sliderMod_x = curveTool_x + curveTool_w + padPanels;
	sliderMod_y = curveTool_y;
	sliderMod_w = box_user_size / 2;
	sliderMod_h = curveTool_h;

	// gradient-curved image LUT [referenced to curve pos (vertical)]
	image_curvedGradient_x = curveTool_w + pad + sliderMod_w + padPanels;//curveTool +
	image_curvedGradient_y = 0;//curveTool +
	image_curvedGradient_w = box_user_size;
	image_curvedGradient_h = curveTool_h;

	// testing curve widgets
	// curve out slider
	slider_x = sliderMod_x + padPanels + image_curvedGradient_w + box_user_size / 2;
	slider_y = curveTool_y;
	slider_w = box_user_size / 2;
	slider_h = curveTool_h;

	// current color box/bar (the one affected by slider position. just for testing gradient purpose)
	currColor_x = slider_x + 2 * (slider_w + padPanels);
	currColor_y = curveTool_y;

	//-

	// user palette (pos related to gradient-pre curve)
	palette_x = grad_x - (grad_w + padPanels);
	palette_y = curveTool_y;

	// user color box monitor picked (same that color picker gui)
	// bar mode
	color_w = (2 * box_user_size);
	color_x = palette_x - (color_w + padPanels + box_user_size / 2 + padPanels);
	color_y = curveTool_y;
	color_h = curveTool_h;
	r_color_picked = ofRectangle(color_x, color_y, color_w, color_h);

	//-

	// algorithmic palettes
	palettes_x = 996;
	palettes_y = 10;

	// color clicked box on palettes(hidden)
	colorPick_x = 390;
	colorPick_y = color_h + 30;
	colorPick_w = colorPick_h = 2 * box_user_size;
	r_color_clicked = ofRectangle(colorPick_x, colorPick_y, colorPick_w, colorPick_h);

	// browser colors palettes
	colorBrowserPos = glm::vec2(300, 335);
}

//--------------------------------------------------------------
void ofxColorManager::palette_rearrenge()
{
	ofLogVerbose(__FUNCTION__) << "palette_rearrenge size: " << btns_palette.size();

	// re-arrenge all resized boxes from interface to fill all bar
	// used when changed palette size

	std::string name;
	int x, y, w, hTotal;
	float h;//of each button
	x = palette_x;
	w = box_user_size;
	int numBtns = btns_palette.size();
	hTotal = curveTool_h;
	h = hTotal / (float)numBtns;

	//TODO: improve boxes borders and padding...
	//int pad = 1;
	//h = (hTotal / (float) numBtns) - pad;

	bool flipBtn = true;//flip color sorting
	for (int i = 0; i < numBtns; i++)
	{
		name = "btn" + ofToString(i);
		auto a = scene->getChildWithName(name, 1000);

		//TODO: could get num childres instead of using 1000
		//auto a = scene->getChildWithName(name, scene->getNumChildren());

		if (flipBtn)
			y = palette_y + (numBtns - i - 1) * h;
		else
			y = palette_y + i * h;

		a->setPosition(x, y);
		a->setSize(w, h);
	}
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
	btn->setup_colorBACK(color_clicked);
	btn->setSelectable(false);//to enable border draw
	btn->setLocked(true);//avoid dragging
	btn->setName("btn" + ofToString(i));// "btn" is the label for user palette color boxes

	//temp positions that we re positioned
	btn->setPosition(palette_x, palette_y + box_user_size + pad + i * (box_user_size + pad));
	btn->setSize(box_user_size, box_user_size);

	if (SHOW_UserPalette)
	{
		btn->setVisible(true);
	}
	else
	{
		btn->setVisible(false);
	}

	scene->addChild(btn);
	btns_palette.push_back(btn);

	//ofLogNotice(__FUNCTION__) << "palette_addColor_toInterface:" << "added btn";
	ofLogVerbose(__FUNCTION__) << "palette_addColor_toInterface: size: [" << btns_palette.size() << "]";

	//-

	// make positions & resizes to fill bar
	palette_rearrenge();
}


//--------------------------------------------------------------
void ofxColorManager::interface_update()
{
	TouchManager::one().update();

	// 1. EACH COLOR OF CURRENT USER PALETTE
	for (int i = 0; i < btns_palette.size(); i++)
	{
		btns_palette[i]->update();
	}

	// 2. ALGORITHMIC PALETTES SELECTORS
	for (int i = 0; i < btns_plt_Selector.size(); i++)
	{
		btns_plt_Selector[i]->update(dt);
	}

	//-

	// 3. ALGORITHMIC PALETTES COLORS

	// 1. triad
	for (int i = 0; i < btns_plt_Triad.size(); i++)
	{
		btns_plt_Triad[i]->update();
	}
	// 2. complement triad
	for (int i = 0; i < btns_plt_ComplTriad.size(); i++)
	{
		btns_plt_ComplTriad[i]->update();
	}
	// 3. complement sat
	for (int i = 0; i < btns_plt_CompSat.size(); i++)
	{
		btns_plt_CompSat[i]->update();
	}
	// 4. complement brgt
	for (int i = 0; i < btns_plt_ComplBrgt.size(); i++)
	{
		btns_plt_ComplBrgt[i]->update();
	}
	// 5. mono sat
	for (int i = 0; i < btns_plt_MonoSat.size(); i++)
	{
		btns_plt_MonoSat[i]->update();
	}
	// 6. mono brgt
	for (int i = 0; i < btns_plt_MonoBrgt.size(); i++)
	{
		btns_plt_MonoBrgt[i]->update();
	}
	// 7. analogue
	for (int i = 0; i < btns_plt_Analog.size(); i++)
	{
		btns_plt_Analog[i]->update();
	}
	//    // 8. random
	//    for (int i = 0; i < btns_plt_Random.size(); i++) {
	//        btns_plt_Random[i]->update();
	//    }
}


//--------------------------------------------------------------
void ofxColorManager::interface_draw()
{
	scene->render();

	if (bShowDebug)
	{
		scene->renderDebug();
	}
}


#pragma mark - IM GUI

////--------------------------------------------------------------
//void ofxColorManager::gui_imGui_CosineGradient()
//{
//    mainSettings.windowPos = ofVec2f(gui4_x + 200, gui4_y);
//    mainSettings.windowSize = ofVec2f(gui4_w, gui4_h);
//
//    if (ofxImGui::BeginWindow("COSINE GRADIENT", mainSettings, false))
//    {
//        if (ofxImGui::BeginTree(this->cosineGradient_params, mainSettings))
//        {
//            ofxImGui::AddParameter(this->mBias);
//            ofxImGui::AddParameter(this->mAmplitude);
//            ofxImGui::AddParameter(this->mFrequency);
//            ofxImGui::AddParameter(this->mPhase);
//
//            ofxImGui::EndTree(mainSettings);
//        }
//    }
//    ofxImGui::EndWindow(mainSettings);
//}


//--------------------------------------------------------------
void ofxColorManager::gui_imGui_ColorPalette()
{
	mainSettings.windowPos = ofVec2f(700, 50);
	mainSettings.windowSize = ofVec2f(200, 400);

	if (ofxImGui::BeginWindow("USER PALETE", mainSettings, false))
	{
		// get clicked color
		static int colBoxSize = 40;

		//-

		//ImGui::Dummy(ImVec2(0.0f, 5));

		const size_t tot = palette.size();

		for (int n = 0; n < tot; n++)
		{
			ImGui::PushID(n);

			// draw border to selected color
			bool bDrawBorder = false;
			if (n == lastColorPicked_Palette)
			{
				bDrawBorder = true;
			}
			if (bDrawBorder)
			{
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1, 1, 1, .40));
				ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
			}

			//-			

			ImGui::SameLine();
			//ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);//vertical inter line

			if (ImGui::ColorButton("##palette", palette[n],
				ImGuiColorEditFlags_NoAlpha |
				ImGuiColorEditFlags_NoPicker |
				ImGuiColorEditFlags_NoTooltip,
				ImVec2(colBoxSize, colBoxSize)))
			{
				lastColorPicked_Palette = n;

				//color = ImVec4(palette[n].x, palette[n].y, palette[n].z, palette.w); // Preserve alpha!
				//color_picked = color;
			}

			//draw border to selected color
			if (bDrawBorder)
			{
				ImGui::PopStyleColor();
				ImGui::PopStyleVar(1);
			}

			//ImGui::PopItemWidth();
			ImGui::PopID();
		}
	}
	ofxImGui::EndWindow(mainSettings);
}

//--------------------------------------------------------------
void ofxColorManager::gui_imGui_ColorPicker()
{
	// 1. COLOR PICKER CUSTOM

	//mainSettings = ofxImGui::Settings();
	mainSettings.windowPos = ofVec2f(gui_x, gui_y);
	mainSettings.windowSize = ofVec2f(guiWidth, ofGetWindowHeight() - gui_y);

	//    static bool no_titlebar = false;
	//    static bool no_scrollbar = false;
	//    static bool no_menu = false;
	//    static bool no_move = false;
	//    static bool no_resize = false;
	//    static bool no_collapse = false;
	//    static bool no_close = false;
	//    static bool no_nav = false;
	//    ImGuiWindowFlags window_flags = 0;
	////    if (no_titlebar)  window_flags |= ImGuiWindowFlags_NoTitleBar;
	////    if (no_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
	////    if (!no_menu)     window_flags |= ImGuiWindowFlags_MenuBar;
	////    if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;
	////    if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
	//    if (no_collapse)  window_flags |= ImGuiWindowFlags_NoCollapse;
	////    if (no_nav)       window_flags |= ImGuiWindowFlags_NoNav;
	////    if (no_close)     p_open = NULL; // Don't pass our bool* to Begin
	//    if (ofxImGui::BeginWindow("COLOR PICKER", mainSettings, window_flags))

	if (ofxImGui::BeginWindow("COLOR PICKER", mainSettings, false))
	{
		//--

		// GET COLOR FROM OUTSIDE COLOR PICKED

		// TEST
		LISTEN_isEnabled = false;//maybe required because get() causes callbacks too (?)
		static ImVec4 color;
		color = color_picked.get();
		LISTEN_isEnabled = true;

		//--

		// 0. CUSTOM BUTTON COLOR BIG

		//ImGui::PushItemWidth(guiWidth * widgetFactor);
		int colorW = guiWidth;
		int colorH = 85;
		int misc_flags = ImGuiColorEditFlags_NoOptions |
			ImGuiColorEditFlags_NoTooltip;

		ImGui::ColorButton("MyColor##3", *(ImVec4 *)&color, misc_flags,
			ImVec2(colorW, colorH));
		//ImGui::PopItemWidth();

		//-

		// 1. USER PALETTE

		if (ofxImGui::BeginTree("USER PALETTE", mainSettings))
		{
			ofxImGui::AddParameter(this->bPaletteEdit);
			ofxImGui::AddParameter(this->bAddColor);
			ImGui::SameLine();
			ofxImGui::AddParameter(this->bRemoveColor);
			ofxImGui::AddParameter(this->bClearPalette);

			ofxImGui::EndTree(mainSettings);
		}

		//-

		// 2. COLOR HSB

		//if (ofxImGui::BeginTree("COLOR", mainSettings))
		if (ImGui::CollapsingHeader("COLOR"))
		{
			ImGui::PushItemWidth(guiWidth * 0.90);

			// TODO: SHOULD APPLY HSV HERE, NOT INTO CALLBACK, BECAUSE IT WILL TRIG
			// THE COLOR PICKED UPDATING CALLBACK!!!

			// TEST
			LISTEN_isEnabled = false; //disable callbacks
			if (ofxImGui::AddParameter(this->color_HUE))
			{
				ofLogNotice(__FUNCTION__) << "ImGui: HUE MOVED !" << endl;
				ofColor c;
				c.set(color_picked.get());
				c.setHue(color_HUE);
				color_picked.set(c);
				update_color_picked_CHANGES();
			}
			if (ofxImGui::AddParameter(this->color_SAT))
			{
				ofLogNotice(__FUNCTION__) << "ImGui: SAT MOVED !" << endl;
				ofColor c;
				c.set(color_picked.get());
				c.setSaturation(color_SAT);
				color_picked.set(c);
				update_color_picked_CHANGES();
			}
			if (ofxImGui::AddParameter(this->color_BRG))
			{
				ofLogNotice(__FUNCTION__) << "ImGui: BRG MOVED !" << endl;
				ofColor c;
				c.set(color_picked.get());
				c.setBrightness(color_BRG);
				color_picked.set(c);
				update_color_picked_CHANGES();
			}
			LISTEN_isEnabled = true;

			ofxImGui::AddParameter(this->bRandomColor);

			//-

			ImGui::PopItemWidth();
			//ofxImGui::EndTree(mainSettings);
		}

		//-

		if (ofxImGui::BeginTree("COLOR WHEEL", mainSettings))//grouped folder
		{
			// 0. custom button color big

			//ImGui::PushItemWidth(guiWidth * widgetFactor);
			ImGui::PushItemWidth(guiWidth * 0.80);

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
			if (ImGui::ColorPicker4("MyColor##4", (float *)&color, colorEdiFlags))
			{
				cout << "PICKER 1 MOVED !" << endl;
				color_picked = color;
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
			if (ImGui::ColorPicker4("MyColor##5", (float *)&color, colorEdiFlags))
			{
				cout << "PICKER 2 MOVED !" << endl;
				color_picked = color;
			}

			//-

			//ofxImGui::AddParameter(this->SHOW_PaletteCustom);

			ImGui::PopItemWidth();
			ofxImGui::EndTree(mainSettings);
		}

		//-------------------------------------------------------------------

		// 3. PALETTE LIBRARY

		//if (ofxImGui::BeginTree("PALETTE LIBRARY", mainSettings))
		//{
		if (ImGui::CollapsingHeader("PALETTE LIBRARY"))
		{
			//--

			// GET COLOR FROM OUTSIDE COLOR PICKED

			LISTEN_isEnabled = false;//maybe required because get() causes callbacks too (?)
			static ImVec4 color;
			color = color_picked.get();
			LISTEN_isEnabled = true;

			//--

			// 2 load/create palete from colorBrowser

			static bool saved_palette_inited = false;
			//const int PaletteSIZE = ColorBrowser_palette.size();//error

			//static ImVec4 _palette[130];//same than openColor palettes
			static ImVec4 saved_palette[NUM_COLORS_PANTONE];//same than Pantone palette

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
			//cout << "IM_ARRAYSIZE(_palette): "<<ofToString(IM_ARRAYSIZE(_palette)) << endl;

			//-

			// layout by pages groups
			int startColorInPal = paletteLibPage * numColorsPage;
			int endColorInPal = startColorInPal + numColorsPage;
			//paletteLibPage = paletteLibPage_param.get();

			ImGui::Text("PANTONE COLORS");

			// load tab2 with lastColorPickedNameColor
			char tab2[32];
			strncpy(tab2, lastColorPickedNameColor.c_str(), sizeof(tab2));
			tab2[sizeof(tab2) - 1] = 0;
			ImGui::PushItemWidth(guiWidth * 0.5);
			ImGui::Text("%s", tab2);//color name label

			//-

			// arrow buttons
			float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
			//ImGui::SameLine();
			ImGui::PushButtonRepeat(true);
			// prev
			if (ImGui::ArrowButton("##left", ImGuiDir_Left))
			{
				if (paletteLibPage > 0)
				{
					paletteLibPage--;
				}
			}
			// next
			ImGui::SameLine(0.0f, spacing);
			if (ImGui::ArrowButton("##right", ImGuiDir_Right))
			{
				if (paletteLibPage < maxPages)
				{
					paletteLibPage++;
				}
			}
			ImGui::PopButtonRepeat();
			ImGui::SameLine();

			//-

			ImGui::SameLine();
			ImGui::SliderInt("PAGE", &paletteLibPage, 0, maxPages);//page slider selector
			ImGui::PopItemWidth();

			//-

			// 2.2 draw all palette colors grid

			for (int n = startColorInPal; n < endColorInPal; n++)
			{
				if (n < totalNumColors) //to avoid empty colors at page end...
				{
					// ImGui::PushItemWidth(guiWidth * 0.2);
					ImGui::PushID(n);

					// get clicked color
					static int colBoxSize;
					colBoxSize = 22;

					//ImGui::PushStyleVar(ImGui::ImGuiStyleVar_FrameBorderSize), 1.0f;

					// PANTONE or other must define his better row size
					if ((n % rowSizePal) != 0)
					{
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);//vertical inter line
					}

					// draw border to selected color
					bool bDrawBorder = false;
					if (n == lastColorPicked)
					{
						bDrawBorder = true;
					}
					if (bDrawBorder)
					{
						ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1, 1, 1, .40));
						ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
					}

					if (ImGui::ColorButton("##palette", saved_palette[n],
						ImGuiColorEditFlags_NoAlpha |
						ImGuiColorEditFlags_NoPicker |
						ImGuiColorEditFlags_NoTooltip,
						ImVec2(colBoxSize, colBoxSize)))
					{
						lastColorPicked = n;

						color = ImVec4(saved_palette[n].x, saved_palette[n].y, saved_palette[n].z, color.w); // Preserve alpha!
						color_picked = color;

						//color name
						lastColorPickedNameColor = "'" + ColorBrowser.pantoneNames[n] + "'";
						string str = "ImGui: PALETTE PICKED: [" + ofToString(lastColorPicked) + "] " + lastColorPickedNameColor;

						ofLogNotice(__FUNCTION__) << str << endl;
					}

					//TODO: draw border to selected color
					if (bDrawBorder)
					{
						ImGui::PopStyleColor();
						ImGui::PopStyleVar(1);
					}

					//ImGui::PopItemWidth();
					ImGui::PopID();
				}
			}
			//ofxImGui::EndTree(mainSettings);
		}
	}
	ofxImGui::EndWindow(mainSettings);
}


//--------------------------------------------------------------
void ofxColorManager::gui_imGui_ControlPanels()
{
	// 4. PANELS MANAGER

	//auto mainSettings = ofxImGui::Settings();
	mainSettings.windowPos = ofVec2f(gui4_x, gui4_y);
	mainSettings.windowSize = ofVec2f(gui4_w, gui4_h);

	if (ofxImGui::BeginWindow("PANELS MANAGER", mainSettings, false))
	{
		//        if (ofxImGui::BeginTree("PANELS", panelsSet))
		//        {
		ofxImGui::AddParameter(this->SHOW_PresetManager);
		ofxImGui::AddParameter(this->SHOW_ColorPicker);
		ofxImGui::AddParameter(this->SHOW_ColorRange);
		ofxImGui::AddParameter(this->SHOW_ColorManager);
		ofxImGui::AddParameter(this->SHOW_UserPalette);
		ofxImGui::AddParameter(this->SHOW_Gradient);
		ofxImGui::AddParameter(this->SHOW_Curve);
		ofxImGui::AddParameter(this->SHOW_TEST_Curve);
		ImGui::Separator();
		ofxImGui::AddParameter(this->SHOW_AlgoPalettes);
		ofxImGui::AddParameter(this->SHOW_ColourLovers);
		ofxImGui::AddParameter(this->SHOW_ColourLovers_searcher);
		ofxImGui::AddParameter(this->SHOW_ColorQuantizer);
		ofxImGui::AddParameter(this->SHOW_BrowserColors);
		//ofxImGui::AddParameter(this->SHOW_CosineGradient);
		//            ofxImGui::EndTree(panelsSet);
		//        }
	}
	ofxImGui::EndWindow(mainSettings);
}


//--------------------------------------------------------------
void ofxColorManager::gui_imGui_ColorRange()
{
	mainSettings.windowPos = ofVec2f(500, 50);
	mainSettings.windowSize = ofVec2f(200, 400);

	if (ofxImGui::BeginWindow("COLOR RANGE", mainSettings, false))
	{
		ofFloatColor cpre1 = guiCol1[0];
		ofFloatColor cpre2 = guiCol2[0];

		ImGui::ColorPicker3("Color From", &guiCol1[0]);
		ofxImGui::AddParameter(color1FromPicker);

		ImGui::Dummy(ImVec2(0.0f, 5));

		ImGui::ColorPicker3("Color To", &guiCol2[0]);
		ofxImGui::AddParameter(color2FromPicker);

		ImGui::Dummy(ImVec2(0.0f, 5));

		//-

		col1 = guiCol1;
		col2 = guiCol2;

		//autogenerate
		if (morphAutoUpdate)
		{
			if (cpre1 != guiCol1[0] || (cpre2 != guiCol2[0]))
				generateRange(col1, col2);
		}

		//if (ofxSurfingHelpers::AddBigButton("GENERATE"))
		if (ImGui::Button("GENERATE"))
		{
			generateRange(col1, col2);
		}

		ofxImGui::AddParameter(morphAutoUpdate);
		ofxImGui::AddParameter(numCcolorsRange);
		ofxSurfingHelpers::AddBigButton(bGetPaletteFromrange);

		ImGui::Dummy(ImVec2(0.0f, 5));

		//-

		// 2.2 draw all palette colors grid
		const int ncols = 11;//?
		const int tot = ncols * ncols;

		//const int ncols = numCcolorsRange.get();
		//const int ty = (int)NUM_TYPES_RANGES;
		//const int tot = ncols * ty;

		static ImVec4 color;
		color = color_picked.get();

		static ImVec4 _palette[tot];
		if (bRefreshMorph)
			for (int n = 0; n < IM_ARRAYSIZE(_palette); n++)
			{
				ofFloatColor c = paletteRange[n];
				_palette[n].x = c.r;
				_palette[n].y = c.g;
				_palette[n].z = c.b;
				_palette[n].w = 1.0f;//alpha
			}
		bRefreshMorph = false;

		//-

		for (int n = 0; n < tot; n++)
		{
			//label for each row
			const int _row = n / numCcolorsRange.get();
			//const int _row = n / 11;

			if (n % ncols == 0)
			{
				////label
				//std::string str;
				//str = types.at(n / ncols);
				//ImGui::Text(str.c_str());

				//labeled button
				//std::string str = rangTypes[_row].getName();
				//if (ImGui::Button(str.c_str()))
				//{
				//}
				//ofxImGui::AddParameter(rangTypes[_row]);
				ofxSurfingHelpers::AddSmallButton(rangTypes[_row]);

				//ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);//vertical inter line
				ImGui::SameLine();
			}

			{
				// ImGui::PushItemWidth(guiWidth * 0.2);
				ImGui::PushID(n);

				// get clicked color
				static int colBoxSize;
				colBoxSize = 22;

				//ImGui::PushStyleVar(ImGui::ImGuiStyleVar_FrameBorderSize), 1.0f;

				if ((n % ncols) != 0)
				{
					ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);//vertical inter line
				}

				// draw border to selected color
				bool bDrawBorder = false;
				if (n == lastColorPicked)
				{
					bDrawBorder = true;
				}
				if (bDrawBorder)
				{
					ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1, 1, 1, .40));
					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
				}

				if (ImGui::ColorButton("##palette", _palette[n],
					ImGuiColorEditFlags_NoAlpha |
					ImGuiColorEditFlags_NoPicker |
					ImGuiColorEditFlags_NoTooltip,
					ImVec2(colBoxSize, colBoxSize)))
				{
					lastColorPicked = n;

					color = ImVec4(_palette[n].x, _palette[n].y, _palette[n].z, color.w); // Preserve alpha!
					color_picked = color;

					//color name
					lastColorPickedNameColor = "'" + ColorBrowser.pantoneNames[n] + "'";
					string str = "ImGui: PALETTE PICKED: [" + ofToString(lastColorPicked) + "] " + lastColorPickedNameColor;

					ofLogNotice(__FUNCTION__) << str << endl;
				}

				//TODO: 
				//draw border to selected color
				if (bDrawBorder)
				{
					ImGui::PopStyleColor();
					ImGui::PopStyleVar(1);
				}

				//ImGui::PopItemWidth();
				ImGui::PopID();
			}
		}
	}
	ofxImGui::EndWindow(mainSettings);
}

//--------------------------------------------------------------
void ofxColorManager::gui_imGui_ColorManager()
{
	// 2. COLOR MANAGER

	//    mainSettings = ofxImGui::Settings();

	//auto mainSettings = ofxImGui::Settings();
	mainSettings.windowPos = ofVec2f(gui2_x, gui2_y);
	mainSettings.windowSize = ofVec2f(gui2_w, gui2_h);

	if (ofxImGui::BeginWindow("COLOR MANAGER", mainSettings, false))
	{
		// GENERAL DATA

		if (ofxImGui::BeginTree("BACKGROUND", mainSettings))
		{
			//--TEST. can't do with pointers to ofParameter...
			ImGui::PushItemWidth(120);

			static ImVec4 color;
			color = color_backGround.get();

			// squared box
			ImGuiColorEditFlags colorEdiFlags =
				ImGuiColorEditFlags_NoSmallPreview |
				ImGuiColorEditFlags_NoTooltip |
				ImGuiColorEditFlags_NoLabel |
				ImGuiColorEditFlags_NoSidePreview |
				ImGuiColorEditFlags_NoOptions |
				ImGuiColorEditFlags_NoAlpha |
				ImGuiColorEditFlags_HSV |
				ImGuiColorEditFlags_RGB |
				ImGuiColorEditFlags_PickerHueBar;

			ImGui::ColorPicker4("Background Color", (float *)&color, colorEdiFlags);
			color_backGround = color;

			ImGui::PopItemWidth();

			//-

			ImGui::PushItemWidth(guiWidth * 0.3);
			ofxImGui::AddParameter(this->color_backGround_SET);
			ofxImGui::AddParameter(this->color_backGround_Darker);
			if (color_backGround_Darker)
			{
				ofxImGui::AddParameter(this->backgroundDarkness);
			}
			ofxImGui::AddParameter(this->color_backGround_SETAUTO);

			//TODO: move endTree down
			ImGui::PopItemWidth();
			ofxImGui::EndTree(mainSettings);

			//ofxImGui::EndTree(mainSettings);
		}

		//--

		// CURVE TOOL

		if (ImGui::CollapsingHeader("CURVE TOOL"))
		{
			ImGui::PushItemWidth(guiWidth * 0.5);
			ofxImGui::AddParameter(this->gradient_hard);
			if (ofxImGui::AddParameter(this->curve_pos))
			{
				curve_pos_slider.setPercent(curve_pos.get());
			}
			ofxImGui::AddParameter(this->curve_pos_out);
			ofxImGui::AddParameter(this->bResetCurve);
			// ofxImGui::AddParameter(this->curveMod);

			//-

			ImGui::PopItemWidth();
		}

		//--

		// TEST CURVE

		if (ImGui::CollapsingHeader("TEST CURVE"))
		{
			ImGui::PushItemWidth(guiWidth * 0.5);
			ImGui::Checkbox("ENABLE TEST", &TEST_MODE);
			ImGui::Checkbox("CYCLE", &TEST_CycleMODE);
			ImGui::Checkbox("TO BACKGROUND", &TEST_toBackground);
			ImGui::SliderFloat("SPEED", &TEST_Speed, 0.0f, 1.0f);
			//ImGui::Checkbox("ENABLE DEMO", &TEST_DEMO);
			ofxImGui::AddParameter(this->TEST_DEMO);
			ImGui::PopItemWidth();
		}

		//--

		// ALGORITHMIC PALETTES

		if (ImGui::CollapsingHeader("ALGORITHMIC PALETTES"))
		{
			ImGui::PushItemWidth(guiWidth * 0.5);
			ofxImGui::AddParameter(this->NUM_COLORS_ALGO_PALETTES);
			ofxImGui::AddParameter(this->MODE_Palette);
			if (!MODE_Palette)
			{
				ofxImGui::AddParameter(this->SATURATION);
				ofxImGui::AddParameter(this->BRIGHTNESS);
			}
			//ofxImGui::AddParameter(this->bRandomPalette);
			ofxImGui::AddParameter(this->bLock_palette);
			ofxImGui::AddParameter(this->bAuto_palette_recall);
			ImGui::PopItemWidth();
		}
	}
	ofxImGui::EndWindow(mainSettings);
}


//--------------------------------------------------------------
void ofxColorManager::gui_imGui_PresetManager()
{
	// 3. PRESET MANAGER

	//mainSettings = ofxImGui::Settings();
	mainSettings.windowSize = ofVec2f(gui3_w, gui3_h);
	mainSettings.windowPos = ofVec2f(gui3_x, gui3_y);
	//mainSettings.windowPos = ofVec2f(ofGetWindowWidth() * 0.5 - gui3_w * 0.5, gui3_y);

	if (ofxImGui::BeginWindow("PRESET MANAGER", mainSettings, false))
	{

		//-

		//        // 1. palettes
		//
		//        ImGui::Text("PALETTE");
		//
		//        if (ImGui::Button("SAVE PALETTE")) {
		//            cout << "SAVE PALETTE" << endl;
		//            PRESET_name = textInput_temp;
		//
		//            myPresetPalette.setName(PRESET_name);
		//            myPresetPalette.setBackgroundColor(color_backGround.get());
		//            myPresetPalette.setPalette(palette);
		//
		//            myPresetPalette.palette_save(PRESET_name);
		//        }
		//
		//        ImGui::SameLine();
		//        if (ImGui::Button("LOAD PALETTE"))
		//        {
		//            cout << "LOAD PALETTE" << endl;
		//            PRESET_name = textInput_temp;
		//            cout << "PRESET_name: " << PRESET_name << endl;
		//
		//            myPresetPalette.palette_load(PRESET_name);
		//
		//            //-
		//
		//            color_backGround = myPresetPalette.getBackground();//TODO: temp solution because required pointer..
		//
		////            //TODO
		////            // apply loaded preset to local system
		////            vector<ofColor> palette_TEMP = myPresetPalette.getPalette();
		////            palette_clear();
		////            for (int i = 0; i < palette_TEMP.size(); i++) {
		////                palette_addColor(palette_TEMP[i]);
		////            }
		////
		////            color_backGround = ofColor(myPresetPalette.getBackground());//get directly without pointing
		//
		//            //-
		//
		//            if (TEST_DEMO)
		//                myDEMO_palette.clear();
		//        }

		//-

		ImGui::Text("PRESET NAME:");
		string textInput_temp = PRESET_name;

		// loaded string into char array
		char tab2[32];
		strncpy(tab2, textInput_temp.c_str(), sizeof(tab2));
		tab2[sizeof(tab2) - 1] = 0;

		if (ImGui::InputText("", tab2, IM_ARRAYSIZE(tab2)))
		{
			cout << "InputText:" << ofToString(tab2) << endl;

			textInput_temp = ofToString(tab2);
			cout << "textInput_temp:" << textInput_temp << endl;

			if (MODE_newPreset)
				MODE_newPreset = false;
		}

		//--

		// arrow buttons
		static int counter = currentFile;
		float spacing = ImGui::GetStyle().ItemInnerSpacing.x;

		ImGui::PushButtonRepeat(true);

		// prev
		if (ImGui::ArrowButton("##left", ImGuiDir_Left))
		{
			if (counter > 0)
			{
				counter--;
				currentFile = counter;
				if (currentFile < files.size())
				{
					PRESET_name = fileNames[currentFile];
					ofLogNotice() << "ARROW: PRESET_name: [" + ofToString(currentFile) + "] " << PRESET_name;
					preset_load(PRESET_name);
				}

				if (MODE_newPreset)
					MODE_newPreset = false;
			}
		}

		// next
		ImGui::SameLine(0.0f, spacing);
		if (ImGui::ArrowButton("##right", ImGuiDir_Right))
		{
			if (counter < files.size() - 1)
			{
				counter++;
				currentFile = counter;
				if (currentFile < files.size())
				{
					PRESET_name = fileNames[currentFile];
					ofLogNotice() << "ARROW: PRESET_name: [" + ofToString(currentFile) + "] " << PRESET_name;
					preset_load(PRESET_name);
				}
			}

			if (MODE_newPreset)
				MODE_newPreset = false;
		}

		ImGui::PopButtonRepeat();

		//preview current preset number to total
		int numPalettes = fileNames.size() - 1;
		ImGui::SameLine();
		ImGui::Text("%d/%d", currentFile, numPalettes);

		//-

		// scrollable list
		if (!fileNames.empty())
		{
			int currentFileIndex = currentFile;
			if (ofxImGui::VectorCombo(" ", &currentFileIndex, fileNames))
			{
				ofLog() << "currentFileIndex: " << ofToString(currentFileIndex);
				if (currentFileIndex < fileNames.size())
				{
					currentFile = currentFileIndex;
					PRESET_name = fileNames[currentFile];
					ofLog() << "PRESET_name: " << PRESET_name;
					preset_load(PRESET_name);
				}

				if (MODE_newPreset)
					MODE_newPreset = false;
			}
		}

		//-

		// 2. presets

		ImGui::Text("PRESETS");

		if (ImGui::Button("SAVE"))
		{
			cout << "SAVE" << endl;

			//TODO
			//should re load by same name and get what position on vector
			//is to reload current preset number
			//            textInput_temp = ofToString(tab2);
			//            cout << "textInput_temp:" << textInput_temp << endl;

			PRESET_name = textInput_temp;
			cout << "PRESET_name: " << PRESET_name << endl;

			preset_save(PRESET_name);
			preset_filesRefresh();
		}

		ImGui::SameLine();
		if (ImGui::Button("UPDATE"))
		{
			cout << "UPDATE" << endl;

			PRESET_name = textInput_temp;
			cout << "PRESET_name: " << PRESET_name << endl;

			//delete old file
			files[currentFile].remove();
			// preset_filesRefresh();

			//save new one
			preset_save(PRESET_name);
			preset_filesRefresh();
		}

		ImGui::SameLine();
		if (ImGui::Button("LOAD"))//not required..
		{
			cout << "LOAD" << endl;
			cout << "PRESET_name: " << PRESET_name << endl;
			preset_load(PRESET_name);
		}

		ImGui::SameLine();
		if (ImGui::Button("DELETE"))//current preset
		{
			cout << "DELETE" << endl;

			files[currentFile].remove();
			preset_filesRefresh();

			//string str = fileNames[currentFile];
			//cout << "DELETE:"<<str<<endl;
			//dir.listDir("user_kits/presets");
			//dir.allowExt("jpg");
			//dir.allowExt("png");
			//dir.sort();
		}

		//export user palette colors to live reload from another parallel app!
		ImGui::SameLine();
		if (ImGui::Button("EXPORT"))
		{
			//cout << "EXPORT" << endl;
			saveColors();
		}

		//ImGui::SameLine();
		//if (ImGui::Button("REFRESH"))//current preset
		//{
		//    cout << "REFRESH" << endl;
		//    preset_filesRefresh();
		//}

		//-

		if (MODE_newPreset)
		{
			ImGui::Separator();
			ImGui::Text("NEW PRESET!");

			// loaded string into char array
			char tab[32];
			strncpy(tab, textInput_New.c_str(), sizeof(tab));
			tab[sizeof(tab) - 1] = 0;

			if (ImGui::InputText("", tab, IM_ARRAYSIZE(tab)))
			{
				cout << "InputText [tab]:" << ofToString(tab) << endl;
				textInput_New = ofToString(tab);
				cout << "textInput_New:" << textInput_New << endl;
			}

			// WORKFLOW: when its editing a new preset..

			ImGui::PushID(1);
			int n = 30;
			float a = ofMap(ofGetFrameNum() % n, 0, n, 0.0f, 1.0f);
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.5, 0.0f, 0.5f, a));

			if (ImGui::Button("SAVE NEW"))
			{
				MODE_newPreset = false;
				cout << "textInput_New: " << textInput_New << endl;
				preset_save(textInput_New);
				preset_filesRefresh();
			}

			ImGui::PopStyleColor(1);
			ImGui::PopID();
		}

		//-
	}
	ofxImGui::EndWindow(mainSettings);
}


//--------------------------------------------------------------
bool ofxColorManager::gui_imGui_Draw()
{
	mainSettings = ofxImGui::Settings();

	this->gui.begin();

	//// 0. COSINE GRADIENT
	//
	//if (SHOW_CosineGradient)
	//    gui_imGui_CosineGradient();

	//-

	// 1. COLOR PICKER

	if (SHOW_ColorPicker)
		gui_imGui_ColorPicker();

	//-

	if (SHOW_ColorRange) {
		gui_imGui_ColorRange();
	}

	if (SHOW_UserPalette) {
		gui_imGui_ColorPalette();
	}

	// 2. COLORS MANAGER

	if (SHOW_ColorManager)
		gui_imGui_ColorManager();

	//-

	// 3. PRESET MANAGER

	if (SHOW_PresetManager)
		gui_imGui_PresetManager();

	//-

	// 4. PANELS MANAGER

	if (SHOW_PanelsManager)
		gui_imGui_ControlPanels();

	//-

	this->gui.end();

	// TODO: must make another key disabler but when mouse is in text-input panels..
	// TODO: bug fix for disabled mouse on startup
	//if (ofGetFrameNum() == 30)
	//{
	//    //ofxMacMousePress(OFX_MAC_MOUSE_BUTTON_LEFT, 700, 450);
	//    ofxMacMouseClick(OFX_MAC_MOUSE_BUTTON_LEFT, 700, 450, 1);
	//
	//    //CGWarpMouseCursorPosition(CGPoint {700,450});//not trigs event
	//    //mouse_event (MOUSEEVENTF_LEFTDOWN, 0,0,0,0);
	//    //mouse_event (MOUSEEVENTF_LEFTUP, 0,0,0,0);
	//
	//    //#include <X11/Xlib.h>
	//    //int newX = 10;
	//    //int newY = 5;
	//    //if ((display = XOpenDisplay(NULL)) == NULL)
	//    //    ofExit(0);
	//    //root = DefaultRootWindow(display);
	//    //XWarpPointer(display, None, None, 0, 0, 0, 0,newX, newY);
	//    //XCloseDisplay(display);
	//
	//    return false;
	//}

	//// TODO: BUG: startup disabled keys..
	//if (ofGetFrameNum() < 60)
	//{
	//    return false;
	//}
	//else
	//{
	return mainSettings.mouseOverGui;
	//}
}


//--------------------------------------------------------------
void ofxColorManager::gui_imGui_Theme()
{
	// must be done after setup the gui

	ImGuiStyle *style = &ImGui::GetStyle();

	// my dark theme
	//    ImVec4* colors = ImGui::GetStyle().Colors;
	style->Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	style->Colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.54f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.26f, 0.26f, 0.68f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.25f, 0.25f, 0.67f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.12f, 0.11f, 0.11f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.74f, 0.75f, 0.77f, 0.79f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.79f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.70f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.31f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	style->Colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.29f, 0.29f, 0.29f, 0.78f);
	style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.44f, 0.44f, 0.44f, 0.25f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.68f, 0.68f, 0.68f, 0.67f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.68f, 0.68f, 0.68f, 0.95f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.81f, 0.79f, 0.79f, 1.00f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.58f, 0.58f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.35f);
	style->Colors[ImGuiCol_DragDropTarget] = ImVec4(0.50f, 0.50f, 0.50f, 0.90f);
	style->Colors[ImGuiCol_NavHighlight] = ImVec4(0.79f, 0.79f, 0.79f, 1.00f);
	style->Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	style->Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	style->Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

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


#pragma mark - CURVE TOOL

//--------------------------------------------------------------
void ofxColorManager::curveTool_setup()
{
	//gradient here too
	curve_img_gradient.allocate(image_curvedGradient_w, image_curvedGradient_h, OF_IMAGE_COLOR);

	curvesTool.setup(curveTool_amount);
	//TODO: TEMP
	curvesTool.load("settings/curves.yml"); //needed because it fills polyline

	curve_pos.set("INPUT", 0., 0., 1.);
	curve_pos_out.set("OUTPUT", 0., 0., 1.);
	bResetCurve.set("RESET CURVE", false);
	SHOW_TEST_Curve.set("SHOW TEST CURVE", false);
	params_curve.add(curve_pos);
	params_curve.add(curve_pos_out);
	params_curve.add(bResetCurve);
	params_curve.add(SHOW_TEST_Curve);
	curveMod.set("CURVE MOD", 0.5, 0., 1.);
	ofAddListener(params_curve.parameterChangedE(), this, &ofxColorManager::Changed_Controls);

	//-

	// slider live test color out for this input
	curve_pos_slider.setup(slider_x + (slider_w + pad), slider_y, slider_w, slider_h, 0., 1., 0, true, false);
	curve_pos_slider.setVisible(SHOW_TEST_Curve);

	// curve mod
	curveMod = 0.5;
	curveMod_Slider.setup(sliderMod_x, sliderMod_y, sliderMod_w, sliderMod_h, 0., 1., 0, true, false);
	curveMod_Slider.setPercent(curveMod);
	curveMod_Slider.setVisible(SHOW_Curve);
	//    curve_pos_slider.setLabelString("input");
}


//--------------------------------------------------------------
void ofxColorManager::curveTool_update()
{
	// update values
	curve_pos = curve_pos_slider.getValue();
	curve_pos_LUT = (int)ofMap(curve_pos.get(), 0., 1., 0, curveTool_amount - 1);

	// curve modifier
	curveMod = curveMod_Slider.getValue();
	int pointsSize = curvesTool.getPointSize();
	int pointToModify = 1;//default 1 if size is 3 points
	int pointY;
	if (pointsSize % 2 != 0 && pointsSize >= 3)//impar
		pointToModify = pointsSize / 2;
	else if (pointsSize % 2 == 0 && pointsSize >= 3)
		pointToModify = pointsSize / 2 - 1;

	//TODO:
	//Windows
	pointY = (curvesTool.getPoint(pointToModify)).x;

	curvesTool.set(pointToModify, ofVec2f(pointY, ofMap(curveMod, 0., 1., 0, curveTool_amount)));

	//--

	// vertical palette colors gradient rectangle modified by curveTool

	// every y point has different color
	for (int y = 0; y < image_curvedGradient_h; y++)
	{
		float input_LUT = ofMap(y, 0, image_curvedGradient_h, 0, curveTool_amount - 1);
		float output_LUT = curvesTool[input_LUT];
		float output;
		output = ofMap(output_LUT, 0, curveTool_amount - 1, 1., 0.);
		ofColor c = gradient.getColorAtPercent(output);

		// all x point has the same color
		for (int x = 0; x < image_curvedGradient_w; x++)
		{
			curve_img_gradient.setColor(x, y, c);
		}
	}
	curve_img_gradient.update();

	//-

	// UPDATE TARGET COLOR POINTER IN ofApp
	if (color_TARGET != nullptr)//only if pointer is setted
	{
		float out = ofMap(curvesTool.getAtPercent(1.0 - curve_pos), 0, curveTool_amount - 1, 1., 0.);
		ofColor c = gradient.getColorAtPercent(out);
		color_TARGET->set(c);//TODO: should reduce calls
	}
}


//--------------------------------------------------------------
void ofxColorManager::gradient_draw()
{

	// GRADIENT
	// 1. un-curved gradient rectangle (left positioned)
	gradient.drawDebug(grad_x, grad_y, grad_w, grad_h);
}


//--------------------------------------------------------------
void ofxColorManager::curveTool_draw()
{

	ofPushMatrix();
	ofPushStyle();

	// NOTE: sometimes we need some tricky hacking to avoid rare fliping from gradients
	curve_pos_out = ofMap(curvesTool.getAtPercent(curve_pos), 0, curveTool_amount - 1, 0., 1.);

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

	ofPopMatrix();
	ofPopStyle();
}


#pragma mark - PALETTES

//--------------------------------------------------------------
void ofxColorManager::palettes_setup()
{
	int btn_pad_w;
	btn_pad_w = 210;//padding to place boxes right to the labels
	int x0;
	x0 = palettes_x + btn_pad_w;
	int y0 = palettes_y;
	int h0 = box_size + pad;

	// 1. FROM OFX-COLOR-PALETTE

	// 1. triad
	for (int i = 0; i < triad.size(); i++)
	{
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
	for (int i = 0; i < complementTriad.size(); i++)
	{
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
	for (int i = 0; i < complement.size(); i++)
	{
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
	for (int i = 0; i < complementBrightness.size(); i++)
	{
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
	for (int i = 0; i < monochrome.size(); i++)
	{
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
	for (int i = 0; i < monochromeBrightness.size(); i++)
	{
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
	for (int i = 0; i < analogue.size(); i++)
	{
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

	//    // 8. random
	//    x0 = palettes_x + btn_pad_w;
	//    y0 += h0;
	//    for (int i = 0; i < random.size(); i++) {
	//        ButtonExample *btn = new ButtonExample();
	//        btn->setup(x0, y0, box_size, box_size);
	//        btn->setup_colorBACK(color_clicked);
	//        btn->setLocked(true);
	//        btn->setName("random" + ofToString(i));
	//        btn->setColor(random[i]);
	//        scene->addChild(btn);
	//        btns_plt_Random.push_back(btn);
	//        x0 += h0;
	//    }

	//-

	// 2. FROM OFX-COLOUR-THEORY

	x0 = palettes_x + btn_pad_w;
	y0 += h0;
	for (int i = 0; i < colors_Analogous.size(); i++)
	{
		ButtonExample *btn = new ButtonExample();
		btn->setup(x0, y0, box_size, box_size);
		btn->setup_colorBACK(color_clicked);
		btn->setLocked(true);
		btn->setName("CT_Analogous" + ofToString(i));
		btn->setColor(colors_Analogous[i]);
		scene->addChild(btn);
		btns_plt_CT_Analogous.push_back(btn);
		x0 += h0;
	}
	x0 = palettes_x + btn_pad_w;
	y0 += h0;
	for (int i = 0; i < colors_Complementary.size(); i++)
	{
		ButtonExample *btn = new ButtonExample();
		btn->setup(x0, y0, box_size, box_size);
		btn->setup_colorBACK(color_clicked);
		btn->setLocked(true);
		btn->setName("CT_Complementary" + ofToString(i));
		btn->setColor(colors_Complementary[i]);
		scene->addChild(btn);
		btns_plt_CT_Complementary.push_back(btn);
		x0 += h0;
	}
	x0 = palettes_x + btn_pad_w;
	y0 += h0;
	for (int i = 0; i < colors_SplitComplementary.size(); i++)
	{
		ButtonExample *btn = new ButtonExample();
		btn->setup(x0, y0, box_size, box_size);
		btn->setup_colorBACK(color_clicked);
		btn->setLocked(true);
		btn->setName("CT_SplitComplementary" + ofToString(i));
		btn->setColor(colors_SplitComplementary[i]);
		scene->addChild(btn);
		btns_plt_CT_SplitComplementary.push_back(btn);
		x0 += h0;
	}
	x0 = palettes_x + btn_pad_w;
	y0 += h0;
	for (int i = 0; i < colors_Compound.size(); i++)
	{
		ButtonExample *btn = new ButtonExample();
		btn->setup(x0, y0, box_size, box_size);
		btn->setup_colorBACK(color_clicked);
		btn->setLocked(true);
		btn->setName("CT_Compound" + ofToString(i));
		btn->setColor(colors_Compound[i]);
		scene->addChild(btn);
		btns_plt_CT_Compound.push_back(btn);
		x0 += h0;
	}
	x0 = palettes_x + btn_pad_w;
	y0 += h0;
	for (int i = 0; i < colors_FlippedCompound.size(); i++)
	{
		ButtonExample *btn = new ButtonExample();
		btn->setup(x0, y0, box_size, box_size);
		btn->setup_colorBACK(color_clicked);
		btn->setLocked(true);
		btn->setName("CT_FlippedCompound" + ofToString(i));
		btn->setColor(colors_FlippedCompound[i]);
		scene->addChild(btn);
		btns_plt_CT_FlippedCompound.push_back(btn);
		x0 += h0;
	}
	x0 = palettes_x + btn_pad_w;
	y0 += h0;
	for (int i = 0; i < colors_Monochrome.size(); i++)
	{
		ButtonExample *btn = new ButtonExample();
		btn->setup(x0, y0, box_size, box_size);
		btn->setup_colorBACK(color_clicked);
		btn->setLocked(true);
		btn->setName("CT_Monochrome" + ofToString(i));
		btn->setColor(colors_Monochrome[i]);
		scene->addChild(btn);
		btns_plt_CT_Monochrome.push_back(btn);
		x0 += h0;
	}
	x0 = palettes_x + btn_pad_w;
	y0 += h0;
	for (int i = 0; i < colors_Triad.size(); i++)
	{
		ButtonExample *btn = new ButtonExample();
		btn->setup(x0, y0, box_size, box_size);
		btn->setup_colorBACK(color_clicked);
		btn->setLocked(true);
		btn->setName("CT_Triad" + ofToString(i));
		btn->setColor(colors_Triad[i]);
		scene->addChild(btn);
		btns_plt_CT_Triad.push_back(btn);
		x0 += h0;
	}
	x0 = palettes_x + btn_pad_w;
	y0 += h0;
	for (int i = 0; i < colors_Tetrad.size(); i++)
	{
		ButtonExample *btn = new ButtonExample();
		btn->setup(x0, y0, box_size, box_size);
		btn->setup_colorBACK(color_clicked);
		btn->setLocked(true);
		btn->setName("CT_Tetrad" + ofToString(i));
		btn->setColor(colors_Tetrad[i]);
		scene->addChild(btn);
		btns_plt_CT_Tetrad.push_back(btn);
		x0 += h0;
	}
}


//--------------------------------------------------------------
void ofxColorManager::palettes_setup_labels()
{

	// 1. populate label buttons to trig palettes

	int x0 = palettes_x;
	int padH = 5;
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

	//-

	// 1. FROM OFX-COLOR-PALETTE

	for (int p = 0; p < NUM_PALETTES; p++)
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
			//            case 7:
			//                btn->setup("RANDOM");
			//                btn->setThisPaletteType(7);
			//                btn->set_SELECTED_palette(SELECTED_palette);
			//                break;
		}

		btn->setPosition(x0, y0 + p * btn_plt_h);
		btn->setBGColor(btn_plt_c);
		btn->setLabelColor(ofColor::white);
		//btn->setBorderColor(ofColor(128));
		btn->setBorder(false);
		scene->addChild(btn);
		btns_plt_Selector.push_back(btn);
	}

	//-

	// 2. FROM OFX-COLOUR-THEORY

	int iStart, iEnd;
	iStart = NUM_PALETTES;
	iEnd = NUM_PALETTES + NUM_CT_PALETTES;

	for (int p = iStart; p < iEnd; p++)
	{
		ButtonPaletteSelector *btn = new ButtonPaletteSelector();

		if (p == iStart + 0)
		{
			btn->setup("CT_ANALOGOUS");
			btn->setThisPaletteType(iStart + 0);
			btn->set_SELECTED_palette(SELECTED_palette);
		}
		else if (p == iStart + 1)
		{
			btn->setup("CT_COMPLEMENTARY");
			btn->setThisPaletteType(iStart + 1);
			btn->set_SELECTED_palette(SELECTED_palette);
		}
		else if (p == iStart + 2)
		{
			btn->setup("CT_SPLITCOMPLEMENTARY");
			btn->setThisPaletteType(iStart + 2);
			btn->set_SELECTED_palette(SELECTED_palette);
		}
		else if (p == iStart + 3)
		{
			btn->setup("CT_COMPOUND");
			btn->setThisPaletteType(iStart + 3);
			btn->set_SELECTED_palette(SELECTED_palette);
		}
		else if (p == iStart + 4)
		{
			btn->setup("CT_FLIPPEDCOMPOUND");
			btn->setThisPaletteType(iStart + 4);
			btn->set_SELECTED_palette(SELECTED_palette);
		}
		else if (p == iStart + 5)
		{
			btn->setup("CT_MONOCHROME");
			btn->setThisPaletteType(iStart + 5);
			btn->set_SELECTED_palette(SELECTED_palette);
		}
		else if (p == iStart + 6)
		{
			btn->setup("CT_TETRAD");
			btn->setThisPaletteType(iStart + 6);
			btn->set_SELECTED_palette(SELECTED_palette);
		}
		else if (p == iStart + 7)
		{
			btn->setup("CT_TRIAD");
			btn->setThisPaletteType(iStart + 7);
			btn->set_SELECTED_palette(SELECTED_palette);
		}

		//-

		btn->setPosition(x0, y0 + p * btn_plt_h);
		btn->setBGColor(btn_plt_c);
		btn->setLabelColor(ofColor::white);
		//btn->setBorderColor(ofColor(128));
		btn->setBorder(false);
		scene->addChild(btn);
		btns_plt_Selector.push_back(btn);
	}

	//-

	// 2. set to draw border to selected palette

	if ((btns_plt_Selector.size() > 0) &&
		(btns_plt_Selector.size() > SELECTED_palette_LAST))
	{
		btns_plt_Selector[SELECTED_palette_LAST]->setBorder(true);
	}
}


//--------------------------------------------------------------
void ofxColorManager::palette_recallFromPalettes(int p)
{
	ofLogNotice(__FUNCTION__) << p;

	palette_clear();
	ofColor color;

	// 1. FROM OFX-COLOR-PALETTE

	switch (p)
	{
	case 0:
		for (int i = 0; i < btns_plt_Triad.size(); i++)
		{
			color = btns_plt_Triad[i]->getColor();
			palette_addColor(color);
		}
		break;
	case 1:
		for (int i = 0; i < btns_plt_ComplTriad.size(); i++)
		{
			color = btns_plt_ComplTriad[i]->getColor();
			palette_addColor(color);
		}
		break;
	case 2:
		for (int i = 0; i < btns_plt_CompSat.size(); i++)
		{
			color = btns_plt_CompSat[i]->getColor();
			palette_addColor(color);
		}
		break;
	case 3:
		for (int i = 0; i < btns_plt_ComplBrgt.size(); i++)
		{
			color = btns_plt_ComplBrgt[i]->getColor();
			palette_addColor(color);
		}
		break;
	case 4:
		for (int i = 0; i < btns_plt_MonoSat.size(); i++)
		{
			color = btns_plt_MonoSat[i]->getColor();
			palette_addColor(color);
		}
		break;
	case 5:
		for (int i = 0; i < btns_plt_MonoBrgt.size(); i++)
		{
			color = btns_plt_MonoBrgt[i]->getColor();
			palette_addColor(color);
		}
		break;
	case 6:
		for (int i = 0; i < btns_plt_Analog.size(); i++)
		{
			color = btns_plt_Analog[i]->getColor();
			palette_addColor(color);
		}
		break;
		//        case 7:
		//            for (int i = 0; i < btns_plt_Random.size(); i++) {
		//                color = btns_plt_Random[i]->getColor();
		//                palette_addColor(color);
		//            }
		//            break;
	}

	//-

	// 2. FROM OFX-COLOUR-THEORY

	switch (p)
	{
		//        case 8:
	case 7:
		for (int i = 0; i < btns_plt_CT_Analogous.size(); i++)
		{
			color = btns_plt_CT_Analogous[i]->getColor();
			palette_addColor(color);
		}
		break;
		//        case 9:
	case 8:
		for (int i = 0; i < btns_plt_CT_Complementary.size(); i++)
		{
			color = btns_plt_CT_Complementary[i]->getColor();
			palette_addColor(color);
		}
		break;
		//        case 10:
	case 9:
		for (int i = 0; i < btns_plt_CT_SplitComplementary.size(); i++)
		{
			color = btns_plt_CT_SplitComplementary[i]->getColor();
			palette_addColor(color);
		}
		break;
		//        case 11:
	case 10:
		for (int i = 0; i < btns_plt_CT_Compound.size(); i++)
		{
			color = btns_plt_CT_Compound[i]->getColor();
			palette_addColor(color);
		}
		break;
		//        case 12:
	case 11:
		for (int i = 0; i < btns_plt_CT_FlippedCompound.size(); i++)
		{
			color = btns_plt_CT_FlippedCompound[i]->getColor();
			palette_addColor(color);
		}
		break;
		//        case 13:
	case 12:
		for (int i = 0; i < btns_plt_CT_Monochrome.size(); i++)
		{
			color = btns_plt_CT_Monochrome[i]->getColor();
			palette_addColor(color);
		}
		break;
		//        case 14:
	case 13:
		for (int i = 0; i < btns_plt_CT_Triad.size(); i++)
		{
			color = btns_plt_CT_Triad[i]->getColor();
			palette_addColor(color);
		}
		break;
		//        case 15:
	case 14:
		for (int i = 0; i < btns_plt_CT_Tetrad.size(); i++)
		{
			color = btns_plt_CT_Tetrad[i]->getColor();
			palette_addColor(color);
		}
		break;
	}

	//-

	// WORKFLOW: set background color from first/last palette color
	if (color_backGround_SETAUTO)
	{
		if (palette.size() > 0)
		{
			ofColor c;
			c = palette[0];//get first color from user palette

			//TODO: must improve
			// modulate darker
			//if (color_backGround_Darker)
			//{
			//    float darkness;
			//    //darkness = ofMap(backgroundDarkness, 0.0, 1.0, 0.0, 1.5);
			//    float b;
			//    if (backgroundDarkness < 0.5)
			//    {
			//        b = c.getBrightness() / backgroundDarkness * 10;
			//    }
			//    else if (backgroundDarkness >= 0.5)
			//    {
			//        b = c.getBrightness() * backgroundDarkness * 20;
			//    }
			//
			//    b = ofClamp(b, 0.0, 255.0);
			//    c.setBrightness(b);
			//}
			if (color_backGround_Darker)
			{
				float darkness = ofMap(backgroundDarkness, 0.0, 1.0, 0.0, 2.0);
				float b = c.getBrightness() * darkness;
				//b = ofClamp(b, 0.0, 1.0);
				c.setBrightness(b);
			}
			color_backGround.set(c);
		}
	}

	//-
}


//--------------------------------------------------------------
void ofxColorManager::palettes_update()
{

	// calculate base primary color
	if (!MODE_Palette)
	{
		// using hue only from picked color and forcing sat/(brg from algo sliders
		base = ofFloatColor::fromHsb(color_picked.get().getHue(),
			ofMap(SATURATION, 0, 255, 0., 1.),
			ofMap(BRIGHTNESS, 0, 255, 0., 1.));
	}
	else
	{
		// check using hue + sat/brg from color ignoring algo panel SV sliders
		base = color_picked.get();
	}

	//--

	// 1. FROM OFX-COLOR-PALETTE

	// update palettes
	complement.setBaseColor(base);
	complementBrightness.setBaseColor(base);
	triad.setBaseColor(base);
	complementTriad.setBaseColor(base);
	monochrome.setBaseColor(base);
	monochromeBrightness.setBaseColor(base);
	analogue.setBaseColor(base);
	//random.setBaseColor(base);

	complement.generateComplementary(ofxColorPalette::SATURATION, NUM_COLORS_ALGO_PALETTES);
	complementBrightness.generateComplementary(ofxColorPalette::BRIGHTNESS, NUM_COLORS_ALGO_PALETTES);
	triad.generateTriad();
	complementTriad.generateComplementaryTriad();
	monochrome.generateMonoChromatic(ofxColorPalette::SATURATION, NUM_COLORS_ALGO_PALETTES);
	monochromeBrightness.generateMonoChromatic(ofxColorPalette::BRIGHTNESS, NUM_COLORS_ALGO_PALETTES);
	analogue.generateAnalogous(NUM_COLORS_ALGO_PALETTES, 0.2);

	//-

	// this will not runned on setup() because vectors are empty yet

	// PALETTES

	// 1. triad
	for (int i = 0; i < btns_plt_Triad.size(); i++)
	{
		btns_plt_Triad[i]->setColor(triad[i]);
	}
	// 2. complement triad
	for (int i = 0; i < btns_plt_ComplTriad.size(); i++)
	{
		btns_plt_ComplTriad[i]->setColor(complementTriad[i]);
	}
	// 3. complement sat
	for (int i = 0; i < btns_plt_CompSat.size(); i++)
	{
		btns_plt_CompSat[i]->setColor(complement[i]);
	}
	// 4. complement brgt
	for (int i = 0; i < btns_plt_ComplBrgt.size(); i++)
	{
		btns_plt_ComplBrgt[i]->setColor(complementBrightness[i]);
	}
	// 5. mono sat
	for (int i = 0; i < btns_plt_MonoSat.size(); i++)
	{
		btns_plt_MonoSat[i]->setColor(monochrome[i]);
	}
	// 6. mono brgt
	for (int i = 0; i < btns_plt_MonoBrgt.size(); i++)
	{
		btns_plt_MonoBrgt[i]->setColor(monochromeBrightness[i]);
	}
	// 7. analogue
	for (int i = 0; i < btns_plt_Analog.size(); i++)
	{
		btns_plt_Analog[i]->setColor(analogue[i]);
	}
	//    // 8. random
	//    for (int i = 0; i < btns_plt_Random.size(); i++) {
	//        btns_plt_Random[i]->setColor(random[i]);
	//    }

	//--

	// 2. FROM OFX-COLOUR-THEORY

	palettes_colorTheory_update();//TODO: reduce calls...

	// 0. btns_plt_CT_Analogous
	for (int i = 0; i < btns_plt_CT_Analogous.size(); i++)
	{
		btns_plt_CT_Analogous[i]->setColor(colors_Analogous[i]);
	}
	// 1. btns_plt_CT_Complementary
	for (int i = 0; i < btns_plt_CT_Complementary.size(); i++)
	{
		btns_plt_CT_Complementary[i]->setColor(colors_Complementary[i]);
	}
	// 2. btns_plt_CT_SplitComplementary
	for (int i = 0; i < btns_plt_CT_SplitComplementary.size(); i++)
	{
		btns_plt_CT_SplitComplementary[i]->setColor(colors_SplitComplementary[i]);
	}
	// 3. btns_plt_CT_Compound
	for (int i = 0; i < btns_plt_CT_Compound.size(); i++)
	{
		btns_plt_CT_Compound[i]->setColor(colors_Compound[i]);
	}
	// 4. btns_plt_CT_FlippedCompound
	for (int i = 0; i < btns_plt_CT_FlippedCompound.size(); i++)
	{
		btns_plt_CT_FlippedCompound[i]->setColor(colors_FlippedCompound[i]);
	}
	// 5. btns_plt_CT_Monochrome
	for (int i = 0; i < btns_plt_CT_Monochrome.size(); i++)
	{
		btns_plt_CT_Monochrome[i]->setColor(colors_Monochrome[i]);
	}
	// 6. btns_plt_CT_Tetrad
	for (int i = 0; i < btns_plt_CT_Tetrad.size(); i++)
	{
		btns_plt_CT_Tetrad[i]->setColor(colors_Tetrad[i]);
	}
	// 7. btns_plt_CT_Triad
	for (int i = 0; i < btns_plt_CT_Triad.size(); i++)
	{
		btns_plt_CT_Triad[i]->setColor(colors_Triad[i]);
	}

	//--
}


//--------------------------------------------------------------
void ofxColorManager::palettes_resize()
{
	// 1. remove all color boxes

	//-

	// 1. FROM OFX-COLOR-PALETTE

	for (int i = 0; i < btns_plt_Triad.size(); i++)
	{
		std::string n = ("triad" + ofToString(i));
		auto a = scene->getChildWithName(n, 1000);
		auto b = a->getName();
		scene->removeChild(a, false);
		ofLogVerbose(__FUNCTION__) << "removed children: " << b;
	}
	btns_plt_Triad.clear();

	for (int i = 0; i < btns_plt_ComplTriad.size(); i++)
	{
		std::string n = ("compTriad" + ofToString(i));
		auto a = scene->getChildWithName(n, 1000);
		auto b = a->getName();
		scene->removeChild(a, false);
		ofLogVerbose(__FUNCTION__) << "removed children: " << b;
	}
	btns_plt_ComplTriad.clear();

	for (int i = 0; i < btns_plt_CompSat.size(); i++)
	{
		std::string n = ("compSat" + ofToString(i));
		auto a = scene->getChildWithName(n, 1000);
		auto b = a->getName();
		scene->removeChild(a, false);
		ofLogVerbose(__FUNCTION__) << "removed children: " << b;
	}
	btns_plt_CompSat.clear();

	for (int i = 0; i < btns_plt_ComplBrgt.size(); i++)
	{
		std::string n = ("compBrgt" + ofToString(i));
		auto a = scene->getChildWithName(n, 1000);
		auto b = a->getName();
		scene->removeChild(a, false);
		ofLogVerbose(__FUNCTION__) << "removed children: " << b;
	}
	btns_plt_ComplBrgt.clear();

	for (int i = 0; i < btns_plt_MonoSat.size(); i++)
	{
		std::string n = ("monoSat" + ofToString(i));
		auto a = scene->getChildWithName(n, 1000);
		auto b = a->getName();
		scene->removeChild(a, false);
		ofLogVerbose(__FUNCTION__) << "removed children: " << b;
	}
	btns_plt_MonoSat.clear();

	for (int i = 0; i < btns_plt_MonoBrgt.size(); i++)
	{
		std::string n = ("monoBrgt" + ofToString(i));
		auto a = scene->getChildWithName(n, 1000);
		auto b = a->getName();
		scene->removeChild(a, false);
		ofLogVerbose(__FUNCTION__) << "removed children: " << b;
	}
	btns_plt_MonoBrgt.clear();

	for (int i = 0; i < btns_plt_Analog.size(); i++)
	{
		std::string n = ("analogue" + ofToString(i));
		auto a = scene->getChildWithName(n, 1000);
		auto b = a->getName();
		scene->removeChild(a, false);
		ofLogVerbose(__FUNCTION__) << "removed children: " << b;
	}
	btns_plt_Analog.clear();

	//    for (int i = 0; i < btns_plt_Random.size(); i++) {
	//        std::string n = ("random" + ofToString(i));
	//        auto a = scene->getChildWithName(n, 1000);
	//        auto b = a->getName();
	//        scene->removeChild(a, false);
	//        ofLogVerbose(__FUNCTION__) << "removed children: " << b;
	//    }
	//    btns_plt_Random.clear();

	//-

	// 2. FROM OFX-COLOUR-THEORY

	for (int i = 0; i < btns_plt_CT_Analogous.size(); i++)
	{
		std::string n = ("CT_Analogous" + ofToString(i));
		auto a = scene->getChildWithName(n, 1000);
		auto b = a->getName();
		scene->removeChild(a, false);
		ofLogVerbose(__FUNCTION__) << "removed children: " << b;
	}
	btns_plt_CT_Analogous.clear();

	for (int i = 0; i < btns_plt_CT_Complementary.size(); i++)
	{
		std::string n = ("CT_Complementary" + ofToString(i));
		auto a = scene->getChildWithName(n, 1000);
		auto b = a->getName();
		scene->removeChild(a, false);
		ofLogVerbose(__FUNCTION__) << "removed children: " << b;
	}
	btns_plt_CT_Complementary.clear();

	for (int i = 0; i < btns_plt_CT_SplitComplementary.size(); i++)
	{
		std::string n = ("CT_SplitComplementary" + ofToString(i));
		auto a = scene->getChildWithName(n, 1000);
		auto b = a->getName();
		scene->removeChild(a, false);
		ofLogVerbose(__FUNCTION__) << "removed children: " << b;
	}
	btns_plt_CT_SplitComplementary.clear();

	for (int i = 0; i < btns_plt_CT_Compound.size(); i++)
	{
		std::string n = ("CT_Compound" + ofToString(i));
		auto a = scene->getChildWithName(n, 1000);
		auto b = a->getName();
		scene->removeChild(a, false);
		ofLogVerbose(__FUNCTION__) << "removed children: " << b;
	}
	btns_plt_CT_Compound.clear();

	for (int i = 0; i < btns_plt_CT_FlippedCompound.size(); i++)
	{
		std::string n = ("CT_FlippedCompound" + ofToString(i));
		auto a = scene->getChildWithName(n, 1000);
		auto b = a->getName();
		scene->removeChild(a, false);
		ofLogVerbose(__FUNCTION__) << "removed children: " << b;
	}
	btns_plt_CT_FlippedCompound.clear();

	for (int i = 0; i < btns_plt_CT_Monochrome.size(); i++)
	{
		std::string n = ("CT_Monochrome" + ofToString(i));
		auto a = scene->getChildWithName(n, 1000);
		auto b = a->getName();
		scene->removeChild(a, false);
		ofLogVerbose(__FUNCTION__) << "removed children: " << b;
	}
	btns_plt_CT_Monochrome.clear();

	for (int i = 0; i < btns_plt_CT_Tetrad.size(); i++)
	{
		std::string n = ("CT_Tetrad" + ofToString(i));
		auto a = scene->getChildWithName(n, 1000);
		auto b = a->getName();
		scene->removeChild(a, false);
		ofLogVerbose(__FUNCTION__) << "removed children: " << b;
	}
	btns_plt_CT_Tetrad.clear();

	for (int i = 0; i < btns_plt_CT_Triad.size(); i++)
	{
		std::string n = ("CT_Triad" + ofToString(i));
		auto a = scene->getChildWithName(n, 1000);
		auto b = a->getName();
		scene->removeChild(a, false);
		ofLogVerbose(__FUNCTION__) << "removed children: " << b;
	}
	btns_plt_CT_Triad.clear();

	//-

	//TODO

	//    random.generateRandom(NUM_COLORS_ALGO_PALETTES);
	palettes_update();
	palettes_setup();

	//-

	// WORKFLOW: trig last picked palette
	if (bAuto_palette_recall)
	{
		palette_clear();
		palette_recallFromPalettes(SELECTED_palette_LAST);
	}

	//-
}

//--------------------------------------------------------------
void ofxColorManager::palettes_setVisible(bool b)
{
	// hide and disable touchs for buttons

	// ALGORITHMIC COLOR PALETTES

	//-

	// 1. FROM OFX-COLOR-PALETTE

	for (auto &btn : btns_plt_Triad)
	{
		btn->setVisible(b);
		btn->setEnabled(b);
	}
	for (auto &btn : btns_plt_ComplTriad)
	{
		btn->setVisible(b);
		btn->setEnabled(b);
	}
	for (auto &btn : btns_plt_CompSat)
	{
		btn->setVisible(b);
		btn->setEnabled(b);
	}
	for (auto &btn : btns_plt_ComplBrgt)
	{
		btn->setVisible(b);
		btn->setEnabled(b);
	}
	for (auto &btn : btns_plt_MonoSat)
	{
		btn->setVisible(b);
		btn->setEnabled(b);
	}
	for (auto &btn : btns_plt_MonoBrgt)
	{
		btn->setVisible(b);
		btn->setEnabled(b);
	}
	for (auto &btn : btns_plt_Analog)
	{
		btn->setVisible(b);
		btn->setEnabled(b);
	}
	//    for (auto &btn : btns_plt_Random) {
	//        btn->setVisible(b);
	//        btn->setEnabled(b);
	//    }

	//-

	// 2. FROM OFX-COLOUR-THEORY

	for (auto &btn : btns_plt_CT_Analogous)
	{
		btn->setVisible(b);
		btn->setEnabled(b);
	}
	for (auto &btn : btns_plt_CT_Complementary)
	{
		btn->setVisible(b);
		btn->setEnabled(b);
	}
	for (auto &btn : btns_plt_CT_SplitComplementary)
	{
		btn->setVisible(b);
		btn->setEnabled(b);
	}
	for (auto &btn : btns_plt_CT_Compound)
	{
		btn->setVisible(b);
		btn->setEnabled(b);
	}
	for (auto &btn : btns_plt_CT_FlippedCompound)
	{
		btn->setVisible(b);
		btn->setEnabled(b);
	}
	for (auto &btn : btns_plt_CT_Monochrome)
	{
		btn->setVisible(b);
		btn->setEnabled(b);
	}
	for (auto &btn : btns_plt_CT_Triad)
	{
		btn->setVisible(b);
		btn->setEnabled(b);
	}
	for (auto &btn : btns_plt_CT_Tetrad)
	{
		btn->setVisible(b);
		btn->setEnabled(b);
	}

	//-

	// 2. LABEL BUTTONS

	for (auto &btn : btns_plt_Selector)
	{
		btn->setVisible(b);
		btn->setLocked(!b);
	}
}


//--------------------------------------------------------------
void ofxColorManager::palettes_colorTheory_setup()
{
	//    group.setName("Color Theory");
	//    group.add(primaryColor.set("Primary Color", ofColor::magenta));
	//    group.add(colorScheme.set("Color Scheme", 6, 0, ColorWheelSchemes::colorSchemes.size() - 1));
	//    group.add(colorSchemeName);
	//    group.add(numColors.set("Num Colors", 8, 1, 10));
	//
	//    panel.setup();
	//    panel.add(group);
	//    panel.setPosition(1000, 400);
	//
	//
	//    numColors = NUM_COLORS_ALGO_PALETTES;
	//
	//    //TEST
	//    colorSchemeName.set(ColorWheelSchemes::colorSchemeNames[colorScheme.get()]);
	//    scheme = ColorWheelSchemes::colorSchemes[colorScheme.get()];
	//    scheme->setPrimaryColor(primaryColor);
	//    colors = scheme->interpolate(numColors.get());

	//-

	//TODO
	// should reduce calls.. to setup only..

	scheme_Analogous_name = (ColorWheelSchemes::colorSchemeNames[0]);
	scheme_Analogous = ColorWheelSchemes::colorSchemes[0];
	colors_Analogous = scheme_Analogous->interpolate(NUM_COLORS_ALGO_PALETTES);

	scheme_Complementary_name = (ColorWheelSchemes::colorSchemeNames[1]);
	scheme_Complementary = ColorWheelSchemes::colorSchemes[1];
	colors_Complementary = scheme_Complementary->interpolate(NUM_COLORS_ALGO_PALETTES);

	scheme_SplitComplementary_name = (ColorWheelSchemes::colorSchemeNames[2]);
	scheme_SplitComplementary = ColorWheelSchemes::colorSchemes[2];
	colors_SplitComplementary = scheme_SplitComplementary->interpolate(NUM_COLORS_ALGO_PALETTES);

	scheme_Compound_name = (ColorWheelSchemes::colorSchemeNames[3]);
	scheme_Compound = ColorWheelSchemes::colorSchemes[3];
	colors_Compound = scheme_Compound->interpolate(NUM_COLORS_ALGO_PALETTES);

	scheme_FlippedCompound_name = (ColorWheelSchemes::colorSchemeNames[4]);
	scheme_FlippedCompound = ColorWheelSchemes::colorSchemes[4];
	colors_FlippedCompound = scheme_FlippedCompound->interpolate(NUM_COLORS_ALGO_PALETTES);

	scheme_Monochrome_name = (ColorWheelSchemes::colorSchemeNames[5]);
	scheme_Monochrome = ColorWheelSchemes::colorSchemes[5];
	colors_Monochrome = scheme_Monochrome->interpolate(NUM_COLORS_ALGO_PALETTES);

	scheme_Tetrad_name = (ColorWheelSchemes::colorSchemeNames[6]);
	scheme_Tetrad = ColorWheelSchemes::colorSchemes[6];
	colors_Tetrad = scheme_Tetrad->interpolate(NUM_COLORS_ALGO_PALETTES);

	scheme_Triad_name = (ColorWheelSchemes::colorSchemeNames[7]);
	scheme_Triad = ColorWheelSchemes::colorSchemes[7];
	colors_Triad = scheme_Triad->interpolate(NUM_COLORS_ALGO_PALETTES);
}


//--------------------------------------------------------------
void ofxColorManager::palettes_colorTheory_update()
{
	//ofFloatColor base;//TODO
	//if (!MODE_Palette)
	//{
	//    // using hue only from picked color and forcing sat/(brg from algo sliders
	//    base = ofFloatColor::fromHsb(color_picked.get().getHue(),
	//        ofMap(SATURATION, 0, 255, 0., 1.),
	//        ofMap(BRIGHTNESS, 0, 255, 0., 1.));
	//}
	//else
	//{
	//    // check using hue + sat/brg from color ignoring algo panel SV sliders
	//    base = color_picked.get();
	//}

	primaryColor.set(base);

	//    //TEST
	//    colorSchemeName.set(ColorWheelSchemes::colorSchemeNames[colorScheme.get()]);
	//    scheme = ColorWheelSchemes::colorSchemes[colorScheme.get()];
	//    scheme->setPrimaryColor(primaryColor);
	//    numColors = NUM_COLORS_ALGO_PALETTES;
	//    colors = scheme->interpolate(numColors.get());

	//-

	scheme_Analogous = ColorWheelSchemes::colorSchemes[0];
	scheme_Analogous->setPrimaryColor(primaryColor);
	colors_Analogous = scheme_Analogous->interpolate(NUM_COLORS_ALGO_PALETTES);

	scheme_Complementary = ColorWheelSchemes::colorSchemes[1];
	scheme_Complementary->setPrimaryColor(primaryColor);
	colors_Complementary = scheme_Complementary->interpolate(NUM_COLORS_ALGO_PALETTES);

	scheme_SplitComplementary = ColorWheelSchemes::colorSchemes[2];
	scheme_SplitComplementary->setPrimaryColor(primaryColor);
	colors_SplitComplementary = scheme_SplitComplementary->interpolate(NUM_COLORS_ALGO_PALETTES);

	scheme_Compound = ColorWheelSchemes::colorSchemes[3];
	scheme_Compound->setPrimaryColor(primaryColor);
	colors_Compound = scheme_Compound->interpolate(NUM_COLORS_ALGO_PALETTES);

	scheme_FlippedCompound = ColorWheelSchemes::colorSchemes[4];
	scheme_FlippedCompound->setPrimaryColor(primaryColor);
	colors_FlippedCompound = scheme_FlippedCompound->interpolate(NUM_COLORS_ALGO_PALETTES);

	scheme_Monochrome = ColorWheelSchemes::colorSchemes[5];
	scheme_Monochrome->setPrimaryColor(primaryColor);
	colors_Monochrome = scheme_Monochrome->interpolate(NUM_COLORS_ALGO_PALETTES);

	scheme_Tetrad = ColorWheelSchemes::colorSchemes[6];
	scheme_Tetrad->setPrimaryColor(primaryColor);
	colors_Tetrad = scheme_Tetrad->interpolate(NUM_COLORS_ALGO_PALETTES);

	scheme_Triad = ColorWheelSchemes::colorSchemes[7];
	scheme_Triad->setPrimaryColor(primaryColor);
	colors_Triad = scheme_Triad->interpolate(NUM_COLORS_ALGO_PALETTES);

	//    NOTE: RANDOM = 0, ANALOGOUS = 1, COMPLEMENTARY = 2,
	//    SPLIT_COMPLEMENTARY = 3, COMPOUND = 4, FLIPPED_COMPOUND = 5,
	//    MONOCHROME = 6, TRIAD = 7, TETRAD = 8
}


////--------------------------------------------------------------
//void ofxColorManager::ColorWheel_draw() {
//
//    //-
//
//    int w = 30;
//    int guiX = panel.getPosition().x;
//    int guiY = panel.getPosition().y;
//
//    for (int i = 0; i < colors.size(); i++) {
//        ofSetColor(colors[i]);
//        ofFill();
//        ofDrawRectangle(guiX + w * i, guiY-2*w, w, w);
//    }
//
//    panel.draw();
//}

#pragma mark - PALETTE

//--------------------------------------------------------------
void ofxColorManager::palette_load_ColourLovers()
{
	ofLogNotice(__FUNCTION__) << "palette_load_ColourLovers";

	// 1. erase user palette and fill a new one with just update/received colour lovers
	palette_clear();
	ofColor c;
	for (int i = 0; i < myPalette.size(); i++)
	{
		c = myPalette[i];
		ofLogNotice(__FUNCTION__) << "color [" << i << "] " << ofToString(c);
		palette_addColor(c);
	}

	//TODO
	//for preset manager
	// 2. get color from colour lovers
	if (palette.size() > 0)
	{
		ofColor c;
		c = palette[0];//get first color
		color_clicked = ofColor(c);
	}

	//-

	// WORKFLOW: set background color from first/last palette color
	if (color_backGround_SETAUTO)
	{
		if (palette.size() > 0)
		{
			ofColor c;
			c = palette[0];//get first color from user palette

			//TODO: must improve
			// modulate darker
			//if (color_backGround_Darker)
			//{
			//    float darkness;
			//    //darkness = ofMap(backgroundDarkness, 0.0, 1.0, 0.0, 1.5);
			//    float b;
			//    if (backgroundDarkness < 0.5)
			//    {
			//        b = c.getBrightness() / backgroundDarkness * 10;
			//    }
			//    else if (backgroundDarkness >= 0.5)
			//    {
			//        b = c.getBrightness() * backgroundDarkness * 20;
			//    }
			//
			//    b = ofClamp(b, 0.0, 255.0);
			//    c.setBrightness(b);
			//}
			if (color_backGround_Darker)
			{
				float darkness = ofMap(backgroundDarkness, 0.0, 1.0, 0.0, 2.0);
				float b = c.getBrightness() * darkness;
				//b = ofClamp(b, 0.0, 1.0);
				c.setBrightness(b);
			}
			color_backGround.set(c);
		}
	}
}


//--------------------------------------------------------------
void ofxColorManager::palette_drawMINI()
{
	glm::vec2 palettePos;
	int boxW = 40;
	int boxPad = 1;
	int boxSize = boxW + boxPad;
	ofRectangle r;

	//    // right top corner
	//    palettePos = glm::vec2(ofGetWidth() - palette.size()*boxSize, 2*boxPad);
	// left top corner
	palettePos = glm::vec2(2, 2 * boxPad);

	ofPushMatrix();
	ofPushStyle();
	ofTranslate(palettePos);

	ofDrawBitmapStringHighlight(PRESET_name, glm::vec2(4, boxW + 15), ofColor::black, ofColor::white);

	for (int col = 0; col < palette.size(); col++)
	{
		r = ofRectangle(0, 0, boxW, boxW);
		ofFill();
		ofSetColor(palette[col]);
		ofDrawRectangle(r);
		ofNoFill();
		ofSetColor(ofColor(ofColor::white, 64));
		ofDrawRectangle(r);
		ofTranslate(boxSize, 0);
	}

	ofPopStyle();
	ofPopMatrix();
}


//--------------------------------------------------------------
void ofxColorManager::palette_addColor(ofColor c)
{
	ofLogVerbose(__FUNCTION__) << "palette_addColor: " << ofToString(c);
	palette.push_back(c);
	gradient.addColor(c);
	palette_addColor_toInterface(c);

	//// TODO: BUG
	//// WORKFLOW: select last one, just the one created now
	//if (palette_colorSelected > -1 && bPaletteEdit)
	//{
	//    palette_colorSelected = palette.size() - 1;
	//}
}


//--------------------------------------------------------------
void ofxColorManager::palette_removeColor(int c)
{
	// remove current color from user palette

	ofLogNotice(__FUNCTION__) << "palette_removeColor: " << c;
	ofLogNotice(__FUNCTION__) << "palette size pre: " << palette.size();

	if (c < palette.size())
	{
		// 0. erase last touched color th element
		//palette.erase(palette.begin() + c - 1);
		palette.erase(palette.begin() + c);

		// 1. debug after remove color from palette vector
		std::cout << "vector palette contains: ";
		for (unsigned i = 0; i < palette.size(); ++i)
			std::cout << ' ' << palette[i];
		std::cout << '\n';
		ofLogNotice(__FUNCTION__) << "palette size post: " << palette.size();

		// 2. reset gradient
		gradient.reset();

		// 3. remove all interface buttons
		for (int i = 0; i < btns_palette.size(); i++)
		{
			std::string n = ("btn" + ofToString(i));
			auto a = scene->getChildWithName(n, 1000);
			auto b = a->getName();
			scene->removeChild(a, false);
			ofLogVerbose(__FUNCTION__) << "removed children: " << b;
		}
		btns_palette.clear();

		// 4. add edited palette colors vector to interface buttons and to gradient
		for (int i = 0; i < palette.size(); i++)
		{
			ofLogNotice(__FUNCTION__) << "add color [" << i << "]";
			gradient.addColor(palette[i]);
			palette_addColor_toInterface(palette[i]);
		}

		// 5. make positions & resizes to fill bar
		palette_rearrenge();
	}
}


//--------------------------------------------------------------
void ofxColorManager::palette_removeColorLast()
{
	if (palette.size() > 0)
	{
		palette.pop_back();
		gradient.removeColorLast();

		if (btns_palette.size() > 0)
		{
			std::string n = ("btn" + ofToString(btns_palette.size() - 1));
			auto a = scene->getChildWithName(n, 100);
			auto b = a->getName();
			scene->removeChild(a, false);
			ofLogVerbose(__FUNCTION__) << "removed children: " << b;
		}
		btns_palette.pop_back();
	}

	palette_rearrenge();
}


//--------------------------------------------------------------
void ofxColorManager::palette_touchedColor(string name)
{
	// WORKFLOW: disable auto get algorithmic palette
	if (bAuto_palette_recall)
		bAuto_palette_recall = false;

	// 1. de-select all buttons and select current touched/clicked
	for (int i = 0; i < btns_palette.size(); i++)
	{
		if (btns_palette[i]->getName() != name)
		{
			btns_palette[i]->setSelected(false);
		}
		else
		{
			// set this button selected
			btns_palette[i]->setSelected(true);//sets border only
			palette_colorSelected = i;
			ofLogNotice(__FUNCTION__) << "user palette selected color: " << i;
		}
	}

	// 2. WORKFLOW: auto set edit mode when click a color user palette
	if (!bPaletteEdit)
	{
		bPaletteEdit = true;
	}
}


//--------------------------------------------------------------
void ofxColorManager::palette_clear()
{
	// remove all colors of user palette

	ofLogNotice(__FUNCTION__) << "palette_clear";

	palette.clear();
	gradient.reset();

	//    ofLogNotice(__FUNCTION__) << "palette_clear::getNumChildren: " << scene->getNumChildren();

	for (int i = 0; i < btns_palette.size(); i++)
	{
		std::string n = ("btn" + ofToString(i));
		auto a = scene->getChildWithName(n, 1000);
		auto b = a->getName();
		scene->removeChild(a, false);
		ofLogVerbose(__FUNCTION__) << "removed children: " << b;
	}
	btns_palette.clear();
}


#pragma mark - CALLBACKS

//--------------------------------------------------------------
void ofxColorManager::Changed_ColorPicked(ofFloatColor &_c)
{
	// TEST
	if (LISTEN_isEnabled == true)
	{
		update_color_picked_CHANGES();

		// TEST
		color_picked_Update_To_HSV();//redundant.. ?

		// DEMO
		if (TEST_DEMO)
			myDEMO_palette.reStart();
	}
}


//--------------------------------------------------------------
void ofxColorManager::Changed_ColorClicked(ofFloatColor &color)
{
	// NOT USED. DISABLED
	ofLogNotice(__FUNCTION__) << ofToString(color);
	//    color_picked.set(color);
}

//--------------------------------------------------------------
void ofxColorManager::Changed_Controls(ofAbstractParameter &e)
{
	string name = e.getName();

	// TODO: should reduce callbacks in output..
	//    if (name != "INPUT" && name != "OUTPUT" &&
	//            name!="H" && name!="S" && name!="V")

	if (name != "INPUT" &&
		name != "OUTPUT")
	{
		ofLogNotice(__FUNCTION__) << name << " : " << e;
	}

	//--

	// CONTROL WINDOWS PANELS

	if (name == "SHOW COLOUR LOVERS")
	{
		ColourLoversHelper.setVisible(SHOW_ColourLovers);

		////TODO
		//if (ENABLE_keys)
		//{
		//    ColourLoversHelper.setKeysEnabled(true);
		//}
	}
	else if (name == "SHOW COLOUR LOVERS SEARCH")
	{
		ColourLoversHelper.setVisibleSearcher(SHOW_ColourLovers_searcher);
		if (SHOW_ColourLovers_searcher && !SHOW_ColourLovers)
			SHOW_ColourLovers = true;

		////TODO
		//if (ENABLE_keys)
		//{
		//    ColourLoversHelper.setKeysEnabled(true);
		//}
	}
	else if (name == "SHOW PALETTES")
	{
		palettes_setVisible(SHOW_AlgoPalettes);
	}
	else if (name == "SHOW BROWSER COLORS")
	{
		ColorBrowser.setVisible(SHOW_BrowserColors);
	}
	else if (name == "SHOW CURVE TOOL")
	{
		curveMod_Slider.setVisible(SHOW_Curve);
	}
	else if (name == "SHOW TEST CURVE")
	{
		curve_pos_slider.setVisible(SHOW_TEST_Curve);
	}
	else if (name == "SHOW USER PALETTE")
	{
		ofLogVerbose(__FUNCTION__) << "SHOW_UserPalette: " << SHOW_UserPalette << endl;
		for (int i = 0; i < btns_palette.size(); i++)
		{
			btns_palette[i]->setVisible(SHOW_UserPalette);
		}
	}
	else if (name == "SHOW COLOR PICTURE")
	{
		// WORKFLOW:
		if (bPaletteEdit && SHOW_ColorQuantizer)
			bPaletteEdit = false;

		// WORKFLOW: BUG
		//if (!SHOW_PresetManager)
		{
#ifdef QUANT_IMA
			colorQuantizer.setActive(SHOW_ColorQuantizer);
			if (SHOW_ColorQuantizer)
			{
				//open colors from one image
				//colorQuantizer.map_setup();
			}
#endif
		}
		//else
		//{
		//    colorQuantizer.setActive(false);
		//}
	}
	else if (name == "SHOW PRESET MANAGER")
	{
		//if (SHOW_PresetManager && SHOW_ColorQuantizer)
		//    colorQuantizer.setActive(false);
		//else if (!SHOW_PresetManager && SHOW_ColorQuantizer)
		//    colorQuantizer.setActive(true);
	}

	//--

	// COLOR

	// TODO: should reduce callbacks
	// TEST
	if (LISTEN_isEnabled)
	{
		if (name == "H" && name == "S" && name == "V")
		{
			ofLogNotice(__FUNCTION__) << "LISTEN_isEnabled: " << name << ":" << e;
		}

		if (name == "H")
		{
			ofColor c;
			c.set(color_picked.get());
			c.setHue(color_HUE);
			color_picked.set(c);
		}
		else if (name == "S")
		{
			ofColor c;
			c.set(color_picked.get());
			c.setSaturation(color_SAT);
			color_picked.set(c);
		}
		else if (name == "V")
		{
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

	if (name == "RANDOM COLOR")
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
			bAddColor = false;
			palette_addColor(ofColor(color_picked.get()));

			// WORKFLOW: select last one, just the one created now
			//if (bPaletteEdit)
			{
				palette_colorSelected = palette.size() - 1;

				// 1. de-select all buttons
				for (int i = 0; i < btns_palette.size(); i++)
				{
					btns_palette[i]->setSelected(false);
				}

				// 2. set last button from current added color
				btns_palette[palette_colorSelected]->setSelected(true);//sets border only
				ofLogNotice(__FUNCTION__) << "user palette selected last color: " << palette_colorSelected;
			}
		}
	}

	else if (name == "EDIT COLOR")
	{
		// 1. deselect all color buttons
		for (int i = 0; i < btns_palette.size(); i++)
		{
			if (!bPaletteEdit)
			{
				// make not selectable all color buttons
				btns_palette[i]->setSelectable(false);
				btns_palette[i]->setSelected(false);//deselect each
			}
			else if (bPaletteEdit)
			{
				// make selectable all color buttons
				btns_palette[i]->setSelectable(true);
				btns_palette[i]->setSelected(false);
			}
		}

		// WORKFLOW:
		if (!bPaletteEdit)
		{
			palette_colorSelected = -1;
		}

		// 2. and load color to the color picker panel
		if (bPaletteEdit && btns_palette.size() > 0)
		{
			// if is not selected any color button, set current color to the first one / 0
			if (palette_colorSelected == -1)
				palette_colorSelected = 0;

			// select box
			btns_palette[palette_colorSelected]->setSelected(true);//sets border only

			// autoload color on picker
			color_picked = palette[palette_colorSelected];
		}
	}

	else if (name == "REMOVE COLOR")
	{
		if (bRemoveColor)
		{
			bRemoveColor = false;

			// 1. remove last
			//palette_removeColorLast();

			// 2. remove selected
			palette_removeColor(palette_colorSelected);

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

	//-

	// ALGORITHMIC PALETTES

	//    else if (name == "RANDOM PALETTE") {
	//        if (bRandomPalette) {
	//            bRandomPalette = false;
	//
	//            random.generateRandom(NUM_COLORS_ALGO_PALETTES);
	//            palettes_update();
	//            if (bAuto_palette_recall) {
	//                //set random palette to user palette
	//                palette_recallFromPalettes(7);
	//            }
	//        }
	//    }
	else if (name == "SIZE COLORS")
	{
		palettes_resize();
	}
	else if (name == "AUTO LIVE MODE")
	{
		if (bAuto_palette_recall)
			bLock_palette = false;
	}
	else if (name == "LOCK PALETTES")
	{
		if (bLock_palette)
			bAuto_palette_recall = false;
	}
	else if (name == "BRIGHTNESS" || name == "SATURATION")
	{
		palettes_update();

		// auto create user palette from algo palette
		if (bAuto_palette_recall)
		{
			palette_clear();
			palette_recallFromPalettes(SELECTED_palette_LAST);//trig last choice

			// DEMO
			if (TEST_DEMO)
				myDEMO_palette.reStart();
		}
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
	else if (name == "GRADIENT HARD")
	{
		gradient.setHardMode(gradient_hard);
	}

	//-

	// BACKGROUND

	else if (name == "SET FROM COLOR")
	{
		if (color_backGround_SET)
		{
			color_backGround_SET = false;
			color_backGround.set(ofColor(color_picked.get()));
		}
	}
	else if (name == "AUTO SET")
	{

	}
	else if (name == "BACKGROUND")
	{
		//        if (color_backGround.get().getBrightness()!=backgroundDarkness_PRE)
		//            backgroundDarkness = color_backGround.get().getBrightness();
		//
		////        if (backgroundDarkness_PRE!=backgroundDarkness)
		////            backgroundDarkness = (int)darkness;
	}
	else if (name == "DARKNESS")
	{
		//TODO: must improve
		//palettes_update();

		// auto create user palette from algo palette
		if (bAuto_palette_recall)
		{
			palette_clear();
			palette_recallFromPalettes(SELECTED_palette_LAST);//trig last choice
		}
	}
}

//load user palette from range
//--------------------------------------------------------------
void ofxColorManager::Changed_Range(ofAbstractParameter &e)
{
	std::string name = e.getName();

	ofLogNotice(__FUNCTION__) << name << " : " << e;

	//if (name == numCcolorsRange.getName())
	//{

	//}
	//else
	{
		for (int i = 0; i < 12; i++) //12
		{
			if (name == rangTypes[i].getName() && rangTypes[i].get())
			{
				rangTypes[i] = false;//off button

				palette_clear();
				ofColor c;
				int st = i * numCcolorsRange.get();
				int ed = st + numCcolorsRange.get();
				for (int j = st; j < ed; j++)
				{
					c = paletteRange[j];
					ofLogNotice(__FUNCTION__) << "color [" << j << "] " << ofToString(c);
					palette_addColor(c);
				}
			}
		}
	}
}


#pragma mark - KEYS MOUSE CALLBACKS

//--------------------------------------------------------------
void ofxColorManager::keyPressed(ofKeyEventArgs &eventArgs)
{
	const int &key = eventArgs.key;

	// un-blockeable keys goes here

	//--

	//    //TODO: BUG:
	//    if (SHOW_ColourLovers_searcher)
	//        ENABLE_keys = false;

	//-

	if (ENABLE_keys)
	{
		//ofLogNotice(__FUNCTION__) << "key: " << key;

		//--

		// COLOR-WHEEL-SCHEMES

		if (SHOW_AlgoPalettes && !SHOW_ColourLovers && !SHOW_ColorQuantizer)
		{
			if (key == OF_KEY_UP)
			{
				SELECTED_palette = SELECTED_palette_LAST;
				SELECTED_palette--;
				if (SELECTED_palette < 0)
				{
					SELECTED_palette = 0;
				}

				if (TEST_DEMO)
					myDEMO_palette.reStart();
			}
			else if (key == OF_KEY_DOWN)
			{
				SELECTED_palette = SELECTED_palette_LAST;
				SELECTED_palette++;
				if (SELECTED_palette > NUM_TOTAL_PALETTES - 1)
				{
					SELECTED_palette = NUM_TOTAL_PALETTES - 1;
				}

				if (TEST_DEMO)
					myDEMO_palette.reStart();
			}
			else if (key == OF_KEY_LEFT && !SHOW_PresetManager)
			{
				NUM_COLORS_ALGO_PALETTES--;
				NUM_COLORS_ALGO_PALETTES = (int)ofClamp(NUM_COLORS_ALGO_PALETTES,
					NUM_COLORS_ALGO_PALETTES.getMin(),
					NUM_COLORS_ALGO_PALETTES.getMax());

				if (TEST_DEMO)
					myDEMO_palette.reStart();
			}
			else if (key == OF_KEY_RIGHT && !SHOW_PresetManager)
			{
				NUM_COLORS_ALGO_PALETTES++;
				NUM_COLORS_ALGO_PALETTES = (int)ofClamp(NUM_COLORS_ALGO_PALETTES,
					NUM_COLORS_ALGO_PALETTES.getMin(),
					NUM_COLORS_ALGO_PALETTES.getMax());

				if (TEST_DEMO)
					myDEMO_palette.reStart();
			}
		}

		//-

		// PRESET MANAGER

		if (SHOW_PresetManager)
		{
			if (key == OF_KEY_LEFT)
			{
				if (currentFile > 0)
				{
					currentFile--;
				}
				if (currentFile < files.size())
				{
					PRESET_name = fileNames[currentFile];
					ofLogNotice() << "OF_KEY_LEFT: PRESET_name: [" + ofToString(currentFile) + "] " << PRESET_name;
					preset_load(PRESET_name);
				}

				// new preset
				if (MODE_newPreset)
					MODE_newPreset = false;

				// demo mode
				if (TEST_DEMO)
					myDEMO_palette.reStart();

				//load first color from preset to algorothmic palettes
				if (palette.size() > 0)
				{
					color_picked = ofFloatColor(palette[0]);
					palettes_update();
				}
			}
			else if (key == OF_KEY_RIGHT)
			{
				if (currentFile < files.size() - 1)
				{
					currentFile++;
				}
				if (currentFile < files.size())
				{
					PRESET_name = fileNames[currentFile];
					ofLogNotice() << "OF_KEY_RIGHT: PRESET_name: [" + ofToString(currentFile) + "] " << PRESET_name;
					preset_load(PRESET_name);
				}

				// new preset
				if (MODE_newPreset)
					MODE_newPreset = false;

				//demo mode
				if (TEST_DEMO)
					myDEMO_palette.reStart();

				//load first color from preset to algorothmic palettes
				if (palette.size() > 0)
				{
					color_picked = ofFloatColor(palette[0]);
					palettes_update();
				}
			}
		}

		//-

		// OFX GUI-PANELS-LAYOUT

#ifdef INCL_LAYOUT
		if (key == 'a')
		{
			panels.SHOW_advanced = !panels.SHOW_advanced;
		}

		//    else if (key == OF_KEY_LEFT)
		//    {
		//        panels.group_Selected--;
		//    }
		//
		//    else if (key == OF_KEY_RIGHT)
		//    {
		//        panels.group_Selected++;
		//    }

		else if (key == '0')
		{
			panels.group_Selected = 0;
		}
		else if (key == '1')
		{
			panels.group_Selected = 1;
		}
		else if (key == '2')
		{
			panels.group_Selected = 2;
		}
		else if (key == '3')
		{
			panels.group_Selected = 3;
		}
		else if (key == '4')
		{
			panels.group_Selected = 4;
		}
		else if (key == '5')
		{
			panels.group_Selected = 5;
		}
		else if (key == '6')
		{
			panels.group_Selected = 6;
		}
		else if (key == '7')
		{
			panels.group_Selected = 7;
		}
		else if (key == '8')
		{
			panels.group_Selected = 8;
		}
		else if (key == '9')
		{
			panels.group_Selected = 9;
		}
#endif

		else if (key == 'g')
		{
			SHOW_Layout_Gui = !SHOW_Layout_Gui;
		}

		//    else if (key == 's')
		//    {
		//        panels.savePanels();
		//        panels.saveGroups();
		//    }
		//
		//    else if (key == 'l')
		//    {
		//        panels.loadGroups();
		//    }

		//--

		// PRESET CLASS

		//    else if (key == 'z') {
		//        preset_save(PRESET_name);
		//    }

		// LOAD

		if (key == 'l')
		{
			preset_load(PRESET_name);
		}

		// SAVE

		else if (key == 's')
		{
			preset_save(PRESET_name);
		}

		//--

		//-

		//        else if (key == 'g') {
		//            SHOW_ALL_GUI = !SHOW_ALL_GUI;
		//            setVisible(SHOW_ALL_GUI);
		//        }

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

			//            myDEMO_palette.toggleMouseCamera();
		}
		//else if (key == 'd')
		//{
		//    // DEBUG INTERFACE
		//    bShowDebug = !bShowDebug;
		//}

		//-

		// RANDOM USER PALETTE

		// 1. randomize one color and build palette if enabled

		else if (key == 'o')
		{
			//TODO
			// WORKFLOW: when loading a colour lover palette we disable auto create from algo palettes
			//            if (!bAuto_palette_recall)
			//            {
			//                bAuto_palette_recall = true;
			//            }

			//TODO: bug because some trigs are flags. we need direct functions
			//        bRandomColor = true;
			color_picked = ofFloatColor(ofRandom(0., 1.), ofRandom(0., 1.), ofRandom(0., 1.));

			if (bAuto_palette_recall)
			{
				palettes_update();
				palette_recallFromPalettes(SELECTED_palette_LAST);//trig last choice
			}

			// undo
			color_Undo = color_picked.get();
			color_Undo.store();

			// PRESET MANAGER
			if (!MODE_newPreset)
				MODE_newPreset = true;
			textInput_New = "random_" + btns_plt_Selector[SELECTED_palette_LAST]->getName();

			if (TEST_DEMO)
				myDEMO_palette.reStart();
		}

		//--

		// 2. randomize one color and created user palette

		else if (key == 'p')
		{
			//-

			if (!bAuto_palette_recall)
			{
				bAuto_palette_recall = true;
			}

			//-

			//// A. create random palette
			//random.generateRandom(NUM_COLORS_ALGO_PALETTES);
			//palettes_update();
			//if (bAuto_palette_recall)
			//{
			//    //set random palette to user palette
			//    palette_recallFromPalettes(7);
			//}

			//-

			// B. get a random palette from PANTONE
			//int r = (int) ofRandom(NUM_COLORS_PANTONE);
			int r = (int)ofRandom(paletteLibPage * numColorsPage, paletteLibPage * numColorsPage + numColorsPage);
			r = ofClamp(r, 0, NUM_COLORS_PANTONE);
			//color pos & name
			lastColorPicked = r;
			lastColorPickedNameColor = "'" + ColorBrowser.pantoneNames[r] + "'";
			color_picked = ofColor(ColorBrowser_palette[r]);
			palettes_update();
			palette_recallFromPalettes(SELECTED_palette_LAST);

			// undo
			color_Undo = color_picked.get();
			color_Undo.store();

			// PRESET MANAGER
			if (!MODE_newPreset)
				MODE_newPreset = true;
			textInput_New = ColorBrowser.pantoneNames[lastColorPicked] + "_";
			textInput_New += btns_plt_Selector[SELECTED_palette_LAST]->getName();

			//-

			if (TEST_DEMO)
				myDEMO_palette.reStart();
		}

		//-

		// TEST MODE

		if (key == 't')
		{
			TEST_MODE = !TEST_MODE;
		}

		//-

		// DEMO

		else if (key == 'd')
		{
			TEST_DEMO = !TEST_DEMO;
			if (TEST_DEMO)
				myDEMO_palette.reStart();
		}

		//--

		// COLOUR LOVERS

		if (SHOW_ColourLovers)
		{
			// 3. randomly get a palette from colour lovers only
			if (key == 'i')
			{
				ColourLoversHelper.randomPalette();

				textInput_temp = PRESET_name;

				//                if (TEST_DEMO)
				//                    myDEMO_palette.reStart();
			}

			//-

			// browse presets

			else if (key == OF_KEY_DOWN)
			{
				ColourLoversHelper.nextPalette();
				////WORKFLOW
				//if(bAutoExportPreset)
				//{
				//    cout << "EXPORT" << endl;
				//    saveColors();
				//}
			}
			else if (key == OF_KEY_UP)
			{
				ColourLoversHelper.prevPalette();
				////WORKFLOW
				//if(bAutoExportPreset)
				//{
				//    cout << "EXPORT" << endl;
				//    saveColors();
				//}
			}

			//--
		}

		//--

		// UNDO COLOR

		if (key == 'z')
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

		//-

		//    if (key == 's')
		//    {
		//        palette_save("myPalette");
		//    }
		//    if (key == 'l')
		//    {
		//        palette_load("myPalette");
		//    }

		//-

		// USER PALETTE

		else if (key == 'c')
		{
			palette_clear();
		}

		else if (key == 'x')
		{
			// 1. remove last
			//palette_removeColorLast();

			// 2. remove selected
			palette_removeColor(palette_colorSelected);
		}

		//else if (key == 'a')
		//{
		//    color_picked = ofFloatColor(ofRandom(0., 1.), ofRandom(0., 1.), ofRandom(0., 1.));
		//    palette_addColor(ofColor(color_picked.get()));
		//}

		//-

		// COLOR BROWSER

		////    ColorBrowser.keyPressed( eventArgs );
		//
		//    else if (key == ' ')
		//        ColorBrowser.switch_palette_Type();
		//
		//    else if (key == OF_KEY_RETURN)
		//        ColorBrowser.switch_sorted_Type();
	}
}


//--------------------------------------------------------------
void ofxColorManager::keyReleased(ofKeyEventArgs &eventArgs)
{
	if (eventArgs.key == ' ')
	{
	}
}

//--------------------------------------------------------------
void ofxColorManager::windowResized(int w, int h)
{
	// COLOUR LOVERS
	glm::vec2 sizeGui(150, 375);
	glm::vec2 sizeGrid(150, h);
	glm::vec2 posGui(w - (sizeGui.x + sizeGrid.x + 4), 0);
	glm::vec2 posGrid(posGui.x + sizeGui.x + 2, 0);
	ColourLoversHelper.setGrid(posGrid, sizeGrid);
	ColourLoversHelper.setPosition(posGui, sizeGui);
	ColourLoversHelper.windowResized(w, h);
}


//--------------------------------------------------------------
void ofxColorManager::addKeysListeners()
{
	ofAddListener(ofEvents().keyPressed, this, &ofxColorManager::keyPressed);
}


//--------------------------------------------------------------
void ofxColorManager::removeKeysListeners()
{
	ofRemoveListener(ofEvents().keyPressed, this, &ofxColorManager::keyPressed);
}


//--------------------------------------------------------------
void ofxColorManager::mouseDragged(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;
	//    ofLogNotice(__FUNCTION__) << "mouseDragged " <<  x << ", " << y << ", " << button;

	TouchManager::one().touchMove(button, ofVec2f(x, y));

	//-
}


//--------------------------------------------------------------
void ofxColorManager::mousePressed(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;
	//ofLogNotice(__FUNCTION__) << "mousePressed " << x << ", " << y << ", " << button;

	TouchManager::one().touchDown(button, ofVec2f(x, y));

	//-

	// filter touch if its an user palette color box button only
	auto a = TouchManager::one().getComponentUnder(ofVec2f(x, y));
	auto b = a->getName();
	ofLogVerbose(__FUNCTION__) << "touched item: " << b;
	auto str = ofSplitString(b, "btn");

	//check if "btn" label is present to identify item as a color box from user palette
	//instead of being another item like a palette label or others..

	if (str.size() > 1)
	{
		ofLogVerbose(__FUNCTION__) << "detected palette touch: " << b;
		palette_touchedColor(b);
	}
	else
	{
		//ofLogVerbose(__FUNCTION__) << "ignored touch: " << b;
	}

	//-

	// DEMO

	if (TEST_DEMO)
	{
		if (button == 2)//second mouse button cleans DEMO
			myDEMO_palette.clear();
		else
			myDEMO_palette.start();//trig DEMO start
	}
}


//--------------------------------------------------------------
void ofxColorManager::mouseReleased(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;
	//    ofLogNotice(__FUNCTION__) << "mouseReleased " <<  x << ", " << y << ", " << button;

	TouchManager::one().touchUp(button, ofVec2f(x, y));
}


//--------------------------------------------------------------
void ofxColorManager::addMouseListeners()
{
	ofAddListener(ofEvents().mouseDragged, this, &ofxColorManager::mouseDragged);
	ofAddListener(ofEvents().mousePressed, this, &ofxColorManager::mousePressed);
	ofAddListener(ofEvents().mouseReleased, this, &ofxColorManager::mouseReleased);
}


//--------------------------------------------------------------
void ofxColorManager::removeMouseListeners()
{
	ofRemoveListener(ofEvents().mouseDragged, this, &ofxColorManager::mouseDragged);
	ofRemoveListener(ofEvents().mousePressed, this, &ofxColorManager::mousePressed);
	ofRemoveListener(ofEvents().mouseReleased, this, &ofxColorManager::mouseReleased);
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


#pragma mark - SETTINGS

//--------------------------------------------------------------
void ofxColorManager::preset_filesRefresh()
{
	//TODO: why hardcoded?
	ofDirectory dataDirectory(ofToDataPath("user_kits/presets", true));

	// clear files and filenames vectors
	files.clear();
	fileNames.clear();

	// load all folder files in one call
	files = dataDirectory.getFiles();
	for (size_t i = 0; i < files.size(); i++)
	{
		fileNames.push_back(files[i].getBaseName());
	}

	//-

	//TODO
	//void to go to 1st...

	// 1. load same position preset
	// if preset is deleted goes to nextone..
	// should check names because sorting changes..
	if (fileNames.size() > 0)
	{
		if (currentFile > fileNames.size() - 1)
			currentFile = fileNames.size() - 1;
		//else if (currentFile > fileNames.size() - 1)

		PRESET_name = fileNames[currentFile];
		preset_load(PRESET_name);
	}
	else
	{
		ofLogError(__FUNCTION__) << "FILE PRESET NOT FOUND!";
	}

	//// 2. always goes to 1st preset 0
	////that's because saving re sort the files
	////and we don't know the position of last saves preset..
	//if (fileNames.size() > 0)
	//{
	//    currentFile = 0;
	//    PRESET_name = fileNames[currentFile];
	//    preset_load(PRESET_name);
	//}
	//else
	//{
	//    ofLogError(__FUNCTION__) << "NOT FOUND ANY FILE PRESET!";
	//}
}

//--------------------------------------------------------------
void ofxColorManager::preset_load(string p)
{
	if (!bErrorNoFiles) {

		ofLogNotice(__FUNCTION__) << "preset_load " << p;

		// setup linking pointers to get back on load
		myPresetPalette.setName(p);
		myPresetPalette.setCurveName(PRESET_curveName);
		myPresetPalette.setPalette(palette);

		//TODO
		//+curve & gradient

		// load preset
		myPresetPalette.preset_load(p);

		//-

		//TODO
		// apply loaded preset to local system
		vector<ofColor> palette_TEMP = myPresetPalette.getPalette();
		palette_clear();
		for (int i = 0; i < palette_TEMP.size(); i++)
		{
			palette_addColor(palette_TEMP[i]);
		}

		//TODO
		//myPresetPalette.setBackgroundColor(color_backGround);//error ofParameter
		color_backGround = ofColor(myPresetPalette.getBackground());//get directly without pointing

		//TODO
		// curve & gradient
		//        PRESET_curveName = curveName_BACK;
		////        string *name_BACK;
		////        vector<ofColor> *palette_BACK;
		////        string *curveName_BACK;

		//-

		if (TEST_DEMO)
			myDEMO_palette.clear();


		//WORKFLOW
		if (bAutoExportPreset)
		{
			//cout << "EXPORT" << endl;
			saveColors();
		}
	}
}


//--------------------------------------------------------------
void ofxColorManager::preset_save(string p)
{
	ofLogNotice(__FUNCTION__) << p;

	myPresetPalette.setName(p);
	myPresetPalette.setCurveName(PRESET_curveName);
	myPresetPalette.setBackgroundColor(color_backGround.get());
	myPresetPalette.setPalette(palette);

	//myPresetPalette.preset_save(PRESET_name);
	myPresetPalette.preset_save(p);
}

//--------------------------------------------------------------
void ofxColorManager::loadAppSettings(ofParameterGroup &g, string path)
{
	ofLogNotice(__FUNCTION__) << path;
	ofXml settings;
	settings.load(path);
	ofDeserialize(settings, g);

	paletteLibPage = paletteLibPage_param.get();
}


//--------------------------------------------------------------
void ofxColorManager::saveAppSettings(ofParameterGroup &g, string path)
{
	ofLogNotice(__FUNCTION__) << path;
	paletteLibPage_param = paletteLibPage;

	ofXml settings;
	ofSerialize(settings, g);
	settings.save(path);
}


#pragma mark - UTILS

//--------------------------------------------------------------
void ofxColorManager::color_picked_Update_To_HSV()
{
	color_HUE = 255 * color_picked.get().getHue();
	color_SAT = 255 * color_picked.get().getSaturation();
	color_BRG = 255 * color_picked.get().getBrightness();
}

//--------------------------------------------------------------
void ofxColorManager::update_color_picked_CHANGES()
{
	// executes workflow app

	ofLogNotice(__FUNCTION__) << ofToString(color_picked);

	//    // TEST
	//    color_picked_Update_To_HSV();//redundant...

	color_clicked.set(color_picked);//TODO: mirror clicked/picked colors

	//-

	//TODO
	// palettes
	//    primaryColor.set(color_picked.get());
	palettes_colorTheory_update();

	//--

	// 1. autosave edited color

	if (bPaletteEdit && palette_colorSelected != -1 && btns_palette.size() > palette_colorSelected)
	{
		// update user palette color with recently picked color

		// 1. interface buttons
		btns_palette[palette_colorSelected]->setColor(color_clicked);

		// 2. palette vector
		palette[palette_colorSelected].set(color_clicked);

		// 3. update gradient
		if (palette_colorSelected < gradient.getNumColors())
			gradient.replaceColorAtIndex(palette_colorSelected, color_clicked);
	}

	//-

	// 2. WORKFLOW: if enabled, when color picked changes, auto trig and put generated palette to user palette

	if (bAuto_palette_recall)
	{
		palettes_update();
		palette_recallFromPalettes(SELECTED_palette_LAST);//trig last choiced algorithmic palette
	}

	// 3. recreate algorithmic palettes if locking is disable

	if (!bLock_palette)
	{
		palettes_update();
	}

	//-

	//    //TODO
	//    // palettes
	////    primaryColor.set(color_picked.get());
	//    palettes_colorTheory_update();
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
	//
	//if (!SHOW_ALL_GUI)
	//{
	//    SHOW_BrowserColors = SHOW_ALL_GUI;
	//    SHOW_Gradient = SHOW_ALL_GUI;
	//    SHOW_Curve = SHOW_ALL_GUI;
	//    SHOW_AlgoPalettes = SHOW_ALL_GUI;
	//    SHOW_TEST_Curve = SHOW_ALL_GUI;
	//    SHOW_ColourLovers = SHOW_ALL_GUI;
	//    SHOW_ColourLovers_searcher = SHOW_ALL_GUI;
	//}
	//
	//this->guiVisible = SHOW_ALL_GUI;
	////    curve_pos_slider.setVisible(b);
	////    curveMod_Slider.setVisible(b);
}


//--------------------------------------------------------------
ofColor ofxColorManager::getColorAtPercent(float control)
{
	float out = ofMap(curvesTool.getAtPercent(1.0 - control), 0, curveTool_amount - 1, 1., 0.);
	ofColor c = gradient.getColorAtPercent(out);
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
void ofxColorManager::gradient_drawPreview(glm::vec2 pos, bool horizontal = true)
{

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


#pragma mark -  COLOUR LOVERS

//--------------------------------------------------------------
void ofxColorManager::colourLovers_drawPreview()
{
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
		y += 3 * pad + lineH;

		ofDrawBitmapStringHighlight("myColor:", x, y, ofColor::black, ofColor::white);
		y += 3 * pad;

		ofSetColor(myColor);
		ofDrawRectangle(ofRectangle(x, y, w, h));
		y += (h + pad);

		y += (lineH);
		ofDrawBitmapStringHighlight("myPalette:", x, y, ofColor::black, ofColor::white);
		y += 3 * pad;

		for (int i = 0; i < myPalette.size(); i++)
		{
			ofSetColor(myPalette[i]);
			ofDrawRectangle(ofRectangle(x + i * (w + pad), y, w, h));
		}
		y += (h + pad);

		y += (lineH);
		ofDrawBitmapStringHighlight("myPalette_Name:", x, y, ofColor::black, ofColor::white);
		y += (lineH);
		ofDrawBitmapStringHighlight(myPalette_Name, x, y, ofColor::black, ofColor::white);

		ofPopStyle();
	}
}

//--------------------------------------------------------------
void ofxColorManager::saveColors()
{
	////TODO:
	////make export dialog to set path for export outside /data

	////path_Colors = "colors/liveColors.json";
	//path_Colors = "liveColors.json";
	////path_Colors = "/Users/manu/Documents/openFrameworks/addons/ofxFontAnimator/4_ofxFontAnimatorNoise/bin/data/colors/liveColors.json";

	//ofLogNotice(__FUNCTION__) << "path_Colors: " << path_Colors;

	////ofxSerializer
	//ofJson j = palette;
	//ofSavePrettyJson(path_Colors, j);
	//ofLogNotice(__FUNCTION__) << "\n" << ofToString(palette);
}

//--------------------------------------------------------------
void ofxColorManager::generateRange(ofColor col1, ofColor col2) {
	ofLogNotice(__FUNCTION__);

	paletteRange.clear();

	int max = 400;
	int step = max / 10;
	//int step = max / (numCcolorsRange.get() - 1);

	for (auto i = 0; i < 12; ++i)
	{
		glm::vec3 left = { 270, 120 + i * step, 0.0 };
		glm::vec3 right = { 670, 120 + i * step, 0.0 };

		for (auto j = 0; j <= max; j += step)
		{
			glm::vec3 pos = { j + 270, 120 + i * step, 0.0 };
			ofColor color = ofxColorMorph::colorMorph(pos, left, col1, right, col2, static_cast<type>(i));

			paletteRange.push_back(color);
		}
	}

	bRefreshMorph = true;
}
