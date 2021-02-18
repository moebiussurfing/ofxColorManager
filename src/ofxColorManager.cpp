#include "ofxColorManager.h"

//TODO:
//--------------------------------------------------------------
void ofxColorManager::dragEvent(ofDragInfo info) {
	colorQuantizer.dragEvent(info);
}

//--------------------------------------------------------------
void ofxColorManager::build_Palette_Flip()
{
	ofLogNotice(__FUNCTION__) << "----------------- BUILD PALETTE FLIP -----------------";

	std::reverse(palette.begin(), palette.end());

	gradientEngine.buildFrom_TARGET();

	//----

	// workflow
	refresh_Background();
	// DEMO 2
	DEMO2_Svg.setPaletteColors(palette);
	// DEMO 1
	if (DEMO1_Test && !DEMO_Auto) myDEMO1.reStart();

	//--

	last_Index_ColorPalette.setMax(palette.size() - 1);
	//last_Index_ColorPalette = 0;
	//if (palette.size() > 0) color_Picked = palette[last_Index_ColorPalette];

	//--

	refresh_Palette_TARGET(palette);

	// export
	if (bAutoExportPreset)
	{
		bExportFlag = true;

		//ofLogNotice(__FUNCTION__) << "Auto EXPORT";
		//exportPalette();
	}
}

//--------------------------------------------------------------
void ofxColorManager::build_Palette_Engine()
{
	ofLogNotice(__FUNCTION__) << "----------------- BUILD PALETTE ENGINE -----------------";

	std::string _name = "";

	//--

	if (SHOW_ColourLovers)
	{
		_name = myPalette_Name_BACK;
		palette_FromColourLovers();
	}
	else if (SHOW_Quantizer)
	{
		_name = myPalette_Name_BACK;
		palette_FromQuantizer();
	}
	else if (SHOW_Theory)
	{
		_name = name_Theory;
		//refresh_Theory_G1();//?
		refresh_Theory_G2();
		palette_FromTheory(last_Index_Theory);
	}
	else if (SHOW_Range)
	{
		_name = name_Range;
		palette_FromRange(last_Index_Range);
	}

	//----

	// presets

	//if (SHOW_Presets)
	//{
	//	if (!MODE_NewPreset) MODE_NewPreset = true;
	//	//if (_name != "") textInput_New = _name;
	//	//else textInput_New = "name";
	//	textInput_New = _name;
	//}

	//--

	//TODO:
	// setup linking pointers to get back on load
	PRESET_Temp.setName_TARGET(_name);
	PRESET_Temp.setPalette_TARGET(palette);

#ifndef USE_SIMPLE_PRESET_PALETTE	
	PRESET_Temp.setNameCurve_TARGET(PRESET_Name_Gradient);
#endif

	//----

	/*
	// workflow
	refresh_Background();

	// workflow
	// new preset
	if (MODE_NewPreset) MODE_NewPreset = false;
	// demo mode
	if (DEMO1_Test && DEMO_Auto) myDEMO1.reStart();

	// DEMO 2
	DEMO2_Svg.setPaletteColors(palette);
	// DEMO 1
	if (DEMO1_Test && !DEMO_Auto) myDEMO1.reStart();

	//--

	last_Index_ColorPalette.setMax(palette.size() - 1);
	//last_Index_ColorPalette = 0;
	//if (palette.size() > 0) color_Picked = palette[last_Index_ColorPalette];

	//--

	// ofApp targets

	// palette
	refresh_Palette_TARGET(palette);

	// name
	if (name_TARGET != nullptr)
	{
		name_TARGET[0] = _name;
	}

	//--

	// export
	if (bAutoExportPreset)
	{
		bExportFlag = true;
		//ofLogNotice(__FUNCTION__) << "Auto EXPORT";
		//exportPalette();
	}
	*/


	//TODO:
	build_Palette_Preset();

	// export
	if (bAutoExportPreset)
	{
		bExportFlag = true;
		//ofLogNotice(__FUNCTION__) << "Auto EXPORT";
		//exportPalette();
	}
}

//--------------------------------------------------------------
void ofxColorManager::build_Palette_Preset()
{
	ofLogNotice(__FUNCTION__) << "----------------- BUILD PALETTE PRESET -----------------";

	std::string _name;

	//-

	// presets

	//if (SHOW_Presets)
	//{
	//	if (!MODE_NewPreset) MODE_NewPreset = true;
	//	//if (_name != "") textInput_New = _name;
	//	//else textInput_New = "name";
	//}

	//----

	// workflow
	refresh_Background();
	// DEMO 2
	DEMO2_Svg.setPaletteColors(palette);
	// DEMO 1
	if (DEMO1_Test && !DEMO_Auto) myDEMO1.reStart();

	//// export
	//if (bAutoExportPreset)
	//{
	//	ofLogNotice(__FUNCTION__) << "Auto EXPORT";
	//	//exportPalette();
	//}

	//--

	last_Index_ColorPalette.setMax(palette.size() - 1);
	last_Index_ColorPalette = 0;
	if (palette.size() > 0) color_Picked = palette[last_Index_ColorPalette];

	//--

	// ofApp targets

	// picked color
	if (colorBg_TARGET != nullptr)
	{
		colorBg_TARGET->set(color_BackGround.get());
	}

	// palette
	refresh_Palette_TARGET(palette);

	// name
	if (name_TARGET != nullptr)
	{
		name_TARGET[0] = PRESET_Name;
	}
}

//--------------------------------------------------------------
ofxColorManager::ofxColorManager()
{
	ofAddListener(ofEvents().update, this, &ofxColorManager::update);
#ifdef AUTO_DRAW_CALLBACK
	ofAddListener(ofEvents().draw, this, &ofxColorManager::draw, OF_EVENT_ORDER_BEFORE_APP);
#endif

	//ofAddListener(ofEvents().update, this, &ofxColorManager::update);

	//mouse callbacks
	ofAddListener(ofEvents().mouseDragged, this, &ofxColorManager::mouseDragged);
	ofAddListener(ofEvents().mouseScrolled, this, &ofxColorManager::mouseScrolled);

	//--

	//default
	fps = 60.0f;
	dt = 1.f / fps;

	//--

	// file paths

	path_Global = "ofxColorManager/";
	ofxSurfingHelpers::CheckFolder(path_Global);
	path_AppState = path_Global + "ofxColorManager_Settings.xml";

	//// gradient
	//path_Layout = path_Global + "layout/";
	//ofxSurfingHelpers::CheckFolder(path_Layout);
	//path_Folder_Gradient = path_Global + "gradients/";
	//ofxSurfingHelpers::CheckFolder(path_Folder_Gradient);
	//path_Name_Gradient = "myGradient";
	//path_Gradient_LUT = path_Folder_Gradient + path_Name_Gradient + ".yml";
	//path_Gradient_Preset = path_Folder_Gradient + path_Name_Gradient + ".xml";
	//PRESET_Name_Gradient = path_Name_Gradient;

	PRESET_Name = "PRESET_Name";

	// export
	path_Folder_ExportColor_Data = path_Global + "export/";
	path_Folder_ExportColor_Custom = path_Global + "export/";//temp
	ofxSurfingHelpers::CheckFolder(path_Folder_ExportColor_Data);
	ofxSurfingHelpers::CheckFolder(path_Folder_ExportColor_Custom);
	path_Name_ExportColor = "ofxColorManager";// name
	path_FileExport = path_Folder_ExportColor_Custom.get() + path_Name_ExportColor;

	//--

	helpInfo = "HELP INFO\n\n";
	helpInfo += "H                   HELP\n";
	//helpInfo += "K                   KEYS\n";
	helpInfo += "G                   GUI\n";
	helpInfo += "\n";
	helpInfo += "SPACE               >>\n";
	helpInfo += "Down|Up             BROWSE\n";
	helpInfo += "-|+                 AMOUNT COLORS\n";
	helpInfo += "\n";
	//helpInfo += "\n";
	helpInfo += "PANELS\n";
	helpInfo += "F1                  PRESETS\n";
	helpInfo += "F2                  PALETTE COLORS\n";
	helpInfo += "F3                  PALETTE EDITOR\n";
	helpInfo += "F4                  PICKER\n";
	helpInfo += "F5                  BACKGROUND\n";
	helpInfo += "F6                  LIBRARY\n";
	helpInfo += "F7                  GRADIENT\n";
	helpInfo += "\n";
	helpInfo += "ENGINES\n";
	helpInfo += "TAB                 APP MODES\n";
	helpInfo += "                    THEORY\n";
	helpInfo += "                    RANGE\n";
	helpInfo += "                    LOVERS\n";
	helpInfo += "                    PICTURE\n";
	helpInfo += "\n";
	helpInfo += "F11                 HIDE ALL\n";
	helpInfo += "\n";
	//helpInfo += "TEST\n";
	//helpInfo += "D                   DEMO SCENE\n";
	//helpInfo += "T                   GRADIENT TEST\n";
	//helpInfo += "m                   MINI PREVIEW\n";
	//helpInfo += "M                   DEBUG\n";
}

//--------------------------------------------------------------
void ofxColorManager::setup()
{
	ofLogNotice(__FUNCTION__) << endl << endl << "----------------- SETUP -----------------" << endl;

	//TODO:
	setup_Range();

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

	//gui layout
	//library
	sizeLibColBox.set("Size Lib", 25, 10, 100);
	lib_Responsive_Mode.set("Responsive", false);
	bPagerized.set("Mode Paging", false);
	//palette
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

	//txt_lineActive[0] = false;
	//txt_lineActive[1] = false;
	//txt_lineActive[2] = false;
	//txt_lineActive[3] = false;
	last_Index_Type = -1;

	//-

	font.load("assets/fonts/telegrama_render.otf", 11, true, true, true);
	//font.load("assets/fonts/LCD_Solid.ttf", 11, true, true);
	//font.load("assets/fonts/overpass-mono-bold.otf", 9, true, true);

	//----

	// colorQuantizer

#ifdef USE_IMAGE_QUANTIZER
	colorQuantizer.setup();

	// receivers pointers
	colorQuantizer.setColor_BACK(myColor_BACK);
	colorQuantizer.setColor_BACK_Refresh(bUpdated_Color_BACK);
	colorQuantizer.setPalette_BACK(myPalette_BACK);
	colorQuantizer.setPalette_BACK_RefreshPalette(bUpdated_Palette_BACK);
	colorQuantizer.setPalette_BACK_Name(myPalette_Name_BACK);
#endif

	//----

	// theory

	setup_Theory_G1();

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

	// lovers

#ifdef USE_COLOR_LOVERS

	colourLoversHelper.setup();

	//----

	// pointers back
	colourLoversHelper.setColor_BACK(myColor_BACK);
	colourLoversHelper.setColor_BACK_Refresh(bUpdated_Color_BACK);
	colourLoversHelper.setPalette_BACK(myPalette_BACK);
	colourLoversHelper.setPalette_BACK_Name(myPalette_Name_BACK);
	colourLoversHelper.setPalette_BACK_Refresh(bUpdated_Palette_BACK);

	//--------------------------------------------------------------
	listener_LoverName = colourLoversHelper.lastPaletteName.newListener([this](std::string &n) {
		//txt_lineActive[0] = false;//preset name
		//txt_lineActive[1] = true;//lover name
		//txt_lineActive[2] = false;//theory name
		//txt_lineActive[3] = false;//range name
		last_Index_Type = 1;

		ofLogNotice("colourLoversHelper > Name Palette: ") << n;

	});

#endif

	//----

	// some initiation values..
	myColor_BACK = ofColor::white;
	myPalette_BACK.resize(2);//pointer setter will clear/resize. nevermind the vector size here
	myPalette_BACK[0] = ofColor::grey;
	myPalette_BACK[1] = ofColor::black;
	myPalette_Name_BACK = "";

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
	color_backGround_SET.set("Set From Picker", false);
	AutoSet_BackGround_Color.set("AutoSet From Palette", true);
	color_BackGround_AmtDarker.set("Amnt Darker", 0.5, 0., 1.);
	color_BackGround_DarkerMode.set("Mode Darker", false);
	color_BackGround_GradientMode.set("Mode Gradient", false);
	color_BackGround_Lock.set("Lock Background", false);

	//color_BackGround_GradientMode.makeReferenceTo(gradientEngine.color_BackGround_GradientMode);
	gradientEngine.color_BackGround_GradientMode.makeReferenceTo(color_BackGround_GradientMode);

	//-----

	// color picker

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

	////TODO: add?	
	//// global to apply to all colors?
	//SATURATION.set("SATURATION", 200, 0, 255);
	//BRIGHTNESS.set("BRIGHTNESS", 200, 0, 255);

	bAuto_Build_Palette.set("Auto Build Palette", true);

	numColors_Theory_G2.set("Amnt Colors Alg", 6, 2, MAX_PALETTE_COLORS);
	numColors_Theory_G2.setSerializable(false);
	analogSpread.set("Analogue Spread", 0.2, 0, 1);

	//bLock_palette.set("Lock Palettes", false);
	//bRandomPalette.set("RANDOM PALETTE", false);

	params_Theory.setName("ADVANCED");
	params_Theory.add(numColors_Theory_G2);
	params_Theory.add(bAuto_Build_Palette);
	params_Theory.add(analogSpread);
	//params_Theory.add(MODE_TweakSatBrg);
	//params_Theory.add(bLock_palette);
	//params_Theory.add(bRandomPalette);
	//params_Theory.add(SATURATION);
	//params_Theory.add(BRIGHTNESS);

	//-

	//exclude

	//SATURATION.setSerializable(false);
	//BRIGHTNESS.setSerializable(false);

	//SHOW_AlgoPalettes.setSerializable(false);
	//color_BackGround.setSerializable(false);
	//color_Picked.setSerializable(false);

	color_HUE.setSerializable(false);
	color_SAT.setSerializable(false);
	color_BRG.setSerializable(false);
	color_HUE_0.setSerializable(false);
	color_SAT_0.setSerializable(false);
	color_BRG_0.setSerializable(false);

	DEMO_Cam.setSerializable(false);
	//DEMO2_Edit.setSerializable(false);

	ofAddListener(params_Theory.parameterChangedE(), this, &ofxColorManager::Changed_Controls);

	//-

	refresh_Theory_G2();

	//-

	SHOW_Gradient.set("GRADIENT", true);
	SHOW_Gradient.makeReferenceTo(gradientEngine.SHOW_Gradient);
	//gradientEngine.SHOW_Gradient.makeReferenceTo(SHOW_Gradient);

	// gradient
	gradientEngine.setup();
	gradientEngine.setBackground_TARGET(color_BackGround);
	gradientEngine.setPalette_TARGET(palette);//subscribe local palette to allow modify from the class object

	//-

	bRandomColor.set("RANDOM C", false);
	bAddColor.set("ADD", false);
	bEditPalette.set("EDIT", false);
	bRemoveColor.set("REMOVE", false);
	bClearPalette.set("CLEAR", false);

	SHOW_Scene.set("SCENE", true);

	//SHOW_Scene.setSerializable(false);
	bEditPalette.setSerializable(false);

	//-

	// panels

	SHOW_ALL_GUI.setName("GUI MAIN");
	SHOW_MINI_Preview.setName("MINI PREVIEW");
	SHOW_UserPaletteFloating.setName("PALETTE");
	SHOW_UserPaletteEditor.setName("EDITOR");
	SHOW_Theory.setName("THEORY");
	SHOW_debugText.setName("SHOW debug");
	SHOW_Quantizer.setName("PICTURE");

	//-

	SHOW_BrowserColors.set("SHOW BROWSER COLORS", false);
	SHOW_BrowserColors.setSerializable(false);

	SHOW_ColourLovers_searcher.set("SHOW COLOUR LOVERS SEARCH", true);
	SHOW_ColourLovers.set("LOVERS", true);

	SHOW_Presets.set("PRESETS", true);
	SHOW_Kit.set("SHOW KIT", true);
	AutoScroll.set("AutoScroll", true);
	SHOW_BackGround.set("BACKGROUND", true);
	SHOW_Picker.set("PICKER", true);
	SHOW_Library.set("LIBRARY", false);
	SHOW_Range.set("RANGE", true);
	SHOW_Panels.set("PANELS", true);
	SHOW_Export.set("EXPORT", true);
	SHOW_Demos.set("DEMO", false);

	//bAutoResizePalette.set("AutoResize", false);
#ifndef USE_SIMPLE_PRESET_PALETTE	
	bModeBundlePreset.set("Mode Bundle", false);
	bModePalettePreset.set("Mode Palette", false);
#endif
	bAutoExportPreset.set("Auto Export", false);
	bExportPreset_DefaultPath.set("Path /bin/data", true);
	path_Folder_ExportColor_Custom.set("ExportPath", "");

	//-

	scale_ColRange.set("Scale Rg", 1.f, 0.25f, 1.25f);

	//-

	//palette

	boxSizeUser.set("Box Size", 40, 10, 200);
	boxMaxRows.set("Max Rows", 10, 1, 20);
	scale_ColPalette.set("Scale Pt", 1.f, 0.25f, 1.25f);
	bFlipUserPalette.set("FLIP", false);
	//bEditUserPalette.set("EDIT", false);
	//bUserPaletteVertical.set("VERTICAL", false);

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
	params_ColorTheory_G2.clear();
	params_ColorTheory_G2.setName("Color Theory G2");
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
		types_Theory_G2[i].set(name, false);
		params_ColorTheory_G2.add(types_Theory_G2[i]);
	}

	ofAddListener(params_ColorTheory_G2.parameterChangedE(), this, &ofxColorManager::Changed_ColorTheory);

	//-

	SHOW_ALL_GUI = true;
	SHOW_debugText = false;
	SHOW_MINI_Preview = false;

	//--

	//exclude
	numColors_Theory_G1.setSerializable(false);
	numColors_Theory_G2.setSerializable(false);

	//----

	// GUI

	//official fork
	////#ifdef INCLUDE_IMGUI_CUSTOM_THEME_AND_FONT
	////	ofxSurfingHelpers::ImGui_FontCustom();
	////#endif
	//gui.setup();
	//ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//ImGui::GetIO().MouseDrawCursor = false;

	//TODO:
	//daan fork
	gui.setup(nullptr, true, ImGuiConfigFlags_DockingEnable, true, false);

	// fonts
#ifdef INCLUDE_IMGUI_CUSTOM_THEME_AND_FONT
	auto normalCharRanges = ImGui::GetIO().Fonts->GetGlyphRangesDefault();
	float _size = 10.f;
	std::string _name = "telegrama_render.otf";
	std::string _path = "assets/fonts/" + _name;//assets folder
	customFont = gui.addFont(_path, _size, nullptr, normalCharRanges);
