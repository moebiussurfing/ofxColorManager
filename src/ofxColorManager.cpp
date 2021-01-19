#include "ofxColorManager.h"

//TODO:
//--------------------------------------------------------------
void ofxColorManager::dragEvent(ofDragInfo info) {
	colorQuantizer.dragEvent(info);
}

//--------------------------------------------------------------
void ofxColorManager::reBuild()
{
	ofLogWarning(__FUNCTION__);

	std::string _name;

	//-

	// presets

	//if (SHOW_Presets)
	//{
	//}
	//else
	{
		if (SHOW_ColourLovers)
		{
			_name = myPalette_Name;
			palette_FromColourLovers();
		}
		else if (SHOW_Quantizer)
		{
			_name = myPalette_Name;
			palette_FromQuantizer();
		}
		else if (SHOW_Theory)
		{
			_name = theory_Name;
			refresh_TheoryEngine();
			palette_FromTheory(last_Index_Theory);
		}
		else if (SHOW_Range)
		{
			_name = range_Name;
			palette_FromRange(last_Index_Range);
		}
	}

	if (SHOW_Presets)
	{
		if (!bNewPreset) bNewPreset = true;
		textInput_New = _name;
	}

	//----

	// DEMO 2

	myDEMO2.setPaletteColors(palette);

	//--

	// DEMO 1

	if (DEMO1_Test) myDEMO1.reStart();

	//-

	// workflow

	if (bAutoExportPreset)
	{
		ofLogNotice(__FUNCTION__) << "Auto EXPORT";
		exportPalette();
	}
}

//--------------------------------------------------------------
ofxColorManager::ofxColorManager()
{
	ofAddListener(ofEvents().update, this, &ofxColorManager::update);
#ifdef AUTO_DRAW_CALLBACK
	ofAddListener(ofEvents().draw, this, &ofxColorManager::draw, OF_EVENT_ORDER_BEFORE_APP);
#endif

	//--

	//default
	fps = 60.0f;
	dt = 1.f / fps;

	//--

	// file paths

	path_Global = "ofxColorManager/";
	ofxSurfingHelpers::CheckFolder(path_Global);
	path_AppState = path_Global + "ofxColorManager_Settings.xml";

	path_Layout = path_Global + "layout/";
	ofxSurfingHelpers::CheckFolder(path_Layout);

	path_Folder_Gradient = path_Global + "gradients/";
	ofxSurfingHelpers::CheckFolder(path_Folder_Gradient);

	path_Name_Gradient = "myGradient";

	PRESET_Name = "PRESET_Name";
	PRESET_Name_Gradient = path_Name_Gradient;

	path_Gradient_LUT = path_Folder_Gradient + path_Name_Gradient + ".yml";
	path_Gradient_Preset = path_Folder_Gradient + path_Name_Gradient + ".xml";

	path_Folder_ExportColor_Data = path_Global + "export/";
	path_Folder_ExportColor_Custom = path_Global + "export/";//temp
	ofxSurfingHelpers::CheckFolder(path_Folder_ExportColor_Data);
	ofxSurfingHelpers::CheckFolder(path_Folder_ExportColor_Custom);

	path_Name_ExportColor = "ofxColorManager";// name
	path_FileExport = path_Folder_ExportColor_Custom.get() + path_Name_ExportColor;

	//--

	infoHelp = "HELP INFO\n\n";
	infoHelp += "H                   HELP\n";
	infoHelp += "K                   KEYS\n";
	infoHelp += "G                   GUI\n";
	infoHelp += "TAB                 MODES\n";
	infoHelp += "SPACE               NEXT\n";
	infoHelp += "Down|Up             NEXT/PREVIOUS\n";
	infoHelp += "+|-                 AMOUNT COLORS\n";
	infoHelp += "\n";
	infoHelp += "PANELS\n";
	infoHelp += "F1                  PALETTE\n";
	infoHelp += "F2                  PICKER\n";
	infoHelp += "F3                  BACKGROUND\n";
	infoHelp += "F4                  LIBRARY\n";
	infoHelp += "\n";
	infoHelp += "MODES\n";
	infoHelp += "F5                  THEORY\n";
	infoHelp += "F6                  RANGE\n";
	infoHelp += "F7                  LOVERS\n";
	infoHelp += "F8                  PICTURE\n";
	infoHelp += "F9                  PRESETS\n";
	infoHelp += "F10                 GRADIENT\n";
	infoHelp += "F11                 HIDE ALL\n";
	infoHelp += "\n";
	infoHelp += "TEST\n";
	infoHelp += "D                   DEMO SCENE\n";
	infoHelp += "T                   GRADIENT TEST\n";
	infoHelp += "m                   MINI PREVIEW\n";
	infoHelp += "M                   DEBUG\n";
}

//--------------------------------------------------------------
void ofxColorManager::setup()
{
	//ofSetLogLevel("ofxColorManager", OF_LOG_NOTICE);

#ifdef USE_SUPER_LOG
	std::string fileLoggingDirectory = "logs";
	bool logToConsole = 0;
	bool logToScreen = true;
	ofSetLoggerChannel(ofxSuperLog::getLogger(logToConsole, logToScreen, "logs"));
#endif

	ENABLE_HelpInfo.set("Help Info", false);
	ENABLE_HelpInfo.setSerializable(false);

	//-

	sizeLibColBox.set("Size Lib", 25, 10, 100);
	lib_Responsive_Mode.set("Responsive", false);
	bPagerized.set("Mode Paging", false);

	sizePaletteBox.set("Size Plt", 25, 10, 500);
	bResponsive_Presets.set("Responsive", false);

	//-

	//display text
	std::string strFont;
	std::string _p = "assets/fonts/";

	strFont = _p + "GTAmerica-ExtendedBlack.ttf";
	//strFont = _p + "GTAmerica-ExpandedBold.ttf";
	//strFont = _p + "telegrama_render.otf";
	//strFont = _p + "PragmataProB_0822.ttf";
	//strFont = _p + "Kazesawa-Extrabold.ttf";

	fontBig.load(strFont, 55);
	fontMedium.load(strFont, 28);
	fontSmall.load(strFont, 22);

	txt_lineActive[0] = false;
	txt_lineActive[1] = false;
	txt_lineActive[2] = false;
	txt_lineActive[3] = false;
	last_Index_Type = -1;

	//-

	font.load("assets/fonts/telegrama_render.otf", 11, true, true, true);
	//font.load("assets/fonts/LCD_Solid.ttf", 11, true, true);
	//font.load("assets/fonts/overpass-mono-bold.otf", 9, true, true);

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

	//----

	// colorQuantizer

#ifdef USE_IMAGE_QUANTIZER
	colorQuantizer.setup();

	// receivers pointers
	colorQuantizer.setPalette_BACK(myPalette);
	colorQuantizer.setPalette_BACK_RefreshPalette(bUpdated_Palette_BACK);
	colorQuantizer.setPalette_BACK_Name(myPalette_Name);
#endif

	//----

	// theory

	setup_Theory();

	//----

	//myPresetManager.setup();

	//----

	// DEMO

	myDEMO1.setup();
	myDEMO1.setPalette(palette);
	//myDEMO1.setEnableMouseCamera(true);

	//----

#ifdef USE_DEBUG_LAYOUT
// mouse debugger
	mouseRuler.setup();
	mouseRuler.toggleVisibility();
#endif

	//----

	// colour lovers

#ifdef USE_COLOR_LOVERS

	colourLoversHelper.setup();

	//----

	// pointers back
	//TODO:
	colourLoversHelper.setColor_BACK(myColor);
	colourLoversHelper.setColor_BACK_Refresh(bUpdated_Color_BACK);
	colourLoversHelper.setPalette_BACK(myPalette);
	colourLoversHelper.setPalette_BACK_Name(myPalette_Name);
	colourLoversHelper.setPalette_BACK_Refresh(bUpdated_Palette_BACK);

	//--------------------------------------------------------------
	listener_LoverName = colourLoversHelper.lastPaletteName.newListener([this](std::string &n) {
		txt_lineActive[0] = false;//preset name
		txt_lineActive[1] = true;//lover name
		txt_lineActive[2] = false;//theory name
		txt_lineActive[3] = false;//range name
		last_Index_Type = 1;

		ofLogNotice("colourLoversHelper > Name Palette: ") << n;

		});

#endif

	//----

	// some initiation values..
	myColor = ofColor::white;
	myPalette.resize(2);//pointer setter will clear/resize. nevermind the vector size here
	myPalette[0] = ofColor::grey;
	myPalette[1] = ofColor::black;
	myPalette_Name = "";

	//----

	// layout

	refresh_Gui_Layout();

	//----

	// color browser

	colorBrowser.setup_colorBACK(color_BACK);
	colorBrowser.setup();
	colorBrowser.setVisible(false);

	refresh_Libs();

	//-

	//--------------------------------------------------------------
	listener_ModeSorting = colorBrowser.MODE_SORTING.newListener([this](int &i) {
		ofLogNotice("ofxColorManager::MODE_SORTING: ") << i;

		refresh_Libs();

		});

	//-

	//--------------------------------------------------------------
	listener_Library = colorBrowser.LibraryColors_Index.newListener([this](int &i) {
		ofLogNotice("ofxColorManager::LibraryColors_Index: ") << i;

		refresh_Libs();

		});

	//----

	// background

	setBackground_ENABLE(true);

	color_BackGround.set("BG", ofColor(8));
	color_backGround_SET.set("Set", false);
	color_BackGround_Darkness.set("Darkerkness", 0.5, 0., 1.);
	color_BackGround_Darker.set("Darker", false);
	color_BackGround_Lock.set("Lock Bg", false);
	AutoSet_BackGround_Color.set("AutoSet", true);

	params_data.setName("GENERAL");
	params_data.add(color_BackGround);
	params_data.add(color_BackGround_Lock);
	params_data.add(color_backGround_SET);
	params_data.add(color_BackGround_Darker);
	params_data.add(AutoSet_BackGround_Color);

	//-----

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

	//----

	// algorithmic palette


	SATURATION.set("SATURATION", 200, 0, 255);
	BRIGHTNESS.set("BRIGHTNESS", 200, 0, 255);

	bAuto_Build_Palette.set("Auto Build Palette", true);

	numColors_Alg.set("Amnt Colors Alg", 6, 2, MAX_PALETTE_COLORS);
	numColors_Alg.setSerializable(false);
	analogSpread.set("Analogue Spread", 0.2, 0, 1);

	//bLock_palette.set("Lock Palettes", false);
	//bRandomPalette.set("RANDOM PALETTE", false);

	params_Theory.setName("ADVANCED");
	params_Theory.add(SATURATION);
	params_Theory.add(BRIGHTNESS);
	params_Theory.add(numColors_Alg);
	params_Theory.add(bAuto_Build_Palette);
	params_Theory.add(analogSpread);
	//params_Theory.add(MODE_TweakSatBrg);
	//params_Theory.add(bLock_palette);
	//params_Theory.add(bRandomPalette);

	//-

	//exclude
	DEMO_Cam.setSerializable(false);
	SHOW_AlgoPalettes.setSerializable(false);
	SATURATION.setSerializable(false);
	BRIGHTNESS.setSerializable(false);
	//color_BackGround.setSerializable(false);
	color_Picked.setSerializable(false);
	color_HUE.setSerializable(false);
	color_SAT.setSerializable(false);
	color_BRG.setSerializable(false);
	color_HUE_0.setSerializable(false);
	color_SAT_0.setSerializable(false);
	color_BRG_0.setSerializable(false);

	ofAddListener(params_Theory.parameterChangedE(), this, &ofxColorManager::Changed_Controls);

	//-

	refresh_TheoryEngine();

	//-

	// gradient

	gradient.reset();
	//gradient.setupAsTurbo(10); //with 10 samples
	gradient_HardMode.set("Hard Steps", false);
	bAutoPaletteFromGradient.set("Auto Palettize", false);

	gradient.setHardMode(gradient_HardMode);
	gradient.setDrawVertical(true);
	gradient.setDrawDirFlip(true);

	//params_curve.setName("GRADIENT CURVE");
	//params_curve.add(MODE_EditGradientLayout);
	//params_curve.add(gradient_HardMode);

	ofAddListener(params_curve.parameterChangedE(), this, &ofxColorManager::Changed_Controls);

	//-

	// curve 

	setup_CurveTool();

	//-

	bRandomColor.set("RANDOM COLOR", false);
	bAddColor.set("ADD COLOR", false);
	bEditPalette.set("EDIT COLOR", false);
	bRemoveColor.set("REMOVE COLOR", false);
	bClearPalette.set("CLEAR PALETTE", false);

	SHOW_Scene.set("SCENE", true);
	//SHOW_Scene.setSerializable(false);

	//-

	// panels

	SHOW_ALL_GUI.setName("GUI MAIN");
	SHOW_MINI_Preview.setName("MINI PREVIEW");
	SHOW_UserPalette.setName("PALETTE");
	SHOW_UserPaletteEditor.setName("SHOW EDITOR");
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
	//SHOW_Gradient.set("GRADIENT", true);
	SHOW_GradientCurve.set("GRADIENT", true);
	MODE_EditGradientLayout.set("Edit Layout", false);
	MODE_EditGradientLayout.setSerializable(false);

	SHOW_Presets.set("PRESETS", true);
	SHOW_Kit.set("SHOW KIT", true);
	AutoScroll.set("AutoScroll", true);
	//SHOW_PresetsPalette.set("Show Palette", false);
	SHOW_BackGround.set("BACKGROUND", true);
	SHOW_Picker.set("PICKER", true);
	SHOW_Library.set("LIBRARY", false);
	SHOW_Range.set("RANGE", true);
	SHOW_Panels.set("PANELS", true);
	SHOW_Export.set("EXPORT", true);
	SHOW_Demos.set("DEMO", false);
	//SHOW_Demo2.set("DEMO2", false);

	//bAutoResizePalette.set("AutoResize", false);
	bAutoExportPreset.set("Auto Export", false);
	bExportPreset_DefaultPath.set("Default Path", true);
	path_Folder_ExportColor_Custom.set("ExportPath", "");

	//-

	scale_ColRange.set("Scale Rg", 1.f, 0.25f, 1.25f);

	//-

	//palette

	boxSizeUser.set("Box Size", 40, 10, 200);
	boxMaxRows.set("Max Rows", 10, 1, 20);
	scale_ColPalette.set("Scale Pt", 1.f, 0.25f, 1.25f);
	//bEditUserPalette.set("EDIT", false);
	//bUserPaletteVertical.set("VERTICAL", false);
	bFlipUserPalette.set("FLIP", false);

	params_Palette.setName("USER PALETTE");
	params_Palette.add(bFlipUserPalette);
	params_Palette.add(boxMaxRows);
	params_Palette.add(scale_ColPalette);
	//params_Palette.add(bEditUserPalette);
	//params_Palette.add(bUserPaletteVertical);
	//params_Palette.add(boxSizeUser);

	ofAddListener(params_Palette.parameterChangedE(), this, &ofxColorManager::Changed_ParamsPalette);

	//-

	//extra algo
	std::string name;
	params_algoTypes.setName("AlgoTypes");
	for (int i = 0; i < NUM_COLOR_THEORY_TYPES_G2; i++)
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

	ofAddListener(params_algoTypes.parameterChangedE(), this, &ofxColorManager::Changed_ColorTheory);

	//-

	SHOW_ALL_GUI = true;
	SHOW_debugText = false;
	SHOW_MINI_Preview = false;

	//--

	//exclude
	numColors_Theory.setSerializable(false);
	numColors_Alg.setSerializable(false);

	//----

	// GUI

#ifdef INCLUDE_IMGUI_CUSTOM_THEME_AND_FONT
	ofxSurfingHelpers::ImGui_FontCustom();
#endif

	// create
	gui.setup();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::GetIO().MouseDrawCursor = false;

	//daan fork
	//gui.setup(nullptr, true, ImGuiConfigFlags_DockingEnable, true, true);
	//gui.setup(nullptr, true, false, true, true);

	// theme customize
#ifdef INCLUDE_IMGUI_CUSTOM_THEME_AND_FONT
	ofxSurfingHelpers::ImGui_ThemeMoebiusSurfing();
	//ofxSurfingHelpers::ImGui_ThemeModernDark();
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

	AppMode.set("AppMode", 0, 0, 5);
	AppMode_name.set("", "");
	AppMode.setSerializable(false);

	//-

	// startup settings

	params_AppState.setName("ofxColorManager");
	params_AppState.add(preset_Index);
	params_AppState.add(AppMode);
	params_AppState.add(SHOW_Scene);
	params_AppState.add(last_Index_Theory);
	params_AppState.add(last_Index_Range);

	// export colors
	params_ExportColors.setName("ExportColors");
	params_ExportColors.add(bAutoExportPreset);
	params_ExportColors.add(bExportPreset_DefaultPath);
	params_ExportColors.add(path_Folder_ExportColor_Custom);
	params_AppState.add(params_ExportColors);

	// amount colors
	params_AppState.add(numColors_Engines);
	//params_AppState.add(numColors_Range);
	//params_AppState.add(numColors_Alg);

	params_AppState.add(analogSpread);

	params_Panels.add(SHOW_ALL_GUI);
	params_Panels.add(SHOW_Presets);
	params_Panels.add(SHOW_UserPalette);
	params_Panels.add(SHOW_UserPaletteEditor);
	params_Panels.add(SHOW_BackGround);
	params_Panels.add(SHOW_Library);
	params_Panels.add(SHOW_Picker);
	params_Panels.add(SHOW_Range);
	params_Panels.add(SHOW_Theory);
	params_Panels.add(SHOW_Panels);
	params_Panels.add(SHOW_Export);
	params_Panels.add(SHOW_Demos);
	//params_Panels.add(SHOW_Demo2);
	params_Panels.add(SHOW_Quantizer);
	params_Panels.add(SHOW_ColourLovers);
	params_Panels.add(SHOW_AlgoPalettes);
	params_Panels.add(SHOW_GradientCurve);
	params_Panels.add(SHOW_BrowserColors);
#ifdef INCL_LAYOUT
	params_Panels.add(SHOW_Gui_Internal);
#endif
	params_Panels.add(SHOW_MINI_Preview);
	params_Panels.add(SHOW_Kit);
	params_Panels.add(AutoScroll);
	params_AppState.add(params_Panels);

	params_Background.add(color_BackGround);
	params_Background.add(color_BackGround_Darker);
	params_Background.add(AutoSet_BackGround_Color);
	params_Background.add(color_BackGround_Darkness);
	params_Background.add(color_BackGround_Lock);
	params_AppState.add(params_Background);

	params_Demo.add(TEST_Mode);
	params_Demo.add(DEMO1_Test);
	params_Demo.add(DEMO2_Test);
	params_Demo.add(DEMO_Auto);
	params_Demo.add(DEMO_Timer);
	params_Demo.add(DEMO_Alpha);
	params_Demo.add(DEMO_Cam);
	params_AppState.add(params_Demo);

	////TODO: must included/linked as presets
	////hide
	params_Gradient.add(bAutoPaletteFromGradient);
	params_Gradient.add(curve_Gradient_Exp);
	params_Gradient.add(curve_Gradient_PickIn);
	params_Gradient.add(gradient_HardMode);//gradient
	//////params_Gradient.add(MODE_EditGradientLayout);
	//////params_Gradient.add(SHOW_Gradient);
	//////params_Gradient.add(SHOW_CosineGradient);
	//////params_Gradient.add(paletteLibPage_param);
	//params_AppState.add(params_Gradient);

	params_Picker.add(BRIGHTNESS);
	params_Picker.add(SATURATION);
	params_Picker.add(bColor_HUE);
	params_Picker.add(bColor_SAT);
	params_Picker.add(bColor_BRG);
	params_Picker.add(color_HUE_0);
	params_Picker.add(color_SAT_0);
	params_Picker.add(color_BRG_0);
	params_Picker.add(color_HUE_Power);
	params_Picker.add(color_SAT_Power);
	params_Picker.add(color_BRG_Power);
	params_Picker.add(color_Picked);
	params_AppState.add(params_Picker);

	params_Library.add(lib_Page_Index);
	params_Library.add(lib_MaxColumns);
	params_Library.add(lib_NumRows);
	params_Library.add(lib_CardsMode);
	params_Library.add(scale_LibCol);
	params_Library.add(scale_ColRange);
	params_Library.add(lib_Responsive_Mode);
	params_Library.add(bPagerized);
	params_Library.add(sizeLibColBox);
	params_AppState.add(params_Library);

	params_Palette2.add(sizePaletteBox);
	params_Palette2.add(scale_ColPalette);
	params_Palette2.add(bResponsive_Presets);
	params_Palette2.add(boxMaxRows);
	//params_Palette2.add(bEditPalette);
	//params_Palette2.add(bAuto_Build_Palette);
	//params_Palette2.add(bAutoResizePalette);
	params_AppState.add(params_Palette2);

	//---

	// grouped on callback

	params_control.setName("ofxColorManager");

	params_control.add(bNewPreset);
	params_control.add(numColors_Engines);
	params_control.add(AppMode);
	params_control.add(last_Index_Theory);
	params_control.add(last_Index_Range);

	params_control.add(curve_Gradient_Exp);
	params_control.add(curve_Gradient_PickIn);
	params_control.add(gradient_HardMode);

	//params_control.add(color_Picked);
	//params_control.add(color_BackGround);
	// edit palette
	params_control.add(bRandomColor);
	params_control.add(bEditPalette);
	params_control.add(bAddColor);
	params_control.add(bRemoveColor);
	params_control.add(bClearPalette);
	// background
	params_control.add(color_backGround_SET);
	params_control.add(AutoSet_BackGround_Color);
	params_control.add(color_BackGround_Darker);
	params_control.add(color_BackGround_Darkness);
	params_control.add(color_BackGround_Lock);
	params_control.add(AutoSet_Background_FromGradient);
	// panels
	params_control.add(SHOW_Panels);
	params_control.add(SHOW_Export);
	params_control.add(SHOW_Demos);
	//params_control.add(SHOW_Demo2);
	params_control.add(SHOW_BrowserColors);
	params_control.add(SHOW_GradientCurve);
	params_control.add(MODE_EditGradientLayout);
	params_control.add(SHOW_Presets);
	params_control.add(SHOW_BackGround);
	params_control.add(SHOW_Library);
	params_control.add(SHOW_Theory);
	params_control.add(SHOW_Range);
	params_control.add(SHOW_Picker);
	params_control.add(SHOW_ColourLovers);
	params_control.add(SHOW_Quantizer);
	params_control.add(SHOW_ColourLovers_searcher);
	params_control.add(SHOW_UserPalette);
	params_control.add(SHOW_UserPaletteEditor);
	params_control.add(SHOW_AlgoPalettes);
	params_control.add(SHOW_ALL_GUI);
	//params_control.add(SHOW_PresetsPalette);
	//params_control.add(SHOW_Gradient);
	//params_control.add(SHOW_CosineGradient);
	//params_control.add(SHOW_PaletteCustom);

	ofAddListener(params_control.parameterChangedE(), this, &ofxColorManager::Changed_Controls);

	//---

//#ifdef USE_OFX_GUI
//	//panel.setup();
//	//panel.add(group);
//	//panel.setPosition(1000, 400);
//#endif

	//------------------------------------------------

	// startup

	startup();
}

//--------------------------------------------------------------
void ofxColorManager::startup()
{
	//--

	ENABLE_keys = true;

	//--

	// curve

	// edit layout

	// a. initialize
	rGradientPreview.setRect(600, 200, 755, 295);

	// b. load settings
	//rGradientPreview.loadSettings();
	rGradientPreview.loadSettings("_Curve_Gui", path_Layout, false);

	refresh_Gui_Layout();

	//-

	// preset

	//bResetCurve = true;

	// lut
	curvesTool.load(path_Gradient_LUT); //needed because it fills polyline

	// preset
	ofxSurfingHelpers::loadGroup(params_Gradient, path_Gradient_Preset);

	curve_Slider_ExpTweak.setup(slider_PickIn_x, slider_PickIn_y, slider_PickIn_w, slider_PickIn_h, 0, 1, curve_Gradient_Exp, true, true);
	curve_Slider_ExpTweak.setPercent(curve_Gradient_Exp);

	curve_Slider_Pick.setup(slider_Exp_x, slider_Exp_y, slider_Exp_w, slider_Exp_h, 0, 1, curve_Gradient_PickIn, true, true);
	curve_Slider_Pick.setPercent(curve_Gradient_PickIn);

	//----

	// preset manager

	preset_RefreshFiles();
	//preset_Load(PRESET_Name);

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

	panels.addToggle(&SHOW_GradientCurve);
	panels.addToggle(&SHOW_Presets);
	//panels.addToggle(&SHOW_PresetsPalette);

	panels.addToggle(&SHOW_ALL_GUI);
	panels.addToggle(&SHOW_MINI_Preview);

	panels.addToggle(&SHOW_Demos);
	//panels.addToggle(&SHOW_Demo2);
	panels.addToggle(&DEMO1_Test);
	panels.addToggle(&DEMO2_Test);

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

	// color ranges
	setup_Range();
	//generate_Range(color_1_Range.get(), color_2_Range.get());

	ofxSurfingHelpers::loadGroup(params_AppState, path_AppState);

	//--

	//startup
	last_Index_Theory = 0;
	theoryTypes[last_Index_Theory] = true;//on button

	last_Index_Range = 0;
	rangTypes[last_Index_Range] = true;//on button

	//--

	//load last session preset index
	if (preset_Index < files.size())
	{
		PRESET_Name = files_Names[preset_Index];
		preset_Load(PRESET_Name);
	}

	//--

	//refresh theory and ranges
	refresh_Picker_Touched();
}

