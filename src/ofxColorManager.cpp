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
void ofxColorManager::startup()
{
	// preset manager

	preset_filesRefresh();

	//TODO:
	preset_load(PRESET_name);

	//-

	palettes_setVisible(SHOW_AlgoPalettes);

	//----

	// ofxGuiPanelsLayout

	// ofxGuiPanelsLayout

	//// 3. toggles

	//p_TOGGLES.setName("TOGGLES PREVIEW");
	//p_TOGGLES.add(SHOW_Panels);
	//p_TOGGLES.add(SHOW_ColourLovers);
	//p_TOGGLES.add(SHOW_ColourLovers_searcher);
	//p_TOGGLES.add(SHOW_AlgoPalettes);
	//p_TOGGLES.add(SHOW_BrowserColors);
	//p_TOGGLES.add(SHOW_Gradient);
	//p_TOGGLES.add(SHOW_Curve);
	//p_TOGGLES.add(SHOW_TEST_Curve);
	//p_TOGGLES.add(SHOW_ImGui);
	//p_TOGGLES.add(SHOW_BackGround);
	//p_TOGGLES.add(SHOW_Picker);
	//p_TOGGLES.add(SHOW_UserPalette);
	//p_TOGGLES.add(SHOW_Quantizer);
	//p_TOGGLES.add(SHOW_Range);
	//p_TOGGLES.add(SHOW_Theory);
	//p_TOGGLES.add(SHOW_Presets);
	////p_TOGGLES.add(SHOW_CosineGradient);
	//p_TOGGLES.add(SHOW_ALL_GUI);
	//p_TOGGLES.add(SHOW_GUI_MINI);
	//p_TOGGLES.add(TEST_DEMO);
	//p_TOGGLES.add(SHOW_debugText);

	//-

	// add panels to manager
#ifdef INCL_LAYOUT
	panels.addToggle(&SHOW_ImGui);
	panels.addToggle(&SHOW_Panels);
	panels.addToggle(&SHOW_UserPalette);
	panels.addToggle(&SHOW_BackGround);
	panels.addToggle(&SHOW_Picker);
	panels.addToggle(&SHOW_Library);
	panels.addToggle(&SHOW_Range);
	panels.addToggle(&SHOW_Theory);
	panels.addToggle(&SHOW_ColourLovers);
	panels.addToggle(&SHOW_ColourLovers_searcher);
	panels.addToggle(&SHOW_AlgoPalettes);
	panels.addToggle(&SHOW_BrowserColors);
	panels.addToggle(&SHOW_Gradient);
	panels.addToggle(&SHOW_Curve);
	panels.addToggle(&SHOW_ALL_GUI);
	panels.addToggle(&SHOW_GUI_MINI);
	panels.addToggle(&SHOW_debugText);
	panels.addToggle(&SHOW_TEST_Curve);
	panels.addToggle(&SHOW_Quantizer);
	panels.addToggle(&SHOW_Presets);
	panels.addToggle(&TEST_DEMO);
	//panels.addToggle(&SHOW_CosineGradient);

	//call after add the panels
	panels.setup();

	//workflow
	panels.group_Selected = 0;
#endif

	//--
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
#ifdef USE_IMAGE_QUANTIZER
	colorQuantizer.setup();
	colorQuantizer.setBottomMode(true);
	colorQuantizer.setMinimal(true);
	colorQuantizer.setVisible(false);
	// ignore y position and put at the window bottom
	//colorQuantizer.setPosition(glm::vec2(0, 400));

	// receivers pointers
	colorQuantizer.setPalette_BACK(myPalette);
	colorQuantizer.setPalette_BACK_Refresh(bUpdated_Palette_BACK);
	colorQuantizer.setPalette_BACK_Name(myPalette_Name);
#endif

	//--

	// ColorWheel

	palettes_colorTheory_setup();

	//--

	myPresetManager.setup();

	//--

	// DEMO 1

	myDEMO_palette.setup();
	myDEMO_palette.setPalette(palette);

	//--

	// mouse debugger

	mouseRuler.setup();
	mouseRuler.toggleVisibility();

	//-

	// colour lovers
#ifdef USE_COLOR_LOVERS
// set positions and panel sizes
	glm::vec2 sizeGui(150, 375);
	glm::vec2 sizeGrid(150, ofGetHeight());
	glm::vec2 posGui(ofGetWidth() - (sizeGui.x + sizeGrid.x + 4), 0);
	glm::vec2 posGrid(posGui.x + sizeGui.x + 2, 0);

	// must be called before setup() to overwrite default settings
	ColourLoversHelper.setGrid(posGrid, sizeGrid);
	ColourLoversHelper.setup(posGui, sizeGui);

	//-

	// pointers back

	ColourLoversHelper.setColor_BACK(myColor);
	ColourLoversHelper.setColor_BACK_Refresh(bUpdated_Color_BACK);
	ColourLoversHelper.setPalette_BACK(myPalette);
	ColourLoversHelper.setPalette_BACK_Name(myPalette_Name);
	ColourLoversHelper.setPalette_BACK_Refresh(bUpdated_Palette_BACK);
#endif

	// some initiation values..
	myColor = ofColor::white;
	myPalette.resize(2);//pointer setter will clear/resize. nevermind the vector size here
	myPalette[0] = ofColor::white;
	myPalette[0] = ofColor::white;
	myPalette_Name = "NOT LOADED";

	//-

	// layout

	gui_setup_layout();

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
	color_backGround.set("BG COLOR", ofColor(8));
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

	MODE_TweakSatBrg.set("TWEAK SAT/BRT", false);
	SATURATION.set("SATURATION", 200, 0, 255);
	BRIGHTNESS.set("BRIGHTNESS", 200, 0, 255);
	//bRandomPalette.set("RANDOM PALETTE", false);
	bAuto_palette_recall.set("AUTO LIVE MODE", false);
	bLock_palette.set("LOCK PALETTES", false);
	amountColors_Alg.set("Amnt Colors", 6, 2, 10);
	params_palette.setName("ADVANCED");
	params_palette.add(MODE_TweakSatBrg);
	params_palette.add(SATURATION);
	params_palette.add(BRIGHTNESS);
	//params_palette.add(bRandomPalette);
	params_palette.add(amountColors_Alg);
	params_palette.add(bAuto_palette_recall);
	params_palette.add(bLock_palette);

	ofAddListener(params_palette.parameterChangedE(), this, &ofxColorManager::Changed_Controls);

	//TODO: add ColorWheelSchemes alternative

	//-

	// INTERFACE
#ifdef USE_RECTANGLE_INTERFACES
	interface_setup();
#endif

	//-

	// ALGORITHMIC PALETTES

	//random.generateRandom(amountColors_Alg);
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
	//params_control.add(color_Picked);
	params_control.add(bRandomColor);
	params_control.add(bPaletteEdit);
	params_control.add(bAddColor);
	params_control.add(bRemoveColor);
	params_control.add(bClearPalette);
	//params_control.add(SHOW_PaletteCustom);
	//params_control.add(color_backGround);

	//-

	// CONTROL WINDOWS

	SHOW_ColourLovers_searcher.set("SHOW COLOUR LOVERS SEARCH", true);
	SHOW_AlgoPalettes.set("SHOW PALETTES", true);
	SHOW_BrowserColors.set("SHOW BROWSER COLORS", true);
	//SHOW_CosineGradient.set("SHOW COSINE GRADIENT", true);

	SHOW_ColourLovers.set("COLOUR-LOVERS", true);
	SHOW_Gradient.set("GRADIENT", true);
	SHOW_Curve.set("CURVE", true);

	SHOW_Presets.set("PRESETS", true);
	SHOW_BackGround.set("BACKGROUND", true);
	SHOW_Picker.set("PICKER", true);
	SHOW_Library.set("LIBRARY", false);
	SHOW_Range.set("RANGE", true);
	SHOW_Panels.set("PANELS", true);

	params_control.add(SHOW_ColourLovers_searcher);
	params_control.add(SHOW_ColourLovers);
	params_control.add(SHOW_AlgoPalettes);
	params_control.add(SHOW_BrowserColors);
	params_control.add(SHOW_Gradient);
	params_control.add(SHOW_Curve);

	params_control.add(SHOW_Presets);
	params_control.add(SHOW_BackGround);
	params_control.add(SHOW_Library);
	params_control.add(SHOW_Picker);
	params_control.add(SHOW_Range);
	params_control.add(SHOW_Theory);
	params_control.add(SHOW_Panels);
	params_control.add(SHOW_UserPalette);
	params_control.add(SHOW_Quantizer);
	//params_control.add(SHOW_CosineGradient);

	//-

	//palette

	boxSizeUser.set("Box Size", 40, 10, 200);
	boxRowsUser.set("Max Rows", 10, 1, 20);
	boxScale.set("Scale", 1.f, 0.25f, 1.25f);
	bEditUserPalette.set("EDIT", false);
	//bUserPaletteVertical.set("VERTICAL", false);
	bFlipUserPalette.set("FLIP", false);

	params_UserPalette.setName("USER PALETTE");
	params_UserPalette.add(bEditUserPalette);
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
	autoPickColor1.set("Auto Pick From", true);
	autoPickColor2.set("Auto Pick To", false);
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

	// BACKGROUND

	params_control.add(color_backGround_SET);
	params_control.add(color_backGround_SETAUTO);
	params_control.add(color_backGround_Darker);
	params_control.add(backgroundDarkness);

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
	ofxSurfingHelpers::ImGui_ThemeMoebiusSurfing();
	//ofxSurfingHelpers::ImGui_ThemeModernDark();
#endif

	//-

	guiVisible = true;

	//--

	// LISTENERS

	addKeysListeners();
	addMouseListeners();

	color_Picked.addListener(this, &ofxColorManager::Changed_ColorPicked);
	color_clicked_param.addListener(this, &ofxColorManager::Changed_ColorClicked);

	//-

	SHOW_ALL_GUI.setName("SHOW_ALL_GUI");
	SHOW_UserPalette.setName("PALETTE");
	SHOW_Theory.setName("THEORY");
	SHOW_GUI_MINI.setName("SHOW_GUI_MINI");
	SHOW_debugText.setName("SHOW debug");
	SHOW_TEST_Curve.setName("TEST CURVE");
	SHOW_Quantizer.setName("PICTURE CAPTURE");

	//------------------------------------------------------

	// startup settings

	XML_params.setName(__FUNCTION__);

	XML_params.add(SHOW_Presets);
	XML_params.add(SHOW_BackGround);
	XML_params.add(SHOW_Library);
	XML_params.add(SHOW_Picker);
	XML_params.add(SHOW_Range);
	XML_params.add(SHOW_Theory);
	XML_params.add(SHOW_Panels);
	XML_params.add(SHOW_Quantizer);
	XML_params.add(SHOW_ColourLovers);
	XML_params.add(SHOW_ColourLovers_searcher);
	XML_params.add(SHOW_AlgoPalettes);
	XML_params.add(SHOW_Gradient);
	XML_params.add(SHOW_Curve);
	XML_params.add(SHOW_TEST_Curve);//curve tool
	XML_params.add(SHOW_BrowserColors);
	XML_params.add(SHOW_Layout_Gui);
	//XML_params.add(SHOW_CosineGradient);

	XML_params.add(color_Picked);
	XML_params.add(color_backGround);
	XML_params.add(color_backGround_Darker);
	XML_params.add(color_backGround_SETAUTO);
	XML_params.add(backgroundDarkness);

	XML_params.add(bPaletteEdit);//user palette
	XML_params.add(MODE_TweakSatBrg);//algorithmic palette
	XML_params.add(amountColors_Alg);
	XML_params.add(BRIGHTNESS);
	XML_params.add(SATURATION);

	XML_params.add(gradient_hard);//gradient
	XML_params.add(bAuto_palette_recall);
	XML_params.add(paletteLibPage_param);

	XML_params.add(bColor_HUE);
	XML_params.add(bColor_SAT);
	XML_params.add(bColor_BRG);

	XML_params.add(color_HUE_0);
	XML_params.add(color_SAT_0);
	XML_params.add(color_BRG_0);

	XML_params.add(color_HUE_Power);
	XML_params.add(color_SAT_Power);
	XML_params.add(color_BRG_Power);

	XML_params.add(paletteLibPage);

	XML_params.add(bAuto_palette_recall);
	XML_params.add(boxRowsUser);
	XML_params.add(boxScale);
	XML_params.add(SHOW_UserPalette);

	//XML_params.add(TEST_MODE);

	//--

	ofxSurfingHelpers::loadGroup(XML_params, XML_path);

	//------------------------------------------------

	// startup

	startup();
}