#endif

	//-

	// theme
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

	AppMode.set("AppMode", 0, 0, NUM_APP_MODES - 1);
	AppMode_name.set("", "");
	AppMode.setSerializable(false);

	//-

	// startup settings

	params_AppState.setName("ofxColorManager");

	params_engines.setName("Engines");
	params_engines.add(last_Index_Preset);
	params_engines.add(AppMode);
	params_engines.add(SHOW_Scene);
	params_engines.add(last_Index_Theory);
	params_engines.add(last_Index_Range);
	params_engines.add(color_1_Range);
	params_engines.add(color_2_Range);
	//params_engines.add(color_Picked);
	// amount colors
	params_engines.add(numColors_Engines);
	//params_engines.add(numColors_Range);
	//params_engines.add(numColors_Theory_G2);
	params_engines.add(analogSpread);
	params_AppState.add(params_engines);

	// export colors
	params_Export.setName("ExportColors");
#ifndef USE_SIMPLE_PRESET_PALETTE	
	params_Export.add(bModeBundlePreset);
	params_Export.add(bModePalettePreset);
#endif
	params_Export.add(bAutoExportPreset);
	params_Export.add(bExportPreset_DefaultPath);
	params_Export.add(path_Folder_ExportColor_Custom);
	params_AppState.add(params_Export);

	params_Panels.add(SHOW_ALL_GUI);
	params_Panels.add(SHOW_Presets);
	params_Panels.add(SHOW_UserPaletteFloating);
	params_Panels.add(SHOW_UserPaletteEditor);
	params_Panels.add(SHOW_BackGround);
	params_Panels.add(SHOW_Library);
	params_Panels.add(SHOW_Picker);
	params_Panels.add(SHOW_Range);
	params_Panels.add(SHOW_Theory);
	params_Panels.add(SHOW_Panels);
	params_Panels.add(SHOW_Export);
	params_Panels.add(SHOW_Demos);
	params_Panels.add(SHOW_Quantizer);
	params_Panels.add(SHOW_ColourLovers);
	params_Panels.add(SHOW_BrowserColors);
	params_Panels.add(SHOW_MINI_Preview);
	params_Panels.add(SHOW_Kit);
	params_Panels.add(AutoScroll);
	params_Panels.add(SHOW_Gradient);
	params_Panels.add(gradientEngine.SHOW_CurveEditor);
	//params_Panels.add(gradientEngine.SHOW_Gradient);
	params_AppState.add(params_Panels);

	params_Background.add(color_BackGround);
	params_Background.add(color_BackGround_DarkerMode);
	params_Background.add(color_BackGround_GradientMode);
	params_Background.add(AutoSet_BackGround_Color);
	params_Background.add(color_BackGround_AmtDarker);
	params_Background.add(color_BackGround_Lock);
	params_AppState.add(params_Background);

	//params_Demo.add(TEST_Mode);
	params_Demo.add(DEMO1_Test);
	params_Demo.add(DEMO2_Test);
	params_Demo.add(DEMO2_Edit);
	params_Demo.add(DEMO2_Scale);
	params_Demo.add(DEMO_Auto);
	params_Demo.add(DEMO_Timer);
	params_Demo.add(DEMO_Alpha);
	params_Demo.add(DEMO_Cam);
	params_AppState.add(params_Demo);

	//params_Picker.add(BRIGHTNESS);
	//params_Picker.add(SATURATION);

	params_Picker.add(bColor_HUE);
	params_Picker.add(bColor_SAT);
	params_Picker.add(bColor_BRG);
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
	params_Palette2.add(bEditPalette);
	//params_Palette2.add(bAuto_Build_Palette);
	//params_Palette2.add(bAutoResizePalette);
	params_AppState.add(params_Palette2);

	//---

	// grouped for callback function

	params_control.setName("ofxColorManager");

	params_control.add(MODE_NewPreset);
	params_control.add(numColors_Engines);
	params_control.add(AppMode);
	params_control.add(last_Index_Theory);
	params_control.add(last_Index_Range);
	params_control.add(last_Index_ColorPalette);
	params_control.add(color_BackGround);
	// edit palette
	params_control.add(bRandomColor);
	params_control.add(bEditPalette);
	params_control.add(bAddColor);
	params_control.add(bRemoveColor);
	params_control.add(bClearPalette);
	// background
	params_control.add(color_backGround_SET);
	params_control.add(AutoSet_BackGround_Color);
	params_control.add(color_BackGround_DarkerMode);
	params_control.add(color_BackGround_AmtDarker);
	params_control.add(color_BackGround_Lock);
	params_control.add(color_BackGround_GradientMode);
	// panels
	params_control.add(SHOW_Panels);
	params_control.add(SHOW_Export);
	params_control.add(SHOW_Demos);
	params_control.add(SHOW_BrowserColors);
	params_control.add(SHOW_Presets);
	params_control.add(SHOW_BackGround);
	params_control.add(SHOW_Library);
	params_control.add(SHOW_Theory);
	params_control.add(SHOW_Range);
	params_control.add(SHOW_Picker);
	params_control.add(SHOW_ColourLovers);
	params_control.add(SHOW_Quantizer);
	params_control.add(SHOW_ColourLovers_searcher);
	params_control.add(SHOW_UserPaletteFloating);
	params_control.add(SHOW_UserPaletteEditor);
	params_control.add(SHOW_ALL_GUI);
	params_control.add(SHOW_Gradient);

#ifndef USE_SIMPLE_PRESET_PALETTE	
	params_control.add(bModeBundlePreset);
	params_control.add(bModePalettePreset);
#endif

	params_control.add(DEMO2_Edit);
	params_control.add(DEMO2_Scale);

	ofAddListener(params_control.parameterChangedE(), this, &ofxColorManager::Changed_Controls);

	//------------------------------------------------

	// startup

	startup();
}

//--------------------------------------------------------------
void ofxColorManager::startup()
{
	ofLogNotice(__FUNCTION__) << endl << endl << "----------------- STARTUP -----------------" << endl;

	//--

	ENABLE_keys = true;

	//--

	// gradient
	gradientEngine.startup();

	//----

	// preset manager
	preset_RefreshFiles();

	//--

	// color ranges
	setup_Range();
	//generate_Range(color_1_Range.get(), color_2_Range.get());

	ofxSurfingHelpers::loadGroup(params_AppState, path_AppState);

	//--

	//startup
	last_Index_Theory = 0;
	theory_Types_G1[last_Index_Theory] = true;//on button

	last_Index_Range = 0;
	types_Range[last_Index_Range] = true;//on button

	//--

	// preset manager

	//load last session preset index
	if (last_Index_Preset < files.size())
	{
		PRESET_Name = files_Names[last_Index_Preset];

		preset_Load(PRESET_Name);
	}

	//--

	//refresh theory and ranges
	refresh_Pick_ToEngines();

	// refresh hsb
	refresh_Pick_ToHSB();

	//--

	// name
	if (name_TARGET != nullptr)
	{
		name_TARGET[0] = PRESET_Name;
	}

	//--

#ifdef LINK_TCP_MASTER_CLIENT
	setupLink();
#endif
}

//--------------------------------------------------------------
void ofxColorManager::update(ofEventArgs & args)
{
	//if (ofGetFrameNum() % int(60 * 1.0) == 0) ofLog();//timer spaced

	//--

#ifdef LINK_TCP_MASTER_CLIENT
	updateLink();
#endif

	//--

	// auto pilot browse presets

	if (auto_pilot && (ofGetElapsedTimeMillis() - auto_pilot_timer > auto_pilot_Duration * 1000))
	{
		auto_pilot_timer = ofGetElapsedTimeMillis();

		{
			if (last_Index_Preset < files.size() - 1)
			{
				last_Index_Preset++;
			}
			else if (last_Index_Preset == files.size() - 1) {//cycle
				last_Index_Preset = 0;
			}

			if (last_Index_Preset < files.size() && files.size() > 0)
			{
				PRESET_Name = files_Names[last_Index_Preset];
				ofLogNotice(__FUNCTION__) << "PRESET_Name: [" + ofToString(last_Index_Preset) + "] " << PRESET_Name;

				preset_Load(PRESET_Name);
			}
		}
	}

	//--

	// export
	if (bExportFlag)
	{
		bExportFlag = false;

		// export
		if (bAutoExportPreset)
		{
			ofLogNotice(__FUNCTION__) << "Auto EXPORT";
			exportPalette();
		}
	}

	//--

	// gradient
	gradientEngine.update();

	//----

	// export
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

	// demos
	{
		// DEMO1
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

		//----

		// DEMO2
		if (DEMO2_Test) DEMO2_Svg.update();
	}

	//----

	// lovers
	{
		colourLoversHelper.update();
	}

	//----

	//TODO:
	// this flags are only used 
	// by quantizer and lover addons
	// it mens that some addon has clicked/changed the main palette/color 
	//bUpdated_Palette_BACK = true;// to trig this

	if (bUpdated_Palette_BACK)
	{
		bUpdated_Palette_BACK = false;

		ofLogNotice(__FUNCTION__) << "  >  bUpdated_Palette_BACK : " << bUpdated_Palette_BACK;

		build_Palette_Engine();

		//--

		// export
		if (bAutoExportPreset)
		{
			bExportFlag = true;

			//ofLogNotice(__FUNCTION__) << "Auto EXPORT";
			//exportPalette();
		}
	}

	//----

	// 2. color pick from palette clicked

	if (bUpdated_Color_BACK)
	{
		bUpdated_Color_BACK = false;

		ofLogWarning(__FUNCTION__) << "  >  bUpdated_Color_BACK : " << bUpdated_Color_BACK;

		// 2. get color
		if (SHOW_ColourLovers) color_Clicked2 = ofColor(myColor_BACK);
		else if (SHOW_Quantizer) color_Clicked.set(myColor_BACK);
		else build_Palette_Engine();
	}

	//----

	//TODO:
	//not used and hidden

	// 3. color clicked

	if (color_Clicked2 != color_Clicked2_PRE && SHOW_ColourLovers)
	{
		ofLogNotice(__FUNCTION__) << "  >  color_Clicked2 !";
		color_Clicked2_PRE = color_Clicked2;

		color_Picked.set(color_Clicked2);
	}

	//----

	// 4. set the local scope color pointer that is into ofxColorBrowser that whill be used as color picked too

	if (color_BACK != color_BACK_PRE)
	{
		color_BACK_PRE = color_BACK;

		ofLogNotice(__FUNCTION__) << "  >  color_BACK ! ";
		//ofLogNotice(__FUNCTION__) << "Changed color_BACK pointer";

		color_Picked.set(color_BACK);
	}
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
void ofxColorManager::draw_Info()
{
	ofPushMatrix();
	ofPushStyle();

	//blink when a new preset is editing
	float freq = 0.2f;//speed freq
	float min = 0.5f;
	float max = 1.0f;
	float a;

	if (MODE_NewPreset) a = ofMap(glm::sin(freq * ofGetFrameNum()), -1.f, 1.f, min, max);
	else a = 1.0f;

	//-

	int pady = 20;
	int padh = 0;

	int sp = 3;
	int x;
	int y;
	int h;

	std::string t0 = name_TARGET[0];

	float _w0 = ofxSurfingHelpers::getWidthBBtextBoxed(fontBig, t0);

	y = pady + fontBig.getSize();

	int _alpha = 200;

	ofColor c0, c1;
	c0.set(0, _alpha * a);
	c1.set(255, _alpha * a);


	h = padh;
	y += h;

	// label tittle

	x = ofGetWidth() * 0.5 - _w0 * 0.5;
	ofSetColor(c0);
	fontBig.drawString(t0, x + sp, y + sp);

	ofSetColor(c1);
	fontBig.drawString(t0, x, y);

	//-

	ofPopStyle();
	ofPopMatrix();
}

//TODO:
//another draw info alternative more detailed into user workflow...
////--------------------------------------------------------------
//void ofxColorManager::draw_Info()
//{
//	ofPushMatrix();
//	ofPushStyle();
//
//	//-
//
//	int pady = 50;
//	int padh;
//	int sp = 3;
//	int x;
//	int y;
//	int h;
//
//	std::string t0 = PRESET_Name;
//	std::string t1 = myPalette_Name_BACK;
//	std::string t2 = name_Theory;
//	std::string t3 = name_Range;
//
//	float _w0 = ofxSurfingHelpers::getWidthBBtextBoxed(fontBig, t0);
//	float _w1 = ofxSurfingHelpers::getWidthBBtextBoxed(fontBig, t1);
//	float _w2 = ofxSurfingHelpers::getWidthBBtextBoxed(fontMedium, t2);
//	float _w3 = ofxSurfingHelpers::getWidthBBtextBoxed(fontSmall, t3);
//
//	int i = 0;
//
//	y = pady + fontBig.getSize();
//
//	ofColor c0, c1, c0_Ghost, c1_Ghost;
//	int _alphaGhost = 32;
//
//	c0.set(0, 200);
//	c1.set(255, 200);
//
//	c0_Ghost.set(0, _alphaGhost);
//	c1_Ghost.set(255, _alphaGhost);
//
//	//TODO: create double line shadow draw methoid into ofxSurfingHelpers
//
//	padh = 15;
//	h = padh;
//	y += h;
//
//	//preset name
//	//if (t0 != "" || txt_lineActive[0])//hide now used lines vs draw but transparent
//	if (txt_lineActive[0])//hide now used lines vs draw but transparent
//	{
//		x = ofGetWidth() * 0.5 - _w0 * 0.5;
//		if (txt_lineActive[i]) ofSetColor(c0);
//		else ofSetColor(c0_Ghost);
//		fontBig.drawString(t0, x + sp, y + sp);
//
//		if (txt_lineActive[i]) ofSetColor(c1);
//		else ofSetColor(c1_Ghost);
//		fontBig.drawString(t0, x, y);
//
//		padh = 25;
//		h = fontBig.getSize() + padh;
//		y += h;
//	}
//	i++;
//
//	//lover name
//	//if (t1 != "" || txt_lineActive[1])
//	if (txt_lineActive[1])
//	{
//		x = ofGetWidth() * 0.5 - _w1 * 0.5;
//		if (txt_lineActive[i]) ofSetColor(c0);
//		else ofSetColor(c0_Ghost);
//		fontBig.drawString(t1, x + sp, y + sp);
//
//		if (txt_lineActive[i]) ofSetColor(c1);
//		else ofSetColor(c1_Ghost);
//		fontBig.drawString(t1, x, y);
//
//		padh = -10;
//		h = fontBig.getSize() + padh;
//		y += h;
//	}
//	i++;
//
//	//theory name
//	//if (t2 != "" || txt_lineActive[1])
//	if (txt_lineActive[2])
//	{
//		x = ofGetWidth() * 0.5 - _w2 * 0.5;
//		if (txt_lineActive[i]) ofSetColor(c0);
//		else ofSetColor(c0_Ghost);
//		fontMedium.drawString(t2, x + sp, y + sp);
//
//		if (txt_lineActive[i]) ofSetColor(c1);
//		else ofSetColor(c1_Ghost);
//		fontMedium.drawString(t2, x, y);
//
//		padh = 20;
//		h = fontMedium.getSize() + padh;
//		y += h;
//	}
//	i++;
//
//	//range name
//	//if (t3 != "" || txt_lineActive[3])
//	if (txt_lineActive[3])
//	{
//		x = ofGetWidth() * 0.5 - _w3 * 0.5;
//		if (txt_lineActive[i]) ofSetColor(c0);
//		else ofSetColor(c0_Ghost);
//		fontSmall.drawString(t3, x + sp, y + sp);
//
//		if (txt_lineActive[i]) ofSetColor(c1);
//		else ofSetColor(c1_Ghost);
//		fontSmall.drawString(t3, x, y);
//	}
//
//	//-
//
//	ofPopStyle();
//	ofPopMatrix();
//}

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
			ofClear(color_BackGround.get());
		}

		//--

		//debug
		if (0)
		{
			ofDrawBitmapStringHighlight("last_Index_Theory: " + ofToString(last_Index_Theory), 20, 20);
		}

		//--

		// DEMO2
		float _w = DEMO2_Svg.getWidth();
		if (DEMO2_Test) DEMO2_Svg.draw();
		//if (DEMO2_Test) DEMO2_Svg.draw(glm::vec2(ofGetWidth() * 0.5 - _w * 0.5, 0));

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

	if (SHOW_Gradient) gradientEngine.draw();

	//--

	// gui

	if (SHOW_ImGui)
	{
		if (guiVisible)
		{
			mouseOverGui = draw_Gui();
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
			}
		}
	}

	//----

	// help info
	if (ENABLE_HelpInfo)
	{
		std::string str = helpInfo;
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


		//--

#ifdef LINK_TCP_MASTER_CLIENT
		drawLink();
#endif
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

	// app state
	ofxSurfingHelpers::saveGroup(params_AppState, path_AppState);

	//----

	color_Picked.removeListener(this, &ofxColorManager::Changed_ColorPicked);
	color_Clicked.removeListener(this, &ofxColorManager::Changed_ColorClicked);

	ofRemoveListener(params_control.parameterChangedE(), this, &ofxColorManager::Changed_Controls);
	ofRemoveListener(params_color.parameterChangedE(), this, &ofxColorManager::Changed_Controls);
	ofRemoveListener(params_Theory.parameterChangedE(), this, &ofxColorManager::Changed_Controls);
	//ofRemoveListener(params_GradientC.parameterChangedE(), this, &ofxColorManager::Changed_Controls);

	ofRemoveListener(params_Ranges.parameterChangedE(), this, &ofxColorManager::Changed_Range);

	ofRemoveListener(params_ColorTheory_G1.parameterChangedE(), this, &ofxColorManager::Changed_ColorTheory);
	ofRemoveListener(params_ColorTheory_G2.parameterChangedE(), this, &ofxColorManager::Changed_ColorTheory);

	ofRemoveListener(params_Palette.parameterChangedE(), this, &ofxColorManager::Changed_ParamsPalette);

	//--

	ofRemoveListener(ofEvents().update, this, &ofxColorManager::update);
#ifdef AUTO_DRAW_CALLBACK
	ofRemoveListener(ofEvents().draw, this, &ofxColorManager::draw);
#endif

	removeKeysListeners();
	removeMouseListeners();

	ofRemoveListener(ofEvents().mouseDragged, this, &ofxColorManager::mouseDragged);
	ofRemoveListener(ofEvents().mouseScrolled, this, &ofxColorManager::mouseScrolled);
}

//----

// engines

