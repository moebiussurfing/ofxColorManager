#include "ofxColorManager.h"

//TODO:
//--------------------------------------------------------------
void ofxColorManager::dragEvent(ofDragInfo info) {
	colorQuantizer.dragEvent(info);
}

//--------------------------------------------------------------
ofxColorManager::ofxColorManager()
{
	ofAddListener(ofEvents().update, this, &ofxColorManager::update);
	ofAddListener(ofEvents().draw, this, &ofxColorManager::draw);

	//default
	fps = 60.0f;
	dt = 1.f / fps;

	//--

	path_Global = "ofxColorManager/";
	ofxSurfingHelpers::CheckFolder(path_Global);

	path_Folder_Curves = path_Global + "curves/";
	path_Curves = path_Folder_Curves + "curves.yml";
	ofxSurfingHelpers::CheckFolder(path_Global);

	path_Folder_Color = path_Global + "colors/";
	path_Colors = path_Folder_Color + "liveColors.json";
	ofxSurfingHelpers::CheckFolder(path_Global);

	path_AppState = path_Global + "AppSettings.xml";
	//path_AppState = path_Global + "ofxColorManager.xml";
}

//--------------------------------------------------------------
void ofxColorManager::startup()
{
	//--

	ofxSurfingHelpers::loadGroup(params_AppState, path_AppState);

	bResetCurve = true;

	//--

	// edit layout

	ENABLE_keys = true;

	//--

	// a. initialize
	rPreview.setRect(600, 200, 755, 295);

	// b. load settings
	rPreview.loadSettings();

	refresh_Gui_Layout();

	//----

	// preset manager

	preset_refreshFiles();
	//preset_load(PRESET_name);

	//-

	curvesTool.load(path_Curves); //needed because it fills polyline

	//-

//#ifdef USE_RECTANGLE_INTERFACES
//	setVisible_Interface(SHOW_AlgoPalettes);
//#endif

	//----

	// ofxGuiPanelsLayout

	//-

	// add panels to manager
#ifdef INCL_LAYOUT
	panels.addToggle(&SHOW_ImGui);
	panels.addToggle(&SHOW_Panels);

	panels.addToggle(&SHOW_UserPalette);
	panels.addToggle(&SHOW_Picker);
	panels.addToggle(&SHOW_Library);
	panels.addToggle(&SHOW_BackGround);

	panels.addToggle(&SHOW_Theory);
	panels.addToggle(&SHOW_Range);
	panels.addToggle(&SHOW_ColourLovers);
	panels.addToggle(&SHOW_Quantizer);

	panels.addToggle(&SHOW_Curve);
	panels.addToggle(&SHOW_Presets);
	panels.addToggle(&SHOW_PresetsPalette);

	panels.addToggle(&SHOW_ALL_GUI);
	panels.addToggle(&SHOW_GUI_MINI);

	panels.addToggle(&SHOW_Demo);
	panels.addToggle(&DEMO_Test);

	//panels.addToggle(&SHOW_ColourLovers_searcher);
	//panels.addToggle(&SHOW_AlgoPalettes);
	//panels.addToggle(&SHOW_BrowserColors);
	//panels.addToggle(&SHOW_Gradient);
	//panels.addToggle(&SHOW_debugText);
	//panels.addToggle(&SHOW_CosineGradient);

	//call after add the panels
	panels.setup();

	//----

	//workflow
	panels.group_Selected = 0;
#endif

	//--

	////workflow
	//colourLoversHelper.nextPalette();

	//--
}

//--------------------------------------------------------------
void ofxColorManager::setup()
{
	//ofSetLogLevel("ofxColorManager", OF_LOG_NOTICE);

	sizeLibColBox.set("Size Lb", 25, 10, 100);
	bLibFillMode.set("Responsive", false);
	bPagerized.set("Mode Paging", false);

	sizePaletteBox.set("Size Pt", 25, 10, 500);
	bPaletteFillMode.set("Responsive", false);

	//-

	//display text
	std::string strFont;
	std::string _p = "assets/fonts/";

	strFont = _p + "GTAmerica-ExtendedBlack.ttf";
	//strFont = _p + "GTAmerica-ExpandedBold.ttf";
	//strFont = _p + "telegrama_render.otf";
	//strFont = _p + "PragmataProB_0822.ttf";
	//strFont = _p + "Kazesawa-Extrabold.ttf";

	fontBig.load(strFont, 70);
	fontMedium.load(strFont, 28);
	fontSmall.load(strFont, 22);

	txt_lineActive[0] = false;
	txt_lineActive[1] = false;
	txt_lineActive[2] = false;
	txt_lineActive[3] = false;

	//-

	////cosine gradient
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

#ifdef USE_IMAGE_QUANTIZER
	colorQuantizer.setup();

	// receivers pointers
	colorQuantizer.setPalette_BACK(myPalette);
	colorQuantizer.setPalette_BACK_RefreshPalette(bUpdated_Palette_BACK);
	colorQuantizer.setPalette_BACK_Name(myPalette_Name);
#endif

	//--

	// theory

	setup_Theory();

	//--

	myPresetManager.setup();

	//--

	// DEMO

	myDEMO.setup();
	//myDEMO.setEnableMouseCamera(true);
	myDEMO.setPalette(palette);

	//--

	// mouse debugger

	mouseRuler.setup();
	mouseRuler.toggleVisibility();

	//-

	// colour lovers

#ifdef USE_COLOR_LOVERS
// set positions and panel sizes
	//glm::vec2 sizeGui(150, 375);
	//glm::vec2 sizeGrid(150, ofGetHeight());
	//glm::vec2 posGui(ofGetWidth() - (sizeGui.x + sizeGrid.x + 4), 0);
	//glm::vec2 posGrid(posGui.x + sizeGui.x + 2, 0);

	//// must be called before setup() to overwrite default settings
	//colourLoversHelper.setGrid(posGrid, sizeGrid);
	//colourLoversHelper.setup(posGui, sizeGui);

	colourLoversHelper.setup();

	//-

	// pointers back

	colourLoversHelper.setColor_BACK(myColor);
	colourLoversHelper.setColor_BACK_Refresh(bUpdated_Color_BACK);
	colourLoversHelper.setPalette_BACK(myPalette);
	colourLoversHelper.setPalette_BACK_Name(myPalette_Name);
	colourLoversHelper.setPalette_BACK_Refresh(bUpdated_Palette_BACK);

	listener_LoverName = colourLoversHelper.lastPaletteName.newListener([this](std::string &n) {
		txt_lineActive[0] = false;//preset name
		txt_lineActive[1] = true;//palette name
		txt_lineActive[2] = false;//theory name
		txt_lineActive[3] = false;//range name
		});

#endif

	// some initiation values..
	myColor = ofColor::white;
	myPalette.resize(2);//pointer setter will clear/resize. nevermind the vector size here
	myPalette[0] = ofColor::white;
	myPalette[1] = ofColor::white;
	//myPalette[0] = ofColor::white;
	myPalette_Name = " ";

	//-

	// layout

	refresh_Gui_Layout();

	//-

	// color browser

	//ColorBrowser.setBoxSize(7);
	//ColorBrowser.setRowsSize(7 * 17);//for Pantone palette
	ColorBrowser.setup_colorBACK(color_BACK);
	ColorBrowser.setPosition(colorBrowserPos);
	ColorBrowser.setup();
	ColorBrowser_palette = ColorBrowser.getPalette();

	//-

	// general data

	setBackground_ENABLE(true);
	color_backGround.set("BG", ofColor(8));
	color_backGround_SET.set("Refresh", false);
	backgroundDarkness.set("Darkerkness", 0.5, 0., 1.);
	color_backGround_Darker.set("Darker", false);
	color_background_AutoSet.set("AutoSet", true);
	params_data.setName("GENERAL");
	params_data.add(color_backGround);
	params_data.add(color_backGround_SET);
	params_data.add(color_backGround_Darker);
	params_data.add(color_background_AutoSet);

	//--

	// color

	color_Picked.set("COLOR", ofFloatColor::red);

	color_HUE.set("H", 0, 0, 255);
	color_SAT.set("S", 0, 0, 255);
	color_BRG.set("B", 0, 0, 255);

	params_color.setName("COLOR");
	params_color.add(color_HUE);
	params_color.add(color_SAT);
	params_color.add(color_BRG);

	// randomizer
	bColor_HUE.set("HUE", true);
	bColor_BRG.set("BRG", true);
	bColor_SAT.set("SAT", true);

	color_HUE_0.set("H 0", 0, 0, 255);
	color_SAT_0.set("S 0", 0, 0, 255);
	color_BRG_0.set("B 0", 0, 0, 255);
	color_HUE_Power.set("H Power", 0.25, 0, 1);
	color_SAT_Power.set("S Power", 0.25, 0, 1);
	color_BRG_Power.set("B Power", 0.25, 0, 1);

	ofAddListener(params_color.parameterChangedE(), this, &ofxColorManager::Changed_Controls);

	//-

	// algorithmic palette

	MODE_TweakSatBrg.set("Tweak Sat/Brt", false);
	SATURATION.set("SATURATION", 200, 0, 255);
	BRIGHTNESS.set("BRIGHTNESS", 200, 0, 255);
	bAuto_palette_recall.set("Auto Build", false);
	bLock_palette.set("Lock Palettes", false);
	amountColors_Alg.set("Amnt Colors", 6, 2, 10);
	params_palette.setName("ADVANCED");
	params_palette.add(MODE_TweakSatBrg);
	params_palette.add(SATURATION);
	params_palette.add(BRIGHTNESS);
	params_palette.add(amountColors_Alg);
	params_palette.add(bAuto_palette_recall);
	params_palette.add(bLock_palette);
	//bRandomPalette.set("RANDOM PALETTE", false);
	//params_palette.add(bRandomPalette);

	//-

	//exclude
	DEMO_Cam.setSerializable(false);
	SHOW_AlgoPalettes.setSerializable(false);
	SATURATION.setSerializable(false);
	BRIGHTNESS.setSerializable(false);
	color_backGround.setSerializable(false);
	color_Picked.setSerializable(false);
	color_HUE.setSerializable(false);
	color_SAT.setSerializable(false);
	color_BRG.setSerializable(false);
	color_HUE_0.setSerializable(false);
	color_SAT_0.setSerializable(false);
	color_BRG_0.setSerializable(false);
	//bColor_SAT.setSerializable(false);

	ofAddListener(params_palette.parameterChangedE(), this, &ofxColorManager::Changed_Controls);

	//TODO: add ColorWheelSchemes alternative

	//-

	// INTERFACE
#ifdef USE_RECTANGLE_INTERFACES
	setup_Interface_Scene();
#endif

	//-

	// ALGORITHMIC PALETTES

	//random.generateRandom(amountColors_Alg);
	update_Engine();
	setup_Interface();
	setup_Labels();

	//-

	// gradient

	gradient.reset();
	//gradient.setupAsTurbo(10); //with 10 samples
	gradient_HardMode.set("Hard Steps", false);
	gradient.setHardMode(gradient_HardMode);
	gradient.setDrawVertical(true);
	gradient.setDrawDirFlip(true);

	params_curve.setName("GRADIENT CURVE");
	params_curve.add(MODE_Editor);
	params_curve.add(gradient_HardMode);

	//-

	// CURVE 

	setup_CurveTool();

	//-

	// CONTROL

	//SHOW_PaletteCustom.set("SHOW PALETTE LIB", false);
	bRandomColor.set("RANDOM COLOR", false);
	bAddColor.set("ADD COLOR", false);
	bPaletteEdit.set("EDIT COLOR", false);
	bRemoveColor.set("REMOVE COLOR", false);
	bClearPalette.set("CLEAR PALETTE", false);
	params_control.setName("COLOR EDITOR");

	params_control.add(bRandomColor);
	params_control.add(bPaletteEdit);
	params_control.add(bAddColor);
	params_control.add(bRemoveColor);
	params_control.add(bClearPalette);
	//params_control.add(color_Picked);
	//params_control.add(SHOW_PaletteCustom);
	//params_control.add(color_backGround);

	//-

	// CONTROL WINDOWS

	SHOW_ALL_GUI.setName("GUI MAIN");
	SHOW_GUI_MINI.setName("GUI MINI");
	SHOW_UserPalette.setName("PALETTE");
	SHOW_Theory.setName("THEORY");
	SHOW_debugText.setName("SHOW debug");
	SHOW_Quantizer.setName("PICTURE");

	//-

	//TODO: delete
	SHOW_AlgoPalettes.set("SHOW PALETTES", true);

	//TODO: should delete
	SHOW_BrowserColors.set("SHOW BROWSER COLORS", false);
	SHOW_BrowserColors.setSerializable(false);

	//SHOW_CosineGradient.set("SHOW COSINE GRADIENT", true);

	SHOW_ColourLovers_searcher.set("SHOW COLOUR LOVERS SEARCH", true);
	SHOW_ColourLovers.set("LOVERS", true);
	SHOW_Gradient.set("GRADIENT", true);
	SHOW_Curve.set("CURVE", true);
	MODE_Editor.set("Edit Layout", false);
	MODE_Editor.setSerializable(false);

	SHOW_Presets.set("PRESETS", true);
	SHOW_PresetsPalette.set("Show Palette", false);
	SHOW_BackGround.set("BACKGROUND", true);
	SHOW_Picker.set("PICKER", true);
	SHOW_Library.set("LIBRARY", false);
	SHOW_Range.set("RANGE", true);
	SHOW_Panels.set("PANELS", true);
	SHOW_Demo.set("DEMO", false);

	params_control.add(SHOW_Panels);
	params_control.add(SHOW_Demo);
	params_control.add(SHOW_BrowserColors);
	params_control.add(SHOW_Curve);
	params_control.add(MODE_Editor);
	params_control.add(SHOW_Presets);
	params_control.add(SHOW_PresetsPalette);
	params_control.add(SHOW_BackGround);
	params_control.add(SHOW_Library);

	params_control.add(SHOW_Theory);
	params_control.add(SHOW_Range);
	params_control.add(SHOW_Picker);
	params_control.add(SHOW_ColourLovers);
	params_control.add(SHOW_Quantizer);

	params_control.add(SHOW_ColourLovers_searcher);
	params_control.add(SHOW_UserPalette);
	params_control.add(SHOW_AlgoPalettes);
	//params_control.add(SHOW_Gradient);
	//params_control.add(SHOW_CosineGradient);

	//-

	rangeScale.set("Scale Rg", 1.f, 0.25f, 1.25f);

	//-

	//palette

	boxSizeUser.set("Box Size", 40, 10, 200);
	boxRowsUser.set("Max Rows", 10, 1, 20);
	boxScale.set("Scale Pt", 1.f, 0.25f, 1.25f);
	//bEditUserPalette.set("EDIT", false);
	//bUserPaletteVertical.set("VERTICAL", false);
	bFlipUserPalette.set("FLIP", false);

	params_UserPalette.setName("USER PALETTE");
	//params_UserPalette.add(bEditUserPalette);
	//params_UserPalette.add(bUserPaletteVertical);
	params_UserPalette.add(bFlipUserPalette);
	//params_UserPalette.add(boxSizeUser);
	params_UserPalette.add(boxRowsUser);
	params_UserPalette.add(boxScale);

	ofAddListener(params_UserPalette.parameterChangedE(), this, &ofxColorManager::Changed_ColorUserPalette);

	//-

	// color ranges

	c1_Rng.set("Color 1", ofColor::orangeRed, ofColor(0), ofColor(255));
	c2_Rng.set("Color 2", ofColor::darkBlue, ofColor(0), ofColor(255));
	guiCol1 = c1_Rng.get();
	guiCol2 = c2_Rng.get();

	types = { "RGB", "HSL", "HSV ", "HSB", "LUV ", "LAB", "HLAB", "LCH", "CMY", "CMYK", "YXY", "XYZ" };//12 types
	bRefreshMorph = false;

	generateRange(guiCol1, guiCol2);

	bRangeAutoGenerate.set("Auto Generate", true);
	autoPickColor1.set("Auto Pick C1", true);
	autoPickColor2.set("Auto Pick C2", false);
	bGetPaletteFromRange.set("To User Palette", false);
	numColorsRange.set("Amnt Colors", 11, 4, 20);

	params_rangTypes.setName("rangTypes");
	for (int i = 0; i < types.size(); i++) //12
	{
		rangTypes[i].set(ofToString(types[i]), false);
		params_rangTypes.add(rangTypes[i]);
	}
	params_rangTypes.add(numColorsRange);
	params_rangTypes.add(autoPickColor1);
	params_rangTypes.add(autoPickColor2);
	params_rangTypes.add(bGetPaletteFromRange);

	ofAddListener(params_rangTypes.parameterChangedE(), this, &ofxColorManager::Changed_ColorRange);

	//-

	//extra algo
	std::string name;
	params_algoTypes.setName("AlgoTypes");
	for (int i = 0; i < 7; i++)
	{
		switch (i)
		{
		case 0:
			name = "Complement Saturation";
			break;
		case 1:
			name = "Complement Brightness";
			break;
		case 2:
			name = "Monochrome Saturation";
			break;
		case 3:
			name = "Monochrome Brightness";
			break;
		case 4:
			name = "Analogue";
			break;
		case 5:
			name = "Triad";
			break;
		case 6:
			name = "Complement Triad";
			break;
		}
		algoTypes[i].set(name, false);
		params_algoTypes.add(algoTypes[i]);
	}

	ofAddListener(params_algoTypes.parameterChangedE(), this, &ofxColorManager::Changed_ColorRange);

	//-

	SHOW_ALL_GUI = true;
	SHOW_debugText = false;
	SHOW_GUI_MINI = false;

	//-

	// background

	params_control.add(color_backGround_SET);
	params_control.add(color_background_AutoSet);
	params_control.add(color_backGround_Darker);
	params_control.add(backgroundDarkness);

	params_control.add(SHOW_ALL_GUI);

	//-

	ofAddListener(params_control.parameterChangedE(), this, &ofxColorManager::Changed_Controls);

	//--

	// GUI

#ifdef INCLUDE_IMGUI_CUSTOM_THEME_AND_FONT
	ofxSurfingHelpers::ImGui_FontCustom();
#endif

	// create
	gui.setup();

	// theme customize
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::GetIO().MouseDrawCursor = false;
	//ImGui::GetIO().ConfigWindowsResizeFromEdges = true;

#ifdef INCLUDE_IMGUI_CUSTOM_THEME_AND_FONT
	//ofxSurfingHelpers::ImGui_ThemeMoebiusSurfing();
	ofxSurfingHelpers::ImGui_ThemeModernDark();
#endif

	mainSettings = ofxImGui::Settings();

	//-

	guiVisible = true;

	//--

	// listeners

	addKeysListeners();
	addMouseListeners();

	color_Picked.addListener(this, &ofxColorManager::Changed_ColorPicked);
	color_Clicked.addListener(this, &ofxColorManager::Changed_ColorClicked);
	
	//------------------------------------------------------

	// startup settings

	params_AppState.setName("ofxColorManager");

	params_AppState.add(SHOW_Presets);
	params_AppState.add(SHOW_PresetsPalette);
	params_AppState.add(SHOW_BackGround);
	params_AppState.add(SHOW_Library);
	params_AppState.add(SHOW_Picker);
	params_AppState.add(SHOW_Range);
	params_AppState.add(SHOW_Theory);
	params_AppState.add(SHOW_Panels);
	params_AppState.add(SHOW_Demo);
	params_AppState.add(SHOW_Quantizer);
	params_AppState.add(SHOW_ColourLovers);
	params_AppState.add(SHOW_AlgoPalettes);
	params_AppState.add(SHOW_Curve);
	params_AppState.add(SHOW_BrowserColors);
	params_AppState.add(SHOW_GuiInternal);
	params_AppState.add(SHOW_GUI_MINI);
	params_AppState.add(MODE_Editor);

	//params_AppState.add(SHOW_ColourLovers_searcher);
	//params_AppState.add(SHOW_Gradient);
	//params_AppState.add(SHOW_CosineGradient);

	params_AppState.add(color_Picked);
	params_AppState.add(color_backGround);
	params_AppState.add(color_backGround_Darker);
	params_AppState.add(color_background_AutoSet);
	params_AppState.add(backgroundDarkness);

	params_AppState.add(MODE_TweakSatBrg);//algorithmic palette
	params_AppState.add(amountColors_Alg);
	params_AppState.add(bPaletteEdit);//user palette
	params_AppState.add(bAuto_palette_recall);
	params_AppState.add(SHOW_UserPalette);

	params_AppState.add(TEST_Mode);

	params_AppState.add(DEMO_Test);
	params_AppState.add(DEMO_Auto);
	params_AppState.add(DEMO_Timer);
	params_AppState.add(DEMO_Alpha);
	params_AppState.add(DEMO_Cam);

	params_AppState.add(gradient_HardMode);//gradient
	params_AppState.add(bAuto_palette_recall);
	//params_AppState.add(paletteLibPage_param);

	params_AppState.add(BRIGHTNESS);
	params_AppState.add(SATURATION);
	params_AppState.add(bColor_HUE);
	params_AppState.add(bColor_SAT);
	params_AppState.add(bColor_BRG);
	params_AppState.add(color_HUE_0);
	params_AppState.add(color_SAT_0);
	params_AppState.add(color_BRG_0);
	params_AppState.add(color_HUE_Power);
	params_AppState.add(color_SAT_Power);
	params_AppState.add(color_BRG_Power);

	params_AppState.add(paletteLibPage);
	params_AppState.add(pantoneMaxColumns);
	params_AppState.add(numLibLines);
	params_AppState.add(bPantoneCards);
	params_AppState.add(pantoneScale);
	params_AppState.add(rangeScale);
	params_AppState.add(bLibFillMode);
	params_AppState.add(bPagerized);
	params_AppState.add(sizeLibColBox);

	params_AppState.add(bPaletteFillMode);
	params_AppState.add(sizePaletteBox);
	params_AppState.add(boxRowsUser);
	params_AppState.add(boxScale);

	//------------------------------------------------

	// startup

	startup();
}