//--------------------------------------------------------------
void ofxColorManager::update(ofEventArgs & args)
{

	////COSINE GRADIENT
	//cosineGradient_update();

	//-

//	// WINDOW TITLE
//#ifdef INCL_LAYOUT
//	std::string str;
//	str += ("[PAGE " + ofToString(panels.group_Selected) + "] ");
//	//str += ofToString((int)ofGetFrameRate()) + "FPS";
//	ofSetWindowTitle(str);
//#endif

	//-

	// preset manager

	myPresetManager.update();

	//--

	// ofxGuiPanelsLayout
	//#define USE_IMAGE_QUANTIZER
#ifdef INCL_LAYOUT
	panels.update();
#endif

	//--

	// DEMO

	if (TEST_DEMO) myDEMO_palette.update();

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

	// colour lovers

#ifdef USE_COLOR_LOVERS
	ColourLoversHelper.update();
#endif

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
#endif

		// 2. get color from colour lovers
		color_clicked = ofColor(myColor);

#ifdef USE_COLOR_LOVERS
		// 3. auto create user palette from algo palette from colour lover picked color
		if (!ColourLoversHelper.MODE_PickPalette_BACK && ColourLoversHelper.MODE_PickColor_BACK)
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
		if (TEST_DEMO) myDEMO_palette.reStart();
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
				palettes_update();

#ifdef USE_RECTANGLE_INTERFACES
				palette_recallFromPalettes(SELECTED_palette_LAST);//trig last choice
#endif
			}
		}
	}

	//-

#ifdef USE_RECTANGLE_INTERFACES
	//interface_update();
#endif

	//TODO:
	//Windows
	curveTool_update();
	ColorBrowser.update();
}