//--------------------------------------------------------------
void ofxColorManager::gui_Theory()
{
	static bool auto_resize = true;
	float ww, hh;
	ww = PANEL_WIDGETS_WIDTH;
	hh = PANEL_WIDGETS_HEIGHT;
	ImGui::SetWindowSize(ImVec2(ww, hh));
	ImGuiWindowFlags flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;

	//--

	if (ofxImGui::BeginWindow("THEORY", mainSettings, flags))
	{
		//--

		// box size
		static int _cSize = 37;

		float _spc = ImGui::GetStyle().ItemInnerSpacing.x;
		float _w = ImGui::GetWindowContentRegionWidth() - _spc;
		int _h = int(COLOR_STRIP_COLOR_HEIGHT);
		float _w50 = MAX(150, _w * 0.33);

		const float butlabelw = 150;

		//-

		ImGuiColorEditFlags _flags;

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
		bool bUpdate = false;

		//----

		//mini box
		if (ImGui::ColorButton("##BoxTheory", *(ImVec4 *)&tmpRef.r, _flags, ImVec2(_w, _h)))
		{
		}

		//-

		// two pickers

		if (ImGui::CollapsingHeader("BASE COLOR"))
		{
			ImGui::Columns(2, NULL, false);

			ImGui::PushItemWidth(_w50);

			// 1. wheel
			if (ImGui::ColorPicker3("##PickerTheory", &tmpRef.r, _flags))
			{
				ofLogNotice(__FUNCTION__) << "Wheel Picker Theory : moved";

				color_TheoryBase.set(tmpRef);

				if (bAuto_Theory_FromPicker) color_Picked.set(color_TheoryBase.get());

				bUpdate = true;

				//-
			}

			ImGui::PopItemWidth();

			//--

			// 2. square

			_flags =
				ImGuiColorEditFlags_NoSmallPreview |
				ImGuiColorEditFlags_NoInputs |
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

					if (bAuto_Theory_FromPicker) color_Picked = color_TheoryBase.get();

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

		// amount colors
		if (ofxSurfingHelpers::AddIntStepped(numColors_Engines))
		{
		}

		if (ImGui::CollapsingHeader("Advanced"))
		{
			ofxImGui::AddParameter(bAuto_Theory_FromPicker);
			ofxImGui::AddParameter(bAuto_Build_Palette);
			//ofxImGui::AddParameter(analogSpread);

			//// layout
			//ImGui::Dummy(ImVec2(0, 5));
			//ImGui::Checkbox("Auto-resize", &auto_resize);
		}

		//--

		ImGui::Dummy(ImVec2(0, 5));

		//----

		ImGuiColorEditFlags _flagsc;
		_flagsc =
			ImGuiColorEditFlags_NoAlpha |
			ImGuiColorEditFlags_NoPicker |
			ImGuiColorEditFlags_NoTooltip;

		// 1. G1

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

			// label button G1

			//std::string _label = ColorWheelSchemes::colorSchemeNames[i];
			if (ofxSurfingHelpers::AddSmallButton(theory_Types_G1[i], butlabelw, _cSize))
			{
				last_Index_Theory_PickPalette = i;
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

			// colors G1 

			int _total = colors_Theory_G1[i].size();

			for (int n = 0; n < _total; n++)
			{
				ofColor _c = colors_Theory_G1[i][n];
				ImGui::PushID(n);
				ImGui::SameLine();

				//we relayout sizes 
				//when not expected amount of colors, due to different theory types: 
				//ie: even allways
				float _szw, _szh;
				_szh = _cSize;
				float _ww = (numColors_Engines * _cSize) + (numColors_Engines * _spc) - _spc;//exact used zone
				if (_total == numColors_Engines)//total is the effective amount
				{
					_szw = _cSize;
				}
				else if (_total > numColors_Engines)
				{
					_szw = _ww / _total - _spc;
				}
				else if (_total < numColors_Engines)
				{
					_szw = _ww / _total - _spc;
				}

				if (ImGui::ColorButton("##ColorButtonTheory_G1",
					_c,
					_flagsc,
					ImVec2(_szw, _szh)))
				{
					//TODO: not using these pickers
					//color_Picked.set(_c);
				}

				ImGui::PopID();
			}
		}

		//----

		// 2. G2

		for (int i = 0; i < NUM_COLOR_THEORY_TYPES_G2; i++)
		{
			int _total = 0;
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
			if ((NUM_COLOR_THEORY_TYPES_G1 + i) == last_Index_Theory)
			{
				bDrawBorder = true;
				ImGui::PushStyleColor(ImGuiCol_Border, color_Pick);
				ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, linew_Pick);
			}

			//-

			// label button G2

			if (ofxSurfingHelpers::AddSmallButton(types_Theory_G2[i], butlabelw, _cSize))
			{
				last_Index_Theory_PickPalette = NUM_COLOR_THEORY_TYPES_G1 + i;
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
				ofColor _c;
				ImGui::PushID(n);
				ImGui::SameLine();

				std::string _name;
				switch (i)
				{
				case 0:
					if (n < complement.size()) _c = complement[n];
					_name = "Complement";
					break;
				case 1:
					if (n < complementBrightness.size()) _c = complementBrightness[n];
					_name = "Complement Brigthness";
					break;
				case 2:
					if (n < monochrome.size()) _c = monochrome[n];
					_name = "Monochrome";
					break;
				case 3:
					if (n < monochromeBrightness.size()) _c = monochromeBrightness[n];
					_name = "Monochrome Brightness";
					break;
				case 4:
					if (n < analogue.size()) _c = analogue[n];
					_name = "Analogue";
					break;
				case 5:
					if (n < triad.size()) _c = triad[n];
					_name = "Triad";
					break;
				case 6:
					if (n < complementTriad.size()) _c = complementTriad[n];
					_name = "Complement Triad";
					break;
				}

				//we relayout sizes 
				//when not expected amount of colors, due to different theory types: 
				//ie: even allways
				float _szw, _szh;
				_szh = _cSize;
				float _ww = (numColors_Engines * _cSize) + (numColors_Engines * _spc) - _spc;//exact used zone
				if (_total == numColors_Engines)//total is the effective amount
				{
					_szw = _cSize;
				}
				else if (_total > numColors_Engines)
				{
					_szw = _ww / _total - _spc;
				}
				else if (_total < numColors_Engines)
				{
					_szw = _ww / _total - _spc;
				}

				// colors G2 

				if (ImGui::ColorButton("##ColorButtonTheory_G2",
					_c,
					_flagsc,
					ImVec2(_szw, _szh)))
				{
					//TODO: not using these pickers
					//color_Picked.set(_c);
				}

				ImGui::PopID();
			}
		}

		//----

		if (last_Index_Theory == 12) ofxImGui::AddParameter(analogSpread);

		//----

		//TODO:
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
	static bool auto_resize = true;

	float ww, hh;
	ww = PANEL_WIDGETS_WIDTH;
	hh = PANEL_WIDGETS_HEIGHT;
	ImGui::SetWindowSize(ImVec2(ww, hh));
	ImGuiWindowFlags flagsw;
	flagsw = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;

	//-

	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(PANEL_WIDGETS_WIDTH, PANEL_WIDGETS_HEIGHT));

	if (ofxImGui::BeginWindow("PALETTE EDITOR", mainSettings, flagsw))
	{
		float _spc = ImGui::GetStyle().ItemInnerSpacing.x;
		float _w = ImGui::GetWindowContentRegionWidth() - 2 * _spc;
		float _w50 = _w * 0.48;
		float _h = BUTTON_BIG_HEIGHT;

		ImGuiColorEditFlags _flags = ImGuiColorEditFlags_None;

		//--

		//if (SHOW_PresetsPalette)
		//if (ImGui::CollapsingHeader("Palette", _flagw))
		{
			//responsive
			ImVec2 button_sz((float)sizePaletteBox.get(), (float)sizePaletteBox.get());
			ImGuiStyle& style = ImGui::GetStyle();
			int _amtBtn = palette.size();
			float _wx2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

			//--

			enum Mode
			{
				Mode_Copy,
				Mode_Swap
			};
			static int mode = 1;

			//--

			// all color boxes
			for (int n = 0; n < palette.size(); n++)
			{
				ImGui::PushID(n);
				{
					// a. fixed size
					int _max = 6;//amount colors per row
					int _r = (int)MIN(_max, palette.size());
					//split in rows
					if ((n % _r) != 0)
					{
						if (n != 0) ImGui::SameLine();
					}

					//----

					float wb;
					wb = (_w / _r) - (2.0f * _spc);
					//wb = (_w / _r) - (1.5 * _spc);
					//if (!bResponsive_Presets) wb = wb * scale_ColPalette.get();

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
					if (n == last_Index_ColorPalette && bEditPalette)
					{
						bDrawBorder = true;
						ImGui::PushStyleColor(ImGuiCol_Border, color_Pick);
						ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, linew_Pick);
					}

					//-

					if (ImGui::ColorButton("##paletteDragEditor",
						palette[n],
						_flags,
						bb))
					{
						last_Index_ColorPalette = n;

						// workflow
						if (!bEditPalette) bEditPalette = true;
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
								DEMO2_Svg.setPaletteColors(palette);
								last_Index_ColorPalette = n;

								refresh_Background();
							}
							if (mode == Mode_Swap)
							{
								const ofColor tmp = palette[n];
								palette[n] = palette[payload_n];
								palette[payload_n] = tmp;
								DEMO2_Svg.setPaletteColors(palette);
								last_Index_ColorPalette = n;

								refresh_Background();
							}

							//--

							// update all pallete (Curve)
							ofLogNotice(__FUNCTION__) << "DONE Dragged";

							gradientEngine.buildFrom_TARGET();
						}
						ImGui::EndDragDropTarget();
					}

					//----
				}
				ImGui::PopID();
			}

			ImGui::Dummy(ImVec2(0, 5));

			//----

			// edit

			//ofxSurfingHelpers::AddBigToggle(bEditPalette, ww, _h * 0.5);
			ofxSurfingHelpers::AddBigToggle(bEditPalette, _w, _h * 0.5);

			if (bEditPalette)
			{
				ImGui::Dummy(ImVec2(0, 5));
				{
					ofxSurfingHelpers::AddBigButton(bAddColor, _w50, _h * 0.5); ImGui::SameLine();
					ofxSurfingHelpers::AddBigButton(bRemoveColor, _w50, _h * 0.5);
					ofxSurfingHelpers::AddBigToggle(bClearPalette, _w50, _h * 0.5);

					if (bColor_HUE || bColor_SAT || bColor_BRG)
					{
						ImGui::SameLine();
						ofxSurfingHelpers::AddBigButton(bRandomColor, _w50, _h * 0.5);
					}
				}

				//--

				ImGui::Dummy(ImVec2(0, 5));
				{
					//flip
					if (ofxSurfingHelpers::AddSmallButton(bFlipUserPalette, _w, 0.5 * BUTTON_BIG_HEIGHT))
					{
					}
					//ImGui::SameLine();

					//re arrenge
					if (ImGui::Button("RANDOM SORT", ImVec2(_w, 0.5 * BUTTON_BIG_HEIGHT)))
					{
						ENABLE_Callbacks_Engines = false;

						//auto rng = std::default_random_engine{};
						//std::shuffle(std::begin(palette), std::end(palette), rng);
						srand(unsigned(time(NULL)));
						std::shuffle(palette.begin(), palette.end(), std::random_device());
						DEMO2_Svg.setPaletteColors(palette);
						gradientEngine.build_FromPaleletteRef(palette);
						last_Index_ColorPalette = 0;

						ENABLE_Callbacks_Engines = true;
					}

					if (ImGui::RadioButton("Copy", mode == Mode_Copy)) { mode = Mode_Copy; } ImGui::SameLine();
					if (ImGui::RadioButton("Swap", mode == Mode_Swap)) { mode = Mode_Swap; }
				}
			}

			//--

			//ImGui::Separator();
			//ImGui::Dummy(ImVec2(0, 10));

			//ImGui::Text("Floating Panel:");
			//ImGui::Dummy(ImVec2(0, 5));

			// show floating palette
			ofxSurfingHelpers::AddBigToggle(SHOW_UserPaletteFloating, _w, _h * 0.5);

			if (SHOW_UserPaletteFloating)
			{
				if (ImGui::CollapsingHeader("Layout"))
				{
					ofxImGui::AddParameter(bResponsive_Presets);

					if (bResponsive_Presets)
					{
						ImGui::InputInt(sizePaletteBox.getName().c_str(), (int*)&sizePaletteBox.get(), 1, 100);
					}
					if (!bResponsive_Presets)
					{
						boxMaxRows.disableEvents();
						ImGui::InputInt(boxMaxRows.getName().c_str(), (int*)&boxMaxRows.get(), 1, 5);
						boxMaxRows = ofClamp(boxMaxRows.get(), boxMaxRows.getMin(), boxMaxRows.getMax());
						boxMaxRows.enableEvents();

						ImGui::InputFloat(scale_ColPalette.getName().c_str(), (float *)&scale_ColPalette.get(), 0.005f, 0.010);
					}
				}
			}

			//ofxImGui::AddParameter(bAutoResizePalette);//not works
			ImGui::Checkbox("Auto-resize", &auto_resize);
		}
	}
	ofxImGui::EndWindow(mainSettings);

	ImGui::PopStyleVar();
}

//--------------------------------------------------------------
void ofxColorManager::gui_PaletteFloating()
{
	static bool auto_resize = false;
	//static bool auto_resize = true;
	//bool auto_resize = bAutoResizePalette.get();

	float ww, hh;
	ww = PANEL_WIDGETS_WIDTH;
	hh = PANEL_WIDGETS_HEIGHT;
	ImGui::SetWindowSize(ImVec2(ww, hh));
	ImGuiWindowFlags flagsw;
	flagsw = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;
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
		int _amtBtn = palette.size();
		float _wx2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
		//--

		enum Mode
		{
			Mode_Copy,
			//Mode_Move,
			Mode_Swap
		};
		static int mode = 1;

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
			if (!bResponsive_Presets) wb = wb * scale_ColPalette.get();

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

			//-

			// border to selected
			bool bDrawBorder = false;
			if (n == last_Index_ColorPalette && bEditPalette)
			{
				bDrawBorder = true;
				ImGui::PushStyleColor(ImGuiCol_Border, color_Pick);
				ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, linew_Pick);
			}

			//-

			if (ImGui::ColorButton("##paletteDragPalette",
				palette[n],
				_flags,
				bb))
			{
				last_Index_ColorPalette = n;

				// workflow
				if (!bEditPalette) bEditPalette = true;
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
				//if (mode == Mode_Move) { ImGui::Text("Move %s", ofToString(n).c_str()); }
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
						DEMO2_Svg.setPaletteColors(palette);

						refresh_Background();
					}
					//if (mode == Mode_Move)
					//{
					//	palette[n] = palette[payload_n];
					//	palette[payload_n] = ofColor(0);
					//	DEMO2_Svg.setPaletteColors(palette);
					//}
					if (mode == Mode_Swap)
					{
						const ofColor tmp = palette[n];
						palette[n] = palette[payload_n];
						palette[payload_n] = tmp;
						DEMO2_Svg.setPaletteColors(palette);

						refresh_Background();
					}

					//--

					// update all pallete (Curve)
					ofLogNotice(__FUNCTION__) << "DONE Dragged _c";

					gradientEngine.buildFrom_TARGET();
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
				if (n + 1 < _amtBtn && next_button_x2 < _wx2) ImGui::SameLine();
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

		// 1. lib name (ie: pantone)
		s = colorBrowser.getNameLib();
		ImGui::Text(s.c_str());

		ImGui::SameLine();

		// 3. index/total
		s = "  " + ofToString(last_Lib_Index) + "/" + ofToString(lib_TotalColors - 1);
		ImGui::Text(s.c_str());

		//ImGui::Dummy(ImVec2(0, 5));

		// 2. color name
		ImGui::Text(last_Lib_NameColor.c_str());

		//ImGui::Dummy(ImVec2(0, 5));

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

		//ImGui::Dummy(ImVec2(0, 5));

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

			//-

			// page
			ImGui::SameLine();
			ImGui::PushItemWidth(_w * 0.5);
			ofxImGui::AddParameter(lib_Page_Index);//page slider selector
			//ImGui::SliderInt("PAGE", &lib_Page_Index, 0, lib_Page_Max);//page slider selector
			//ImGui::DragInt("PAGE", (int *)&lib_Page_Index, 0, lib_Page_Max);//collide..
			ImGui::PopItemWidth();

			lib_Page_Index = ofClamp(lib_Page_Index, 0, lib_Page_Index.getMax());
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
	flagsw = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;

	//--

	if (ofxImGui::BeginWindow("Live Export", mainSettings, flagsw))
	{
		float _spc = ImGui::GetStyle().ItemInnerSpacing.x;

		//float _h = 0.5f * BUTTON_BIG_HEIGHT;
		//float _h = float(COLOR_STRIP_COLOR_HEIGHT);
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
			if (ImGui::Button("EXPORT", ImVec2(_w, BUTTON_BIG_HEIGHT)))
			{
				exportPalette();
			}
			//ImGui::PopItemWidth();
#ifndef USE_SIMPLE_PRESET_PALETTE	
			ofxSurfingHelpers::AddBigToggle(bModeBundlePreset, _w, BUTTON_BIG_HEIGHT / 2);
			ofxSurfingHelpers::AddBigToggle(bModePalettePreset, _w, BUTTON_BIG_HEIGHT / 2);
#endif
			ofxSurfingHelpers::AddBigToggle(bAutoExportPreset, _w, BUTTON_BIG_HEIGHT / 2);
			//ofxImGui::AddParameter(bAutoExportPreset);

			if (ImGui::CollapsingHeader("Advanced"))
			{
				ofxImGui::AddParameter(bExportPreset_DefaultPath);
				if (!bExportPreset_DefaultPath)
					if (ImGui::Button("Set Path", ImVec2(_w, 0.5*BUTTON_BIG_HEIGHT)))
					{
						bOpen = true;
					}

				ImGui::Dummy(ImVec2(0, 2));
				std::string ss;

				if (!bExportPreset_DefaultPath) {
					ss = path_Folder_ExportColor_Custom.get();
				}
				else {
					ss = "/data/" + path_Folder_ExportColor_Data.get();
					//ImGui::Text(path_Folder_ExportColor_Data.get().c_str());
				}
				//ImGui::Text("Path:");
				//ImGui::Text(ss.c_str());

				//floating
				ImGui::Text("Path:");
				ImGui::SameLine();
				ImGui::TextDisabled("(?)");
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
					ImGui::TextUnformatted(ss.c_str());
					ImGui::PopTextWrapPos();
					ImGui::EndTooltip();
				}


				ImGui::Text("Names:");
				ImGui::Dummy(ImVec2(0, 2));

				ImGui::Text("Preset:");
				ImGui::Text(PRESET_Temp.name.c_str());

				//#ifdef USE_SIMPLE_PRESET_PALETTE
				//				std::string _path = path_FileExport + "_Palette.json";
				//				ImGui::Text(_path.c_str());//hardcoded
				//#endif

								//ImGui::Dummy(ImVec2(0, 2));
								//ImGui::Text("Gradient:");
								//ImGui::Text(gradientEngine.path_Name_Gradient.c_str());
								////ImGui::Text(path_Name_Gradient.c_str());

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
	flagsw = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;

	//--

	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(PANEL_WIDGETS_WIDTH, PANEL_WIDGETS_HEIGHT));

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

		bool bChg_Pick = false;//to concentrate callbacks once
		bool bChg_HSB = false;//to concentrate callbacks once

		//--

		// 0. box color

		ImGuiColorEditFlags _flags = ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoTooltip;

		ImGui::ColorButton("##PickerBox", *(ImVec4 *)&cTmp, _flags, ImVec2(_w, _h));

		//-

		if (ofxImGui::BeginTree("COLOR PICKERS", mainSettings))//grouped folder
		{
			ImGuiColorEditFlags _flagw;

			//--

			// 1. color picker

			//_flagw = ImGuiTreeNodeFlags_None;
			_flagw = ImGuiTreeNodeFlags_DefaultOpen;

			if (ImGui::CollapsingHeader("WHEEL", _flagw))
			{
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

					bChg_Pick = true;
				}

				ImGui::PopItemWidth();
			}

			//--

			// 2. square

			//_flagw = ImGuiTreeNodeFlags_DefaultOpen;// open
			_flagw = ImGuiTreeNodeFlags_None;// closed

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

					bChg_Pick = true;
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
			ImGui::PushItemWidth(_w * 0.9);

			if (ofxImGui::AddParameter(color_HUE))
			{
			}
			if (ofxImGui::AddParameter(color_SAT))
			{
			}
			if (ofxImGui::AddParameter(color_BRG))
			{
			}

			//--

			ImGui::PopItemWidth();

			ofxImGui::EndTree(mainSettings);
		}

		//--

		if (ImGui::CollapsingHeader("Randomizer"))
		{
			if (bColor_HUE || bColor_SAT || bColor_BRG)
				ofxSurfingHelpers::AddBigButton(bRandomColor, _w, 2 * _h);

			if (bColor_HUE) color_HUE_0 = color_HUE;
			if (bColor_SAT) color_SAT_0 = color_SAT;
			if (bColor_BRG) color_BRG_0 = color_BRG;

			{ if (ofxImGui::AddParameter(bColor_HUE)) {}ImGui::SameLine(); }
			{ if (ofxImGui::AddParameter(bColor_SAT)) {}ImGui::SameLine(); }
			{ if (ofxImGui::AddParameter(bColor_BRG)) {} }

			if (bColor_HUE)
				if (ofxImGui::AddParameter(color_HUE_Power))
				{
					ofLogNotice(__FUNCTION__) << "HUE P : moved";
				}

			if (bColor_SAT)
				if (ofxImGui::AddParameter(color_SAT_Power))
				{
					ofLogNotice(__FUNCTION__) << "SAT P : moved";
				}

			if (bColor_BRG)
				if (ofxImGui::AddParameter(color_BRG_Power))
				{
					ofLogNotice(__FUNCTION__) << "BRG P : moved";
				}
		}

		//----

		//to concentrate callbacks once

		if (bChg_Pick)
		{
			color_Picked = cTmp;
		}

		//----

		//if (ImGui::CollapsingHeader("Advanced"))
		//{
		//	ImGui::Checkbox("Auto-resize", &auto_resize);
		//}
	}
	ofxImGui::EndWindow(mainSettings);

	ImGui::PopStyleVar();
}