//--------------------------------------------------------------
void ofxColorManager::update(ofEventArgs & args)
{
	//-

//	// window title
//#ifdef INCL_LAYOUT
//	std::string str;
//	str += ("[PAGE " + ofToString(panels.group_Selected) + "] ");
//	//str += ofToString((int)ofGetFrameRate()) + "FPS";
//	ofSetWindowTitle(str);
//#endif

	//-

	////cosine gradient
	//cosineGradient_update();

	//--

	// preset manager

	myPresetManager.update();

	//--

	// ofxGuiPanelsLayout

#ifdef INCL_LAYOUT
	panels.update();
#endif

	//--

	// DEMO

	if (DEMO_Test) myDEMO.update();

	//---

	// TEST CURVE

	int frameBuffer = (int)ofMap(TEST_Speed, 0., 1., TEST_maxFrames, 30);
	int frameCurrent = ofGetFrameNum() % frameBuffer;//0 to maxFrames

	if (TEST_Mode)
	{
		if (!bTEST_pause)
		{
			framePrc = ofMap(frameCurrent, 0, frameBuffer, 0., 1.);
			float control;
			if (!TEST_LFO_Mode) control = ofClamp(framePrc, 0., 1.);
			else
			{
				float mySin = std::sin(PI * framePrc);
				control = ofClamp(mySin, 0., 1.);
			}
			if (TEST_toBackground) color_backGround.set(getColorAtPercent(control));

			setControl(control);
		}

		if (frameCurrent == frameBuffer - 1)
		{
			bTEST_pause = !bTEST_pause;

			//round end position to clamp
			float control;
			if (!TEST_LFO_Mode)
			{
				control = 1.;
				framePrc = 1.;
			}
			else
			{
				control = 0.;
				framePrc = 1.;
			}
			if (TEST_toBackground) color_backGround.set(getColorAtPercent(control));

			setControl(control);
		}
	}

	//--

	update_Curve();

	//--

	// colour lovers

#ifdef USE_COLOR_LOVERS
	colourLoversHelper.update();
#endif

	//--

	// DEMO

	if (DEMO_Test && DEMO_Auto)
		if ((ofGetElapsedTimeMillis() - Demo_Timer) > MAX(Demo_Timer_Max * (1 - DEMO_Timer), 10)) {
			Demo_Timer = ofGetElapsedTimeMillis();

			myDEMO.start();
		}

	//--

	// 1. colour lover palette has been clicked/changed selected

	if (bUpdated_Palette_BACK)
	{
		bUpdated_Palette_BACK = false;
		ofLogNotice(__FUNCTION__) << "bUpdated_Palette_BACK: " << bUpdated_Palette_BACK;

		// TODO: WORKFLOW: if mode is palette&color should load first palette color
		// and forget the clicked color

		// 1. get palette colors from colour lovers
		palette_load_FromColourLovers();

		//-

		// WORKFLOW: when loading a color lover palette we disable auto create from algo palettes
		if (bAuto_palette_recall)
		{
			bAuto_palette_recall = false;
		}

		//-

		// presets

		if (!MODE_newPreset) MODE_newPreset = true;
		textInput_New = myPalette_Name;

		//-

		// DEMO

		if (DEMO_Test) myDEMO.reStart();

		//-

		//workflow
		if (bAutoExportPreset)
		{
			ofLogNotice(__FUNCTION__) << "bAutoExportPreset: " << bAutoExportPreset;
			//ofLogNotice(__FUNCTION__) << "EXPORT";

			saveColors();
		}
	}

	//-

	// 2. color pick from palette clicked

	if (bUpdated_Color_BACK)
	{
		bUpdated_Color_BACK = false;
		ofLogWarning(__FUNCTION__) << "bUpdated_Color_BACK: " << bUpdated_Color_BACK;

#ifdef USE_COLOR_LOVERS
		// WORKFLOW: 
		//TODO: disable to avoid overwrite the selected color into the palette just created
		if (colourLoversHelper.MODE_PickPalette_BACK && colourLoversHelper.MODE_PickColor_BACK)
		{
			// WORKFLOW: auto disable edit mode
			if (bPaletteEdit)
			{
				bPaletteEdit = false;
			}
		}
#endif

		// 2. get color from colour lovers
		color_clicked = ofColor(myColor);

#ifdef USE_COLOR_LOVERS
		// 3. auto create user palette from algo palette from colour lover picked color
		if (!colourLoversHelper.MODE_PickPalette_BACK && colourLoversHelper.MODE_PickColor_BACK)
		{
			if (bAuto_palette_recall)
			{
				palette_clear();

#ifdef USE_RECTANGLE_INTERFACES
				palette_recallFromPalettes(SELECTED_palette_LAST);//trig last choice
#endif
			}
		}
#endif

		if (!MODE_newPreset) MODE_newPreset = true;

		// DEMO
		if (DEMO_Test) myDEMO.reStart();
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
#ifdef USE_RECTANGLE_INTERFACES
		palette_recallFromPalettes(SELECTED_palette);
#endif

		SELECTED_palette_LAST = SELECTED_palette;//memorize what was the last trigged palette
		SELECTED_palette_PRE = SELECTED_palette = -1;//disable to stop check..
		//bug if not if pressed same button

		//-

#ifdef USE_RECTANGLE_INTERFACES

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
#endif

		//-

		// presets 

		if (!MODE_newPreset) MODE_newPreset = true;

#ifdef USE_RECTANGLE_INTERFACES
		textInput_New = btns_plt_Selector[SELECTED_palette_LAST]->getName();
#endif

		//-

		// DEMO

		if (DEMO_Test) myDEMO.reStart();
	}

	//-

	//TODO:
	//not used and hidden
	// 3. color clicked
	if (color_clicked != color_clicked_PRE)
	{
		color_clicked_PRE = color_clicked;
		ofLogNotice(__FUNCTION__) << "CHANGED color_clicked";
		color_Picked.set(color_clicked);
	}

	//-

	// 4. set the local scope color pointer that is into ofxColorBrowser that whill be used as color picked too

	if (color_BACK != color_BACK_PRE)
	{
		color_BACK_PRE = color_BACK;
		ofLogNotice(__FUNCTION__) << "CHANGED color_BACK pointer";
		color_Picked.set(color_BACK);

		if (!bPaletteEdit)
		{
			if (bAuto_palette_recall)
			{
				update_Engine();

#ifdef USE_RECTANGLE_INTERFACES
				palette_recallFromPalettes(SELECTED_palette_LAST);//trig last choice
#endif
			}
		}
	}

	//-

//#ifdef USE_RECTANGLE_INTERFACES
//	//update_Interface();
//#endif

	//-

	//ColorBrowser.update();

	//-

	update_CurveTool();

	if (MODE_Editor)
	{
		refresh_Gui_Layout();
	}
}

//--------------------------------------------------------------
void ofxColorManager::update_Curve()
{
	float out = ofMap(curvesTool.getAtPercent(1.0 - curve_Ctrl_In), 0, cAmt - 1, 1, 0);
	colCurveTest = gradient.getColorAtPercent(out);
}

//--------------------------------------------------------------
void ofxColorManager::draw_Curve()
{
	gui_Curve();

	//-

	ofColor c;

	//-

	if (SHOW_Curve)
	{
		// 1. draggable bg box

		ofPushStyle();
		ofFill();
		ofSetColor(ofColor(0, 220));
		rPreview.draw();
		ofDrawRectRounded(rPreview, 5);
		ofPopStyle();

		//-

		ofPushStyle();

		//-

		// 1. gradiented color

		ofRectangle rCurrCol(currColor_x, currColor_y, box_size_user * 2.0, curveTool_h);

		//-

		// 2. current box color at input curve point (right positioned)
		c.set(colCurveTest);

		//-

		// filled
		ofFill();

		ofSetColor(c);
		ofDrawRectangle(rCurrCol);

		//-

		//// 3. top progress bar prc
		//if (DEMO_Test)
		//{
		//	int _lw = 4;
		//	ofRectangle rProgress(0, 0, framePrc * ofGetWidth(), _lw);
		//	ofSetColor(ofColor(ofColor::black, 200));
		//	ofDrawRectangle(rProgress);
		//}

		//-

		// border
		ofNoFill();
		ofSetLineWidth(2);
		ofSetColor(ofColor(255, 225));

		ofDrawRectangle(rCurrCol);

		//-

		ofPopStyle();

		//-

		draw_CurveTools();
	}

	//--

	////cosine gradient
	//if (SHOW_Gradient) draw_Gradient();
}

//--------------------------------------------------------------
void ofxColorManager::draw_Info()
{
	ofPushMatrix();
	ofPushStyle();

	//-

	int pady = 50;
	int padh;
	int sp = 3;
	int x;
	int y;
	int h;

	std::string t0 = PRESET_name;
	std::string t1 = myPalette_Name;
	std::string t2 = theory_Name;
	std::string t3 = range_Name;

	float _w0 = ofxSurfingHelpers::getWidthBBtextBoxed(fontBig, t0);
	float _w1 = ofxSurfingHelpers::getWidthBBtextBoxed(fontBig, t1);
	float _w2 = ofxSurfingHelpers::getWidthBBtextBoxed(fontMedium, t2);
	float _w3 = ofxSurfingHelpers::getWidthBBtextBoxed(fontSmall, t3);

	int i = 0;

	y = pady + fontBig.getSize();

	ofColor c0, c1, c0_Ghost, c1_Ghost;

	c0.set(0, 200);
	c1.set(255, 200);

	//c0_Ghost.set(0, 0);
	//c1_Ghost.set(255, 0);
	c0_Ghost.set(0, 32);
	c1_Ghost.set(255, 32);

	//TODO: create double line shadow draw methoid into ofxSurfingHelpers

	padh = 15;
	h = padh;
	y += h;

	if (t0 != "" || txt_lineActive[0]) {//preset name
		x = ofGetWidth() * 0.5 - _w0 * 0.5;
		if (txt_lineActive[i]) ofSetColor(c0);
		else ofSetColor(c0_Ghost);
		fontBig.drawString(t0, x + sp, y + sp);
		if (txt_lineActive[i]) ofSetColor(c1);
		else ofSetColor(c1_Ghost);
		fontBig.drawString(t0, x, y);

		i++;
		padh = 25;
		h = fontBig.getSize() + padh;
		y += h;
	}

	if (t1 != "" || txt_lineActive[1]) {//palette name
		x = ofGetWidth() * 0.5 - _w1 * 0.5;
		if (txt_lineActive[i]) ofSetColor(c0);
		else ofSetColor(c0_Ghost);
		fontBig.drawString(t1, x + sp, y + sp);
		if (txt_lineActive[i]) ofSetColor(c1);
		else ofSetColor(c1_Ghost);
		fontBig.drawString(t1, x, y);

		i++;
		padh = -10;
		h = fontBig.getSize() + padh;
		y += h;
	}

	if (t2 != "" || txt_lineActive[2]) {//theory name
		x = ofGetWidth() * 0.5 - _w2 * 0.5;
		if (txt_lineActive[i]) ofSetColor(c0);
		else ofSetColor(c0_Ghost);
		fontMedium.drawString(t2, x + sp, y + sp);
		if (txt_lineActive[i]) ofSetColor(c1);
		else ofSetColor(c1_Ghost);
		fontMedium.drawString(t2, x, y);

		i++;
		padh = 20;
		h = fontMedium.getSize() + padh;
		y += h;
	}

	if (t3 != "" || txt_lineActive[3]) {//range name
		x = ofGetWidth() * 0.5 - _w3 * 0.5;
		if (txt_lineActive[i]) ofSetColor(c0);
		else ofSetColor(c0_Ghost);
		fontSmall.drawString(t3, x + sp, y + sp);
		if (txt_lineActive[i]) ofSetColor(c1);
		else ofSetColor(c1_Ghost);
		fontSmall.drawString(t3, x, y);
	}

	//-

	ofPopStyle();
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofxColorManager::draw(ofEventArgs & args)
{
	// background

	if (background_Draw_ENABLE)
	{
		if (color_background_AutoSet.get())// && SHOW_Curve.get()) 
		{
			ofClear(colCurveTest);

			//workflow
			color_backGround.set(colCurveTest);
		}
		else
		{
			ofClear(ofColor(color_backGround.get()));
		}
	}

	//--

	//// cosine gradient
	//if (SHOW_CosineGradient)
	//{
	//    mCosineGradient.render(glm::vec2(0), ofGetWidth(), ofGetHeight());
	//}

	//--

	////TODO: 
	////BUG: startup..
	////if (ENABLE_keys)
	//if (mouseOverGui)
	//{
	//    ofClear(ofColor::red);
	//}
	//else
	//{
	//    ofClear(ofColor::blue);
	//}

	//--

	// presets

	//if (SHOW_Presets)
	//{
	//	myPresetManager.draw();
	//	//myPresetPalette.draw();
	//}

	//--

	// DEMO

	if (DEMO_Test) myDEMO.draw(DEMO_Alpha);

	//--

	// colors browser

	//if (SHOW_BrowserColors) ColorBrowser.draw();

	//--

	if (SHOW_ImGui)
	{
		//TODO: BUG: 
		//solve startup bug that disables keys
		//if (ENABLE_keys != ENABLE_keys_PRE)
		//{
		//	ENABLE_keys_PRE = ENABLE_keys;

		//--

		// quantizer

#ifdef USE_IMAGE_QUANTIZER
		if (SHOW_Quantizer) colorQuantizer.draw();
#endif
		//--

		//TODO
		//if (SHOW_ColourLovers || SHOW_ColourLovers_searcher)
		//    colourLoversHelper.setEnableKeys(false);

		//--

		////TODO: 
		//disables keys when using colour lovers
		//if (SHOW_ColourLovers || SHOW_ColourLovers_searcher)
		//{
		//    ENABLE_keys = false;
		//}

		//--

		draw_Info();
	}

	//--

	// mini

	if (SHOW_GUI_MINI) draw_Mini();

	//--

	// ofxGuiPanelsLayout

#ifdef INCL_LAYOUT
	if (SHOW_GuiInternal) panels.draw();
#endif

	//--

	// GUI

	if (SHOW_ImGui)
	{
		if (guiVisible)
		{
			mouseOverGui = gui_Draw();
			mouseOverGui = ofxImGui::IsMouseOverGui();

			if (mouseOverGui != mouseOverGui_PRE)
			{
				mouseOverGui_PRE = mouseOverGui;

				ofLogVerbose(__FUNCTION__) << "mouseOverGui: " << (mouseOverGui ? " TRUE" : " FALSE");

				if (mouseOverGui)
				{
					ENABLE_keys = false;
				}
				else
				{
					ENABLE_keys = true;
				}

				//myDEMO.setEnableMouseCamera();
			}
		}
	}

	//--

	// COLOR BOX PICKER (CURRENT)

	//ofPushStyle();
	//ofFill();
	//ofSetColor(ofColor( color_Picked.get() ));
	//ofDrawRectangle(r_color_picked);
	//ofPopStyle();

	//--

	// COLOR BOX CLICKED

	//ofPushStyle();
	//ofFill();
	//
	//ofSetColor( ofColor( color_clicked ) );
	////ofSetColor( ofColor( color_Clicked.get() ) );
	//
	//ofDrawRectangle(r_color_clicked);
	//ofPopStyle();

	//--

	//// interface
	//#ifdef USE_RECTANGLE_INTERFACES
	////draw_Interface_Scene();
	//#endif

	//--

	////TODO:
	//std::string strKeys = "COLOUR LOVERS KEYS: ";
	//strKeys += (colourLoversHelper.ENABLER_Keys ? "ENTER/UNFOCUS":"FOCUS/LOAD");
	//std::string strKeys2 = "MOUSE OVER GUI: ";
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
	ofLogNotice(__FUNCTION__);

	//palette_save("myPalette");

	//app state
	ofxSurfingHelpers::saveGroup(params_AppState, path_AppState);

	//-

	curvesTool.save(path_Curves); //needed because it fills polyline


	//-

	//gui layout
	rPreview.saveSettings();

	//-

	ColorBrowser.exit();

#ifdef USE_COLOR_LOVERS
	colourLoversHelper.exit();
#endif

	//-

	ofRemoveListener(params_control.parameterChangedE(), this, &ofxColorManager::Changed_Controls);
	ofRemoveListener(params_color.parameterChangedE(), this, &ofxColorManager::Changed_Controls);
	ofRemoveListener(params_palette.parameterChangedE(), this, &ofxColorManager::Changed_Controls);
	ofRemoveListener(params_curve.parameterChangedE(), this, &ofxColorManager::Changed_Controls);

	ofRemoveListener(params_rangTypes.parameterChangedE(), this, &ofxColorManager::Changed_ColorRange);
	ofRemoveListener(params_algoTypes.parameterChangedE(), this, &ofxColorManager::Changed_ColorRange);

	ofRemoveListener(params_ColorTheory.parameterChangedE(), this, &ofxColorManager::Changed_ColorTheory);
	ofRemoveListener(params_UserPalette.parameterChangedE(), this, &ofxColorManager::Changed_ColorUserPalette);

	//--

	// ofxGuiPanelsLayout
#ifdef INCL_LAYOUT
	panels.exit();
#endif

	//--

	removeKeysListeners();
	removeMouseListeners();

	ofRemoveListener(ofEvents().update, this, &ofxColorManager::update);
	ofRemoveListener(ofEvents().draw, this, &ofxColorManager::draw);
}

//--------------------------------------------------------------
void ofxColorManager::refresh_Gui_Layout()
{
	curve_SliderTest.setVisible(SHOW_Curve);
	curve_SliderTweak.setVisible(SHOW_Curve);

	//-

	//between panels in curve sections
	int _pad1 = 30;
	int _pad2 = 3;

	int _xx, _yy, _ww, _hh;

	//if (MODE_Editor)
	if (1)
	{
		_xx = rPreview.getX() + _pad1;
		_yy = rPreview.getY() + _pad1;
		_ww = rPreview.getWidth() + (2 * _pad1);
		_hh = rPreview.getHeight() + (2 * _pad1);
	}
	else
	{

	}

	//--

	//color boxes
	box_size = 30;

	//user palette colors
	box_size_user = 50;

	//_pad1 = 0;//between box colors in palettes

	//--

	// curve tool pos (anchor for others)

	// a. hardcoded
	curveTool_x = _xx;
	curveTool_y = _yy;
	curveTool_w = cAmt;
	curveTool_h = cAmt;

	//--

	// curve mod slider
	sliderMod_x = curveTool_x + curveTool_w + _pad2;
	sliderMod_y = _yy;
	sliderMod_w = box_size_user;
	sliderMod_h = curveTool_h;

	//--

	// gradient-pre curve (bad sorted to the left but anchored to curve..)
	grad_x = sliderMod_x + 0.5 * sliderMod_w + _pad2;
	grad_y = _yy + curveTool_h;
	grad_w = box_size_user;
	grad_h = curveTool_h;

	//--

	// gradient-curved image LUT 
	//[referenced to curve pos (vertical)]
	image_curvedGradient_x = grad_x + 0.5 * grad_w;// +_pad2;
	image_curvedGradient_y = _yy;
	image_curvedGradient_w = curveTool_w;
	image_curvedGradient_h = curveTool_h;

	//--

	// testing curve widgets
	// curve out slider
	slider_x = image_curvedGradient_x + image_curvedGradient_w + _pad2;
	slider_y = _yy;
	slider_w = box_size_user;
	slider_h = curveTool_h;

	//--

	curve_SliderTest.setup(sliderMod_x, sliderMod_y, sliderMod_w, sliderMod_h, 0, 1, 0, true, true);
	curve_SliderTweak.setup(slider_x, slider_y, slider_w, slider_h, 0, 1, 0, true, true);

	//--	

	// current color box/bar (the one affected by slider position. just for testing gradient purpose)
	currColor_x = slider_x + slider_w + _pad2;
	currColor_y = _yy;

	//----

	//#ifdef USE_RECTANGLE_INTERFACES

	//	// interface buttons

	//// user palette (pos related to gradient-pre curve)
	//palette_x = grad_x - (grad_w + _pad2);
	//palette_y = curveTool_y;

	//// user color box monitor picked (same that color picker gui)
	//// bar mode
	//color_w = (2 * box_size_user);
	//color_x = palette_x - (color_w + _pad2 + box_size_user / 2 + _pad2);
	//color_y = curveTool_y;
	//color_h = curveTool_h;

	//--

	//r_color_picked = ofRectangle(color_x, color_y, color_w, color_h);

	//--

	//	// 1. algorithmic palettes
	//	palettes_x = 996;
	//	palettes_y = 10;
	//
	//	// 2. color clicked box on palettes(hidden)
	//	colorPick_x = 390;
	//	colorPick_y = color_h + 30;
	//	colorPick_w = colorPick_h = 2 * box_size_user;
	//	r_color_clicked = ofRectangle(colorPick_x, colorPick_y, colorPick_w, colorPick_h);
	//
	//	// 3. browser colors palettes
	//	colorBrowserPos = glm::vec2(300, 335);
	//#endif

	//----
}

#pragma mark - INTERFACE


//--------------------------------------------------------------
void ofxColorManager::setup_Interface_Scene()
{
#ifdef USE_RECTANGLE_INTERFACES
	scene = new Node();
	scene->setSize(ofGetWidth(), ofGetHeight());
	scene->setName("Scene");

	TouchManager::one().setup(scene);

	//TODO:
	scene->deactivate();
#endif
}

//--------------------------------------------------------------
void ofxColorManager::palette_rearrenge()
{
#ifdef USE_RECTANGLE_INTERFACES
	ofLogVerbose(__FUNCTION__) << "size: " << btns_palette.size();

	// re-arrenge all resized boxes from interface to fill all bar
	// used when changed palette size

	std::string name;
	int x, y, w, hTotal;
	float h;//of each button
	x = palette_x;
	w = box_size_user;
	int numBtns = btns_palette.size();
	hTotal = curveTool_h;
	h = hTotal / (float)numBtns;

	//TODO: improve boxes borders and padding...
	//int _pad1 = 1;
	//h = (hTotal / (float) numBtns) - _pad1;

	bool flipBtn = true;//flip color sorting
	for (int i = 0; i < numBtns; i++)
	{
		name = "btn" + ofToString(i);
		auto a = scene->getChildWithName(name, 1000);

		//TODO: could get num childres instead of using 1000
		//auto a = scene->getChildWithName(name, scene->getNumChildren());

		if (flipBtn) y = palette_y + (numBtns - i - 1) * h;
		else y = palette_y + i * h;

		a->setPosition(x, y);
		a->setSize(w, h);
	}
#endif
}

#ifdef USE_RECTANGLE_INTERFACES

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
	btn->setPosition(palette_x, palette_y + box_size_user + pad + i * (box_size_user + pad));
	btn->setSize(box_size_user, box_size_user);

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
void ofxColorManager::update_Interface()
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
void ofxColorManager::draw_Interface_Scene()
{
	scene->render();

	if (bShowDebug)
	{
		scene->renderDebug();
	}
}
#endif

//----

#pragma mark - IM GUI

////--------------------------------------------------------------
//void ofxColorManager::gui_imGui_CosineGradient()
//{
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
void ofxColorManager::gui_Theory()
{
	// box size
	static int _cSize = 37;
	ImGuiColorEditFlags colorEdiFlags;

	if (ofxImGui::BeginWindow("THEORY", mainSettings, false))
	{
		ImGui::Dummy(ImVec2(0.0f, 10.f));

		float _spc = ImGui::GetStyle().ItemInnerSpacing.x;
		float _w = ImGui::GetWindowContentRegionWidth() - _spc;
		int _h = 20;
		float _w50 = MAX(150, _w * 0.33);

		//colorEdiFlags = ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoTooltip;

		colorEdiFlags =
			ImGuiColorEditFlags_NoSmallPreview |
			ImGuiColorEditFlags_NoTooltip |
			ImGuiColorEditFlags_NoLabel |
			ImGuiColorEditFlags_NoSidePreview |
			ImGuiColorEditFlags_NoAlpha |
			//ImGuiColorEditFlags_HSV |
			//ImGuiColorEditFlags_RGB |
			//ImGuiColorEditFlags_HDR |
			ImGuiColorEditFlags_NoInputs |
			ImGuiColorEditFlags_PickerHueWheel;

		//-

		ofParameter<ofFloatColor> c;
		c.set(color_TheoryBase.getName(),
			ofFloatColor(
				color_TheoryBase.get().r / 255.f,
				color_TheoryBase.get().g / 255.f,
				color_TheoryBase.get().b / 255.f,
				color_TheoryBase.get().a / 255.f),
			ofFloatColor(0, 0, 0, 0),
			ofFloatColor(1.f, 1.f, 1.f, 1.f)
		);
		auto tmpRef = c.get();

		//-

		//mini preview box
		if (ImGui::ColorButton("##cTheoryWl", *(ImVec4 *)&tmpRef.r, colorEdiFlags, ImVec2(_w, _h * 0.5)))
		{
		}

		//-

		if (ImGui::CollapsingHeader("BASE COLOR"))
		{
			ImGui::Columns(2, NULL, false);

			ImGui::PushItemWidth(_w50);

			//wheel
			if (ImGui::ColorPicker3("ColTheory", &tmpRef.r, colorEdiFlags))
			{
				ofLogNotice(__FUNCTION__) << "Wheel Picker Theory moved!";
				color_TheoryBase.set(tmpRef);
				color_Picked = color_TheoryBase.get();
			}

			ImGui::PopItemWidth();

			//if (ofxImGui::AddParameter(color_TheoryBase))
			//{
			//	ofLogNotice(__FUNCTION__) << "Rectangle Picker Theory moved!";
			//	color_Picked = color_TheoryBase.get();
			//}

			//square

			colorEdiFlags =
				ImGuiColorEditFlags_NoSmallPreview |
				ImGuiColorEditFlags_NoTooltip |
				ImGuiColorEditFlags_NoLabel |
				ImGuiColorEditFlags_NoSidePreview |
				ImGuiColorEditFlags_NoAlpha |
				//ImGuiColorEditFlags_HSV |
				//ImGuiColorEditFlags_RGB |
				//ImGuiColorEditFlags_HDR |
				//ImGuiColorEditFlags_NoInputs |
				ImGuiColorEditFlags_PickerHueBar;

			ImGui::NextColumn();

			ImGui::PushItemWidth(_w50);

			//if (ImGui::CollapsingHeader("SQUARE"))
			{
				if (ImGui::ColorPicker4("##cTheorySq", (float *)&tmpRef, colorEdiFlags))
				{
					ofLogNotice(__FUNCTION__) << "PICKER 2 MOVED !";
				}
			}

			ImGui::PopItemWidth();

			ImGui::Columns(1);
		}

		//----

		// controls

		ImGui::Dummy(ImVec2(0.0f, 5));

		//ImGui::SameLine();

		ofxImGui::AddParameter(bGetFromPicker);
		ofxImGui::AddParameter(bAuto_palette_recall);
		//if (ImGui::InputInt(amountColors.getName().c_str(), (int*)&amountColors.get(), 1, 5))
		if (ofxImGui::AddParameter(amountColors))
		{
			setup_Theory();
		}

		//-

		//ImGui::InputInt(amountColors.getName().c_str(), (int *)&amountColors.get());
		//ofxImGui::AddParameter(amountColors_Alg);
		//ofxImGui::AddParameter(colorScheme);
		//ofxImGui::AddParameter(colorSchemeName);

		// algorithmic palettes
		if (ImGui::CollapsingHeader("ADVANCED"))
		{
			//ImGui::PushItemWidth(_w * 0.5);

			ofxImGui::AddParameter(MODE_TweakSatBrg);
			//ofxImGui::AddParameter(amountColors_Alg);

			if (MODE_TweakSatBrg)
			{
				//ImGui::PushItemWidth(_w * 0.5);

				//ImGui::SliderFloat(SATURATION.getName().c_str(),
				//	(float *)&SATURATION.get(),
				//	SATURATION.getMin(),
				//	SATURATION.getMax());
				//ImGui::SliderFloat(BRIGHTNESS.getName().c_str(),
				//	(float *)&BRIGHTNESS.get(),
				//	BRIGHTNESS.getMin(),
				//	BRIGHTNESS.getMax());

				//ImGui::PopItemWidth();

				//-

				ofxImGui::AddParameter(SATURATION);
				ofxImGui::AddParameter(BRIGHTNESS);
			}
			ofxImGui::AddParameter(bLock_palette);
			//ofxImGui::AddParameter(bAuto_palette_recall);
			//ofxImGui::AddParameter(bRandomPalette);

			//ImGui::PopItemWidth();
		}

		//--

		ImGui::Dummy(ImVec2(0.0f, 5));

		//--

		// 1. basic

		for (int i = 0; i < NUM_COLOR_THEORY_TYPES; i++)
		{
			// border to selected
			bool bDrawBorder = false;
			if (i == lastTheory)
			{
				bDrawBorder = true;
			}
			if (bDrawBorder)
			{
				ImGui::PushStyleColor(ImGuiCol_Border, color_Pick);
				ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, linew_Pick);
			}

			//-

			// 1.1 label button

			//std::string _label = ColorWheelSchemes::colorSchemeNames[i];
			if (ofxSurfingHelpers::AddSmallButton(theoryTypes[i], 150, _cSize))
			{
			}

			//-

			if (bDrawBorder)
			{
				ImGui::PopStyleColor();
				ImGui::PopStyleVar(1);
			}

			//--

			ImGui::SameLine();

			//--

			const size_t _total = colorsTheory[i].size();

			for (int n = 0; n < _total; n++)
			{
				ImGui::PushID(n);

				//-			

				ImGui::SameLine();

				if (ImGui::ColorButton("##paletteTheory",
					colorsTheory[i][n],
					ImGuiColorEditFlags_NoAlpha |
					ImGuiColorEditFlags_NoPicker |
					ImGuiColorEditFlags_NoTooltip,
					ImVec2(_cSize, _cSize)))
				{
					lastColorTheoryPicked_Palette = n;

					//color = ImVec4(palette[n].x, palette[n].y, palette[n].z, palette.w); // Preserve alpha!
					//color_Picked = color;
				}

				//-

				ImGui::PopID();
			}
		}

		//----

		// 2. extra

		for (int i = 0; i < 7; i++)
		{
#ifndef USE_RECTANGLE_INTERFACES
			size_t _total;
			switch (i)
			{
			case 0:
				_total = complement.size();
				break;
			case 1:
				_total = complementBrightness.size();
				break;
			case 2:
				_total = monochrome.size();
				break;
			case 3:
				_total = monochromeBrightness.size();
				break;
			case 4:
				_total = analogue.size();
				break;
			case 5:
				_total = triad.size();
				break;
			case 6:
				_total = complementTriad.size();
				break;
	}
#endif

#ifdef USE_RECTANGLE_INTERFACES
			size_t _total;
			switch (i)
			{
			case 0:
				_total = btns_plt_CompSat.size();
				break;
			case 1:
				_total = btns_plt_ComplBrgt.size();
				break;
			case 2:
				_total = btns_plt_MonoSat.size();
				break;
			case 3:
				_total = btns_plt_MonoBrgt.size();
				break;
			case 4:
				_total = btns_plt_Analog.size();
				break;
			case 5:
				_total = btns_plt_Triad.size();
				break;
			case 6:
				_total = btns_plt_ComplTriad.size();
				break;
			}
#endif

			//-

			// border to selected
			bool bDrawBorder = false;
			if (i + NUM_COLOR_THEORY_TYPES == lastTheory)
			{
				bDrawBorder = true;
			}
			if (bDrawBorder)
			{
				ImGui::PushStyleColor(ImGuiCol_Border, color_Pick);
				ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, linew_Pick);
			}

			//-

			if (ofxSurfingHelpers::AddSmallButton(algoTypes[i], 150, _cSize))
			{
				//theory_Name = algoTypes[i].getName();
				//lastTheory = i;
			}

			//-

			if (bDrawBorder)
			{
				ImGui::PopStyleColor();
				ImGui::PopStyleVar(1);
			}

			//-

			ImGui::SameLine();

			for (int n = 0; n < _total; n++)
			{
				ImGui::PushID(n);
				std::string _name;

				ofColor c;
				switch (i)
				{
				case 0:
					if (n < complement.size())
						c = complement[n];
					_name = "Complement";
					break;
				case 1:
					if (n < complementBrightness.size())
						c = complementBrightness[n];
					_name = "Complement Brigthness";
					break;
				case 2:
					if (n < monochrome.size())
						c = monochrome[n];
					_name = "Monochrome";
					break;
				case 3:
					if (n < monochromeBrightness.size())
						c = monochromeBrightness[n];
					_name = "Monochrome Brightness";
					break;
				case 4:
					if (n < analogue.size())
						c = analogue[n];
					_name = "Analogue";
					break;
				case 5:
					if (n < triad.size())
						c = triad[n];
					_name = "Triad";
					break;
				case 6:
					if (n < complementTriad.size())
						c = complementTriad[n];
					_name = "Complement Triad";
					break;
				}

				ImGui::SameLine();

				if (ImGui::ColorButton("##paletteAlgorithm",

					c,
					ImGuiColorEditFlags_NoAlpha |
					ImGuiColorEditFlags_NoPicker |
					ImGuiColorEditFlags_NoTooltip,
					ImVec2(_cSize, _cSize)))
				{
					lastColorTheoryPicked_Palette = n + colorsTheory[i].size();

					//TODO:
					color_Picked.set(c);

					//refresh_Picker_Touched();
					//refresh_Picked_Update_To_HSV();//redundant.. ?
				}

				ImGui::PopID();
			}
}
	}
	ofxImGui::EndWindow(mainSettings);
}