//--------------------------------------------------------------
void ofxColorManager::draw(ofEventArgs & args)
{
	// background

	if (backgroundENABLE) ofClear(ofColor(color_backGround.get()));

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

	// presets

	if (SHOW_Presets)
	{
		//myPresetPalette.draw();
		myPresetManager.draw();
	}

	//--

	// DEMO

	if (TEST_DEMO) myDEMO_palette.draw();

	//--

	//if (SHOW_ALL_GUI)
	//{
	//    ofPushMatrix();
	//    ofPushStyle();

	//-

	// COLOR BOX PICKER (CURRENT)

	//ofPushStyle();
	//ofFill();
	//ofSetColor(ofColor( color_Picked.get() ));
	//ofDrawRectangle(r_color_picked);
	//ofPopStyle();

	//-

	// COLOR BOX CLICKED

	//ofPushStyle();
	//ofFill();
	//
	//ofSetColor( ofColor( color_clicked ) );
	////ofSetColor( ofColor( color_clicked_param.get() ) );
	//
	//ofDrawRectangle(r_color_clicked);
	//ofPopStyle();

	//-

	// interface
#ifdef USE_RECTANGLE_INTERFACES
//interface_draw();
#endif

	//-

	// CURVE

	if (SHOW_Curve)
	{
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

		curveTool_draw();
	}

	//-

	// CURVE

	////COSINE GRADIENT
	//if (SHOW_Gradient)
	//{
	//    gradient_draw();
	//}

	// COLORS BROWSER
	if (SHOW_BrowserColors)
	{
		ColorBrowser.draw();
	}

#ifdef USE_COLOR_LOVERS
	//// COLOR lovers
	//if (SHOW_ColourLovers || SHOW_ColourLovers_searcher)
	//{
	//	ColourLoversHelper.draw();
	//}
	// debug
	//        if (SHOW_ColourLovers) {
	//            colourLovers_drawPreview();
	//        }
#endif

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
			mouseOverGui = gui_Draw();

			////TODO
			//gui_Draw();
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
#ifdef USE_IMAGE_QUANTIZER
			if (SHOW_Quantizer && !ENABLE_keys)
			{
				colorQuantizer.setActive(false);
			}
			else if (SHOW_Quantizer && ENABLE_keys)
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

#ifdef USE_IMAGE_QUANTIZER
	if (SHOW_Quantizer) colorQuantizer.draw();
#endif

	//--

	////TODO:
	//std::string strKeys = "COLOUR LOVERS KEYS: ";
	//strKeys += (ColourLoversHelper.ENABLER_Keys ? "ENTER/UNFOCUS":"FOCUS/LOAD");
	//std::string strKeys2 = "MOUSE OVER GUI: ";
	//strKeys2 += (ENABLE_keys ? "FALSE":"TRUE");
	//ofDrawBitmapStringHighlight( strKeys, glm::vec2(500,ofGetHeight()-40) );
	//ofDrawBitmapStringHighlight( strKeys2, glm::vec2(500,ofGetHeight()-20) );
}

//--------------------------------------------------------------
ofxColorManager::~ofxColorManager()
{
	//exit();
}

//--------------------------------------------------------------
void ofxColorManager::exit()
{
	//palette_save("myPalette");
	ofxSurfingHelpers::saveGroup(XML_params, XML_path);

	//-

	ColorBrowser.exit();
#ifdef USE_COLOR_LOVERS
	ColourLoversHelper.exit();
#endif

#ifdef USE_IMAGE_QUANTIZER
	colorQuantizer.exit();
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

#pragma mark - INTERFACE

#ifdef USE_RECTANGLE_INTERFACES

//--------------------------------------------------------------
void ofxColorManager::interface_setup()
{
	scene = new Node();
	scene->setSize(ofGetWidth(), ofGetHeight());
	scene->setName("Scene");
	TouchManager::one().setup(scene);
}

//--------------------------------------------------------------
void ofxColorManager::palette_rearrenge()
{
	ofLogVerbose(__FUNCTION__) << "size: " << btns_palette.size();

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
	//static int colBoxSize = 45;
	static int colBoxSize = 37;

	if (ofxImGui::BeginWindow("THEORY", mainSettings, false))
	{
		if (ofxImGui::AddParameter(colorTheoryBase))
		{
			ofLogNotice(__FUNCTION__) << "PICKER THEORY MOVED !";
			color_Picked = colorTheoryBase.get();
		}

		//ImGui::SameLine();
		ofxImGui::AddParameter(bGetFromPicker);

		if (ofxImGui::AddParameter(amountColors)) {
			palettes_colorTheory_setup();
		}

		//ImGui::InputInt(amountColors.getName().c_str(), (int *)&amountColors.get());
		//ofxImGui::AddParameter(amountColors_Alg);

		//ofxImGui::AddParameter(colorScheme);
		//ofxImGui::AddParameter(colorSchemeName);

		// algorithmic palettes
		if (ImGui::CollapsingHeader("ADVANCED"))
		{
			//ImGui::PushItemWidth(guiWidth * 0.5);
			//ofxImGui::AddParameter(this->amountColors_Alg);
			ofxImGui::AddParameter(MODE_TweakSatBrg);

			if (MODE_TweakSatBrg)
			{
				//ImGui::PushItemWidth(guiWidth * 0.5);

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

				//ofxSurfingHelpers::AddBigButton
			}
			//ofxImGui::AddParameter(this->bRandomPalette);
			ofxImGui::AddParameter(bLock_palette);
			ofxImGui::AddParameter(bAuto_palette_recall);
			//ImGui::PopItemWidth();
		}

		//--

		ImGui::Dummy(ImVec2(0.0f, 5));

		//--

		for (int i = 0; i < NUM_COLOR_THEORY_TYPES; i++)
		{
			//std::string _label = ColorWheelSchemes::colorSchemeNames[i];
			ofxSurfingHelpers::AddSmallButton(theoryTypes[i], 150, colBoxSize);
			ImGui::SameLine();

			//-

			const size_t _total = colorsTheory[i].size();
			for (int n = 0; n < _total; n++)
			{
				ImGui::PushID(n);

				//// draw border to selected color
				//bool bDrawBorder = false;
				//if (n == lastColorTheoryPicked_Palette)
				//{
				//	bDrawBorder = true;
				//}
				//if (bDrawBorder)
				//{
				//	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1, 1, 1, .40));
				//	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
				//}

				//-			

				ImGui::SameLine();

				if (ImGui::ColorButton("##paletteTheory",
					colorsTheory[i][n],
					ImGuiColorEditFlags_NoAlpha |
					ImGuiColorEditFlags_NoPicker |
					ImGuiColorEditFlags_NoTooltip,
					ImVec2(colBoxSize, colBoxSize)))
				{
					lastColorTheoryPicked_Palette = n;

					//color = ImVec4(palette[n].x, palette[n].y, palette[n].z, palette.w); // Preserve alpha!
					//color_Picked = color;
				}

				////draw border to selected color
				//if (bDrawBorder)
				//{
				//	ImGui::PopStyleColor();
				//	ImGui::PopStyleVar(1);
				//}

				//ImGui::PopItemWidth();
				ImGui::PopID();
			}
		}

		//-

		//extra
		for (int i = 0; i < 7; i++)
		{
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

			ofxSurfingHelpers::AddSmallButton(algoTypes[i], 150, colBoxSize);
			ImGui::SameLine();

			for (int n = 0; n < _total; n++)
			{
				ImGui::PushID(n);

				ofColor c;

				switch (i)
				{
				case 0:
					c = complement[n];
					break;
				case 1:
					c = complementBrightness[n];
					break;
				case 2:
					c = monochrome[n];
					break;
				case 3:
					c = monochromeBrightness[n];
					break;
				case 4:
					c = analogue[n];
					break;
				case 5:
					c = triad[n];
					break;
				case 6:
					c = complementTriad[n];
					break;
				}

				ImGui::SameLine();

				if (ImGui::ColorButton("##paletteAlgorithm",

					c,
					ImGuiColorEditFlags_NoAlpha |
					ImGuiColorEditFlags_NoPicker |
					ImGuiColorEditFlags_NoTooltip,
					ImVec2(colBoxSize, colBoxSize)))
				{
					lastColorTheoryPicked_Palette = n + colorsTheory[i].size();
				}

				ImGui::PopID();
			}
		}

		////-

		////extra
		////same but with bright/saturation
		//for (int i = 0; i <= 7; i++)
		//{
		//	ofxSurfingHelpers::AddSmallButton(theoryTypes[i], 150, colBoxSize);
		//	ImGui::SameLine();

		//	size_t _total;

		//	switch (i)
		//	{
		//	case 0:
		//		_total = colors_Analogous.size();
		//		break;
		//	case 1:
		//		_total = colors_Complementary.size();
		//		break;
		//	case 2:
		//		_total = colors_SplitComplementary.size();
		//		break;
		//	case 3:
		//		_total = colors_Compound.size();
		//		break;
		//	case 4:
		//		_total = colors_FlippedCompound.size();
		//		break;
		//	case 5:
		//		_total = colors_Monochrome.size();
		//		break;
		//	case 6:
		//		_total = colors_Tetrad.size();
		//		break;
		//	case 7:
		//		_total = colors_Triad.size();
		//		break;
		//	}

		//	for (int n = 0; n < _total; n++)
		//	{
		//		ImGui::PushID(n);

		//		ofColor c;

		//		switch (i)
		//		{
		//		case 0:
		//			c = colors_Analogous[n];
		//			break;
		//		case 1:
		//			c = colors_Complementary[n];
		//			break;
		//		case 2:
		//			c = colors_SplitComplementary[n];
		//			break;
		//		case 3:
		//			c = colors_Compound[n];
		//			break;
		//		case 4:
		//			c = colors_FlippedCompound[n];
		//			break;
		//		case 5:
		//			c = colors_Monochrome[n];
		//			break;
		//		case 6:
		//			c = colors_Tetrad[n];
		//			break;
		//		case 7:
		//			c = colors_Triad[n];
		//			break;
		//		}

		//		ImGui::SameLine();

		//		if (ImGui::ColorButton("##paletteAlgorith",

		//			c,
		//			ImGuiColorEditFlags_NoAlpha |
		//			ImGuiColorEditFlags_NoPicker |
		//			ImGuiColorEditFlags_NoTooltip,
		//			ImVec2(colBoxSize, colBoxSize)))
		//		{
		//			lastColorTheoryPicked_Palette = n + colorsTheory[i].size();
		//		}

		//		ImGui::PopID();
		//	}
		//}
	}
	ofxImGui::EndWindow(mainSettings);
}

//--------------------------------------------------------------
void ofxColorManager::gui_Palette()
{
	// box size
	//int colBoxSize = boxSizeUser.get();

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

		if (bEditUserPalette)
		{
			//ofxImGui::AddParameter(bUserPaletteVertical);
			//ofxImGui::AddParameter(boxSizeUser);
			//ofxImGui::AddParameter(boxRowsUser);

			boxRowsUser.disableEvents();
			ImGui::InputInt(boxRowsUser.getName().c_str(), (int *)&boxRowsUser.get());
			boxRowsUser = ofClamp(boxRowsUser.get(), boxRowsUser.getMin(), boxRowsUser.getMax());
			boxRowsUser.enableEvents();

			ImGui::InputFloat(boxScale.getName().c_str(), (float *)&boxScale.get(), 0.005f, 0.010);

			//-

			if (ImGui::RadioButton("Copy", mode == Mode_Copy)) { mode = Mode_Copy; } ImGui::SameLine();
			if (ImGui::RadioButton("Move", mode == Mode_Move)) { mode = Mode_Move; } ImGui::SameLine();
			if (ImGui::RadioButton("Swap", mode == Mode_Swap)) { mode = Mode_Swap; }

			ofxSurfingHelpers::AddSmallButton(bFlipUserPalette);
		}

		//-

		{
			for (int n = 0; n < palette.size(); n++)
			{
				ImGui::PushID(n);

				//if (!bUserPaletteVertical) 
				{
					//split in rows
					if (boxRowsUser != 0) {
						if ((n % boxRowsUser) != 0)
							if (n != 0)
							{
								ImGui::SameLine();
							}
					}
				}
				//else
				//{
				//	//split in rows
				//	if (boxRowsUser != 0) {
				//		if ((n % boxRowsUser) != 0)
				//			if (n != 0)
				//			{
				//				ImGui::SameLine();
				//			}
				//	}

				//}

				//ImGuiColorEditFlags _flags =
				//	ImGuiColorEditFlags_NoSmallPreview |
				//	ImGuiColorEditFlags_NoTooltip |
				//	ImGuiColorEditFlags_NoLabel |
				//	ImGuiColorEditFlags_NoSidePreview |
				//	ImGuiColorEditFlags_HSV |
				//	ImGuiColorEditFlags_RGB |
				//	ImGuiColorEditFlags_NoInputs |
				//	ImGuiColorEditFlags_NoAlpha |
				//	ImGuiColorEditFlags_PickerHueWheel;

				ImGuiColorEditFlags colorEdiFlags = false;

				//static ImVec4 color;
				//color = color_Picked.get();
				//if (ImGui::ColorButton("##paletteDrag", (float*)&color, _flags,

				int wb;
				//wb = colBoxSize;

				int _r = MIN(boxRowsUser.get(), palette.size());
				//wb = ImGui::GetWindowContentRegionWidth();
				wb = ImGui::GetWindowContentRegionWidth() / _r;

				float _spc = ImGui::GetStyle().ItemInnerSpacing.x;
				wb = wb - _spc;
				wb -= 5;

				wb = wb * boxScale.get();

				//box
				if (ImGui::ColorButton("##paletteDrag",
					palette[n],
					colorEdiFlags,
					ImVec2(wb, wb)))
				{
					lastColorPicked_Palette = n;
				}

				// Our buttons are both drag sources and drag targets here!
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
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::PopID();
			}
		}

		//--

		//edit

		ofxImGui::AddParameter(bEditUserPalette);

		//-

		ImGui::Dummy(ImVec2(0.0f, 10));

		//-
	}
	ofxImGui::EndWindow(mainSettings);
}