//--------------------------------------------------------------
void ofxColorManager::gui_Panels()
{
	ImGuiWindowFlags flags;
	flags = ImGuiWindowFlags_None;

	//static bool auto_resize = true;
	//flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;//TODO: not working for my toggles

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

		ImGui::Columns(4);

		ofxSurfingHelpers::AddBigToggle(SHOW_Presets);
		ofxSurfingHelpers::AddBigToggle(SHOW_UserPaletteFloating);
		ofxSurfingHelpers::AddBigToggle(SHOW_UserPaletteEditor);
		ofxSurfingHelpers::AddBigToggle(SHOW_Gradient);

		ImGui::NextColumn();

		ofxSurfingHelpers::AddBigToggle(SHOW_Picker);
		ofxSurfingHelpers::AddBigToggle(SHOW_BackGround);
		ofxSurfingHelpers::AddBigToggle(SHOW_Library);

		ImGui::NextColumn();
		//ImGui::Separator();

		ofxSurfingHelpers::AddBigToggle(SHOW_Theory);
		ofxSurfingHelpers::AddBigToggle(SHOW_Range);
		ofxSurfingHelpers::AddBigToggle(SHOW_ColourLovers);
		ofxSurfingHelpers::AddBigToggle(SHOW_Quantizer);

		ImGui::NextColumn();
		//ImGui::Separator();

		ofxSurfingHelpers::AddBigToggle(SHOW_MINI_Preview);
		ofxSurfingHelpers::AddBigToggle(SHOW_Demos);
		ofxSurfingHelpers::AddBigToggle(SHOW_Export);
		ofxImGui::AddParameter(ENABLE_keys);
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
	ImGuiWindowFlags flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;

	//--

	if (ofxImGui::BeginWindow("RANGE", mainSettings, flags))
	{
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
				ImGui::Text("Color 1");
				ofxSurfingHelpers::AddBigToggle(bAuto_Color1_FromPicker_Range, _w50, BUTTON_BIG_HEIGHT);
				//ofxImGui::AddParameter(bAuto_Color1_FromPicker_Range);
				// mini box
				if (ImGui::ColorButton("##C1", *(ImVec4 *)&_c1[0], _flags, ImVec2(_w50, _h)))
				{
					bChanged = true;
					refresh_Range_AutoUpdate();
				}
				ImGui::PopItemWidth();

				ImGui::NextColumn();

				ImGui::PushItemWidth(_w50);
				ImGui::Text("Color 2");
				ofxSurfingHelpers::AddBigToggle(bAuto_Color2_FromPicker_Range, _w50, BUTTON_BIG_HEIGHT);
				//ofxImGui::AddParameter(bAuto_Color2_FromPicker_Range);
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
					if (bAuto_Color1_FromPicker_Range) color_Picked.set(_c1[0]);
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
					if (bAuto_Color2_FromPicker_Range) color_Picked.set(_c2[0]);
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

				// workflow
				//get main color from range picker
				if (bAuto_Color1_FromPicker_Range) color_Picked.set(color_1_Range.get());
				else if (bAuto_Color2_FromPicker_Range) color_Picked.set(color_2_Range.get());
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

			//ofxSurfingHelpers::AddSmallButton(bGetPalette_From_Range, 150, 30);

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

				if (ofxSurfingHelpers::AddSmallButton(types_Range[t], _szLabel, _sz))
				{
					//ofLogNotice(__FUNCTION__) << types_Range[t].getName() << " #" << t;// ? allways triggering..
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
void ofxColorManager::gui_Background()
{
	static bool auto_resize = true;
	float ww, hh;
	ww = PANEL_WIDGETS_WIDTH;
	hh = PANEL_WIDGETS_HEIGHT;
	ImGui::SetWindowSize(ImVec2(ww, hh));
	ImGuiWindowFlags flagsw = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;

	//--

	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(PANEL_WIDGETS_WIDTH, PANEL_WIDGETS_HEIGHT));

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

		bool bChanged = false;
		static ImVec4 color;
		color = color_BackGround.get();

		//-

		{
			//ImGui::Dummy(ImVec2(0, 5));

			//-

			// 0. color big

			float _w = ImGui::GetWindowContentRegionWidth();
			int _h = int(COLOR_STRIP_COLOR_HEIGHT);

			ImGuiColorEditFlags _flags;
			_flags = ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoTooltip;

			if (ImGui::ColorButton("##ColorBgPicker", *(ImVec4 *)&color, _flags, ImVec2(_w, _h)))
			{
				bChanged = true;
			}

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

			if (ImGui::ColorPicker4("Background Color", (float *)&color, _flags))
			{
				bChanged = true;
			}

			ImGui::PopItemWidth();

			ImGui::Dummy(ImVec2(0, 5));

			//-

			if (ImGui::CollapsingHeader("Advanced"))
			{
				ImGui::PushItemWidth(_w50);

				ofxImGui::AddParameter(background_Draw_ENABLE);
				ofxImGui::AddParameter(color_BackGround_Lock);

				if (!color_BackGround_Lock)
				{
					ofxImGui::AddParameter(AutoSet_BackGround_Color);

					//TODO:
					if (AutoSet_BackGround_Color)
					{
						ofxSurfingHelpers::AddBigToggle(color_BackGround_DarkerMode);
						ofxSurfingHelpers::AddBigToggle(color_BackGround_GradientMode);

						if (color_BackGround_DarkerMode)
						{
							ofxImGui::AddParameter(color_BackGround_AmtDarker);
						}
						if (color_BackGround_GradientMode)
						{
							//ImGui::Text("From Gradient:");
							ofxImGui::AddParameter(gradientEngine.pickIn);
						}
					}
				}
				ofxImGui::AddParameter(color_backGround_SET);

				ImGui::PopItemWidth();

				//ImGui::Dummy(ImVec2(0, 5));
				//ImGui::Checkbox("Auto-resize", &auto_resize);
			}

			//-

			//TODO: 
			//? should update only when changed
			if (bChanged) color_BackGround = color;
		}
	}
	ofxImGui::EndWindow(mainSettings);

	ImGui::PopStyleVar();
}

//--------------------------------------------------------------
void ofxColorManager::gui_Presets()
{
	////static int t_auto_pilot = ofGetEll;
	//static bool auto_pilot = false;
	//bool bPilotDo = false;

	static bool auto_resize = true;
	float ww, hh;
	ww = PANEL_WIDGETS_WIDTH;
	hh = PANEL_WIDGETS_HEIGHT;
	ImGui::SetWindowSize(ImVec2(ww, hh));//not doing nothing
	ImGuiWindowFlags flags;
	flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;

	//--

	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(PANEL_WIDGETS_WIDTH, PANEL_WIDGETS_HEIGHT));

	if (ofxImGui::BeginWindow("PRESETS", mainSettings, flags))
	{
		float _spc = ImGui::GetStyle().ItemInnerSpacing.x;
		float _w100 = ImGui::GetWindowContentRegionWidth();
		//float _w99 = _w100 - 10;
		float _w99 = _w100 - _spc;
		float _w50 = _w99 * 0.5 - 2 * _spc;
		float _h = BUTTON_BIG_HEIGHT;

		ImGuiColorEditFlags _flags;

		//--

		// workflow

		//blink when a new preset is editing
		float freq = 0.075f;//speed freq
		float min = 0.20;
		float max = 0.60;
		float a = ofMap(glm::sin(freq * ofGetFrameNum()), -1, 1, min, max);

		//-

		// palette colors mini preview
		// auto browse presets. to testing and auto export
		if (ofxImGui::AddParameter(auto_pilot))
		{

		}
		if (auto_pilot) ofxImGui::AddParameter(auto_pilot_Duration);

		ImGui::Dummy(ImVec2(0, 5));

		ofxSurfingHelpers::AddBigToggle(SHOW_UserPaletteFloating, _w99, _h * 0.5);
		ofxSurfingHelpers::AddBigToggle(SHOW_UserPaletteEditor, _w99, _h * 0.5);

		//ImGui::Text("Name");
		ImGui::Dummy(ImVec2(0, 5));

		ImGui::Text(PRESET_Name.c_str());

		//----

		//ImGui::Dummy(ImVec2(0, 5));

		int counter = last_Index_Preset.get();
		//static int counter = last_Index_Preset;

		//----

		// index preset / total
		int numPalettes = files_Names.size() - 1;
		ImGui::Text("%d/%d", counter, numPalettes);
		//ImGui::Text("%d/%d", last_Index_Preset.get(), numPalettes);

		ImGui::Dummy(ImVec2(0, 5));

		ImGui::PushButtonRepeat(true);

		if (ImGui::Button("Previous", ImVec2(_w50, _h)))
		{
			if (files.size() > 0)
			{
				if (counter > 0)
				{
					counter--;
					last_Index_Preset = counter;

				}
				else
				{
					if (counter == 0)
					{
						counter = files.size() - 1;
						last_Index_Preset = counter;
					}
				}
				counter = ofClamp(counter, 0, last_Index_Preset.getMax());

				if (last_Index_Preset < files.size() && last_Index_Preset >= 0)
				{
					PRESET_Name = files_Names[last_Index_Preset];
					ofLogNotice(__FUNCTION__) << "PRESET: [" + ofToString(last_Index_Preset) + "] " << PRESET_Name;

					preset_Load(PRESET_Name);
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
					last_Index_Preset = counter;
				}
				else {//cycle
					counter = 0;
					last_Index_Preset = counter;
				}
				counter = ofClamp(counter, 0, last_Index_Preset.getMax());

				if (last_Index_Preset < files.size() && last_Index_Preset >= 0)
				{
					PRESET_Name = files_Names[last_Index_Preset];
					ofLogNotice(__FUNCTION__) << "PRESET: [" + ofToString(last_Index_Preset) + "] " << PRESET_Name;

					preset_Load(PRESET_Name);
				}
			}
		}

		ImGui::PopButtonRepeat();

		ImGui::Dummy(ImVec2(0, 5));

		//----

		// scrollable list

		if (!files_Names.empty())
		{
			int _i = last_Index_Preset;

			ImGui::PushItemWidth(_w99 - 10);

			if (ofxImGui::VectorCombo(" ", &_i, files_Names))
			{
				ofLogNotice(__FUNCTION__) << "_i: " << ofToString(_i);

				if (_i < files_Names.size())
				{
					last_Index_Preset = _i;
					PRESET_Name = files_Names[last_Index_Preset];
					ofLogNotice(__FUNCTION__) << "PRESET_Name: " << PRESET_Name;

					preset_Load(PRESET_Name);
				}

				if (MODE_NewPreset) MODE_NewPreset = false;
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

			ofLogNotice(__FUNCTION__) << "PRESET_name: " << PRESET_Name;

			preset_Save(PRESET_Name);
			preset_RefreshFiles();
		}

		ImGui::SameLine();

		if (ImGui::Button("LOAD", ImVec2(_w50, _h * 0.5)))//not required..
		{
			ofLogNotice(__FUNCTION__) << "LOAD PRESET_Name: " << PRESET_Name;

			preset_Load(PRESET_Name);
		}

		//ImGui::SameLine();

		if (ImGui::Button("DELETE", ImVec2(_w50, _h * 0.5))) ImGui::OpenPopup("DELETE?");
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
					files[last_Index_Preset].remove();
					preset_RefreshFiles();

					ImGui::CloseCurrentPopup();
				}
				ImGui::SetItemDefaultFocus();
				ImGui::SameLine();
				if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			}
			else {
				ofLogNotice(__FUNCTION__) << "DELETE";
				files[last_Index_Preset].remove();
				preset_RefreshFiles();

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		//if (ImGui::Button("DELETE", ImVec2(_w50, _h * 0.5)))//current preset
		//{
		//	ofLogNotice(__FUNCTION__) << "DELETE";
		//	files[last_Index_Preset].remove();
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
				if (ImGui::Button("OK", ImVec2(120, 0)))
				{
					ofLogNotice(__FUNCTION__) << "EXPORT";
					//path_Folder_ExportColor_Custom = ;

					//exportPalette();

					ImGui::CloseCurrentPopup();
				}

				if (ImGui::Button("SET PATH", ImVec2(120, 0)))
				{
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
			//else
			//{
			//	//exportPalette();
			//}

			ImGui::EndPopup();
		}

		if (ImGui::Button("UPDATE", ImVec2(_w50, _h * 0.5)))
		{
			ofLogNotice(__FUNCTION__) << "UPDATE";

			//PRESET_Name = textInput_temp;

			ofLogNotice(__FUNCTION__) << "PRESET_Name: " << PRESET_Name;

			//delete old file
			//files[last_Index_Preset].remove();

			//save new one
			preset_Save(PRESET_Name);
			preset_RefreshFiles();
		}

		ImGui::SameLine();

		if (ImGui::Button("COPY", ImVec2(_w50, _h * 0.5)))
		{
			ofLogNotice(__FUNCTION__) << "COPY";

			textInput_New = PRESET_Name + "_copy";
			//textInput_New = textInput_New + "_copy";

			preset_Save(textInput_New);
			preset_RefreshFiles();

			//--

			refresh_Files(textInput_New);
		}

		if (ImGui::Button("REFRESH KIT", ImVec2(_w50, _h * 0.5)))
		{
			ofLogNotice(__FUNCTION__) << "REFRESH KIT";

			preset_RefreshFiles();
		}

		ImGui::Dummy(ImVec2(0, 5));

		//----

		// new preset 

		ImGui::Separator();

		ImGui::Dummy(ImVec2(0, 5));

		//new preset toggle
		if (ofxSurfingHelpers::AddBigToggle(MODE_NewPreset, _w99, _h))
		{
			//TODO:
			textInput_New = name_TARGET[0];//set
			//name_TARGET[0] = &textInput_New[0];//set
			if (textInput_New == "") textInput_New = "type";
		}
		//ofxImGui::AddParameter(MODE_NewPreset);

		if (MODE_NewPreset.get())
		{
			//ImGui::Dummy(ImVec2(0, 5));

			ImGui::PushItemWidth(_w99);
			{
				// loaded string into char array
				char tab[32];
				strncpy(tab, textInput_New.c_str(), sizeof(tab));
				tab[sizeof(tab) - 1] = 0;

				//-

				if (ImGui::InputText("", tab, IM_ARRAYSIZE(tab)))
				{
					textInput_New = ofToString(tab);

					name_TARGET[0] = &textInput_New[0];

					ofLogNotice(__FUNCTION__) << "textInput_New:" << textInput_New;
				}

				//-

				//TODO:
				//to disable all other key commands
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
			}
			ImGui::PopItemWidth();

			//--

			//ImGui::Dummy(ImVec2(0, 5));

			ImGui::PushID(1);
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.5f, a));
			if (ImGui::Button("SAVE NEW", ImVec2(_w99, _h)))
			{
				has_focus = 0;
				MODE_NewPreset = false;
				if (textInput_New != "")
				{
					ofLogNotice(__FUNCTION__) << "textInput_New: " << textInput_New;

					preset_Save(textInput_New);
					preset_RefreshFiles();

					//--

					refresh_Files(textInput_New);
				}
				else ofLogError(__FUNCTION__) << "Empty name on textInput !";
			}
			ImGui::PopStyleColor(1);
			ImGui::PopID();
		}

		ImGui::Dummy(ImVec2(0, 5));
		ImGui::Separator();

		//----

		ImGui::Dummy(ImVec2(0, 2));

		ofxSurfingHelpers::AddBigToggle(SHOW_Kit, _w99, _h * 0.5);

		if (SHOW_Kit) ofxImGui::AddParameter(AutoScroll);
		//ofxImGui::AddParameter(SHOW_Kit);
		//ofxImGui::AddParameter(SHOW_PresetsPalette);

		ImGui::Dummy(ImVec2(0, 5));

		//--

		//ImGui::Checkbox("Auto-resize", &auto_resize);
	}

	ofxImGui::EndWindow(mainSettings);

	ImGui::PopStyleVar();

	//----

	//add kit palettes browser
	if (SHOW_Kit)
	{
		ww = PANEL_WIDGETS_WIDTH;
		hh = PANEL_WIDGETS_HEIGHT;
		ImGui::SetWindowSize(ImVec2(ww, hh));
		//ImGuiWindowFlags flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;

		//--

		ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(PANEL_WIDGETS_WIDTH, PANEL_WIDGETS_HEIGHT));

		if (ofxImGui::BeginWindow("KIT", mainSettings, flags))
		{
			// populate widgets
			int indexPreset = gui_GridPalettes(kit, last_Index_Preset, AutoScroll.get());

			// workflow
			if (indexPreset != -1)
			{
				last_Index_Preset = indexPreset;
				preset_Load(files_Names[last_Index_Preset]);

				//-

				// DEMO
				if (DEMO1_Test && !DEMO_Auto) myDEMO1.reStart();
			}
		}
		ofxImGui::EndWindow(mainSettings);

		ImGui::PopStyleVar();
	}
}