//--------------------------------------------------------------
void ofxColorManager::gui_Palette()
{
	//ImGuiColorEditFlags colorEdiFlags =
	//	ImGuiColorEditFlags_NoSmallPreview |
	//	ImGuiColorEditFlags_NoTooltip |
	//	ImGuiColorEditFlags_NoLabel |
	//	ImGuiColorEditFlags_NoSidePreview |
	//	ImGuiColorEditFlags_NoInputs |
	//	ImGuiColorEditFlags_NoAlpha;

	ImGuiColorEditFlags ImGuiColorEditFlags = false;

	//-

	if (ofxImGui::BeginWindow("PALETE", mainSettings, false))
	{
		//-

		enum Mode
		{
			Mode_Copy,
			Mode_Move,
			Mode_Swap
		};
		static int mode = 2;

		//-

		if (ImGui::CollapsingHeader("Advanced"))
		{
			ofxImGui::AddParameter(bPaletteFillMode);
			if (bPaletteFillMode) {
				ImGui::InputInt(sizePaletteBox.getName().c_str(), (int*)&sizePaletteBox.get(), 5, 100);
				//ofxImGui::AddParameter(sizePaletteBox);
			}
			if (!bPaletteFillMode) {
				boxRowsUser.disableEvents();
				ImGui::InputInt(boxRowsUser.getName().c_str(), (int*)&boxRowsUser.get(), 1, 5);
				//ImGui::InputInt(boxRowsUser.getName().c_str(), (int *)&boxRowsUser.get());
				boxRowsUser = ofClamp(boxRowsUser.get(), boxRowsUser.getMin(), boxRowsUser.getMax());
				boxRowsUser.enableEvents();

				//ofxImGui::AddParameter(boxScale);
				ImGui::InputFloat(boxScale.getName().c_str(), (float *)&boxScale.get(), 0.005f, 0.010);
			}

			//-

			ImGui::Dummy(ImVec2(0.0f, 5));

			if (ImGui::RadioButton("Copy", mode == Mode_Copy)) { mode = Mode_Copy; } ImGui::SameLine();
			if (ImGui::RadioButton("Move", mode == Mode_Move)) { mode = Mode_Move; } ImGui::SameLine();
			if (ImGui::RadioButton("Swap", mode == Mode_Swap)) { mode = Mode_Swap; }

			ImGui::Dummy(ImVec2(0.0f, 5));

			if (ofxSurfingHelpers::AddSmallButton(bFlipUserPalette, 100, 0.5 * BUTTON_BIG_HEIGHT))
			{
				build_GradientPalette();
			}

			ImGui::Dummy(ImVec2(0.0f, 10));
		}

		//-

		//responsive
		ImVec2 button_sz((float)sizePaletteBox.get(), (float)sizePaletteBox.get());
		ImGuiStyle& style = ImGui::GetStyle();
		int buttons_count = palette.size();
		float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

		//-

		for (int n = 0; n < palette.size(); n++)
		{
			// responsive buttons size
			if (bPaletteFillMode) {
				ImGui::PushID(n);
				string name = ofToString(n);

				////customize colors
				//if (n == indexBrowser)//when selected
				//{
				//	const ImVec4 color1 = ImVec4(0.1, 0.1, 0.1, 0.8);//changes button color to black
				//	ImGui::PushStyleColor(ImGuiCol_Button, color1);
				//}
				//else //not selected
				{
					const ImVec4 color2 = style.Colors[ImGuiCol_Button];//do not changes the color
					ImGui::PushStyleColor(ImGuiCol_Button, color2);
				}
			}

			//--

			ImGui::PushID(n);

			if (!bPaletteFillMode) {
				//split in rows
				if (boxRowsUser != 0)
				{
					if ((n % boxRowsUser) != 0)
					{
						if (n != 0)
						{
							ImGui::SameLine();
						}
					}
				}
			}

			//-

			float _spc = ImGui::GetStyle().ItemInnerSpacing.x;
			float _w = ImGui::GetWindowContentRegionWidth() - _spc;

			//-

			int _r = (int)MIN(boxRowsUser.get(), palette.size());

			int wb;
			wb = (_w / _r) - _spc;
			wb = wb * boxScale.get();

			//--

			ImGuiColorEditFlags =
				ImGuiColorEditFlags_NoSmallPreview |
				ImGuiColorEditFlags_NoTooltip |
				ImGuiColorEditFlags_NoLabel |
				ImGuiColorEditFlags_NoSidePreview |
				ImGuiColorEditFlags_NoInputs |
				ImGuiColorEditFlags_NoAlpha;

			//--

			// color box
			ImVec2 bb;
			if (bPaletteFillMode) bb = button_sz;
			else bb = ImVec2(wb, wb);
			if (ImGui::ColorButton("##paletteDrag",
				palette[n],
				ImGuiColorEditFlags,
				bb))
			{
				lastColorPicked_Palette = n;
			}

			//--

			// dragggable boxes

			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
			{
				ImGui::SetDragDropPayload("DND_DEMO_CELL", &n, sizeof(int));
				if (mode == Mode_Copy) { ImGui::Text("Copy %s", ofToString(n).c_str()); }
				if (mode == Mode_Move) { ImGui::Text("Move %s", ofToString(n).c_str()); }
				if (mode == Mode_Swap) { ImGui::Text("Swap %s", ofToString(n).c_str()); }
				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
				{
					IM_ASSERT(payload->DataSize == sizeof(int));
					int payload_n = *(const int*)payload->Data;

					if (mode == Mode_Copy)
					{
						palette[n] = palette[payload_n];
					}
					if (mode == Mode_Move)
					{
						palette[n] = palette[payload_n];
						palette[payload_n] = ofColor(0);
					}
					if (mode == Mode_Swap)
					{
						const ofColor tmp = palette[n];
						palette[n] = palette[payload_n];
						palette[payload_n] = tmp;
					}

					//--

					// update all pallete (Curve)
					ofLogNotice(__FUNCTION__) << "DONE Dragged color";

					build_GradientPalette();
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::PopID();

			//--

			// responsive buttons size
			if (bPaletteFillMode) {
				ImGui::PopStyleColor();
				float last_button_x2 = ImGui::GetItemRectMax().x;
				float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
				if (n + 1 < buttons_count && next_button_x2 < window_visible_x2) ImGui::SameLine();
				ImGui::PopID();
			}
		}

		ImGui::Dummy(ImVec2(0.0f, 10));

		//-
	}
	ofxImGui::EndWindow(mainSettings);
}

//--------------------------------------------------------------
void ofxColorManager::gui_Library()
{
	if (ofxImGui::BeginWindow("LIBRARY", mainSettings))
	{
		{
			// get clicked color

			float _spc = ImGui::GetStyle().ItemInnerSpacing.x;
			//float _w = ImGui::GetWindowContentRegionWidth() - 25;
			float _w = ImGui::GetWindowContentRegionWidth() - _spc;

			int rCols;
			if (bPantoneCards) {
				rCols = rowSizePal;
			}
			else {
				rCols = pantoneMaxColumns;
			}

			//--

			// a. pantone cards: 7 colors / row
			//numColorsPage = numLibLines * rowSizePal;

			// b. responsive
			numColorsPage = numLibLines * pantoneMaxColumns.get();

			maxPages = totalNumColorsLib / (numColorsPage - 1);
			paletteLibPage.setMax(maxPages);
			//paletteLibPage.setMax(maxPages - 1);

			//--

			//pagerize
			int startColorInPal;
			int endColorInPal;
			if (bPagerized) {
				startColorInPal = paletteLibPage * numColorsPage;
				endColorInPal = startColorInPal + numColorsPage;
			}
			else {
				startColorInPal = 0;
				endColorInPal = totalNumColorsLib - 1;
			}

			//paletteLibPage = paletteLibPage_param.get();

			//--

			// get color from outside color picked

			bCallback_ENABLED = false;//maybe required because get() causes callbacks too (?)
			static ImVec4 color;
			color = color_Picked.get();
			bCallback_ENABLED = true;

			//--

			// 2 load/create palete from colorBrowser

			static bool saved_palette_inited = false;
			//const int PaletteSIZE = ColorBrowser_palette.size();//error
			//static ImVec4 _palette[130];//same than openColor palettes

			static ImVec4 saved_palette[NUM_COLORS_PANTONE];//same than Pantone palette

			if (!saved_palette_inited) {
				for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
				{
					ofFloatColor c = ofColor(ColorBrowser_palette[n]);
					saved_palette[n].x = c.r;
					saved_palette[n].y = c.g;
					saved_palette[n].z = c.b;
					saved_palette[n].w = 1.0f;//alpha
				}
			}
			saved_palette_inited = true;

			//-

			ImGui::Dummy(ImVec2(0.0f, 5));

			std::string s = "";
			s += "PANTONE (c)";
			s += "    ";
			s += ofToString(lastPantoneIndex) + "/" + ofToString(ColorBrowser.pantoneNames.size() - 1);
			ImGui::Text(s.c_str());

			ImGui::Dummy(ImVec2(0.0f, 5));

			//-

			// name color
			// load tab2 with lastColorPickedNameColor
			char tab2[32];
			strncpy(tab2, lastColorPickedNameColor.c_str(), sizeof(tab2));
			tab2[sizeof(tab2) - 1] = 0;
			//ImGui::PushItemWidth(_w * 0.5);
			ImGui::Text("%s", tab2);//color name label

			ImGui::Dummy(ImVec2(0.0f, 5));

			//-

			// controls
			if (ImGui::CollapsingHeader("Advanced"))
			{
				ImGui::InputInt(sizeLibColBox.getName().c_str(), (int*)&sizeLibColBox.get(), 1, 5);
				ofxImGui::AddParameter(bLibFillMode);
				ofxImGui::AddParameter(bPagerized);

				//-

				// responsive buttons size
				if (!bLibFillMode) {
					ofxImGui::AddParameter(bPantoneCards);
					ImGui::InputFloat(pantoneScale.getName().c_str(), (float *)&pantoneScale.get(), 0.02f, 0.1f);

					if (!bPantoneCards)
					{
						ImGui::InputInt(numLibLines.getName().c_str(), (int*)&numLibLines.get(), 1, 5);
						ImGui::InputInt(pantoneMaxColumns.getName().c_str(), (int*)&pantoneMaxColumns.get(), 1, 5);

					}
				}

				//--

				// 2.2 draw all palette colors grid

				//--
			}

			//-

			// arrow buttons

			ImGui::Dummy(ImVec2(0.0f, 5));

			if (bPagerized) {
				float spacing = ImGui::GetStyle().ItemInnerSpacing.x;

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

				ofxImGui::AddParameter(paletteLibPage);//page slider selector
				//ImGui::SliderInt("PAGE", &paletteLibPage, 0, maxPages);//page slider selector
				//ImGui::DragInt("PAGE", (int *)&paletteLibPage, 0, maxPages);
			}

			ImGui::Dummy(ImVec2(0.0f, 5));

			//-

			//responsive
			ImVec2 button_sz((float)sizeLibColBox.get(), (float)sizeLibColBox.get());
			ImGuiStyle& style = ImGui::GetStyle();
			int buttons_count;
			if (bLibFillMode) buttons_count = totalNumColorsLib;
			else buttons_count = endColorInPal - startColorInPal;
			float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

			//-

			for (int n = startColorInPal; n < endColorInPal; n++)
			{
				//--

				// responsive buttons size
				if (bLibFillMode) {
					ImGui::PushID(n);
					string name = ofToString(n);

					////customize colors
					//if (n == indexBrowser)//when selected
					//{
					//	const ImVec4 color1 = ImVec4(0.1, 0.1, 0.1, 0.8);//changes button color to black
					//	ImGui::PushStyleColor(ImGuiCol_Button, color1);
					//}
					//else //not selected
					{
						const ImVec4 color2 = style.Colors[ImGuiCol_Button];//do not changes the color
						ImGui::PushStyleColor(ImGuiCol_Button, color2);
					}
				}

				//--

				if (n < totalNumColorsLib) //to avoid empty colors at page end...
				{
					// ImGui::PushItemWidth(_w * 0.2);
					ImGui::PushID(n);

					//ImGui::PushStyleVar(ImGui::ImGuiStyleVar_FrameBorderSize), 1.0f;

					if (!bLibFillMode) {
						if ((n % rCols) != 0)
						{
							ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);//vertical inter line
						}
					}

					// draw border to selected color
					bool bDrawBorder = false;
					if (n == lastColorPicked)
					{
						bDrawBorder = true;
					}
					if (bDrawBorder)
					{
						ImGui::PushStyleColor(ImGuiCol_Border, color_Pick);
						ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, linew_Pick);
					}

					//--

					// color button

					int _flags = ImGuiColorEditFlags_NoAlpha |
						ImGuiColorEditFlags_NoPicker |
						ImGuiColorEditFlags_NoTooltip;

					ImVec2 _bb;
					if (bLibFillMode) _bb = button_sz;
					else _bb = ImVec2(sizeLibColBox * pantoneScale, sizeLibColBox * pantoneScale);
					//button_sz))
					//ImVec2(sizeLibColBox, sizeLibColBox)))
					//ImVec2(sizeLibColBox * pantoneScale, sizeLibColBox * pantoneScale)))

					if (ImGui::ColorButton("##palette", saved_palette[n],
						_flags,
						_bb))

					{
						lastColorPicked = n;

						color = ImVec4(saved_palette[n].x, saved_palette[n].y, saved_palette[n].z, color.w); // Preserve alpha!
						color_Picked = color;

						lastPantoneIndex = n;

						//color name
						lastColorPickedNameColor = ColorBrowser.pantoneNames[n];
						std::string str = "ImGui: PALETTE PICKED: [" + ofToString(lastColorPicked) + "] " + lastColorPickedNameColor;

						ofLogNotice(__FUNCTION__) << str;
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

				//--

				// responsive buttons size
				if (bLibFillMode) {
					ImGui::PopStyleColor();
					float last_button_x2 = ImGui::GetItemRectMax().x;
					float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
					if (n + 1 < buttons_count && next_button_x2 < window_visible_x2) ImGui::SameLine();
					ImGui::PopID();
				}
			}
			//ofxImGui::EndTree(mainSettings);
		}
	}
	ofxImGui::EndWindow(mainSettings);
}

//--------------------------------------------------------------
void ofxColorManager::gui_Picker()
{
	if (ofxImGui::BeginWindow("PICKER", mainSettings, false))
	{
		// get color from outside color picked
		// get color from outside color picked

		bCallback_ENABLED = false;//maybe required because get() causes callbacks too (?)
		static ImVec4 color;
		color = color_Picked.get();
		bCallback_ENABLED = true;

		//--

		// 0. color big
		float _spc = ImGui::GetStyle().ItemInnerSpacing.x;
		float _w = ImGui::GetWindowContentRegionWidth() - _spc;
		int _h = 20;

		ImGuiColorEditFlags colorEdiFlags = ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoTooltip;

		if (true)
		{
			ImGui::ColorButton("MyColor##Picker", *(ImVec4 *)&colCurveTest, colorEdiFlags, ImVec2(_w, _h));
		}
		else
		{
			ImGui::ColorButton("MyColor##Picker", *(ImVec4 *)&color, colorEdiFlags, ImVec2(_w, _h));
		}

		//-

		if (ofxImGui::BeginTree("WHEEL", mainSettings))//grouped folder
		{
			//ImGui::PushItemWidth(_w);
			ImGui::PushItemWidth(_w* 0.90);

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

			if (0)
			{
				ImGui::ColorPicker4("MyColor##Wheel", (float *)&colCurveTest, colorEdiFlags);
				color_Picked = colCurveTest;
			}
			else {
				ImGui::ColorPicker4("MyColor##Wheel", (float *)&color, colorEdiFlags);
				color_Picked = color;
			}
			//{
			//	//ofLogNotice(__FUNCTION__) << "PICKER 1 MOVED !";
			//}

			//-

			// 1.2 square
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

			if (ImGui::CollapsingHeader("SQUARE"))
			{
				if (ImGui::ColorPicker4("MyColor##5", (float *)&color, colorEdiFlags))
				{
					ofLogNotice(__FUNCTION__) << "PICKER 2 MOVED !";

					if (0)
					{
						color_Picked = colCurveTest;
					}
					else {
						color_Picked = color;
					}
				}
			}

			//-

			ImGui::PopItemWidth();

			ofxImGui::EndTree(mainSettings);
		}

		//-

		// 2. HSB

		//if (ImGui::CollapsingHeader("HSB"))
		if (ofxImGui::BeginTree("HSB", mainSettings))
		{
			ImGui::PushItemWidth(_w * 0.8);

			// TODO: ?
			//should apply hsb here, not into callback, 
			//because it will trig
			//the color picked updating callback!!!

			bCallback_ENABLED = false; //disable callbacks

			if (ofxImGui::AddParameter(color_HUE))
			{
				ofLogNotice(__FUNCTION__) << "ImGui: HUE MOVED !";
				ofColor c;
				c.set(color_Picked.get());
				c.setHue(color_HUE);
				color_Picked.set(c);
				refresh_Picker_Touched();
			}
			if (ofxImGui::AddParameter(color_SAT))
			{
				ofLogNotice(__FUNCTION__) << "ImGui: SAT MOVED !";
				ofColor c;
				c.set(color_Picked.get());
				c.setSaturation(color_SAT);
				color_Picked.set(c);
				refresh_Picker_Touched();
			}
			if (ofxImGui::AddParameter(color_BRG))
			{
				ofLogNotice(__FUNCTION__) << "ImGui: BRG MOVED !";
				ofColor c;
				c.set(color_Picked.get());
				c.setBrightness(color_BRG);
				color_Picked.set(c);
				refresh_Picker_Touched();
			}

			bCallback_ENABLED = true;

			//-

			ImGui::PopItemWidth();

			ofxImGui::EndTree(mainSettings);
		}

		if (ImGui::CollapsingHeader("RANDOMIZER"))
		{
			//ofxImGui::AddParameter(bRandomColor);
			ofxSurfingHelpers::AddBigButton(bRandomColor, 150, 70);

			//enablers

			//ofxSurfingHelpers::AddBigToggle(bColor_HUE);
			//ofxSurfingHelpers::AddBigToggle(bColor_SAT);
			//ofxSurfingHelpers::AddBigToggle(bColor_BRG);

			//bCallback_ENABLED = false; //disable callbacks

			ImGui::Dummy(ImVec2(0.0f, 5));

			if (bColor_HUE) {
				color_HUE_0 = color_HUE;
			}
			if (bColor_SAT) {
				color_SAT_0 = color_SAT;
			}
			if (bColor_BRG) {
				color_BRG_0 = color_BRG;
			}

			if (ofxImGui::AddParameter(bColor_HUE))
			{
			}
			ImGui::SameLine();
			if (ofxImGui::AddParameter(bColor_SAT))
			{
			}
			ImGui::SameLine();
			if (ofxImGui::AddParameter(bColor_BRG))
			{
			}

			ImGui::Dummy(ImVec2(0.0f, 5));

			//ImGui::Text("Min");

			if (ofxImGui::AddParameter(color_HUE_0))
			{
				ofLogNotice(__FUNCTION__) << "ImGui: HUE MOVED !";

				if (bColor_HUE) {
					color_HUE = color_HUE_0;
				}
			}
			if (ofxImGui::AddParameter(color_SAT_0))
			{
				ofLogNotice(__FUNCTION__) << "ImGui: SAT MOVED !";

				if (bColor_SAT) {
					color_SAT = color_SAT_0;
				}
			}
			if (ofxImGui::AddParameter(color_BRG_0))
			{
				ofLogNotice(__FUNCTION__) << "ImGui: BRG MOVED !";

				if (bColor_BRG) {
					color_BRG = color_BRG_0;
				}
			}

			ImGui::Dummy(ImVec2(0.0f, 5));

			//ImGui::Text("Max");

			if (ofxImGui::AddParameter(color_HUE_Power))
			{
				ofLogNotice(__FUNCTION__) << "ImGui: HUE MOVED !";
			}
			if (ofxImGui::AddParameter(color_SAT_Power))
			{
				ofLogNotice(__FUNCTION__) << "ImGui: SAT MOVED !";
			}
			if (ofxImGui::AddParameter(color_BRG_Power))
			{
				ofLogNotice(__FUNCTION__) << "ImGui: BRG MOVED !";
			}
		}

		//--

		// user palette

		//if (ofxImGui::BeginTree("PALETTE", mainSettings))
		if (ImGui::CollapsingHeader("PALETTE"))
		{
			int pad = _w * 0.1;
			//int _pad1 = _w * 0.1;
			//int _pad1 = _w * 0.2;

			float w50 = ImGui::GetWindowWidth()*0.5f - pad * 0.5f;
			float w100 = ImGui::GetWindowWidth() - pad;

			ofxSurfingHelpers::AddBigButton(bAddColor, w50, 30); ImGui::SameLine();
			ofxSurfingHelpers::AddBigButton(bRemoveColor, w50, 30);
			ofxSurfingHelpers::AddBigToggle(bPaletteEdit, w100);

			ofxImGui::AddParameter(bClearPalette);

			//ofxImGui::EndTree(mainSettings);
		}
	}
	ofxImGui::EndWindow(mainSettings);
}

//--------------------------------------------------------------
void ofxColorManager::gui_Panels()
{
	if (ofxImGui::BeginWindow("PANELS", mainSettings, false))
	{
		//ImGui::Dummy(ImVec2(0.0f, 10));

		ImGui::Columns(3);

		//ofxSurfingHelpers::AddBigToggle(SHOW_Library, -1);

		ofxImGui::AddParameter(SHOW_UserPalette);
		ofxImGui::AddParameter(SHOW_Picker);
		ofxImGui::AddParameter(SHOW_Library);
		ofxImGui::AddParameter(SHOW_BackGround);

		ofxImGui::AddParameter(SHOW_Curve);

		ImGui::NextColumn();
		//ImGui::Separator();

		ofxImGui::AddParameter(SHOW_Theory);
		ofxImGui::AddParameter(SHOW_Range);
		ofxImGui::AddParameter(SHOW_ColourLovers);
		ofxImGui::AddParameter(SHOW_Quantizer);

		ImGui::NextColumn();
		//ImGui::Separator();

		ofxImGui::AddParameter(SHOW_ALL_GUI);
		ofxImGui::AddParameter(SHOW_GUI_MINI);
		ofxImGui::AddParameter(SHOW_Demo);
		ofxImGui::AddParameter(SHOW_Presets);

#ifdef INCL_LAYOUT
		ofxImGui::AddParameter(SHOW_GuiInternal);
#endif

		//ImGui::Separator();
		ImGui::Dummy(ImVec2(0.0f, 10));

		//ImGui::Separator();
		//ImGui::Dummy(ImVec2(0.0f, 10));

		//ofxImGui::AddParameter(SHOW_Gradient);
		//ofxImGui::AddParameter(SHOW_AlgoPalettes);
		//ofxImGui::AddParameter(SHOW_ColourLovers_searcher);
		//ofxImGui::AddParameter(SHOW_BrowserColors);
	}
	ofxImGui::EndWindow(mainSettings);
}

//--------------------------------------------------------------
void ofxColorManager::gui_Range()
{
	int _boxSize = 40 * rangeScale.get();
	//static int _boxSize = 40;

	if (ofxImGui::BeginWindow("RANGE", mainSettings, false))
	{
		ImGui::Dummy(ImVec2(0.0f, 10.f));

		ofFloatColor c1_Pre = guiCol1[0];
		ofFloatColor c2_Pre = guiCol2[0];

		float _spc = ImGui::GetStyle().ItemInnerSpacing.x;
		float _w = ImGui::GetWindowContentRegionWidth() - _spc;
		int _h = 20;
		float w50 = _w * 0.35f;
		float w100 = _w;

		ImGui::Columns(2);

		//ImGui::Spacing(50, 0);

		ImGui::PushItemWidth(w50);

		ImGuiColorEditFlags colorEdiFlags =
			ImGuiColorEditFlags_NoSmallPreview |
			ImGuiColorEditFlags_NoTooltip |
			ImGuiColorEditFlags_NoLabel |
			ImGuiColorEditFlags_NoSidePreview |
			ImGuiColorEditFlags_HSV |
			ImGuiColorEditFlags_RGB |
			//ImGuiColorEditFlags_NoInputs |
			ImGuiColorEditFlags_NoAlpha |
			ImGuiColorEditFlags_PickerHueWheel;

		//--

		// picker 1

		//wheel
		if (ImGui::ColorPicker3("1", &guiCol1[0], colorEdiFlags))
		{
			if (autoPickColor1) color_Picked.set(guiCol1[0]);
		}
		//mini box
		if (ImGui::ColorButton("##C1", *(ImVec4 *)&guiCol1[0], colorEdiFlags, ImVec2(w50, _h)))
		{
		}

		ImGui::PopItemWidth();

		//--

		ofxImGui::AddParameter(autoPickColor1);

		ImGui::NextColumn();

		//-

		//ImGui::NextColumn();

		//ImGui::Dummy(ImVec2(0.0f, 5));

		//-

		ImGui::PushItemWidth(w50);

		//--

		// picker 2

		//wheel
		if (ImGui::ColorPicker3("2", &guiCol2[0], colorEdiFlags))
		{
			if (autoPickColor2) color_Picked.set(guiCol2[0]);
		}
		//mini box
		if (ImGui::ColorButton("##C2", *(ImVec4 *)&guiCol2[0], colorEdiFlags, ImVec2(w50, _h)))
		{
		}

		ImGui::PopItemWidth();

		//--

		ofxImGui::AddParameter(autoPickColor2);

		//--

		ImGui::Columns(1);

		ImGui::Dummy(ImVec2(0.0f, 5));

		//-

		c1_Rng.set(guiCol1);
		c2_Rng.set(guiCol2);

		//c1_Rng = guiCol1;
		//c2_Rng = guiCol2;

		// autogenerate
		if (bRangeAutoGenerate)
		{
			if (c1_Pre != guiCol1[0] ||
				c2_Pre != guiCol2[0])

				generateRange(c1_Rng, c2_Rng);
		}
		ofxImGui::AddParameter(bRangeAutoGenerate);

		//if (ofxSurfingHelpers::AddBigButton("GENERATE"))
		if (ImGui::Button("GENERATE", ImVec2(200, 0.5 * BUTTON_BIG_HEIGHT)))
		{
			generateRange(c1_Rng, c2_Rng);
		}

		//-

		ImGui::Dummy(ImVec2(0.0f, 10));

		//ofxImGui::AddParameter(rangeScale);
		ImGui::InputFloat(rangeScale.getName().c_str(), (float *)&rangeScale.get(), 0.02f, 0.1f);

		ImGui::Dummy(ImVec2(0.0f, 5));

		//-

		ofxImGui::AddParameter(numColorsRange);

		//ofxSurfingHelpers::AddSmallButton(bGetPaletteFromRange, 150, 30);

		ImGui::Dummy(ImVec2(0.0f, 5));

		//-

		// 2.2 draw all palette colors grid

		const int _nCols = 11;//?
		const int _total = _nCols * _nCols;

		//const int _nCols = numColorsRange.get();
		//const int ty = (int)NUM_TYPES_RANGES;
		//const int _total = _nCols * ty;

		static ImVec4 color;
		color = color_Picked.get();

		static ImVec4 _palette[_total];
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

		//----

		// 3. colors boxes

		for (int n = 0; n < _total; n++)
		{
			//label for each row
			const int _row = n / numColorsRange.get();
			//const int _row = n / 11;

			if (n % _nCols == 0)
			{
				// label button

				//-

				// border to selected
				bool bDrawBorder = false;
				if (_row == lastRange)
				{
					bDrawBorder = true;
				}
				if (bDrawBorder)
				{
					ImGui::PushStyleColor(ImGuiCol_Border, color_Pick);
					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, linew_Pick);
				}

				//-

				// label name (type)

				if (ofxSurfingHelpers::AddSmallButton(rangTypes[_row], 100, _boxSize)) {

				}

				//-

				if (bDrawBorder)
				{
					ImGui::PopStyleColor();
					ImGui::PopStyleVar(1);
				}

				ImGui::SameLine();
			}

			{
				ImGui::PushID(n);

				//ImGui::PushItemWidth(_w * 0.2);
				//ImGui::PushStyleVar(ImGui::ImGuiStyleVar_FrameBorderSize), 1.0f;

				if ((n % _nCols) != 0)
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
					ImVec2(_boxSize, _boxSize)))
				{
					lastColorPicked = n;

					color = ImVec4(_palette[n].x, _palette[n].y, _palette[n].z, color.w); // Preserve alpha!
					color_Picked = color;

					//color name
					lastColorPickedNameColor = ColorBrowser.pantoneNames[n];
					std::string str = "ImGui: PALETTE PICKED: [" + ofToString(lastColorPicked) + "] " + lastColorPickedNameColor;

					ofLogNotice(__FUNCTION__) << str;
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
	}
	ofxImGui::EndWindow(mainSettings);
}

//--------------------------------------------------------------
void ofxColorManager::build_GradientPalette()
{
	gradient.reset();
	for (int i = 0; i < palette.size(); i++) {
		gradient.addColor(palette[i]);

	}
	//update_Engine();
	//setup_Interface();
	//refresh_ColorTheory();
}

//--------------------------------------------------------------
void ofxColorManager::gui_Curve()
{
	if (ofxImGui::BeginWindow("CURVE", mainSettings, false))
	{
		float _spc;
		float _w;
		float _w50;
		float _h;

		_spc = ImGui::GetStyle().ItemInnerSpacing.x;
		//_w = ImGui::GetWindowContentRegionWidth();
		_w = ImGui::GetWindowContentRegionWidth() - 3 * _spc;
		_w50 = _w * 0.5;
		_h = 1. * BUTTON_BIG_HEIGHT;

		//-

		//if (ImGui::TreeNode("EDIT"))
		if (ofxImGui::BeginTree("EDIT", mainSettings))
		{
			// reset
			//ImGui::PushItemWidth(_w);
			if (ImGui::Button(bResetCurve.getName().c_str(), ImVec2(_w, _h)))
			{
				bResetCurve = true;
				//rPreview.setRect(600, 200, 755, 295);
				//refresh_Gui_Layout();
			}
			//ImGui::PopItemWidth();

			ImGui::Dummy(ImVec2(0, 10));

			ofxImGui::AddParameter(gradient_HardMode);
			ImGui::Checkbox("toBackground", &TEST_toBackground);

			ImGui::Dummy(ImVec2(0, 10));

			// ctrl in/out
			ImGui::PushItemWidth(_w * 0.8);
			if (ofxImGui::AddParameter(curve_Ctrl_In))
			{
				curve_SliderTweak.setPercent(curve_Ctrl_In.get());
			}
			ofxImGui::AddParameter(curve_Ctrl_Out);
			ImGui::PopItemWidth();

			//ofxSurfingHelpers::AddBigButton(bResetCurve);
			//ofxImGui::AddParameter(bResetCurve);
			//ofxImGui::AddParameter(curveMod);
			//ofxImGui::AddParameter(pos_CurveEditor);

			//-

			//ImGui::TreePop();
			ofxImGui::EndTree(mainSettings);
		}

		ImGui::Dummy(ImVec2(0, 10));

		//--

		// curve Test

		//if (ImGui::TreeNode("CURVE TEST"))
		if (ofxImGui::BeginTree("CURVE TEST", mainSettings))
		{
			//ImGui::PushItemWidth(_w);

			ofxImGui::AddParameter(TEST_Mode);
			//ImGui::Checkbox("Enable", &TEST_Mode); 

			if (TEST_Mode)
			{
				ImGui::SameLine();
				ImGui::Checkbox("LFO", &TEST_LFO_Mode);
			}
			ImGui::SliderFloat("Speed", &TEST_Speed, 0.0f, 1.0f);

			//ImGui::PopItemWidth();

			//-

			//ImGui::TreePop();
			ofxImGui::EndTree(mainSettings);
		}

		//-

		ImGui::Dummy(ImVec2(0, 10));

		{
			//ImGui::PushItemWidth(_w);
			if (ofxSurfingHelpers::AddBigToggle(MODE_Editor, _w, 0.5* _h))
			{
			}
			//ofxImGui::AddParameter(MODE_Editor);
			//ImGui::PopItemWidth();
		}
	}
	ofxImGui::EndWindow(mainSettings);
}

//--------------------------------------------------------------
void ofxColorManager::gui_Background()
{
	if (ofxImGui::BeginWindow("BACKGROUND", mainSettings, false))
	{
		{
			ofxImGui::AddParameter(background_Draw_ENABLE);

			//-

			static ImVec4 color;
			color = color_backGround.get();

			ImGui::Dummy(ImVec2(0.0f, 10));

			//-

			// 0. color big

			float _w = ImGui::GetWindowContentRegionWidth();
			int _h = 20;

			ImGuiColorEditFlags colorEdiFlags;
			colorEdiFlags = ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoTooltip;

			ImGui::ColorButton("MyColor##Picker", *(ImVec4 *)&color, colorEdiFlags, ImVec2(_w, _h));

			ImGui::Dummy(ImVec2(0.0f, 10));

			//-

			int pad = _w * 0.1;
			float w50 = ImGui::GetWindowWidth()*0.5f - pad * 0.5f;
			float w100 = ImGui::GetWindowWidth() - pad;
			ImGui::PushItemWidth(w100);

			// squared box

			colorEdiFlags =
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

			ImGui::Dummy(ImVec2(0.0f, 15));

			//-

			ImGui::PushItemWidth(_w * 0.3);

			ofxImGui::AddParameter(color_backGround_SET);
			ofxImGui::AddParameter(color_backGround_Darker);
			if (color_backGround_Darker)
			{
				ofxImGui::AddParameter(backgroundDarkness);
			}
			ofxImGui::AddParameter(color_background_AutoSet);

			ImGui::PopItemWidth();
		}
	}
	ofxImGui::EndWindow(mainSettings);
}

//--------------------------------------------------------------
void ofxColorManager::gui_Presets()
{
	if (ofxImGui::BeginWindow("PRESETS", mainSettings))
	{
		float _spc = ImGui::GetStyle().ItemInnerSpacing.x;
		float _w = ImGui::GetWindowContentRegionWidth() - _spc;
		float _w50 = _w * 0.5;
		float _h = 0.5 * BUTTON_BIG_HEIGHT;

		//--
		////TODO:
		////if (ImGui::BeginMenuBar())
		//{
		//	ImGui::PushItemWidth(50);
		//	if (ImGui::BeginMenu("File"))
		//	{
		//		if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
		//		if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
		//		if (ImGui::MenuItem("Close", "Ctrl+W")) {}
		//		ImGui::EndMenu();
		//	}
		//	ImGui::EndMenuBar();
		//	ImGui::PopItemWidth();
		//}
		//--
		/*
		//        // 1. palettes
		//
		//        ImGui::Text("PALETTE");
		//
		//        if (ImGui::Button("SAVE PALETTE")) {
		//            ofLogNotice(__FUNCTION__) << "SAVE PALETTE";
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
		//            ofLogNotice(__FUNCTION__) << "LOAD PALETTE";
		//            PRESET_name = textInput_temp;
		//            ofLogNotice(__FUNCTION__) << "PRESET_name: " << PRESET_name;
		//
		//            myPresetPalette.palette_load(PRESET_name);
		//
		//            //-
		//
		//            color_backGround = myPresetPalette.getBackground();//TODO: temp solution because required pointer..
		//
		////            //TODO
		////            // apply loaded preset to local system
		////            vector<ofColor> _p = myPresetPalette.getPalette();
		////            palette_clear();
		////            for (int i = 0; i < _p.size(); i++) {
		////                palette_addColor(_p[i]);
		////            }
		////
		////            color_backGround = ofColor(myPresetPalette.getBackground());//get directly without pointing
		//
		//            //-
		//
		//            if (DEMO_Test) myDEMO.clear();
		//        }
		*/

		//--

		ImGui::Text("Name");
		//ImGui::Text("Type Name:");

		ImGui::Dummy(ImVec2(0.0f, 5));

		std::string textInput_temp = PRESET_name;

		// loaded string into char array
		char tab2[32];
		strncpy(tab2, textInput_temp.c_str(), sizeof(tab2));
		tab2[sizeof(tab2) - 1] = 0;

		ImGui::PushItemWidth(-1);
		if (ImGui::InputText("", tab2, IM_ARRAYSIZE(tab2)))
		{
			ofLogNotice(__FUNCTION__) << "InputText:" << ofToString(tab2);
			textInput_temp = ofToString(tab2);

			if (MODE_newPreset) MODE_newPreset = false;
		}
		ImGui::PopItemWidth();

		//----

		/*
		//ImGui::Dummy(ImVec2(0.0f, 10));

		//// arrow buttons

		//static int counter = currentFile;

		//ImGui::PushButtonRepeat(true);

		//// prev

		//if (ImGui::ArrowButton("##leftP", ImGuiDir_Left))
		//{
		//	if (counter > 0)
		//	{
		//		counter--;
		//		currentFile = counter;
		//		if (currentFile < files.size())
		//		{
		//			PRESET_name = files_Names[currentFile];
		//			ofLogNotice(__FUNCTION__) << "ARROW: PRESET_name: [" + ofToString(currentFile) + "] " << PRESET_name;

		//			preset_load(PRESET_name);
		//		}

		//		if (MODE_newPreset) MODE_newPreset = false;
		//	}
		//}

		//// next

		//ImGui::SameLine(0.0f, _spc);
		//if (ImGui::ArrowButton("##rightP", ImGuiDir_Right))
		//{
		//	if (counter < files.size() - 1)
		//	{
		//		counter++;
		//		currentFile = counter;
		//		if (currentFile < files.size())
		//		{
		//			PRESET_name = files_Names[currentFile];
		//			ofLogNotice(__FUNCTION__) << "ARROW: PRESET_name: [" + ofToString(currentFile) + "] " << PRESET_name;

		//			preset_load(PRESET_name);
		//		}
		//	}

		//	if (MODE_newPreset) MODE_newPreset = false;
		//}

		//ImGui::PopButtonRepeat();
		*/

		//ImGui::SameLine();

		//----

		ImGui::Dummy(ImVec2(0.0f, 5));

		static int counter = currentFile;

		//----

		// index preset / total
		int numPalettes = files_Names.size() - 1;
		ImGui::Text("%d/%d", currentFile, numPalettes);

		ImGui::Dummy(ImVec2(0.0f, 5));

		ImGui::PushButtonRepeat(true);

		if (ImGui::Button("Previous", ImVec2(_w50, _h)))
		{
			if (counter > 0)
			{
				counter--;
				currentFile = counter;
				if (currentFile < files.size())
				{
					PRESET_name = files_Names[currentFile];
					ofLogNotice(__FUNCTION__) << "ARROW: PRESET_name: [" + ofToString(currentFile) + "] " << PRESET_name;

					preset_load(PRESET_name);
				}

				if (MODE_newPreset) MODE_newPreset = false;

				//workflow
				if (DEMO_Test) myDEMO.reStart();
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Next", ImVec2(_w50, _h)))
		{
			if (counter < files.size() - 1)
			{
				counter++;
				currentFile = counter;
				if (currentFile < files.size())
				{
					PRESET_name = files_Names[currentFile];
					ofLogNotice(__FUNCTION__) << "ARROW: PRESET_name: [" + ofToString(currentFile) + "] " << PRESET_name;

					preset_load(PRESET_name);
				}
			}

			if (MODE_newPreset) MODE_newPreset = false;

			//workflow
			if (DEMO_Test) myDEMO.reStart();
		}

		ImGui::PopButtonRepeat();

		ImGui::Dummy(ImVec2(0.0f, 5));

		//----

		// scrollable list

		if (!files_Names.empty())
		{
			int currentFileIndex = currentFile;

			ImGui::PushItemWidth(-1);

			if (ofxImGui::VectorCombo(" ", &currentFileIndex, files_Names))
			{
				ofLogNotice(__FUNCTION__) << "currentFileIndex: " << ofToString(currentFileIndex);

				if (currentFileIndex < files_Names.size())
				{
					currentFile = currentFileIndex;
					PRESET_name = files_Names[currentFile];
					ofLogNotice(__FUNCTION__) << "PRESET_name: " << PRESET_name;

					preset_load(PRESET_name);
				}

				if (MODE_newPreset) MODE_newPreset = false;
			}

			ImGui::PopItemWidth();
		}

		ImGui::Dummy(ImVec2(0.0f, 10));

		//-

		// 2. presets

		if (ImGui::Button("SAVE", ImVec2(_w50, _h)))
		{
			ofLogNotice(__FUNCTION__) << "SAVE";

			//TODO
			//should re load by same name and get what position on vector
			//is to reload current preset number
			//textInput_temp = ofToString(tab2);
			//ofLogNotice(__FUNCTION__) << "textInput_temp:" << textInput_temp;

			PRESET_name = textInput_temp;

			ofLogNotice(__FUNCTION__) << "PRdrawESET_name: " << PRESET_name;

			preset_save(PRESET_name);
			preset_refreshFiles();
		}

		ImGui::SameLine();

		if (ImGui::Button("LOAD", ImVec2(_w50, _h)))//not required..
		{
			ofLogNotice(__FUNCTION__) << "LOAD";
			ofLogNotice(__FUNCTION__) << "PRESET_name: " << PRESET_name;
			preset_load(PRESET_name);
		}

		//ImGui::SameLine();

		if (ImGui::Button("DELETE", ImVec2(_w50, _h)))//current preset
		{
			ofLogNotice(__FUNCTION__) << "DELETE";

			files[currentFile].remove();
			preset_refreshFiles();

			//ofLogNotice(__FUNCTION__) << "DELETE:"<<str<<endl;
			//dir.listDir("user_kits/presets");
			//dir.allowExt("jpg");
			//dir.allowExt("png");
			//dir.sort();
		}

		//export user palette colors to live reload from another parallel app!
		ImGui::SameLine();

		if (ImGui::Button("EXPORT", ImVec2(_w50, _h)))
		{
			ofLogNotice(__FUNCTION__) << "EXPORT";

			saveColors();
		}

		if (ImGui::Button("UPDATE", ImVec2(_w50, _h)))
		{
			ofLogNotice(__FUNCTION__) << "UPDATE";

			PRESET_name = textInput_temp;

			ofLogNotice(__FUNCTION__) << "PRESET_name: " << PRESET_name;

			//delete old file
			files[currentFile].remove();
			//preset_refreshFiles();

			//save new one
			preset_save(PRESET_name);
			preset_refreshFiles();
		}

		//ImGui::SameLine();
		//if (ImGui::Button("REFRESH"))//current preset
		//{
		//    ofLogNotice(__FUNCTION__) << "REFRESH";
		//    preset_refreshFiles();
		//}

		ImGui::Dummy(ImVec2(0.0f, 10));

		//--

		if (MODE_newPreset)
		{
			ImGui::Separator();

			ImGui::Dummy(ImVec2(0.0f, 10));

			ImGui::Text("NEW PRESET!");

			ImGui::Dummy(ImVec2(0.0f, 10));

			// loaded string into char array
			char tab[32];
			strncpy(tab, textInput_New.c_str(), sizeof(tab));
			tab[sizeof(tab) - 1] = 0;

			ImGui::PushItemWidth(-1);
			if (ImGui::InputText("", tab, IM_ARRAYSIZE(tab)))
			{
				textInput_New = ofToString(tab);
				ofLogNotice(__FUNCTION__) << "textInput_New:" << textInput_New;
			}
			ImGui::PopItemWidth();

			//workflow: 
			//when its editing a new preset..

			int n = 30;
			float a = ofMap(ofGetFrameNum() % n, 0, n, 0.0f, 1.0f);

			ImGui::PushID(1);
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.5, 0.0f, 0.5f, a));

			ImGui::Dummy(ImVec2(0.0f, 10.f));

			if (ImGui::Button("SAVE NEW", ImVec2(_w, _h)))
			{
				MODE_newPreset = false;
				ofLogNotice(__FUNCTION__) << "textInput_New: " << textInput_New;
				preset_save(textInput_New);
				preset_refreshFiles();

				if (SHOW_Demo) myDEMO.clear();
			}

			ImGui::PopStyleColor(1);
			ImGui::PopID();
		}

		//----

		// palette colors mini preview

		ImGui::Dummy(ImVec2(0.0f, 10.f));

		ofxImGui::AddParameter(SHOW_PresetsPalette);

		ImGui::Dummy(ImVec2(0.0f, 5.f));

		if (SHOW_PresetsPalette)
		{
			//responsive
			ImVec2 button_sz((float)sizePaletteBox.get(), (float)sizePaletteBox.get());
			ImGuiStyle& style = ImGui::GetStyle();
			int buttons_count = palette.size();
			float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

			//-

			for (int n = 0; n < palette.size(); n++)
			{
				// responsive buttons size
				if (bPaletteFillMode) {
					ImGui::PushID(n);
					string name = ofToString(n);

					////customize colors
					//if (n == indexBrowser)//when selected
					//{
					//	const ImVec4 color1 = ImVec4(0.1, 0.1, 0.1, 0.8);//changes button color to black
					//	ImGui::PushStyleColor(ImGuiCol_Button, color1);
					//}
					//else //not selected
					{
						const ImVec4 color2 = style.Colors[ImGuiCol_Button];//do not changes the color
						ImGui::PushStyleColor(ImGuiCol_Button, color2);
					}
				}

				//----

				ImGui::PushID(n);

				if (!bPaletteFillMode) {
					//split in rows
					if (boxRowsUser != 0)
					{
						if ((n % boxRowsUser) != 0)
						{
							if (n != 0)
							{
								ImGui::SameLine();
							}
						}
					}
				}

				//----

				float _spc = ImGui::GetStyle().ItemInnerSpacing.x;
				float _w = ImGui::GetWindowContentRegionWidth() - _spc;

				//-

				int _r = (int)MIN(boxRowsUser.get(), palette.size());

				int wb;
				wb = (_w / _r) - _spc;
				wb = wb * boxScale.get();

				//--

				ImGuiColorEditFlags ImGuiColorEditFlags =
					ImGuiColorEditFlags_NoSmallPreview |
					ImGuiColorEditFlags_NoTooltip |
					ImGuiColorEditFlags_NoLabel |
					ImGuiColorEditFlags_NoSidePreview |
					ImGuiColorEditFlags_NoInputs |
					ImGuiColorEditFlags_NoAlpha;

				//--

				// color box
				ImVec2 bb;
				if (bPaletteFillMode) bb = button_sz;
				else bb = ImVec2(wb, wb);
				if (ImGui::ColorButton("##paletteDragPrst",
					palette[n],
					ImGuiColorEditFlags,
					bb))
				{
					lastColorPicked_Palette = n;
				}

				//--

				/*
				// dragggable boxes

				//if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				//{
				//	ImGui::SetDragDropPayload("DND_DEMO_CELL", &n, sizeof(int));
				//	if (mode == Mode_Copy) { ImGui::Text("Copy %s", ofToString(n).c_str()); }
				//	if (mode == Mode_Move) { ImGui::Text("Move %s", ofToString(n).c_str()); }
				//	if (mode == Mode_Swap) { ImGui::Text("Swap %s", ofToString(n).c_str()); }
				//	ImGui::EndDragDropSource();
				//}

				//if (ImGui::BeginDragDropTarget())
				//{
				//	if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
				//	{
				//		IM_ASSERT(payload->DataSize == sizeof(int));
				//		int payload_n = *(const int*)payload->Data;

				//		if (mode == Mode_Copy)
				//		{
				//			palette[n] = palette[payload_n];
				//		}
				//		if (mode == Mode_Move)
				//		{
				//			palette[n] = palette[payload_n];
				//			palette[payload_n] = ofColor(0);
				//		}
				//		if (mode == Mode_Swap)
				//		{
				//			const ofColor tmp = palette[n];
				//			palette[n] = palette[payload_n];
				//			palette[payload_n] = tmp;
				//		}

				//		//--

				//		// update all pallete (Curve)
				//		ofLogNotice(__FUNCTION__) << "DONE Dragged color";

				//		build_GradientPalette();
				//	}
				//	ImGui::EndDragDropTarget();
				//}
				*/

				ImGui::PopID();

				//--

				// responsive buttons size
				if (bPaletteFillMode) {
					ImGui::PopStyleColor();
					float last_button_x2 = ImGui::GetItemRectMax().x;
					float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
					if (n + 1 < buttons_count && next_button_x2 < window_visible_x2) ImGui::SameLine();
					ImGui::PopID();
				}
			}

			ImGui::Dummy(ImVec2(0.0f, 10));
		}

		//-
	}
	ofxImGui::EndWindow(mainSettings);
}

//--------------------------------------------------------------
void ofxColorManager::gui_Demo()
{
	if (ofxImGui::BeginWindow("DEMO", mainSettings, false))
	{
		//ImGui::Dummy(ImVec2(0.0f, 5.f));

		ofxImGui::AddParameter(DEMO_Test);
		ofxImGui::AddParameter(DEMO_Auto);
		ofxImGui::AddParameter(DEMO_Timer);
		ofxImGui::AddParameter(DEMO_Alpha);
		if (ofxImGui::AddParameter(DEMO_Cam))
		{
			myDEMO.setEnableMouseCamera(DEMO_Cam);
		}
	}
	ofxImGui::EndWindow(mainSettings);
}

//--------------------------------------------------------------
bool ofxColorManager::gui_Draw()
{
	gui.begin();

	//--

	//TODO:
	//if (ImGui::BeginMenuBar())
	//{
	//	//ImGui::PushItemWidth(_w * 0.2);
	//	if (ImGui::BeginMenu("File"))
	//	{
	//		if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
	//		if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
	//		if (ImGui::MenuItem("Close", "Ctrl+W")) {}
	//		ImGui::EndMenu();
	//	}
	//	ImGui::EndMenuBar();
	//	//ImGui::PopItemWidth();
	//}

	//--

	if (SHOW_UserPalette) gui_Palette();

	if (SHOW_ALL_GUI)
	{
		if (SHOW_Presets) gui_Presets();

		if (SHOW_Picker) gui_Picker();

		if (SHOW_Library) gui_Library();

		if (SHOW_Range) gui_Range();

		if (SHOW_Theory) gui_Theory();

		if (SHOW_BackGround) gui_Background();

		if (SHOW_Curve) draw_Curve();

		if (SHOW_Panels) gui_Panels();

		if (SHOW_Demo) gui_Demo();

		if (SHOW_Quantizer) colorQuantizer.gui_Quantizer();

#ifdef USE_COLOR_LOVERS
		if (SHOW_ColourLovers) colourLoversHelper.draw();
#endif

		//if (SHOW_CosineGradient) gui_imGui_CosineGradient();
	}

	//--

	gui.end();

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

////--------------------------------------------------------------
//void ofxColorManager::gui_imGui_Theme()
//{
//	// must be done after setup the gui
//
//	ImGuiStyle *style = &ImGui::GetStyle();
//
//	// my dark theme
//	//    ImVec4* colors = ImGui::GetStyle().Colors;
//	style->Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
//	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
//	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
//	style->Colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
//	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
//	style->Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
//	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
//	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.54f);
//	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.26f, 0.26f, 0.68f);
//	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.25f, 0.25f, 0.67f);
//	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
//	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
//	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
//	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
//	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
//	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.12f, 0.11f, 0.11f, 1.00f);
//	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
//	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
//	style->Colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
//	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
//	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.74f, 0.75f, 0.77f, 0.79f);
//	style->Colors[ImGuiCol_Button] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
//	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.79f);
//	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.70f);
//	style->Colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.31f);
//	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
//	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
//	style->Colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
//	style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.29f, 0.29f, 0.29f, 0.78f);
//	style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
//	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.44f, 0.44f, 0.44f, 0.25f);
//	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.68f, 0.68f, 0.68f, 0.67f);
//	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.68f, 0.68f, 0.68f, 0.95f);
//	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.81f, 0.79f, 0.79f, 1.00f);
//	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.58f, 0.58f, 0.58f, 1.00f);
//	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
//	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
//	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.35f);
//	style->Colors[ImGuiCol_DragDropTarget] = ImVec4(0.50f, 0.50f, 0.50f, 0.90f);
//	style->Colors[ImGuiCol_NavHighlight] = ImVec4(0.79f, 0.79f, 0.79f, 1.00f);
//	style->Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
//	style->Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
//	style->Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
//
//	//    // 2. defaut dark theme exported
//	//    style->WindowMinSize = ImVec2(160, 65);
//	//    style->FramePadding = ImVec2(4, 2);
//	//    style->ItemSpacing = ImVec2(6, 2);
//	//    style->ItemInnerSpacing = ImVec2(6, 4);
//	//    style->Alpha = 1.0f;
//	//    style->WindowRounding = 0.0f;
//	//    style->FrameRounding = 0.0f;
//	//    style->IndentSpacing = 6.0f;
//	//    style->ItemInnerSpacing = ImVec2(2, 4);
//	//    style->ColumnsMinSpacing = 50.0f;
//	//    style->GrabMinSize = 14.0f;
//	//    style->GrabRounding = 0.0f;
//	//    style->ScrollbarSize = 12.0f;
//	//    style->ScrollbarRounding = 0.0f;
//}


#pragma mark - CURVE TOOL

//--------------------------------------------------------------
void ofxColorManager::setup_CurveTool()
{
	//gradient here too
	curve_img_gradient.allocate(image_curvedGradient_w, image_curvedGradient_h, OF_IMAGE_COLOR);

	curvesTool.setup(cAmt);

	//TODO: TEMP
	//curvesTool.load("settings/curves.yml"); //needed because it fills polyline

	pos_CurveEditor.set("POS CURVE", glm::vec2(500), glm::vec2(0), glm::vec2(1920, 180));

	curve_Ctrl_In.set("In", 0., 0., 1.);
	curve_Ctrl_Out.set("Out", 0., 0., 1.);
	bResetCurve.set("Reset Curve", false);
	SHOW_Curve.set("CURVE", false);
	curveMod.set("GRADIENT TEST", 0.5, 0., 1.);

	params_curve.add(curve_Ctrl_In);
	params_curve.add(curve_Ctrl_Out);
	params_curve.add(bResetCurve);
	params_curve.add(SHOW_Curve);
	params_curve.add(MODE_Editor);
	params_curve.add(SHOW_Editor);
	params_curve.add(pos_CurveEditor);

	ofAddListener(params_curve.parameterChangedE(), this, &ofxColorManager::Changed_Controls);

	//-

	// curve mod
	curve_SliderTest.setup(sliderMod_x, sliderMod_y, sliderMod_w * 2, sliderMod_h, 0, 1, 0, true, true);
	curve_SliderTest.setPercent(curveMod);
	curve_SliderTest.setVisible(SHOW_Curve);
	curve_SliderTest.setLabelString("Exp");

	// slider live test color out for this input
	curve_SliderTweak.setup(slider_x + (slider_w + pad), slider_y, 2 * slider_w, slider_h, 0, 1, 0, true, true);
	curve_SliderTweak.setVisible(SHOW_Curve);
	curve_SliderTweak.setLabelString("Pick");
}


//--------------------------------------------------------------
void ofxColorManager::update_CurveTool()
{
	//----

	// update values
	curve_Ctrl_In = curve_SliderTweak.getValue();
	curve_Index = (int)ofMap(curve_Ctrl_In.get(), 0., 1., 0, cAmt - 1);

	// curve modifier
	curveMod = curve_SliderTest.getValue();
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

	curvesTool.set(pointToModify, ofVec2f(pointY, ofMap(curveMod, 0., 1., 0, cAmt)));

	//--

	// vertical palette colors gradient rectangle modified by curveTool

	// every y point has different color
	for (int y = 0; y < image_curvedGradient_h; y++)
	{
		float input_LUT = ofMap(y, 0, image_curvedGradient_h, 0, cAmt - 1);
		float output_LUT = curvesTool[input_LUT];
		float output;
		output = ofMap(output_LUT, 0, cAmt - 1, 1., 0.);
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
		float out = ofMap(curvesTool.getAtPercent(1.0 - curve_Ctrl_In), 0, cAmt - 1, 1., 0.);
		ofColor c = gradient.getColorAtPercent(out);
		color_TARGET->set(c);//TODO: should reduce calls
	}
}

//--------------------------------------------------------------
void ofxColorManager::draw_Gradient()
{
	// GRADIENT
	// 1. un-curved gradient rectangle (left positioned)
	gradient.drawDebug(grad_x, grad_y, grad_w, grad_h);
}

//--------------------------------------------------------------
void ofxColorManager::draw_CurveTools()
{
	if (SHOW_Curve)
	{
		ofPushMatrix();
		ofPushStyle();

		//-

		// 1. splines editor

		ofSetColor(255, 225);

		curvesTool.draw(curveTool_x, curveTool_y, curve_Index, 225);

		//-

		// 1.2 horizontal line

		ofSetColor(ofColor(ofColor::white, 225));
		float y = curveTool_y + cAmt - curvesTool[curve_Index];
		ofDrawLine(curveTool_x, y, curveTool_x + curveTool_w, y);

		//// 1.3 current pos circle point
		//ofSetColor(255, 32);
		//ofDrawCircle(curveTool_x + curve_Index, curveTool_y + y, 3);

		//-

		// NOTE: 
		//sometimes we need some tricky hacking to avoid rare fliping from gradients
		curve_Ctrl_Out = ofMap(curvesTool.getAtPercent(curve_Ctrl_In), 0, cAmt - 1, 0., 1.);

		//-

		// 3. big gradient colors

		// image box gradient LUT
		ofSetColor(255, 255);

		curve_img_gradient.draw(
			image_curvedGradient_x, image_curvedGradient_y,
			image_curvedGradient_w, image_curvedGradient_h);

		//-

		// 4. border
		ofNoFill();
		ofSetLineWidth(2);
		ofSetColor(255, 225);

		ofRectangle r = ofRectangle(
			image_curvedGradient_x, image_curvedGradient_y,
			image_curvedGradient_w, slider_h);

		ofDrawRectangle(r);

		//-

		ofPopMatrix();
		ofPopStyle();
	}
}


#pragma mark - PALETTES

//--------------------------------------------------------------
void ofxColorManager::setup_Interface()
{
#ifdef USE_RECTANGLE_INTERFACES

	int btn_pad_w;
	btn_pad_w = 210;//padding to place boxes right to the labels
	int x0;
	x0 = palettes_x + btn_pad_w;
	int y0 = palettes_y;
	int h0 = box_size + pad;

	//--

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

	//----

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

#endif
}

//--------------------------------------------------------------
void ofxColorManager::setup_Labels()
{
#ifdef USE_RECTANGLE_INTERFACES

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

#endif
}


#ifdef USE_RECTANGLE_INTERFACES
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

	// workflow: 
	//set background color from first/last palette color
	if (color_background_AutoSet)
	{
		if (palette.size() > 0)
		{
			ofColor c;
			c = palette[0];//get first color from user palette

			if (color_backGround_Darker)
			{
				float darkness = ofMap(backgroundDarkness, 0.0, 1.0, 0.0, 2.0);
				float b = c.getBrightness() * darkness;
				b = ofClamp(b, 0.0, 1.0);
				c.setBrightness(b);
			}
			color_backGround.set(c);

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
		}
	}

	//-
}
#endif


//--------------------------------------------------------------
void ofxColorManager::update_Engine()
{
	// calculate base primary color
	if (MODE_TweakSatBrg)
	{
		// using hue only from picked color and forcing sat/(brg from algo sliders
		base = ofFloatColor::fromHsb(
			color_Picked.get().getHue(),
			ofMap(SATURATION, 0, 255, 0.f, 1.f),
			ofMap(BRIGHTNESS, 0, 255, 0.f, 1.f));
	}
	else
	{
		// check using hue + sat/brg from color ignoring algo panel SV sliders
		//base = color_Picked.get();
		base.set(color_Picked.get());
	}

	color_TheoryBase = base;

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

	complement.generateComplementary(ofxColorPalette::SATURATION, amountColors_Alg);
	complementBrightness.generateComplementary(ofxColorPalette::BRIGHTNESS, amountColors_Alg);
	triad.generateTriad();
	complementTriad.generateComplementaryTriad();
	monochrome.generateMonoChromatic(ofxColorPalette::SATURATION, amountColors_Alg);
	monochromeBrightness.generateMonoChromatic(ofxColorPalette::BRIGHTNESS, amountColors_Alg);
	analogue.generateAnalogous(amountColors_Alg, 0.2);

	//-

#ifdef USE_RECTANGLE_INTERFACES
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
		if (i < complement.size())//BUG: avoid crash
			btns_plt_CompSat[i]->setColor(complement[i]);
	}
	// 4. complement brgt
	for (int i = 0; i < btns_plt_ComplBrgt.size(); i++)
	{
		if (i < complementBrightness.size())//BUG: avoid crash
			btns_plt_ComplBrgt[i]->setColor(complementBrightness[i]);
	}
	// 5. mono sat
	for (int i = 0; i < btns_plt_MonoSat.size(); i++)
	{
		if (i < monochrome.size())//BUG: avoid crash
			btns_plt_MonoSat[i]->setColor(monochrome[i]);
	}
	// 6. mono brgt
	for (int i = 0; i < btns_plt_MonoBrgt.size(); i++)
	{
		if (i < monochromeBrightness.size())//BUG: avoid crash
			btns_plt_MonoBrgt[i]->setColor(monochromeBrightness[i]);
	}
	// 7. analogue
	for (int i = 0; i < btns_plt_Analog.size(); i++)
	{
		if (i < analogue.size())//BUG: avoid crash
			btns_plt_Analog[i]->setColor(analogue[i]);
	}
	//    // 8. random
	//    for (int i = 0; i < btns_plt_Random.size(); i++) {
	//        btns_plt_Random[i]->setColor(random[i]);
	//    }
#endif

	//--

	// 2. FROM OFX-COLOUR-THEORY

	update_Theory();//TODO: reduce calls...

	//--

#ifdef USE_RECTANGLE_INTERFACES

	// 0. btns_plt_CT_Analogous
	for (int i = 0; i < btns_plt_CT_Analogous.size(); i++)
	{
		if (i < colors_Analogous.size())//BUG: avoid crash
			btns_plt_CT_Analogous[i]->setColor(colors_Analogous[i]);
	}
	// 1. btns_plt_CT_Complementary
	for (int i = 0; i < btns_plt_CT_Complementary.size(); i++)
	{
		if (i < colors_Complementary.size())//BUG: avoid crash
			btns_plt_CT_Complementary[i]->setColor(colors_Complementary[i]);
	}
	// 2. btns_plt_CT_SplitComplementary
	for (int i = 0; i < btns_plt_CT_SplitComplementary.size(); i++)
	{
		if (i < colors_SplitComplementary.size())//BUG: avoid crash
			btns_plt_CT_SplitComplementary[i]->setColor(colors_SplitComplementary[i]);
	}
	// 3. btns_plt_CT_Compound
	for (int i = 0; i < btns_plt_CT_Compound.size(); i++)
	{
		if (i < colors_Compound.size())//BUG: avoid crash
			btns_plt_CT_Compound[i]->setColor(colors_Compound[i]);
	}
	// 4. btns_plt_CT_FlippedCompound
	for (int i = 0; i < btns_plt_CT_FlippedCompound.size(); i++)
	{
		if (i < colors_FlippedCompound.size())//BUG: avoid crash
			btns_plt_CT_FlippedCompound[i]->setColor(colors_FlippedCompound[i]);
	}
	// 5. btns_plt_CT_Monochrome
	for (int i = 0; i < btns_plt_CT_Monochrome.size(); i++)
	{
		if (i < colors_Monochrome.size())//BUG: avoid crash
			btns_plt_CT_Monochrome[i]->setColor(colors_Monochrome[i]);
	}
	// 6. btns_plt_CT_Tetrad
	for (int i = 0; i < btns_plt_CT_Tetrad.size(); i++)
	{
		if (i < colors_Tetrad.size())//BUG: avoid crash
			btns_plt_CT_Tetrad[i]->setColor(colors_Tetrad[i]);
	}
	// 7. btns_plt_CT_Triad
	for (int i = 0; i < btns_plt_CT_Triad.size(); i++)
	{
		if (i < colors_Triad.size())//BUG: avoid crash
			btns_plt_CT_Triad[i]->setColor(colors_Triad[i]);
	}
#endif

	//--
}


//--------------------------------------------------------------
void ofxColorManager::palettes_resize()
{
	// 1. remove all color boxes

	//-

#ifdef USE_RECTANGLE_INTERFACES
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
#endif

	//-

	//TODO

	//random.generateRandom(amountColors_Alg);
	update_Engine();
	setup_Interface();

	//-

	// WORKFLOW: trig last picked palette
	if (bAuto_palette_recall)
	{
		palette_clear();

		//-

#ifdef USE_RECTANGLE_INTERFACES
		palette_recallFromPalettes(SELECTED_palette_LAST);
#endif
	}

	//-
}

//--------------------------------------------------------------
void ofxColorManager::setVisible_Interface(bool b)
{
	//#ifdef USE_RECTANGLE_INTERFACES
	//	// hide and disable touchs for buttons
	//
	//	// ALGORITHMIC COLOR PALETTES
	//
	//	//-
	//
	//	// 1. FROM OFX-COLOR-PALETTE
	//
	//	for (auto &btn : btns_plt_Triad)
	//	{
	//		btn->setVisible(b);
	//		btn->setEnabled(b);
	//	}
	//	for (auto &btn : btns_plt_ComplTriad)
	//	{
	//		btn->setVisible(b);
	//		btn->setEnabled(b);
	//	}
	//	for (auto &btn : btns_plt_CompSat)
	//	{
	//		btn->setVisible(b);
	//		btn->setEnabled(b);
	//	}
	//	for (auto &btn : btns_plt_ComplBrgt)
	//	{
	//		btn->setVisible(b);
	//		btn->setEnabled(b);
	//	}
	//	for (auto &btn : btns_plt_MonoSat)
	//	{
	//		btn->setVisible(b);
	//		btn->setEnabled(b);
	//	}
	//	for (auto &btn : btns_plt_MonoBrgt)
	//	{
	//		btn->setVisible(b);
	//		btn->setEnabled(b);
	//	}
	//	for (auto &btn : btns_plt_Analog)
	//	{
	//		btn->setVisible(b);
	//		btn->setEnabled(b);
	//	}
	//	//    for (auto &btn : btns_plt_Random) {
	//	//        btn->setVisible(b);
	//	//        btn->setEnabled(b);
	//	//    }
	//
	//	//-
	//
	//	// 2. FROM OFX-COLOUR-THEORY
	//
	//	for (auto &btn : btns_plt_CT_Analogous)
	//	{
	//		btn->setVisible(b);
	//		btn->setEnabled(b);
	//	}
	//	for (auto &btn : btns_plt_CT_Complementary)
	//	{
	//		btn->setVisible(b);
	//		btn->setEnabled(b);
	//	}
	//	for (auto &btn : btns_plt_CT_SplitComplementary)
	//	{
	//		btn->setVisible(b);
	//		btn->setEnabled(b);
	//	}
	//	for (auto &btn : btns_plt_CT_Compound)
	//	{
	//		btn->setVisible(b);
	//		btn->setEnabled(b);
	//	}
	//	for (auto &btn : btns_plt_CT_FlippedCompound)
	//	{
	//		btn->setVisible(b);
	//		btn->setEnabled(b);
	//	}
	//	for (auto &btn : btns_plt_CT_Monochrome)
	//	{
	//		btn->setVisible(b);
	//		btn->setEnabled(b);
	//	}
	//	for (auto &btn : btns_plt_CT_Triad)
	//	{
	//		btn->setVisible(b);
	//		btn->setEnabled(b);
	//	}
	//	for (auto &btn : btns_plt_CT_Tetrad)
	//	{
	//		btn->setVisible(b);
	//		btn->setEnabled(b);
	//	}
	//
	//	//-
	//
	//	// 2. LABEL BUTTONS
	//
	//	for (auto &btn : btns_plt_Selector)
	//	{
	//		btn->setVisible(b);
	//		btn->setLocked(!b);
	//	}
	//#endif
}

//--------------------------------------------------------------
void ofxColorManager::refresh_ColorTheory()
{
	ofLogNotice(__FUNCTION__);

	////selected
	//colorSchemeName.set(ColorWheelSchemes::colorSchemeNames[colorScheme.get()]);
	//scheme = ColorWheelSchemes::colorSchemes[colorScheme.get()];
	//scheme->setPrimaryColor(color_TheoryBase.get());

	//colorsTheory = scheme->interpolate(amountColors.get());
	for (int i = 0; i < NUM_COLOR_THEORY_TYPES; i++)
	{
		shared_ptr<ColorWheelScheme> _scheme;
		_scheme = ColorWheelSchemes::colorSchemes[i];
		_scheme->setPrimaryColor(color_TheoryBase.get());
		colorsTheory[i] = _scheme->interpolate(amountColors.get());
	}
}

//--------------------------------------------------------------
void ofxColorManager::setup_Theory()
{
	amountColors_Alg.makeReferenceTo(amountColors);
	//amountColors.makeReferenceTo(amountColors_Alg);

	params_ColorTheory.setName("Color Theory");
	params_ColorTheory.add(color_TheoryBase.set("Base", ofColor::magenta, ofColor(0), ofColor(255)));
	params_ColorTheory.add(bGetFromPicker.set("Auto Picker", true));
	params_ColorTheory.add(colorScheme.set("Color Scheme", 6, 0, ColorWheelSchemes::colorSchemes.size() - 1));
	params_ColorTheory.add(colorSchemeName);
	params_ColorTheory.add(amountColors.set("Amnt Colors", 8, 2, 20));
	params_ColorTheory.add(lastColorTheoryPicked_Palette.set("Last Theory Picked", 0, 0, NUM_COLOR_THEORY_TYPES - 1));

	//toggles
	for (int i = 0; i < NUM_COLOR_THEORY_TYPES; i++)
	{
		theoryTypes[i].set(ColorWheelSchemes::colorSchemeNames[i], false);
		params_ColorTheory.add(theoryTypes[i]);
	}

	ofAddListener(params_ColorTheory.parameterChangedE(), this, &ofxColorManager::Changed_ColorTheory);

	refresh_ColorTheory();

	//---

//#ifdef USE_OFX_GUI
//	//panel.setup();
//	//panel.add(group);
//	//panel.setPosition(1000, 400);
//#endif

	//-

	//amountColors = amountColors_Alg;
	//
	////TEST
	//colorSchemeName.set(ColorWheelSchemes::colorSchemeNames[colorScheme.get()]);
	//scheme = ColorWheelSchemes::colorSchemes[colorScheme.get()];
	//scheme->setPrimaryColor(primaryColor);
	//colors = scheme->interpolate(amountColors.get());

	//-

	//TODO:
	// should reduce calls.. to setup only..

	scheme_Analogous_name = (ColorWheelSchemes::colorSchemeNames[0]);
	scheme_Analogous = ColorWheelSchemes::colorSchemes[0];
	colors_Analogous = scheme_Analogous->interpolate(amountColors_Alg);

	scheme_Complementary_name = (ColorWheelSchemes::colorSchemeNames[1]);
	scheme_Complementary = ColorWheelSchemes::colorSchemes[1];
	colors_Complementary = scheme_Complementary->interpolate(amountColors_Alg);

	scheme_SplitComplementary_name = (ColorWheelSchemes::colorSchemeNames[2]);
	scheme_SplitComplementary = ColorWheelSchemes::colorSchemes[2];
	colors_SplitComplementary = scheme_SplitComplementary->interpolate(amountColors_Alg);

	scheme_Compound_name = (ColorWheelSchemes::colorSchemeNames[3]);
	scheme_Compound = ColorWheelSchemes::colorSchemes[3];
	colors_Compound = scheme_Compound->interpolate(amountColors_Alg);

	scheme_FlippedCompound_name = (ColorWheelSchemes::colorSchemeNames[4]);
	scheme_FlippedCompound = ColorWheelSchemes::colorSchemes[4];
	colors_FlippedCompound = scheme_FlippedCompound->interpolate(amountColors_Alg);

	scheme_Monochrome_name = (ColorWheelSchemes::colorSchemeNames[5]);
	scheme_Monochrome = ColorWheelSchemes::colorSchemes[5];
	colors_Monochrome = scheme_Monochrome->interpolate(amountColors_Alg);

	scheme_Tetrad_name = (ColorWheelSchemes::colorSchemeNames[6]);
	scheme_Tetrad = ColorWheelSchemes::colorSchemes[6];
	colors_Tetrad = scheme_Tetrad->interpolate(amountColors_Alg);

	scheme_Triad_name = (ColorWheelSchemes::colorSchemeNames[7]);
	scheme_Triad = ColorWheelSchemes::colorSchemes[7];
	colors_Triad = scheme_Triad->interpolate(amountColors_Alg);
}

//--------------------------------------------------------------
void ofxColorManager::update_Theory()
{
	//	//color theory
	//	colorSchemeName.set(ColorWheelSchemes::colorSchemeNames[colorScheme.get()]);
	//	scheme = ColorWheelSchemes::colorSchemes[colorScheme.get()];
	//	scheme->setPrimaryColor(primaryColor.get());
	//	colors = scheme->interpolate(amountColors.get());

	//--

	primaryColor.set(base);

	//-

	scheme_Analogous = ColorWheelSchemes::colorSchemes[0];
	scheme_Analogous->setPrimaryColor(primaryColor);
	colors_Analogous = scheme_Analogous->interpolate(amountColors_Alg);

	scheme_Complementary = ColorWheelSchemes::colorSchemes[1];
	scheme_Complementary->setPrimaryColor(primaryColor);
	colors_Complementary = scheme_Complementary->interpolate(amountColors_Alg);

	scheme_SplitComplementary = ColorWheelSchemes::colorSchemes[2];
	scheme_SplitComplementary->setPrimaryColor(primaryColor);
	colors_SplitComplementary = scheme_SplitComplementary->interpolate(amountColors_Alg);

	scheme_Compound = ColorWheelSchemes::colorSchemes[3];
	scheme_Compound->setPrimaryColor(primaryColor);
	colors_Compound = scheme_Compound->interpolate(amountColors_Alg);

	scheme_FlippedCompound = ColorWheelSchemes::colorSchemes[4];
	scheme_FlippedCompound->setPrimaryColor(primaryColor);
	colors_FlippedCompound = scheme_FlippedCompound->interpolate(amountColors_Alg);

	scheme_Monochrome = ColorWheelSchemes::colorSchemes[5];
	scheme_Monochrome->setPrimaryColor(primaryColor);
	colors_Monochrome = scheme_Monochrome->interpolate(amountColors_Alg);

	scheme_Tetrad = ColorWheelSchemes::colorSchemes[6];
	scheme_Tetrad->setPrimaryColor(primaryColor);
	colors_Tetrad = scheme_Tetrad->interpolate(amountColors_Alg);

	scheme_Triad = ColorWheelSchemes::colorSchemes[7];
	scheme_Triad->setPrimaryColor(primaryColor);
	colors_Triad = scheme_Triad->interpolate(amountColors_Alg);

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
void ofxColorManager::palette_load_FromColourLovers()
{
	ofLogNotice(__FUNCTION__);

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
	if (color_background_AutoSet)
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
void ofxColorManager::draw_Mini()
{
	ofPushMatrix();
	ofPushStyle();

	glm::vec2 _pos;
	int _w = 60;
	int _p = 0;
	int _inner = 20;
	int _size = _w + _p;
	ofRectangle _r;

	// left top corner
	_pos = glm::vec2(_inner, _inner);
	//// right top corner
	//_pos = glm::vec2(ofGetWidth() - palette.size()*_size, 2*_p);

	ofTranslate(_pos);

	ofDrawBitmapStringHighlight(PRESET_name, glm::vec2(4, _w + _inner - 7), ofColor::black, ofColor::white);

	for (int col = 0; col < palette.size(); col++)
	{
		_r = ofRectangle(col * _size, 0, _w, _w);

		ofFill();
		ofSetColor(palette[col]);
		ofDrawRectangle(_r);
		ofNoFill();
		ofSetColor(ofColor(ofColor::white, 64));

		ofDrawRectangle(_r);
	}

	ofPopStyle();
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofxColorManager::palette_addColor(ofColor c)
{
	ofLogNotice(__FUNCTION__) << " : " << ofToString(c);

	palette.push_back(c);
	gradient.addColor(c);

	//-

#ifdef USE_RECTANGLE_INTERFACES
	palette_addColor_toInterface(c);
#endif

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

	ofLogNotice(__FUNCTION__) << " : " << c;
	ofLogNotice(__FUNCTION__) << "size pre: " << palette.size();

	if (c < palette.size())
	{
		// 0. erase last touched color th element
		//palette.erase(palette.begin() + c - 1);
		palette.erase(palette.begin() + c);

		// 1. debug after remove color from palette vector
		ofLogNotice(__FUNCTION__) << "vector palette contains: ";
		for (unsigned i = 0; i < palette.size(); ++i)
			ofLogNotice(__FUNCTION__) << ' ' << palette[i];
		ofLogNotice(__FUNCTION__) << '\n';
		ofLogNotice(__FUNCTION__) << "palette size post: " << palette.size();

		// 2. reset gradient
		gradient.reset();

		// 3. remove all interface buttons
#ifdef USE_RECTANGLE_INTERFACES
		for (int i = 0; i < btns_palette.size(); i++)
		{
			std::string n = ("btn" + ofToString(i));
			auto a = scene->getChildWithName(n, 1000);
			auto b = a->getName();
			scene->removeChild(a, false);
			ofLogNotice(__FUNCTION__) << "removed children: " << b;
		}
		btns_palette.clear();
#endif

		// 4. add edited palette colors vector to interface buttons and to gradient
		for (int i = 0; i < palette.size(); i++)
		{
			ofLogNotice(__FUNCTION__) << "add color [" << i << "]";
			gradient.addColor(palette[i]);

#ifdef USE_RECTANGLE_INTERFACES
			palette_addColor_toInterface(palette[i]);
#endif
		}

		// 5. make positions & resizes to fill bar
		palette_rearrenge();
	}
}


//--------------------------------------------------------------
void ofxColorManager::palette_removeColorLast()
{
#ifdef USE_RECTANGLE_INTERFACES
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
#endif
}


#ifdef USE_RECTANGLE_INTERFACES
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
#endif

//--------------------------------------------------------------
void ofxColorManager::palette_clear()
{
	ofLogNotice(__FUNCTION__);

	// remove all colors from the user palette
	palette.clear();
	gradient.reset();

	//-

#ifdef USE_RECTANGLE_INTERFACES

	ofLogVerbose(__FUNCTION__) << "getNumChildren: " << scene->getNumChildren();

	for (int i = 0; i < btns_palette.size(); i++)
	{
		std::string n = ("btn" + ofToString(i));
		auto a = scene->getChildWithName(n, 1000);
		auto b = a->getName();
		scene->removeChild(a, false);
		ofLogVerbose(__FUNCTION__) << "removed children: " << b;
	}
	btns_palette.clear();

#endif
}

#pragma mark - CALLBACKS

//--------------------------------------------------------------
void ofxColorManager::Changed_ColorPicked(ofFloatColor &_c)
{
	// TEST
	if (bCallback_ENABLED == true)
	{
		refresh_Picker_Touched();

		// TEST
		refresh_Picked_Update_To_HSV();//redundant.. ?

		// DEMO
		if (DEMO_Test) myDEMO.reStart();
	}
}

//--------------------------------------------------------------
void ofxColorManager::Changed_ColorClicked(ofFloatColor &color)
{
	// NOT USED. DISABLED
	ofLogNotice(__FUNCTION__) << ofToString(color);
	//color_Picked.set(color);
}

//--------------------------------------------------------------
void ofxColorManager::Changed_ColorUserPalette(ofAbstractParameter &e)
{
	std::string name = e.getName();

	//if (name != "In" &&
	//	name != "Out")
	{
		ofLogNotice(__FUNCTION__) << name << " : " << e;
	}

	if (false) {}

	else if (name == bFlipUserPalette.getName() && bFlipUserPalette) {
		bFlipUserPalette = false;

		std::reverse(palette.begin(), palette.end());
	}
}

//--------------------------------------------------------------
void ofxColorManager::Changed_ColorTheory(ofAbstractParameter &e)
{
	std::string name = e.getName();

	//if (name != "In" &&
	//	name != "Out")
	{
		ofLogNotice(__FUNCTION__) << name << " : " << e;
	}

	//colorSchemeName.set(ColorWheelSchemes::colorSchemeNames[colorScheme.get()]);
	//scheme = ColorWheelSchemes::colorSchemes[colorScheme.get()];
	//scheme->setPrimaryColor(primaryColor.get());
	//colors = scheme->interpolate(amountColors.get());

	//-

	if (false) {}

	else if (name == lastColorTheoryPicked_Palette.getName())
	{
	}

	else if (name == colorScheme.getName() || name == color_TheoryBase.getName())
	{
		refresh_ColorTheory();
	}

	else if (name == amountColors.getName())
	{
		amountColors_Alg = amountColors.get();

		update_Engine();
		refresh_ColorTheory();
	}

	// toggles
	else
	{
		for (int i = 0; i < NUM_COLOR_THEORY_TYPES; i++)
		{
			if (name == theoryTypes[i].getName() && theoryTypes[i].get())
			{
				theoryTypes[i] = false;//off button

				palette_clear();
				ofColor c;
				for (int n = 0; n < amountColors; n++)
				{
					c = colorsTheory[i][n];
					ofLogNotice(__FUNCTION__) << "color [" << n << "] " << ofToString(c);
					palette_addColor(c);
				}

				//-

				theory_Name = theoryTypes[i].getName();
				lastTheory = i;

				txt_lineActive[0] = true;//preset name
				txt_lineActive[1] = false;//palette name
				txt_lineActive[2] = true;//theory name
				txt_lineActive[3] = false;//range name

				// DEMO
				if (DEMO_Test) myDEMO.reStart();
			}
		}
	}
}

//--------------------------------------------------------------
void ofxColorManager::Changed_Controls(ofAbstractParameter &e)
{
	std::string name = e.getName();

	// TODO: should reduce callbacks in output..
	//    if (name != "In" && name != "Out" &&
	//            name!="H" && name!="S" && name!="B")

	if (name != "In" &&
		name != "Out")
	{
		ofLogNotice(__FUNCTION__) << name << " : " << e;
	}

	//--

	//panels

	//lovers
	if (name == SHOW_ColourLovers.getName())
	{
		colourLoversHelper.setEnableKeys(SHOW_ColourLovers);

		if (SHOW_ColourLovers)
		{
			SHOW_Theory = false;
			SHOW_Range = false;
			SHOW_Quantizer = false;
		}
	}

	//theory
	if (name == SHOW_Theory.getName())
	{
		if (SHOW_Theory)
		{
			//SHOW_Theory = false;
			SHOW_Range = false;
			SHOW_ColourLovers = false;
			SHOW_Quantizer = false;
		}
	}

	//theory
	if (name == SHOW_Range.getName())
	{
		if (SHOW_Range)
		{
			SHOW_Theory = false;
			SHOW_ColourLovers = false;
			SHOW_Quantizer = false;
		}
	}

	//quantizer
	if (name == SHOW_Quantizer.getName())
	{
		if (SHOW_Quantizer)
		{
			SHOW_Theory = false;
			SHOW_Range = false;
			SHOW_ColourLovers = false;
		}

		colorQuantizer.setActive(SHOW_Quantizer);
	}

	//--

	//all gui
	if (name == SHOW_ALL_GUI.getName())
	{
		if (SHOW_Curve)
		{
			curve_SliderTweak.setVisible(SHOW_ALL_GUI);
			curve_SliderTest.setVisible(SHOW_ALL_GUI);
		}
	}

	//position
	if (name == pos_CurveEditor.getName())
	{
	}

	//edit layout
	if (name == MODE_Editor.getName())
	{
		ofLogNotice(__FUNCTION__) << name << (MODE_Editor ? " TRUE" : " FALSE");

		if (MODE_Editor.get())
		{
			rPreview.enableEdit();
		}
		else
		{
			rPreview.disableEdit();
		}

		refresh_Gui_Layout();
	}

	//-

	// PANELS

#ifdef USE_COLOR_LOVERS
	if (name == "LOVERS")
	{
		colourLoversHelper.setVisible(SHOW_ColourLovers);

		////TODO
		//if (ENABLE_keys)
		//{
		colourLoversHelper.setEnableKeys(SHOW_ColourLovers);
		//}

		//-

		////workflow
		//colourLoversHelper.setVisibleSearcher(SHOW_ColourLovers_searcher);
		//if (SHOW_ColourLovers_searcher && !SHOW_ColourLovers)
		//	SHOW_ColourLovers = true;

		//-

		////TODO
		//if (ENABLE_keys)
		//{
		//    colourLoversHelper.setEnableKeys(true);
		//}
	}
#endif
	else if (name == "SHOW PALETTES")
	{
		setVisible_Interface(SHOW_AlgoPalettes);
	}
	else if (name == "SHOW BROWSER COLORS")
	{
		ColorBrowser.setVisible(SHOW_BrowserColors);
	}
	//else if (name == "SHOW CURVE TOOL")
	//{
	//}
	else if (name == "CURVE")
	{
		curve_SliderTest.setVisible(SHOW_Curve);
		curve_SliderTweak.setVisible(SHOW_Curve);
	}
	else if (name == "PALETTE")
	{
#ifdef USE_RECTANGLE_INTERFACES
		ofLogVerbose(__FUNCTION__) << "SHOW_UserPalette: " << SHOW_UserPalette;
		for (int i = 0; i < btns_palette.size(); i++)
		{
			btns_palette[i]->setVisible(SHOW_UserPalette);
		}
#endif
	}
	else if (name == "PICTURE")
	{
		// WORKFLOW:
		if (bPaletteEdit && SHOW_Quantizer) bPaletteEdit = false;

		// WORKFLOW: BUG
		//if (!SHOW_Presets)
		{
#ifdef USE_IMAGE_QUANTIZER
			colorQuantizer.setActive(SHOW_Quantizer);
			if (SHOW_Quantizer)
			{
				//open colors from one image
				//colorQuantizer.rebuildMap();
			}
#endif
		}
		//else
		//{
		//    colorQuantizer.setActive(false);
		//}
	}
	else if (name == "PRESETS")
	{
		//if (SHOW_Presets && SHOW_Quantizer)
		//    colorQuantizer.setActive(false);
		//else if (!SHOW_Presets && SHOW_Quantizer)
		//    colorQuantizer.setActive(true);
	}

	//--

	// HSB

	if (bCallback_ENABLED)
	{
		if (name == "H" && name == "S" && name == "B")
		{
			ofLogNotice(__FUNCTION__) << name << " : " << e;
		}

		if (name == "H")
		{
			ofColor c;
			c.set(color_Picked.get());
			c.setHue(color_HUE);
			color_Picked.set(c);
		}
		else if (name == "S")
		{
			ofColor c;
			c.set(color_Picked.get());
			c.setSaturation(color_SAT);
			color_Picked.set(c);
		}
		else if (name == "B")
		{
			ofColor c;
			c.set(color_Picked.get());
			c.setBrightness(color_BRG);
			color_Picked.set(c);
		}
	}

	//--

	if (name == "RANDOM COLOR")
	{
		if (bRandomColor)
		{
			bRandomColor = false;

			float _hue;
			float _sat;
			float _brg;

			//float _hue = ofRandom(color_HUE_0/255.f, color_HUE_Power / 255.f);

			if (bColor_HUE) {
				_hue = ofRandom(color_HUE_0 - 128 * color_HUE_Power, color_HUE_0 + 128 * color_HUE_Power);
				color_HUE = _hue;
			}
			if (bColor_SAT) {
				_sat = ofRandom(color_SAT_0 - 128 * color_SAT_Power, color_SAT_0 + 128 * color_SAT_Power);
				color_SAT = _sat;
			}
			if (bColor_BRG) {
				_brg = ofRandom(color_BRG_0 - 128 * color_BRG_Power, color_BRG_0 + 128 * color_BRG_Power);
				color_BRG = _brg;
			}

			//ofColor c;
			//c.setSaturation(_sat);
			//c.setBrightness(_brg);
			//c.setHue(_hue);
			//color_Picked = c;

			//color_Picked = ofFloatColor(ofRandom(0., 1.), ofRandom(0., 1.), ofRandom(0., 1.));
		}
	}

	else if (name == "ADD COLOR")
	{
#ifdef USE_RECTANGLE_INTERFACES
		if (bAddColor)
		{
			bAddColor = false;
			palette_addColor(ofColor(color_Picked.get()));

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
#endif
	}

	else if (name == "EDIT COLOR")
	{
#ifdef USE_RECTANGLE_INTERFACES
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
			color_Picked = palette[palette_colorSelected];
		}
#endif
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
	//            random.generateRandom(amountColors_Alg);
	//            update_Engine();
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
	else if (name == bAuto_palette_recall.getName())
	{
		if (bAuto_palette_recall)
			bLock_palette = false;
	}
	else if (name == bLock_palette.getName())
	{
		if (bLock_palette)
			bAuto_palette_recall = false;
	}

	//--

	else if (name == "BRIGHTNESS" || name == "SATURATION")
	{
		update_Engine();

		// auto create user palette from algo palette
		if (bAuto_palette_recall)
		{
			palette_clear();

#ifdef USE_RECTANGLE_INTERFACES
			palette_recallFromPalettes(SELECTED_palette_LAST);//trig last choice
#endif
			// DEMO
			if (DEMO_Test) myDEMO.reStart();
		}
	}

	//--

	// CURVE

	else if (name == "Reset Curve")
	{
		if (bResetCurve)
		{
			bResetCurve = false;

			curvesTool.clear();
			curvesTool.add(ofVec2f(0, 0));
			curvesTool.add(ofVec2f(127, 127));
			curvesTool.add(ofVec2f(255, 255));
			curveMod = 0.5;
			curve_SliderTest.setPercent(curveMod);
		}
	}
	else if (name == "Hard Steps")
	{
		gradient.setHardMode(gradient_HardMode);
	}

	//-

	// BACKGROUND

	else if (name == "Refresh")
	{
		if (color_backGround_SET)
		{
			color_backGround_SET = false;
			color_backGround.set(ofColor(color_Picked.get()));
		}
	}
	else if (name == "AutoSet")
	{

	}
	else if (name == "BG")
	{
		//        if (color_backGround.get().getBrightness()!=backgroundDarkness_PRE)
		//            backgroundDarkness = color_backGround.get().getBrightness();
		//
		////        if (backgroundDarkness_PRE!=backgroundDarkness)
		////            backgroundDarkness = (int)darkness;
	}
	else if (name == "Darkerkness")
	{
		//TODO: must improve
		//update_Engine();

		// auto create user palette from algo palette
		if (bAuto_palette_recall)
		{
			palette_clear();

#ifdef USE_RECTANGLE_INTERFACES
			palette_recallFromPalettes(SELECTED_palette_LAST);//trig last choice
#endif
		}
	}
}

//load user palette from range
//--------------------------------------------------------------
void ofxColorManager::Changed_ColorRange(ofAbstractParameter &e)
{
	std::string name = e.getName();
	ofLogNotice(__FUNCTION__) << name << " : " << e;

	if (false) {}

	//else if (name == c1_Rng.getName())
	//{
	//	guiCol1 = c1_Rng.get();
	//}
	//else if (name == c2_Rng.getName())
	//{
	//	guiCol2 = c2_Rng.get();
	//}

	else if (name == autoPickColor1.getName())
	{
		if (autoPickColor1) autoPickColor2 = false;
	}
	else if (name == autoPickColor2.getName())
	{
		if (autoPickColor2) autoPickColor1 = false;
	}

	else
	{
		// ranges

		for (int i = 0; i < NUM_TYPES_RANGES; i++)
		{
			if (name == rangTypes[i].getName() && rangTypes[i].get())
			{
				rangTypes[i] = false;//off button

				palette_clear();
				int st = i * numColorsRange.get();
				int ed = st + numColorsRange.get();
				for (int j = st; j < ed; j++)
				{
					ofColor c = paletteRange[j];
					ofLogNotice(__FUNCTION__) << "color [" << j << "] " << ofToString(c);
					palette_addColor(c);
				}

				//-

				//index selected
				lastRange = i;
				range_Name = rangTypes[i].getName();

				txt_lineActive[0] = true;//preset name
				txt_lineActive[1] = false;//palette name
				txt_lineActive[2] = false;//theory name
				txt_lineActive[3] = true;//range name

				// DEMO
				if (DEMO_Test) myDEMO.reStart();
			}
		}

		//--

		// algo/theory types

		for (int i = 0; i < 7; i++)
		{
			if (name == algoTypes[i].getName() && algoTypes[i].get())
			{
				algoTypes[i] = false;//off button

				vector<ofColor> _cols;
				palette_clear();

				//-

				theory_Name = algoTypes[i].getName();
				lastTheory = i + NUM_COLOR_THEORY_TYPES;

				txt_lineActive[0] = true;//preset name
				txt_lineActive[1] = false;//palette name
				txt_lineActive[2] = true;//theory name
				txt_lineActive[3] = false;//range name

				// DEMO
				if (DEMO_Test) myDEMO.reStart();

				//-

				switch (i)
				{

				case 0:
					_cols.clear();
					_cols.resize(complement.size());
					for (int j = 0; j < complement.size(); j++) {
						_cols[j] = complement[j];
						palette_addColor(_cols[j]);
					}
					break;

				case 1:
					_cols.clear();
					_cols.resize(complementBrightness.size());
					for (int j = 0; j < complementBrightness.size(); j++) {
						_cols[j] = complementBrightness[j];
						palette_addColor(_cols[j]);
					}
					break;

				case 2:
					_cols.clear();
					_cols.resize(monochrome.size());
					for (int j = 0; j < monochrome.size(); j++) {
						_cols[j] = monochrome[j];
						palette_addColor(_cols[j]);
					}
					break;

				case 3:
					_cols.clear();
					_cols.resize(monochromeBrightness.size());
					for (int j = 0; j < monochromeBrightness.size(); j++) {
						_cols[j] = monochromeBrightness[j];
						palette_addColor(_cols[j]);
					}
					break;

				case 4:
					_cols.clear();
					_cols.resize(analogue.size());
					for (int j = 0; j < analogue.size(); j++) {
						_cols[j] = analogue[j];
						palette_addColor(_cols[j]);
					}
					break;

				case 5:
					_cols.clear();
					_cols.resize(triad.size());
					for (int j = 0; j < triad.size(); j++) {
						_cols[j] = triad[j];
						palette_addColor(_cols[j]);
					}
					break;

				case 6:
					_cols.clear();
					_cols.resize(complementTriad.size());
					for (int j = 0; j < complementTriad.size(); j++) {
						_cols[j] = complementTriad[j];
						palette_addColor(_cols[j]);
					}
					break;

				}
			}
		}
	}
}

#pragma mark - KEYS MOUSE CALLBACKS

//--------------------------------------------------------------
void ofxColorManager::keyPressed(ofKeyEventArgs &eventArgs)
{
	if (ENABLE_keys)
	{
		const int &key = eventArgs.key;

		//--

		ofLogNotice(__FUNCTION__) << " : " << key;

		//----

		// panels

		if (key == OF_KEY_F1)//palette
		{
			SHOW_UserPalette = !SHOW_UserPalette;
		}
		if (key == OF_KEY_F2)//picker
		{
			SHOW_Picker = !SHOW_Picker;
			//if (SHOW_Picker) SHOW_BackGround = false;
		}
		if (key == OF_KEY_F3)//library
		{
			SHOW_Library = !SHOW_Library;
		}
		if (key == OF_KEY_F4)//bg
		{
			SHOW_BackGround = !SHOW_BackGround;
			//if (SHOW_BackGround) SHOW_Picker = false;
		}

		if (key == OF_KEY_F5)//theory
		{
			SHOW_Theory = !SHOW_Theory;
		}
		if (key == OF_KEY_F6)//range
		{
			SHOW_Range = !SHOW_Range;
		}
		if (key == OF_KEY_F7)//lovers
		{
			SHOW_ColourLovers = !SHOW_ColourLovers;
		}
		if (key == OF_KEY_F8)//quantizer
		{
			SHOW_Quantizer = !SHOW_Quantizer;
		}

		if (key == OF_KEY_F9)//curve
		{
			SHOW_Curve = !SHOW_Curve;

			if (SHOW_Curve) {
				SHOW_UserPalette = false;
				SHOW_Picker = false;
				SHOW_Library = false;
				SHOW_BackGround = false;
				SHOW_Theory = false;
				SHOW_Range = false;
				SHOW_ColourLovers = false;
				SHOW_Quantizer = false;
				//SHOW_Presets = false;
			}
		}
		if (key == OF_KEY_F10)//all on
		{
			SHOW_UserPalette = true;// F1
			SHOW_Picker = true;// F2
			SHOW_Curve = true;// F3
			SHOW_Library = true;// F4
			SHOW_BackGround = true;
			SHOW_Theory = true;// F5
			SHOW_Range = true;// F6
			SHOW_ColourLovers = true;// F7
			SHOW_Quantizer = true;// F8
			SHOW_Presets = true;
		}
		if (key == OF_KEY_F11)//all off
		{
			SHOW_UserPalette = false;
			SHOW_Picker = false;
			SHOW_Curve = false;
			SHOW_Library = false;
			SHOW_BackGround = false;
			SHOW_Theory = false;
			SHOW_Range = false;
			SHOW_ColourLovers = false;
			SHOW_Quantizer = false;
			SHOW_Presets = false;
		}

		//----
/*
#ifdef USE_RECTANGLE_INTERFACES
		//// algo palettes

		////TODO:
		////SELECTED_palette is for interface index..
		//if (SHOW_AlgoPalettes && SHOW_Theory && !SHOW_Range && !SHOW_ColourLovers && !SHOW_Quantizer && !SHOW_Presets)
		//{
		//	//presets
		//	if (key == OF_KEY_UP)
		//	{
		//		SELECTED_palette = SELECTED_palette_LAST;
		//		SELECTED_palette--;
		//		if (SELECTED_palette < 0)
		//		{
		//			SELECTED_palette = 0;
		//		}

		//		if (DEMO_Test) myDEMO.reStart();
		//	}
		//	else if (key == OF_KEY_DOWN)
		//	{
		//		SELECTED_palette = SELECTED_palette_LAST;
		//		SELECTED_palette++;
		//		if (SELECTED_palette > NUM_TOTAL_PALETTES - 1)
		//		{
		//			SELECTED_palette = NUM_TOTAL_PALETTES - 1;
		//		}

		//		if (DEMO_Test) myDEMO.reStart();
		//	}

		//	//-

		//	else if (key == OF_KEY_LEFT && !SHOW_Presets)
		//	{
		//		amountColors_Alg--;
		//		amountColors_Alg = (int)ofClamp(amountColors_Alg,
		//			amountColors_Alg.getMin(),
		//			amountColors_Alg.getMax());

		//		if (DEMO_Test) myDEMO.reStart();
		//	}
		//	else if (key == OF_KEY_RIGHT && !SHOW_Presets)
		//	{
		//		amountColors_Alg++;
		//		amountColors_Alg = (int)ofClamp(amountColors_Alg,
		//			amountColors_Alg.getMin(),
		//			amountColors_Alg.getMax());

		//		if (DEMO_Test) myDEMO.reStart();
		//	}
		//}
#endif
*/
//-----

		// presets

		if (SHOW_Presets && !SHOW_Theory && !SHOW_Range && !SHOW_ColourLovers && !SHOW_Quantizer)
		{
			if (key == OF_KEY_UP)
			{
				if (currentFile > 0)
				{
					currentFile--;
				}
				if (currentFile < files.size())
				{
					PRESET_name = files_Names[currentFile];
					ofLogNotice(__FUNCTION__) << "PRESET_name: [" + ofToString(currentFile) + "] " << PRESET_name;
					preset_load(PRESET_name);
				}

				// new preset
				if (MODE_newPreset) MODE_newPreset = false;

				// demo mode
				if (DEMO_Test) myDEMO.reStart();

				//load first color from preset to algorothmic palettes
				if (palette.size() > 0)
				{
					color_Picked = ofFloatColor(palette[0]);
					update_Engine();
				}
			}

			else if (key == OF_KEY_DOWN)
			{
				if (currentFile < files.size() - 1)
				{
					currentFile++;
				}
				if (currentFile < files.size())
				{
					PRESET_name = files_Names[currentFile];
					ofLogNotice(__FUNCTION__) << "PRESET_name: [" + ofToString(currentFile) + "] " << PRESET_name;
					preset_load(PRESET_name);
				}

				// new preset
				if (MODE_newPreset) MODE_newPreset = false;
				// demo mode
				if (DEMO_Test) myDEMO.reStart();
				// load first color
				if (palette.size() > 0)
				{
					color_Picked = ofFloatColor(palette[0]);
					update_Engine();
				}
			}

			// next without clamp
			else if (key == ' ')
			{
				if (currentFile < files.size() - 1)
				{
					currentFile++;
				}
				else
				{
					if (currentFile == files.size() - 1) currentFile = 0;
				}

				if (currentFile < files.size())
				{
					PRESET_name = files_Names[currentFile];
					ofLogNotice(__FUNCTION__) << "PRESET_name: [" + ofToString(currentFile) + "] " << PRESET_name;
					preset_load(PRESET_name);
				}

				// new preset
				if (MODE_newPreset) MODE_newPreset = false;
				// demo mode
				if (DEMO_Test) myDEMO.reStart();
				// load first color
				if (palette.size() > 0)
				{
					color_Picked = ofFloatColor(palette[0]);
					update_Engine();
				}
			}

			// random
			else if (key == 'R' || key == 'r')
			{
				currentFile = (int)ofRandom(0, files.size());

				if (currentFile < files.size())
				{
					PRESET_name = files_Names[currentFile];
					ofLogNotice(__FUNCTION__) << "PRESET_name: [" + ofToString(currentFile) + "] " << PRESET_name;
					preset_load(PRESET_name);
				}

				// new preset
				if (MODE_newPreset) MODE_newPreset = false;
				//demo mode
				if (DEMO_Test) myDEMO.reStart();
				//load first color
				if (palette.size() > 0)
				{
					color_Picked = ofFloatColor(palette[0]);
					update_Engine();
				}
			}

			//----

			//// LOAD
			//else if (key == 'l')
			//{
			//	preset_load(PRESET_name);
			//}
			//// SAVE
			//else if (key == 's')
			//{
			//	preset_save(PRESET_name);
			//}
		}

		//--

		// theory browse

		if (!SHOW_Presets && !SHOW_Theory && SHOW_Range && !SHOW_ColourLovers && !SHOW_Quantizer)
		{
			if (key == OF_KEY_UP)
			{
				lastRange--;
				lastRange = (int)ofClamp(lastRange, 0, NUM_TYPES_RANGES - 1);

				for (int i = 0; i < NUM_TYPES_RANGES; i++)
				{
					rangTypes[i].disableEvents();
					rangTypes[i] = false;
					rangTypes[i].enableEvents();
				}
				rangTypes[lastRange] = true;
			}
			if (key == OF_KEY_DOWN)
			{
				lastRange++;
				lastRange = (int)ofClamp(lastRange, 0, NUM_TYPES_RANGES - 1);

				for (int i = 0; i < NUM_TYPES_RANGES; i++)
				{
					rangTypes[i].disableEvents();
					rangTypes[i] = false;
					rangTypes[i].enableEvents();
				}
				rangTypes[lastRange] = true;
			}
		}

		//--

		// theory

		if (!SHOW_Presets && SHOW_Theory && !SHOW_Range && !SHOW_ColourLovers && !SHOW_Quantizer)
		{
			if (key == OF_KEY_UP)
			{
				lastColorTheoryPicked_Palette--;
				lastColorTheoryPicked_Palette = ofClamp(
					lastColorTheoryPicked_Palette.get(),
					lastColorTheoryPicked_Palette.getMin(),
					lastColorTheoryPicked_Palette.getMax());

				//poweroff
				for (int i = 0; i < NUM_COLOR_THEORY_TYPES; i++)
				{
					theoryTypes[i].disableEvents();
					theoryTypes[i] = false;
					theoryTypes[i].disableEvents();
				}
				//enable
				if (lastColorTheoryPicked_Palette >= 0 &&
					lastColorTheoryPicked_Palette < NUM_COLOR_THEORY_TYPES)
				{
					theoryTypes[lastColorTheoryPicked_Palette] = true;
				}
			}
		}

		//----

		// layout

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

		//else if (key == '0')
		//{
		//	panels.group_Selected = 0;
		//}
		//else if (key == '1')
		//{
		//	panels.group_Selected = 1;
		//}
		//else if (key == '2')
		//{
		//	panels.group_Selected = 2;
		//}
		//else if (key == '3')
		//{
		//	panels.group_Selected = 3;
		//}
		//else if (key == '4')
		//{
		//	panels.group_Selected = 4;
		//}
		//else if (key == '5')
		//{
		//	panels.group_Selected = 5;
		//}
		//else if (key == '6')
		//{
		//	panels.group_Selected = 6;
		//}
		//else if (key == '7')
		//{
		//	panels.group_Selected = 7;
		//}
		//else if (key == '8')
		//{
		//	panels.group_Selected = 8;
		//}
		//else if (key == '9')
		//{
		//	panels.group_Selected = 9;
		//}

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
#endif
		//----

		else if (key == 'G')
		{
			SHOW_GuiInternal = !SHOW_GuiInternal;
	}

		//----

		//else if (key == 'g') {
		//    SHOW_ALL_GUI = !SHOW_ALL_GUI;
		//    setVisible(SHOW_ALL_GUI);
		//}

		//else if (key == 'P')
		//{
		//	bPaletteEdit = !bPaletteEdit;
		//}

		//----

		//edit layout
		else if (key == 'E' || key == 'e')
		{
			MODE_Editor = !MODE_Editor;
		}

		//    else if (key == 'l')
		//    {
		//        bLock_palette = !bLock_palette;
		//    }

		else if (key == 'M')
		{
			mouseRuler.toggleVisibility();

			//myDEMO.toggleMouseCamera();
		}

		//else if (key == 'd')
		//{
		//    // DEBUG INTERFACE
		//    bShowDebug = !bShowDebug;
		//}

		//----

		//TODO:
		//randoms

		// random user palette

		// 1. randomize one color and build palette if enabled

		else if (key == 'o')
		{
			//TODO
			// WORKFLOW: when loading a colour lover palette we disable auto create from algo palettes
			//if (!bAuto_palette_recall)
			//{
			//bAuto_palette_recall = true;
			//}

			//TODO: bug because some trigs are flags. we need direct functions
			//bRandomColor = true;
			color_Picked = ofFloatColor(ofRandom(0., 1.), ofRandom(0., 1.), ofRandom(0., 1.));

			if (bAuto_palette_recall)
			{
				update_Engine();

#ifdef USE_RECTANGLE_INTERFACES
				palette_recallFromPalettes(SELECTED_palette_LAST);//trig last choice
#endif
			}

			//-

			// undo
			color_Undo = color_Picked.get();
			color_Undo.store();

			//-

			// preset manager
			if (!MODE_newPreset) MODE_newPreset = true;

#ifdef USE_RECTANGLE_INTERFACES
			textInput_New = "random_" + btns_plt_Selector[SELECTED_palette_LAST]->getName();
#endif
			//-

			if (DEMO_Test) myDEMO.reStart();
		}

		//----

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

			//random.generateRandom(amountColors_Alg);
			//update_Engine();
			//if (bAuto_palette_recall)
			//{
			//    //set random palette to user palette
			//    palette_recallFromPalettes(7);
			//}

			//-

			// B. get a random palette from PANTONE

			//int _r = (int) ofRandom(NUM_COLORS_PANTONE);
			int r = (int)ofRandom(paletteLibPage * numColorsPage, paletteLibPage * numColorsPage + numColorsPage);
			r = ofClamp(r, 0, NUM_COLORS_PANTONE);
			//color pos & name
			lastColorPicked = r;
			lastColorPickedNameColor = ColorBrowser.pantoneNames[r];
			color_Picked = ofColor(ColorBrowser_palette[r]);
			update_Engine();

#ifdef USE_RECTANGLE_INTERFACES
			palette_recallFromPalettes(SELECTED_palette_LAST);
#endif
			//----

			// undo

			//----

			color_Undo = color_Picked.get();
			color_Undo.store();

			//----

			// presets

			if (!MODE_newPreset) MODE_newPreset = true;

			textInput_New = ColorBrowser.pantoneNames[lastColorPicked] + "_";

#ifdef USE_RECTANGLE_INTERFACES
			textInput_New += btns_plt_Selector[SELECTED_palette_LAST]->getName();
#endif

			//----

			if (DEMO_Test) myDEMO.reStart();
		}

		//--

		//// test
		//if (key == 'T')
		//{
		//	TEST_Mode = !TEST_Mode;
		//}

		//-

		//// DEMO
		//else if (key == 'D')
		//{
		//	DEMO_Test = !DEMO_Test;
		//	if (DEMO_Test) myDEMO.reStart();
		//}

		//--

		// colour-lovers

		if (SHOW_ColourLovers)
		{
			// 3. randomly get a palette from colour lovers only
			if (key == 'r' || key == 'R')
			{
#ifdef USE_COLOR_LOVERS
				colourLoversHelper.randomPalette();
#endif
				textInput_temp = PRESET_name;

				// if (DEMO_Test)
				// myDEMO.reStart();
			}

			//-

			// browse presets
#ifdef USE_COLOR_LOVERS
			if ((SHOW_ColourLovers && key == ' '))
			{
				colourLoversHelper.nextPalette();
			}
#endif
		}

		//----

		// undo color

		if (key == 'z')
		{
			color_Undo.undo();
			color_Picked = color_Undo;
		}
		else if (key == 'y')
		{
			color_Undo.redo();
			color_Picked = color_Undo;
		}
		//    else if()
		//        color_Undo.clearRedo();

		//----

		//    if (key == 's')
		//    {
		//        palette_save("myPalette");
		//    }
		//    if (key == 'l')
		//    {
		//        palette_load("myPalette");
		//    }

		//----

		// user palette

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
		//    color_Picked = ofFloatColor(ofRandom(0., 1.), ofRandom(0., 1.), ofRandom(0., 1.));
		//    palette_addColor(ofColor(color_Picked.get()));
		//}

		//----

		// color browser

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
#ifdef USE_COLOR_LOVERS
	glm::vec2 sizeGui(150, 375);
	glm::vec2 sizeGrid(150, h);
	glm::vec2 posGui(w - (sizeGui.x + sizeGrid.x + 4), 0);
	glm::vec2 posGrid(posGui.x + sizeGui.x + 2, 0);
	colourLoversHelper.setGrid(posGrid, sizeGrid);
	colourLoversHelper.setPosition(posGui, sizeGui);
	colourLoversHelper.windowResized(w, h);
#endif
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

#ifdef USE_RECTANGLE_INTERFACES
	TouchManager::one().touchMove(button, ofVec2f(x, y));
#endif

	//-
}


//--------------------------------------------------------------
void ofxColorManager::mousePressed(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;

#ifdef USE_RECTANGLE_INTERFACES
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
#endif

	//-

	// DEMO

	if (DEMO_Test && !mouseOverGui)
	{
		if (button == 2)//second mouse button cleans DEMO
			myDEMO.clear();
		else
			myDEMO.start();//trig DEMO start
	}
}

//--------------------------------------------------------------
void ofxColorManager::mouseReleased(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;

#ifdef USE_RECTANGLE_INTERFACES
	TouchManager::one().touchUp(button, ofVec2f(x, y));
#endif
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
void ofxColorManager::preset_refreshFiles()
{
	//TODO: why hardcoded?
	//std::string _path = "user_kits/presets";
	std::string path_Global = "ofxColorManager/";
	std::string path_Presets = path_Global + "user_kits/presets/";
	std::string path_Palettes = path_Global + "user_kits/palettes/";
	std::string _path = path_Presets;
	ofxSurfingHelpers::CheckFolder(_path);

	ofDirectory dataDirectory(ofToDataPath(_path, true));
	ofxSurfingHelpers::CheckFolder(_path);

	ofLogNotice(__FUNCTION__) << _path;

	// clear files and filenames vectors
	files.clear();
	files_Names.clear();

	// load all folder files in one call
	files = dataDirectory.getFiles();

	for (size_t i = 0; i < files.size(); i++)
	{
		files_Names.push_back(files[i].getBaseName());

		ofLogNotice(__FUNCTION__) << files_Names[i];
	}

	//-

	//TODO:
	//void to go to 1st...

	// 1. load same position preset
	// if preset is deleted goes to nextone..
	// should check names because sorting changes..
	if (files_Names.size() > 0)
	{
		if (currentFile > files_Names.size() - 1)
			currentFile = files_Names.size() - 1;

		//else if (currentFile > files_Names.size() - 1)

		PRESET_name = files_Names[currentFile];

		ofLogNotice(__FUNCTION__) << PRESET_name;

		preset_load(PRESET_name);
	}
	else
	{
		ofLogError(__FUNCTION__) << "FILE PRESET NOT FOUND!";
	}

	//// 2. always goes to 1st preset 0
	////that's because saving re sort the files
	////and we don't know the position of last saves preset..
	//if (files_Names.size() > 0)
	//{
	//    currentFile = 0;
	//    PRESET_name = files_Names[currentFile];
	//    preset_load(PRESET_name);
	//}
	//else
	//{
	//    ofLogError(__FUNCTION__) << "NOT FOUND ANY FILE PRESET!";
	//}
}

//--------------------------------------------------------------
void ofxColorManager::preset_load(std::string p)
{
	//TODO:
	//if (0)
	{
		ofLogNotice(__FUNCTION__) << p;

		// setup linking pointers to get back on load
		myPresetPalette.setName(p);
		myPresetPalette.setCurveName(PRESET_curveName);
		myPresetPalette.setPalette(palette);

		txt_lineActive[0] = true;//preset name
		txt_lineActive[1] = false;//palette name
		txt_lineActive[2] = false;//theory name
		txt_lineActive[3] = false;//range name

		// 1. load palette preset
		myPresetPalette.preset_load(p);

		//--

		//TODO:

		// 2. curve gradient preset

		//--

		//TODO:

		// apply loaded preset to local system

		palette_clear();

		vector<ofColor> p = myPresetPalette.getPalette();

		for (int i = 0; i < p.size(); i++)
		{
			ofLogNotice(__FUNCTION__) << "Col: " << ofToString(i) << " > " << ofToString(p[i]);
			palette_addColor(p[i]);
		}

		//--

		////TODO:
		////myPresetPalette.setBackgroundColor(color_backGround);//error ofParameter
		//color_backGround = ofColor(myPresetPalette.getBackground());//get directly without pointing

		////TODO
		//// curve & gradient
		//PRESET_curveName = curveName_BACK;
		//std::string *name_BACK;
		//vector<ofColor> *palette_BACK;
		//std::string *curveName_BACK;

		//--

		////workflow
		//if (DEMO_Test) myDEMO.clear();

		//--

		//workflow
		//if (bAutoExportPreset)
		//{
		//	ofLogNotice(__FUNCTION__) << "EXPORT";
		//	saveColors();
		//}
	}
}

//--------------------------------------------------------------
void ofxColorManager::preset_save(std::string p)
{
	ofLogNotice(__FUNCTION__) << p;

	//TODO:
	//this is using pointers.. ?

	myPresetPalette.setName(p);
	myPresetPalette.setCurveName(PRESET_curveName);
	myPresetPalette.setBackgroundColor(color_backGround.get());
	myPresetPalette.setPalette(palette);

	//myPresetPalette.preset_save(PRESET_name);
	myPresetPalette.preset_save(p);
}

#pragma mark - UTILS

//--------------------------------------------------------------
void ofxColorManager::refresh_Picked_Update_To_HSV()
{
	color_HUE = 255 * color_Picked.get().getHue();
	color_SAT = 255 * color_Picked.get().getSaturation();
	color_BRG = 255 * color_Picked.get().getBrightness();
}

//--------------------------------------------------------------
void ofxColorManager::refresh_Picker_Touched()
{
	// workflow

	ofLogNotice(__FUNCTION__) << ofToString(color_Picked);

	if (bGetFromPicker) {
		color_TheoryBase = color_Picked.get();
	}

	if (autoPickColor1) {
		guiCol1 = color_Picked.get();
		c1_Rng = color_Picked.get();
		if (bRangeAutoGenerate) generateRange(c1_Rng, c2_Rng);
	}

	if (autoPickColor2) {
		guiCol2 = color_Picked.get();
		c2_Rng = color_Picked.get();
		if (bRangeAutoGenerate) generateRange(c1_Rng, c2_Rng);
	}

	//-

	////TEST
	//refresh_Picked_Update_To_HSV();//redundant...

	//TODO: 
	//mirror clicked/picked colors
	color_clicked.set(color_Picked);

	//-

	//TODO:
	//palettes
	//primaryColor.set(color_Picked.get());
	update_Theory();

	//--

	// 1. autosave edited color

#ifdef USE_RECTANGLE_INTERFACES
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
#endif

	//-

	// 2. workflow:
	//if enabled, when color picked changes, auto trig and put generated palette to user palette

	if (bAuto_palette_recall)
	{
		update_Engine();

#ifdef USE_RECTANGLE_INTERFACES
		palette_recallFromPalettes(SELECTED_palette_LAST);//trig last choiced algorithmic palette
#endif
	}

	// 3. recreate algorithmic palettes if locking is disable

	if (!bLock_palette)
	{
		update_Engine();
	}

	//-

	////TODO
	//// palettes
	////primaryColor.set(color_Picked.get());
	//update_Theory();
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
	if (!MODE_Editor) {
		curve_Ctrl_In = control;
		curve_SliderTweak.setPercent(control);
	}
}

//--------------------------------------------------------------
vector<ofColor> ofxColorManager::getPalette()
{
	ofLogNotice(__FUNCTION__);
	for (int i = 0; i < palette.size(); i++)
	{
		ofLogNotice(__FUNCTION__) << ofToString(palette[i]);
	}

	return palette;
}

//--------------------------------------------------------------
void ofxColorManager::setToggleVisible()
{
	SHOW_ALL_GUI = !SHOW_ALL_GUI;

	if (SHOW_Curve)
	{
		curve_SliderTweak.setVisible(SHOW_ALL_GUI);
		curve_SliderTest.setVisible(SHOW_ALL_GUI);
	}
}

//--------------------------------------------------------------
void ofxColorManager::setVisible(bool b)
{
	SHOW_ALL_GUI = b;
}

//--------------------------------------------------------------
ofColor ofxColorManager::getColorAtPercent(float control)
{
	float out = ofMap(curvesTool.getAtPercent(1.0 - control), 0, cAmt - 1, 1., 0.);
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
	background_Draw_ENABLE = b;
}

//--------------------------------------------------------------
void ofxColorManager::draw_GradientPreview(glm::vec2 pos, bool horizontal = true)
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
	//TODO:
	//make export dialog to set path for export outside /data
	//path_Colors = "/Users/manu/Documents/openFrameworks/addons/ofxFontAnimator/4_ofxFontAnimatorNoise/bin/data/colors/liveColors.json";

	ofxSurfingHelpers::CheckFolder(path_Folder_Color);

	ofLogNotice(__FUNCTION__) << "path: " << path_Colors;

	//ofxSerializer
	ofJson j = palette;
	ofSavePrettyJson(path_Colors, j);
	ofLogNotice(__FUNCTION__) << "\n" << ofToString(palette);
}

//--------------------------------------------------------------
void ofxColorManager::generateRange(ofColor col1, ofColor col2) {
	ofLogNotice(__FUNCTION__);

	paletteRange.clear();

	int max = 400;

	int step = max / 10;
	//int step = max / (int)(numColorsRange.get() - 1);//hangs!

	//for (auto i = 0; i < 12; ++i)
	for (int i = 0; i < NUM_TYPES_RANGES; ++i)
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