//--------------------------------------------------------------
void ofxColorManager::gui_Library()
{
	if (ofxImGui::BeginWindow("LIBRARY", mainSettings, false))
	{
		//if (ImGui::CollapsingHeader("PALETTE LIBRARY"))
		{
			// get clicked color
			static int colBoxSize;

			// layout by pages groups
			doublePage = true;

			//numLines = doublePage ? 20 : 10;

			//numLines = 30;
			//colBoxSize = 25;

			numLines = 25;
			colBoxSize = 25;

			//numLines = 30;
			//colBoxSize = 22;

			//-

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

			//// layout by pages groups
			//doublePage = true;

			////numLines = doublePage ? 20 : 10;
			//numLines = 20;
			////numLines = 30;

			numColorsPage = numLines * rowSizePal;
			maxPages = totalNumColors / (numColorsPage - 1);
			paletteLibPage.setMax(maxPages - 1);

			int startColorInPal = paletteLibPage * numColorsPage;
			int endColorInPal = startColorInPal + numColorsPage;
			//paletteLibPage = paletteLibPage_param.get();

			//-

			ImGui::Dummy(ImVec2(0.0f, 5));

			ImGui::Text("PANTONE (c)");

			//ImGui::Dummy(ImVec2(0.0f, 5));

			// name color
			// load tab2 with lastColorPickedNameColor
			char tab2[32];
			strncpy(tab2, lastColorPickedNameColor.c_str(), sizeof(tab2));
			tab2[sizeof(tab2) - 1] = 0;
			ImGui::PushItemWidth(guiWidth * 0.5);
			ImGui::Text("%s", tab2);//color name label

			//ImGui::SameLine();

			ImGui::Dummy(ImVec2(0.0f, 5));

			//-

			// arrow buttons
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

			ImGui::PopItemWidth();

			ImGui::Dummy(ImVec2(0.0f, 5));

			//-

			// 2.2 draw all palette colors grid

			for (int n = startColorInPal; n < endColorInPal; n++)
			{
				if (n < totalNumColors) //to avoid empty colors at page end...
				{
					// ImGui::PushItemWidth(guiWidth * 0.2);
					ImGui::PushID(n);

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
						color_Picked = color;

						//color name
						lastColorPickedNameColor = "'" + ColorBrowser.pantoneNames[n] + "'";
						std::string str = "ImGui: PALETTE PICKED: [" + ofToString(lastColorPicked) + "] " + lastColorPickedNameColor;

						ofLogNotice(__FUNCTION__) << str;
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
void ofxColorManager::gui_Picker()
{
	if (ofxImGui::BeginWindow("PICKER", mainSettings, false))
	{
		// get color from outside color picked

		bCallback_ENABLED = false;//maybe required because get() causes callbacks too (?)
		static ImVec4 color;
		color = color_Picked.get();
		bCallback_ENABLED = true;

		//--

		// 0. color big

		int _w = ImGui::GetWindowContentRegionWidth();
		int _h = 85;
		int _flags = ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoTooltip;

		ImGui::ColorButton("MyColor##Picker", *(ImVec4 *)&color, _flags, ImVec2(_w, _h));

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

			if (ImGui::ColorPicker4("MyColor##Wheel", (float *)&color, colorEdiFlags))
			{
				ofLogNotice(__FUNCTION__) << "PICKER 1 MOVED !";
				color_Picked = color;
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
				ofLogNotice(__FUNCTION__) << "PICKER 2 MOVED !";
				color_Picked = color;
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
				update_color_picked_CHANGES();
			}
			if (ofxImGui::AddParameter(color_SAT))
			{
				ofLogNotice(__FUNCTION__) << "ImGui: SAT MOVED !";
				ofColor c;
				c.set(color_Picked.get());
				c.setSaturation(color_SAT);
				color_Picked.set(c);
				update_color_picked_CHANGES();
			}
			if (ofxImGui::AddParameter(color_BRG))
			{
				ofLogNotice(__FUNCTION__) << "ImGui: BRG MOVED !";
				ofColor c;
				c.set(color_Picked.get());
				c.setBrightness(color_BRG);
				color_Picked.set(c);
				update_color_picked_CHANGES();
			}
			bCallback_ENABLED = true;

			//-

			ImGui::PopItemWidth();

			ofxImGui::EndTree(mainSettings);
		}
		if (ImGui::CollapsingHeader("RANDOMIZER"))
		{
			//ofxImGui::AddParameter(bRandomColor);
			ofxSurfingHelpers::AddBigButton(bRandomColor);

			//enablers

			//ofxSurfingHelpers::AddBigToggle(bColor_HUE);
			//ofxSurfingHelpers::AddBigToggle(bColor_SAT);
			//ofxSurfingHelpers::AddBigToggle(bColor_BRG);

			//bCallback_ENABLED = false; //disable callbacks

			ImGui::Dummy(ImVec2(0.0f, 5));

			if (ofxImGui::AddParameter(bColor_HUE))
			{
			}
			if (ofxImGui::AddParameter(bColor_SAT))
			{
			}
			if (ofxImGui::AddParameter(bColor_BRG))
			{
			}

			ImGui::Dummy(ImVec2(0.0f, 5));

			//ImGui::Text("Min");

			if (ofxImGui::AddParameter(color_HUE_0))
			{
				ofLogNotice(__FUNCTION__) << "ImGui: HUE MOVED !";
			}
			if (ofxImGui::AddParameter(color_SAT_0))
			{
				ofLogNotice(__FUNCTION__) << "ImGui: SAT MOVED !";
			}
			if (ofxImGui::AddParameter(color_BRG_0))
			{
				ofLogNotice(__FUNCTION__) << "ImGui: BRG MOVED !";
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

		//if (ofxImGui::BeginTree("USER PALETTE", mainSettings))
		if (ImGui::CollapsingHeader("PALETTE"))
		{
			int pad = _w * 0.1;
			//int pad = _w * 0.1;
			//int pad = _w * 0.2;

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
void ofxColorManager::gui_Quantizer()
{
	if (ofxImGui::BeginWindow("QUANTIZER", mainSettings, false))
	{
		ImGui::Dummy(ImVec2(0.0f, 10));

		//ofParameterGroup _g = colorQuantizer.getParameters();
		ofxImGui::AddGroup(colorQuantizer.getParameters(), mainSettings);
	}
	ofxImGui::EndWindow(mainSettings);
}

//--------------------------------------------------------------
void ofxColorManager::gui_Panels()
{
	if (ofxImGui::BeginWindow("PANELS", mainSettings, false))
	{
		ImGui::Dummy(ImVec2(0.0f, 10));

		//ofxSurfingHelpers::AddBigToggle(SHOW_Picker, -1);
		//ofxSurfingHelpers::AddBigToggle(SHOW_UserPalette, -1);
		//ofxSurfingHelpers::AddBigToggle(SHOW_Library, -1);
		//ofxSurfingHelpers::AddBigToggle(SHOW_Theory, -1);
		//ofxSurfingHelpers::AddBigToggle(SHOW_Range, -1);
		//ofxSurfingHelpers::AddBigToggle(SHOW_ColourLovers, -1);
		//ofxSurfingHelpers::AddBigToggle(SHOW_BackGround, -1);

		ofxImGui::AddParameter(SHOW_Picker);
		ofxImGui::AddParameter(SHOW_UserPalette);
		ofxImGui::AddParameter(SHOW_Library);
		ofxImGui::AddParameter(SHOW_Theory);
		ofxImGui::AddParameter(SHOW_Range);
		ofxImGui::AddParameter(SHOW_ColourLovers);
		ofxImGui::AddParameter(SHOW_BackGround);
		ofxImGui::AddParameter(SHOW_Presets);

		ImGui::Dummy(ImVec2(0.0f, 10));

		ImGui::Separator();

		ImGui::Dummy(ImVec2(0.0f, 10));

		ofxImGui::AddParameter(SHOW_Gradient);
		ofxImGui::AddParameter(SHOW_Curve);
		ofxImGui::AddParameter(SHOW_Quantizer);
		//ofxImGui::AddParameter(SHOW_TEST_Curve);
		//ofxImGui::AddParameter(SHOW_AlgoPalettes);
		//ofxImGui::AddParameter(SHOW_ColourLovers_searcher);
		//ofxImGui::AddParameter(SHOW_BrowserColors);
	}
	ofxImGui::EndWindow(mainSettings);
}

//--------------------------------------------------------------
void ofxColorManager::gui_Range()
{
	static int _boxSize = 40;
	//int _boxSize = ImGui::GetWindowContentRegionWidth();

	if (ofxImGui::BeginWindow("RANGE", mainSettings, false))
	{
		ImGui::Dummy(ImVec2(0.0f, 10));

		ofFloatColor c1_Pre = guiCol1[0];
		ofFloatColor c2_Pre = guiCol2[0];
				
		int _h = 25;
		int _w = ImGui::GetWindowContentRegionWidth();
		int pad = _w * 0.4;
		float w50 = ImGui::GetWindowWidth()*0.5f - pad * 0.5f;
		float w100 = ImGui::GetWindowWidth() - pad;
		
		ImGui::Columns(3);
		ImGui::NextColumn();
		
		ImGui::PushItemWidth(w50);

		//int _flags =
		//	ImGuiColorEditFlags_NoOptions |
		//	ImGuiColorEditFlags_NoSmallPreview |
		//	ImGuiColorEditFlags_NoSidePreview |
		//	ImGuiColorEditFlags_InputHSV |
		//	//ImGuiColorEditFlags_NoInputs |
		//	//ImGuiColorEditFlags_No| 
		//	//ImGuiColorEditFlags_cNoLabel |
		//	ImGuiColorEditFlags_NoTooltip;

		// circled & triangle
		ImGuiColorEditFlags _flags =
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

		// picker

		if (ImGui::ColorPicker3("1", &guiCol1[0], _flags)) {
			if (autoPickColor1) color_Picked.set(guiCol1[0]);
		}

		if (ImGui::ColorButton("MyColor##From", *(ImVec4 *)&guiCol1[0], _flags, ImVec2(w50, _h)))
		{

		}

		//--

		// big

		//_w = ImGui::GetWindowContentRegionWidth();
		//_flags = ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoTooltip;
		//ImGui::ColorButton("MyColor##From", *(ImVec4 *)&guiCol1[0], _flags, ImVec2(w50, _h));

		//--

		ImGui::PopItemWidth();

		ofxImGui::AddParameter(autoPickColor1);

		//-

		ImGui::NextColumn();

		//ImGui::Dummy(ImVec2(0.0f, 5));
		
		//-

		ImGui::PushItemWidth(w50);

		//--

		// picker
		if (ImGui::ColorPicker3("2", &guiCol2[0], _flags)) {
			if (autoPickColor2) color_Picked.set(guiCol2[0]);
		}
		

		//--

		// big

		if (ImGui::ColorButton("MyColor##To", *(ImVec4 *)&guiCol2[0], _flags, ImVec2(w50, _h)))
		{

		}

		ImGui::PopItemWidth();

		//--
		

		ofxImGui::AddParameter(autoPickColor2);

		//-
		
		ImGui::Columns(1);
		//ImGui::NextColumn();

		ImGui::Dummy(ImVec2(0.0f, 5));

		//-

		c1_Rng.set(guiCol1);
		c2_Rng.set(guiCol2);

		//c1_Rng = guiCol1;
		//c2_Rng = guiCol2;

		//autogenerate
		if (bRangeAutoGenerate)
		{
			if (c1_Pre != guiCol1[0] ||
				c2_Pre != guiCol2[0])

				generateRange(c1_Rng, c2_Rng);
		}

		//if (ofxSurfingHelpers::AddBigButton("GENERATE"))
		if (ImGui::Button("GENERATE", ImVec2(150, 25)))
		{
			generateRange(c1_Rng, c2_Rng);
		}
		ofxImGui::AddParameter(bRangeAutoGenerate);

		ofxImGui::AddParameter(numColorsRange);
		//ofxSurfingHelpers::AddSmallButton(bGetPaletteFromRange, 150, 30);

		ImGui::Dummy(ImVec2(0.0f, 5));

		//-

		// 2.2 draw all palette colors grid
		const int ncols = 11;//?
		const int _total = ncols * ncols;

		//const int ncols = numColorsRange.get();
		//const int ty = (int)NUM_TYPES_RANGES;
		//const int _total = ncols * ty;

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

		//-

		for (int n = 0; n < _total; n++)
		{
			//label for each row
			const int _row = n / numColorsRange.get();
			//const int _row = n / 11;

			if (n % ncols == 0)
			{
				ofxSurfingHelpers::AddSmallButton(rangTypes[_row], 100, _boxSize);
				ImGui::SameLine();
			}

			{
				ImGui::PushID(n);

				//ImGui::PushItemWidth(guiWidth * 0.2);
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
					ImVec2(_boxSize, _boxSize)))
				{
					lastColorPicked = n;

					color = ImVec4(_palette[n].x, _palette[n].y, _palette[n].z, color.w); // Preserve alpha!
					color_Picked = color;

					//color name
					lastColorPickedNameColor = "'" + ColorBrowser.pantoneNames[n] + "'";
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
void ofxColorManager::gui_Curve()
{
	if (ofxImGui::BeginWindow("CURVE GRADIENT", mainSettings, false))
	{
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
	}
	ofxImGui::EndWindow(mainSettings);
}

//--------------------------------------------------------------
void ofxColorManager::gui_Background()
{
	if (ofxImGui::BeginWindow("BACKGROUND", mainSettings, false))
	{
		{
			ofxImGui::AddParameter(backgroundENABLE);

			int _w = ImGui::GetWindowContentRegionWidth();
			int pad = _w * 0.1;
			//int pad = _w * 0.2;
			float w50 = ImGui::GetWindowWidth()*0.5f - pad * 0.5f;
			float w100 = ImGui::GetWindowWidth() - pad;

			ImGui::PushItemWidth(w100);
			//ImGui::PushItemWidth(120);

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

			ImGui::PopItemWidth();
		}
	}
	ofxImGui::EndWindow(mainSettings);
}

//--------------------------------------------------------------
void ofxColorManager::gui_Presets()
{
	// PRESET MANAGER

	if (ofxImGui::BeginWindow("PRESETS", mainSettings, false))
	{
		//-

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

		ImGui::Text("Type Name:");
		std::string textInput_temp = PRESET_name;

		// loaded string into char array
		char tab2[32];
		strncpy(tab2, textInput_temp.c_str(), sizeof(tab2));
		tab2[sizeof(tab2) - 1] = 0;

		//int wt = ImGui::GetWindowContentRegionWidth() * 0.8;

		if (ImGui::InputText("", tab2, IM_ARRAYSIZE(tab2)))
		{
			ofLogNotice(__FUNCTION__) << "InputText:" << ofToString(tab2);

			textInput_temp = ofToString(tab2);
			ofLogNotice(__FUNCTION__) << "textInput_temp:" << textInput_temp;

			if (MODE_newPreset) MODE_newPreset = false;
		}

		//--

		ImGui::Dummy(ImVec2(0.0f, 10));

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

		ImGui::Dummy(ImVec2(0.0f, 10));

		//-

		// 2. presets

		//ImGui::Text("PRESETS");

		if (ImGui::Button("SAVE"))
		{
			ofLogNotice(__FUNCTION__) << "SAVE";

			//TODO
			//should re load by same name and get what position on vector
			//is to reload current preset number
			//            textInput_temp = ofToString(tab2);
			//            ofLogNotice(__FUNCTION__) << "textInput_temp:" << textInput_temp;

			PRESET_name = textInput_temp;
			ofLogNotice(__FUNCTION__) << "PRESET_name: " << PRESET_name;

			preset_save(PRESET_name);
			preset_filesRefresh();
		}

		ImGui::SameLine();
		if (ImGui::Button("UPDATE"))
		{
			ofLogNotice(__FUNCTION__) << "UPDATE";

			PRESET_name = textInput_temp;
			ofLogNotice(__FUNCTION__) << "PRESET_name: " << PRESET_name;

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
			ofLogNotice(__FUNCTION__) << "LOAD";
			ofLogNotice(__FUNCTION__) << "PRESET_name: " << PRESET_name;
			preset_load(PRESET_name);
		}

		//ImGui::SameLine();
		if (ImGui::Button("DELETE"))//current preset
		{
			ofLogNotice(__FUNCTION__) << "DELETE";

			files[currentFile].remove();
			preset_filesRefresh();

			//std::string str = fileNames[currentFile];
			//ofLogNotice(__FUNCTION__) << "DELETE:"<<str<<endl;
			//dir.listDir("user_kits/presets");
			//dir.allowExt("jpg");
			//dir.allowExt("png");
			//dir.sort();
		}

		//export user palette colors to live reload from another parallel app!
		ImGui::SameLine();
		if (ImGui::Button("EXPORT"))
		{
			//ofLogNotice(__FUNCTION__) << "EXPORT";
			saveColors();
		}

		//ImGui::SameLine();
		//if (ImGui::Button("REFRESH"))//current preset
		//{
		//    ofLogNotice(__FUNCTION__) << "REFRESH";
		//    preset_filesRefresh();
		//}

		ImGui::Dummy(ImVec2(0.0f, 10));

		//-

		if (MODE_newPreset)
		{
			ImGui::Separator();

			ImGui::Dummy(ImVec2(0.0f, 10));

			ImGui::Text("NEW PRESET!");

			// loaded string into char array
			char tab[32];
			strncpy(tab, textInput_New.c_str(), sizeof(tab));
			tab[sizeof(tab) - 1] = 0;

			if (ImGui::InputText("", tab, IM_ARRAYSIZE(tab)))
			{
				ofLogNotice(__FUNCTION__) << "InputText [tab]:" << ofToString(tab);
				textInput_New = ofToString(tab);
				ofLogNotice(__FUNCTION__) << "textInput_New:" << textInput_New;
			}

			// WORKFLOW: when its editing a new preset..

			ImGui::PushID(1);
			int n = 30;
			float a = ofMap(ofGetFrameNum() % n, 0, n, 0.0f, 1.0f);
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.5, 0.0f, 0.5f, a));

			if (ImGui::Button("SAVE NEW"))
			{
				MODE_newPreset = false;
				ofLogNotice(__FUNCTION__) << "textInput_New: " << textInput_New;
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
bool ofxColorManager::gui_Draw()
{
	mainSettings = ofxImGui::Settings();

	gui.begin();

	//// 0. COSINE GRADIENT
	//
	//if (SHOW_CosineGradient)
	//    gui_imGui_CosineGradient();

	//-

	// 1. COLOR PICKER

	if (SHOW_Picker)
		gui_Picker();

	if (SHOW_Library)
		gui_Library();

	//-

	if (SHOW_UserPalette) {
		gui_Palette();
	}

	if (SHOW_Range) {
		gui_Range();
	}

	if (SHOW_Theory) {
		gui_Theory();
	}

	// 2. COLORS MANAGER

	if (SHOW_BackGround)
		gui_Background();

	if (SHOW_Curve)
		gui_Curve();

	//-

	// 3. PRESET MANAGER

	if (SHOW_Presets)
		gui_Presets();

	//-

	// 4. PANELS MANAGER

	if (SHOW_Panels)
		gui_Panels();

	if (SHOW_Quantizer)
		gui_Quantizer();

	//-

	// COLOR lovers
#ifdef USE_COLOR_LOVERS
	if (SHOW_ColourLovers || SHOW_ColourLovers_searcher)
	{
		ColourLoversHelper.draw();
	}
#endif

	//-

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
#endif


//--------------------------------------------------------------
void ofxColorManager::palettes_update()
{
	// calculate base primary color
	if (MODE_TweakSatBrg)
	{
		// using hue only from picked color and forcing sat/(brg from algo sliders
		base = ofFloatColor::fromHsb(color_Picked.get().getHue(),
			ofMap(SATURATION, 0, 255, 0., 1.),
			ofMap(BRIGHTNESS, 0, 255, 0., 1.));
	}
	else
	{
		// check using hue + sat/brg from color ignoring algo panel SV sliders
		base = color_Picked.get();
	}

	colorTheoryBase = base;

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
#endif

	//--

	// 2. FROM OFX-COLOUR-THEORY

	palettes_colorTheory_update();//TODO: reduce calls...

	//--

#ifdef USE_RECTANGLE_INTERFACES

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
	palettes_update();
	palettes_setup();

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

#ifdef USE_RECTANGLE_INTERFACES
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
#endif

//--------------------------------------------------------------
void ofxColorManager::refreshColorTheory()
{
	ofLogNotice(__FUNCTION__);

	////selected
	//colorSchemeName.set(ColorWheelSchemes::colorSchemeNames[colorScheme.get()]);
	//scheme = ColorWheelSchemes::colorSchemes[colorScheme.get()];
	//scheme->setPrimaryColor(colorTheoryBase.get());

	//colorsTheory = scheme->interpolate(amountColors.get());
	for (int i = 0; i < NUM_COLOR_THEORY_TYPES; i++)
	{
		shared_ptr<ColorWheelScheme> _scheme;
		_scheme = ColorWheelSchemes::colorSchemes[i];
		_scheme->setPrimaryColor(colorTheoryBase.get());
		colorsTheory[i] = _scheme->interpolate(amountColors.get());
	}
}

//--------------------------------------------------------------
void ofxColorManager::palettes_colorTheory_setup()
{
	amountColors_Alg.makeReferenceTo(amountColors);
	//amountColors.makeReferenceTo(amountColors_Alg);

	params_ColorTheory.setName("Color Theory");
	params_ColorTheory.add(colorTheoryBase.set("Base", ofColor::magenta, ofColor(0), ofColor(255)));
	params_ColorTheory.add(bGetFromPicker.set("Auto Picker", true));
	params_ColorTheory.add(colorScheme.set("Color Scheme", 6, 0, ColorWheelSchemes::colorSchemes.size() - 1));
	params_ColorTheory.add(colorSchemeName);
	params_ColorTheory.add(amountColors.set("Amnt Colors", 8, 2, 20));
	params_ColorTheory.add(lastColorTheoryPicked_Palette.set("Last Theory Picked", 0, 0, 0));

	//toggles
	for (int i = 0; i < NUM_COLOR_THEORY_TYPES; i++)
	{
		theoryTypes[i].set(ColorWheelSchemes::colorSchemeNames[i], false);
		params_ColorTheory.add(theoryTypes[i]);
	}

	ofAddListener(params_ColorTheory.parameterChangedE(), this, &ofxColorManager::Changed_ColorTheory);

	refreshColorTheory();

	//---

	//panel.setup();
	//panel.add(group);
	//panel.setPosition(1000, 400);
	//
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
void ofxColorManager::palettes_colorTheory_update()
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
void ofxColorManager::palette_load_ColourLovers()
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
	ofLogVerbose(__FUNCTION__) << " : " << ofToString(c);
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

#ifdef USE_RECTANGLE_INTERFACES
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

//--------------------------------------------------------------
void ofxColorManager::palette_clear()
{
	// remove all colors of user palette

	ofLogNotice(__FUNCTION__);

	palette.clear();
	gradient.reset();

	//-

	//ofLogNotice(__FUNCTION__) << "palette_clear::getNumChildren: " << scene->getNumChildren();

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
#endif

#pragma mark - CALLBACKS

//--------------------------------------------------------------
void ofxColorManager::Changed_ColorPicked(ofFloatColor &_c)
{
	// TEST
	if (bCallback_ENABLED == true)
	{
		update_color_picked_CHANGES();

		// TEST
		color_picked_Update_To_HSV();//redundant.. ?

		// DEMO
		if (TEST_DEMO) myDEMO_palette.reStart();
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

	//if (name != "INPUT" &&
	//	name != "OUTPUT")
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

	//if (name != "INPUT" &&
	//	name != "OUTPUT")
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

	else if (name == colorScheme.getName() || name == colorTheoryBase.getName())
	{
		refreshColorTheory();
	}

	else if (name == amountColors.getName())
	{
		amountColors_Alg = amountColors.get();

		palettes_update();
		refreshColorTheory();
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
			}
		}
	}
}

//--------------------------------------------------------------
void ofxColorManager::Changed_Controls(ofAbstractParameter &e)
{
	std::string name = e.getName();

	// TODO: should reduce callbacks in output..
	//    if (name != "INPUT" && name != "OUTPUT" &&
	//            name!="H" && name!="S" && name!="B")

	if (name != "INPUT" &&
		name != "OUTPUT")
	{
		ofLogNotice(__FUNCTION__) << name << " : " << e;
	}

	//--

	// PANELS

#ifdef USE_COLOR_LOVERS
	if (name == "COLOUR-LOVERS")
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
#endif
	else if (name == "SHOW PALETTES")
	{
		palettes_setVisible(SHOW_AlgoPalettes);
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
		curveMod_Slider.setVisible(SHOW_Curve);
		curve_pos_slider.setVisible(SHOW_TEST_Curve);
	}
	else if (name == "PALETTE")
	{
		ofLogVerbose(__FUNCTION__) << "SHOW_UserPalette: " << SHOW_UserPalette;
		for (int i = 0; i < btns_palette.size(); i++)
		{
			btns_palette[i]->setVisible(SHOW_UserPalette);
		}
	}
	else if (name == "PICTURE CAPTURE")
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
			color_Picked = palette[palette_colorSelected];
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
	//            random.generateRandom(amountColors_Alg);
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

#ifdef USE_RECTANGLE_INTERFACES
			palette_recallFromPalettes(SELECTED_palette_LAST);//trig last choice
#endif
			// DEMO
			if (TEST_DEMO) myDEMO_palette.reStart();
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
			color_backGround.set(ofColor(color_Picked.get()));
		}
	}
	else if (name == "AUTO SET")
	{

	}
	else if (name == "BG COLOR")
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

		if (SHOW_AlgoPalettes && !SHOW_ColourLovers && !SHOW_Quantizer)
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
			else if (key == OF_KEY_LEFT && !SHOW_Presets)
			{
				amountColors_Alg--;
				amountColors_Alg = (int)ofClamp(amountColors_Alg,
					amountColors_Alg.getMin(),
					amountColors_Alg.getMax());

				if (TEST_DEMO)
					myDEMO_palette.reStart();
			}
			else if (key == OF_KEY_RIGHT && !SHOW_Presets)
			{
				amountColors_Alg++;
				amountColors_Alg = (int)ofClamp(amountColors_Alg,
					amountColors_Alg.getMin(),
					amountColors_Alg.getMax());

				if (TEST_DEMO)
					myDEMO_palette.reStart();
			}
		}

		//-

		// PRESET MANAGER

		if (SHOW_Presets)
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
					color_Picked = ofFloatColor(palette[0]);
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
					color_Picked = ofFloatColor(palette[0]);
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

		else if (key == 'R')
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
			color_Picked = ofFloatColor(ofRandom(0., 1.), ofRandom(0., 1.), ofRandom(0., 1.));

			if (bAuto_palette_recall)
			{
				palettes_update();

#ifdef USE_RECTANGLE_INTERFACES
				palette_recallFromPalettes(SELECTED_palette_LAST);//trig last choice
#endif
			}

			// undo
			color_Undo = color_Picked.get();
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
			//random.generateRandom(amountColors_Alg);
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
			color_Picked = ofColor(ColorBrowser_palette[r]);
			palettes_update();

#ifdef USE_RECTANGLE_INTERFACES
			palette_recallFromPalettes(SELECTED_palette_LAST);
#endif

			// undo
			color_Undo = color_Picked.get();
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
#ifdef USE_COLOR_LOVERS
				ColourLoversHelper.randomPalette();
#endif

				textInput_temp = PRESET_name;

				//                if (TEST_DEMO)
				//                    myDEMO_palette.reStart();
			}

			//-

			// browse presets
#ifdef USE_COLOR_LOVERS
			else if (key == OF_KEY_DOWN)
			{
				ColourLoversHelper.nextPalette();
				////WORKFLOW
				//if(bAutoExportPreset)
				//{
				//    ofLogNotice(__FUNCTION__) << "EXPORT";
				//    saveColors();
				//}
			}
			else if (key == OF_KEY_UP)
			{
				ColourLoversHelper.prevPalette();
				////WORKFLOW
				//if(bAutoExportPreset)
				//{
				//    ofLogNotice(__FUNCTION__) << "EXPORT";
				//    saveColors();
				//}
			}
#endif

			//--
		}

		//--

		// UNDO COLOR

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
		//    color_Picked = ofFloatColor(ofRandom(0., 1.), ofRandom(0., 1.), ofRandom(0., 1.));
		//    palette_addColor(ofColor(color_Picked.get()));
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
#ifdef USE_COLOR_LOVERS
	glm::vec2 sizeGui(150, 375);
	glm::vec2 sizeGrid(150, h);
	glm::vec2 posGui(w - (sizeGui.x + sizeGrid.x + 4), 0);
	glm::vec2 posGrid(posGui.x + sizeGui.x + 2, 0);
	ColourLoversHelper.setGrid(posGrid, sizeGrid);
	ColourLoversHelper.setPosition(posGui, sizeGui);
	ColourLoversHelper.windowResized(w, h);
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
	std::string _path = "user_kits/presets";
	ofDirectory dataDirectory(ofToDataPath(_path, true));
	ofxSurfingHelpers::CheckFolder(_path);

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

		ofLogNotice(__FUNCTION__) << p;

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
		////        std::string *name_BACK;
		////        vector<ofColor> *palette_BACK;
		////        std::string *curveName_BACK;

		//-

		if (TEST_DEMO) myDEMO_palette.clear();

		//workflow
		if (bAutoExportPreset)
		{
			//ofLogNotice(__FUNCTION__) << "EXPORT";
			saveColors();
		}
	}
}


//--------------------------------------------------------------
void ofxColorManager::preset_save(std::string p)
{
	ofLogNotice(__FUNCTION__) << p;

	myPresetPalette.setName(p);
	myPresetPalette.setCurveName(PRESET_curveName);
	myPresetPalette.setBackgroundColor(color_backGround.get());
	myPresetPalette.setPalette(palette);

	//myPresetPalette.preset_save(PRESET_name);
	myPresetPalette.preset_save(p);
}

////--------------------------------------------------------------
//void ofxColorManager::loadAppSettings(ofParameterGroup &g, std::string path)
//{
//	ofLogNotice(__FUNCTION__) << path;
//	ofXml settings;
//	settings.load(path);
//	ofDeserialize(settings, g);
//
//	paletteLibPage = paletteLibPage_param.get();
//}
//
////--------------------------------------------------------------
//void ofxColorManager::saveAppSettings(ofParameterGroup &g, std::string path)
//{
//	ofLogNotice(__FUNCTION__) << path;
//	paletteLibPage_param = paletteLibPage;
//
//	ofXml settings;
//	ofSerialize(settings, g);
//	settings.save(path);
//}

#pragma mark - UTILS

//--------------------------------------------------------------
void ofxColorManager::color_picked_Update_To_HSV()
{
	color_HUE = 255 * color_Picked.get().getHue();
	color_SAT = 255 * color_Picked.get().getSaturation();
	color_BRG = 255 * color_Picked.get().getBrightness();
}

//--------------------------------------------------------------
void ofxColorManager::update_color_picked_CHANGES()
{
	// workflow

	ofLogNotice(__FUNCTION__) << ofToString(color_Picked);

	if (bGetFromPicker) {
		colorTheoryBase = color_Picked.get();
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

	//// TEST
	//color_picked_Update_To_HSV();//redundant...

	//TODO: 
	//mirror clicked/picked colors
	color_clicked.set(color_Picked);

	//-

	//TODO
	//palettes
	//primaryColor.set(color_Picked.get());
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

	// 2. workflow:
	//if enabled, when color picked changes, auto trig and put generated palette to user palette

	if (bAuto_palette_recall)
	{
		palettes_update();

#ifdef USE_RECTANGLE_INTERFACES
		palette_recallFromPalettes(SELECTED_palette_LAST);//trig last choiced algorithmic palette
#endif
	}

	// 3. recreate algorithmic palettes if locking is disable

	if (!bLock_palette)
	{
		palettes_update();
	}

	//-

	//    //TODO
	//    // palettes
	////    primaryColor.set(color_Picked.get());
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
	//log
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
}

//--------------------------------------------------------------
void ofxColorManager::setVisible(bool b)
{
	SHOW_ALL_GUI = b;

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