//--------------------------------------------------------------
void ofxColorManager::update(ofEventArgs & args)
{
	//----

	// export engine

	if (bOpen) //programmed open dialog from ImGui:
	{
		//Open the Open File Dialog
		ofFileDialogResult openFileResult = ofSystemLoadDialog("Select the export path", true);

		//Check if the user opened a file
		if (openFileResult.bSuccess)
		{
			//processOpenFileSelection(openFileResult);

			path_Folder_ExportColor_Custom = openFileResult.getPath();

			path_Folder_ExportColor_Custom += "\\";// windows

			//path_FileExport = path_Folder_ExportColor_Custom.get() + "\\liveColors.json";
			path_FileExport = path_Folder_ExportColor_Custom.get() + path_Name_ExportColor + ".json";
			bExportPreset_DefaultPath = false;//flag
			ofLogNotice(__FUNCTION__) << "User selected a path: " << path_Folder_ExportColor_Custom;
		}
		else
		{
			ofLogNotice(__FUNCTION__) << "User hit cancel";
			path_Folder_ExportColor_Custom = "";
			bExportPreset_DefaultPath = true;//set back default OF_APP/bin/data
		}

		bOpen = false;
	}

	//----

	// DEMO1

	{
		if (DEMO1_Test)
		{
			myDEMO1.update();

			if (DEMO_Auto) {}
			if ((ofGetElapsedTimeMillis() - Demo_Timer) > MAX(Demo_Timer_Max * (1 - DEMO_Timer), 10))
			{
				Demo_Timer = ofGetElapsedTimeMillis();

				myDEMO1.start();
			}
		}
	}


	//----

	// DEMO2

	{
		if (DEMO2_Test) myDEMO2.update();
	}

	//----

	// gradient

	update_Gradient();

	//----

	// colour lovers

	{
		colourLoversHelper.update();
	}

	//----

	//TODO:
	// some mode has clicked/changed some palette 

	//bUpdated_Palette_BACK = true;// to trig this

	if (bUpdated_Palette_BACK)
	{
		bUpdated_Palette_BACK = false;

		ofLogNotice(__FUNCTION__) << "  >  bUpdated_Palette_BACK : " << bUpdated_Palette_BACK;

		reBuild();

	}

	//----

	// 2. color pick from palette clicked

	if (bUpdated_Color_BACK)
	{
		bUpdated_Color_BACK = false;
		ofLogWarning(__FUNCTION__) << "  >  bUpdated_Color_BACK : " << bUpdated_Color_BACK;

		// 2. get color from colour lovers
		color_Clicked2 = ofColor(myColor);

		reBuild();
	}

	//--

	//TODO:
	//not used and hidden
	// 3. color clicked
	if (color_Clicked2 != color_Clicked2_PRE && SHOW_ColourLovers)
	{
		ofLogNotice(__FUNCTION__) << "  >  color_Clicked2 !";
		color_Clicked2_PRE = color_Clicked2;
		color_Picked.set(color_Clicked2);
	}

	//---

	// 4. set the local scope color pointer that is into ofxColorBrowser that whill be used as color picked too

	if (color_BACK != color_BACK_PRE)
	{
		color_BACK_PRE = color_BACK;

		ofLogNotice(__FUNCTION__) << "  >  color_BACK ! ";
		//ofLogNotice(__FUNCTION__) << "Changed color_BACK pointer";

		color_Picked.set(color_BACK);
	}

	//-

	if (MODE_EditGradientLayout)
	{
		refresh_Gui_Layout();
	}

	//--

	////cosine gradient
	//cosineGradient_update();

	//--

	// preset manager

	//myPresetManager.update();

	//----

	// ofxGuiPanelsLayout

#ifdef INCL_LAYOUT
	panels.update();
#endif
}

//--------------------------------------------------------------
void ofxColorManager::refresh_Libs()
{
	lib_TotalColors = colorBrowser.colors_STRUCT.size();
	lib_MaxColumns = colorBrowser.getSizeCards();//better fit layout

	lib_Page_NumColors = lib_NumRows * lib_RowSize;
	lib_Page_Max = lib_TotalColors / lib_Page_NumColors - 1;

	palette_Lib_Cols.clear();
	palette_Lib_Cols = colorBrowser.getPalette();

	palette_Lib_Names.clear();
	palette_Lib_Names = colorBrowser.getNames();

	lib_Page_Index = 0;
	last_ColorPicked_Lib = 0;
}

//--------------------------------------------------------------
void ofxColorManager::update_Curve()
{
	float out = ofMap(curvesTool.getAtPercent(1.0 - curve_Ctrl_In), 0, MAX_LUT_SIZE - 1, 1, 0);
	colCurveTest = gradient.getColorAtPercent(out);
}

//--------------------------------------------------------------
void ofxColorManager::draw_Curve()
{
	gui_Gradient();

	//----

	ofColor c;

	//-

	if (SHOW_GradientCurve)
	{
		// 1. draggable bg box

		ofPushStyle();
		ofFill();
		ofSetColor(ofColor(0, 220));
		rGradientPreview.draw();
		ofDrawRectRounded(rGradientPreview, 5);
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
		//if (DEMO1_Test)
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

	std::string t0 = PRESET_Name;
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
	int _alphaGhost = 32;

	c0.set(0, 200);
	c1.set(255, 200);

	c0_Ghost.set(0, _alphaGhost);
	c1_Ghost.set(255, _alphaGhost);

	//TODO: create double line shadow draw methoid into ofxSurfingHelpers

	padh = 15;
	h = padh;
	y += h;

	//preset name
	//if (t0 != "" || txt_lineActive[0])//hide now used lines vs draw but transparent
	if (txt_lineActive[0])//hide now used lines vs draw but transparent
	{
		x = ofGetWidth() * 0.5 - _w0 * 0.5;
		if (txt_lineActive[i]) ofSetColor(c0);
		else ofSetColor(c0_Ghost);
		fontBig.drawString(t0, x + sp, y + sp);

		if (txt_lineActive[i]) ofSetColor(c1);
		else ofSetColor(c1_Ghost);
		fontBig.drawString(t0, x, y);

		padh = 25;
		h = fontBig.getSize() + padh;
		y += h;
	}
	i++;

	//lover name
	//if (t1 != "" || txt_lineActive[1])
	if (txt_lineActive[1])
	{
		x = ofGetWidth() * 0.5 - _w1 * 0.5;
		if (txt_lineActive[i]) ofSetColor(c0);
		else ofSetColor(c0_Ghost);
		fontBig.drawString(t1, x + sp, y + sp);

		if (txt_lineActive[i]) ofSetColor(c1);
		else ofSetColor(c1_Ghost);
		fontBig.drawString(t1, x, y);

		padh = -10;
		h = fontBig.getSize() + padh;
		y += h;
	}
	i++;

	//theory name
	//if (t2 != "" || txt_lineActive[1])
	if (txt_lineActive[2])
	{
		x = ofGetWidth() * 0.5 - _w2 * 0.5;
		if (txt_lineActive[i]) ofSetColor(c0);
		else ofSetColor(c0_Ghost);
		fontMedium.drawString(t2, x + sp, y + sp);

		if (txt_lineActive[i]) ofSetColor(c1);
		else ofSetColor(c1_Ghost);
		fontMedium.drawString(t2, x, y);

		padh = 20;
		h = fontMedium.getSize() + padh;
		y += h;
	}
	i++;

	//range name
	//if (t3 != "" || txt_lineActive[3])
	if (txt_lineActive[3])
	{
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
#ifndef AUTO_DRAW_CALLBACK
void ofxColorManager::draw()
#endif
#ifdef AUTO_DRAW_CALLBACK
void ofxColorManager::draw(ofEventArgs & args)
#endif
{
	if (SHOW_Scene)
	{
		// background
		if (background_Draw_ENABLE)
		{
			//if (DEMO2_Test) 
			//{
			//	ofClear(8);
			//}
			//else
			{
				if (AutoSet_BackGround_Color.get() && !color_BackGround_Lock)// && SHOW_GradientCurve.get()) 
				{
					ofClear(colCurveTest);

					//workflow
					color_BackGround.set(colCurveTest);
				}
				else
				{
					ofClear(ofColor(color_BackGround.get()));
				}
			}
		}

		//--

		//// cosine gradient
		//if (SHOW_CosineGradient)
		//{
		//    mCosineGradient.render(glm::vec2(0), ofGetWidth(), ofGetHeight());
		//}

		//--

		// presets

		//if (SHOW_Presets)
		//{
		//	myPresetManager.draw();
		//	//PRESET_Temp.draw();
		//}

		//--

		// DEMO2
		float _w = myDEMO2.getWidth();
		if (DEMO2_Test) myDEMO2.draw(glm::vec2(ofGetWidth() * 0.5 - _w * 0.5, 0));

		// DEMO1
		if (DEMO1_Test) myDEMO1.draw(DEMO_Alpha);

		//--

		if (SHOW_ImGui)
		{
			// quantizer
#ifdef USE_IMAGE_QUANTIZER
			if (SHOW_Quantizer) colorQuantizer.draw();
#endif
			//--

			// info
			draw_Info();
		}
	}

	//--

	// mini

	if (SHOW_MINI_Preview)
	{
		miniPreview.draw_MiniPreview(PRESET_Name, palette, color_BackGround.get());
		//ofxColorClient::draw_MiniPreview(font, PRESET_Name, palette, color_BackGround.get());
		//(SHOW_MINI_Preview) draw_MiniPreview();
	}

	//--

	// ofxGuiPanelsLayout

#ifdef INCL_LAYOUT
	if (SHOW_Gui_Internal) panels.draw();
#endif

	//--

	// gui

	if (SHOW_ImGui)
	{
		if (guiVisible)
		{
			mouseOverGui = draw_Gui();
			//mouseOverGui = ofxImGui::IsMouseOverGui();

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

				//myDEMO1.setEnableMouseCamera();
			}
		}
	}

	//----

	// 1. help info
	if (ENABLE_HelpInfo)
	{
		std::string str = infoHelp;
		ofPushMatrix();
		{
			//center box
			int w = ofxSurfingHelpers::getWidthBBtextBoxed(font, str);
			int h = ofxSurfingHelpers::getHeightBBtextBoxed(font, str);
			ofTranslate(ofGetWidth() * 0.5 - w * 0.5, ofGetHeight() * 0.5 - h * 0.5);
			ofSetColor(255, 255);
			ofxSurfingHelpers::drawTextBoxed(font, str);
		}
		ofPopMatrix();
	}
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

	//----

	// curve

	// layout
	//rGradientPreview.saveSettings();
	rGradientPreview.saveSettings("_Curve_Gui", path_Layout, false);

	// preset

	// gradient curve
	curvesTool.save(path_Gradient_LUT);//needed because it fills polyline

	// refresh before save
	curve_Gradient_Exp = curve_Slider_ExpTweak.getValue();
	curve_Gradient_PickIn = curve_Slider_Pick.getValue();

	// curve preset
	ofxSurfingHelpers::saveGroup(params_Gradient, path_Gradient_Preset);

	//----

	// app state
	ofxSurfingHelpers::saveGroup(params_AppState, path_AppState);

	//----

	color_Picked.removeListener(this, &ofxColorManager::Changed_ColorPicked);
	color_Clicked.removeListener(this, &ofxColorManager::Changed_ColorClicked);

	ofRemoveListener(params_control.parameterChangedE(), this, &ofxColorManager::Changed_Controls);
	ofRemoveListener(params_color.parameterChangedE(), this, &ofxColorManager::Changed_Controls);
	ofRemoveListener(params_Theory.parameterChangedE(), this, &ofxColorManager::Changed_Controls);
	ofRemoveListener(params_curve.parameterChangedE(), this, &ofxColorManager::Changed_Controls);

	ofRemoveListener(params_Ranges.parameterChangedE(), this, &ofxColorManager::Changed_ColorRange);

	ofRemoveListener(params_algoTypes.parameterChangedE(), this, &ofxColorManager::Changed_ColorTheory);
	ofRemoveListener(params_ColorTheory.parameterChangedE(), this, &ofxColorManager::Changed_ColorTheory);

	ofRemoveListener(params_Palette.parameterChangedE(), this, &ofxColorManager::Changed_ParamsPalette);

	//--

	ofRemoveListener(ofEvents().update, this, &ofxColorManager::update);
#ifdef AUTO_DRAW_CALLBACK
	ofRemoveListener(ofEvents().draw, this, &ofxColorManager::draw);
#endif

	removeKeysListeners();
	removeMouseListeners();

	//--

	// ofxGuiPanelsLayout
#ifdef INCL_LAYOUT
	panels.exit();
#endif
}

//--------------------------------------------------------------
void ofxColorManager::refresh_Gui_Layout()
{
	curve_Slider_ExpTweak.setVisible(SHOW_GradientCurve);
	curve_Slider_Pick.setVisible(SHOW_GradientCurve);

	//-

	//between panels in curve sections
	int _pad1 = 30;
	int _pad2 = 3;

	int _xx, _yy, _ww, _hh;

	//if (MODE_EditGradientLayout)
	if (1)
	{
		_xx = rGradientPreview.getX() + _pad1;
		_yy = rGradientPreview.getY() + _pad1;
		_ww = rGradientPreview.getWidth() + (2 * _pad1);
		_hh = rGradientPreview.getHeight() + (2 * _pad1);
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
	curveTool_w = MAX_LUT_SIZE;
	curveTool_h = MAX_LUT_SIZE;

	//--

	// curve mod slider
	slider_PickIn_x = curveTool_x + curveTool_w + _pad2;
	slider_PickIn_y = _yy;
	slider_PickIn_w = box_size_user;
	slider_PickIn_h = curveTool_h;

	//--

	// gradient-pre curve (bad sorted to the left but anchored to curve..)
	grad_x = slider_PickIn_x + 0.5 * slider_PickIn_w + _pad2;
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
	slider_Exp_x = image_curvedGradient_x + image_curvedGradient_w + _pad2;
	slider_Exp_y = _yy;
	slider_Exp_w = box_size_user;
	slider_Exp_h = curveTool_h;

	//--

	curve_Slider_Pick.setup(slider_PickIn_x, slider_PickIn_y, slider_PickIn_w, slider_PickIn_h, 0, 1, curve_Gradient_PickIn, true, true);
	curve_Slider_ExpTweak.setup(slider_Exp_x, slider_Exp_y, slider_Exp_w, slider_Exp_h, 0, 1, curve_Gradient_Exp, true, true);

	//--	

	// current color box/bar (the one affected by slider position. just for testing gradient purpose)
	currColor_x = slider_Exp_x + slider_Exp_w + _pad2;
	currColor_y = _yy;

	//----
}

//--------------------------------------------------------------
void ofxColorManager::palette_rearrenge()
{
	//refresh_Palette_TARGET(palette);
}


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
	static bool auto_resize = true;
	float ww, hh;
	ww = PANEL_WIDGETS_WIDTH;
	hh = PANEL_WIDGETS_HEIGHT;
	ImGui::SetWindowSize(ImVec2(ww, hh));
	ImGuiWindowFlags flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : 0;

	//--

	if (ofxImGui::BeginWindow("THEORY", mainSettings, flags))
	{
		//--

		// box size
		static int _cSize = 37;

		ImGuiColorEditFlags _flags;

		float _spc = ImGui::GetStyle().ItemInnerSpacing.x;
		float _w = ImGui::GetWindowContentRegionWidth() - _spc;
		int _h = int(COLOR_STRIP_COLOR_HEIGHT);
		float _w50 = MAX(150, _w * 0.33);

		//-

		//_flags = ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoTooltip;

		//-

		_flags =
			ImGuiColorEditFlags_NoSmallPreview |
			ImGuiColorEditFlags_NoTooltip |
			ImGuiColorEditFlags_NoLabel |
			ImGuiColorEditFlags_NoSidePreview |
			ImGuiColorEditFlags_NoAlpha |
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

		//----

		//mini box
		if (ImGui::ColorButton("##BoxTheory", *(ImVec4 *)&tmpRef.r, _flags, ImVec2(_w, _h)))
		{
		}

		//-

		// two pickers

		bool bUpdate = false;

		if (ImGui::CollapsingHeader("BASE COLOR"))
		{
			ImGui::Columns(2, NULL, false);

			ImGui::PushItemWidth(_w50);

			// 1. wheel
			if (ImGui::ColorPicker3("##PickerTheory", &tmpRef.r, _flags))
			{
				ofLogNotice(__FUNCTION__) << "Wheel Picker Theory : moved";

				color_TheoryBase.set(tmpRef);
				color_Picked = color_TheoryBase.get();

				bUpdate = true;

				//-
			}

			ImGui::PopItemWidth();

			//--

			// 2. square

			_flags =
				ImGuiColorEditFlags_NoSmallPreview |
				ImGuiColorEditFlags_NoTooltip |
				ImGuiColorEditFlags_NoLabel |
				ImGuiColorEditFlags_NoSidePreview |
				ImGuiColorEditFlags_NoAlpha |
				ImGuiColorEditFlags_PickerHueBar;

			ImGui::NextColumn();

			ImGui::PushItemWidth(_w50);

			//if (ImGui::CollapsingHeader("SQUARE"))
			{
				if (ImGui::ColorPicker4("##SquareTheory", (float *)&tmpRef, _flags))
				{
					ofLogNotice(__FUNCTION__) << "Square Picker Theory : moved";

					color_TheoryBase.set(tmpRef);
					color_Picked = color_TheoryBase.get();

					bUpdate = true;
				}
			}

			ImGui::PopItemWidth();

			ImGui::Columns(1);
		}

		//----

		// controls

		ImGui::Dummy(ImVec2(0, 5));

		//ImGui::SameLine();

		if (ofxSurfingHelpers::AddIntStepped(numColors_Engines))
		{
		}
		// amount colors
		//if (ofxImGui::AddParameter(numColors_Engines))
		//{
		//}
		//if (ImGui::InputInt(numColors_Theory.getName().c_str(), (int*)&numColors_Theory.get(), 1, 5))
		//if (ofxImGui::AddParameter(numColors_Theory))
		//{
		//}
		//if (ofxImGui::AddParameter(numColors_Alg))
		//{
		//}

		if (ImGui::CollapsingHeader("Advanced"))
		{
			ofxImGui::AddParameter(bAuto_TheoryFromPicker);
			ofxImGui::AddParameter(bAuto_Build_Palette);
			ofxImGui::AddParameter(analogSpread);
			// layout
			ImGui::Dummy(ImVec2(0, 5));
			ImGui::Checkbox("Auto-resize", &auto_resize);
		}

		//--

		ImGui::Dummy(ImVec2(0, 5));

		//--

		// 1. basic

		for (int i = 0; i < NUM_COLOR_THEORY_TYPES_G1; i++)
		{
			// border to selected
			bool bDrawBorder = false;
			if (i == last_Index_Theory)
			{
				bDrawBorder = true;
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

			// label type buttons 

			const size_t _total = colorsTheory[i].size();

			for (int n = 0; n < _total; n++)
			{
				ImGui::PushID(n);

				//-			

				ImGui::SameLine();

				if (ImGui::ColorButton("##ColorButtonTheory",
					colorsTheory[i][n],
					ImGuiColorEditFlags_NoAlpha |
					ImGuiColorEditFlags_NoPicker |
					ImGuiColorEditFlags_NoTooltip,
					ImVec2(_cSize, _cSize)))
				{
					last_Theory_PickPalette = n;
				}

				//-

				ImGui::PopID();
			}
		}

		//----

		// 2. extra

		for (int i = 0; i < NUM_COLOR_THEORY_TYPES_G2; i++)
		{
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

			//-

			// border to selected
			bool bDrawBorder = false;
			if (i + NUM_COLOR_THEORY_TYPES_G1 == last_Index_Theory)
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

				if (ImGui::ColorButton("##BoxPaletteAlgorithm",

					c,
					ImGuiColorEditFlags_NoAlpha |
					ImGuiColorEditFlags_NoPicker |
					ImGuiColorEditFlags_NoTooltip,
					ImVec2(_cSize, _cSize)))
				{
					last_Theory_PickPalette = n + colorsTheory[i].size();

					//TODO:
					color_Picked.set(c);
				}

				ImGui::PopID();
			}
		}

		//----

		if (bUpdate)
		{
			refresh_FromPicked();
		}
	}
	ofxImGui::EndWindow(mainSettings);
}


//--------------------------------------------------------------
void ofxColorManager::gui_PaletteEditor()
{
	static bool auto_resize = false;
	//static bool auto_resize = true;
	//bool auto_resize = bAutoResizePalette.get();

	float ww, hh;
	ww = PANEL_WIDGETS_WIDTH;
	hh = PANEL_WIDGETS_HEIGHT;
	ImGui::SetWindowSize(ImVec2(ww, hh));
	ImGuiWindowFlags flagsw;
	flagsw = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : 0;
	//flagsw = false;

	//-

	if (ofxImGui::BeginWindow("PALETTE EDITOR", mainSettings, flagsw))
	{
		float _spc = ImGui::GetStyle().ItemInnerSpacing.x;
		float _w = ImGui::GetWindowContentRegionWidth() - 2 * _spc;
		//float _w = PANEL_WIDGETS_WIDTH - 2 * _spc;
		float _w50 = _w * 0.5;
		float _h = BUTTON_BIG_HEIGHT;

		ImGuiColorEditFlags _flags;

		//--

		//if (SHOW_PresetsPalette)
		//if (ImGui::CollapsingHeader("Palette", _flagw))
		{
			//responsive
			ImVec2 button_sz((float)sizePaletteBox.get(), (float)sizePaletteBox.get());
			ImGuiStyle& style = ImGui::GetStyle();
			int buttons_count = palette.size();
			float _wx2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

			//--

			enum Mode
			{
				Mode_Copy,
				Mode_Move,
				Mode_Swap
			};
			static int mode = 2;

			//--

			for (int n = 0; n < palette.size(); n++)
			{
				// color picker
				//if (n == last_ColorPicked_Palette)
				//{
				//}

				//----

				ImGui::PushID(n);
				{
					// a. fixed size
					int _max = 6;//amount colors per row
					int _r = (int)MIN(_max, palette.size());
					//split in rows
					if ((n % _r) != 0)
					{
						if (n != 0)
						{
							ImGui::SameLine();
						}
					}

					//----

					int wb;
					wb = (_w / _r) - (1.5 * _spc);
					wb = wb * scale_ColPalette.get();

					//--

					_flags =
						ImGuiColorEditFlags_NoSmallPreview |
						ImGuiColorEditFlags_NoTooltip |
						ImGuiColorEditFlags_NoLabel |
						ImGuiColorEditFlags_NoSidePreview |
						ImGuiColorEditFlags_NoInputs |
						ImGuiColorEditFlags_NoAlpha;

					//--

					// color box

					ImVec2 bb;
					//if (bResponsive_Presets) bb = button_sz;
					//else bb = ImVec2(wb, wb);
					bb = ImVec2(wb, wb);

					//-

					// border to selected
					bool bDrawBorder = false;
					if (n == palette_colorSelected && bEditPalette)
					{
						bDrawBorder = true;
						ImGui::PushStyleColor(ImGuiCol_Border, color_Pick);
						ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, linew_Pick);
					}

					//-

					if (ImGui::ColorButton("##paletteDragPrst",
						palette[n],
						_flags,
						bb))
					{
						//last_ColorPicked_Palette = n;
						palette_colorSelected = n;
					}

					//-

					if (bDrawBorder && bEditPalette)
					{
						ImGui::PopStyleColor();
						ImGui::PopStyleVar(1);
					}

					//----

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
								myDEMO2.setPaletteColors(palette);
							}
							if (mode == Mode_Move)
							{
								palette[n] = palette[payload_n];
								palette[payload_n] = ofColor(0);
								myDEMO2.setPaletteColors(palette);
							}
							if (mode == Mode_Swap)
							{
								const ofColor tmp = palette[n];
								palette[n] = palette[payload_n];
								palette[payload_n] = tmp;
								myDEMO2.setPaletteColors(palette);
							}

							//--

							// update all pallete (Curve)
							ofLogNotice(__FUNCTION__) << "DONE Dragged _c";

							build_GradientPalette();
						}
						ImGui::EndDragDropTarget();
					}

					//----
				}
				ImGui::PopID();

				//--

				//// responsive buttons size
				//if (bResponsive_Presets)
				//{
				//	ImGui::PopStyleColor();
				//	float last_button_x2 = ImGui::GetItemRectMax().x;
				//	float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
				//	if (n + 1 < buttons_count && next_button_x2 < _wx2) ImGui::SameLine();
				//	ImGui::PopID();
				//}
			}

			//----

			// edit

			if (ImGui::CollapsingHeader("Edit"))
			{
				ofxSurfingHelpers::AddBigButton(bAddColor, _w50, _h * 0.5); ImGui::SameLine();
				ofxSurfingHelpers::AddBigButton(bRemoveColor, _w50, _h * 0.5);
				ofxSurfingHelpers::AddBigToggle(bEditPalette, _w50, _h * 0.5); ImGui::SameLine();
				ofxSurfingHelpers::AddBigToggle(bClearPalette, _w50, _h * 0.5);
				//ofxImGui::AddParameter(bClearPalette);
				if (bEditPalette) {
					ofxImGui::AddParameter(palette_colorSelected);
				}
			}

			//--

			if (ImGui::CollapsingHeader("Arrange"))
			{
				ImGui::Dummy(ImVec2(0, 5));

				if (ImGui::RadioButton("Copy", mode == Mode_Copy)) { mode = Mode_Copy; } ImGui::SameLine();
				if (ImGui::RadioButton("Move", mode == Mode_Move)) { mode = Mode_Move; } ImGui::SameLine();
				if (ImGui::RadioButton("Swap", mode == Mode_Swap)) { mode = Mode_Swap; }

				ImGui::Dummy(ImVec2(0, 5));

				//flip
				if (ofxSurfingHelpers::AddSmallButton(bFlipUserPalette, 100, 0.5 * BUTTON_BIG_HEIGHT))
				{
				}
			}

			//--

			//ImGui::Separator();
			//ImGui::Dummy(ImVec2(0, 5));
			//ImGui::Text("PALETTE PANEL");
			//ImGui::Dummy(ImVec2(0, 5));
			//ImGui::Dummy(ImVec2(0, 5));

			ofxSurfingHelpers::AddBigToggle(SHOW_UserPalette, _w, _h * 0.5);

			if (ImGui::CollapsingHeader("Layout"))
			{
				ofxImGui::AddParameter(bResponsive_Presets);

				if (bResponsive_Presets)
				{
					ImGui::InputInt(sizePaletteBox.getName().c_str(), (int*)&sizePaletteBox.get(), 5, 100);
					//ofxImGui::AddParameter(sizePaletteBox);
				}
				if (!bResponsive_Presets)
				{
					boxMaxRows.disableEvents();
					ImGui::InputInt(boxMaxRows.getName().c_str(), (int*)&boxMaxRows.get(), 1, 5);
					//ImGui::InputInt(boxMaxRows.getName().c_str(), (int *)&boxMaxRows.get());
					boxMaxRows = ofClamp(boxMaxRows.get(), boxMaxRows.getMin(), boxMaxRows.getMax());
					boxMaxRows.enableEvents();

					//ofxImGui::AddParameter(scale_ColPalette);
					ImGui::InputFloat(scale_ColPalette.getName().c_str(), (float *)&scale_ColPalette.get(), 0.005f, 0.010);
				}

				//ofxImGui::AddParameter(bAutoResizePalette);//not works
			}

			//--

			//ImGui::Checkbox("Auto-resize", &auto_resize);
		}
	}
	ofxImGui::EndWindow(mainSettings);
}