//--------------------------------------------------------------
void ofxColorManager::gui_Gradient()
{
	static bool auto_resize = true;
	float ww, hh;
	ww = PANEL_WIDGETS_WIDTH;
	hh = PANEL_WIDGETS_HEIGHT;
	//ImGui::SetWindowSize(ImVec2(ww, hh));

	ImGuiWindowFlags flagsw;
	flagsw = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;

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

			ofxSurfingHelpers::AddBigToggle(gradientEngine.SHOW_CurveEditor, _w, _h / 2);

			ImGui::PushItemWidth(_w);
			if (ImGui::Button(gradientEngine.bResetCurve.getName().c_str(), ImVec2(_w, _h)))
			{
				gradientEngine.bResetCurve = true;
			}
			ImGui::PopItemWidth();

			ImGui::Dummy(ImVec2(0, 5));

			ImGui::PushItemWidth(_w20);
			ofxImGui::AddParameter(gradientEngine.gradient_HardMode);
			ofxImGui::AddParameter(gradientEngine.bAutoSet_Background);
			ofxSurfingHelpers::AddBigToggle(gradientEngine.color_BackGround_GradientMode, _w, _h / 2);
			if (ImGui::Button(gradientEngine.bPalettize.getName().c_str(), ImVec2(_w, _h * 0.5)))
			{
				gradientEngine.bPalettize = true;
			}
			//ofxImGui::AddParameter(gradientEngine.bAutoPaletteFromGradient);
			ImGui::PopItemWidth();

			//-

			//ImGui::TreePop();
			ofxImGui::EndTree(mainSettings);
		}

		//ImGui::Dummy(ImVec2(0, 5));

		//--

		// curve Test

		//if (ImGui::TreeNode("CURVE TEST"))
		//_flagw = ImGuiWindowFlags_;
		if (ImGui::CollapsingHeader("Curve TEST"))
			//if (ofxImGui::BeginTree("CURVE TEST", mainSettings))
		{
			ImGui::PushItemWidth(_w50);

			ofxImGui::AddParameter(gradientEngine.bTEST_Enable);
			//ImGui::Checkbox("Enable", &TEST_Mode); 

			if (gradientEngine.bTEST_Enable)
			{
				ImGui::SameLine();
				ImGui::Checkbox("LFO", &gradientEngine.bTEST_LFO_Mode);
			}
			ImGui::SliderFloat("Speed", &gradientEngine.TEST_Speed, 0.0f, 1.0f);

			ImGui::PopItemWidth();

			//-

			//ImGui::TreePop();
			//ofxImGui::EndTree(mainSettings);
		}

		//ImGui::Dummy(ImVec2(0, 5));

		//--

		if (ImGui::CollapsingHeader("Advanced", _flagw))
		{
			// ctrl in/out
			//ImGui::PushItemWidth(_w50);

			ofxImGui::AddParameter(gradientEngine.expTweak);

			//ImGui::Dummy(ImVec2(0, 2));

			if (ofxImGui::AddParameter(gradientEngine.pickIn))
			{
			}

			//ofxImGui::AddParameter(gradientEngine.curveOut);

			//ImGui::PopItemWidth();

			//-

			ImGui::Dummy(ImVec2(0, 5));
			ImGui::PushItemWidth(_w);
			if (ofxSurfingHelpers::AddBigToggle(gradientEngine.bEditLayout, _w, 0.5 * _h))
			{
			}
			//ofxImGui::AddParameter(bEditLayout);
			ImGui::PopItemWidth();

			ImGui::Checkbox("Auto-resize", &auto_resize);
		}
	}
	ofxImGui::EndWindow(mainSettings);
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
	flagsw = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;

	//-

	//static float scale = 1.0f;

	if (ofxImGui::BeginWindow("DEMO", mainSettings, flagsw))
	{
		ofxImGui::AddParameter(DEMO1_Test);
		ofxImGui::AddParameter(DEMO2_Test);
		ofxImGui::AddParameter(DEMO2_Edit);
		//if (ImGui::DragFloat("Scale", &scale, 0.2, 1.0))
		if (ofxImGui::AddParameter(DEMO2_Scale))
		{
			//DEMO2_Svg.setScale(DEMO2_Scale);
		}

		if (DEMO1_Test)
		{
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

#ifdef INCLUDE_IMGUI_CUSTOM_THEME_AND_FONT
	ImGui::PushFont(customFont);
#endif

	//--

	if (SHOW_ALL_GUI)
	{
		if (SHOW_UserPaletteFloating) gui_PaletteFloating();
		if (SHOW_UserPaletteEditor) gui_PaletteEditor();
		if (SHOW_Presets) gui_Presets();
		if (SHOW_Picker) gui_Picker();
		if (SHOW_Library) gui_Library();
		if (SHOW_BackGround) gui_Background();

		if (SHOW_Theory) gui_Theory();
		if (SHOW_Range) gui_Range();
#ifdef USE_COLOR_LOVERS
		if (SHOW_ColourLovers) colourLoversHelper.draw();
#endif
		if (SHOW_Quantizer) colorQuantizer.draw_Gui();
		if (SHOW_Panels) gui_Panels();
		if (SHOW_Export) gui_Export();
		if (SHOW_Demos) gui_Demo();

		if (SHOW_Gradient) gui_Gradient();
	}

	//--

#ifdef INCLUDE_IMGUI_CUSTOM_THEME_AND_FONT
	ImGui::PopFont();
#endif

	gui.end();

	//-

	// 2 different modes:
	// mouse over any panel or over text input only
	if (bCheckMouseOverTextInput) return bTextInputActive;
	else return mainSettings.mouseOverGui;
}

//--------------------------------------------------------------
void ofxColorManager::palette_FromTheory(int p)
{
	ofLogNotice(__FUNCTION__) << p;

	palette_Clear();

	ofColor _c;

	//-

	// 1. G1

	switch (p)
	{
	case 0:
		for (int i = 0; i < colors_Analogous.size(); i++)
		{
			_c = colors_Analogous[i];
			palette_AddColor(_c);
		}
		break;
	case 1:
		for (int i = 0; i < colors_Complementary.size(); i++)
		{
			_c = colors_Complementary[i];
			palette_AddColor(_c);
		}
		break;
	case 2:
		for (int i = 0; i < colors_SplitComplementary.size(); i++)
		{
			_c = colors_SplitComplementary[i];
			palette_AddColor(_c);
		}
		break;
	case 3:
		for (int i = 0; i < colors_Compound.size(); i++)
		{
			_c = colors_Compound[i];
			palette_AddColor(_c);
		}
		break;
	case 4:
		for (int i = 0; i < colors_FlippedCompound.size(); i++)
		{
			_c = colors_FlippedCompound[i];
			palette_AddColor(_c);
		}
		break;
	case 5:
		for (int i = 0; i < colors_Monochrome.size(); i++)
		{
			_c = colors_Monochrome[i];
			palette_AddColor(_c);
		}
		break;
	case 7:
		for (int i = 0; i < colors_Triad.size(); i++)
		{
			_c = colors_Triad[i];
			palette_AddColor(_c);
		}
		break;
	case 6:
		for (int i = 0; i < colors_Tetrad.size(); i++)
		{
			_c = colors_Tetrad[i];
			palette_AddColor(_c);
		}
		break;
	}

	//-

	// 2. G2

	switch (p)
	{
	case 8:
		for (int i = 0; i < complement.size(); i++)
		{
			_c = complement[i];
			palette_AddColor(_c);
		}
		break;
	case 9:
		for (int i = 0; i < complementBrightness.size(); i++)
		{
			_c = complementBrightness[i];
			palette_AddColor(_c);
		}
		break;
	case 10:
		for (int i = 0; i < monochrome.size(); i++)
		{
			_c = monochrome[i];
			palette_AddColor(_c);
		}
		break;
	case 11:
		for (int i = 0; i < monochromeBrightness.size(); i++)
		{
			_c = monochromeBrightness[i];
			palette_AddColor(_c);
		}
		break;
	case 12:
		for (int i = 0; i < analogue.size(); i++)
		{
			_c = analogue[i];
			palette_AddColor(_c);
		}
		break;
	case 13:
		for (int i = 0; i < triad.size(); i++)
		{
			_c = triad[i];
			palette_AddColor(_c);
		}
		break;
	case 14:
		for (int i = 0; i < complementTriad.size(); i++)
		{
			_c = complementTriad[i];
			palette_AddColor(_c);
		}
		break;
	}

	//--

	// workflow
	refresh_Background();

	//--

	//refresh_Palette_TARGET(palette);

	//--

	//// export
	//if (bAutoExportPreset)
	//{
	//	ofLogNotice(__FUNCTION__) << "Auto EXPORT";
	//	exportPalette();
	//}
}

//--------------------------------------------------------------
void ofxColorManager::refresh_Theory_G2()
{
	ofLogNotice(__FUNCTION__);

	// G1

	//// 2.1 clear
	//complement.initGen();
	//complementBrightness.initGen();
	//triad.initGen();
	//complementTriad.initGen();
	//monochrome.initGen();
	//monochromeBrightness.initGen();
	//analogue.initGen();

	// 1. set base color
	complement.setBaseColor(color_TheoryBase);
	complementBrightness.setBaseColor(color_TheoryBase);
	triad.setBaseColor(color_TheoryBase);
	complementTriad.setBaseColor(color_TheoryBase);
	monochrome.setBaseColor(color_TheoryBase);
	monochromeBrightness.setBaseColor(color_TheoryBase);
	analogue.setBaseColor(color_TheoryBase);
	//random.setBaseColor(base);

	// 2.2 generate
	complement.generateComplementary(ofxColorPalette::SATURATION, numColors_Theory_G2);
	complementBrightness.generateComplementary(ofxColorPalette::BRIGHTNESS, numColors_Theory_G2);
	triad.generateTriad();
	complementTriad.generateComplementaryTriad();
	//complementTriad.generateComplementaryTriad(analogSpread);
	monochrome.generateMonoChromatic(ofxColorPalette::SATURATION, numColors_Theory_G2);
	monochromeBrightness.generateMonoChromatic(ofxColorPalette::BRIGHTNESS, numColors_Theory_G2);
	analogue.generateAnalogous(numColors_Theory_G2, analogSpread);
	//analogue.generateAnalogous(numColors_Theory_G2, 0.2);

	//TODO:
	//add randoms...

	//---

	// G2

	refresh_Theory_G2_2();//TODO: reduce calls...
}

//--------------------------------------------------------------
void ofxColorManager::palettes_Resize()
{
	//TODO: not required
	numColors_Theory_G1 = numColors_Engines;
	numColors_Theory_G2 = numColors_Engines;

	refresh_Theory_G1();
	refresh_Theory_G2();

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
void ofxColorManager::refresh_Theory_G1() //populates palettes
{
	ofLogNotice(__FUNCTION__);

	for (int i = 0; i < NUM_COLOR_THEORY_TYPES_G1; i++)
	{
		//shared_ptr<ColorWheelScheme> _scheme;
		_scheme = ColorWheelSchemes::colorSchemes[i];
		_scheme->setPrimaryColor(color_TheoryBase.get());

		colors_Theory_G1[i] = _scheme->interpolate(numColors_Theory_G1.get());
		ofLogNotice(__FUNCTION__) << i;// << " " << _scheme->;
	}
}

//--------------------------------------------------------------
void ofxColorManager::setup_Theory_G1()
{
	ofLogNotice(__FUNCTION__);

	numColors_Engines.set("Amnt Colors", 8, 2, MAX_PALETTE_COLORS);
	numColors_Engines.setSerializable(true);

	//--

	//numColors_Theory_G2.makeReferenceTo(numColors_Theory_G1);
	//numColors_Theory_G1.makeReferenceTo(numColors_Theory_G2);

	params_ColorTheory_G1.clear();
	params_ColorTheory_G1.setName("Color Theory G1");
	params_ColorTheory_G1.add(color_TheoryBase.set("Base", ofColor::magenta, ofColor(0), ofColor(255)));
	params_ColorTheory_G1.add(bAuto_Theory_FromPicker.set("Auto Get Picker", true));
	params_ColorTheory_G1.add(colorScheme.set("Color Scheme", 6, 0, ColorWheelSchemes::colorSchemes.size() - 1));
	params_ColorTheory_G1.add(colorSchemeName);
	params_ColorTheory_G1.add(numColors_Theory_G1.set("Amnt Colors Thy ", 8, 2, MAX_PALETTE_COLORS));
	params_ColorTheory_G1.add(last_Index_Theory_PickPalette.set("Last Theory Picked", 0, 0, NUM_COLOR_THEORY_TYPES_G1 + NUM_COLOR_THEORY_TYPES_G2 - 1));
	//params_ColorTheory_G1.add(last_Index_Theory_PickPalette.set("Last Theory Picked", 0, 0, NUM_COLOR_THEORY_TYPES_G1 - 1));

	//toggles
	for (int i = 0; i < NUM_COLOR_THEORY_TYPES_G1; i++)
	{
		theory_Types_G1[i].set(ColorWheelSchemes::colorSchemeNames[i], false);
		params_ColorTheory_G1.add(theory_Types_G1[i]);
	}

	ofAddListener(params_ColorTheory_G1.parameterChangedE(), this, &ofxColorManager::Changed_ColorTheory);

	refresh_Theory_G1();
}

//--------------------------------------------------------------
void ofxColorManager::refresh_Theory_G2_2()
{
	ofLogNotice(__FUNCTION__);

	scheme_Analogous = ColorWheelSchemes::colorSchemes[0];
	scheme_Analogous->setPrimaryColor(color_TheoryBase);
	colors_Analogous = scheme_Analogous->interpolate(numColors_Theory_G2);

	scheme_Complementary = ColorWheelSchemes::colorSchemes[1];
	scheme_Complementary->setPrimaryColor(color_TheoryBase);
	colors_Complementary = scheme_Complementary->interpolate(numColors_Theory_G2);

	scheme_SplitComplementary = ColorWheelSchemes::colorSchemes[2];
	scheme_SplitComplementary->setPrimaryColor(color_TheoryBase);
	colors_SplitComplementary = scheme_SplitComplementary->interpolate(numColors_Theory_G2);

	scheme_Compound = ColorWheelSchemes::colorSchemes[3];
	scheme_Compound->setPrimaryColor(color_TheoryBase);
	colors_Compound = scheme_Compound->interpolate(numColors_Theory_G2);

	scheme_FlippedCompound = ColorWheelSchemes::colorSchemes[4];
	scheme_FlippedCompound->setPrimaryColor(color_TheoryBase);
	colors_FlippedCompound = scheme_FlippedCompound->interpolate(numColors_Theory_G2);

	scheme_Monochrome = ColorWheelSchemes::colorSchemes[5];
	scheme_Monochrome->setPrimaryColor(color_TheoryBase);
	colors_Monochrome = scheme_Monochrome->interpolate(numColors_Theory_G2);

	scheme_Tetrad = ColorWheelSchemes::colorSchemes[6];
	scheme_Tetrad->setPrimaryColor(color_TheoryBase);
	colors_Tetrad = scheme_Tetrad->interpolate(numColors_Theory_G2);

	scheme_Triad = ColorWheelSchemes::colorSchemes[7];
	scheme_Triad->setPrimaryColor(color_TheoryBase);
	colors_Triad = scheme_Triad->interpolate(numColors_Theory_G2);

	// NOTE: RANDOM = 0, ANALOGOUS = 1, COMPLEMENTARY = 2,
	// SPLIT_COMPLEMENTARY = 3, COMPOUND = 4, FLIPPED_COMPOUND = 5,
	// MONOCHROME = 6, TRIAD = 7, TETRAD = 8
}

//----

//--------------------------------------------------------------
void ofxColorManager::palette_FromColourLovers() // ?
{
	ofLogNotice(__FUNCTION__);

	// 1. erase user palette and fill a new one with just update/received colour lovers
	palette_Clear();

	for (int i = 0; i < myPalette_BACK.size(); i++)
	{
		ofColor c = myPalette_BACK[i];
		ofLogNotice(__FUNCTION__) << "_c [" << i << "] " << ofToString(c);
		palette_AddColor(c);
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

	// workflow 
	refresh_Background();

	//--

	//refresh_Palette_TARGET(palette);
}

//--------------------------------------------------------------
void ofxColorManager::refresh_Palette_TARGET(vector<ofColor> &p)
{
	ofLogNotice(__FUNCTION__);

	//refresh/link the subscribed palette in ofApp

	if (palette_TARGET != nullptr)
	{
		palette_TARGET->clear();
		palette_TARGET->resize(p.size());

		for (int c = 0; c < p.size(); c++)
		{
			//palette_TARGET->at(c).set(p[c]);
			(*palette_TARGET)[c].set(p[c]);
		}

		//TODO. there's a faster way without size problems?
		//palette_TARGET = (vector<ofColor>)(p);
	}
}

//----

// palette manager

//--------------------------------------------------------------
void ofxColorManager::palette_AddColor(ofColor c)
{
	ofLogNotice(__FUNCTION__) << "+ " << ofToString(palette.size()) << " : " << ofToString(c);

	palette.push_back(c);

	gradientEngine.addColor(c);

	//--

	// workflow 
	last_Index_ColorPalette.setMax(palette.size() - 1);
}

//--------------------------------------------------------------
void ofxColorManager::palette_RemoveColor(int c)
{
	// remove current color from user palette

	ofLogNotice(__FUNCTION__) << " " << c;
	ofLogNotice(__FUNCTION__) << "size pre: " << palette.size();

	if (c < palette.size())
	{
		// 0. erase last touched color th element
		//palette.erase(palette.begin() + c - 1);
		palette.erase(palette.begin() + c);

		// 1. debug after remove color from palette vector
		ofLogNotice(__FUNCTION__) << "Palette content: ";
		for (unsigned i = 0; i < palette.size(); ++i) ofLogNotice(__FUNCTION__) << ' ' << palette[i];
		ofLogNotice(__FUNCTION__) << '\n';
		ofLogNotice(__FUNCTION__) << "palette size post: " << palette.size();

		// 2. reset gradient
		gradientEngine.reset();

		// 4. add edited palette colors vector to interface buttons and to gradient
		for (int i = 0; i < palette.size(); i++)
		{
			ofLogNotice(__FUNCTION__) << "add _c [" << i << "]";
			gradientEngine.addColor(palette[i]);
		}

		//--

		// workflow
		last_Index_ColorPalette.setMax(palette.size() - 1);
	}
}

//--------------------------------------------------------------
void ofxColorManager::palette_RemoveColorLast()
{
	if (palette.size() > 0)
	{
		palette.pop_back();
		gradientEngine.removeColorLast();
	}

	// 4. add edited palette colors vector to interface buttons and to gradient
	for (int i = 0; i < palette.size(); i++)
	{
		ofLogNotice(__FUNCTION__) << "add _c [" << i << "]";
		gradientEngine.addColor(palette[i]);
	}

	//--

	// workflow
	last_Index_ColorPalette.setMax(palette.size() - 1);
}

//--------------------------------------------------------------
void ofxColorManager::palette_Clear()
{
	ofLogNotice(__FUNCTION__) << "----------------- CLEAR PALETTE -----------------";

	// remove all colors from the user palette
	palette.clear();
	gradientEngine.reset();

	//-

	if (DEMO1_Test) myDEMO1.clear();

	//--

	//TODO:
	refresh_Palette_TARGET(palette);

	last_Index_ColorPalette.setMax(palette.size() - 1);
}

//----

// callbacks

//--------------------------------------------------------------
void ofxColorManager::Changed_ColorPicked(ofFloatColor &c)
{
	ofLogNotice(__FUNCTION__) << " >>>>  color_Picked  >>>>  " << ofToString(c);// color_Picked

	// update TARGET color pointer (ofApp)
	if (color_TARGET != nullptr)
	{
		color_TARGET->set(c);
	}

	ENABLE_Callbacks_cPickers = false;
	refresh_Pick_ToHSB();
	ENABLE_Callbacks_cPickers = true;

	//--

	// workflow
	if (ENABLE_Callbacks_Engines) refresh_Pick_ToEngines();
}

//--------------------------------------------------------------
void ofxColorManager::Changed_ColorClicked(ofFloatColor &c)
{
	ofLogNotice(__FUNCTION__) << " >>>>  color_Clicked  >>>>  " << ofToString(c);// color_Clicked

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

		ENABLE_Callbacks_Engines = false;
		build_Palette_Flip();
		ENABLE_Callbacks_Engines = true;
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

	else if (name == numColors_Theory_G1.getName())
	{
		numColors_Theory_G2.setWithoutEventNotifications(numColors_Engines);
		numColors_Range.setWithoutEventNotifications(numColors_Engines);

		palettes_Resize();
	}

	else if (name == color_TheoryBase.getName())
	{
		refresh_Theory_G1();

		//color_TheoryBase.set(color_Picked);

		// ?
		//refresh_Theory_G2();

		//TODO:

		////refresh
		//ENABLE_Callbacks_cPickers = false;
		//refresh_Pick_ToHSB();
		//ENABLE_Callbacks_cPickers = true;
		//refresh_Pick_ToEngines();
	}

	//else if (name == colorScheme.getName())
	//{
	//	refresh_Theory_G1();
	//}

	else if (name == last_Index_Theory_PickPalette.getName())
	{
		ofLogWarning(__FUNCTION__) << "  >>>  last_Index_Theory_PickPalette : " << last_Index_Theory_PickPalette;

		last_Index_Theory_PickPalette = ofClamp(
			last_Index_Theory_PickPalette.get(),
			last_Index_Theory_PickPalette.getMin(),
			last_Index_Theory_PickPalette.getMax());

		//poweroff all
		for (int i = 0; i < NUM_COLOR_THEORY_TYPES_G1; i++)
		{
			theory_Types_G1[i] = false;
		}
		for (int i = 0; i < NUM_COLOR_THEORY_TYPES_G2; i++)
		{
			types_Theory_G2[i] = false;
		}

		//enable
		if (last_Index_Theory_PickPalette >= 0 &&
			last_Index_Theory_PickPalette < NUM_COLOR_THEORY_TYPES_G1)
		{
			theory_Types_G1[last_Index_Theory_PickPalette] = true;
		}
		if (last_Index_Theory_PickPalette >= NUM_COLOR_THEORY_TYPES_G1 &&
			last_Index_Theory_PickPalette < NUM_COLOR_THEORY_TYPES_G1 + NUM_COLOR_THEORY_TYPES_G2)
		{
			types_Theory_G2[last_Index_Theory_PickPalette - NUM_COLOR_THEORY_TYPES_G1] = true;
		}
	}

	//----

	// theory types G1

	else
	{
		for (int i = 0; i < NUM_COLOR_THEORY_TYPES_G1; i++)
		{
			if (name == theory_Types_G1[i].getName() && theory_Types_G1[i].get())
			{
				theory_Types_G1[i] = false;//backoff button

				palette_Clear();

				for (int n = 0; n < numColors_Theory_G1; n++)
				{
					palette_AddColor(colors_Theory_G1[i][n]);

					//ofColor c = colors_Theory_G1[i][n];
					//ofLogNotice(__FUNCTION__) << "  " << n << " : " << ofToString(c);
					//palette_AddColor(c);
				}

				//-

				name_Theory = theory_Types_G1[i].getName();

				//last_Index_Theory = i;
				last_Index_Theory.setWithoutEventNotifications(i);

				//-

				build_Palette_Engine();

				return;
			}
		}
	}

	//----

	// theory types G2

	for (int i = 0; i < NUM_COLOR_THEORY_TYPES_G2; i++)
	{
		if (name == types_Theory_G2[i].getName() && types_Theory_G2[i].get())
		{
			types_Theory_G2[i] = false;//backoff button

			vector<ofColor> __p;
			__p.clear();

			palette_Clear();

			//-

			switch (i)
			{

			case 0:
				__p.resize(complement.size());
				for (int j = 0; j < complement.size(); j++) {
					__p[j] = complement[j];
					palette_AddColor(__p[j]);
				}
				break;

			case 1:
				__p.resize(complementBrightness.size());
				for (int j = 0; j < complementBrightness.size(); j++) {
					__p[j] = complementBrightness[j];
					palette_AddColor(__p[j]);
				}
				break;

			case 2:
				__p.resize(monochrome.size());
				for (int j = 0; j < monochrome.size(); j++) {
					__p[j] = monochrome[j];
					palette_AddColor(__p[j]);
				}
				break;

			case 3:
				__p.clear();
				__p.resize(monochromeBrightness.size());
				for (int j = 0; j < monochromeBrightness.size(); j++) {
					__p[j] = monochromeBrightness[j];
					palette_AddColor(__p[j]);
				}
				break;

			case 4:
				__p.resize(analogue.size());
				for (int j = 0; j < analogue.size(); j++) {
					__p[j] = analogue[j];
					palette_AddColor(__p[j]);
				}
				break;

			case 5:
				__p.resize(triad.size());
				for (int j = 0; j < triad.size(); j++) {
					__p[j] = triad[j];
					palette_AddColor(__p[j]);
				}
				break;

			case 6:
				__p.resize(complementTriad.size());
				for (int j = 0; j < complementTriad.size(); j++) {
					__p[j] = complementTriad[j];
					palette_AddColor(__p[j]);
				}
				break;
			}

			//-

			name_Theory = types_Theory_G2[i].getName();

			//last_Index_Theory = i + NUM_COLOR_THEORY_TYPES_G1;
			last_Index_Theory.setWithoutEventNotifications(i + NUM_COLOR_THEORY_TYPES_G1);

			//-

			build_Palette_Engine();

			return;
		}
	}
}

//--------------------------------------------------------------
void ofxColorManager::Changed_Controls(ofAbstractParameter &e)
{
	std::string name = e.getName();

	ofLogNotice(__FUNCTION__) << name << " : " << e;

	//----

	if (false) {}

	// demo2 svg
	else if (name == DEMO2_Edit.getName())
	{
		DEMO2_Svg.setEdit(DEMO2_Edit);
	}
	else if (name == DEMO2_Scale.getName())
	{
		DEMO2_Svg.setScale(DEMO2_Scale);
	}

	// app modes
	else if (name == AppMode.getName())
	{
		//clamp
		//AppMode = ofClamp(
		//	AppMode.get(),
		//	AppMode.getMin(),
		//	AppMode.getMax());

		//cycle
		if (AppMode > AppMode.getMax())
			AppMode.setWithoutEventNotifications(AppMode.getMin());
		else if (AppMode < 0)
			AppMode.setWithoutEventNotifications(AppMode.getMax());
		if (current_element != AppMode)
			current_element = AppMode.get();
		AppMode_name = ofToString(element_names[AppMode.get()]);

		switch (AppMode.get())
		{
			// none
		case 0:
		{
			SHOW_Theory = false;
			SHOW_Range = false;
			SHOW_ColourLovers = false;
			SHOW_Quantizer = false;

			//SHOW_UserPaletteFloating = false;
			//SHOW_UserPaletteEditor = false;
			//SHOW_Picker = false;
			//SHOW_Gradient = false;
			//SHOW_Library = false;
			//SHOW_BackGround = false;
			//SHOW_Presets = false;
		}
		break;

		// theory
		case 1:
		{
			SHOW_Theory = true;
			SHOW_Range = false;
			SHOW_ColourLovers = false;
			SHOW_Quantizer = false;
		}
		break;

		// range
		case 2:
		{
			SHOW_Range = true;
			SHOW_Theory = false;
			SHOW_ColourLovers = false;
			SHOW_Quantizer = false;
		}
		break;

		// lovers
		case 3:
		{
			SHOW_ColourLovers = true;
			SHOW_Theory = false;
			SHOW_Range = false;
			SHOW_Quantizer = false;
		}
		break;

		// quantizer
		case 4:
		{
			SHOW_Quantizer = true;
			SHOW_Theory = false;
			SHOW_Range = false;
			SHOW_ColourLovers = false;
		}
		break;
		}
	}

	//----

	// bg 
	else if (name == color_BackGround.getName())
	{
		if (colorBg_TARGET != nullptr)
		{
			colorBg_TARGET->set(color_BackGround.get());
		}
	}

	//----

	else if (name == MODE_NewPreset.getName())
	{
		// workflow
		focus_1 = MODE_NewPreset;
	}

	//-

#ifndef USE_SIMPLE_PRESET_PALETTE	
	//export modes
	else if (name == bModeBundlePreset.getName())
	{
		bModePalettePreset.setWithoutEventNotifications(!bModeBundlePreset);
	}
	else if (name == bModePalettePreset.getName())
	{
		bModeBundlePreset = !bModePalettePreset;
	}
#endif

	//----

	// last index from palette colors

	else if (name == last_Index_ColorPalette.getName())
	{
		last_Index_ColorPalette = (int)ofClamp(
			last_Index_ColorPalette,
			last_Index_ColorPalette.getMin(),
			last_Index_ColorPalette.getMax());

		if (bEditPalette &&
			last_Index_ColorPalette >= 0 &&
			last_Index_ColorPalette <= palette.size() - 1 &&
			palette.size() > 0)
		{
			// autoload color picker to selected index color on palette
			color_Picked = palette[last_Index_ColorPalette.get()];
		}
	}

	//----

	// last index

	else if (name == last_Index_Theory.getName())
	{
		build_Palette_Engine();

		//// export
		//if (bAutoExportPreset)
		//{
		//	ofLogNotice(__FUNCTION__) << "Auto EXPORT";
		//	exportPalette();
		//}

		//txt_lineActive[0] = false;//preset name
		//txt_lineActive[1] = false;//lover name
		//txt_lineActive[2] = true;//theory name
		//txt_lineActive[3] = false;//range name
		last_Index_Type = 2;

		//if (SHOW_Presets) 
		//{
		//	textInput_New = name_Theory;
		//	MODE_NewPreset = true;
		//}
	}

	else if (name == last_Index_Range.getName())
	{
		//// export
		//if (bAutoExportPreset)
		//{
		//	ofLogNotice(__FUNCTION__) << "Auto EXPORT";
		//	exportPalette();
		//}
	}

	//----

	// num colors

	else if (name == numColors_Engines.getName())
	{
		numColors_Theory_G1.setWithoutEventNotifications(numColors_Engines);
		numColors_Theory_G2.setWithoutEventNotifications(numColors_Engines);

		numColors_Range = numColors_Engines;

		palettes_Resize();

		////--

		//// export
		//if (bAutoExportPreset)
		//{
		//	ofLogNotice(__FUNCTION__) << "Auto EXPORT";
		//	exportPalette();
		//}
	}

	else if (name == numColors_Theory_G2.getName())
	{
		numColors_Theory_G1.setWithoutEventNotifications(numColors_Theory_G2);
		numColors_Engines.setWithoutEventNotifications(numColors_Theory_G2);

		palettes_Resize();
	}

	else if (name == analogSpread.getName())
	{
		//TODO:
		refresh_Theory_G1();
		refresh_Theory_G2();

		// workflow
		if (bAuto_Build_Palette)
		{
			//if (SHOW_Theory) palette_FromTheory(last_Index_Theory);
			build_Palette_Engine();
		}
	}

	//----

	// HSB

	if (ENABLE_Callbacks_cPickers)
	{
		if (name == color_HUE.getName()) // "H"
		{
			ofLogNotice(__FUNCTION__) << " >>>>  " << name << " : " << e;

			ofFloatColor c;
			c.set(color_Picked.get());
			c.setHue((color_HUE + 0.5f) / 255.f);
			//c.setHue(color_HUE / 255.f);
			color_Picked.set(c);
		}
		else if (name == color_SAT.getName()) // "S"
		{
			ofLogNotice(__FUNCTION__) << " >>>>  " << name << " : " << e;

			ofFloatColor c;
			c.set(color_Picked.get());
			c.setSaturation((color_SAT + 0.5f) / 255.f);
			//c.setSaturation(color_SAT / 255.f);
			color_Picked.set(c);
		}
		else if (name == color_BRG.getName()) // "B"
		{
			ofLogNotice(__FUNCTION__) << " >>>>  " << name << " : " << e;

			ofFloatColor c;
			c.set(color_Picked.get());
			c.setBrightness((color_BRG + 0.5f) / 255.f);
			//c.setBrightness(color_BRG / 255.f);
			color_Picked.set(c);
		}
	}

	//----

	// panels

	//theory
	if (name == SHOW_Theory.getName())
	{
		if (SHOW_Theory.get())
		{
			AppMode = 1;
			//SHOW_Range = false;
			//SHOW_ColourLovers = false;
			//SHOW_Quantizer = false;

			// workflow
			if (bEditPalette) bEditPalette = false;

			//TODO:
			refresh_Theory_G1();
			refresh_Theory_G2();
		}
	}

	//range
	else if (name == SHOW_Range.getName())
	{
		if (SHOW_Range.get())
		{
			AppMode = 2;
			//SHOW_Theory = false;
			//SHOW_ColourLovers = false;
			//SHOW_Quantizer = false;

			// workflow
			if (bEditPalette) bEditPalette = false;
			//--

			// workflow
			refresh_Pick_ToEngines();
		}
	}

	//lovers
	else if (name == SHOW_ColourLovers.getName())
	{
		colourLoversHelper.setEnableKeys(SHOW_ColourLovers);

		if (SHOW_ColourLovers)
		{
			AppMode = 3;
			//SHOW_Theory = false;
			//SHOW_Range = false;
			//SHOW_Quantizer = false;

			// workflow
			if (bEditPalette) bEditPalette = false;
		}
	}

	//quantizer
	else if (name == SHOW_Quantizer.getName())
	{
		if (SHOW_Quantizer)
		{
			AppMode = 4;
			//SHOW_Theory = false;
			//SHOW_Range = false;
			//SHOW_ColourLovers = false;

			// workflow
			if (bEditPalette) bEditPalette = false;
		}

		colorQuantizer.setActive(SHOW_Quantizer);
	}

	//----

	// editor

	else if (name == bRandomColor.getName() && bRandomColor.get())
	{
		bRandomColor = false;

		float _hue;
		float _sat;
		float _brg;

		if (bColor_HUE) {
			_hue = ofRandom(color_HUE_0 - 128.f * color_HUE_Power, color_HUE_0 + 128.f * color_HUE_Power);
			color_HUE = ofClamp(_hue, 0, 255);
		}
		if (bColor_SAT) {
			_sat = ofRandom(color_SAT_0 - 128.f * color_SAT_Power, color_SAT_0 + 128.f * color_SAT_Power);
			color_SAT = ofClamp(_sat, 0, 255);
		}
		if (bColor_BRG) {
			_brg = ofRandom(color_BRG_0 - 128.f * color_BRG_Power, color_BRG_0 + 128.f * color_BRG_Power);
			color_BRG = ofClamp(_brg, 0, 255);
		}
	}

	//----

	// edit manager

	else if (name == bEditPalette.getName())
	{
		if (bEditPalette)
		{
			//// workflow
			////avoid collide palette auto builder with the edited color
			//if (SHOW_Range) SHOW_Range = false;
			//if (SHOW_Theory) SHOW_Theory = false;
			//if (SHOW_ColourLovers) SHOW_ColourLovers = false;
			//if (SHOW_Quantizer) SHOW_Quantizer = false;

			//selected color from palette
			last_Index_ColorPalette = ofClamp(last_Index_ColorPalette, 0, last_Index_ColorPalette.getMax());
		}
	}

	else if (name == bAddColor.getName())
	{
		if (bAddColor)
		{
			bAddColor = false;

			palette_AddColor(ofColor(color_Picked.get()));

			// workflow
			build_Palette_Engine();
			last_Index_ColorPalette = last_Index_ColorPalette.getMax();
		}
	}

	else if (name == bRemoveColor.getName())
	{
		if (bRemoveColor)
		{
			bRemoveColor = false;

			//remove selected
			palette_RemoveColor(last_Index_ColorPalette);

			// workflow
			build_Palette_Engine();
			last_Index_ColorPalette = last_Index_ColorPalette.getMax();
		}
	}

	else if (name == bClearPalette.getName())
	{
		if (bClearPalette)
		{
			bClearPalette = false;

			palette_Clear();

			// workflow
			build_Palette_Engine();
			last_Index_ColorPalette = last_Index_ColorPalette.getMax();
			palette_AddColor(color_Picked.get());
		}
	}

	//----

	// background

	else if (name == color_BackGround_GradientMode.getName())
	{
		if (color_BackGround_GradientMode) color_BackGround_DarkerMode = false;
		if (!color_BackGround_DarkerMode && !color_BackGround_GradientMode) color_BackGround_DarkerMode = true;//one must enabled
	}

	else if (name == color_BackGround_DarkerMode.getName() || name == color_BackGround_AmtDarker.getName())
	{
		// workflow
		if (name == color_BackGround_DarkerMode.getName())
		{
			if (color_BackGround_DarkerMode) color_BackGround_GradientMode = false;
			if (!color_BackGround_DarkerMode && !color_BackGround_GradientMode) color_BackGround_DarkerMode = true;//one must enabled
		}

		//set background color from first/last palette color
		refresh_Background();
	}

	else if (name == color_backGround_SET.getName())
	{
		if (color_backGround_SET)
		{
			color_backGround_SET = false;
			color_BackGround.set(ofColor(color_Picked.get()));
		}
	}

	//---

	// workflow
	//else if (name == bAuto_Build_Palette.getName())
	//{
	//	//if (bAuto_Build_Palette) bLock_palette = false;
	//}
	//else if (name == bLock_palette.getName())
	//{
	//	if (bLock_palette) bAuto_Build_Palette = false;
	//}
}

//--------------------------------------------------------------
void ofxColorManager::Changed_Range(ofAbstractParameter &e)
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
			numColors_Engines.setWithoutEventNotifications(numColors_Range);
			numColors_Theory_G1.setWithoutEventNotifications(numColors_Range);
			numColors_Theory_G2.setWithoutEventNotifications(numColors_Range);

			generate_Range(color_1_Range.get(), color_2_Range.get());

			//auto create palette
			refresh_Range_AutoUpdate();
		}

		//-

		else if (name == bAuto_Color1_FromPicker_Range.getName())
		{
			if (bAuto_Color1_FromPicker_Range) {
				bAuto_Color2_FromPicker_Range = false;
				color_Picked.set(color_1_Range.get());
			}
		}
		else if (name == bAuto_Color2_FromPicker_Range.getName())
		{
			if (bAuto_Color2_FromPicker_Range) {
				bAuto_Color1_FromPicker_Range = false;
				color_Picked.set(color_2_Range.get());
			}
		}

		//-

		else
		{
			// ranges label picked

			for (int i = 0; i < NUM_TYPES_RANGES; i++)
			{
				if (name == types_Range[i].getName() && types_Range[i].get())
				{
					types_Range[i] = false;//off button

					//-

					//index selected
					last_Index_Range = i;
					name_Range = types_Range[i].getName();
					//txt_lineActive[0] = false;//preset name
					//txt_lineActive[1] = false;//lover name
					//txt_lineActive[2] = false;//theory name
					//txt_lineActive[3] = true;//range name
					last_Index_Type = 3;

					//-

					build_Palette_Engine();
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofxColorManager::refresh_Background()
{

	//TODO:
	// background
	//// 1. from preset 
	//if (!color_BackGround_Lock)
	//{
	//	color_BackGround = ofColor(PRESET_Temp.getBackground());//get directly without pointing
	//}
	// 2. or autogenerated from darken/gradient picker..


	// workflow
	if (AutoSet_BackGround_Color && !color_BackGround_Lock)
	{
		if (palette.size() > 0)
		{
			ofFloatColor c;

			if (color_BackGround_DarkerMode)
			{
				c = palette[0];//get first color from user palette
				float darkness = ofMap(color_BackGround_AmtDarker.get(), 1.0, 0.0, 0.0, 2.0);
				float b = c.getBrightness() * darkness;
				b = ofClamp(b, 0.0, 1.0);

				c.setBrightness(b);
			}

			else if (color_BackGround_GradientMode)
			{
				c = gradientEngine.getRefreshBackground();
			}

			color_BackGround.set(c);
		}
	}
}

//--------------------------------------------------------------
void ofxColorManager::palette_FromQuantizer()
{
	ofLogNotice(__FUNCTION__);

	auto p = colorQuantizer.getPalette();

	palette_Clear();

	for (int j = 0; j < p.size(); j++)
	{
		ofColor c = p[j];
		palette_AddColor(c);
		ofLogNotice(__FUNCTION__) << "[" << j << "] > " << ofToString(c);
	}

	//--

	refresh_Background();

	//refresh_Palette_TARGET(palette);
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

		palette_Clear();

		for (int j = st; j < ed; j++)
		{
			if (j < palette_Range.size())
			{
				ofColor c = palette_Range[j];
				palette_AddColor(c);

				ofLogNotice(__FUNCTION__) << "[" << last_Index_Range << "][" << (j - st) << "] > " << ofToString(c);
			}
		}

		//--

		// workflow
		refresh_Background();

		//--

		// export
		if (bAutoExportPreset)
		{
			bExportFlag = true;

			//ofLogNotice(__FUNCTION__) << "Auto EXPORT";
			//exportPalette();
		}
	}
}

//--------------------------------------------------------------
void ofxColorManager::refresh_Range_AutoUpdate()
{
	ofLogNotice(__FUNCTION__);

	//auto create palette
	if (autoGenerate_Range && (last_Index_Range != -1))
	{
		int i = last_Index_Range;

		types_Range[i] = true;
		name_Range = types_Range[i].getName();
		//txt_lineActive[0] = false;//preset name
		//txt_lineActive[1] = false;//lover name
		//txt_lineActive[2] = false;//theory name
		//txt_lineActive[3] = true;//range name
		last_Index_Type = 3;

		//// presets 
		//if (SHOW_Presets && SHOW_Range) 
		//{
		//	textInput_New = name_Range;
		//	MODE_NewPreset = true;
		//}

		//--

		ofColor c;
		if (bAuto_Color1_FromPicker_Range) c = color_1_Range.get();
		else if (bAuto_Color2_FromPicker_Range) c = color_2_Range.get();

		if (color_TARGET != nullptr)
		{
			color_TARGET->set(c);
		}
		myColor_BACK.set(c);
		//if (myColor_BACK != nullptr)
		//{
		//	myColor_BACK->set(c);
		//}
		//if (myColor_BACK) {
		//myColor_BACK
	}
}

#pragma mark - KEYS MOUSE CALLBACKS

//--------------------------------------------------------------
void ofxColorManager::keyPressed(ofKeyEventArgs &eventArgs)
{
	const int key = eventArgs.key;

	// modifiers
	bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
	bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
	bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
	bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);

	ofLogNotice(__FUNCTION__) << " : " << key;

	if (key == 'K') ENABLE_keys = !ENABLE_keys;

	if (ENABLE_keys)
	{
		//----

		if (0) {}

		//--

		// help
		else if (key == 'h' || key == 'H')
		{
			ENABLE_HelpInfo = !ENABLE_HelpInfo;
		}
		// gui
		else if (key == 'g')
		{
			setToggleVisible();
			// workflowh
			if (!SHOW_ALL_GUI) ENABLE_HelpInfo = false;
		}
		// mini preview
		else if (key == 'm')
		{
			SHOW_MINI_Preview = !SHOW_MINI_Preview;
		}

		//--

		// user workflow

		// randomize color
		else if (key == OF_KEY_RETURN)
		{
			bRandomColor = true;
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

		// app modes
		else if (key == OF_KEY_TAB && !mod_CONTROL) {
			AppMode++;
		}
		else if (key == OF_KEY_TAB && mod_CONTROL) {
			AppMode--;
		}

		//-----

		// editor

		if (bEditPalette && SHOW_Presets && !SHOW_Theory && !SHOW_Range && !SHOW_ColourLovers && !SHOW_Quantizer)
		{
			if (key == OF_KEY_LEFT)
			{
				//if (!bEditPalette) bEditPalette = true;
				if (last_Index_ColorPalette > 0) last_Index_ColorPalette--;
			}
			else if (key == OF_KEY_RIGHT)
			{
				//if (!bEditPalette) bEditPalette = true;
				if (last_Index_ColorPalette < last_Index_ColorPalette.getMax()) last_Index_ColorPalette++;
			}
			//else if (key == ' ')
			//{
			//}
		}

		//-----

		// presets

		if (SHOW_Presets && !SHOW_Theory && !SHOW_Range && !SHOW_ColourLovers && !SHOW_Quantizer)
		{
			if (key == OF_KEY_UP || (key == ' ' && mod_CONTROL))
			{
				if (last_Index_Preset > 0)
				{
					last_Index_Preset--;
				}
				else
				{
					if (last_Index_Preset == 0)
						last_Index_Preset = files.size() - 1;
				}

				if (last_Index_Preset < files.size() && files.size() > 0)
				{
					PRESET_Name = files_Names[last_Index_Preset];
					ofLogNotice(__FUNCTION__) << "PRESET_Name: [" + ofToString(last_Index_Preset) + "] " << PRESET_Name;

					preset_Load(PRESET_Name);
				}
			}

			else if (key == OF_KEY_DOWN || (key == ' ' && !mod_CONTROL))
			{
				if (last_Index_Preset < files.size() - 1)
				{
					last_Index_Preset++;
				}
				else if (last_Index_Preset >= files.size() - 1) {//cycle
					last_Index_Preset = 0;
				}

				if (last_Index_Preset < files.size() && files.size() > 0)
				{
					PRESET_Name = files_Names[last_Index_Preset];
					ofLogNotice(__FUNCTION__) << "PRESET_Name: [" + ofToString(last_Index_Preset) + "] " << PRESET_Name;

					preset_Load(PRESET_Name);
				}
			}

			//else if (key == ' ' && !mod_CONTROL)
			//{
			//	if (last_Index_Preset < files.size() - 1)
			//	{
			//		last_Index_Preset++;
			//	}
			//	else
			//	{
			//		if (last_Index_Preset == files.size() - 1) last_Index_Preset = 0;//cycle
			//	}

			//	if (last_Index_Preset < files.size())
			//	{
			//		PRESET_Name = files_Names[last_Index_Preset];
			//		ofLogNotice(__FUNCTION__) << "PRESET_Name: [" + ofToString(last_Index_Preset) + "] " << PRESET_Name;
			//		preset_Load(PRESET_Name);
			//	}
			//}

			// get some presets by index random 
			else if (key == 'R' || key == 'r')
			{
				last_Index_Preset = (int)ofRandom(0, files.size());

				if (last_Index_Preset < files.size())
				{
					PRESET_Name = files_Names[last_Index_Preset];
					ofLogNotice(__FUNCTION__) << "PRESET_Name: [" + ofToString(last_Index_Preset) + "] " << PRESET_Name;

					preset_Load(PRESET_Name);
				}
			}
		}

		//----

		// range

		if (SHOW_Range && !SHOW_Theory && !SHOW_ColourLovers && !SHOW_Quantizer)
		{
			if (key == OF_KEY_UP || (key == ' ' && mod_CONTROL))
			{
				if (last_Index_Range == 0) last_Index_Range = NUM_TYPES_RANGES - 1;
				else if (last_Index_Range > 0) last_Index_Range--;
				last_Index_Range = (int)ofClamp(last_Index_Range, 0, NUM_TYPES_RANGES - 1);

				for (int i = 0; i < NUM_TYPES_RANGES; i++)
				{
					types_Range[i].disableEvents();
					types_Range[i] = false;
					types_Range[i].enableEvents();
				}
				types_Range[last_Index_Range] = true;
			}
			if (key == OF_KEY_DOWN || (key == ' ' && !mod_CONTROL))
			{
				if (last_Index_Range >= NUM_TYPES_RANGES - 1) last_Index_Range = 0;
				else last_Index_Range++;
				last_Index_Range = (int)ofClamp(last_Index_Range, 0, NUM_TYPES_RANGES - 1);

				for (int i = 0; i < NUM_TYPES_RANGES; i++)
				{
					types_Range[i].disableEvents();
					types_Range[i] = false;
					types_Range[i].enableEvents();
				}
				types_Range[last_Index_Range] = true;
			}

			//switch color c1/c2 selectors
			if (key == OF_KEY_BACKSPACE)
			{
				if (!bAuto_Color1_FromPicker_Range && !bAuto_Color2_FromPicker_Range) bAuto_Color1_FromPicker_Range = true;
				else if (bAuto_Color1_FromPicker_Range && !bAuto_Color2_FromPicker_Range) bAuto_Color2_FromPicker_Range = true;
				else if (!bAuto_Color1_FromPicker_Range && bAuto_Color2_FromPicker_Range) bAuto_Color1_FromPicker_Range = true;

				// workflow
				//get main color from range picker
				if (bAuto_Color1_FromPicker_Range) color_Picked.set(color_1_Range.get());
				else if (bAuto_Color2_FromPicker_Range) color_Picked.set(color_2_Range.get());
			}
		}

		//----

		// theory

		if (SHOW_Theory && !SHOW_Range && !SHOW_ColourLovers && !SHOW_Quantizer)
		{
			if (key == OF_KEY_UP || (key == ' ' && mod_CONTROL))
			{
				if (last_Index_Theory_PickPalette == 0) last_Index_Theory_PickPalette = last_Index_Theory_PickPalette.getMax();
				else if (last_Index_Theory_PickPalette > 0) last_Index_Theory_PickPalette = last_Index_Theory_PickPalette.get() - 1;
			}

			if (key == OF_KEY_DOWN || (key == ' ' && !mod_CONTROL))
			{
				//last_Index_Theory_PickPalette = last_Index_Theory_PickPalette.get() + 1;

				if (last_Index_Theory_PickPalette == last_Index_Theory_PickPalette.getMax())//cycle 
				{
					last_Index_Theory_PickPalette = last_Index_Theory_PickPalette.getMin();
				}
				else {
					last_Index_Theory_PickPalette = last_Index_Theory_PickPalette.get() + 1;
				}
			}

			//if (key == ' ')//cycle
			//{
			//	if (last_Index_Theory_PickPalette == last_Index_Theory_PickPalette.getMax())//cycle 
			//	{
			//		last_Index_Theory_PickPalette = last_Index_Theory_PickPalette.getMin();
			//	}
			//	else {
			//		last_Index_Theory_PickPalette = last_Index_Theory_PickPalette.get() + 1;
			//	}
			//}
		}

		//----

		// lovers

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
}

		//----

		// extra

#ifdef USE_DEBUG_LAYOUT
		else if (key == 'M')
		{
			mouseRuler.toggleVisibility();
			//myDEMO1.toggleMouseCamera();
		}
#endif
		// TEST
		if (key == 'T')
		{
			gradientEngine.setToggleTest();
		}

		// DEMO
		else if (key == 'D')
		{
			DEMO1_Test = !DEMO1_Test;
			if (DEMO1_Test&& DEMO_Auto) myDEMO1.reStart();
		}

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
		//	bEditLayout = !bEditLayout;
		//}

		//else if (key == 'd')
		//{
		//    // DEBUG INTERFACE
		//    bShowDebug = !bShowDebug;
		//}

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

		// panels

		//-

		else if (key == OF_KEY_F1)//presets
		{
			SHOW_Presets = !SHOW_Presets;

			//if (SHOW_Presets) 
			//{
			//	SHOW_UserPaletteFloating = false;
			//	SHOW_UserPaletteEditor = false;
			//	SHOW_Picker = false;
			//	SHOW_Library = false;
			//	SHOW_BackGround = false;
			//	SHOW_Theory = false;
			//	SHOW_Range = false;
			//	SHOW_ColourLovers = false;
			//	SHOW_Quantizer = false;
			//	SHOW_Gradient = false;
			//}
		}
		else if (key == OF_KEY_F2)//floating palette
		{
			SHOW_UserPaletteFloating = !SHOW_UserPaletteFloating;
		}
		else if (key == OF_KEY_F3)//palette editor
		{
			SHOW_UserPaletteEditor = !SHOW_UserPaletteEditor;
		}

		else if (key == OF_KEY_F4)//picker
		{
			SHOW_Picker = !SHOW_Picker;
		}
		else if (key == OF_KEY_F5)//bg
		{
			SHOW_BackGround = !SHOW_BackGround;
		}
		else if (key == OF_KEY_F6)//library
		{
			SHOW_Library = !SHOW_Library;
		}

		else if (key == OF_KEY_F7)//curve
		{
			SHOW_Gradient = !SHOW_Gradient;
			//gradientEngine.SHOW_Gradient = !gradientEngine.SHOW_Gradient;

			if (SHOW_Gradient) {
				SHOW_UserPaletteFloating = false;
				SHOW_UserPaletteEditor = false;
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

		//-

		// engines

		//else if (key == OF_KEY_F5)//theory
		//{
		//	SHOW_Theory = !SHOW_Theory;
		//}
		//else if (key == OF_KEY_F6)//range
		//{
		//	SHOW_Range = !SHOW_Range;
		//}
		//else if (key == OF_KEY_F7)//lovers
		//{
		//	SHOW_ColourLovers = !SHOW_ColourLovers;
		//}
		//else if (key == OF_KEY_F8)//quantizer
		//{
		//	SHOW_Quantizer = !SHOW_Quantizer;
		//}

		else if (key == OF_KEY_F11)//all off
		{
			SHOW_UserPaletteFloating = false;
			SHOW_UserPaletteEditor = false;
			SHOW_Picker = false;
			SHOW_Gradient = false;
			SHOW_Library = false;
			SHOW_BackGround = false;
			SHOW_Theory = false;
			SHOW_Range = false;
			SHOW_ColourLovers = false;
			SHOW_Quantizer = false;
			SHOW_Presets = false;
		}
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
		if (!DEMO_Auto) {//disable mouse on auto mode
			//second mouse button cleans DEMO
			if (button == 2) myDEMO1.clear();
			else myDEMO1.start();//trig DEMO start
		}
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
void ofxColorManager::mouseScrolled(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const float &scrollX = eventArgs.scrollX;
	const float &scrollY = eventArgs.scrollY;
	ofLogNotice(__FUNCTION__) << "scrollX: " << scrollX << "  scrollY: " << scrollY;

	if (DEMO2_Edit)
	{
		if (DEMO2_Svg.rectDgSvg.inside(glm::vec2(x, y)))//zoom 
			if (scrollY == 1) DEMO2_Scale += 0.025f;
			else if (scrollY == -1) DEMO2_Scale -= 0.025f;
	}
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

////--------------------------------------------------------------
//void ofxColorManager::disableListeners()
//{
//	addMouseListeners();
//	addKeysListeners();
//}
//
////--------------------------------------------------------------
//void ofxColorManager::enableListeners()
//{
//	removeMouseListeners();
//	removeKeysListeners();
//}

//--

// settings

//--------------------------------------------------------------
void ofxColorManager::refresh_Files(std::string name)
{
	//--

	// workflow

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

		last_Index_Preset = ii;
		PRESET_Name = files_Names[last_Index_Preset];

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

	last_Index_Preset.setMax(files.size() - 1);
	if (last_Index_Preset > files.size() - 1) last_Index_Preset = -1;

	//-

	// workflow

	//TODO:
	//void to go to 1st...

	// 1. load same position preset
	// if preset is deleted goes to nextone..
	// should check names because sorting changes..
	if (files_Names.size() > 0)
	{
		if (last_Index_Preset > files_Names.size() - 1) last_Index_Preset = files_Names.size() - 1;

		//else if (last_Index_Preset > files_Names.size() - 1)

		PRESET_Name = files_Names[last_Index_Preset];
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
	//    last_Index_Preset = 0;
	//    PRESET_Name = files_Names[last_Index_Preset];
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
	ofLogNotice(__FUNCTION__) << "----------------- PRESET LOAD -----------------";
	ofLogNotice(__FUNCTION__) << p;

	//--

	//TODO:
	// workflow
	//avoid collide palette auto builder with auto generator engines
	if (SHOW_Range) SHOW_Range = false;
	if (SHOW_Theory) SHOW_Theory = false;
	if (SHOW_ColourLovers) SHOW_ColourLovers = false;
	if (SHOW_Quantizer) SHOW_Quantizer = false;

	//--

	// setup linking pointers to get back on load
	PRESET_Temp.setName_TARGET(p);
	PRESET_Temp.setPalette_TARGET(palette);

#ifndef USE_SIMPLE_PRESET_PALETTE	
	PRESET_Temp.setNameCurve_TARGET(PRESET_Name_Gradient);
#endif

	//--

	//txt_lineActive[0] = true;//preset name
	//txt_lineActive[1] = false;//lover name
	//txt_lineActive[2] = false;//theory name
	//txt_lineActive[3] = false;//range name
	last_Index_Type = 0;

	//--

	p = p + ".json";

	// 1. load palette preset (target will be the above pointers) //TODO: should (late?) improve this..
	bool b = PRESET_Temp.preset_Load(p);
	if (!b)
	{
		ofLogError(__FUNCTION__) << "Preset file " << p << " not found! ";
		return;
	}
	// palette and name will we auto updated here bc are pointer-back-referenced
	// preset loaded succesfully here!

	//--

	//TODO:
	// curve gradient preset

	//--

	//TODO:
	// populate palette colors
	palette_Clear();
	vector<ofColor> _p = PRESET_Temp.getPalette();
	for (int i = 0; i < _p.size(); i++)
	{
		ofLogNotice(__FUNCTION__) << "Color #" << ofToString(i) << " " << ofToString(_p[i]);
		palette_AddColor(_p[i]);
	}

	//--

	//TODO:

	// workflow
	// new preset
	if (MODE_NewPreset) MODE_NewPreset = false;
	//// demo mode
	//if (DEMO1_Test && DEMO_Auto) myDEMO1.reStart();

	// load first color
	if (palette.size() > 0) color_Picked = ofFloatColor(palette[0]);

	//--

	build_Palette_Preset();

	// export
	if (bAutoExportPreset)
	{
		bExportFlag = true;
		//ofLogNotice(__FUNCTION__) << "Auto EXPORT";
		//exportPalette();
	}
}

//--------------------------------------------------------------
void ofxColorManager::preset_Save(std::string p)
{
	ofLogNotice(__FUNCTION__) << "----------------- PRESET SAVE -----------------" << p;
	//ofLogNotice(__FUNCTION__) << p;

	//TODO:
	//this is using pointers.. ?

	PRESET_Temp.setName_TARGET(p);
	PRESET_Temp.setPalette_TARGET(palette);

#ifndef USE_SIMPLE_PRESET_PALETTE	
	PRESET_Temp.setNameCurve_TARGET(PRESET_Name_Gradient);
#endif
	PRESET_Temp.setBackgroundColor(color_BackGround.get());//no pointer now

	PRESET_Temp.preset_Save(p, false);
	//PRESET_Temp.preset_Save(PRESET_Name);
}

//----

//--------------------------------------------------------------
void ofxColorManager::refresh_FromPicked()
{
	ofLogNotice(__FUNCTION__);

	color_TheoryBase.set(color_Picked.get());
}

//--------------------------------------------------------------
void ofxColorManager::refresh_Pick_ToHSB()
{
	ofLogNotice(__FUNCTION__);

	//TODO:
	color_HUE = int(255.f * color_Picked.get().getHue() + 0.5f);
	color_SAT = int(255.f * color_Picked.get().getSaturation() + 0.5f);
	color_BRG = int(255.f * color_Picked.get().getBrightness() + 0.5f);
}

//--------------------------------------------------------------
void ofxColorManager::refresh_Pick_ToEngines()
{
	ofLogNotice(__FUNCTION__) << " >>>> Spread color_Picked : " << ofToString(color_Picked);

	ENABLE_Callbacks_cPickers = false; //disable callbacks
	{
		//-

		// 1. theory color base

		if (bAuto_Theory_FromPicker)
		{
			color_TheoryBase = color_Picked.get();
		}

		//-

		// 2. range color1

		if (bAuto_Color1_FromPicker_Range)
		{
			color_1_Range = color_Picked.get();
			if (autoGenerate_Range) generate_Range(color_1_Range.get(), color_2_Range.get());
		}

		//-

		// 3. range color2

		if (bAuto_Color2_FromPicker_Range)
		{
			color_2_Range = color_Picked.get();
			if (autoGenerate_Range) generate_Range(color_1_Range.get(), color_2_Range.get());
		}

		//--

		//TODO:
		// mirror clicked/picked colors
		color_Clicked2.set(color_Picked.get());
	}
	ENABLE_Callbacks_cPickers = true;

	//--

	// autosave edited color

	if (bEditPalette &&
		last_Index_ColorPalette >= 0 &&
		last_Index_ColorPalette <= palette.size() - 1 &&
		palette.size() > 0)
	{
		// update user palette color with recently picked color
		palette[last_Index_ColorPalette].set(color_Picked.get());

		// update gradient
		if (last_Index_ColorPalette < gradientEngine.getNumColors())
		{
			gradientEngine.replaceColorAtIndex(last_Index_ColorPalette, color_Picked.get());
		}
	}

	//--

	// workflow

	if (bAuto_Build_Palette)
	{
		build_Palette_Engine();
	}
}

//----

// API - pointer references to ofApp color and palette

//--------------------------------------------------------------
void ofxColorManager::setName_TARGET(std::string &s)
{
	ofLogNotice(__FUNCTION__) << s;
	name_TARGET = &s;
}

//--------------------------------------------------------------
void ofxColorManager::setColor_TARGET(ofColor &c)
{
	ofLogNotice(__FUNCTION__) << ofToString(c);
	color_TARGET = &c;
}

//--------------------------------------------------------------
void ofxColorManager::setColorBg_TARGET(ofColor &c)
{
	ofLogNotice(__FUNCTION__) << ofToString(c);
	colorBg_TARGET = &c;
}

//--------------------------------------------------------------
void ofxColorManager::setPalette_TARGET(vector<ofColor> &p)
{
	ofLogNotice(__FUNCTION__) << ofToString(p);
	palette_TARGET = &p;
}

////--
//
////--------------------------------------------------------------
//void ofxColorManager::setControl_Gradient(float control)
//{
//	ofLogNotice(__FUNCTION__) << control;
//
//	if (!bEditLayout)
//	{
//		curvePickIn.setWithoutEventNotifications(control);
//		//curvePickIn = control;
//
//		curve_Slider_Pick.setPercent(control);
//	}
//}

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

	if (SHOW_Gradient)
	{
		gradientEngine.setVisiblePreviewPalette(SHOW_ALL_GUI);
	}
}

//--------------------------------------------------------------
void ofxColorManager::setVisible(bool b)
{
	SHOW_ALL_GUI = b;
	ofLogNotice(__FUNCTION__) << "SHOW_ALL_GUI:" << SHOW_ALL_GUI;
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

////--------------------------------------------------------------
//void ofxColorManager::draw_GradientPreview(glm::vec2 pos, bool horizontal = true)
//{
//	if (horizontal)
//	{
//		image_GradientCurve.draw(pos);
//	}
//
//	//TODO: how to draw rotated... can image.rotate the image in some trig...
//	//    else
//	//    {
//	//        ofPushMatrix();
//	//        ofTranslate(pos);
//	//        ofRotateDeg(90, 1, 0, 0);
//	//        image_GradientCurve.draw(0, 0);
//	//        ofPopMatrix();
//	//    }
//}

//--------------------------------------------------------------
void ofxColorManager::setVisible_debugText(bool b)
{
	ofLogNotice(__FUNCTION__) << b;
	SHOW_debugText = b;
}

//----

//--------------------------------------------------------------
void ofxColorManager::setup_Range()
{
	color_1_Range.set("Color 1", ofColor::red, ofColor(0), ofColor(255));
	color_2_Range.set("Color 2", ofColor::blue, ofColor(0), ofColor(255));

	// 12 types
	names_Types_Range = { "RGB", "HSL", "HSV ", "HSB", "LUV ", "LAB", "HLAB", "LCH", "CMY", "CMYK", "YXY", "XYZ" };

	autoGenerate_Range.set("Auto Generate", true);
	bAuto_Color1_FromPicker_Range.set("Auto Pick C1", true);
	bAuto_Color2_FromPicker_Range.set("Auto Pick C2", false);
	bGetPalette_From_Range.set("To User Palette", false);

	numColors_Range.set("Amnt Colors Rng", (int)MAX_PALETTE_COLORS, 3, (int)MAX_PALETTE_COLORS);
	numColors_Range.setSerializable(false);

	params_Ranges.setName("Params Ranges");
	for (int i = 0; i < int(NUM_TYPES_RANGES); i++) //12
	{
		types_Range[i].set(ofToString(names_Types_Range[i]), false);
		params_Ranges.add(types_Range[i]);
	}
	params_Ranges.add(numColors_Range);
	params_Ranges.add(bAuto_Color1_FromPicker_Range);
	params_Ranges.add(bAuto_Color2_FromPicker_Range);
	params_Ranges.add(bGetPalette_From_Range);

	ofAddListener(params_Ranges.parameterChangedE(), this, &ofxColorManager::Changed_Range);

	//--

	generate_Range(color_1_Range.get(), color_2_Range.get());

	bRange_Intitiated = true;
}

//--------------------------------------------------------------
void ofxColorManager::generate_Range(ofColor col1, ofColor col2) {
	if (bRange_Intitiated)
	{

		ofLogNotice(__FUNCTION__);
		ofLogNotice(__FUNCTION__) << "col1:" << ofToString(col1) << " > col2:" << ofToString(col2);

		int _max = 400;
		int _step;
		int _div;

		ofLogNotice(__FUNCTION__) << numColors_Range.getName() << " : " << numColors_Range.get();

		//_div = 10;
		_div = int((numColors_Range.get() - 1));
		if (_div > 0)
		{
			_step = _max / _div;

			ofLogNotice(__FUNCTION__) << "_div : " << _div;
			ofLogNotice(__FUNCTION__) << "_step: " << _step;

			if (_step > 0)
			{
				palette_Range.clear();

				for (int i = 0; i < int(NUM_TYPES_RANGES); ++i)
				{
					glm::vec3 left = { 270, 120 + i * _step, 0.0 };
					glm::vec3 right = { 670, 120 + i * _step, 0.0 };

					for (auto j = 0; j <= _max; j += _step)
					{
						glm::vec3 pos = { j + 270, 120 + i * _step, 0.0 };

						ofColor color = ofxColorMorph::colorMorph(pos, left, col1, right, col2, static_cast<type>(i));

						ofLogNotice(__FUNCTION__) << i << ":" << j << " \t> " << color;

						palette_Range.push_back(color);
					}
				}

				ofLogNotice(__FUNCTION__) << "palette_Range size: " << palette_Range.size();
			}
			else
			{
				ofLogWarning(__FUNCTION__) << "skip bc params wrong defined yet..";
			}
		}
		else {
			ofLogWarning(__FUNCTION__) << "skip bc params wrong defined yet..";
		}
	}
}

//----

//--------------------------------------------------------------
void ofxColorManager::exportPalette()
{
	ofLogNotice(__FUNCTION__) << "----------------- EXPORT PALETTE -----------------";

	ofJson j;

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

	//--

#ifndef USE_SIMPLE_PRESET_PALETTE

	if (!bModeBundlePreset)
	{
		// A. save palette colors only (without background neither gradient)
		//using ofxSerializer
		j = palette;
		std::string _path = path_FileExport + "_Palette.json";
		ofLogNotice(__FUNCTION__) << "\n" << ofToString(j);

		ofSavePrettyJson(_path, j);
	}

	//-

	else if (bModeBundlePreset)
	{
		// B. save full preset
		//TODO:
		//should save colors + bakground too + gradient! = preset
		PRESET_Name_Gradient = gradientEngine.path_Name_Gradient;
		PRESET_Temp.name = PRESET_Name;//? should use pointer
		PRESET_Temp.nameCurve = PRESET_Name_Gradient;//? path_Name_Gradient

		PRESET_Temp.preset_Save(path_FileExport + "_Bundled", true);

		//preset_Save(path_Folder_ExportColor_Custom);

		j = PRESET_Temp.getPresetJsonLastSaved();//for TCP link only
	}

#endif

	//-

#ifdef USE_SIMPLE_PRESET_PALETTE
	// save palette colors only (without background neither gradient, bg or name)
	//using ofxSerializer
	j = palette;
	std::string _path = path_FileExport + "_Palette.json";
	ofLogNotice(__FUNCTION__) << "\n" << ofToString(j);

	ofSavePrettyJson(_path, j);
#endif

	//--

	// TCP LINK

	//TODO: 
	//add bundle json too
	//j = PRESET_Temp.getJson();
	//j = load(path_FileExport + "_Bundled"
#ifdef LINK_TCP_MASTER_CLIENT
	std::stringstream ss;
	ss << j;
	//ss << "[/TCP]";

	ofLogNotice(__FUNCTION__) << "LINK: " + ss.str();

	TCP.sendToAll(ss.str());//send to all clients
	//for (int i = 0; i < TCP.getLastID(); i++)//many clients can be connected 
	//{
	//	//TCP.send(i, ss.str());
	//}

	storeText.push_back(ss.str() + "\n");
#endif

	//--
}

#ifdef LINK_TCP_MASTER_CLIENT
//--------------------------------------------------------------
void ofxColorManager::setupLink() {

	// setup the server to listen on 11999
	ofxTCPSettings settings(port);

	// set other options
	//settings.blocking = false;
	//settings.reuse = true;
	//settings.messageDelimiter = "\n";

	TCP.setup(settings);

	// optionally set the delimiter to something else.  
	//The delimiter in the client and the server have to be the same, default being [/TCP]
	//TCP.setMessageDelimiter("\n");
	lastCheckLink = 0;
}

//--------------------------------------------------------------
void ofxColorManager::updateLink() {

	// for each client lets send them a message letting them know what port they are connected on
	// we throttle the message sending frequency to once every 100ms
	uint64_t now = ofGetElapsedTimeMillis();
	if (now - lastCheckLink >= 2000)
	{
		for (int i = 0; i < TCP.getLastID(); i++)
		{
			if (!TCP.isClientConnected(i)) continue;

			ofLogNotice(__FUNCTION__) << "Connected on port: " + ofToString(TCP.getClientPort(i));

			//TCP.send(i, "hello client - you are connected on port - " + ofToString(TCP.getClientPort(i)));
		}
		lastCheckLink = now;
	}
}

//--------------------------------------------------------------
void ofxColorManager::drawLink() {
	std::string ss;
	ss = "TCP SERVER\n\port: " + ofToString(TCP.getPort()) + "\n";

	// for each connected client lets get the data being sent and lets print it to the screen
	for (unsigned int i = 0; i < (unsigned int)TCP.getLastID(); i++) {

		if (!TCP.isClientConnected(i))continue;

		// get the ip and port of the client
		std::string port = ofToString(TCP.getClientPort(i));
		std::string ip = TCP.getClientIP(i);
		std::string info = "client " + ofToString(i) + " " + ip + ":" + port;

		// if we don't have a string allocated yet
		// lets create one
		if (i >= storeText.size())
		{
			storeText.push_back(std::string());
		}

		// receive all the available messages, separated by \n
		// and keep only the last one
		std::string str;
		std::string tmp;
		do
		{
			str = tmp;
			tmp = TCP.receive(i);
		} while (tmp != "");

		// if there was a message set it to the corresponding client
		if (str.length() > 0)
		{
			storeText[i] = str;
		}

		//// draw the info text and the received text bellow it
		//ofDrawBitmapString(info, xPos, yPos);
		//ofDrawBitmapString(storeText[i], 25, yPos + 20);

		ss += info;
		ss += storeText[i];
	}

	ofxSurfingHelpers::drawTextBoxed(font, ss, 20, 20);
}
#endif