//--------------------------------------------------------------
void ofxColorManager::gui_Palette()
{
	static bool auto_resize = false;
	//static bool auto_resize = true;
	//bool auto_resize = bAutoResizePalette.get();

	float ww, hh;
	ww = PANEL_WIDGETS_WIDTH;
	hh = PANEL_WIDGETS_HEIGHT;
	ImGui::SetWindowSize(ImVec2(ww, hh));
	ImGuiWindowFlags flagsw;
	flagsw = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : 0;
	//flagsw = false;

	//-

	if (ofxImGui::BeginWindow("PALETTE", mainSettings, flagsw))
	{
		//ImGui::Checkbox("autoResize", &auto_resize);

		//--

		ImGuiColorEditFlags _flags;

		//_flags =
		//	ImGuiColorEditFlags_NoSmallPreview |
		//	ImGuiColorEditFlags_NoTooltip |
		//	ImGuiColorEditFlags_NoLabel |
		//	ImGuiColorEditFlags_NoSidePreview |
		//	ImGuiColorEditFlags_NoInputs |
		//	ImGuiColorEditFlags_NoAlpha;

		_flags = false;

		//-

		float _spc;
		float _w;
		float _w50;
		float _w20;
		float _h;

		_spc = ImGui::GetStyle().ItemInnerSpacing.x;

		//_w = ImGui::GetWindowContentRegionWidth();
		if (auto_resize) _w = ww;
		else _w = ImGui::GetWindowContentRegionWidth() - 3 * _spc;
		_w50 = _w * 0.5;
		_w20 = _w * 0.2;

		_h = BUTTON_BIG_HEIGHT;

		//-

		// responsive
		ImVec2 button_sz((float)sizePaletteBox.get(), (float)sizePaletteBox.get());

		ImGuiStyle& style = ImGui::GetStyle();
		int buttons_count = palette.size();
		float _wx2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
		//--

		enum Mode
		{
			Mode_Copy,
			Mode_Move,
			Mode_Swap
		};
		static int mode = 2;

		//-

		// color buttons

		for (int n = 0; n < palette.size(); n++)
		{
			ImGui::PushID(n);

			// responsive buttons size
			if (bResponsive_Presets)
			{
				std::string name = ofToString(n);

				const ImVec4 color2 = style.Colors[ImGuiCol_Button];//do not changes the color
				ImGui::PushStyleColor(ImGuiCol_Button, color2);

				////customize colors
				//if (n == indexBrowser)//when selected
				//{
				//	const ImVec4 color1 = ImVec4(0.1, 0.1, 0.1, 0.8);//changes button color to black
				//	ImGui::PushStyleColor(ImGuiCol_Button, color1);
				//}
				//else //not selected
				//{
				//	const ImVec4 color2 = style.Colors[ImGuiCol_Button];//do not changes the color
				//	ImGui::PushStyleColor(ImGuiCol_Button, color2);
				//}
			}

			//--

			if (!bResponsive_Presets)
			{
				//split in rows
				if (boxMaxRows != 0)
				{
					if ((n % boxMaxRows) != 0)
					{
						if (n != 0)
						{
							ImGui::SameLine();
						}
					}
				}
			}

			//--

			int _r = (int)MIN(boxMaxRows.get(), palette.size());

			_spc = ImGui::GetStyle().ItemInnerSpacing.x;
			_w = ImGui::GetWindowContentRegionWidth() - 3 * _spc;

			int wb;
			wb = (_w / _r) - (1.5 * _spc);
			wb = wb * scale_ColPalette.get();

			//--

			_flags =
				ImGuiColorEditFlags_NoSmallPreview |
				ImGuiColorEditFlags_NoDragDrop |
				ImGuiColorEditFlags_NoTooltip |
				ImGuiColorEditFlags_NoLabel |
				ImGuiColorEditFlags_NoSidePreview |
				ImGuiColorEditFlags_NoInputs |
				ImGuiColorEditFlags_NoAlpha;

			//--

			// color box

			ImVec2 bb;//size
			if (bResponsive_Presets) bb = button_sz;
			else bb = ImVec2(wb, wb);

			if (ImGui::ColorButton("##paletteDrag",
				palette[n],
				_flags,
				bb))
			{
				//last_ColorPicked_Palette = n;
				palette_colorSelected = n;
			}

			//----

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
						myDEMO2.setPaletteColors(palette);
					}
					if (mode == Mode_Move)
					{
						palette[n] = palette[payload_n];
						palette[payload_n] = ofColor(0);
						myDEMO2.setPaletteColors(palette);
					}
					if (mode == Mode_Swap)
					{
						const ofColor tmp = palette[n];
						palette[n] = palette[payload_n];
						palette[payload_n] = tmp;
						myDEMO2.setPaletteColors(palette);
					}

					//--

					// update all pallete (Curve)
					ofLogNotice(__FUNCTION__) << "DONE Dragged _c";

					build_GradientPalette();
				}
				ImGui::EndDragDropTarget();
			}

			//----

			// responsive buttons size
			if (bResponsive_Presets)
			{
				ImGui::PopStyleColor();
				float last_button_x2 = ImGui::GetItemRectMax().x;
				float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
				if (n + 1 < buttons_count && next_button_x2 < _wx2) ImGui::SameLine();
			}

			ImGui::PopID();
		}

		ImGui::Dummy(ImVec2(0, 5));
	}
	ofxImGui::EndWindow(mainSettings);
}

//--------------------------------------------------------------
void ofxColorManager::gui_Library()
{
	if (ofxImGui::BeginWindow("LIBRARY", mainSettings))
	{
		bool bUpdate = false;

		//--

		float _spc = ImGui::GetStyle().ItemInnerSpacing.x;
		float _w = ImGui::GetWindowContentRegionWidth() - _spc;

		int _colsSize;
		if (lib_CardsMode) _colsSize = lib_RowSize;
		else _colsSize = lib_MaxColumns;

		//--

		// a. pantone cards: 7 colors / row
		//lib_Page_NumColors = lib_NumRows * lib_RowSize;

		// b. responsive
		lib_Page_NumColors = lib_NumRows * lib_MaxColumns.get();

		lib_Page_Max = lib_TotalColors / (lib_Page_NumColors - 1);
		lib_Page_Index.setMax(lib_Page_Max - 1);

		//--

		// pagerize
		int lib_StartCol;
		int lib_EndCol;

		if (bPagerized)
		{
			lib_StartCol = lib_Page_Index * lib_Page_NumColors;
			lib_EndCol = lib_StartCol + lib_Page_NumColors;
		}
		else
		{
			lib_StartCol = 0;
			lib_EndCol = lib_TotalColors - 1;
		}

		//--

		// get color from outside color picked

		//maybe required because get() causes callbacks too (?)
		ENABLE_Callbacks_cPickers = false;
		ImVec4 _c = color_Picked.get();
		ENABLE_Callbacks_cPickers = true;

		//--

		std::string s;

		//ImGui::Dummy(ImVec2(0, 5));

		// lib name
		s = colorBrowser.getNameLib();
		ImGui::Text(s.c_str());

		ImGui::Dummy(ImVec2(0, 5));

		// color name
		ImGui::Text(last_Lib_NameColor.c_str());

		//ImGui::Dummy(ImVec2(0, 5));

		// index
		s = ofToString(last_Lib_Index) + "/" + ofToString(lib_TotalColors - 1);
		ImGui::Text(s.c_str());

		//--

		// advanced

		if (ImGui::CollapsingHeader("Advanced"))
		{
			if (ImGui::CollapsingHeader("Layout"))
			{
				ImGui::InputInt(sizeLibColBox.getName().c_str(), (int*)&sizeLibColBox.get(), 1, 5);
				ofxImGui::AddParameter(lib_Responsive_Mode);
				ofxImGui::AddParameter(bPagerized);

				//-

				// responsive

				if (!lib_Responsive_Mode)
				{
					ofxImGui::AddParameter(lib_CardsMode);
					ImGui::InputFloat(scale_LibCol.getName().c_str(), (float *)&scale_LibCol.get(), 0.02f, 0.1f);

					if (!lib_CardsMode)
					{
						ImGui::InputInt(lib_NumRows.getName().c_str(), (int*)&lib_NumRows.get(), 1, 5);
						ImGui::InputInt(lib_MaxColumns.getName().c_str(), (int*)&lib_MaxColumns.get(), 1, 5);

					}
				}
			}

			//--

			// 2.2 draw all palette colors grid

			//--

#ifdef USE_EXTRA_LIBRARIES
			//ImGui::Dummy(ImVec2(0, 5));

			// library

			if (ImGui::CollapsingHeader("Library"))
			{
				ofxImGui::AddParameter(colorBrowser.LibraryColors_Index);
				ofxImGui::AddParameter(colorBrowser.LibraryColors_Index_name);

				ImGui::Dummy(ImVec2(0, 5));

				ofxImGui::AddParameter(colorBrowser.MODE_SORTING);
				ofxImGui::AddParameter(colorBrowser.MODE_SORTING_name);

				ImGui::Dummy(ImVec2(0, 5));

				ofxImGui::AddParameter(colorBrowser.ENABLE_keys);
			}
#endif
		}

		//--

		// arrow buttons

		ImGui::Dummy(ImVec2(0, 5));

		if (bPagerized)
		{
			ImGui::PushButtonRepeat(true);

			// prev
			if (ImGui::ArrowButton("##left", ImGuiDir_Left))
			{
				if (lib_Page_Index > 0)
				{
					lib_Page_Index--;
				}
			}

			ImGui::SameLine(0, _spc);

			// next
			if (ImGui::ArrowButton("##right", ImGuiDir_Right))
			{
				if (lib_Page_Index < lib_Page_Max)
				{
					lib_Page_Index++;
				}
			}

			ImGui::PopButtonRepeat();

			//ImGui::SameLine();

			//-

			// page
			ofxImGui::AddParameter(lib_Page_Index);//page slider selector
			//ImGui::SliderInt("PAGE", &lib_Page_Index, 0, lib_Page_Max);//page slider selector
			//ImGui::DragInt("PAGE", (int *)&lib_Page_Index, 0, lib_Page_Max);//collide..
		}

		ImGui::Dummy(ImVec2(0, 5));

		//----

		// grid colors

		// responsive
		ImVec2 _sz((float)sizeLibColBox.get(), (float)sizeLibColBox.get());

		ImGuiStyle& style = ImGui::GetStyle();
		int _countBtns;
		if (lib_Responsive_Mode) _countBtns = lib_TotalColors;
		else _countBtns = lib_EndCol - lib_StartCol;
		float _wx2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

		//--

		for (int n = lib_StartCol; n < lib_EndCol; n++)
		{
			//--

			// responsive buttons size
			if (lib_Responsive_Mode)
			{
				ImGui::PushID(n);

				const ImVec4 color2 = style.Colors[ImGuiCol_Button];//do not changes the color
				ImGui::PushStyleColor(ImGuiCol_Button, color2);
			}

			//--

			// color box

			{
				ImGui::PushID(n);

				//ImGui::PushStyleVar(ImGui::ImGuiStyleVar_FrameBorderSize), 1.0f;

				if (!lib_Responsive_Mode)
				{
					if ((n % _colsSize) != 0)
					{
						ImGui::SameLine(0, ImGui::GetStyle().ItemSpacing.y);//vertical inter line
					}
				}

				// draw border to selected color
				bool bDrawBorder = false;
				if (n == last_ColorPicked_Lib)
				{
					bDrawBorder = true;
					ImGui::PushStyleColor(ImGuiCol_Border, color_Pick);
					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, linew_Pick);
				}

				//--

				// color button

				ImGuiColorEditFlags _flags =
					ImGuiColorEditFlags_NoAlpha |
					ImGuiColorEditFlags_NoPicker |
					ImGuiColorEditFlags_NoTooltip;

				ImVec2 _bb;
				if (lib_Responsive_Mode) _bb = _sz;
				else _bb = ImVec2(sizeLibColBox * scale_LibCol, sizeLibColBox * scale_LibCol);

				ofFloatColor _c = ofColor(palette_Lib_Cols[n]);

				if (ImGui::ColorButton("##paletteLib",
					_c,
					_flags,
					_bb))

				{
					// picked
					color_Picked = _c;

					// index
					last_ColorPicked_Lib = n;
					last_Lib_Index = n;

					// color name
					if (n < palette_Lib_Names.size())
					{
						last_Lib_NameColor = palette_Lib_Names[n];

						std::string str = "Lib Picked: [" + ofToString(last_ColorPicked_Lib) + "] " +
							last_Lib_NameColor;
						ofLogNotice(__FUNCTION__) << str;
					}

					//-

					bUpdate = true;
				}

				//----

				//TODO: 
				// draw border to selected color
				if (bDrawBorder)
				{
					ImGui::PopStyleColor();
					ImGui::PopStyleVar(1);
				}

				ImGui::PopID();
			}

			//--

			// responsive buttons size
			if (lib_Responsive_Mode)
			{
				ImGui::PopStyleColor();

				float last_button_x2 = ImGui::GetItemRectMax().x;

				float next_button_x2 = last_button_x2 + style.ItemSpacing.x + _sz.x; // Expected position if next button was on same line

				if (n + 1 < _countBtns && next_button_x2 < _wx2)
				{
					ImGui::SameLine(0, ImGui::GetStyle().ItemSpacing.y);//vertical inter line
				}

				ImGui::PopID();
			}
		}
		//ofxImGui::EndTree(mainSettings);

		//----

		if (bUpdate)
		{
			refresh_FromPicked();
		}
	}
	ofxImGui::EndWindow(mainSettings);
}

//--------------------------------------------------------------
void ofxColorManager::gui_Export()
{
	static bool auto_resize = true;
	float ww, hh;
	ww = PANEL_WIDGETS_WIDTH;
	hh = PANEL_WIDGETS_HEIGHT;
	ImGui::SetWindowSize(ImVec2(ww, hh));
	ImGuiWindowFlags flagsw;
	flagsw = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : 0;

	//--

	if (ofxImGui::BeginWindow("Live Export", mainSettings, flagsw))
	{
		float _spc = ImGui::GetStyle().ItemInnerSpacing.x;

		//float _h = 0.5f * BUTTON_BIG_HEIGHT;
		float _h = float(COLOR_STRIP_COLOR_HEIGHT);
		float _w;

		//if (auto_resize) _w = ww;
		//else _w = ImGui::GetWindowContentRegionWidth() - 2 * _spc;
		_w = ImGui::GetWindowContentRegionWidth() - 2 * _spc;
		float _w50 = _w * 0.5f;
		float _w100 = _w + 2.5f * _spc;

		//--

		//ImGui::Separator();
		//ImGui::Dummy(ImVec2(0, 10));

		//if (ImGui::CollapsingHeader("Live Export"))
		{
			//ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());
			if (ImGui::Button("Export", ImVec2(_w, BUTTON_BIG_HEIGHT)))
			{
				exportPalette();
			}
			//ImGui::PopItemWidth();

			ofxImGui::AddParameter(bAutoExportPreset);

			if (ImGui::CollapsingHeader("Advanced"))
			{
				ofxImGui::AddParameter(bExportPreset_DefaultPath);
				if (ImGui::Button("Set Path", ImVec2(_w, 0.5*BUTTON_BIG_HEIGHT)))
				{
					bOpen = true;
				}

				ImGui::Dummy(ImVec2(0, 2));
				ImGui::Text("Path:");
				if (!bExportPreset_DefaultPath) {
					ImGui::Text(path_Folder_ExportColor_Custom.get().c_str());
				}
				else {
					std::string ss = "/data/" + path_Folder_ExportColor_Data.get();
					ImGui::Text(ss.c_str());
					//ImGui::Text(path_Folder_ExportColor_Data.get().c_str());
				}

				ImGui::Dummy(ImVec2(0, 2));
				ImGui::Text("Preset Name:");
				ImGui::Text(PRESET_Temp.name.c_str());

				ImGui::Dummy(ImVec2(0, 2));
				ImGui::Text("Gradient Name:");
				ImGui::Text(path_Name_Gradient.c_str());

				ImGui::Dummy(ImVec2(0, 5));
				ImGui::Checkbox("Auto-resize", &auto_resize);
			}
		}
	}
	ofxImGui::EndWindow(mainSettings);
}

//--------------------------------------------------------------
void ofxColorManager::gui_Picker()
{
	static bool auto_resize = true;
	float ww, hh;
	ww = PANEL_WIDGETS_WIDTH;
	hh = PANEL_WIDGETS_HEIGHT;
	ImGui::SetWindowSize(ImVec2(ww, hh));
	ImGuiWindowFlags flagsw;
	flagsw = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : 0;

	//--

	if (ofxImGui::BeginWindow("PICKER", mainSettings, flagsw))
	{
		float _spc = ImGui::GetStyle().ItemInnerSpacing.x;

		//float _h = 0.5f * BUTTON_BIG_HEIGHT;
		float _h = float(COLOR_STRIP_COLOR_HEIGHT);
		float _w;

		if (auto_resize) _w = ww;
		else _w = ImGui::GetWindowContentRegionWidth() - 2 * _spc;
		float _w50 = _w * 0.5f;
		float _w100 = _w + 2.5f * _spc;

		//--

		// get color from outside color picked

		ImVec4 cTmp = color_Picked.get();
		//static ImVec4 cTmp = color_Picked.get();

		bool bChg_Pick = false;//to concentrate calbacks once
		bool bChg_HSB = false;//to concentrate calbacks once

		//--

		// 0. box color

		ImGuiColorEditFlags _flags = ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoTooltip;

		ImGui::ColorButton("##PickerBox", *(ImVec4 *)&cTmp, _flags, ImVec2(_w, _h));
		//if (0)
		//{
		//	ImGui::ColorButton("##PickerBox", *(ImVec4 *)&colCurveTest, _flags, ImVec2(_w, _h));
		//}
		//else
		//{
		//	ImGui::ColorButton("##PickerBox", *(ImVec4 *)&cTmp, _flags, ImVec2(_w, _h));
		//}

		//-

		if (ofxImGui::BeginTree("COLOR PICKERS", mainSettings))//grouped folder
		{
			ImGuiColorEditFlags _flagw;

			//--

			_flagw = ImGuiTreeNodeFlags_None;
			//_flagw = ImGuiTreeNodeFlags_DefaultOpen;
			if (ImGui::CollapsingHeader("WHEEL", _flagw))
			{
				// 1. color picker

				// 1.1 circled
				ImGuiColorEditFlags _flags =
					ImGuiColorEditFlags_NoSmallPreview |
					ImGuiColorEditFlags_NoTooltip |
					ImGuiColorEditFlags_NoLabel |
					ImGuiColorEditFlags_NoSidePreview |
					ImGuiColorEditFlags_HSV |
					ImGuiColorEditFlags_RGB |
					ImGuiColorEditFlags_NoInputs |
					ImGuiColorEditFlags_NoAlpha |
					ImGuiColorEditFlags_PickerHueWheel;

				//-

				ImGui::PushItemWidth(_w);

				if (ImGui::ColorPicker4("##PickerWheel", (float *)&cTmp, _flags))
				{
					ofLogNotice(__FUNCTION__) << "Wheel Picker : moved";

					ENABLE_Callbacks_cPickers = false;
					color_Picked = cTmp;
					ENABLE_Callbacks_cPickers = true;

					bChg_Pick = true;
				}

				////TODO:
				//if (0)
				//{
				//	if (ImGui::ColorPicker4("MyColor##Wheel", (float *)&colCurveTest, _flags))
				//	{
				//		ofLogNotice(__FUNCTION__) << "Wheel Picker : moved";
				//		ENABLE_Callbacks_cPickers = false;
				//		color_Picked = colCurveTest;
				//		ENABLE_Callbacks_cPickers = true;
				//	}
				//}
				//else
				//{
				//	if (ImGui::ColorPicker4("##PickerWheel", (float *)&cTmp, _flags))
				//	{
				//		ofLogNotice(__FUNCTION__) << "Wheel Picker : moved";
				//		ENABLE_Callbacks_cPickers = false;
				//		color_Picked = cTmp;
				//		ENABLE_Callbacks_cPickers = true;
				//		bChg_Pick = true;
				//	}
				//}

				ImGui::PopItemWidth();
			}

			//--

			// 2. square
			_flagw = ImGuiTreeNodeFlags_DefaultOpen;
			if (ImGui::CollapsingHeader("SQUARE", _flagw))
			{
				_flags =
					ImGuiColorEditFlags_NoSmallPreview |
					ImGuiColorEditFlags_NoTooltip |
					ImGuiColorEditFlags_NoLabel |
					ImGuiColorEditFlags_NoSidePreview |
					ImGuiColorEditFlags_HSV |
					ImGuiColorEditFlags_RGB |
					ImGuiColorEditFlags_HDR |
					ImGuiColorEditFlags_NoAlpha |
					ImGuiColorEditFlags_PickerHueBar;

				ImGui::PushItemWidth(_w);

				if (ImGui::ColorPicker4("##PickerSquare", (float *)&cTmp, _flags))
				{
					ofLogNotice(__FUNCTION__) << "Square Picker : moved";

					ENABLE_Callbacks_cPickers = false;
					color_Picked = cTmp;
					ENABLE_Callbacks_cPickers = true;

					bChg_Pick = true;

					//if (0)
					//{
					//	ENABLE_Callbacks_cPickers = false;
					//	color_Picked = colCurveTest;
					//	ENABLE_Callbacks_cPickers = true;
					//}
					//else
					//{
					//	ENABLE_Callbacks_cPickers = false;
					//	color_Picked = cTmp;
					//	ENABLE_Callbacks_cPickers = true;
					//	bChg_Pick = true;
					//}
				}

				ImGui::PopItemWidth();
			}

			//--

			ofxImGui::EndTree(mainSettings);
		}

		//----

		// 2. HSB

		//if (ImGui::CollapsingHeader("HSB"))
		if (ofxImGui::BeginTree("HSB", mainSettings))
		{
			ImGui::PushItemWidth(_w * 0.80);
			//ImGui::PushItemWidth(_w50);
			//ImGui::PushItemWidth(_w);

			// TODO: ?
			//should apply hsb here, not into callback, 
			//because it will trig
			//the color picked updating callback!!!

			ENABLE_Callbacks_cPickers = false; //disable callbacks
			{
				if (ofxImGui::AddParameter(color_HUE))
				{
					ofLogNotice(__FUNCTION__) << "H : moved";

					ofColor c;
					c.set(color_Picked.get());
					c.setHue(color_HUE);
					color_Picked.set(c);

					bChg_HSB = true;//to concentrate calbacks once

					//refresh_Picker_Touched();
				}
				if (ofxImGui::AddParameter(color_SAT))
				{
					ofLogNotice(__FUNCTION__) << "S : moved";

					ofColor c;
					c.set(color_Picked.get());
					c.setSaturation(color_SAT);
					color_Picked.set(c);

					bChg_HSB = true;//to concentrate calbacks once

					//refresh_Picker_Touched();
				}
				if (ofxImGui::AddParameter(color_BRG))
				{
					ofLogNotice(__FUNCTION__) << "B : moved";

					ofColor c;
					c.set(color_Picked.get());
					c.setBrightness(color_BRG);
					color_Picked.set(c);

					bChg_HSB = true;//to concentrate calbacks once

					//refresh_Picker_Touched();
				}
			}
			ENABLE_Callbacks_cPickers = true;

			//-

			ImGui::PopItemWidth();

			ofxImGui::EndTree(mainSettings);
		}

		//--

		if (ImGui::CollapsingHeader("Advanced"))
		{
			if (ImGui::CollapsingHeader("RANDOMIZER"))
			{
				ofxSurfingHelpers::AddBigButton(bRandomColor, _w, _h);
				//ofxImGui::AddParameter(bRandomColor);

				//enablers
				//ofxSurfingHelpers::AddBigToggle(bColor_HUE);
				//ofxSurfingHelpers::AddBigToggle(bColor_SAT);
				//ofxSurfingHelpers::AddBigToggle(bColor_BRG);

				ImGui::Dummy(ImVec2(0, 5));

				if (bColor_HUE) color_HUE_0 = color_HUE;
				if (bColor_SAT) color_SAT_0 = color_SAT;
				if (bColor_BRG) color_BRG_0 = color_BRG;

				if (ofxImGui::AddParameter(bColor_HUE)) {}ImGui::SameLine();
				if (ofxImGui::AddParameter(bColor_SAT)) {}ImGui::SameLine();
				if (ofxImGui::AddParameter(bColor_BRG)) {}

				ImGui::Dummy(ImVec2(0, 5));

				//if (ofxImGui::AddParameter(color_HUE_0))
				//{
				//	ofLogNotice(__FUNCTION__) << "HUE 0 : moved";
				//	if (bColor_HUE) {
				//		color_HUE = color_HUE_0;
				//	}
				//}
				//if (ofxImGui::AddParameter(color_SAT_0))
				//{
				//	ofLogNotice(__FUNCTION__) << "SAT 0 : moved";
				//	if (bColor_SAT) {
				//		color_SAT = color_SAT_0;
				//	}
				//}
				//if (ofxImGui::AddParameter(color_BRG_0))
				//{
				//	ofLogNotice(__FUNCTION__) << "BRG 0 : moved";
				//	if (bColor_BRG) {
				//		color_BRG = color_BRG_0;
				//	}
				//}

				ImGui::Dummy(ImVec2(0, 5));

				if (ofxImGui::AddParameter(color_HUE_Power))
				{
					ofLogNotice(__FUNCTION__) << "HUE P : moved";
				}
				if (ofxImGui::AddParameter(color_SAT_Power))
				{
					ofLogNotice(__FUNCTION__) << "SAT P : moved";
				}
				if (ofxImGui::AddParameter(color_BRG_Power))
				{
					ofLogNotice(__FUNCTION__) << "BRG P : moved";
				}
			}

			//----

			//to concentrate calbacks once
			//color_Picked
			if (bChg_Pick && bChg_HSB) //all
			{
				refresh_FromPicked();
			}
			else
			{
				if (bChg_Pick)
				{
					ENABLE_Callbacks_cPickers = false;
					refresh_Picked_toHSB();
					ENABLE_Callbacks_cPickers = true;
				}
				if (bChg_Pick || bChg_HSB)
				{
					refresh_Picker_Touched();
				}
			}

			ImGui::Checkbox("Auto-resize", &auto_resize);
		}
	}
	ofxImGui::EndWindow(mainSettings);
}

//--------------------------------------------------------------
void ofxColorManager::gui_Panels()
{
	ImGuiWindowFlags flags;
	flags = ImGuiWindowFlags_None;

	//static bool auto_resize = true;
	//flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : 0;//TODO: not working for my toggles

	//static bool bResize = false;
	//if (!bResize) {
	//	flags = 
	//		//ImGuiWindowFlags_NoDecoration ||
	//		ImGuiWindowFlags_NoBackground ||
	//		ImGuiWindowFlags_NoMove ||
	//		ImGuiWindowFlags_NoResize ||
	//		ImGuiWindowFlags_NoResize// ||
	//		//ImGuiWindowFlags_NoTitleBar ||
	//		;
	//}
	//else {
	//	flags = ImGuiWindowFlags_None;
	//}

	//----

	if (ofxImGui::BeginWindow("MAIN PANEL", mainSettings, flags))
	{
		//ImGui::Columns(1);

		//app mode
		//ImGui::Text(AppMode_name.get().c_str());
		//ofxImGui::AddStepper(AppMode, 1, 2);

		const char* current_element_name = (current_element >= 0 && current_element <
			Element_COUNT) ? element_names[current_element] : "Unknown";

		if (ImGui::SliderInt("APP MODE", &current_element, 0, Element_COUNT - 1, current_element_name))
			if (current_element != AppMode) AppMode = current_element;

		ImGui::Dummy(ImVec2(0, 10));

		//--

		ImGui::Columns(3);

		ofxSurfingHelpers::AddBigToggle(SHOW_UserPalette);
		ofxSurfingHelpers::AddBigToggle(SHOW_Picker);
		ofxSurfingHelpers::AddBigToggle(SHOW_BackGround);
		ofxSurfingHelpers::AddBigToggle(SHOW_Library);
		ofxSurfingHelpers::AddBigToggle(SHOW_GradientCurve);

		ImGui::NextColumn();
		//ImGui::Separator();

		ofxSurfingHelpers::AddBigToggle(SHOW_Theory);
		ofxSurfingHelpers::AddBigToggle(SHOW_Range);
		ofxSurfingHelpers::AddBigToggle(SHOW_ColourLovers);
		ofxSurfingHelpers::AddBigToggle(SHOW_Quantizer);

		ImGui::NextColumn();
		//ImGui::Separator();

		//ofxSurfingHelpers::AddBigToggle(SHOW_ALL_GUI);
		ofxSurfingHelpers::AddBigToggle(SHOW_Presets);
		ofxSurfingHelpers::AddBigToggle(SHOW_MINI_Preview);
		ofxSurfingHelpers::AddBigToggle(SHOW_Demos);
		//ofxSurfingHelpers::AddBigToggle(SHOW_Demo2);
		ofxSurfingHelpers::AddBigToggle(SHOW_Export);
		ofxImGui::AddParameter(ENABLE_keys);

#ifdef INCL_LAYOUT
		ofxSurfingHelpers::AddBigToggle(SHOW_Gui_Internal);
#endif
		//ImGui::Separator();
		//ImGui::Dummy(ImVec2(0, 10));
		//ofxImGui::AddParameter(SHOW_Gradient);
		//ofxImGui::AddParameter(SHOW_AlgoPalettes);
		//ofxImGui::AddParameter(SHOW_BrowserColors);

		//ImGui::Columns(1);

		//ImGui::Checkbox("Resize", &bResize);
	}
	ofxImGui::EndWindow(mainSettings);
}

//--------------------------------------------------------------
void ofxColorManager::gui_Range()
{
	static bool auto_resize = true;
	float ww, hh;
	ww = PANEL_WIDGETS_WIDTH;
	hh = PANEL_WIDGETS_HEIGHT;
	ImGui::SetWindowSize(ImVec2(ww, hh));
	ImGuiWindowFlags flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : 0;

	//--

	if (ofxImGui::BeginWindow("RANGE", mainSettings, flags))
	{
		//ImGui::Dummy(ImVec2(0, 10));

		//--

		float _sz = int(BUTTON_COLOR_SIZE) * scale_ColRange.get();
		float _szLabel = 100;

		int _h = int(COLOR_STRIP_COLOR_HEIGHT);
		float _spc = ImGui::GetStyle().ItemInnerSpacing.x;
		float _w = ImGui::GetWindowContentRegionWidth() - _spc;
		float _w50 = _w * 0.45f;
		float _w100 = _w;

		ofFloatColor _c1;
		ofFloatColor _c2;

		//-

		ImGuiColorEditFlags _flags;

		_flags =
			ImGuiColorEditFlags_NoSmallPreview |
			ImGuiColorEditFlags_NoTooltip |
			ImGuiColorEditFlags_NoLabel |
			ImGuiColorEditFlags_NoSidePreview |
			ImGuiColorEditFlags_HSV |
			ImGuiColorEditFlags_RGB |
			ImGuiColorEditFlags_NoAlpha |
			ImGuiColorEditFlags_PickerHueWheel;

		//----

		// 1. pickers
		{
			bool bChanged = false;//some picker changed

			ImGui::Dummy(ImVec2(0, 5));

			_c1 = color_1_Range.get();
			_c2 = color_2_Range.get();

			//ImGui::Spacing(50, 0);
			{
				ImGui::Columns(2);

				ImGui::PushItemWidth(_w50);
				ImGui::Text("Color1");
				ofxSurfingHelpers::AddBigToggle(autoPick_RangeColor1, _w50, BUTTON_BIG_HEIGHT);
				//ofxImGui::AddParameter(autoPick_RangeColor1);
				// mini box
				if (ImGui::ColorButton("##C1", *(ImVec4 *)&_c1[0], _flags, ImVec2(_w50, _h)))
				{
					bChanged = true;
					refresh_Range_AutoUpdate();
				}
				ImGui::PopItemWidth();

				ImGui::NextColumn();

				ImGui::PushItemWidth(_w50);
				ImGui::Text("Color2");
				ofxSurfingHelpers::AddBigToggle(autoPick_RangeColor2, _w50, BUTTON_BIG_HEIGHT);
				//ofxImGui::AddParameter(autoPick_RangeColor2);
				// mini box
				if (ImGui::ColorButton("##C2", *(ImVec4 *)&_c2[0], _flags, ImVec2(_w50, _h)))
				{
					bChanged = true;
					refresh_Range_AutoUpdate();
				}
				ImGui::PopItemWidth();

				ImGui::Columns(1);
			}

			//----

			if (ImGui::CollapsingHeader("PICKERS"))
			{
				// picker 1

				ImGui::Columns(2);

				// wheel
				ImGui::PushItemWidth(MIN(_w50, PANEL_WIDGETS_WIDTH));
				if (ImGui::ColorPicker3("1", &_c1[0], _flags))
				{
					if (autoPick_RangeColor1) color_Picked.set(_c1[0]);
					bChanged = true;
					refresh_Range_AutoUpdate();
				}
				ImGui::PopItemWidth();

				//--

				ImGui::NextColumn();

				//----

				// picker 2

				// wheel
				ImGui::PushItemWidth(MIN(_w50, PANEL_WIDGETS_WIDTH));
				if (ImGui::ColorPicker3("2", &_c2[0], _flags))
				{
					if (autoPick_RangeColor2) color_Picked.set(_c2[0]);
					bChanged = true;
					refresh_Range_AutoUpdate();
				}
				ImGui::PopItemWidth();

				//--
			}

			//----

			ImGui::Columns(1);

			//ImGui::Dummy(ImVec2(0, 10));

			// set picked colors
			if (bChanged)
			{
				color_1_Range.set(_c1);
				color_2_Range.set(_c2);

				//workflow
				//get main color from range picker
				if (autoPick_RangeColor1) color_Picked.set(color_1_Range.get());
				else if (autoPick_RangeColor2) color_Picked.set(color_2_Range.get());
			}

			//----

			// autogenerate

			if (autoGenerate_Range && bChanged)
			{
				generate_Range(color_1_Range.get(), color_2_Range.get());
			}

			if (!autoGenerate_Range)
			{
				//if (ofxSurfingHelpers::AddBigButton("GENERATE"))
				if (ImGui::Button("GENERATE", ImVec2(_w, 0.5 * BUTTON_BIG_HEIGHT)))
				{
					generate_Range(color_1_Range.get(), color_2_Range.get());
				}
			}

			//ofxSurfingHelpers::AddSmallButton(bGetPaletteFromRange, 150, 30);

			//-

			//ImGui::Dummy(ImVec2(0, 5));

			//-

			if (ofxSurfingHelpers::AddIntStepped(numColors_Range))
			{
			}
			//ofxImGui::AddParameter(numColors_Range);
			//if (ImGui::InputInt(numColors_Range.getName().c_str(), (int *)&numColors_Range.get(), 1, 2))
			//{}

			if (ImGui::CollapsingHeader("Advanced"))
			{
				ofxImGui::AddParameter(bAuto_Build_Palette);
				ofxImGui::AddParameter(autoGenerate_Range);

				//ImGui::Dummy(ImVec2(0, 5));

				if (ImGui::CollapsingHeader("Layout"))
				{
					// layout
					ImGui::Checkbox("Auto Resize", &auto_resize);
					ImGui::InputFloat(scale_ColRange.getName().c_str(), (float *)&scale_ColRange.get(), 0.02f, 0.1f);
					//ofxImGui::AddParameter(scale_ColRange);
				}
			}

			ImGui::Dummy(ImVec2(0, 5));
		}

		//----

		// 2. draw all palette colors grid

		for (int t = 0; t < int(NUM_TYPES_RANGES); t++)
		{
			// 2.1 each labels itself for each row (1st box)
			{
				// label button

				//-

				// border to selected
				bool bDrawBorder = false;
				if (t == last_Index_Range)
				{
					bDrawBorder = true;
					ImGui::PushStyleColor(ImGuiCol_Border, color_Pick);
					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, linew_Pick);
				}

				//-

				// label name 
				//whick range lab type

				if (ofxSurfingHelpers::AddSmallButton(rangTypes[t], _szLabel, _sz))
				{
					//ofLogNotice(__FUNCTION__) << rangTypes[t].getName() << " #" << t;// ? allways triggering..
				}

				//-

				if (bDrawBorder)
				{
					ImGui::PopStyleColor();
					ImGui::PopStyleVar(1);
				}

				ImGui::SameLine();

				//----

				// 2.2 colors boxes

				for (int c = 0; c < numColors_Range.get(); c++)
				{
					ImGui::SameLine(0, ImGui::GetStyle().ItemSpacing.y);//vertical inter line

					int cc = t * numColors_Range.get() + c;
					ImGui::PushID(cc);
					if (cc < palette_Range.size())
					{
						if (ImGui::ColorButton(
							"##PaletteRange",
							palette_Range[cc],
							_flags,
							ImVec2(_sz, _sz)))
						{
							ofLogNotice(__FUNCTION__) << "Range Box: " << t << "-" << c;
						}
					}
					ImGui::PopID();
				}
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
}

//--------------------------------------------------------------
void ofxColorManager::gui_Gradient()
{
	static bool auto_resize = true;
	float ww, hh;
	ww = PANEL_WIDGETS_WIDTH;
	hh = PANEL_WIDGETS_HEIGHT;
	ImGui::SetWindowSize(ImVec2(ww, hh));
	ImGuiWindowFlags flagsw;
	flagsw = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : 0;

	//--

	if (ofxImGui::BeginWindow("GRADIENT", mainSettings, flagsw))
	{
		//--

		float _spc;
		float _w;
		float _w50;
		float _w20;
		float _h;

		_spc = ImGui::GetStyle().ItemInnerSpacing.x;

		//_w = ImGui::GetWindowContentRegionWidth();
		if (auto_resize) _w = ww;
		else _w = ImGui::GetWindowContentRegionWidth() - 3 * _spc;
		_w50 = _w * 0.5;
		_w20 = _w * 0.2;

		_h = BUTTON_BIG_HEIGHT;

		//-

		ImGuiColorEditFlags _flagw;
		_flagw = false;

		//--

		//if (ImGui::TreeNode("EDIT"))
		if (ofxImGui::BeginTree("EDIT", mainSettings))
		{
			ImGui::Dummy(ImVec2(0, 5));

			// reset

			ImGui::PushItemWidth(_w);
			if (ImGui::Button(bResetCurve.getName().c_str(), ImVec2(_w, _h)))
				//if (ImGui::Button(bResetCurve.getName().c_str()))
				//if (ImGui::Button(bResetCurve.getName().c_str(), ImVec2(_w, _h)))
			{
				bResetCurve = true;
				//rGradientPreview.setRect(600, 200, 755, 295);
				//refresh_Gui_Layout();
			}
			ImGui::PopItemWidth();
			ImGui::Dummy(ImVec2(0, 5));

			ImGui::PushItemWidth(_w20);
			ofxImGui::AddParameter(gradient_HardMode);
			ofxImGui::AddParameter(AutoSet_Background_FromGradient);
			ofxImGui::AddParameter(bAutoPaletteFromGradient);
			//ImGui::Checkbox("AutoSet Background", &AutoSet_Background_FromGradient);
			//ofxSurfingHelpers::AddBigButton(bResetCurve);
			//ofxImGui::AddParameter(bResetCurve);
			//ofxImGui::AddParameter(curve_Gradient_TEST_Prc);
			//ofxImGui::AddParameter(pos_CurveEditor);
			ImGui::PopItemWidth();

			//-

			//ImGui::TreePop();
			ofxImGui::EndTree(mainSettings);
		}

		ImGui::Dummy(ImVec2(0, 5));

		//--

		// curve Test

		//if (ImGui::TreeNode("CURVE TEST"))
		//if (ImGui::CollapsingHeader("CURVE TEST", _flagw))
		if (ofxImGui::BeginTree("CURVE TEST", mainSettings))
		{
			ImGui::PushItemWidth(_w50);

			ofxImGui::AddParameter(TEST_Mode);
			//ImGui::Checkbox("Enable", &TEST_Mode); 

			if (TEST_Mode)
			{
				ImGui::SameLine();
				ImGui::Checkbox("LFO", &TEST_LFO_Mode);
			}
			ImGui::SliderFloat("Speed", &TEST_Speed, 0.0f, 1.0f);

			ImGui::PopItemWidth();

			//-

			//ImGui::TreePop();
			//ofxImGui::EndTree(mainSettings);
		}

		ImGui::Dummy(ImVec2(0, 5));

		//--

		if (ImGui::CollapsingHeader("Advanced", _flagw))
		{
			// ctrl in/out
			ImGui::PushItemWidth(_w50);

			ofxImGui::AddParameter(curve_Gradient_Exp);

			ImGui::Dummy(ImVec2(0, 2));

			if (ofxImGui::AddParameter(curve_Gradient_PickIn))
			{
				curve_Ctrl_In = curve_Gradient_PickIn;
			}

			ofxImGui::AddParameter(curve_Ctrl_Out);

			ImGui::PopItemWidth();

			//-

			ImGui::Dummy(ImVec2(0, 10));
			ImGui::PushItemWidth(_w);
			if (ofxSurfingHelpers::AddBigToggle(MODE_EditGradientLayout, _w, 0.5 * _h))
			{
			}
			//ofxImGui::AddParameter(MODE_EditGradientLayout);
			ImGui::PopItemWidth();

			ImGui::Checkbox("Auto-resize", &auto_resize);
		}
	}
	ofxImGui::EndWindow(mainSettings);
}

//--------------------------------------------------------------
void ofxColorManager::gui_Background()
{
	static bool auto_resize = true;
	float ww, hh;
	ww = PANEL_WIDGETS_WIDTH;
	hh = PANEL_WIDGETS_HEIGHT;
	ImGui::SetWindowSize(ImVec2(ww, hh));
	ImGuiWindowFlags flagsw = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : 0;

	//--

	if (ofxImGui::BeginWindow("BACKGROUND", mainSettings, flagsw))
	{
		//--

		float _spc;
		float _w;
		float _w50;
		float _w20;
		float _h;

		_spc = ImGui::GetStyle().ItemInnerSpacing.x;
		//_w = ImGui::GetWindowContentRegionWidth();

		if (auto_resize) _w = ww;
		else _w = ImGui::GetWindowContentRegionWidth() - 3 * _spc;
		_w50 = _w * 0.5;
		_w20 = _w * 0.2;

		_h = BUTTON_BIG_HEIGHT;

		//-

		{
			ofxImGui::AddParameter(background_Draw_ENABLE);

			//-

			static ImVec4 color;
			color = color_BackGround.get();

			ImGui::Dummy(ImVec2(0, 5));

			//-

			// 0. color big

			float _w = ImGui::GetWindowContentRegionWidth();
			int _h = int(COLOR_STRIP_COLOR_HEIGHT);

			ImGuiColorEditFlags _flags;
			_flags = ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoTooltip;

			ImGui::ColorButton("##ColorBgPicker", *(ImVec4 *)&color, _flags, ImVec2(_w, _h));

			ImGui::Dummy(ImVec2(0, 5));

			//-

			int pad = _w * 0.1;
			float w50 = ImGui::GetWindowWidth()*0.5f - pad * 0.5f;
			float w100 = ImGui::GetWindowWidth() - pad;
			ImGui::PushItemWidth(w100);

			// squared box

			_flags =
				ImGuiColorEditFlags_NoSmallPreview |
				ImGuiColorEditFlags_NoTooltip |
				ImGuiColorEditFlags_NoLabel |
				ImGuiColorEditFlags_NoSidePreview |
				ImGuiColorEditFlags_NoOptions |
				ImGuiColorEditFlags_NoAlpha |
				ImGuiColorEditFlags_HSV |
				ImGuiColorEditFlags_RGB |
				ImGuiColorEditFlags_PickerHueBar;

			ImGui::ColorPicker4("Background Color", (float *)&color, _flags);
			color_BackGround = color;

			ImGui::PopItemWidth();

			ImGui::Dummy(ImVec2(0, 5));

			//-

			if (ImGui::CollapsingHeader("Advanced"))
			{
				ImGui::PushItemWidth(_w50);

				ofxImGui::AddParameter(color_backGround_SET);
				ofxImGui::AddParameter(color_BackGround_Lock);
				ofxImGui::AddParameter(color_BackGround_Darker);
				if (color_BackGround_Darker)
				{
					ofxImGui::AddParameter(color_BackGround_Darkness);
				}
				ofxImGui::AddParameter(AutoSet_BackGround_Color);
				ofxImGui::AddParameter(curve_Gradient_PickIn);

				ImGui::PopItemWidth();

				ImGui::Checkbox("Auto-resize", &auto_resize);
			}
		}
	}
	ofxImGui::EndWindow(mainSettings);
}

//--------------------------------------------------------------
void ofxColorManager::gui_Presets()
{
	static bool auto_resize = true;
	float ww, hh;
	ww = PANEL_WIDGETS_WIDTH;
	hh = PANEL_WIDGETS_HEIGHT;
	ImGui::SetWindowSize(ImVec2(ww, hh));
	ImGuiWindowFlags flags;
	flags = false;
	//flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : 0;
	//TODO: not working..non stop grow

	//--

	if (ofxImGui::BeginWindow("PRESETS", mainSettings, flags))
	{
		//ImGui::Checkbox("Auto-resize", &auto_resize);

		//--

		float _spc = ImGui::GetStyle().ItemInnerSpacing.x;
		float _w = ImGui::GetWindowContentRegionWidth() - 2 * _spc;
		//float _w = PANEL_WIDGETS_WIDTH - 2 * _spc;
		float _w50 = _w * 0.5;
		float _h = BUTTON_BIG_HEIGHT;

		ImGuiColorEditFlags _flags;

		//--

		// workflow

		//blink when a new preset is editing
		float freq = 0.075;//speed freq
		float min = 0.25;
		float max = 0.75;
		float a = ofMap(glm::sin(freq * ofGetFrameNum()), -1, 1, min, max);

		//-

		//ImGui::Text("Name");
		ImGui::Dummy(ImVec2(0, 5));

		ImGui::Text(PRESET_Name.c_str());

		//----

		ImGui::Dummy(ImVec2(0, 5));

		int counter = preset_Index.get();
		//static int counter = preset_Index;

		//----

		// index preset / total
		int numPalettes = files_Names.size() - 1;
		ImGui::Text("%d/%d", counter, numPalettes);
		//ImGui::Text("%d/%d", preset_Index.get(), numPalettes);

		ImGui::Dummy(ImVec2(0, 5));

		ImGui::PushButtonRepeat(true);

		if (ImGui::Button("Previous", ImVec2(_w50, _h)))
		{
			if (files.size() > 0)
			{
				if (counter > 0)
				{
					counter--;
					preset_Index = counter;

				}
				else
				{
					if (counter == 0)
					{
						counter = files.size() - 1;
						preset_Index = counter;
					}
				}
				counter = ofClamp(counter, 0, preset_Index.getMax());

				if (preset_Index < files.size() && preset_Index>0)
				{
					PRESET_Name = files_Names[preset_Index];
					ofLogNotice(__FUNCTION__) << "PRESET: [" + ofToString(preset_Index) + "] " << PRESET_Name;

					preset_Load(PRESET_Name);

					//-

					if (bNewPreset) bNewPreset = false;

					//workflow
					if (DEMO1_Test) myDEMO1.reStart();
				}
			}
		}

		//--

		ImGui::SameLine();

		if (ImGui::Button("Next", ImVec2(_w50, _h)))
		{
			if (files.size() > 0)
			{
				if (counter < files.size() - 1)
				{
					counter++;
					preset_Index = counter;
				}
				else {//cycle
					counter = 0;
					preset_Index = counter;
				}
				counter = ofClamp(counter, 0, preset_Index.getMax());

				if (preset_Index < files.size() && preset_Index>0)
				{
					PRESET_Name = files_Names[preset_Index];
					ofLogNotice(__FUNCTION__) << "PRESET: [" + ofToString(preset_Index) + "] " << PRESET_Name;

					preset_Load(PRESET_Name);

					//-

					if (bNewPreset) bNewPreset = false;

					//workflow
					if (DEMO1_Test) myDEMO1.reStart();
				}
			}
		}

		ImGui::PopButtonRepeat();

		ImGui::Dummy(ImVec2(0, 5));

		//----

		// scrollable list

		if (!files_Names.empty())
		{
			int currentFileIndex = preset_Index;

			//ImGui::PushItemWidth(-1);
			ImGui::PushItemWidth(_w);

			if (ofxImGui::VectorCombo(" ", &currentFileIndex, files_Names))
			{
				ofLogNotice(__FUNCTION__) << "currentFileIndex: " << ofToString(currentFileIndex);

				if (currentFileIndex < files_Names.size())
				{
					preset_Index = currentFileIndex;
					PRESET_Name = files_Names[preset_Index];
					ofLogNotice(__FUNCTION__) << "PRESET_Name: " << PRESET_Name;

					preset_Load(PRESET_Name);
				}

				if (bNewPreset) bNewPreset = false;
			}

			ImGui::PopItemWidth();
		}

		ImGui::Dummy(ImVec2(0, 10));

		//----


		// 2. presets

		if (ImGui::Button("SAVE", ImVec2(_w50, _h * 0.5)))
		{
			ofLogNotice(__FUNCTION__) << "SAVE";

			//TODO
			//should re load by same name and get what position on vector
			//is to reload current preset number
			//textInput_temp = ofToString(tab2);
			//ofLogNotice(__FUNCTION__) << "textInput_temp:" << textInput_temp;

			PRESET_Name = textInput_temp;

			ofLogNotice(__FUNCTION__) << "PRdrawESET_name: " << PRESET_Name;

			preset_Save(PRESET_Name);
			preset_RefreshFiles();
		}

		ImGui::SameLine();

		if (ImGui::Button("LOAD", ImVec2(_w50, _h * 0.5)))//not required..
		{
			ofLogNotice(__FUNCTION__) << "LOAD";
			ofLogNotice(__FUNCTION__) << "PRESET_Name: " << PRESET_Name;
			preset_Load(PRESET_Name);
		}

		//ImGui::SameLine();

		if (ImGui::Button("DELETE", ImVec2(_w50, _h * 0.5)))
			ImGui::OpenPopup("DELETE?");
		if (ImGui::BeginPopupModal("DELETE?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Current Preset will be deleted.\nThis operation cannot be undone!\n\n");
			ImGui::Separator();

			static bool dont_ask_me_next_time = false;
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
			ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
			ImGui::PopStyleVar();

			if (!dont_ask_me_next_time) {
				if (ImGui::Button("OK", ImVec2(120, 0))) {
					ofLogNotice(__FUNCTION__) << "DELETE";
					files[preset_Index].remove();
					preset_RefreshFiles();

					ImGui::CloseCurrentPopup();
				}
				ImGui::SetItemDefaultFocus();
				ImGui::SameLine();
				if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			}
			else {
				ofLogNotice(__FUNCTION__) << "DELETE";
				files[preset_Index].remove();
				preset_RefreshFiles();

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		//if (ImGui::Button("DELETE", ImVec2(_w50, _h * 0.5)))//current preset
		//{
		//	ofLogNotice(__FUNCTION__) << "DELETE";
		//	files[preset_Index].remove();
		//	preset_RefreshFiles();
		//}

		ImGui::SameLine();

		//----

		//export user palette colors to live reload from another parallel app!

		if (ImGui::Button("EXPORT", ImVec2(_w50, _h * 0.5))) ImGui::OpenPopup("EXPORT ");
		if (ImGui::BeginPopupModal("EXPORT ", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			//ImGui::Text("Pick export path..\n\n");
			//ImGui::Separator();

			static bool dont_ask_me_next_time = false;
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
			ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
			ImGui::PopStyleVar();

			if (!dont_ask_me_next_time) {
				if (ImGui::Button("OK", ImVec2(120, 0))) {
					ofLogNotice(__FUNCTION__) << "EXPORT";
					//path_Folder_ExportColor_Custom = ;
					exportPalette();

					ImGui::CloseCurrentPopup();
				}

				if (ImGui::Button("SET PATH", ImVec2(120, 0))) {
					ofLogNotice(__FUNCTION__) << "SET EXPORT PATH";
					ImGui::Text("Pick export path..\n\n");

					// TODO:
					//path_Folder_ExportColor_Custom = ;
					bOpen = true;

					ImGui::CloseCurrentPopup();
				}

				ImGui::SetItemDefaultFocus();
				ImGui::SameLine();
				if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			}
			else {
				exportPalette();
			}

			ImGui::EndPopup();
		}

		if (ImGui::Button("UPDATE", ImVec2(_w50, _h * 0.4)))
		{
			ofLogNotice(__FUNCTION__) << "UPDATE";

			//PRESET_Name = textInput_temp;

			ofLogNotice(__FUNCTION__) << "PRESET_Name: " << PRESET_Name;

			//delete old file
			//files[preset_Index].remove();

			//save new one
			preset_Save(PRESET_Name);
			preset_RefreshFiles();
		}

		ImGui::SameLine();

		if (ImGui::Button("COPY", ImVec2(_w50, _h * 0.4)))
		{
			ofLogNotice(__FUNCTION__) << "COPY";

			textInput_New = textInput_New + "_copy";

			preset_Save(textInput_New);
			preset_RefreshFiles();

			//--

			refresh_RearrengeFiles(textInput_New);
		}

		if (ImGui::Button("REFRESH KIT", ImVec2(_w50, _h * 0.4)))
		{
			ofLogNotice(__FUNCTION__) << "REFRESH KIT";

			preset_RefreshFiles();
		}

		ImGui::Dummy(ImVec2(0, 5));

		ofxImGui::AddParameter(bNewPreset);

		//ImGui::SameLine();
		//if (ImGui::Button("REFRESH"))//current preset
		//{
		//    ofLogNotice(__FUNCTION__) << "REFRESH";
		//    preset_RefreshFiles();
		//}

		//----

		if (MODE_NewPreset && bNewPreset)
		{
			ImGui::Separator();
			ImGui::Dummy(ImVec2(0, 10));

			//-

			ImGui::PushID(1);
			ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::ImColor(1.0f, 1.0f, 1.0f, 1 - a));
			ImGui::Text("NEW PRESET");
			ImGui::PopStyleColor(1);
			ImGui::PopID();

			//-

			ImGui::Dummy(ImVec2(0, 5));

			ImGui::PushItemWidth(_w);
			//ImGui::PushItemWidth(-1);

			// loaded string into char array
			char tab[32];
			strncpy(tab, textInput_New.c_str(), sizeof(tab));
			tab[sizeof(tab) - 1] = 0;

			//-

			if (ImGui::InputText("", tab, IM_ARRAYSIZE(tab)))
			{
				textInput_New = ofToString(tab);
				ofLogNotice(__FUNCTION__) << "textInput_New:" << textInput_New;
			}

			//TODO:
			bool b = bTextInputActive;
			bTextInputActive = ImGui::IsItemActive();
			if (bTextInputActive != b) ofLogNotice(__FUNCTION__) << "TextInput : " << (bTextInputActive ? "ACTIVE" : "DISABLED");

			//-

			////TODO: ??
			//has_focus = 0;
			//if (focus_1) ImGui::SetKeyboardFocusHere();
			//ImGui::InputText("", tab, IM_ARRAYSIZE(tab));
			//if (ImGui::IsItemActive())
			//{
			//	has_focus = 1;
			//	textInput_New = ofToString(tab);
			//	ofLogNotice(__FUNCTION__) << "textInput_New:" << textInput_New;
			//}

			//-

			ImGui::PopItemWidth();

			//--

			ImGui::PushID(1);
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.5, 0.0f, 0.5f, a));

			//ImGui::Dummy(ImVec2(0, 5));

			if (ImGui::Button("SAVE NEW", ImVec2(_w, _h)))
			{
				has_focus = 0;
				bNewPreset = false;

				ofLogNotice(__FUNCTION__) << "textInput_New: " << textInput_New;

				preset_Save(textInput_New);
				preset_RefreshFiles();

				//--

				refresh_RearrengeFiles(textInput_New);

				//--

				// DEMO
				if (SHOW_Demos) myDEMO1.reStart();
			}

			ImGui::PopStyleColor(1);
			ImGui::PopID();

			//-

			ImGui::Dummy(ImVec2(0, 5));
			ImGui::Separator();
		}

		//----

		// palette colors mini preview

		ImGui::Dummy(ImVec2(0, 5));

		//ImGuiColorEditFlags _flagw;
		//_flagw = ImGuiTreeNodeFlags_None;
		//_flagw = ImGuiTreeNodeFlags_DefaultOpen;

		ofxSurfingHelpers::AddBigToggle(SHOW_UserPaletteEditor, _w, _h * 0.5);

		ofxSurfingHelpers::AddBigToggle(SHOW_Kit, _w, _h * 0.5);
		//ofxImGui::AddParameter(SHOW_Kit);
		if (SHOW_Kit)
			ofxImGui::AddParameter(AutoScroll);
		//ofxImGui::AddParameter(SHOW_PresetsPalette);

		ImGui::Dummy(ImVec2(0, 5));

		//--
	}
	ofxImGui::EndWindow(mainSettings);

	//----

	//add kit palettes browser
	if (SHOW_Kit)
	{
		ww = PANEL_WIDGETS_WIDTH;
		hh = PANEL_WIDGETS_HEIGHT;
		ImGui::SetWindowSize(ImVec2(ww, hh));
		//ImGuiWindowFlags flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : 0;

		//--

		if (ofxImGui::BeginWindow("KIT", mainSettings, flags))
		{
			// populate widgets
			int indexPreset = gui_GridPalettes(kit, preset_Index, AutoScroll.get());

			// workflow
			if (indexPreset != -1)
			{
				preset_Index = indexPreset;
				preset_Load(files_Names[preset_Index]);

				//-

				// DEMO
				if (DEMO1_Test) myDEMO1.reStart();
			}
		}
		ofxImGui::EndWindow(mainSettings);
	}

	//----

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
}

//--------------------------------------------------------------
void ofxColorManager::gui_Demo()
{
	static bool auto_resize = true;
	float ww, hh;
	ww = PANEL_WIDGETS_WIDTH;
	hh = PANEL_WIDGETS_HEIGHT;
	ImGui::SetWindowSize(ImVec2(ww, hh));
	ImGuiWindowFlags flagsw;
	flagsw = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : 0;

	//-

	if (ofxImGui::BeginWindow("DEMO", mainSettings, flagsw))
	{
		ofxImGui::AddParameter(DEMO1_Test);
		ofxImGui::AddParameter(DEMO2_Test);
		if (DEMO1_Test) {
			ofxImGui::AddParameter(DEMO_Auto);
			ofxImGui::AddParameter(DEMO_Timer);
			ofxImGui::AddParameter(DEMO_Alpha);
			if (ofxImGui::AddParameter(DEMO_Cam))
			{
				myDEMO1.setEnableMouseCamera(DEMO_Cam);
			}
		}
	}
	ofxImGui::EndWindow(mainSettings);
}

//--------------------------------------------------------------
bool ofxColorManager::draw_Gui()
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

	if (SHOW_ALL_GUI)
	{
		if (SHOW_UserPalette) gui_Palette();
		if (SHOW_UserPaletteEditor) gui_PaletteEditor();
		if (SHOW_Presets) gui_Presets();
		if (SHOW_Picker) gui_Picker();
		if (SHOW_Library) gui_Library();
		if (SHOW_BackGround) gui_Background();
		if (SHOW_GradientCurve) draw_Curve();
		if (SHOW_Theory) gui_Theory();
		if (SHOW_Range) gui_Range();
#ifdef USE_COLOR_LOVERS
		if (SHOW_ColourLovers) colourLoversHelper.draw();
#endif
		if (SHOW_Quantizer) colorQuantizer.gui_Quantizer();
		if (SHOW_Panels) gui_Panels();
		if (SHOW_Export) gui_Export();
		if (SHOW_Demos) gui_Demo();
		//if (SHOW_CosineGradient) gui_imGui_CosineGradient();
	}

	//--

	gui.end();

	//--

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

	//-

	// 2 different modes:
	// mouse over any panel or over text input only
	if (bCheckMouseOverTextInput) return bTextInputActive;
	else return mainSettings.mouseOverGui;
}

#pragma mark - CURVE TOOL

//--------------------------------------------------------------
void ofxColorManager::setup_CurveTool()
{
	//gradient here too
	image_GradientCurve.allocate(image_curvedGradient_w, image_curvedGradient_h, OF_IMAGE_COLOR);

	curvesTool.setup(MAX_LUT_SIZE);

	pos_CurveEditor.set("POS CURVE", glm::vec2(500), glm::vec2(0), glm::vec2(1920, 180));

	curve_Ctrl_In.set("Pick", 0, 0, 1);
	curve_Ctrl_Out.set("Out", 0, 0, 1);
	bResetCurve.set("Reset Curve", false);
	SHOW_GradientCurve.set("GRADIENT", false);
	curve_Gradient_TEST_Prc.set("GRADIENT TEST", 0.5, 0, 1);

	params_curve.setName("GRADIENT CURVE");
	params_curve.add(curve_Ctrl_In);
	params_curve.add(curve_Ctrl_Out);
	params_curve.add(bResetCurve);
	params_curve.add(SHOW_GradientCurve);
	params_curve.add(MODE_EditGradientLayout);
	params_curve.add(SHOW_Editor);
	params_curve.add(pos_CurveEditor);

	//-

	// curve mod
	curve_Slider_Pick.setup(slider_PickIn_x, slider_PickIn_y, slider_PickIn_w, slider_PickIn_h, 0, 1, curve_Gradient_PickIn.get(), true, true);
	curve_Slider_Pick.setPercent(curve_Gradient_PickIn);
	curve_Slider_Pick.setVisible(SHOW_GradientCurve);
	curve_Slider_Pick.setLabelString("Pick");

	// slider live test color out for this input
	curve_Slider_ExpTweak.setup(slider_Exp_x + (slider_Exp_w + pad), slider_Exp_y, 2 * slider_Exp_w, slider_Exp_h, 0, 1, curve_Gradient_Exp, true, true);
	curve_Slider_ExpTweak.setPercent(curve_Gradient_Exp);
	curve_Slider_ExpTweak.setVisible(SHOW_GradientCurve);
	curve_Slider_ExpTweak.setLabelString("Exp");
}

//--------------------------------------------------------------
void ofxColorManager::update_CurveTool()
{
	//----

	if (curve_Gradient_PickIn.get() != curve_Slider_Pick.getValue())
	{
		curve_Gradient_PickIn = curve_Slider_Pick.getValue();
	}

	// update values
	if (curve_Ctrl_In.get() != curve_Slider_Pick.getValue())
	{
		curve_Ctrl_In.setWithoutEventNotifications(curve_Slider_Pick.getValue());
		curve_Index = (int)ofMap(curve_Ctrl_In.get(), 0, 1, 0, MAX_LUT_SIZE - 1);
	}

	if (curve_Slider_ExpTweak.getValue() != curve_Gradient_Exp.get())
	{
		curve_Gradient_Exp = curve_Slider_ExpTweak.getValue();
	}

	// curve modifier
	curve_Gradient_TEST_Prc = curve_Slider_ExpTweak.getValue();
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

	curvesTool.set(pointToModify, ofVec2f(pointY, ofMap(curve_Gradient_TEST_Prc, 0., 1., 0, MAX_LUT_SIZE)));

	//----

	// vertical palette colors gradient rectangle modified by curveTool

	// every y point has different color
	for (int y = 0; y < image_curvedGradient_h; y++)
	{
		float input_LUT = ofMap(y, 0, image_curvedGradient_h, 0, MAX_LUT_SIZE - 1);
		float output_LUT = curvesTool[input_LUT];
		float output;
		output = ofMap(output_LUT, 0, MAX_LUT_SIZE - 1, 1., 0.);
		ofColor c = gradient.getColorAtPercent(output);

		// all x point has the same color
		for (int x = 0; x < image_curvedGradient_w; x++)
		{
			image_GradientCurve.setColor(x, y, c);
		}
	}
	image_GradientCurve.update();

	//---

	//TODO:
	//// update TARGET color pointer (ofApp)
	//if (color_TARGET != nullptr)
	//{
	//	float out = ofMap(curvesTool.getAtPercent(1.0 - curve_Ctrl_In), 0, MAX_LUT_SIZE - 1, 1., 0.);
	//	ofColor c = gradient.getColorAtPercent(out);
	//	color_TARGET->set(c);
	//}
}

//--------------------------------------------------------------
void ofxColorManager::update_Gradient()
{
	// test curve
	{
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
				if (AutoSet_Background_FromGradient) color_BackGround.set(getColorAtPercent(control));

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
				if (AutoSet_Background_FromGradient) color_BackGround.set(getColorAtPercent(control));

				setControl(control);
			}
		}

		update_CurveTool();

		//--

		update_Curve();
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
	if (SHOW_GradientCurve)
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
		float y = curveTool_y + MAX_LUT_SIZE - curvesTool[curve_Index];
		ofDrawLine(curveTool_x, y, curveTool_x + curveTool_w, y);

		//// 1.3 current pos circle point
		//ofSetColor(255, 32);
		//ofDrawCircle(curveTool_x + curve_Index, curveTool_y + y, 3);

		//-

		// NOTE: 
		//sometimes we need some tricky hacking to avoid rare fliping from gradients
		curve_Ctrl_Out.setWithoutEventNotifications(ofMap(curvesTool.getAtPercent(curve_Ctrl_In), 0.f, MAX_LUT_SIZE - 1, 0.f, 1.f));
		//curve_Ctrl_Out = ofMap(curvesTool.getAtPercent(curve_Ctrl_In), 0, MAX_LUT_SIZE - 1, 0., 1.);

		//-

		// 3. big gradient colors

		// image box gradient LUT
		ofSetColor(255, 255);

		image_GradientCurve.draw(
			image_curvedGradient_x, image_curvedGradient_y,
			image_curvedGradient_w, image_curvedGradient_h);

		//-

		// 4. border
		ofNoFill();
		ofSetLineWidth(2);
		ofSetColor(255, 225);

		ofRectangle r = ofRectangle(
			image_curvedGradient_x, image_curvedGradient_y,
			image_curvedGradient_w, slider_Exp_h);

		ofDrawRectangle(r);

		//-

		ofPopMatrix();
		ofPopStyle();
	}
}


//--------------------------------------------------------------
void ofxColorManager::palette_FromTheory(int p)
{
	ofLogNotice(__FUNCTION__) << p;

	palette_clear();
	ofColor color;

	// 1. FROM OFX-COLOR-PALETTE

	switch (p)
	{
	case 0:
		for (int i = 0; i < triad.size(); i++)
		{
			color = triad[i];
			palette_addColor(color);
		}
		break;
	case 1:
		for (int i = 0; i < complementTriad.size(); i++)
		{
			color = complementTriad[i];
			palette_addColor(color);
		}
		break;
	case 2:
		for (int i = 0; i < complement.size(); i++)
		{
			color = complement[i];
			palette_addColor(color);
		}
		break;
	case 3:
		for (int i = 0; i < complementBrightness.size(); i++)
		{
			color = complementBrightness[i];
			palette_addColor(color);
		}
		break;
	case 4:
		for (int i = 0; i < monochrome.size(); i++)
		{
			color = monochrome[i];
			palette_addColor(color);
		}
		break;
	case 5:
		for (int i = 0; i < monochromeBrightness.size(); i++)
		{
			color = monochromeBrightness[i];
			palette_addColor(color);
		}
		break;
	case 6:
		for (int i = 0; i < analogue.size(); i++)
		{
			color = analogue[i];
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
	case 7:
		for (int i = 0; i < colors_Analogous.size(); i++)
		{
			color = colors_Analogous[i];
			palette_addColor(color);
		}
		break;
	case 8:
		for (int i = 0; i < colors_Complementary.size(); i++)
		{
			color = colors_Complementary[i];
			palette_addColor(color);
		}
		break;
	case 9:
		for (int i = 0; i < colors_SplitComplementary.size(); i++)
		{
			color = colors_SplitComplementary[i];
			palette_addColor(color);
		}
		break;
	case 10:
		for (int i = 0; i < colors_Compound.size(); i++)
		{
			color = colors_Compound[i];
			palette_addColor(color);
		}
		break;
	case 11:
		for (int i = 0; i < colors_FlippedCompound.size(); i++)
		{
			color = colors_FlippedCompound[i];
			palette_addColor(color);
		}
		break;
	case 12:
		for (int i = 0; i < colors_Monochrome.size(); i++)
		{
			color = colors_Monochrome[i];
			palette_addColor(color);
		}
		break;
	case 13:
		for (int i = 0; i < colors_Triad.size(); i++)
		{
			color = colors_Triad[i];
			palette_addColor(color);
		}
		break;
	case 14:
		for (int i = 0; i < colors_Tetrad.size(); i++)
		{
			color = colors_Tetrad[i];
			palette_addColor(color);
		}
		break;
	}

	//--

	// workflow

	//set background color from first/last palette color
	if (AutoSet_BackGround_Color && !color_BackGround_Lock)
	{
		if (palette.size() > 0)
		{
			ofColor c;
			c = palette[0];//get first color from user palette

			if (color_BackGround_Darker)
			{
				float darkness = ofMap(color_BackGround_Darkness, 0.0, 1.0, 0.0, 2.0);
				float b = c.getBrightness() * darkness;
				b = ofClamp(b, 0.0, 1.0);
				c.setBrightness(b);
			}
			color_BackGround.set(c);

			//-

			//TODO: must improve
			// modulate darker
			//if (color_BackGround_Darker)
			//{
			//    float darkness;
			//    //darkness = ofMap(color_BackGround_Darkness, 0.0, 1.0, 0.0, 1.5);
			//    float b;
			//    if (color_BackGround_Darkness < 0.5)
			//    {
			//        b = c.getBrightness() / color_BackGround_Darkness * 10;
			//    }
			//    else if (color_BackGround_Darkness >= 0.5)
			//    {
			//        b = c.getBrightness() * color_BackGround_Darkness * 20;
			//    }
			//
			//    b = ofClamp(b, 0.0, 255.0);
			//    c.setBrightness(b);
			//}
		}
	}

	//--

	refresh_Palette_TARGET(palette);
}

//--------------------------------------------------------------
void ofxColorManager::refresh_TheoryEngine()
{
	// 1. FROM OFX-COLOR-PALETTE

	// update palettes
	complement.setBaseColor(color_TheoryBase);
	complementBrightness.setBaseColor(color_TheoryBase);
	triad.setBaseColor(color_TheoryBase);
	complementTriad.setBaseColor(color_TheoryBase);
	monochrome.setBaseColor(color_TheoryBase);
	monochromeBrightness.setBaseColor(color_TheoryBase);
	analogue.setBaseColor(color_TheoryBase);
	//random.setBaseColor(base);

	complement.generateComplementary(ofxColorPalette::SATURATION, numColors_Alg);
	complementBrightness.generateComplementary(ofxColorPalette::BRIGHTNESS, numColors_Alg);
	triad.generateTriad();
	complementTriad.generateComplementaryTriad();
	//complementTriad.generateComplementaryTriad(analogSpread);
	monochrome.generateMonoChromatic(ofxColorPalette::SATURATION, numColors_Alg);
	monochromeBrightness.generateMonoChromatic(ofxColorPalette::BRIGHTNESS, numColors_Alg);
	analogue.generateAnalogous(numColors_Alg, analogSpread);
	//analogue.generateAnalogous(numColors_Alg, 0.2);

	//---

	// 2. FROM OFX-COLOUR-THEORY

	update_Theory();//TODO: reduce calls...
}

//--------------------------------------------------------------
void ofxColorManager::palettes_resize()
{
	//TODO: not required
	numColors_Theory = numColors_Engines;
	numColors_Alg = numColors_Engines;

	refresh_Theory3();
	refresh_TheoryEngine();

	//--

	// workflow
	//TODO:
	if (bAuto_Build_Palette)
	{
		if (SHOW_Theory) palette_FromTheory(last_Index_Theory);
		else if (SHOW_Range) palette_FromRange(last_Index_Range);
	}
}

//--------------------------------------------------------------
void ofxColorManager::refresh_Theory3() //populates palettes
{
	ofLogNotice(__FUNCTION__);

	for (int i = 0; i < NUM_COLOR_THEORY_TYPES_G1; i++)
	{
		shared_ptr<ColorWheelScheme> _scheme;
		_scheme = ColorWheelSchemes::colorSchemes[i];
		ofLogNotice(__FUNCTION__) << i;
		_scheme->setPrimaryColor(color_TheoryBase.get());
		colorsTheory[i] = _scheme->interpolate(numColors_Theory.get());
	}
}

//--------------------------------------------------------------
void ofxColorManager::setup_Theory()
{
	numColors_Engines.set("Amnt Colors", 8, 2, MAX_PALETTE_COLORS);
	numColors_Engines.setSerializable(true);

	//--

	//numColors_Alg.makeReferenceTo(numColors_Theory);
	//numColors_Theory.makeReferenceTo(numColors_Alg);

	params_ColorTheory.clear();
	params_ColorTheory.setName("Color Theory");
	params_ColorTheory.add(color_TheoryBase.set("Base", ofColor::magenta, ofColor(0), ofColor(255)));
	params_ColorTheory.add(bAuto_TheoryFromPicker.set("Auto Get Picker", true));
	params_ColorTheory.add(colorScheme.set("Color Scheme", 6, 0, ColorWheelSchemes::colorSchemes.size() - 1));
	params_ColorTheory.add(colorSchemeName);
	params_ColorTheory.add(numColors_Theory.set("Amnt Colors Thy ", 8, 2, MAX_PALETTE_COLORS));
	params_ColorTheory.add(last_Theory_PickPalette.set("Last Theory Picked", 0, 0, NUM_COLOR_THEORY_TYPES_G1 - 1));

	//toggles
	for (int i = 0; i < NUM_COLOR_THEORY_TYPES_G1; i++)
	{
		theoryTypes[i].set(ColorWheelSchemes::colorSchemeNames[i], false);
		params_ColorTheory.add(theoryTypes[i]);
	}

	ofAddListener(params_ColorTheory.parameterChangedE(), this, &ofxColorManager::Changed_ColorTheory);

	refresh_Theory3();
}

//--------------------------------------------------------------
void ofxColorManager::update_Theory()
{
	scheme_Analogous = ColorWheelSchemes::colorSchemes[0];
	scheme_Analogous->setPrimaryColor(color_TheoryBase);
	colors_Analogous = scheme_Analogous->interpolate(numColors_Alg);

	scheme_Complementary = ColorWheelSchemes::colorSchemes[1];
	scheme_Complementary->setPrimaryColor(color_TheoryBase);
	colors_Complementary = scheme_Complementary->interpolate(numColors_Alg);

	scheme_SplitComplementary = ColorWheelSchemes::colorSchemes[2];
	scheme_SplitComplementary->setPrimaryColor(color_TheoryBase);
	colors_SplitComplementary = scheme_SplitComplementary->interpolate(numColors_Alg);

	scheme_Compound = ColorWheelSchemes::colorSchemes[3];
	scheme_Compound->setPrimaryColor(color_TheoryBase);
	colors_Compound = scheme_Compound->interpolate(numColors_Alg);

	scheme_FlippedCompound = ColorWheelSchemes::colorSchemes[4];
	scheme_FlippedCompound->setPrimaryColor(color_TheoryBase);
	colors_FlippedCompound = scheme_FlippedCompound->interpolate(numColors_Alg);

	scheme_Monochrome = ColorWheelSchemes::colorSchemes[5];
	scheme_Monochrome->setPrimaryColor(color_TheoryBase);
	colors_Monochrome = scheme_Monochrome->interpolate(numColors_Alg);

	scheme_Tetrad = ColorWheelSchemes::colorSchemes[6];
	scheme_Tetrad->setPrimaryColor(color_TheoryBase);
	colors_Tetrad = scheme_Tetrad->interpolate(numColors_Alg);

	scheme_Triad = ColorWheelSchemes::colorSchemes[7];
	scheme_Triad->setPrimaryColor(color_TheoryBase);
	colors_Triad = scheme_Triad->interpolate(numColors_Alg);

	//    NOTE: RANDOM = 0, ANALOGOUS = 1, COMPLEMENTARY = 2,
	//    SPLIT_COMPLEMENTARY = 3, COMPOUND = 4, FLIPPED_COMPOUND = 5,
	//    MONOCHROME = 6, TRIAD = 7, TETRAD = 8
}

#pragma mark - PALETTE

//--------------------------------------------------------------
void ofxColorManager::palette_FromColourLovers() // ?
{
	ofLogNotice(__FUNCTION__);

	// 1. erase user palette and fill a new one with just update/received colour lovers
	palette_clear();
	ofColor c;
	for (int i = 0; i < myPalette.size(); i++)
	{
		c = myPalette[i];
		ofLogNotice(__FUNCTION__) << "_c [" << i << "] " << ofToString(c);
		palette_addColor(c);
	}

	//TODO
	//for preset manager
	// 2. get color from colour lovers
	if (palette.size() > 0)
	{
		ofColor c = palette[0];//get first color
		color_Clicked2 = ofColor(c);
	}

	//-

	// workflow: 
	//set background color from first/last palette color
	if (AutoSet_BackGround_Color && !color_BackGround_Lock)
	{
		if (palette.size() > 0)
		{
			ofColor c;
			c = palette[0];//get first color from user palette

			//TODO: must improve
			// modulate darker
			//if (color_BackGround_Darker)
			//{
			//    float darkness;
			//    //darkness = ofMap(color_BackGround_Darkness, 0.0, 1.0, 0.0, 1.5);
			//    float b;
			//    if (color_BackGround_Darkness < 0.5)
			//    {
			//        b = c.getBrightness() / color_BackGround_Darkness * 10;
			//    }
			//    else if (color_BackGround_Darkness >= 0.5)
			//    {
			//        b = c.getBrightness() * color_BackGround_Darkness * 20;
			//    }
			//
			//    b = ofClamp(b, 0.0, 255.0);
			//    c.setBrightness(b);
			//}
			if (color_BackGround_Darker)
			{
				float darkness = ofMap(color_BackGround_Darkness, 0.0, 1.0, 0.0, 2.0);
				float b = c.getBrightness() * darkness;
				//b = ofClamp(b, 0.0, 1.0);
				c.setBrightness(b);
			}
			color_BackGround.set(c);
		}
	}

	//--

	refresh_Palette_TARGET(palette);
}

//--------------------------------------------------------------
void ofxColorManager::refresh_Palette_TARGET(vector<ofColor> &p)
{
	////TODO:
	////modify a suscribed palette

	//// update TARGET color pointer (ofApp)
	//if (palette_TARGET != nullptr)
	//{
	//	//palette_TARGET = (vector<ofColor>)(p);//TODO. faster mode ?

	//	palette_TARGET->clear();
	//	palette_TARGET->resize(p.size());

	//	////TODO:
	//	//for (int c = 0; c < p.size(); c++)
	//	//{
	//	//	(palette_TARGET)[c]->set(p[c]);//?
	//	//}
	//}
}

////--------------------------------------------------------------
//void ofxColorManager::draw_MiniPreview()
//{
//	ofPushMatrix();
//	ofPushStyle();
//	{
//		int _x = 30;
//		int _y = 30;
//		glm::vec2 _pos;
//		int _sz = 60;//size boxes
//		int _p = 3;//pad between colors
//		int _sz2 = _sz + _p;
//		int _pad = 35;
//		int _pad2 = 35;
//		int _padlabel = 10;
//		int _hBg = 15;
//		float _round2 = 3;
//		float _round = 4;
//		ofRectangle _rBg;
//		ofColor colorBackground{ 0, 225 };//bg box
//		ofColor _cb = ofColor(ofColor::black, 64);//border color black
//		//ofColor _cb = ofColor(ofColor::white, 64);//border color white
//
//		//-
//
//		// 1. left top corner
//		_pos = glm::vec2(_x, _y);
//		//_pos = glm::vec2(_x, _y + _pad2);
//		//// 2. right top corner
//		//_pos = glm::vec2(ofGetWidth() - palette.size()*_sz2, 2*_p);
//
//		ofTranslate(_pos);
//
//		//-
//
//		//prepare bg box
//		ofRectangle _r{
//			0, 0,
//			float(palette.size() * _sz2 - _p), float(_pad2 + _sz2 + _hBg) };
//		_r.setHeight(_r.getHeight() + _pad);
//		_r.setX(_r.getPosition().x - _pad / 2.);
//		_r.setY(_r.getPosition().y - _pad / 2.);
//
//		//-
//
//		// preset name
//
//		//label + name
//		std::string s = PRESET_Name;
//		ofColor font0_Color{ 255, 255 };
//		float _ww2 = font.getStringBoundingBox(s, 0, 0).getWidth();
//
//		//draw bg box
//		_r.setWidth(MAX(_r.getWidth() + _pad, _ww2 + _pad));
//		ofSetColor(colorBackground);
//		ofFill();
//		ofDrawRectRounded(_r, _round);
//
//		//draw text
//		ofSetColor(font0_Color);
//		font.drawString(s, _padlabel, 0 + _pad * 0.5);
//
//		//-
//
//		ofTranslate(0, 35);
//
//		//3. palette colors
//		for (int col = 0; col < palette.size(); col++)
//		{
//			_r = ofRectangle(col * _sz2, 0, _sz, _sz);
//			ofFill();
//			ofSetColor(palette[col]);
//			ofDrawRectRounded(_r, _round2);
//			//ofDrawRectangle(_r);
//			ofNoFill();
//			ofSetColor(_cb);
//			ofDrawRectRounded(_r, _round2);
//			//ofDrawRectangle(_r);
//		}
//
//		ofTranslate(0, _sz + _p);
//
//		//1. background color box
//		_rBg = ofRectangle(0, 0, palette.size() * _sz2 - _p, _hBg);
//		ofFill();
//		ofSetColor(color_BackGround.get());
//		ofDrawRectRounded(_rBg, _round2);
//		//ofDrawRectangle(_rBg);
//		ofNoFill();
//		ofSetColor(_cb);
//		ofDrawRectRounded(_rBg, _round2);
//		//ofDrawRectangle(_rBg);
//	}
//	ofPopStyle();
//	ofPopMatrix();
//}

//--------------------------------------------------------------
void ofxColorManager::palette_addColor(ofColor c)
{
	ofLogNotice(__FUNCTION__) << "+ " << ofToString(palette.size()) << " : " << ofToString(c);

	palette.push_back(c);
	gradient.addColor(c);

	//--

	// workflow: 
	//select last one, just the one created now
	if (bEditPalette)
	{
		palette_colorSelected.setMax(palette.size() - 1);
		//palette_colorSelected = palette.size() - 1;
	}

	//--

	//refresh_Palette_TARGET(palette);
}

//--------------------------------------------------------------
void ofxColorManager::palette_removeColor(int c)
{
	// remove current color from user palette

	ofLogNotice(__FUNCTION__) << " " << c;
	ofLogNotice(__FUNCTION__) << "size pre: " << palette.size();

	if (c < palette.size())
	{
		// 0. erase last touched color th element
		//palette.erase(palette.begin() + c - 1);
		palette.erase(palette.begin() + c);

		palette_colorSelected.setMax(palette.size() - 1);

		// 1. debug after remove color from palette vector
		ofLogNotice(__FUNCTION__) << "Palette content: ";
		for (unsigned i = 0; i < palette.size(); ++i) ofLogNotice(__FUNCTION__) << ' ' << palette[i];
		ofLogNotice(__FUNCTION__) << '\n';
		ofLogNotice(__FUNCTION__) << "palette size post: " << palette.size();

		// 2. reset gradient
		gradient.reset();

		// 4. add edited palette colors vector to interface buttons and to gradient
		for (int i = 0; i < palette.size(); i++)
		{
			ofLogNotice(__FUNCTION__) << "add _c [" << i << "]";
			gradient.addColor(palette[i]);
		}

		// 5. make positions & resizes to fill bar
		//palette_rearrenge();

		//--

		// workflow: 
		if (bEditPalette)
		{
			palette_colorSelected.setMax(palette.size() - 1);
		}
	}
}

//--------------------------------------------------------------
void ofxColorManager::palette_removeColorLast()
{
	if (palette.size() > 0)
	{
		palette.pop_back();
		gradient.removeColorLast();
	}

	// 4. add edited palette colors vector to interface buttons and to gradient
	for (int i = 0; i < palette.size(); i++)
	{
		ofLogNotice(__FUNCTION__) << "add _c [" << i << "]";
		gradient.addColor(palette[i]);
	}

	//palette_rearrenge();

	//--

	// workflow: 
	if (bEditPalette)
	{
		palette_colorSelected.setMax(palette.size() - 1);
	}
}

//--------------------------------------------------------------
void ofxColorManager::palette_clear()
{
	ofLogNotice(__FUNCTION__) << "----------------- CLEAR PALETTE -----------------";

	// remove all colors from the user palette
	palette.clear();
	gradient.reset();

	//-

	if (DEMO1_Test) myDEMO1.clear();

	//--

	//TODO:
	refresh_Palette_TARGET(palette);
}

#pragma mark - CALLBACKS

//--------------------------------------------------------------
void ofxColorManager::Changed_ColorPicked(ofFloatColor &c)
{
	ofLogNotice(__FUNCTION__) << ofToString(c);// color_Picked

	// update TARGET color pointer (ofApp)
	if (color_TARGET != nullptr)
	{
		color_TARGET->set(c);
	}

	//--

	//workflow

	refresh_Picker_Touched();
}

//--------------------------------------------------------------
void ofxColorManager::Changed_ColorClicked(ofFloatColor &c)
{
	ofLogNotice(__FUNCTION__) << ofToString(c);// color_Clicked	

	// update TARGET color pointer (ofApp)
	if (color_TARGET != nullptr)
	{
		color_TARGET->set(c);
	}
}

//--------------------------------------------------------------
void ofxColorManager::Changed_ParamsPalette(ofAbstractParameter &e)
{
	std::string name = e.getName();

	ofLogNotice(__FUNCTION__) << name << " : " << e;

	if (false) {}

	else if (name == bFlipUserPalette.getName() && bFlipUserPalette)
	{
		bFlipUserPalette = false;

		std::reverse(palette.begin(), palette.end());

		// DEMO 2
		myDEMO2.setPaletteColors(palette);

		build_GradientPalette();
	}
}

//--------------------------------------------------------------
void ofxColorManager::Changed_ColorTheory(ofAbstractParameter &e)
{
	std::string name = e.getName();

	ofLogNotice(__FUNCTION__) << name << " : " << e;

	//--

	if (false) {}

	//----

	// num colors

	else if (name == numColors_Theory.getName())
	{
		numColors_Alg.setWithoutEventNotifications(numColors_Engines);
		numColors_Range.setWithoutEventNotifications(numColors_Engines);
		numColors_Engines.setWithoutEventNotifications(numColors_Engines);

		palettes_resize();
	}

	else if (name == colorScheme.getName() || name == color_TheoryBase.getName())
	{
		refresh_Theory3();
	}

	else if (name == last_Theory_PickPalette.getName())
	{
	}

	//----

	// toggles
	else
	{
		for (int i = 0; i < NUM_COLOR_THEORY_TYPES_G1; i++)
		{
			if (name == theoryTypes[i].getName() && theoryTypes[i].get())
			{
				theoryTypes[i] = false;//backoff button

				palette_clear();
				ofColor c;
				for (int n = 0; n < numColors_Theory; n++)
				{
					c = colorsTheory[i][n];
					ofLogNotice(__FUNCTION__) << "  " << n << " : " << ofToString(c);
					palette_addColor(c);
				}

				//-

				theory_Name = theoryTypes[i].getName();

				last_Index_Theory = i;
			}
		}
	}

	//--

	//TODO: !
	//move to theory callback!

	// algo/theory types

	for (int i = 0; i < NUM_COLOR_THEORY_TYPES_G2; i++)
	{
		if (name == algoTypes[i].getName() && algoTypes[i].get())
		{
			algoTypes[i] = false;//off button

			vector<ofColor> _cols;
			palette_clear();

			//-

			theory_Name = algoTypes[i].getName();
			last_Index_Theory = i + NUM_COLOR_THEORY_TYPES_G1;

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

//--------------------------------------------------------------
void ofxColorManager::Changed_Controls(ofAbstractParameter &e)
{
	std::string name = e.getName();

	ofLogNotice(__FUNCTION__) << name << " : " << e;

	//----

	//app modes

	if (name == AppMode.getName())
	{
		//clamp
		//AppMode = ofClamp(
		//	AppMode.get(),
		//	AppMode.getMin(),
		//	AppMode.getMax());

		//cycle
		if (AppMode > AppMode.getMax()) AppMode = AppMode.getMin();

		if (current_element != AppMode) current_element = AppMode.get();

		AppMode_name = ofToString(element_names[AppMode.get()]);

		switch (AppMode)
		{

		case 0:
			SHOW_Presets = true;
			if (SHOW_Presets)
			{
				//SHOW_UserPalette = false;
				SHOW_Picker = false;
				SHOW_Library = false;
				SHOW_BackGround = false;
				SHOW_Theory = false;
				SHOW_Range = false;
				SHOW_ColourLovers = false;
				SHOW_Quantizer = false;
				SHOW_GradientCurve = false;
			}
			break;

		case 1:
			SHOW_Theory = true;
			if (SHOW_Theory)
			{
				//SHOW_UserPalette = false;
				SHOW_Picker = false;
				SHOW_Library = false;
				SHOW_BackGround = false;
				//SHOW_Theory = false;
				SHOW_Range = false;
				SHOW_ColourLovers = false;
				SHOW_Quantizer = false;
				SHOW_GradientCurve = false;
			}
			break;

		case 2:
			SHOW_Range = true;
			if (SHOW_Range)
			{
				//SHOW_UserPalette = false;
				SHOW_Picker = false;
				SHOW_Library = false;
				SHOW_BackGround = false;
				SHOW_Theory = false;
				//SHOW_Range = false;
				SHOW_ColourLovers = false;
				SHOW_Quantizer = false;
				SHOW_GradientCurve = false;
			}
			break;

		case 3:
			SHOW_ColourLovers = true;
			if (SHOW_ColourLovers)
			{
				//SHOW_UserPalette = false;
				SHOW_Picker = false;
				SHOW_Library = false;
				SHOW_BackGround = false;
				SHOW_Theory = false;
				SHOW_Range = false;
				//SHOW_ColourLovers = false;
				SHOW_Quantizer = false;
				SHOW_GradientCurve = false;
			}
			break;

		case 4:
			SHOW_Quantizer = true;
			if (SHOW_Quantizer)
			{
				//SHOW_UserPalette = false;
				SHOW_Picker = false;
				SHOW_Library = false;
				SHOW_BackGround = false;
				SHOW_Theory = false;
				SHOW_Range = false;
				SHOW_ColourLovers = false;
				//SHOW_Quantizer = false;
				SHOW_GradientCurve = false;
			}
			break;

		case 5:
			SHOW_GradientCurve = true;
			if (SHOW_GradientCurve)
			{
				//SHOW_UserPalette = false;
				SHOW_Picker = false;
				SHOW_Library = false;
				SHOW_BackGround = false;
				SHOW_Theory = false;
				SHOW_Range = false;
				SHOW_ColourLovers = false;
				SHOW_Quantizer = false;
				//SHOW_GradientCurve = false;
			}
			break;
		}
	}

	//----

	else if (name == bNewPreset.getName())
	{
		MODE_NewPreset = bNewPreset;
		//wf
		focus_1 = bNewPreset;
	}

	//----

	// last index

	else if (name == last_Index_Theory.getName())
	{
		// TODO:
		reBuild();

		txt_lineActive[0] = false;//preset name
		txt_lineActive[1] = false;//lover name
		txt_lineActive[2] = true;//theory name
		txt_lineActive[3] = false;//range name
		last_Index_Type = 2;

		// DEMO
		if (DEMO1_Test) myDEMO1.reStart();

		// presets 

		if (SHOW_Presets) {
			textInput_New = theory_Name;
			bNewPreset = true;
		}
	}

	else if (name == last_Index_Range.getName())
	{
	}

	//----

	// num colors

	else if (name == numColors_Engines.getName())
	{
		numColors_Theory.setWithoutEventNotifications(numColors_Engines);
		numColors_Alg.setWithoutEventNotifications(numColors_Engines);
		//numColors_Range.setWithoutEventNotifications(numColors_Engines);

		//generate_Range(color_1_Range.get(), color_2_Range.get());
		////auto create palette
		//refresh_Range_AutoUpdate();

		//numColors_Alg = numColors_Engines;
		//numColors_Theory = numColors_Engines;
		numColors_Range = numColors_Engines;

		palettes_resize();
	}

	else if (name == numColors_Alg.getName())
	{
		numColors_Theory.setWithoutEventNotifications(numColors_Alg);
		numColors_Engines.setWithoutEventNotifications(numColors_Alg);

		palettes_resize();
	}

	else if (name == analogSpread.getName())
	{
		refresh_TheoryEngine();
	}

	//----

	// HSB

	if (ENABLE_Callbacks_cPickers)
	{
		if (name == color_HUE.getName()) // "H"
		{
			ofLogNotice(__FUNCTION__) << name << " : " << e;

			ofColor c;
			c.set(color_Picked.get());
			c.setHue(color_HUE);
			color_Picked.set(c);
		}
		else if (name == color_SAT.getName()) // "S"
		{
			ofLogNotice(__FUNCTION__) << name << " : " << e;

			ofColor c;
			c.set(color_Picked.get());
			c.setSaturation(color_SAT);
			color_Picked.set(c);
		}
		else if (name == color_BRG.getName()) // "B"
		{
			ofLogNotice(__FUNCTION__) << name << " : " << e;

			ofColor c;
			c.set(color_Picked.get());
			c.setBrightness(color_BRG);
			color_Picked.set(c);
		}
	}

	//----

	// panels

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
	else if (name == SHOW_Theory.getName())
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
	else if (name == SHOW_Range.getName())
	{
		if (SHOW_Range)
		{
			SHOW_Theory = false;
			SHOW_ColourLovers = false;
			SHOW_Quantizer = false;
		}
	}

	//quantizer
	else if (name == SHOW_Quantizer.getName())
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
	else if (name == SHOW_ALL_GUI.getName())
	{
		if (SHOW_GradientCurve)
		{
			curve_Slider_Pick.setVisible(SHOW_ALL_GUI);
			curve_Slider_ExpTweak.setVisible(SHOW_ALL_GUI);
		}
	}

	//----

	// gradient

	//curve position
	else if (name == pos_CurveEditor.getName())
	{
	}

	//curve pick
	else if (name == curve_Gradient_PickIn.getName())
	{
		curve_Slider_Pick.setPercent(curve_Gradient_PickIn.get());
	}

	//curve exp
	else if (name == curve_Gradient_Exp.getName())
	{
		curve_Slider_ExpTweak.setPercent(curve_Gradient_Exp);

		if (bAutoPaletteFromGradient)
		{
			palette_FromGradient();
		}
	}

	//edit curve layout
	else if (name == MODE_EditGradientLayout.getName())
	{
		ofLogNotice(__FUNCTION__) << name << (MODE_EditGradientLayout ? " TRUE" : " FALSE");

		if (MODE_EditGradientLayout.get())
		{
			rGradientPreview.enableEdit();
		}
		else
		{
			rGradientPreview.disableEdit();
		}

		refresh_Gui_Layout();
	}

	//-

	// panels

#ifdef USE_COLOR_LOVERS
	else if (name == "LOVERS")
	{
		colourLoversHelper.setVisible(SHOW_ColourLovers);

		////TODO
		//if (ENABLE_keys)
		//{
		colourLoversHelper.setEnableKeys(SHOW_ColourLovers);
		//}
	}
#endif

	//else if (name == SHOW_AlgoPalettes.getName())
	//{
	//	setVisible_Interface(SHOW_AlgoPalettes);
	//}

	else if (name == SHOW_BrowserColors.getName())
	{
		colorBrowser.setVisible(SHOW_BrowserColors);
	}

	//else if (name == "SHOW CURVE TOOL")
	//{
	//}

	else if (name == SHOW_GradientCurve.getName())
	{
		curve_Slider_ExpTweak.setVisible(SHOW_GradientCurve);
		curve_Slider_Pick.setVisible(SHOW_GradientCurve);
	}
	else if (name == SHOW_Quantizer.getName())
	{
		// workflow:
		//if (bEditPalette && SHOW_Quantizer) bEditPalette = false;

		// workflow: 

		{
#ifdef USE_IMAGE_QUANTIZER
			colorQuantizer.setActive(SHOW_Quantizer);
#endif
		}
	}

	//--

	else if (name == bRandomColor.getName())
	{
		if (bRandomColor)
		{
			bRandomColor = false;

			float _hue;
			float _sat;
			float _brg;

			if (bColor_HUE) {
				_hue = ofRandom(color_HUE_0 - 128.f * color_HUE_Power, color_HUE_0 + 128.f * color_HUE_Power);
				color_HUE = _hue;
			}
			if (bColor_SAT) {
				_sat = ofRandom(color_SAT_0 - 128.f * color_SAT_Power, color_SAT_0 + 128.f * color_SAT_Power);
				color_SAT = _sat;
			}
			if (bColor_BRG) {
				_brg = ofRandom(color_BRG_0 - 128.f * color_BRG_Power, color_BRG_0 + 128.f * color_BRG_Power);
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

	//----

	// edit

	else if (name == bAddColor.getName())
	{
		if (bAddColor)
		{
			bAddColor = false;

			palette_addColor(ofColor(color_Picked.get()));
		}
	}

	else if (name == bEditPalette.getName())
	{
		if (bEditPalette) {
			if (palette_colorSelected > palette.size() - 1)
				palette_colorSelected = palette.size() - 1;

			// autoload color picker to selected index color on palette
			color_Picked = palette[palette_colorSelected];
		}
	}

	else if (name == bRemoveColor.getName())
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

	else if (name == bClearPalette.getName())
	{
		if (bClearPalette)
		{
			bClearPalette = false;

			palette_clear();
		}
	}

	//----

	// curve

	else if (name == bResetCurve.getName())
	{
		if (bResetCurve)
		{
			bResetCurve = false;

			curvesTool.clear();
			curvesTool.add(ofVec2f(0, 0));
			curvesTool.add(ofVec2f(127, 127));
			curvesTool.add(ofVec2f(255, 255));

			curve_Gradient_TEST_Prc = 0.5;
			TEST_Mode = false;

			// pick in to out
			curve_Gradient_PickIn = 0.5;
			curve_Slider_Pick.setup(slider_Exp_x, slider_Exp_y, slider_Exp_w, slider_Exp_h, 0, 1, curve_Gradient_PickIn, true, true);
			curve_Slider_Pick.setPercent(curve_Gradient_PickIn);

			// exp
			curve_Gradient_Exp = 0.5;
			curve_Slider_ExpTweak.setup(slider_PickIn_x, slider_PickIn_y, slider_PickIn_w, slider_PickIn_h, 0, 1, curve_Gradient_Exp, true, true);
			curve_Slider_ExpTweak.setPercent(curve_Gradient_Exp);
		}
	}

	else if (name == gradient_HardMode.getName())
	{
		gradient.setHardMode(gradient_HardMode);
	}

	else if (name == AutoSet_Background_FromGradient.getName())
	{
		if (AutoSet_Background_FromGradient) AutoSet_BackGround_Color = true;
	}

	//----

	// background

	else if (name == color_backGround_SET.getName())
	{
		if (color_backGround_SET)
		{
			color_backGround_SET = false;
			color_BackGround.set(ofColor(color_Picked.get()));
		}
	}

	//	else if (name == color_BackGround_Darkness.getName())
	//	{
	//		//TODO: must improve
	//		//refresh_TheoryEngine();
	//
	//		// auto create user palette from algo palette
	//		if (bAuto_Build_Palette)
	//		{
	//			palette_clear();
	//
	//#ifdef USE_RECTANGLE_INTERFACES
	//			palette_FromTheory(SELECTED_palette_LAST);//trig last choice
	//#endif
	//		}
	//	}

		//---

		/*
		//else if (name == "AutoSet")
		//{

		//}
		//else if (name == "BG")
		//{
		//	//if (color_BackGround.get().getBrightness()!=backgroundDarkness_PRE)
		//	//color_BackGround_Darkness = color_BackGround.get().getBrightness();
		//	//
		//	////if (backgroundDarkness_PRE!=color_BackGround_Darkness)
		//	////color_BackGround_Darkness = (int)darkness;
		//}

		// algorithmic
		//    else if (name == "RANDOM PALETTE") {
		//        if (bRandomPalette) {
		//            bRandomPalette = false;
		//
		//            random.generateRandom(numColors_Alg);
		//            refresh_TheoryEngine();
		//            if (bAuto_Build_Palette) {
		//                //set random palette to user palette
		//                palette_FromTheory(7);
		//            }
		//        }
		//    }

		//else if (name == bAuto_Build_Palette.getName())
		//{
		//	//if (bAuto_Build_Palette) bLock_palette = false;
		//}

		//else if (name == bLock_palette.getName())
		//{
		//	if (bLock_palette) bAuto_Build_Palette = false;
		//}

		//--

	//	else if (name == BRIGHTNESS.getName() || name == SATURATION.getName())
	//	{
	//		refresh_TheoryEngine();
	//
	//		//-
	//
	//		// auto create user palette from theory palette
	//		if (bAuto_Build_Palette)
	//		{
	//			palette_clear();
	//
	//#ifdef USE_RECTANGLE_INTERFACES
	//			palette_FromTheory(SELECTED_palette_LAST);//trig last choice
	//#endif
	//			// DEMO
	//			if (DEMO1_Test) myDEMO1.reStart();
	//		}
	//	}
	*/
}

//load user palette from range
//--------------------------------------------------------------
void ofxColorManager::Changed_ColorRange(ofAbstractParameter &e)
{
	if (bRange_Intitiated)
	{
		std::string name = e.getName();
		ofLogNotice(__FUNCTION__) << name << " : " << e;

		if (false) {}

		//-

		// num colors

		else if (name == numColors_Range.getName())
		{
			numColors_Alg.setWithoutEventNotifications(numColors_Range);
			numColors_Engines.setWithoutEventNotifications(numColors_Range);
			numColors_Theory.setWithoutEventNotifications(numColors_Range);

			generate_Range(color_1_Range.get(), color_2_Range.get());

			//auto create palette
			refresh_Range_AutoUpdate();
		}

		//-

		else if (name == autoPick_RangeColor1.getName())
		{
			if (autoPick_RangeColor1) autoPick_RangeColor2 = false;
		}
		else if (name == autoPick_RangeColor2.getName())
		{
			if (autoPick_RangeColor2) autoPick_RangeColor1 = false;
		}

		//-

		else
		{
			// ranges label pickers

			for (int i = 0; i < NUM_TYPES_RANGES; i++)
			{
				if (name == rangTypes[i].getName() && rangTypes[i].get())
				{
					rangTypes[i] = false;//off button

					//-

					//index selected
					last_Index_Range = i;
					range_Name = rangTypes[i].getName();
					txt_lineActive[0] = false;//preset name
					txt_lineActive[1] = false;//lover name
					txt_lineActive[2] = false;//theory name
					txt_lineActive[3] = true;//range name
					last_Index_Type = 3;

					//-

					//TODO:
					bUpdated_Palette_BACK = true;

					////-

					//// populate palette
					//palette_FromRange(last_Index_Range);

					////int st = last_Index_Range * numColors_Range.get();
					////int ed = st + numColors_Range.get();

					////palette_clear();
					////for (int j = st; j < ed; j++)
					////{
					////	ofColor c = palette_Range[j];
					////	palette_addColor(c);
					////	ofLogNotice(__FUNCTION__) << "[" << i << "][" << (j - st) << "] > " << ofToString(c);
					////}

					////--

					////workflow

					//// DEMO
					//if (DEMO1_Test) myDEMO1.reStart();

					//textInput_New = range_Name;
					//bNewPreset = true;
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofxColorManager::palette_FromGradient()
{
	ofLogNotice(__FUNCTION__);

	//TODO:
	//must have an aux palette because we are overwritting the main palette...

	vector<ofColor> p;
	p.resize(numColors_Engines);

	for (int i = 0; i < numColors_Engines; i++)
	{
		float val = float(i / (float)(numColors_Engines - 1));
		ofColor c = getColorAtPercent(val);
		ofLogNotice(__FUNCTION__) << "Color:" << c;
		p[i] = c;

		ofLogNotice(__FUNCTION__) << "[" << i << "] > " << ofToString(c);
	}

	palette_clear();
	for (int i = 0; i < numColors_Engines.get(); i++)
	{
		palette_addColor(p[i]);
	}

	//--

	// workflow

	//set background color from first/last palette color
	if (AutoSet_BackGround_Color && !color_BackGround_Lock)
	{
		if (palette.size() > 0)
		{
			ofColor c = palette[0];//get first color from user palette

			if (color_BackGround_Darker)
			{
				float darkness = ofMap(color_BackGround_Darkness, 0.0, 1.0, 0.0, 2.0);
				float b = c.getBrightness() * darkness;
				b = ofClamp(b, 0.0, 1.0);
				c.setBrightness(b);
			}

			color_BackGround.set(c);
		}
	}

	//--

	refresh_Palette_TARGET(palette);
}

//--------------------------------------------------------------
void ofxColorManager::palette_FromQuantizer()
{
	ofLogNotice(__FUNCTION__);

	auto p = colorQuantizer.getPalette();

	palette_clear();

	for (int j = 0; j < p.size(); j++)
	{
		ofColor c = p[j];
		palette_addColor(c);
		ofLogNotice(__FUNCTION__) << "[" << j << "] > " << ofToString(c);
	}

	//--

	// workflow

	//set background color from first/last palette color
	if (AutoSet_BackGround_Color && !color_BackGround_Lock)
	{
		if (palette.size() > 0)
		{
			ofColor c;
			c = palette[0];//get first color from user palette

			if (color_BackGround_Darker)
			{
				float darkness = ofMap(color_BackGround_Darkness, 0.0, 1.0, 0.0, 2.0);
				float b = c.getBrightness() * darkness;
				b = ofClamp(b, 0.0, 1.0);
				c.setBrightness(b);
			}

			color_BackGround.set(c);
		}
	}

	//--

	refresh_Palette_TARGET(palette);
}

//--------------------------------------------------------------
void ofxColorManager::palette_FromRange(int index)
{
	last_Index_Range = index;//TODO:

	ofLogNotice(__FUNCTION__);
	if (palette_Range.size() > 0)
	{
		// populate palette

		int st = last_Index_Range * numColors_Range.get();
		int ed = st + numColors_Range.get();

		palette_clear();

		for (int j = st; j < ed; j++)
		{
			if (j < palette_Range.size())
			{
				ofColor c = palette_Range[j];
				palette_addColor(c);

				ofLogNotice(__FUNCTION__) << "[" << last_Index_Range << "][" << (j - st) << "] > " << ofToString(c);
			}
		}

		//--

		// workflow

		//set background color from first/last palette color
		if (AutoSet_BackGround_Color && !color_BackGround_Lock)
		{
			if (palette.size() > 0)
			{
				ofColor c = palette[0];//get first color from user palette

				if (color_BackGround_Darker)
				{
					float darkness = ofMap(color_BackGround_Darkness, 0.0, 1.0, 0.0, 2.0);
					float b = c.getBrightness() * darkness;
					b = ofClamp(b, 0.0, 1.0);
					c.setBrightness(b);
				}
				color_BackGround.set(c);
			}
		}
	}
}

//--------------------------------------------------------------
void ofxColorManager::refresh_Range_AutoUpdate()
{
	//auto create palette
	if (autoGenerate_Range && (last_Index_Range != -1))
	{
		int i = last_Index_Range;

		rangTypes[i] = true;
		range_Name = rangTypes[i].getName();
		txt_lineActive[0] = false;//preset name
		txt_lineActive[1] = false;//lover name
		txt_lineActive[2] = false;//theory name
		txt_lineActive[3] = true;//range name
		last_Index_Type = 3;

		textInput_New = range_Name;
		bNewPreset = true;
	}
}

#pragma mark - KEYS MOUSE CALLBACKS

//--------------------------------------------------------------
void ofxColorManager::keyPressed(ofKeyEventArgs &eventArgs)
{
	const int &key = eventArgs.key;
	ofLogNotice(__FUNCTION__) << " : " << key;

	if (key == 'K') ENABLE_keys = !ENABLE_keys;

	if (ENABLE_keys)
	{
		//----

		if (0) {}

		else if (key == 'g')
		{
			setToggleVisible();
		}

		else if (key == 'm')
		{
			SHOW_MINI_Preview = !SHOW_MINI_Preview;
		}

		// amount colors
		else if (key == '-')
		{
			numColors_Engines--;
		}
		else if (key == '+')
		{
			numColors_Engines++;
		}

		//--

		// help
		else if (key == 'h' || key == 'H')
		{
			ENABLE_HelpInfo = !ENABLE_HelpInfo;
		}

		//--

		// panels

		// browse modes
		else if (key == OF_KEY_TAB) {
			AppMode++;
		}

		//--

		else if (key == OF_KEY_F1)//palette
		{
			SHOW_UserPalette = !SHOW_UserPalette;
		}
		else if (key == OF_KEY_F2)//picker
		{
			SHOW_Picker = !SHOW_Picker;
			//if (SHOW_Picker) SHOW_BackGround = false;
		}
		else if (key == OF_KEY_F3)//bg
		{
			SHOW_BackGround = !SHOW_BackGround;
			//if (SHOW_BackGround) SHOW_Picker = false;
		}
		else if (key == OF_KEY_F4)//library
		{
			SHOW_Library = !SHOW_Library;
		}

		else if (key == OF_KEY_F5)//theory
		{
			SHOW_Theory = !SHOW_Theory;
		}
		else if (key == OF_KEY_F6)//range
		{
			SHOW_Range = !SHOW_Range;
		}
		else if (key == OF_KEY_F7)//lovers
		{
			SHOW_ColourLovers = !SHOW_ColourLovers;
		}
		else if (key == OF_KEY_F8)//quantizer
		{
			SHOW_Quantizer = !SHOW_Quantizer;
		}

		else if (key == OF_KEY_F9)//presets
		{
			SHOW_Presets = !SHOW_Presets;

			if (SHOW_Presets) {
				SHOW_UserPalette = false;
				SHOW_Picker = false;
				SHOW_Library = false;
				SHOW_BackGround = false;
				SHOW_Theory = false;
				SHOW_Range = false;
				SHOW_ColourLovers = false;
				SHOW_Quantizer = false;
				SHOW_GradientCurve = false;
			}
		}

		else if (key == OF_KEY_F10)//curve
		{
			SHOW_GradientCurve = !SHOW_GradientCurve;

			if (SHOW_GradientCurve) {
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
		//else if (key == OF_KEY_F10)//all on
		//{
		//	SHOW_UserPalette = true;// F1
		//	SHOW_Picker = true;// F2
		//	SHOW_GradientCurve = true;// F3
		//	SHOW_Library = true;// F4
		//	SHOW_BackGround = true;
		//	SHOW_Theory = true;// F5
		//	SHOW_Range = true;// F6
		//	SHOW_ColourLovers = true;// F7
		//	SHOW_Quantizer = true;// F8
		//	SHOW_Presets = true;
		//}
		else if (key == OF_KEY_F11)//all off
		{
			SHOW_UserPalette = false;
			SHOW_Picker = false;
			SHOW_GradientCurve = false;
			SHOW_Library = false;
			SHOW_BackGround = false;
			SHOW_Theory = false;
			SHOW_Range = false;
			SHOW_ColourLovers = false;
			SHOW_Quantizer = false;
			SHOW_Presets = false;
		}

		//-----

		// presets

		if (SHOW_Presets && !SHOW_Theory && !SHOW_Range && !SHOW_ColourLovers && !SHOW_Quantizer)
		{
			if (key == OF_KEY_UP)
			{
				if (preset_Index > 0)
				{
					preset_Index--;
				}
				if (preset_Index < files.size())
				{
					PRESET_Name = files_Names[preset_Index];
					ofLogNotice(__FUNCTION__) << "PRESET_Name: [" + ofToString(preset_Index) + "] " << PRESET_Name;
					preset_Load(PRESET_Name);
				}

				// new preset
				if (MODE_NewPreset) bNewPreset = false;
				// demo mode
				if (DEMO1_Test) myDEMO1.reStart();
				//load first color from preset to algorothmic palettes
				if (palette.size() > 0)
				{
					color_Picked = ofFloatColor(palette[0]);
					refresh_TheoryEngine();
				}
			}

			else if (key == OF_KEY_DOWN)
			{
				if (preset_Index < files.size() - 1)
				{
					preset_Index++;
				}
				if (preset_Index < files.size())
				{
					PRESET_Name = files_Names[preset_Index];
					ofLogNotice(__FUNCTION__) << "PRESET_Name: [" + ofToString(preset_Index) + "] " << PRESET_Name;
					preset_Load(PRESET_Name);
				}

				// new preset
				if (MODE_NewPreset) bNewPreset = false;
				// demo mode
				if (DEMO1_Test) myDEMO1.reStart();
				// load first color
				if (palette.size() > 0)
				{
					color_Picked = ofFloatColor(palette[0]);
					refresh_TheoryEngine();
				}
			}

			else if (key == ' ')
			{
				if (preset_Index < files.size() - 1)
				{
					preset_Index++;
				}
				else
				{
					if (preset_Index == files.size() - 1) preset_Index = 0;//cycle
				}

				if (preset_Index < files.size())
				{
					PRESET_Name = files_Names[preset_Index];
					ofLogNotice(__FUNCTION__) << "PRESET_Name: [" + ofToString(preset_Index) + "] " << PRESET_Name;
					preset_Load(PRESET_Name);
				}

				// new preset
				if (MODE_NewPreset) bNewPreset = false;
				// demo mode
				if (DEMO1_Test) myDEMO1.reStart();
				// load first color
				if (palette.size() > 0)
				{
					color_Picked = ofFloatColor(palette[0]);
					refresh_TheoryEngine();
				}
			}

			// get some presets by index random 
			else if (key == 'R' || key == 'r')
			{
				preset_Index = (int)ofRandom(0, files.size());

				if (preset_Index < files.size())
				{
					PRESET_Name = files_Names[preset_Index];
					ofLogNotice(__FUNCTION__) << "PRESET_Name: [" + ofToString(preset_Index) + "] " << PRESET_Name;
					preset_Load(PRESET_Name);
				}

				// new preset
				if (MODE_NewPreset) bNewPreset = false;
				//demo mode
				if (DEMO1_Test) myDEMO1.reStart();
				//load first color
				if (palette.size() > 0)
				{
					color_Picked = ofFloatColor(palette[0]);
					refresh_TheoryEngine();
				}
			}
		}

		//----

		// range

		if (SHOW_Range && !SHOW_Theory && !SHOW_ColourLovers && !SHOW_Quantizer)
		{
			if (key == OF_KEY_UP)
			{
				last_Index_Range--;
				last_Index_Range = (int)ofClamp(last_Index_Range, 0, NUM_TYPES_RANGES - 1);

				for (int i = 0; i < NUM_TYPES_RANGES; i++)
				{
					rangTypes[i].disableEvents();
					rangTypes[i] = false;
					rangTypes[i].enableEvents();
				}
				rangTypes[last_Index_Range] = true;
			}
			if (key == OF_KEY_DOWN)
			{
				last_Index_Range++;
				last_Index_Range = (int)ofClamp(last_Index_Range, 0, NUM_TYPES_RANGES - 1);

				for (int i = 0; i < NUM_TYPES_RANGES; i++)
				{
					rangTypes[i].disableEvents();
					rangTypes[i] = false;
					rangTypes[i].enableEvents();
				}
				rangTypes[last_Index_Range] = true;
			}
			if (key == ' ')
			{
				if (last_Index_Range == NUM_TYPES_RANGES - 1) last_Index_Range = 0;//cycle
				else if (last_Index_Range < NUM_TYPES_RANGES - 1) last_Index_Range++;

				for (int i = 0; i < NUM_TYPES_RANGES; i++)
				{
					rangTypes[i].disableEvents();
					rangTypes[i] = false;
					rangTypes[i].enableEvents();
				}
				rangTypes[last_Index_Range] = true;
			}

			//switch color c1/c2 selectors
			if (key == OF_KEY_BACKSPACE)
			{
				if (!autoPick_RangeColor1 && !autoPick_RangeColor2) autoPick_RangeColor1 = true;
				else if (autoPick_RangeColor1 && !autoPick_RangeColor2) autoPick_RangeColor2 = true;
				else if (!autoPick_RangeColor1 && autoPick_RangeColor2) autoPick_RangeColor1 = true;

				//workflow
				//get main color from range picker
				if (autoPick_RangeColor1) color_Picked.set(color_1_Range.get());
				else if (autoPick_RangeColor2) color_Picked.set(color_2_Range.get());
			}
		}

		//----

		// theory

		if (SHOW_Theory && !SHOW_Range && !SHOW_ColourLovers && !SHOW_Quantizer)
		{
			if (key == OF_KEY_UP)
			{
				last_Theory_PickPalette--;
				last_Theory_PickPalette = ofClamp(
					last_Theory_PickPalette.get(),
					last_Theory_PickPalette.getMin(),
					last_Theory_PickPalette.getMax());

				//poweroff
				for (int i = 0; i < NUM_COLOR_THEORY_TYPES_G1; i++)
				{
					theoryTypes[i].disableEvents();
					theoryTypes[i] = false;
					theoryTypes[i].disableEvents();
				}
				//enable
				if (last_Theory_PickPalette >= 0 &&
					last_Theory_PickPalette < NUM_COLOR_THEORY_TYPES_G1)
				{
					theoryTypes[last_Theory_PickPalette] = true;
				}
			}
			if (key == OF_KEY_DOWN)
			{
				last_Theory_PickPalette++;
				last_Theory_PickPalette = ofClamp(
					last_Theory_PickPalette.get(),
					last_Theory_PickPalette.getMin(),
					last_Theory_PickPalette.getMax());

				//poweroff
				for (int i = 0; i < NUM_COLOR_THEORY_TYPES_G1; i++)
				{
					theoryTypes[i].disableEvents();
					theoryTypes[i] = false;
					theoryTypes[i].disableEvents();
				}
				//enable
				if (last_Theory_PickPalette >= 0 &&
					last_Theory_PickPalette < NUM_COLOR_THEORY_TYPES_G1)
				{
					theoryTypes[last_Theory_PickPalette] = true;
				}
			}
			if (key == ' ')
			{
				last_Theory_PickPalette++;
				if (last_Theory_PickPalette == last_Theory_PickPalette.getMax())//cycle 
				{
					last_Theory_PickPalette = last_Theory_PickPalette.getMin();
				}

				//poweroff
				for (int i = 0; i < NUM_COLOR_THEORY_TYPES_G1; i++)
				{
					theoryTypes[i].disableEvents();
					theoryTypes[i] = false;
					theoryTypes[i].disableEvents();
				}
				//enable
				if (last_Theory_PickPalette >= 0 &&
					last_Theory_PickPalette < NUM_COLOR_THEORY_TYPES_G1)
				{
					theoryTypes[last_Theory_PickPalette] = true;
				}
			}
		}

		//----

		// layout

#ifdef INCL_LAYOUT
		//if (key == 'a')
		//{
		//	panels.SHOW_advanced = !panels.SHOW_advanced;
		//}
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

#ifdef INCL_LAYOUT
		else if (key == 'G')
		{
			SHOW_Gui_Internal = !SHOW_Gui_Internal;
		}
#endif

		//else if (key == 'g') {
		//    SHOW_ALL_GUI = !SHOW_ALL_GUI;
		//    setVisible(SHOW_ALL_GUI);
		//}

		//else if (key == 'P')
		//{
		//	bEditPalette = !bEditPalette;
		//}

		////edit layout
		//else if (key == 'E' || key == 'e')
		//{
		//	MODE_EditGradientLayout = !MODE_EditGradientLayout;
		//}

		//    else if (key == 'l')
		//    {
		//        bLock_palette = !bLock_palette;
		//    }

#ifdef USE_DEBUG_LAYOUT
		else if (key == 'M')
		{
			mouseRuler.toggleVisibility();

			//myDEMO1.toggleMouseCamera();
		}
#endif

		//else if (key == 'd')
		//{
		//    // DEBUG INTERFACE
		//    bShowDebug = !bShowDebug;
		//}

		//----

		//TODO:
		//randoms

//		// random user palette
//
//		// 1. randomize one color and build palette if enabled
//
//		else if (key == 'o')
//		{
//			//TODO
//			// WORKFLOW: when loading a colour lover palette we disable auto create from algo palettes
//			//if (!bAuto_Build_Palette)
//			//{
//			//bAuto_Build_Palette = true;
//			//}
//
//			//TODO: bug because some trigs are flags. we need direct functions
//			//bRandomColor = true;
//			color_Picked = ofFloatColor(ofRandom(0., 1.), ofRandom(0., 1.), ofRandom(0., 1.));
//
//			if (bAuto_Build_Palette)
//			{
//				refresh_TheoryEngine();
//
//#ifdef USE_RECTANGLE_INTERFACES
//				palette_FromTheory(SELECTED_palette_LAST);//trig last choice
//#endif
//			}
//
//			//-
//
//			// undo
//			color_Undo = color_Picked.get();
//			color_Undo.store();
//
//			//-
//
//			// preset manager
//			if (!MODE_NewPreset) bNewPreset = true;
//
//#ifdef USE_RECTANGLE_INTERFACES
//			textInput_New = "random_" + btns_plt_Selector[SELECTED_palette_LAST]->getName();
//#endif
//			//-
//
//			if (DEMO1_Test) myDEMO1.reStart();
//		}

		//----

//		// 2. randomize one color and created user palette
//
//		else if (key == 'p')
//		{
//			//-
//
//			if (!bAuto_Build_Palette)
//			{
//				bAuto_Build_Palette = true;
//			}
//
//			//-
//
//			//// A. create random palette
//
//			//random.generateRandom(numColors_Alg);
//			//refresh_TheoryEngine();
//			//if (bAuto_Build_Palette)
//			//{
//			//    //set random palette to user palette
//			//    palette_FromTheory(7);
//			//}
//
//			//-
//
//			// B. get a random palette from PANTONE
//
//			//int _r = (int) ofRandom(NUM_COLORS_PANTONE);
//			int r = (int)ofRandom(lib_Page_Index * lib_Page_NumColors, lib_Page_Index * lib_Page_NumColors + lib_Page_NumColors);
//			r = ofClamp(r, 0, NUM_COLORS_PANTONE);
//
//			//color pos & name
//			last_ColorPicked_Lib = r;
//			last_Lib_NameColor = palette_Lib_Names[r];
//			//last_Lib_NameColor = colorBrowser.colors_PantoneNames[r];
//			color_Picked = ofColor(palette_Lib_Cols[r]);
//			refresh_TheoryEngine();
//
//#ifdef USE_RECTANGLE_INTERFACES
//			palette_FromTheory(SELECTED_palette_LAST);
//#endif
//			//----
//
//			// undo
//
//			//----
//
//			color_Undo = color_Picked.get();
//			color_Undo.store();
//
//			//----
//
//			// presets
//
//			if (!MODE_NewPreset) bNewPreset = true;
//
//			textInput_New = colorBrowser.colors_PantoneNames[last_ColorPicked_Lib] + "_";
//
//#ifdef USE_RECTANGLE_INTERFACES
//			textInput_New += btns_plt_Selector[SELECTED_palette_LAST]->getName();
//#endif
//
//			//----
//
//			if (DEMO1_Test) myDEMO1.reStart();
//		}

		//--

		// TEST
		if (key == 'T')
		{
			TEST_Mode = !TEST_Mode;
		}

		//-

		// DEMO
		else if (key == 'D')
		{
			DEMO1_Test = !DEMO1_Test;
			if (DEMO1_Test) myDEMO1.reStart();
		}

		//--

		// colour-lovers

		if (SHOW_ColourLovers && !SHOW_Presets)
		{
			// 3. randomly get a palette from colour lovers only
			if (key == 'r' || key == 'R')
			{
#ifdef USE_COLOR_LOVERS
				colourLoversHelper.randomPalette();
#endif
				textInput_temp = PRESET_Name;
			}

			//-

			// browse presets
//#ifdef USE_COLOR_LOVERS
//			if ((SHOW_ColourLovers && key == ' '))
//			{
//				colourLoversHelper.nextPalette();
//			}
//#endif
		}

		//----

		//// undo color

		//if (key == 'z')
		//{
		//	color_Undo.undo();
		//	color_Picked = color_Undo;
		//}
		//else if (key == 'y')
		//{
		//	color_Undo.redo();
		//	color_Picked = color_Undo;
		//}
		////    else if()
		////        color_Undo.clearRedo();

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

		//// user palette

		//else if (key == 'c')
		//{
		//	palette_clear();
		//}

		//else if (key == 'x')
		//{
		//	// 1. remove last
		//	//palette_removeColorLast();

		//	// 2. remove selected
		//	palette_removeColor(palette_colorSelected);
		//}

		//else if (key == 'a')
		//{
		//    color_Picked = ofFloatColor(ofRandom(0., 1.), ofRandom(0., 1.), ofRandom(0., 1.));
		//    palette_addColor(ofColor(color_Picked.get()));
		//}

		//----

		// color browser

		////    colorBrowser.keyPressed( eventArgs );
		//
		//    else if (key == ' ')
		//        colorBrowser.switch_palette_Type();
		//
		//    else if (key == OF_KEY_RETURN)
		//        colorBrowser.switch_sorted_Type();
	}
}

//--------------------------------------------------------------
void ofxColorManager::keyReleased(ofKeyEventArgs &eventArgs)
{
	//if (eventArgs.key == ' ')
	//{
	//}
}

//--------------------------------------------------------------
void ofxColorManager::windowResized(int w, int h)
{
	//	// COLOUR LOVERS
	//#ifdef USE_COLOR_LOVERS
	//	glm::vec2 sizeGui(150, 375);
	//	glm::vec2 sizeGrid(150, h);
	//	glm::vec2 posGui(w - (sizeGui.x + sizeGrid.x + 4), 0);
	//	glm::vec2 posGrid(posGui.x + sizeGui.x + 2, 0);
	//	colourLoversHelper.setGrid(posGrid, sizeGrid);
	//	colourLoversHelper.setPosition(posGui, sizeGui);
	//	colourLoversHelper.windowResized(w, h);
	//#endif
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
}

//--------------------------------------------------------------
void ofxColorManager::mousePressed(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;

	//-

	// DEMO

	if (DEMO1_Test && !mouseOverGui)
	{
		//second mouse button cleans DEMO
		if (button == 2) myDEMO1.clear();
		else myDEMO1.start();//trig DEMO start
	}
}

//--------------------------------------------------------------
void ofxColorManager::mouseReleased(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;
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
void ofxColorManager::refresh_RearrengeFiles(std::string name)
{
	//--

	//workflow

	//go to ne index after adding a new preset
	ofLogNotice(__FUNCTION__) << "Rearrenge index file";
	//std::string path = path_Presets + textInput_New + ".json";

	//locate new position of old (saved) preset
	int ii = -1;
	for (size_t i = 0; i < files.size() && ii == -1; i++)
	{
		std::string n = files_Names[i];
		//std::string n = files[i].getBaseName();

		ofLogNotice(__FUNCTION__) << files_Names[i];

		if (n == name)
		{
			ii = int(i);
			ofLogNotice(__FUNCTION__) << "Found " << files_Names[i] << " at " << i << " index";
		}
	}

	//reload the same preset at newer index
	if (ii != -1)
	{
		//preset_Load(textInput_New);;

		preset_Index = ii;
		PRESET_Name = files_Names[preset_Index];
		ofLogNotice(__FUNCTION__) << PRESET_Name;

		preset_Load(PRESET_Name);
	}
}
//--------------------------------------------------------------
void ofxColorManager::preset_RefreshFiles()
{
	ofLogNotice(__FUNCTION__);

	//-

	// initialize

	path_Global = "ofxColorManager/";
	path_Kits = "kits/";
	path_Presets = path_Global + path_Kits + "presets/";
	path_Palettes = path_Global + path_Kits + "palettes/";
	ofxSurfingHelpers::CheckFolder(path_Global + path_Kits);

	//std::string _path = path_Presets;
	ofxSurfingHelpers::CheckFolder(path_Presets);
	ofDirectory dataDirectory(ofToDataPath(path_Presets, true));
	ofxSurfingHelpers::CheckFolder(path_Presets);
	ofLogNotice(__FUNCTION__) << path_Presets;

	// export for live reload preset from client app
	//path_Name_ExportColor = "ofxColorManager";

	//-

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

	preset_Index.setMax(files.size() - 1);
	if (preset_Index > files.size()) preset_Index = -1;

	//-

	//workflow

	//TODO:
	//void to go to 1st...

	// 1. load same position preset
	// if preset is deleted goes to nextone..
	// should check names because sorting changes..
	if (files_Names.size() > 0)
	{
		if (preset_Index > files_Names.size() - 1)
			preset_Index = files_Names.size() - 1;

		//else if (preset_Index > files_Names.size() - 1)

		PRESET_Name = files_Names[preset_Index];
		ofLogNotice(__FUNCTION__) << PRESET_Name;

		preset_Load(PRESET_Name);
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
	//    preset_Index = 0;
	//    PRESET_Name = files_Names[preset_Index];
	//    preset_Load(PRESET_Name);
	//}
	//else
	//{
	//    ofLogError(__FUNCTION__) << "NOT FOUND ANY FILE PRESET!";
	//}

	//--

	kit.resize(files_Names.size());
	for (int i = 0; i < files_Names.size(); i++)
	{
		kit[i] = PRESET_Temp.getPreset(files_Names[i]);

		//log
		ofLogNotice(__FUNCTION__) << "[ " << i << " ] " << files_Names[i];
		for (int c = 0; c < kit[i].palette.size(); c++)
		{
			ofLogNotice(__FUNCTION__) << c << " : " << ofToString(kit[i].palette[c]);
		}
	}
}

//--------------------------------------------------------------
void ofxColorManager::preset_Load(std::string p)
{
	ofLogNotice(__FUNCTION__) << p;

	//--

	// setup linking pointers to get back on load
	PRESET_Temp.setName(p);
	PRESET_Temp.setCurveName(PRESET_Name_Gradient);
	PRESET_Temp.setPalette(palette);

	//--

	txt_lineActive[0] = true;//preset name
	txt_lineActive[1] = false;//lover name
	txt_lineActive[2] = false;//theory name
	txt_lineActive[3] = false;//range name
	last_Index_Type = 0;

	//--

	// 1. load palette preset (target will be the above pointers) //TODO: should (late?) improve this..
	bool b = PRESET_Temp.preset_Load(p);
	if (!b)
	{
		ofLogError(__FUNCTION__) << "Preset file " << p << " not found! ";
		return;
	}

	//--

	//TODO:

	// curve gradient preset

	//--

	//TODO:

	// palette colors

	palette_clear();
	vector<ofColor> _p = PRESET_Temp.getPalette();
	for (int i = 0; i < _p.size(); i++)
	{
		ofLogNotice(__FUNCTION__) << "Col: " << ofToString(i) << " " << ofToString(_p[i]);
		palette_addColor(_p[i]);
	}

	//--

	// preset background
	if (!color_BackGround_Lock)
	{
		color_BackGround = ofColor(PRESET_Temp.getBackground());//get directly without pointing
		////PRESET_Temp.setBackgroundColor(color_BackGround);//error ofParameter
	}

	//--

	//reBuild();

	//----

	// DEMO 2
	myDEMO2.setPaletteColors(palette);

	//-

	// DEMO 1

	if (DEMO1_Test) myDEMO1.reStart();

	//-

	// workflow

	if (bAutoExportPreset)
	{
		ofLogNotice(__FUNCTION__) << "Auto EXPORT";
		exportPalette();
	}
}

//--------------------------------------------------------------
void ofxColorManager::preset_Save(std::string p)
{
	ofLogNotice(__FUNCTION__) << p;

	//TODO:
	//this is using pointers.. ?

	PRESET_Temp.setName(p);
	PRESET_Temp.setCurveName(PRESET_Name_Gradient);
	PRESET_Temp.setBackgroundColor(color_BackGround.get());
	PRESET_Temp.setPalette(palette);

	//PRESET_Temp.preset_Save(PRESET_Name);
	PRESET_Temp.preset_Save(p);
}

//----

//--------------------------------------------------------------
void ofxColorManager::refresh_FromPicked()
{
	ofLogNotice(__FUNCTION__);

	color_TheoryBase = color_Picked.get();
	//color_TheoryBase.set(color_Picked);

	// ?
	refresh_TheoryEngine();

	//TODO:

	//refresh
	ENABLE_Callbacks_cPickers = false;
	refresh_Picked_toHSB();
	ENABLE_Callbacks_cPickers = true;
	refresh_Picker_Touched();
}

//--------------------------------------------------------------
void ofxColorManager::refresh_Picked_toHSB()
{
	ofLogNotice(__FUNCTION__);

	color_HUE = int(255.f * color_Picked.get().getHue() + 0.5f);
	color_SAT = int(255.f * color_Picked.get().getSaturation() + 0.5f);
	color_BRG = int(255.f * color_Picked.get().getBrightness() + 0.5f);
}

//--------------------------------------------------------------
void ofxColorManager::refresh_Picker_Touched()
{
	ofLogNotice(__FUNCTION__) << "> color_Picked : " << ofToString(color_Picked);

	ENABLE_Callbacks_cPickers = false; //disable callbacks
	{
		// 1. theory color base

		if (bAuto_TheoryFromPicker)
		{
			color_TheoryBase = color_Picked.get();
		}

		//-

		// 2. range color1

		if (autoPick_RangeColor1)
		{
			color_1_Range = color_Picked.get();
			if (autoGenerate_Range) generate_Range(color_1_Range.get(), color_2_Range.get());
		}

		//-

		// 3. range color2

		if (autoPick_RangeColor2)
		{
			color_2_Range = color_Picked.get();
			if (autoGenerate_Range) generate_Range(color_1_Range.get(), color_2_Range.get());
		}

		//--

		refresh_Picked_toHSB();//redundant...

		//--

		//TODO:
		// mirror clicked/picked colors

		color_Clicked2.set(color_Picked.get());
	}
	ENABLE_Callbacks_cPickers = true;

	//--

	// autosave edited color

	if (bEditPalette &&
		palette_colorSelected != -1 &&
		palette_colorSelected < palette.size()-1)
	{
		// update user palette color with recently picked color
		palette[palette_colorSelected].set(color_Clicked2);

		// update gradient
		if (palette_colorSelected < gradient.getNumColors())
			gradient.replaceColorAtIndex(palette_colorSelected, color_Clicked2);
	}

	//--

	//workflow

	if (bAuto_Build_Palette)
	{
		reBuild();
	}
}

//----

// API - pointer references to ofApp color and palette

//--------------------------------------------------------------
void ofxColorManager::setColor_TARGET(ofColor &c)
{
	ofLogNotice(__FUNCTION__) << ofToString(c);
	color_TARGET = &c;
}

//--------------------------------------------------------------
void ofxColorManager::setPalette_TARGET(vector<ofColor> &p)
{
	ofLogNotice(__FUNCTION__) << ofToString(p);
	palette_TARGET = &p;
}

//--

//--------------------------------------------------------------
void ofxColorManager::setControl(float control)
{
	ofLogNotice(__FUNCTION__) << control;

	if (!MODE_EditGradientLayout)
	{
		curve_Ctrl_In.setWithoutEventNotifications(control);
		//curve_Ctrl_In = control;

		curve_Slider_Pick.setPercent(control);
	}
}

//--------------------------------------------------------------
std::string ofxColorManager::getPaletteName() {
	return PRESET_Name;
}

//--------------------------------------------------------------
ofColor ofxColorManager::getColor(int index)
{
	ofLogNotice(__FUNCTION__) << index;

	ofColor c;
	if (index == -1) c = ofColor(0, 255);
	else
	{
		if (index < palette.size())
		{
			c = palette[index];
		}
		else if (palette.size() > 0)
		{
			c = palette[0];
			ofLogError(__FUNCTION__) << "Index out of palette size. Get first color";
		}
		else {
			c = ofColor(0, 255);
			ofLogError(__FUNCTION__) << "No colors on palette: Empty! Return black";
		}
	}

	ofLogNotice(__FUNCTION__) << ofToString(c);
	return c;
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
	ofLogNotice(__FUNCTION__) << "SHOW_ALL_GUI:" << SHOW_ALL_GUI;

	if (SHOW_GradientCurve)
	{
		curve_Slider_Pick.setVisible(SHOW_ALL_GUI);
		curve_Slider_ExpTweak.setVisible(SHOW_ALL_GUI);
	}
}

//--------------------------------------------------------------
void ofxColorManager::setVisible(bool b)
{
	SHOW_ALL_GUI = b;
	ofLogNotice(__FUNCTION__) << "SHOW_ALL_GUI:" << SHOW_ALL_GUI;
}

//--------------------------------------------------------------
ofColor ofxColorManager::getColorAtPercent(float control)
{
	ofLogNotice(__FUNCTION__) << control;
	float out = ofMap(curvesTool.getAtPercent(1.0 - control), 0, MAX_LUT_SIZE - 1, 1., 0.);
	ofColor c = gradient.getColorAtPercent(out);
	return c;
}

//--------------------------------------------------------------
void ofxColorManager::setVisible_GUI_MINI(bool b)
{
	ofLogNotice(__FUNCTION__) << b;
	SHOW_MINI_Preview = b;
}

//--------------------------------------------------------------
void ofxColorManager::setBackground_ENABLE(bool b)
{
	ofLogNotice(__FUNCTION__) << b;
	background_Draw_ENABLE = b;
}

//--------------------------------------------------------------
void ofxColorManager::draw_GradientPreview(glm::vec2 pos, bool horizontal = true)
{
	if (horizontal)
	{
		image_GradientCurve.draw(pos);
	}

	//TODO: how to draw rotated... can image.rotate the image in some trig...
	//    else
	//    {
	//        ofPushMatrix();
	//        ofTranslate(pos);
	//        ofRotateDeg(90, 1, 0, 0);
	//        image_GradientCurve.draw(0, 0);
	//        ofPopMatrix();
	//    }
}

//--------------------------------------------------------------
void ofxColorManager::setVisible_debugText(bool b)
{
	ofLogNotice(__FUNCTION__) << b;
	SHOW_debugText = b;
}

//----

#pragma mark -  COLOUR LOVERS

////--------------------------------------------------------------
//void ofxColorManager::colourLovers_drawPreview()
//{
//	//// preview receivers
//
//	//if (SHOW_ColourLovers)
//	//{
//	//	int x, y, w, h, pad, lineH;
//	//	x = 320;
//	//	y = ofGetHeight() - 200;
//	//	w = h = 40;
//	//	pad = 3;
//	//	lineH = 20;
//
//	//	ofPushStyle();
//	//	ofFill();
//
//	//	ofDrawBitmapStringHighlight("RECEIVERS IN ofApp", x, y, ofColor::black, ofColor::white);
//	//	y += 3 * pad + lineH;
//
//	//	ofDrawBitmapStringHighlight("myColor:", x, y, ofColor::black, ofColor::white);
//	//	y += 3 * pad;
//
//	//	ofSetColor(myColor);
//	//	ofDrawRectangle(ofRectangle(x, y, w, h));
//	//	y += (h + pad);
//
//	//	y += (lineH);
//	//	ofDrawBitmapStringHighlight("myPalette:", x, y, ofColor::black, ofColor::white);
//	//	y += 3 * pad;
//
//	//	for (int i = 0; i < myPalette.size(); i++)
//	//	{
//	//		ofSetColor(myPalette[i]);
//	//		ofDrawRectangle(ofRectangle(x + i * (w + pad), y, w, h));
//	//	}
//	//	y += (h + pad);
//
//	//	y += (lineH);
//	//	ofDrawBitmapStringHighlight("myPalette_Name:", x, y, ofColor::black, ofColor::white);
//	//	y += (lineH);
//	//	ofDrawBitmapStringHighlight(myPalette_Name, x, y, ofColor::black, ofColor::white);
//
//	//	ofPopStyle();
//	//}
//}

//--------------------------------------------------------------
void ofxColorManager::setup_Range()
{
	color_1_Range.set("Color 1", ofColor::red, ofColor(0), ofColor(255));
	color_2_Range.set("Color 2", ofColor::blue, ofColor(0), ofColor(255));

	// 12 types
	rangeTypes_names = { "RGB", "HSL", "HSV ", "HSB", "LUV ", "LAB", "HLAB", "LCH", "CMY", "CMYK", "YXY", "XYZ" };

	autoGenerate_Range.set("Auto Generate", true);
	autoPick_RangeColor1.set("Auto Pick C1", true);
	autoPick_RangeColor2.set("Auto Pick C2", false);
	bGetPaletteFromRange.set("To User Palette", false);

	numColors_Range.set("Amnt Colors Rng", MAX_PALETTE_COLORS, 3, MAX_PALETTE_COLORS);
	numColors_Range.setSerializable(false);

	params_Ranges.setName("Params Ranges");
	for (int i = 0; i < int(NUM_TYPES_RANGES); i++) //12
	{
		rangTypes[i].set(ofToString(rangeTypes_names[i]), false);
		params_Ranges.add(rangTypes[i]);
	}
	params_Ranges.add(numColors_Range);
	params_Ranges.add(autoPick_RangeColor1);
	params_Ranges.add(autoPick_RangeColor2);
	params_Ranges.add(bGetPaletteFromRange);

	ofAddListener(params_Ranges.parameterChangedE(), this, &ofxColorManager::Changed_ColorRange);

	//--

	generate_Range(color_1_Range.get(), color_2_Range.get());

	bRange_Intitiated = true;
}

//--------------------------------------------------------------
void ofxColorManager::exportPalette()
{
	// default OF data path
	if (bExportPreset_DefaultPath)
	{
		path_FileExport = path_Folder_ExportColor_Data.get() + path_Name_ExportColor;
		ofxSurfingHelpers::CheckFolder(path_Folder_ExportColor_Data);
	}
	// custom path
	else
	{
		path_FileExport = path_Folder_ExportColor_Custom.get() + path_Name_ExportColor;
		ofxSurfingHelpers::CheckFolder(path_Folder_ExportColor_Custom);
	}
	ofLogNotice(__FUNCTION__) << "Export path: " << path_FileExport;

	//-

	// A. save palette colors only (without background)
	//ofxSerializer
	ofJson j = palette;
	string _path = path_FileExport + "_Palette.json";
	ofLogNotice(__FUNCTION__) << "\n" << ofToString(j);

	ofSavePrettyJson(_path, j);

	//-

	// B. save full preset
	//TODO:
	//should save colors + bakground too + gradient! = preset
	PRESET_Name_Gradient = path_Name_Gradient;
	PRESET_Temp.name = PRESET_Name;//? should use pointer
	PRESET_Temp.nameCurve = PRESET_Name_Gradient;//? path_Name_Gradient

	PRESET_Temp.preset_Save(path_FileExport + "_Bundled", true);

	//preset_Save(path_Folder_ExportColor_Custom);
}

//--------------------------------------------------------------
void ofxColorManager::generate_Range(ofColor col1, ofColor col2) {
	ofLogNotice(__FUNCTION__);

	int _max = 400;
	int _step;
	int _div;

	//_div = 10;
	_div = int((numColors_Range.get() - 1));
	_step = _max / _div;

	//ofLogNotice(__FUNCTION__) << "_div: " << _div;
	//ofLogNotice(__FUNCTION__) << "_step: " << _step;

	//-

	palette_Range.clear();

	for (int i = 0; i < int(NUM_TYPES_RANGES); ++i)
	{
		glm::vec3 left = { 270, 120 + i * _step, 0.0 };
		glm::vec3 right = { 670, 120 + i * _step, 0.0 };

		for (auto j = 0; j <= _max; j += _step)
		{
			glm::vec3 pos = { j + 270, 120 + i * _step, 0.0 };

			ofColor color = ofxColorMorph::colorMorph(pos, left, col1, right, col2, static_cast<type>(i));

			palette_Range.push_back(color);
		}
	}
}
