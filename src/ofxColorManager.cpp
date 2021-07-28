#include "ofxColorManager.h"

//TODO:
//not working maybe bc rights
//--------------------------------------------------------------
void ofxColorManager::dragEvent(ofDragInfo info) {
	colorQuantizer.dragEvent(info);
}

//--------------------------------------------------------------
void ofxColorManager::build_Palette_SortShift(bool bDirLeft)
{
	ofLogNotice(__FUNCTION__) << "----------------- BUILD PALETTE SORT SHIFT -----------------";

	if (bDirLeft) std::rotate(palette.begin(), palette.begin() + 1, palette.end());//rotate left
	else std::rotate(palette.rbegin(), palette.rbegin() + 1, palette.rend());//rotate right

	//--

	gradientEngine.build_FromPaletteLinked(palette);
	last_Index_ColorPalette = 0;

	ENABLE_CALLBACKS_Engines = true;

	refresh_Palette_TARGET(palette);// ?

	// export
	if (bAutoExportPreset)
	{
		bExportFlag = true;
	}

	// workflow
	bFlag_refresh_EnginesFromPalette = true;

	// workflow
	refresh_ColorPickedFromEngines();
}

//--------------------------------------------------------------
void ofxColorManager::build_Palette_SortRandom()
{
	ofLogNotice(__FUNCTION__) << "----------------- BUILD PALETTE SORT RANDOM -----------------";

	ENABLE_CALLBACKS_Engines = false;

	//auto rng = std::default_random_engine{};
	//std::shuffle(std::begin(palette), std::end(palette), rng);
	srand(unsigned(time(NULL)));
	std::shuffle(palette.begin(), palette.end(), std::random_device());

	//--

	gradientEngine.build_FromPaletteLinked(palette);
	last_Index_ColorPalette = 0;

	ENABLE_CALLBACKS_Engines = true;

	refresh_Palette_TARGET(palette);// ?

	// export
	if (bAutoExportPreset)
	{
		bExportFlag = true;
	}

	// workflow
	bFlag_refresh_EnginesFromPalette = true;

	// workflow
	refresh_ColorPickedFromEngines();
}

//--------------------------------------------------------------
void ofxColorManager::build_Palette_SortFlip()
{
	ofLogNotice(__FUNCTION__) << "----------------- BUILD PALETTE FLIP -----------------";

	std::reverse(palette.begin(), palette.end());

	gradientEngine.build_From_TARGET();

	//----

	// workflow
	refresh_Background();

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
	}

	// workflow
	bFlag_refresh_EnginesFromPalette = true;

	// workflow
	refresh_ColorPickedFromEngines();
}

//--------------------------------------------------------------
void ofxColorManager::build_Palette_Engine()
{
	ofLogNotice(__FUNCTION__) << "----------------- BUILD PALETTE ENGINE -----------------";

	std::string _name = "";

	//--

	bool bNew = false;

	if (SHOW_Theory)
	{
		_name = name_Theory;
		palette_FromTheory(last_Index_Theory);
		bNew = true;
	}
	else if (SHOW_Range)
	{
		_name = name_Range;
		if (last_Index_Range >= 0) palette_FromRange(last_Index_Range);
		bNew = true;
	}
	else if (SHOW_ColourLovers)
	{
		_name = myPalette_Name_BACK;

		//clean rare chars from name
		ofStringReplace(_name, "/", "");
		ofStringReplace(_name, "\\", "");
		//ofStringReplace(_name, "-", "");
		ofStringReplace(_name, "*", "");

		palette_FromColourLovers();
		bNew = true;
	}
	else if (SHOW_Quantizer)
	{
		_name = myPalette_Name_BACK;
		palette_FromQuantizer();
		bNew = true;
	}

	if (name_TARGET != nullptr) name_TARGET[0] = _name;

	//----

	// presets

	//if (SHOW_Presets)
	{
		// workflow
		//if (!MODE_NewPreset && bNew) MODE_NewPreset = true;
		MODE_ReadyToUpdate = false;

		textInput_New = _name;
	}

	//--

	PRESET_Temp.setLinkName(_name);

#ifndef USE_SIMPLE_PRESET_PALETTE	
	PRESET_Temp.setNameCurve_TARGET(PRESET_Name_Gradient);
#endif

	//--

	// workflow
	if (DEMO1_Enable || SHOW_About) DEMO1_Bubbles.reStart();
	if (DEMO2_Enable || SHOW_About) DEMO2_Spheres.start();

	//--

	// export
	if (bAutoExportPreset)
	{
		bExportFlag = true;
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

	//TODO:
	// name
	if (name_TARGET != nullptr) name_TARGET[0] = PRESET_Name;
}

//--------------------------------------------------------------
ofxColorManager::ofxColorManager()
{
	//TODO: trouble with drag event image files to extract
	ofRestoreWorkingDirectoryToDefault();

	// log
#ifdef MAKE_RELEASE_VERSION 
	ofSetLogLevel(OF_LOG_SILENT);
	ofSetLogLevel("ofApp", OF_LOG_SILENT);
	ofSetLogLevel("ofxColorManager", OF_LOG_SILENT);
	ofSetLogLevel("ofxColorClient", OF_LOG_SILENT);
#else
	ofSetLogLevel("ofxColorManager", OF_LOG_NOTICE);
#endif

	//--

	ofAddListener(ofEvents().update, this, &ofxColorManager::update);
	ofAddListener(ofEvents().draw, this, &ofxColorManager::draw, OF_EVENT_ORDER_BEFORE_APP);
	//ofAddListener(ofEvents().fileDragEvent, this, &ofxColorManager::dragEvent);//? error

	//-

	//mouse callbacks
	ofAddListener(ofEvents().mouseDragged, this, &ofxColorManager::mouseDragged);
	ofAddListener(ofEvents().mouseScrolled, this, &ofxColorManager::mouseScrolled);

	//--

	//default
	fps = 60.0f;
	dt = 1.f / fps;

	//--

	// file paths

	path_Kit = "USER KIT"; // default kit
	path_Global = "ofxColorManager/";
	ofxSurfingHelpers::CheckFolder(path_Global);
	path_AppState = path_Global + "ofxColorManager_Settings.xml";

	path_ImGui = "layout/ImGui/";
	//path_ImGui = "ImGui/";
	ofxSurfingHelpers::CheckFolder(path_ImGui);
	path_ImGui = ofToDataPath(path_ImGui, true);
	//path_ImGui = getAbsolutePath(path_ImGui, true);

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

	buildHelpInfo();
}

//--------------------------------------------------------------
void ofxColorManager::setup()
{
	ofLogNotice(__FUNCTION__) << endl << endl << "----------------- SETUP -----------------" << endl;

	ENABLE_keys.setSerializable(false);

#ifdef USE_OFX_WINDOWAPP
	windowApp.setFrameRate(60);
	windowApp.setVerticalSync(false);
	windowApp.setShowDebug(false);
#else
	ofSetFrameRate(60);
#endif

	//--

#ifdef USE_VERSION_CHECKER
	// app version checker
	_isAnUpdateAvailable = false;
	_appUpdateNotifier.init(1, "version.json", 1);
	ofAddListener(_appUpdateNotifier.newVersionAvailable, this, &ofxColorManager::onNewVersionAvailable);
#endif

	//--

	setupRange();

#ifdef USE_SUPER_LOG
	std::string fileLoggingDirectory = "logs";
	bool logToConsole = 0;
	bool logToScreen = true;
	ofSetLoggerChannel(ofxSuperLog::getLogger(logToConsole, logToScreen, "logs"));
#endif

	SHOW_HelpInfo.set("Help Info", false);
	SHOW_HelpInfo.setSerializable(false);

	//-

	// library

	sizeLibColBox.set("Size Lib", 25, 10, 100);
	lib_Responsive_ModeGrid.set("Responsive Grid", false);
	lib_Responsive_ModeFit.set("Responsive Fit", true);
	bPagerized.set("Mode Paging", false);
	//palette
	sizePaletteBox.set("Size Plt", 25, 10, 500);
	bResponsive_Panels.set("Responsive", false);

	bFitLayout.set("Fit", true);

	//-

	// display text for extra preset name

	std::string strFont;
	std::string _p = "assets/fonts/";

	strFont = _p + "Kazesawa-Extrabold.ttf";
	//strFont = _p + "Ruda-Bold.ttf";
	//strFont = _p + "GTAmerica-ExtendedBlack.ttf";
	//strFont = _p + "GTAmerica-ExpandedBold.ttf";
	//strFont = _p + "telegrama_render.otf";
	//strFont = _p + "PragmataProB_0822.ttf";

	int _szbig = 50;
	fontBig.load(strFont, _szbig);
	fontMedium.load(strFont, _szbig - 10);
	fontSmall.load(strFont, _szbig - 15);

	last_Index_Type = -1;

	//-

	// help info
	int _sz = 11;
	font.load("assets/fonts/telegrama_render.otf", _sz, true, true, true);
	//font.load("assets/fonts/LCD_Solid.ttf", 11, true, true);
	//font.load("assets/fonts/overpass-mono-bold.otf", 9, true, true);

	//----

//	// colorQuantizer
//
//#ifdef USE_IMAGE_QUANTIZER
//	colorQuantizer.setup();
//
//	// receivers pointers
//	colorQuantizer.setColor_BACK(myColor_BACK);
//	colorQuantizer.setColor_BACK_Refresh(bUpdated_Color_BACK);
//	colorQuantizer.setPalette_BACK(myPalette_BACK);
//	colorQuantizer.setPalette_BACK_RefreshPalette(bUpdated_Palette_BACK);
//	colorQuantizer.setPalette_BACK_Name(myPalette_Name_BACK);
//#endif

	//----

	// theory

	setupTheory_G1();

	//----

#ifdef USE_DEBUG_LAYOUT
	// mouse ruler debugger
	mouseRuler.setup();
	mouseRuler.toggleVisibility();
#endif

	//----

	// lovers

#ifdef USE_COLOR_LOVERS

	colourLoversHelper.setup();

	//----

	// pointers back
	colourLoversHelper.setColorPtr(myColor_BACK);
	colourLoversHelper.setColor_BACK_Refresh(bUpdated_Color_BACK);
	colourLoversHelper.setPalettePtr(myPalette_BACK);
	colourLoversHelper.setPaletteNamePtr(myPalette_Name_BACK);
	colourLoversHelper.setPalette_BACK_Refresh(bUpdated_Palette_BACK);

	//SHOW_ColourLovers.makeReferenceTo(colourLoversHelper.bGui);

	//--------------------------------------------------------------
	listener_LoverName = colourLoversHelper.lastPaletteName.newListener([this](std::string &n) {
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

	color_HUE.set("HUE", 0, 0, 255);
	color_SAT.set("SAT", 0, 0, 255);
	color_BRG.set("BRG", 0, 0, 255);

	params_color.setName("COLOR");
	params_color.add(color_HUE);
	params_color.add(color_SAT);
	params_color.add(color_BRG);

	// randomizer
	bColor_rHUE.set("rHUE ", true);
	bColor_rBRG.set("rBRG ", true);
	bColor_rSAT.set("rSAT ", true);

	color_HUE_0.set("H 0", 0, 0, 255);
	color_SAT_0.set("S 0", 0, 0, 255);
	color_BRG_0.set("V 0", 0, 0, 255);
	color_HUE_Power.set("H POW", 0.25, 0, 1);
	color_SAT_Power.set("S POW", 0.25, 0, 1);
	color_BRG_Power.set("V POW", 0.25, 0, 1);

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
	triadSpread.set("Triad Spread", 0.083, 0, 1);

	//bLock_palette.set("Lock Palettes", false);
	//bRandomPalette.set("RANDOM PALETTE", false);

	params_Theory.setName("ADVANCED");
	params_Theory.add(numColors_Theory_G2);
	params_Theory.add(bAuto_Build_Palette);
	params_Theory.add(analogSpread);
	params_Theory.add(triadSpread);
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

	DEMO1_Cam.setSerializable(false);
	DEMO2_Spheres.mouseCam.setSerializable(false);
	//DEMO2_Edit.setSerializable(false);

	ofAddListener(params_Theory.parameterChangedE(), this, &ofxColorManager::Changed_Controls);

	//-

	refresh_Theory_G2();

	//-

	SHOW_Gradient.set("GRADIENT", true);
	//SHOW_Gradient.makeReferenceTo(gradientEngine.SHOW_Gradient);
	////gradientEngine.SHOW_Gradient.makeReferenceTo(SHOW_Gradient)

	// gradient
	gradientEngine.setup();
	gradientEngine.setBackground_TARGET(color_BackGround);
	gradientEngine.setLinkPalette(palette);//subscribe local palette to allow modify from the class object

	//-

	bRandomColor.set("RANDOMIZE", false);
	bAddColor.set("COPY", false);
	bEditPalette.set("EDIT", false);
	bRemoveColor.set("REMOVE", false);
	bClearPalette.set("CLEAR", false);

	SHOW_Scene.set("SCENE", true);

	//SHOW_Scene.setSerializable(false);
	bEditPalette.setSerializable(false);

	//-

	// panels

	SHOW_ALL_GUI.setName("GUI MAIN");
	SHOW_MINI_Preview.setName("SHOW MINI PALETTE");
	SHOW_Palette.setName("PALETTE");
	SHOW_Palette = false;
	SHOW_Editor.setName("EDITOR");
	SHOW_Editor = false;
	SHOW_Theory.setName("THEORY");
	SHOW_Theory = false;
	SHOW_debugText.setName("SHOW debug");
	SHOW_Quantizer.setName("PICTURE");
	SHOW_Quantizer = false;

	//-

	SHOW_BrowserColors.set("SHOW BROWSER COLORS", false);
	SHOW_BrowserColors.setSerializable(false);

	SHOW_ColourLovers_searcher.set("SHOW COLOUR LOVERS SEARCH", true);
	SHOW_ColourLovers.set("LOVERS", false);
	colourLoversHelper.bGui.makeReferenceTo(SHOW_ColourLovers);

	SHOW_Presets.set("PRESETS", false);
	SHOW_Kit.set("KIT", false);
	AutoScroll.set("AutoScroll", true);
#ifdef MODE_BACKGROUND
	SHOW_BackGround.set("BACKGROUND", true);
#endif
	SHOW_Picker.set("PICKER", false);
	SHOW_Library.set("LIBRARY", false);
	SHOW_Engines.set("ENGINES", true);
	SHOW_Range.set("RANGE", false);
	SHOW_Panels.set("PANELS", true);
	SHOW_LinkExport.set("LINK", false);
	SHOW_Demos.set("DEMO", false);

	//bAutoResizePalette.set("AutoResize", false);
#ifndef USE_SIMPLE_PRESET_PALETTE	
	bModeBundlePreset.set("Mode Bundle", false);
	bModePalettePreset.set("Mode Palette", false);
#endif
	bAutoExportPreset.set("Auto Export", true);
	bExportByTCP.set("TCP", false);
	bExportByFile.set("FILE", false);
	bExportPreset_DefaultPath.set("Path /bin/data", true);
	path_Folder_ExportColor_Custom.set("ExportPath", "");

	//-

	scale_ColRange.set("Scale Rg", 1.f, 0.25f, 1.25f);

	//-

	//palette

	boxSizeUser.set("Box Size", 40, 10, 200);
	boxMaxRows.set("Max Rows", 10, 1, 20);
	scale_ColPalette.set("Scale Pt", 1.f, 0.25f, 1.25f);
	bFlipUserPalette.set("SORT FLIP", false);
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
			name = "COMPLEMENT SAT";
			break;
		case 1:
			name = "COMPLEMENT BRT";
			break;
		case 2:
			name = "MONOCHROME SAT";
			break;
		case 3:
			name = "MONOCHROME BRT";
			break;
		case 4:
			name = "ANALOGUE";
			break;
		case 5:
			name = "TRIAD";
			break;
		case 6:
			name = "TRIAD COMPLEMENT";
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
	setupGui();

	//--

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

#ifdef USE_IM_GUI__QUANTIZER_INTERNAL
	colorQuantizer.setImGuiPointer(guiManager.getGui());
	//ofxImGui::Gui gui = guiManager.getGuiPtr();
	//colorQuantizer.setImGuiPointer(gui);
#endif

	//--

	// listeners

	addKeysListeners();
	addMouseListeners();

	color_Picked.addListener(this, &ofxColorManager::Changed_ColorPicked);
	color_Clicked.addListener(this, &ofxColorManager::Changed_ColorClicked);

	//------------------------------------------------------

	AppEnginesMode.set("AppEnginesMode", 0, 0, NUM_APP_MODES - 1);
	AppMode_name.set("", "");
	AppEnginesMode.setSerializable(false);

	// layouts panel
	params_LayoutSPanel.add(b0, b1, b2, b3, b4);
	ofAddListener(params_LayoutSPanel.parameterChangedE(), this, &ofxColorManager::Changed_LayoutPanels);

	//-

	// startup settings

	params_AppState.setName("ofxColorManager");

	params_engines.setName("Engines");

	params_engines.add(last_Index_Kit);
	params_engines.add(last_Index_Preset);

	params_engines.add(AppEnginesMode);
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
	params_engines.add(triadSpread);
	params_AppState.add(params_engines);

	// export colors
	params_Export.setName("ExportColors");
#ifndef USE_SIMPLE_PRESET_PALETTE	
	params_Export.add(bModeBundlePreset);
	params_Export.add(bModePalettePreset);
#endif
	params_Export.add(bAutoExportPreset);
	params_Export.add(bExportByFile);
	params_Export.add(bExportByTCP);
	params_Export.add(bExportPreset_DefaultPath);
	params_Export.add(path_Folder_ExportColor_Custom);
	params_AppState.add(params_Export);

	params_Panels.add(SHOW_ALL_GUI);
	params_Panels.add(SHOW_Presets);
	params_Panels.add(SHOW_Palette);
	params_Panels.add(SHOW_Editor);
#ifdef MODE_BACKGROUND
	params_Panels.add(SHOW_BackGround);
#endif
	params_Panels.add(SHOW_Library);
	params_Panels.add(SHOW_Picker);
	params_Panels.add(SHOW_Range);
	params_Panels.add(SHOW_Theory);
	params_Panels.add(SHOW_Engines);
	params_Panels.add(SHOW_Panels);
	params_Panels.add(SHOW_LinkExport);
	params_Panels.add(SHOW_Demos);
	params_Panels.add(SHOW_Quantizer);
	params_Panels.add(SHOW_ColourLovers);
	params_Panels.add(SHOW_BrowserColors);
	params_Panels.add(SHOW_MINI_Preview);
	params_Panels.add(SHOW_Kit);
	params_Panels.add(AutoScroll);
	params_Panels.add(SHOW_Gradient);
	//params_Panels.add(gradientEngine.SHOW_CurveEditor);
	params_AppState.add(params_Panels);

	//TODO: show bg color (gradient picker engine) on mode no background
#ifdef MODE_BACKGROUND 
	params_Background.add(color_BackGround);
	params_Background.add(color_BackGround_DarkerMode);
	params_Background.add(color_BackGround_GradientMode);
	params_Background.add(AutoSet_BackGround_Color);
	params_Background.add(color_BackGround_AmtDarker);
	params_Background.add(color_BackGround_Lock);
	params_AppState.add(params_Background);
#endif

	//TODO:
	//params_Picker.add(BRIGHTNESS);
	//params_Picker.add(SATURATION);

	setupDemos();

	params_Demo.add(DEMO1_Enable);
	params_Demo.add(DEMO1_Auto);
	params_Demo.add(DEMO1_Timer);
	params_Demo.add(DEMO1_Alpha);
	params_Demo.add(DEMO1_Cam);
	params_Demo.add(DEMO2_Enable);
	params_Demo.add(DEMO2_Spheres.alpha_);
	params_Demo.add(DEMO2_Spheres.speed);
	params_Demo.add(DEMO2_Spheres.zoom);
	params_Demo.add(DEMO2_Spheres.mouseCam);
	params_AppState.add(params_Demo);

	params_Picker.add(bColor_rHUE);
	params_Picker.add(bColor_rSAT);
	params_Picker.add(bColor_rBRG);
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
	params_Library.add(lib_Responsive_ModeGrid);
	params_Library.add(lib_Responsive_ModeFit);
	params_Library.add(bPagerized);
	params_Library.add(sizeLibColBox);
	params_AppState.add(params_Library);

	params_Palette2.add(sizePaletteBox);
	params_Palette2.add(scale_ColPalette);
	params_Palette2.add(bResponsive_Panels);
	params_Palette2.add(bFitLayout);
	params_Palette2.add(boxMaxRows);
	params_Palette2.add(bEditPalette);
	//params_Palette2.add(bAuto_Build_Palette);
	//params_Palette2.add(bAutoResizePalette);
	params_AppState.add(params_Palette2);
	params_AppState.add(fontSizeParam);
	params_AppState.add(fontSizeBigParam);

	params_AppState.add(SHOW_Layouts);
	//params_AppState.add(SHOW_LayoutsAdvanced);
	params_AppState.add(appLayoutIndex);
	params_AppState.add(SHOW_MenuBar);
	params_AppState.add(positionGuiLayout);
	params_AppState.add(bAutoHide_Name);
	params_AppState.add(bTweakPalette);
	params_AppState.add(bSortPalette);

	//params_AppState.add(SHOW_Advanced);
	SHOW_Advanced.setSerializable(false);

	//---

	// grouped for callback function

	params_control.setName("ofxColorManager");

	params_control.add(MODE_NewPreset);
	params_control.add(numColors_Engines);
	params_control.add(AppEnginesMode);
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
	params_control.add(SHOW_Engines);
	params_control.add(SHOW_Panels);
	params_control.add(SHOW_LinkExport);
	params_control.add(SHOW_Demos);
	params_control.add(SHOW_BrowserColors);
	params_control.add(SHOW_Presets);
#ifdef MODE_BACKGROUND
	params_control.add(SHOW_BackGround);
#endif
	params_control.add(SHOW_Library);
	params_control.add(SHOW_Theory);
	params_control.add(SHOW_Range);
	params_control.add(SHOW_Picker);
	params_control.add(SHOW_ColourLovers);
	params_control.add(SHOW_Quantizer);
	params_control.add(SHOW_ColourLovers_searcher);
	params_control.add(SHOW_Palette);
	params_control.add(SHOW_Editor);
	params_control.add(SHOW_ALL_GUI);
	//params_control.add(gradientEngine.SHOW_CurveEditor);
	params_control.add(SHOW_Gradient);

	params_control.add(bResponsive_Panels);
	params_control.add(bFitLayout);
	params_control.add(SHOW_Advanced);
	params_control.add(SHOW_MenuBar);
	//params_control.add(SHOW_Name);

#ifndef USE_SIMPLE_PRESET_PALETTE	
	params_control.add(bModeBundlePreset);
	params_control.add(bModePalettePreset);
#endif
	params_control.add(fontSizeParam);
	params_control.add(fontSizeBigParam);
	params_control.add(appLayoutIndex);
	params_control.add(Lock_DockingLayout);
	params_control.add(bAutoSave_Layout);
	params_control.add(SHOW_Layouts);
	params_control.add(SHOW_LayoutsAdvanced);
	params_control.add(bExportByTCP);
	params_control.add(last_Index_Kit);

	ofAddListener(params_control.parameterChangedE(), this, &ofxColorManager::Changed_Controls);

	refresh_DemoFboAbout();

	// included params on layouts manager
	params_LayoutPanelsState.add(SHOW_Palette);
	params_LayoutPanelsState.add(SHOW_PanelEngines);
	params_LayoutPanelsState.add(SHOW_Theory);
	params_LayoutPanelsState.add(SHOW_Range);
	params_LayoutPanelsState.add(SHOW_ColourLovers);
	params_LayoutPanelsState.add(SHOW_Quantizer);
	params_LayoutPanelsState.add(SHOW_Presets);
	params_LayoutPanelsState.add(SHOW_Kit);
	params_LayoutPanelsState.add(SHOW_Editor);
	params_LayoutPanelsState.add(SHOW_Picker);
	params_LayoutPanelsState.add(SHOW_Library);
	params_LayoutPanelsState.add(SHOW_Demos);
	params_LayoutPanelsState.add(SHOW_MINI_Preview);
	params_LayoutPanelsState.add(SHOW_LinkExport);
	params_LayoutPanelsState.add(SHOW_Advanced);
	params_LayoutPanelsState.add(SHOW_MenuBar);
	//params_LayoutPanelsState.add(bAutoHide_Name);
	//params_LayoutPanelsState.add(gradientEngine.SHOW_CurveEditor);
	params_LayoutPanelsState.add(SHOW_Gradient);

	//workflow
	////exclude. shared by all the layout presets
	params_LayoutPanelsState.add(SHOW_Panels);
	params_LayoutPanelsState.add(Lock_DockingLayout);


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

	//--

	// color ranges
	setupRange();

	//--

	//refresh theory and ranges
	refresh_Pick_ToEngines();

	// refresh hsb
	refresh_Pick_ToHSB();

	//--

	DEMO3_Svg.startup();

	//--

	setupLink();

	//--

	PRESET_Temp.setLinkPalette(palette);

	//--

	bLast_Index_Theory = true;
	bLast_Index_Range = true;
	if (last_Index_Theory > 0) theory_Types_G1[last_Index_Theory] = true;//on button
	if (last_Index_Range > 0) types_Range[last_Index_Range] = true;//on button

	//--

	// files paths

	refresh_Paths();

	//-

	// kits
	refresh_Kits();

	// preset manager
	preset_RefreshFiles();

	//--

	// load all app states

	ofxSurfingHelpers::loadGroup(params_AppState, path_AppState);


	//--

	// load last session preset index

	if (last_Index_Preset < files.size())
	{
		PRESET_Name = files_Names[last_Index_Preset];

		//--

		// name
		if (name_TARGET != nullptr) name_TARGET[0] = PRESET_Name;

		//preset_Load(PRESET_Name);// moved to frist frame update
	}

	ofSetCircleResolution(200);//called after all addons reso modifications

	//--

	// splash screen
	//splash.setup("../../../docs/itch.io/Paletto_Banner.png");
	splash.setup("assets/images/Paletto_Banner.png");

	//--

	// refresh
	fontSizeParam = fontSizeParam.get();
	fontSizeBigParam = fontSizeBigParam.get();

	//// load layout
	//appLayoutIndex_PRE = -1;
	//loadAppLayout(AppLayouts(appLayoutIndex.get()));
	//ImGui::LoadIniSettingsFromDisk(ini_to_load);//force here befor update
	//ini_to_load = NULL;

	resetLibraryLayout();
}

//--------------------------------------------------------------
void ofxColorManager::update(ofEventArgs & args)
{
	//--

	// layouts

	if (ini_to_load)
	{
		ImGui::LoadIniSettingsFromDisk(ini_to_load);
		ini_to_load = NULL;
	}

	if (ini_to_save)
	{
		ImGui::SaveIniSettingsToDisk(ini_to_save);
		ini_to_save = NULL;

		ofxSurfingHelpers::saveGroup(params_LayoutPanelsState, ini_to_save_Str + ".json");
	}

	//--

	//TODO:

	bBlockedKeys = bTextInputActive || bCheckMouseOverTextInputLovers;//text inputs

	static bool bBlockedKeys_PRE = false;
	//bBlockedKeys |= 
	if (bBlockedKeys != bBlockedKeys_PRE) {
		bBlockedKeys_PRE = bBlockedKeys;
		colorQuantizer.setActive(!bBlockedKeys);
		windowApp.setEnableKeys(!bBlockedKeys);
	}

	//TODO:
	//BUG:
	//bBlockedKeys = false;
	//sometime keys are locked disabled..

	//// text input blocker
	//if (!SHOW_ColourLovers) bBlockedKeys = !(ENABLE_keys && !bTextInputActive);
	//else bBlockedKeys = !(ENABLE_keys && !bTextInputActive && !bCheckMouseOverTextInputLovers);

	//--

	//time spaced log
	//if (ofGetFrameNum() % int(60 * 1.0) == 0) ofLog();

	//--

	// workaround
	// load preset after setup
	if (ofGetFrameNum() == 1)
	{
		preset_Load(PRESET_Name);
	}

	//--

	updateLink();

	//--

	// auto pilot: browse presets

	if (bPlaySlideShow && (ofGetElapsedTimeMillis() - auto_pilot_timer > auto_pilot_Duration * 1000))
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

	if (bFlag_refresh_EnginesFromPalette)
	{
		bFlag_refresh_EnginesFromPalette = false;

		refresh_EnginesFromPalette();
	}

	//--

	// export

	if (bExportFlag)// trig if flagged. generally, maximum once per frame
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

	// programmed open dialog from ImGui:

	if (bOpen)
	{
		setPathPresetWatch();
	}

	//--

	// gradient

	gradientEngine.update();

	//----

	// demos
	updateDemos();

	//----

	// lovers

	//colourLoversHelper.update();

	//----

	//TODO:
	// this flags are only used 
	// by quantizer and lover addons
	// it meAns that some module/addon has clicked/changed the main palette/color 
	// then we will get the name and the colors.
	//bUpdated_Palette_BACK = true;// to trig this

	if (bUpdated_Palette_BACK)
	{
		bUpdated_Palette_BACK = false;

		ofLogNotice(__FUNCTION__) << "> bUpdated_Palette_BACK : " << bUpdated_Palette_BACK;

		build_Palette_Engine();

		//--

		// export
		if (bAutoExportPreset)
		{
			bExportFlag = true;
		}
	}

	//----

	// 2. color pick from palette clicked

	if (bUpdated_Color_BACK)
	{
		bUpdated_Color_BACK = false;

		ofLogWarning(__FUNCTION__) << "> bUpdated_Color_BACK : " << bUpdated_Color_BACK;

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
		ofLogNotice(__FUNCTION__) << "> color_Clicked2 !";

		color_Clicked2_PRE = color_Clicked2;

		color_Picked.set(color_Clicked2);
	}

	//----

	// 4. set the local scope color pointer that is into ofxColorBrowser that whill be used as color picked too

	if (color_BACK != color_BACK_PRE)
	{
		color_BACK_PRE = color_BACK;

		ofLogNotice(__FUNCTION__) << ">  color_BACK ! ";

		color_Picked.set(color_BACK);
	}
}

//--------------------------------------------------------------
void ofxColorManager::refresh_Kits()
{
	string _path_KitsPresets = path_Global + path_Kits;

	files_KitNames.clear();

	ofDirectory dir(_path_KitsPresets);

	//dir.allowExt("png");
	dir.listDir();

	for (int i = 0; i < dir.size(); i++)
	{
		ofLogNotice(dir.getPath(i));
		files_KitNames.push_back(dir.getName(i));
	}
	last_Index_Kit.setMin(0);
	last_Index_Kit.setMax(files_KitNames.size() - 1);

	//std::vector<ofFile> _files;
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

	//--

	resetLibraryLayout();
}

////--------------------------------------------------------------
//void ofxColorManager::draw_PresetName()
//{
//	//ofPushMatrix();
//	ofPushStyle();
//
//	//blink when a new preset is editing
//	float a;
//	if (MODE_NewPreset) a = ofxSurfingHelpers::getFadeBlink();
//	else a = 1.0f;
//
//	//-
//
//	float sp = 2;//shadow
//
//	int padh = 0;
//	int pady;
//	int x;
//	int y;
//	int h;
//
//	std::string str = "";
//	if (name_TARGET != nullptr) name_TARGET[0];
//
//	//get text width
//	float _w0 = ofxSurfingHelpers::getWidthBBtextBoxed(fontBig, str);
//
//	//--
//
//	//// a. top
//	//pady = 150;//with upper palette
//	////pady = 20;
//	//y = pady + fontBig.getSize();
//	//h = padh;
//	//y += h;
//	//x = ofGetWidth() * 0.5 - _w0 * 0.5;
//
//	//// b. bottom
//	//pady = 0;
//	//y = ofGetWindowHeight() - pady - fontBig.getSize();
//	//h = padh;
//	//y += h;
//	//x = ofGetWidth() * 0.5 - _w0 * 0.5;
//
//	// c. locked to svg demo
//	x = DEMO3_Svg.getPositionTittle().x;
//	y = DEMO3_Svg.getPositionTittle().y;
//	pady = fontBig.getSize() / 2;
//	y -= pady;
//	x -= _w0 * 0.5;//center
//
//	//--
//
//	enum THEME_COLORS
//	{
//		THEME_DAY = 0,
//		THEME_NIGHT
//
//	};
//
//	THEME_COLORS _THEME = THEME_DAY;
//	//THEME_COLORS _THEME_SCHEME = THEME_NIGHT;
//
//	//-
//
//	// label tittle
//
//	int _alpha = 200;
//	ofColor c0, c1;//shadow and letter colors
//
//	if (_THEME == THEME_NIGHT)
//	{
//		c0.set(0, _alpha * a * 0.9);
//		c1.set(240, _alpha * a);
//	}
//	else if (_THEME == THEME_DAY)
//	{
//		c0.set(164, _alpha * a);
//		c1.set(0, _alpha * a * 0.9);
//	}
//
//	ofSetColor(c0);
//	fontBig.drawString(str, x + sp, y + sp);
//	ofSetColor(c1);
//	fontBig.drawString(str, x, y);
//
//	//-
//
//	ofPopStyle();
//	//ofPopMatrix();
//}

//--------------------------------------------------------------
void ofxColorManager::draw(ofEventArgs & args)
{
	if (SHOW_Scene)
	{
		// background

		ofColor _cBg;

		// background mode
#ifdef MODE_BACKGROUND
		if (background_Draw_ENABLE)
		{
			_cBg = color_BackGround.get();
		}
#endif
		// using picked color gradient 
#ifdef USE_SVG_MASK
		if (!DEMO3_Svg.DEMO2_BgWhite || (DEMO3_Svg.DEMO3_Enable && DEMO3_Svg.fileIndex == 0))
#endif
			//if (!DEMO3_Svg.DEMO3_Enable || (DEMO3_Svg.DEMO3_Enable && DEMO3_Svg.fileIndex == 0))
		{
			_cBg = gradientEngine.getColorPicked();
		}

		ofClear(_cBg);

		//--

		draw_Demos();

		//--

		// about window

		if (SHOW_About)
		{
			fboBig.begin();
			ofClear(gradientEngine.getColorPicked());
			DEMO2_Spheres.draw(ofGetWindowRect());
			fboBig.end();
		}
		//if (DEMO1_Enable) fboBig.draw(0, 0, ofGetWidth(), ofGetHeight());

		//--

		if (SHOW_ImGui)
		{
			// quantizer
#ifdef USE_IMAGE_QUANTIZER
			if (SHOW_Quantizer) colorQuantizer.draw();
#endif
			//--

			//// info
			//if (SHOW_Name) draw_PresetName();
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

	//if (gradientEngine.SHOW_CurveEditor) gradientEngine.draw();
	if (SHOW_Gradient) gradientEngine.draw();

	//--

	// gui

	if (SHOW_ImGui)
	{
		if (guiVisible)
		{
			draw_Gui();

			/*
			//mouseLockedByGui = draw_Gui();
			//TODO:
			//BUG:
			//bBlockedKeys = false;
			//sometime keys are locked disabled..

			//if (mouseLockedByGui != mouseLockedByGui_PRE)// changed
			//{
			//	mouseLockedByGui_PRE = mouseLockedByGui;
			//	if (mouseLockedByGui)
			//	{
			//		ENABLE_keys = false;
			//	}
			//	else
			//	{
			//		ENABLE_keys = true;
			//	}

			//	//ofLogVerbose(__FUNCTION__) << "mouseLockedByGui: " << (mouseLockedByGui ? " IN" : " OUT");
			//	ofLogNotice(__FUNCTION__) << "mouseLockedByGui: " << (mouseLockedByGui ? " IN" : " OUT");

			//	//--

			//	//TODO:
			//	// lovers
			//	bool b = false;
			//	if (SHOW_ColourLovers) {
			//		b = !mouseLockedByGui;
			//	}
			//	else {
			//		b = false;
			//	}
			//	colourLoversHelper.setEnableKeys(b);
			//}
			*/
		}
	}

	//----

	// help info

	if (SHOW_HelpInfo)
	{
		ofPushMatrix();
		int w = ofxSurfingHelpers::getWidthBBtextBoxed(font, helpInfo);
		int h = ofxSurfingHelpers::getHeightBBtextBoxed(font, helpInfo);
		int xoffset = 0;
		int yoffset = 0;
		int x = ofGetWidth() * 0.5 - w * 0.5 + xoffset;
		int y = ofGetHeight() * 0.5 - h * 0.5 + yoffset;
		{
			//center box
			ofTranslate(x, y);
			ofSetColor(255, 255);
			//x = 0, int y = 0, ofColor font0_Color = 255, ofColor colorBackground = ofColor(0, 247), bool useShadow = false, ofColor colorShadow = 128
			ofxSurfingHelpers::drawTextBoxed(font, helpInfo, 0, 0, 255, 0, true, 64);
		}
		ofPopMatrix();
	}

	//--

	//TODO: 
	//BUG:
	//disabled autodraw here but not working and draws in front
	//if gui.setup(..) is called in another ImGui instance 
	//like in this case the ImGui from ofxColourLovers
	//if (!splash.isSplashing())
	//gui.draw();

	//--

	// splash screen (in front gui)

	splash.draw();
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
	ofRemoveListener(params_Ranges.parameterChangedE(), this, &ofxColorManager::Changed_Range);
	ofRemoveListener(params_ColorTheory_G1.parameterChangedE(), this, &ofxColorManager::Changed_ColorTheory);
	ofRemoveListener(params_ColorTheory_G2.parameterChangedE(), this, &ofxColorManager::Changed_ColorTheory);
	ofRemoveListener(params_Palette.parameterChangedE(), this, &ofxColorManager::Changed_ParamsPalette);
	ofRemoveListener(params_LayoutSPanel.parameterChangedE(), this, &ofxColorManager::Changed_LayoutPanels);

	//--

	ofRemoveListener(ofEvents().update, this, &ofxColorManager::update);
	ofRemoveListener(ofEvents().draw, this, &ofxColorManager::draw);
	//ofRemoveListener(ofEvents().fileDragEvent, this, &ofxColorManager::dragEvent);

	removeKeysListeners();
	removeMouseListeners();

	ofRemoveListener(ofEvents().mouseDragged, this, &ofxColorManager::mouseDragged);
	ofRemoveListener(ofEvents().mouseScrolled, this, &ofxColorManager::mouseScrolled);

#ifdef LINK_TCP_MASTER_CLIENT_KU
	TCP_Sender.close();
#endif

	//-

	//TODO:
	//fails and overwrites ??
	//if (bAutoSave_Layout)
	//{
	//	//force to ensure save bc update chain load and save below
	//	appLayoutIndex_PRE = -1;
	//	saveAppLayout(AppLayouts(appLayoutIndex.get()));
	//	
	//	ini_to_save = ini_to_save_Str.c_str();//flags to save on update
	//	ImGui::SaveIniSettingsToDisk(ini_to_save);
	//}


#ifdef USE_VERSION_CHECKER
	_appUpdateNotifier.exit();
#endif
}

//----

// engines

//--------------------------------------------------------------
void ofxColorManager::gui_Theory()
{
	static bool auto_resize = false;

	float ww, hh;
	ww = PANEL_WIDGETS_WIDTH;
	hh = PANEL_WIDGETS_HEIGHT;
	ImGui::SetWindowSize(ImVec2(ww, hh));

	ImGuiWindowFlags flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;
	flags |= flagsWindowsLocked;

	const float butlabelw = 140;// width label text

	//offset
	float _offset = 23;//extra height to include extra slider for analog
	//float _offset = 35;//extra height to include extra slider for analog

	int ii = 0;

	//--

	//ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(2 * PANEL_WIDGETS_WIDTH, 800));

	guiManager.beginWindow(SHOW_Theory, flags);
	//guiManager.beginWindow("THEORY", flags);
	//if (ofxImGui::BeginWindow("THEORY", mainSettings, flags))
	{
		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		// amount colors
		ImGui::PushItemWidth(110);
		if (ofxImGuiSurfing::AddIntStepped(numColors_Engines)) {}
		ImGui::PopItemWidth();

		ImGui::Dummy(ImVec2(0.0f, 2.0f));

		//TODO:
		float _spcx;
		float _spcy;
		float _w100;
		float _h100;
		float _w99;
		float _w50;
		float _w33;
		float _w25;
		float _h;
		ofxImGuiSurfing::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);
		_h *= 2;

		float _w49 = _w50 - 2;
		//float _h100;
		float _hSz2;

		// box size
		float _wSz = ((_w99 - butlabelw - _spcx - 8) / numColors_Engines);
		float _hSz = 32;

		float _wSz2;// for when amount colors differs the expected due to some theory limitations

		int _numTheories = int(NUM_COLOR_THEORY_TYPES_G1 + NUM_COLOR_THEORY_TYPES_G2);

		//--

		ImGuiColorEditFlags _flags =
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

		// 1. mini box color

		ImGui::Text("Base Color");

		if (ImGui::ColorButton("##BoxTheory", *(ImVec4 *)&tmpRef.r, _flags, ImVec2(_w99, _h))) {}

		//-

		// 2. picker

		if (ImGui::CollapsingHeader("PICKER", ImGuiWindowFlags_NoCollapse))
		{
			// square/wheel

			_flags =
				ImGuiColorEditFlags_NoSmallPreview |
				ImGuiColorEditFlags_NoInputs |
				ImGuiColorEditFlags_NoTooltip |
				ImGuiColorEditFlags_NoLabel |
				ImGuiColorEditFlags_NoSidePreview |
				ImGuiColorEditFlags_NoAlpha |
				ImGuiColorEditFlags_PickerHueBar;
			//ImGuiColorEditFlags_PickerHueWheel;

			float _www;
			if (_w100 >= MAX_PICKER_WIDTH) _www = MAX_PICKER_WIDTH;
			else if (_w100 < MAX_PICKER_WIDTH) _www = MIN(_w49, PANEL_WIDGETS_WIDTH);
			ImGui::PushItemWidth(_www);

			if (ImGui::ColorPicker4("##cPickerTheory", (float *)&tmpRef, _flags))
			{
				ofLogNotice(__FUNCTION__) << "Square Picker Theory : moved";

				color_TheoryBase.set(tmpRef);

				if (bAuto_Theory_FromPicker) color_Picked = color_TheoryBase.get();

				bUpdate = true;
			}

			ImGui::PopItemWidth();
		}

		//----

		// controls

		ImGui::Dummy(ImVec2(0.0f, 2.0f));

		if (SHOW_Advanced)
		{
			if (ImGui::CollapsingHeader("Advanced"))
			{
				ofxImGui::AddParameter(bAuto_Theory_FromPicker);
				ofxImGui::AddParameter(bAuto_Build_Palette);
				//ofxImGui::AddParameter(analogSpread);
				//// layout
				//ImGui::Dummy(ImVec2(0.0f, 5.0f));
				//ImGui::Checkbox("Auto-Resize", &auto_resize);
			}
		}

		//--

		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		//----

		ImGuiColorEditFlags _flagsc =
			ImGuiColorEditFlags_NoAlpha |
			ImGuiColorEditFlags_NoPicker |
			ImGuiColorEditFlags_NoLabel |
			ImGuiColorEditFlags_NoDragDrop |
			ImGuiColorEditFlags_NoTooltip;

		//--

		_h100 = ImGui::GetContentRegionAvail().y;
		_hSz2 = ((_h100 - _offset) / _numTheories) - _spcy;

		//--

		// 1. G1

		for (int i = 0; i < NUM_COLOR_THEORY_TYPES_G1; i++)
		{
			// border to selected

			bool bDrawBorder = false;

			if (i == last_Index_Theory && bLast_Index_Theory)
			{
				bDrawBorder = true;

				ImGui::PushStyleColor(ImGuiCol_Border, borderLineColor);
				ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, borderLineWidth);
				const ImVec4 colorActive = style->Colors[ImGuiCol_ButtonHovered];
				ImGui::PushStyleColor(ImGuiCol_Button, colorActive);
			}

			//-

			// 1.1 label button G1

			//align
			//ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(labelPadding, 0.5f));
			//std::string _label = ColorWheelSchemes::colorSchemeNames[i];
			if (ofxImGuiSurfing::AddSmallButton(theory_Types_G1[i], butlabelw, _hSz2))
			{
				last_Index_Theory_PickPalette = i;

				// workflow
				if (bEditPalette) bEditPalette = false;
				bLast_Index_Theory = true;
			}
			//ImGui::PopStyleVar();

			//-

			if (bDrawBorder)
			{
				ImGui::PopStyleColor(2);
				ImGui::PopStyleVar();
			}

			//--

			ImGui::SameLine();

			//--

			// 1.2 colors G1 

			int _szT = colors_Theory_G1[i].size();

			for (int n = 0; n < _szT; n++)
			{
				ofColor _c = colors_Theory_G1[i][n];

				if (n == 0) ImGui::SameLine();
				else ImGui::SameLine(0, 0);

				//--

				//we relayout sizes 
				//when not expected amount of colors, due to different theory types: 
				//ie: even allways

				//normal expected free width after label button
				float _ww = numColors_Engines * _wSz;

				//some theories have even amount or a fixed size as triad
				//total is the exact expected amount

				if (_szT == numColors_Engines)
				{
					_wSz2 = _wSz;
				}
				else if (_szT > numColors_Engines)
				{
					_wSz2 = _ww / _szT;
				}
				else if (_szT < numColors_Engines)
				{
					_wSz2 = _ww / _szT;
				}

				//ii = n + i * n;
				ImGui::PushID(ii);
				string ss = "##cThyG1" + ofToString(ii);
				ii++;

				//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
				if (ImGui::ColorButton(ss.c_str(), _c, _flagsc, ImVec2(_wSz2, _hSz2)))
				{
					ofLogWarning(__FUNCTION__) << "click: " << i << "," << n;

					color_Picked.set(_c);
					bUpdate = true;
				}
				//ImGui::PopStyleVar();

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
			case 0: _total = complement.size(); break;
			case 1: _total = complementBrightness.size(); break;
			case 2: _total = monochrome.size(); break;
			case 3: _total = monochromeBrightness.size(); break;
			case 4: _total = analogue.size(); break;
			case 5: _total = triad.size(); break;
			case 6: _total = complementTriad.size(); break;
			}

			//-

			// border to selected

			bool bDrawBorder = false;

			if ((NUM_COLOR_THEORY_TYPES_G1 + i) == last_Index_Theory && bLast_Index_Theory)
			{
				//bDrawBorder = true && !bEditPalette;// workflow
				bDrawBorder = true;

				ImGui::PushStyleColor(ImGuiCol_Border, borderLineColor);
				ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, borderLineWidth);
				const ImVec4 colorActive = style->Colors[ImGuiCol_ButtonHovered];
				ImGui::PushStyleColor(ImGuiCol_Button, colorActive);
			}

			//-

			// 2.1 label button G2

			//align
			//ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(labelPadding, 0.5f));
			if (ofxImGuiSurfing::AddSmallButton(types_Theory_G2[i], butlabelw, _hSz2))
			{
				last_Index_Theory_PickPalette = NUM_COLOR_THEORY_TYPES_G1 + i;

				// workflow
				if (bEditPalette) bEditPalette = false;

				bLast_Index_Theory = true;
			}
			//ImGui::PopStyleVar();

			//-

			if (bDrawBorder)
			{
				ImGui::PopStyleColor(2);
				ImGui::PopStyleVar(1);
			}

			//-

			ImGui::SameLine();

			for (int n = 0; n < _total; n++)
			{
				ofColor _c;

				if (n == 0) ImGui::SameLine();
				else ImGui::SameLine(0, 0);

				std::string _name;
				switch (i)
				{
				case 0:
					if (n < complement.size()) _c = complement[n];
					_name = "COMPLEMENT";
					break;
				case 1:
					if (n < complementBrightness.size()) _c = complementBrightness[n];
					_name = "COMPLEMENT BRT";
					break;
				case 2:
					if (n < monochrome.size()) _c = monochrome[n];
					_name = "MONOCHROME";
					break;
				case 3:
					if (n < monochromeBrightness.size()) _c = monochromeBrightness[n];
					_name = "MONOCHROME BRT";
					break;
				case 4:
					if (n < analogue.size()) _c = analogue[n];
					_name = "ANALOGUE";
					break;
				case 5:
					if (n < triad.size()) _c = triad[n];
					_name = "TRIAD";
					break;
				case 6:
					if (n < complementTriad.size()) _c = complementTriad[n];
					_name = "TRIAD COMPLEMENT";
					break;
				}

				//--

				//we relayout sizes 
				//when not expected amount of colors, due to different theory types: 
				//ie: even allways

				//normal expected free width after label button
				float _ww = numColors_Engines * _wSz;

				//some theories have even amount or a fixed size as triad
				//total is the exact expected amount

				if (_total == numColors_Engines)
				{
					_wSz2 = _wSz;
				}
				else if (_total > numColors_Engines)
				{
					_wSz2 = _ww / _total;
				}
				else if (_total < numColors_Engines)
				{
					_wSz2 = _ww / _total;
				}

				//-

				// 2.2 colors G2 

				//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

				//ii = n + (i + NUM_COLOR_THEORY_TYPES_G1) * n;
				ImGui::PushID(ii);
				std::string ss = "##cThyG2" + ofToString(ii);
				ii++;

				if (ImGui::ColorButton(ss.c_str(), _c, _flagsc, ImVec2(_wSz2, _hSz2)))
				{
					ofLogWarning(__FUNCTION__) << "click: " << i + NUM_COLOR_THEORY_TYPES_G1 << "," << n;

					color_Picked.set(_c);
					bUpdate = true;
				}

				ImGui::PopID();
			}
		}

		//----

		// analog has an extra tweak
		if (last_Index_Theory == 12) ofxImGui::AddParameter(analogSpread);
		if (last_Index_Theory == 14) ofxImGui::AddParameter(triadSpread);

		//----

		if (bUpdate)
		{
			refresh_FromPicked();
		}
	}
	//ofxImGui::EndWindow(mainSettings);
	guiManager.endWindow();

	//ImGui::PopStyleVar();
}

//--------------------------------------------------------------
void ofxColorManager::gui_Editor()
{
	static bool auto_resize = true;

	float ww = PANEL_WIDGETS_WIDTH;
	float hh = PANEL_WIDGETS_HEIGHT;
	ImGui::SetWindowSize(ImVec2(ww, hh));

	ImGuiWindowFlags flagsw;
	flagsw = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;
	flagsw |= flagsWindowsLocked;

	//blink when a new preset is editing
	float a = ofxSurfingHelpers::getFadeBlink();

	//--

	//ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(PANEL_WIDGETS_WIDTH, PANEL_WIDGETS_HEIGHT));
	
	guiManager.beginWindow(SHOW_Editor, flagsw);
	//if (ofxImGui::BeginWindow("EDITOR", mainSettings, flagsw))

	{
		float _spcx;
		float _spcy;
		float _w100;
		float _h100;
		float _w99;
		float _w50;
		float _w33;
		float _w25;
		float _h;
		ofxImGuiSurfing::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);
		_h *= 2;

		ImGuiColorEditFlags _flags = ImGuiColorEditFlags_None;

		//--

		{
			// responsive
			ImVec2 button_sz((float)sizePaletteBox.get(), (float)sizePaletteBox.get());
			int int_amtBtn = palette.size();
			float _wx2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

			//--

			// 1. all palette color boxes

			for (int n = 0; n < palette.size(); n++)
			{
				ImGui::PushID(n);
				{
					//// a. fixed size. splitted by rows
					//int _max = 6;//amount colors per row
					//int _r = (int)MIN(_max, palette.size());
					////split in rows
					//if ((n % _r) != 0)
					//{
					//	if (n != 0) ImGui::SameLine(0, 0);
					//	//if (n != 0) ImGui::SameLine();
					//}

					// b. fit for width
					int _r = palette.size();
					if (n != 0) ImGui::SameLine(0, 0);

					//----

					float wb = (_w100 / _r);
					//float hb = WIDGETS_HEIGHT / 2;
					float hb = _h;
					//float hb = 40;

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

					ImVec2 bb = ImVec2(wb, hb);

					//-

					// border to selected
					bool bDrawBorder = false;
					if (n == last_Index_ColorPalette && bEditPalette)
					{
						bDrawBorder = true;
						ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, borderLineWidth + 1);
						ImGui::PushStyleColor(ImGuiCol_Border, borderLineColor);
					}

					//-

					// each color
					ofFloatColor _cc = palette[n];
					if (ImGui::ColorButton("##paletteDragEditor", _cc, _flags, bb))
					{
						last_Index_ColorPalette = n;

						// workflow
						if (!bEditPalette) bEditPalette = true;

						bFlag_refresh_EnginesFromPalette = true;

						//// workflow
						//if (bAuto_Color1_FromPicker_Range) color_1_Range = _cc;
						//else if (bAuto_Color2_FromPicker_Range) color_2_Range = _cc;
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

								last_Index_ColorPalette = n;

								refresh_Background();
							}
							if (mode == Mode_Swap)
							{
								const ofColor tmp = palette[n];
								palette[n] = palette[payload_n];
								palette[payload_n] = tmp;

								last_Index_ColorPalette = n;

								refresh_Background();
							}

							//--

							// update all palette (Curve)
							ofLogNotice(__FUNCTION__) << "DONE Dragged";

							gradientEngine.build_From_TARGET();

							// workflow
							bFlag_refresh_EnginesFromPalette = true;

							//--

							// export
							if (bAutoExportPreset)
							{
								bExportFlag = true;
							}
						}
						ImGui::EndDragDropTarget();
					}

					//----
				}
				ImGui::PopID();
			}

			ImGui::Dummy(ImVec2(0.0f, 2.0f));

			//----

			ofxImGuiSurfing::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);
			_h *= 2;

			//TODO:
			//add here to for using when presets panel is hidden
			// new preset 
			// manual toggle
			if (!SHOW_Presets)
			{
				float _hh = _h / 2;//smaller

				if (ofxImGuiSurfing::AddBigToggle(MODE_NewPreset, _w100, _h))
				{
					if (name_TARGET != nullptr) textInput_New = name_TARGET[0];
					if (textInput_New == "") textInput_New = "type name";
				}

				// new preset

				if (MODE_NewPreset.get())
				{
					// 4. save new

					ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.0f, a));
					if (ImGui::Button("SAVE NEW", ImVec2(_w100, _h * 2)))
					{
						ofLogNotice(__FUNCTION__) << "SAVE NEW: " << textInput_New;

						//NOTE: preset filename must to not include any extra '.' char
						//clean all extra '.' chars
						ofStringReplace(textInput_New, ".", "");

						//has_focus = 0;
						MODE_NewPreset = false;
						MODE_ReadyToUpdate = true;

						if (textInput_New != "")
						{
							ofLogNotice(__FUNCTION__) << "textInput_New: " << textInput_New;

							//--

							//TODO:
							//not working

							// check overwrite

							string _pathfile = path_Presets + textInput_New + ".json";
							bAlertFileOverWrite = ofxSurfingHelpers::ofxKuFileExists(_pathfile);
							ofLogNotice(__FUNCTION__) << "File " << _pathfile << " " << (bAlertFileOverWrite ? "FOUND" : "NOT FOUND");
							bool bSkipSave = false;

							if (bAlertFileOverWrite) ImGui::OpenPopup("OVERWRITE");

							if (ImGui::BeginPopupModal("OVERWRITE", NULL, ImGuiWindowFlags_AlwaysAutoResize))
							{
								ImGui::Text("Current Preset will be overwritten.\nThis operation cannot be undone!\n\n");
								ImGui::Separator();

								if (ImGui::Button("OK", ImVec2(_w50, 0)))
								{
									ofLogNotice(__FUNCTION__) << "OVERWRITE";
									bSkipSave = false;
									bAlertFileOverWrite = false;

									ImGui::CloseCurrentPopup();
								}

								ImGui::SetItemDefaultFocus();
								ImGui::SameLine();

								if (ImGui::Button("CANCEL", ImVec2(_w50, 0)))
								{
									ofLogNotice(__FUNCTION__) << "CANCEL";
									bSkipSave = true;
									bAlertFileOverWrite = false;

									ImGui::CloseCurrentPopup();
								}

								ImGui::EndPopup();
							}

							//--

							if (!bSkipSave)
							{
								preset_Save(textInput_New);
								preset_RefreshFiles(false);
							}

							//--

							// workflow
							//go to ne index after adding a new preset
							refresh_FilesSorting(textInput_New);
						}
						else ofLogError(__FUNCTION__) << "Empty name on textInput !";
					}
					ImGui::PopStyleColor();
				}
				else
				{
					if (MODE_ReadyToUpdate)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.0f, a));

						if (ImGui::Button("UPDATE", ImVec2(_w100, _hh)))
						{
							//TODO:
							//should re load by same name and get what position on vector
							//is to reload current preset number
							//textInput_temp = ofToString(tab2);
							//ofLogNotice(__FUNCTION__) << "textInput_temp:" << textInput_temp;

							//PRESET_Name = textInput_temp;
							textInput_New = PRESET_Name;

							ofLogNotice(__FUNCTION__) << "UPDATE : " << PRESET_Name;

							//save/update
							preset_Save(PRESET_Name);
							preset_RefreshFiles(false);
						}
						ImGui::PopStyleColor();
					}
				}

				ImGui::Dummy(ImVec2(0.0f, 5.0f));
			}

			//----

			ImGui::Dummy(ImVec2(0.0f, 2.0f));

			// 2. edit

			//TODO:
			//blink when enabled. not wworking
			//if (bEditPalette.get()) 
			//{
			//	ImGui::PushStyleColor(ImGuiCol_Border, borderLineColor);
			//	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, borderLineWidth);
			//	//ImGui::PushID(1);
			//	//ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.5f, a));
			//}

			ofxImGuiSurfing::AddBigToggle(bEditPalette, _w100, _h / 2);

			//if (bEditPalette.get())
			//{
			//	ImGui::PopStyleColor();
			//	ImGui::PopStyleVar(1);
			//	//ImGui::PopStyleColor();
			//	////ImGui::PopID();
			//}

			//-

			// 1. EDIT

			if (bEditPalette)
			{
				ImGui::Indent();
				ofxImGuiSurfing::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);
				_h *= 2;

				//--

				//selected color
				ImGui::PushItemWidth(-70);
				if (ofxImGui::AddParameter(last_Index_ColorPalette)) {
				};
				ImGui::PopItemWidth();
				//apply to selected color
				ofxImGuiSurfing::AddBigButton(bRemoveColor, _w50, _h / 2);
				ImGui::SameLine();
				ofxImGuiSurfing::AddBigButton(bAddColor, _w50, _h / 2);
				ofxImGuiSurfing::AddBigButton(bClearPalette, _w50, _h / 2);
				//random
				if (bColor_rHUE || bColor_rSAT || bColor_rBRG)
				{
					ImGui::SameLine();
					ofxImGuiSurfing::AddBigButton(bRandomColor, _w50, _h / 2);
				}
				//amount colors
				if (!SHOW_Presets && !SHOW_ColourLovers && SHOW_Engines)
				{
					if (ImGui::Button("-", ImVec2(_w50, _h / 2)))
					{
						numColors_Engines--;
					}
					ImGui::SameLine();
					if (ImGui::Button("+", ImVec2(_w50, _h / 2)))
					{
						numColors_Engines++;
					}
				}
				ImGui::Dummy(ImVec2(0.0f, 5.0f));

				ImGui::Unindent();
			}

			ImGui::Dummy(ImVec2(0.0f, 2.0f));

			//--

			ofxImGuiSurfing::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);
			_h *= 2;

			// 2. TWEAK

			// aux palette tweaker

			if (ofxImGuiSurfing::AddBigToggle(bTweakPalette, _w100, _h / 2))
			{
				if (bTweakPalette) {//get palette
					palette_AUX = palette;
				}
			};

			if (bTweakPalette)
			{
				ImGui::Indent();

				if (ImGui::Button("GET", ImVec2(_w50, _h / 2)))
				{
					doGetPaletteToTweaker();
				}
				ImGui::SameLine();
				if (ImGui::Button("APPLY", ImVec2(_w50, _h / 2)))
				{
					doApplyTweaker();
				}

				//if (ImGui::CollapsingHeader("TWEAKER", ImGuiWindowFlags_NoCollapse))

				// colors
				for (int n = 0; n < palette_AUX.size(); n++)
				{
					int _r = palette_AUX.size();
					if (n != 0) ImGui::SameLine(0, 0);

					float wb = (_w100 / _r);
					float hb = _h;
					//float hb = 40 / 2;
					//float hb = 40;
					//float hb = WIDGETS_HEIGHT / 2;

					_flags =
						ImGuiColorEditFlags_NoSmallPreview |
						ImGuiColorEditFlags_NoTooltip |
						ImGuiColorEditFlags_NoLabel |
						ImGuiColorEditFlags_NoSidePreview |
						ImGuiColorEditFlags_NoInputs |
						ImGuiColorEditFlags_NoAlpha;

					ImVec2 bb = ImVec2(wb, hb);
					ImGui::PushID(n);
					{
						ofFloatColor _cc = palette_AUX[n];
						if (ImGui::ColorButton("##paletteAux", _cc, _flags, bb))
						{
						}
					}
					ImGui::PopID();
				}

				//if (ImGui::Button("GET", ImVec2(_w50, _h / 2)))
				//{
				//	saturationTweak = 0;
				//	brigthnesTweak = 0;
				//	palette_AUX = palette;
				//}
				//ImGui::SameLine();
				//if (ImGui::Button("APPLY", ImVec2(_w50, _h / 2)))
				//{
				//	palette = palette_AUX;
				//}

				if (bTweakPalette)
				{
					//float value_p = 0.5;
					//float minv = 0;
					//float maxv = 1;
					//ofxImGui::Knob("knob1", &value_p, minv, maxv);

					//static float val = 0.0;
					//ofxImGui::Knob("value", &val, 0, 1); ImGui::SameLine();
					//ImGui::KnobNeedleTrail("value", &val, 0, 1, 0);
					//ofxSurfingHelpers::AddKnob(hueTweak);// ImGui::SameLine();
					////ofxSurfingHelpers::AddKnob(saturationTweak); ImGui::SameLine();
					////ofxSurfingHelpers::AddKnob(brigthnesTweak);

					const float RATIO_ABS = 4.0f;
					const float RATIO_MIN = 1.0f / RATIO_ABS;
					const float RATIO_MAX = RATIO_ABS;

					ImGui::PushItemWidth(-70);

					if (ofxImGui::AddParameter(hueTweak)) {
						for (int i = 0; i < palette.size(); i++)
						{
							float _hue = palette[i].getHue();
							float _hueTweak;

							if (hueTweak >= -1 && hueTweak <= 0)
								_hueTweak = ofMap(hueTweak, -1, 0, RATIO_MIN, 1);
							else if (hueTweak > 0 && hueTweak <= 1)
								_hueTweak = ofMap(hueTweak, 0, 1, 1, RATIO_MAX);
							_hueTweak *= _hue;

							palette_AUX[i].setHue(_hueTweak);
						}
					};

					if (ofxImGui::AddParameter(saturationTweak)) {
						for (int i = 0; i < palette.size(); i++)
						{
							float _sat = palette[i].getSaturation();
							float _satTweak;

							if (saturationTweak >= -1 && saturationTweak <= 0)
								_satTweak = ofMap(saturationTweak, -1, 0, RATIO_MIN, 1);
							else if (saturationTweak > 0 && saturationTweak <= 1)
								_satTweak = ofMap(saturationTweak, 0, 1, 1, RATIO_MAX);
							_satTweak *= _sat;

							palette_AUX[i].setSaturation(_satTweak);
						}
					};

					if (ofxImGui::AddParameter(brigthnesTweak)) {
						for (int i = 0; i < palette.size(); i++)
						{
							float _brg = palette[i].getBrightness();
							float _brgTweak;

							if (brigthnesTweak >= -1 && brigthnesTweak <= 0)
								_brgTweak = ofMap(brigthnesTweak, -1, 0, RATIO_MIN, 1);
							else if (brigthnesTweak > 0 && brigthnesTweak <= 1)
								_brgTweak = ofMap(brigthnesTweak, 0, 1, 1, RATIO_MAX);
							_brgTweak *= _brg;

							palette_AUX[i].setBrightness(_brgTweak);
						}
					};

					if (ImGui::Button("INVERT", ImVec2(_w100, _h / 2)))
					{
						invert(palette_AUX);
					}

					ImGui::PopItemWidth();
				}

				ImGui::Unindent();

				ImGui::Dummy(ImVec2(0.0f, 5.0f));
			}

			//--

			// 3. sorting

			ImGui::Dummy(ImVec2(0.0f, 2.0f));
			ofxImGuiSurfing::AddBigToggle(bSortPalette, _w100, _h / 2);
			//if (ImGui::CollapsingHeader("ORDERING", ImGuiWindowFlags_None))
			if (bSortPalette)
			{
				ImGui::Indent();

				ofxImGuiSurfing::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);
				_h *= 2;
				{
					// sort shift

					if (ImGui::Button("< SHIFT", ImVec2(_w50, _h / 2)))
					{
						build_Palette_SortShift(true);
					}
					ImGui::SameLine();
					if (ImGui::Button("SHIFT >", ImVec2(_w50, _h / 2)))
					{
						build_Palette_SortShift();
					}

					// sort flip

					if (ofxImGuiSurfing::AddSmallButton(bFlipUserPalette, _w50, _h / 2)) {}
					ImGui::SameLine();

					// sort random

					if (ImGui::Button("SORT RANDOM", ImVec2(_w50, _h / 2)))
					{
						build_Palette_SortRandom();
					}
				}

				ImGui::Text("Drag"); //ImGui::SameLine();
				if (ImGui::RadioButton("Copy", mode == Mode_Copy)) { mode = Mode_Copy; } ImGui::SameLine();
				if (ImGui::RadioButton("Swap", mode == Mode_Swap)) { mode = Mode_Swap; }

				ImGui::Dummy(ImVec2(0.0f, 5.0f));

				ImGui::Unindent();
			}

			//--

			//// coolors.co

			////ImGui::Dummy(ImVec2(0.0f, 2.0f));

			//// https://coolors.co/3d5a80-98c1d9-e0fbfc-ee6c4d-293241

			//if (ImGui::CollapsingHeader("COOLORS.CO", ImGuiWindowFlags_None))
			//{
			//	ofxImGuiSurfing::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);

			//	if (ImGui::Button("IMPORT", ImVec2(_w50, _h / 2)))
			//	{
			//		ofLogNotice(__FUNCTION__) << "IMPORT URL COOLORS";
			//		doImportPaletteCoolors(ofGetClipboardString());
			//	}

			//	ImGui::SameLine();

			//	if (ImGui::Button("EXPORT", ImVec2(_w50, _h / 2)))
			//	{
			//		ofLogNotice(__FUNCTION__) << "EXPORT URL COOLORS";
			//		doExportPaletteCoolors();
			//	}
			//}

			//--

//#ifndef USE_MINIMAL_GUI
//			if (SHOW_Advanced)
//			{
//				//ImGui::Separator();
//				ImGui::Dummy(ImVec2(0.0f, 2.0f));
//
//				// show floating palette
//				ofxImGuiSurfing::AddBigToggle(SHOW_Palette, _w99, _h / 2);
//			}
//#endif

			//--

			if (SHOW_Advanced)
			{
				ImGui::Dummy(ImVec2(0.0f, 2.0f));

				if (SHOW_Palette)
				{
					if (ImGui::CollapsingHeader("Layout"))
					{
						ofxImGuiSurfing::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);
						_h *= 2;

						ofxImGui::AddParameter(bFitLayout);
						if (!bFitLayout) {
							ofxImGui::AddParameter(bResponsive_Panels);

							if (bResponsive_Panels)
							{
								ImGui::InputInt(sizePaletteBox.getName().c_str(), (int*)&sizePaletteBox.get(), 1, 100);
							}
							if (!bResponsive_Panels)
							{
								boxMaxRows.disableEvents();
								ImGui::InputInt(boxMaxRows.getName().c_str(), (int*)&boxMaxRows.get(), 1, 5);
								boxMaxRows = ofClamp(boxMaxRows.get(), boxMaxRows.getMin(), boxMaxRows.getMax());
								boxMaxRows.enableEvents();

								ImGui::InputFloat(scale_ColPalette.getName().c_str(), (float *)&scale_ColPalette.get(), 0.005f, 0.010);
							}
						}
						ImGui::Checkbox("Auto-Resize", &auto_resize);
					}
				}

				//ofxImGui::AddParameter(bAutoResizePalette);//not works
			}

			//--

			// show picker
			ofxImGuiSurfing::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);
			_h *= 2;

			ImGui::Dummy(ImVec2(0.0f, 5.0f));
			ImGui::Separator();
			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			ofxImGuiSurfing::AddBigToggle(SHOW_Picker, _w100, _h / 2);
			ofxImGuiSurfing::AddBigToggle(SHOW_Library, _w100, _h / 2); }
	}

	//ofxImGui::EndWindow(mainSettings);
	guiManager.endWindow();

	//ImGui::PopStyleVar();
}

//--------------------------------------------------------------
void ofxColorManager::gui_Palette()
{
	static bool auto_resize = false;
	static bool lockMin = false;

	float ww, hh;
	ww = PANEL_WIDGETS_WIDTH;
	hh = PANEL_WIDGETS_HEIGHT;
	ImGui::SetWindowSize(ImVec2(ww, hh));

	ImGuiWindowFlags flagsw;
	flagsw = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None | ImGuiWindowFlags_NoScrollbar;
	flagsw |= flagsWindowsLocked;

	//-

	//if (lockMin)ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(500, PANEL_WIDGETS_HEIGHT));

	guiManager.beginWindow(SHOW_Palette, flagsw);
	//if (ofxImGui::BeginWindow("PALETTE", mainSettings, flagsw))
	{
		ImGuiColorEditFlags _flags = ImGuiColorEditFlags_None;

		//--

		float _spc;
		float _w;
		float _h;
		float _w50;
		float _w20;

		_spc = ImGui::GetStyle().ItemInnerSpacing.x;

		//if (auto_resize) _w = ww;
		////else _w = ImGui::GetWindowContentRegionWidth() - 3 * _spc;
		//else _w = ImGui::GetContentRegionAvail().x;

		_w = ImGui::GetContentRegionAvail().x;
		_h = ImGui::GetContentRegionAvail().y;

		_w50 = _w * 0.5;
		_w20 = _w * 0.2;

		bool bPortrait = false;
		if (_h > _w) bPortrait = true;

		//if (!bPortrait) _hb = BUTTON_BIG_HEIGHT;
		//else _hb = _h;

		//-

		// responsive
		ImVec2 button_sz((float)sizePaletteBox.get(), (float)sizePaletteBox.get());

		int _amtBtn = palette.size();
		float _wx2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

		//-

		// color buttons
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

		for (int n = 0; n < palette.size(); n++)
		{
			ImGui::PushID(n);

			// mode a. 
			// fit
			if (bFitLayout)
			{
				if (!bPortrait)
					if (n != 0)
					{
						ImGui::SameLine(0, 0);
					}
			}
			else
			{
				// responsive buttons size
				if (bResponsive_Panels)
				{
					const ImVec4 color2 = style->Colors[ImGuiCol_Button];//do not changes the color
					ImGui::PushStyleColor(ImGuiCol_Button, color2);
				}

				//--

				if (!bResponsive_Panels)
				{
					// mode b.
					// split in rows
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
			}

			//--

			int _r = (int)MIN(boxMaxRows.get(), palette.size());
			float wb;
			if (!bPortrait)
			{
				wb = (_w / _r);
				//wb = (_w / _r) - (1.5 * _spc);
			}
			else
			{
				//wb = _w;
			}
			if (!bResponsive_Panels) wb = wb * scale_ColPalette.get();

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

			ImVec2 bb;// size
			if (bFitLayout)
			{
				float _hhB;
				float _wwB;
				int _sizeP = palette.size();

				if (!bPortrait)
				{
					_hhB = _h;
					_wwB = _w / _sizeP;
				}
				else
				{
					_hhB = _h / _sizeP;
					_wwB = _w;
				}

				// shape
				bb = ImVec2(_wwB, _hhB);
			}
			else
			{
				if (bResponsive_Panels) bb = button_sz;
				else bb = ImVec2(wb, wb);
			}

			//-

			// border to selected
			bool bDrawBorder = false;

			if (n == last_Index_ColorPalette && bEditPalette)
			{
				bDrawBorder = true;
				ImVec4 borderLineColor2 = ImVec4(borderLineColor.x, borderLineColor.y, borderLineColor.z, borderLineColor.w - 0.15);
				ImGui::PushStyleColor(ImGuiCol_Border, borderLineColor2);
				ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, borderLineWidth + 2.0f);
			}

			//-

			if (ImGui::ColorButton("##paletteDragPalette", palette[n], _flags, bb))
			{
				last_Index_ColorPalette = n;

				// workflow
				if (!bEditPalette) bEditPalette = true;

				// workflow
				if (SHOW_Range) {
					if (bAuto_Color1_FromPicker_Range) color_1_Range = palette[n];
					else if (bAuto_Color2_FromPicker_Range) color_2_Range = palette[n];
					//refresh_Range_AutoUpdate();
					// autogenerate
					if (autoGenerate_Range) generate_Range(color_1_Range.get(), color_2_Range.get());
				}

				// workflow
				if (SHOW_Theory) {
					color_TheoryBase.set(palette[n]);
					if (bAuto_Theory_FromPicker) color_Picked = color_TheoryBase.get();
					refresh_FromPicked();
				}
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

						refresh_Background();
					}
					if (mode == Mode_Swap)
					{
						const ofColor tmp = palette[n];
						palette[n] = palette[payload_n];
						palette[payload_n] = tmp;

						refresh_Background();
					}

					//--

					// update all pallete (Curve)
					ofLogNotice(__FUNCTION__) << "DONE Dragged";

					gradientEngine.build_From_TARGET();

					// workflow
					bFlag_refresh_EnginesFromPalette = true;

					//--

					// export
					if (bAutoExportPreset)
					{
						bExportFlag = true;
					}

				}
				ImGui::EndDragDropTarget();
			}

			//----

			if (!bFitLayout)
			{
				// responsive buttons size
				if (bResponsive_Panels)
				{
					ImGui::PopStyleColor();
					float last_button_x2 = ImGui::GetItemRectMax().x;
					float next_button_x2 = last_button_x2 + style->ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
					if (n + 1 < _amtBtn && next_button_x2 < _wx2) ImGui::SameLine();
				}
			}

			ImGui::PopID();
		}

		ImGui::PopStyleVar();

		ImGui::Dummy(ImVec2(0.0f, 5.0f));
	}
	//ofxImGui::EndWindow(mainSettings);
	guiManager.endWindow();

	if (lockMin)ImGui::PopStyleVar();
}

//--------------------------------------------------------------
void ofxColorManager::gui_Library()
{
	static bool auto_resize = false;

	float ww, hh;
	ww = 175;
	hh = 350;
	ImGui::SetWindowSize(ImVec2(ww, hh));

	ImGuiWindowFlags flagsw;
	flagsw = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;
	flagsw |= flagsWindowsLocked;

	//--

	//blink when a new preset is editing
	float a = ofxSurfingHelpers::getFadeBlink();

	// color and line ofr selected widgets
	ImVec4 borderLineColor2 = ImVec4(0, 0, 0, a);

	//--

	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(ww, hh));

	guiManager.beginWindow(SHOW_Library, flagsw);
	//if (ofxImGui::BeginWindow("LIBRARY", mainSettings, flagsw))

	{
		bool bUpdate = false;

		//--

		float _spcx;
		float _spcy;
		float _w100;
		float _h100;
		float _w99;
		float _w50;
		float _w33;
		float _w25;
		float _h;
		ofxImGuiSurfing::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);
		_h *= 2;

		int _colsSize;
		if (lib_CardsMode) _colsSize = lib_RowSize;
		else _colsSize = lib_MaxColumns;

		//--

		// a. pantone cards: 7 colors / row
		//lib_Page_NumColors = lib_NumRows * lib_RowSize;

		// b. responsive
		lib_Page_NumColors = lib_NumRows * lib_MaxColumns.get();

		//lib_Page_Max = lib_TotalColors / (lib_Page_NumColors - 1);
		//lib_Page_Index.setMax(lib_Page_Max - 1);

		lib_Page_Max = lib_TotalColors / lib_Page_NumColors;
		lib_Page_Index.setMax(lib_Page_Max);

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
		ENABLE_CALLBACKS_Pickers = false;
		ImVec4 _c = color_Picked.get();
		ENABLE_CALLBACKS_Pickers = true;

		//--

		std::string s;

		// 1. lib name (ie: pantone)
		s = colorBrowser.getNameLib();
		ImGui::Text(s.c_str());

		ImGui::SameLine();

		// 3. index/total
		s = "    " + ofToString(last_Lib_Index) + " / " + ofToString(lib_TotalColors - 1);
		ImGui::Text(s.c_str());

		// 2. color name
		ImGui::Text(last_Lib_NameColor.c_str());

		//--

		// advanced

#ifdef USE_EXTRA_LIBRARIES
		if (ImGui::CollapsingHeader("Advanced"))
#endif
		{
			if (SHOW_Advanced)
			{
				if (ImGui::CollapsingHeader("Layout"))
				{
					ImGui::PushItemWidth(_w33);

					ofxImGui::AddParameter(lib_Responsive_ModeFit);
					if (!lib_Responsive_ModeFit)
					{
						ofxImGui::AddParameter(lib_Responsive_ModeGrid);
						ImGui::InputInt(sizeLibColBox.getName().c_str(), (int*)&sizeLibColBox.get(), 1, 5);
						ofxImGui::AddParameter(bPagerized);
					}

					//-

					// responsive

					if (lib_Responsive_ModeFit)
					{
						ImGui::InputInt(lib_NumRows.getName().c_str(), (int*)&lib_NumRows.get(), 1, 5);
						ImGui::InputInt(lib_MaxColumns.getName().c_str(), (int*)&lib_MaxColumns.get(), 1, 5);
					}
					else if (!lib_Responsive_ModeGrid)
					{
						ofxImGui::AddParameter(lib_CardsMode);
						ImGui::InputFloat(scale_LibCol.getName().c_str(), (float *)&scale_LibCol.get(), 0.02f, 0.1f);

						if (!lib_CardsMode)
						{
							ImGui::InputInt(lib_NumRows.getName().c_str(), (int*)&lib_NumRows.get(), 1, 5);
							ImGui::InputInt(lib_MaxColumns.getName().c_str(), (int*)&lib_MaxColumns.get(), 1, 5);

						}
					}
					else if (lib_Responsive_ModeGrid)
					{
						ofxImGui::AddParameter(lib_CardsMode);
						ImGui::InputFloat(scale_LibCol.getName().c_str(), (float *)&scale_LibCol.get(), 0.02f, 0.1f);
					}

					lib_MaxColumns = ofClamp(lib_MaxColumns, 1, 100);
					lib_NumRows = ofClamp(lib_NumRows, 1, 100);

					ImGui::PopItemWidth();

					if (ImGui::Button("Reset", ImVec2(_w100, WIDGETS_HEIGHT))) {
						resetLibraryLayout();
					}
				}
			}

			//--

			// 2.2 draw all palette colors grid

			//--

			// libraries

#ifdef USE_EXTRA_LIBRARIES

			if (ImGui::CollapsingHeader("Library"))
			{
				ofxImGui::AddParameter(colorBrowser.LibraryColors_Index);
				ofxImGui::AddParameter(colorBrowser.LibraryColors_Index_name);

				ImGui::Dummy(ImVec2(0.0f, 5.0f));

				ofxImGui::AddParameter(colorBrowser.MODE_SORTING);
				ofxImGui::AddParameter(colorBrowser.MODE_SORTING_name);

				ImGui::Dummy(ImVec2(0.0f, 5.0f));

				ofxImGui::AddParameter(colorBrowser.ENABLE_keys);
			}
#endif
		}

		//--

		// 1. arrow buttons

		if (bPagerized)
		{
			ImGui::PushButtonRepeat(true);

			// 1.1 prev
			if (ImGui::ArrowButton("##left", ImGuiDir_Left))
			{
				if (lib_Page_Index > 0)
				{
					lib_Page_Index--;
				}
			}

			ImGui::SameLine(0, _spcy);

			// 1.2 next
			if (ImGui::ArrowButton("##right", ImGuiDir_Right))
			{
				if (lib_Page_Index < lib_Page_Max)
				{
					lib_Page_Index++;
				}
			}

			ImGui::PopButtonRepeat();

			//-

			// 2. page slider

			ImGui::SameLine();
			ImGui::PushItemWidth(-40);
			ofxImGui::AddParameter(lib_Page_Index);//page slider selector
			ImGui::PopItemWidth();

			lib_Page_Index = ofClamp(lib_Page_Index.get(), 0, lib_Page_Index.getMax());
		}

		//-

		//TODO:
		// random buttons
		//float _h = BUTTON_BIG_HEIGHT;
		//ImVec2 bb{ _w50, _h / 2 };
		//if (ImGui::Button("Random1", bb))
		//{
		//	doRandomizeColorLibrary(false);
		//}
		//ImGui::SameLine();
		//if (ImGui::Button("Random2", bb))
		//{
		//	doRandomizeColorLibrary(true);
		//}

		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		//----

		// 2. grid colors

		// responsive
		ImVec2 _sz((float)sizeLibColBox.get(), (float)sizeLibColBox.get());

		int _countBtns;
		if (lib_Responsive_ModeGrid) _countBtns = lib_TotalColors;
		else _countBtns = lib_EndCol - lib_StartCol;
		float _wx2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

		//--

		// boxes sizes

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

		_w100 = ImGui::GetContentRegionAvail().x - _spcx;
		_h = ImGui::GetContentRegionAvail().y - _spcy;

		ImVec2 _bb;

		if (lib_Responsive_ModeFit)
		{
			float _wb = _w100 / lib_MaxColumns;
			float _wh = _h / lib_NumRows;
			_bb = ImVec2(_wb, _wh);
		}
		else if (lib_Responsive_ModeGrid)
		{
			_bb = _sz;
		}
		else
		{
			_bb = ImVec2(sizeLibColBox * scale_LibCol, sizeLibColBox * scale_LibCol);
		}

		//--

		for (int n = lib_StartCol; n < lib_EndCol && n < lib_TotalColors; n++)
		{
			//--

			// responsive buttons size

			if (lib_Responsive_ModeGrid && !lib_Responsive_ModeFit)
			{
				ImGui::PushID(n);

				const ImVec4 color2 = style->Colors[ImGuiCol_Button];//do not changes the color
				ImGui::PushStyleColor(ImGuiCol_Button, color2);
			}

			//--

			// color box
			{
				ImGui::PushID(n);

				//ImGui::PushStyleVar(ImGui::ImGuiStyleVar_FrameBorderSize), 1.0f;

				if (!lib_Responsive_ModeGrid)
				{
					if ((n % _colsSize) != 0)
					{
						ImGui::SameLine(0, 0);//vertical inter line
					}
				}

				// draw border to selected color
				bool bDrawBorder = false;
				if (n == last_ColorPicked_Lib)
				{
					bDrawBorder = true;
					ImGui::PushStyleColor(ImGuiCol_Border, borderLineColor2);
					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, borderLineWidth + 1.5);
				}

				//--

				// color button

				ImGuiColorEditFlags _flags =
					ImGuiColorEditFlags_NoAlpha |
					ImGuiColorEditFlags_NoPicker |
					ImGuiColorEditFlags_NoTooltip;

				ofFloatColor _c = ofColor(palette_Lib_Cols[n]);

				// if one clicked

				if (ImGui::ColorButton("##paletteLib", _c, _flags, _bb))

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
			if (lib_Responsive_ModeGrid && !lib_Responsive_ModeFit)
			{
				ImGui::PopStyleColor();

				float last_button_x2 = ImGui::GetItemRectMax().x;

				float next_button_x2 = last_button_x2 + _sz.x; // Expected position if next button was on same line
				//float next_button_x2 = last_button_x2 + style->ItemSpacing.x + _wSz.x; // Expected position if next button was on same line

				if (n + 1 < _countBtns && next_button_x2 < _wx2)
				{
					ImGui::SameLine(0, 0);//vertical inter line
					//ImGui::SameLine(0, ImGui::GetStyle().ItemSpacing.y);//vertical inter line
				}

				ImGui::PopID();
			}
		}

		ImGui::PopStyleVar();

		//----

		if (bUpdate)
		{
			refresh_FromPicked();
		}
	}

	guiManager.endWindow();
	//ofxImGui::EndWindow(mainSettings);

	//ImGui::PopStyleVar();
}

//--------------------------------------------------------------
void ofxColorManager::gui_LinkExport()
{
	static bool auto_resize = true;
	float ww, hh;
	ww = PANEL_WIDGETS_WIDTH;
	hh = PANEL_WIDGETS_HEIGHT;
	ImGui::SetWindowSize(ImVec2(ww, hh));

	ImGuiWindowFlags flagsw;
	flagsw = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;
	flagsw |= flagsWindowsLocked;

	//ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(ww, hh));

	//--

	guiManager.beginWindow(SHOW_LinkExport, flagsw);
	//if (ofxImGui::BeginWindow("LINK", mainSettings, flagsw))

	{
		float _spcx;
		float _spcy;
		float _w100;
		float _h100;
		float _w99;
		float _w50;
		float _w33;
		float _w25;
		float _h;
		ofxImGuiSurfing::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);
		_h *= 2;

		//--

		//if (ImGui::CollapsingHeader("Link"))
		{
			//ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());
			if (ImGui::Button("EXPORT PRESET", ImVec2(_w100, _h)))
			{
				exportPalette();
			}
			//ImGui::PopItemWidth();

#ifndef USE_SIMPLE_PRESET_PALETTE	
			ofxImGuiSurfing::AddBigToggle(bModeBundlePreset, _w100, _h / 2);
			ofxImGuiSurfing::AddBigToggle(bModePalettePreset, _w100, _h / 2);
#endif
			ofxImGuiSurfing::AddBigToggle(bAutoExportPreset, _w100, _h / 2);

			ImGui::Dummy(ImVec2(0.0f, 2.0f));
			//ImGui::Text("LINK:");

			//--

			// tcp and file
			// blink borders

			float a;
			bool b;

			b = bExportByTCP.get();
			if (b) a = ofxSurfingHelpers::getFadeBlink();
			else a = 1.0f;
			if (b) ImGui::PushStyleColor(ImGuiCol_Border, (ImVec4)ImColor::HSV(0.5f, 0.0f, 1.0f, a));
			ofxImGuiSurfing::AddBigToggle(bExportByTCP, _w100, _h, false); /*ImGui::SameLine();*/
			if (b) ImGui::PopStyleColor();

			b = bExportByFile.get();
			if (b) a = ofxSurfingHelpers::getFadeBlink();
			else a = 1.0f;
			if (b) ImGui::PushStyleColor(ImGuiCol_Border, (ImVec4)ImColor::HSV(0.5f, 0.0f, 1.0f, a));
			ofxImGuiSurfing::AddBigToggle(bExportByFile, _w100, _h, false);
			if (b) ImGui::PopStyleColor();

			//--

			//ofxImGuiSurfing::AddBigToggle(bExportByFile, _w50, _h / 2); ImGui::SameLine();
			//ofxImGuiSurfing::AddBigToggle(bExportByTCP, _w50, _h / 2);
			////ofxImGui::AddParameter(bAutoExportPreset);

			//-

			ImGui::Dummy(ImVec2(0.0f, 2.0f));

			// coolors.com import export
			if (ImGui::CollapsingHeader("COOLORS.CO"))
			{
				//ImGui::Text("Coolors.co");
				//https://coolors.co/f53c65-470229-05a66e

				if (ImGui::Button("PASTE / IMPORT", ImVec2(_w50, _h))) {

					doImportPaletteCoolors(ofGetClipboardString());
				}
				ImGui::SameLine();

				if (ImGui::Button("COPY / EXPORT", ImVec2(_w50, _h))) {

					doExportPaletteCoolors();
				}

				//TODO: tooltip
				std::string ss;
				ss += "Get palettes from Coolors.co website: \n";
				ss += "Export > Copy URL > PASTE/IMPORT \n";
				ss += "\nSend palettes to Coolors.co: \n";
				ss += "COPY/EXPORT \n";
				ss += "This will open the website in your browser. \n";
				ss += "\nNotice that you can Export and convert to: \n";
				ss += "ASE, CSS, SVG, PDF...etc \n";

				//ImGui::SameLine();
				ImGui::TextDisabled("(?)");
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
					//ImGui::Text(ss.c_str());
					ImGui::TextUnformatted(ss.c_str());
					ImGui::PopTextWrapPos();
					ImGui::EndTooltip();
				}
			}

			//-

			ImGui::Dummy(ImVec2(0.0f, 2.0f));

			if (ImGui::CollapsingHeader("EXTRA"))
			{
				if (bExportByFile)
					if (ImGui::CollapsingHeader("LINK FILE"))
					{
						ofxImGui::AddParameter(bExportPreset_DefaultPath);
						if (!bExportPreset_DefaultPath)
							if (ImGui::Button("Set Path", ImVec2(_w100, 0.5*_h)))
							{
								bOpen = true;
							}

						ImGui::Dummy(ImVec2(0.0f, 2.0f));

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

						ImGui::Dummy(ImVec2(0.0f, 2.0f));

						ImGui::Text("Name:");
						//ImGui::Text(PRESET_Temp.name.c_str());
						if (name_TARGET != nullptr) ImGui::Text(name_TARGET[0].c_str());

						ImGui::Dummy(ImVec2(0.0f, 2.0f));

						ImGui::Text("File:");
#ifdef USE_SIMPLE_PRESET_PALETTE
						//std::string _path = path_FileExport + "_Palette.json";
						std::string _path = path_Name_ExportColor + "_Palette.json";
						ImGui::Text(_path.c_str());//hardcoded
#endif
						ImGui::Dummy(ImVec2(0.0f, 2.0f));

						//ImGui::Text("Gradient:");
						//ImGui::Text(gradientEngine.path_Name_Gradient.c_str());
						////ImGui::Text(path_Name_Gradient.c_str());
					}

				if (bExportByTCP)
					if (ImGui::CollapsingHeader("LINK TCP"))
					{
						std::string s1 = "Host: " + host;
						std::string s2 = "Port: " + ofToString(port);
						ImGui::Text(s1.c_str());
						ImGui::Text(s2.c_str());
					}

				//ImGui::Dummy(ImVec2(0.0f, 5.0f));

				if (ImGui::Button("EXPORT KIT", ImVec2(_w100, _h)))
				{
					exportKit();
				}

				//-

				ImGui::Dummy(ImVec2(0.0f, 2.0f));

				ImGui::Checkbox("Auto-Resize", &auto_resize);
			}
		}
	}
	//ofxImGui::EndWindow(mainSettings);
	guiManager.endWindow();

	//ImGui::PopStyleVar();
}

//--------------------------------------------------------------
void ofxColorManager::gui_Picker()
{
	static bool auto_resize = true;
	static bool default_wheel = false;//or squared picker
	float ww, hh;
	ww = PANEL_WIDGETS_WIDTH;
	hh = PANEL_WIDGETS_HEIGHT;
	ImGui::SetWindowSize(ImVec2(ww, hh));

	ImGuiWindowFlags flagsw = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;
	flagsw |= flagsWindowsLocked;

	//--

	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(ww, hh));

	guiManager.beginWindow(SHOW_Picker, flagsw);
	//if (ofxImGui::BeginWindow("PICKER", mainSettings, flagsw))

	{
		float _spcx;
		float _spcy;
		float _w100;
		float _h100;
		float _w99;
		float _w50;
		float _w33;
		float _w25;
		float _h;
		ofxImGuiSurfing::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);
		_h *= 2;

		//--

		// get color from outside color picked

		ImVec4 cTmp = color_Picked.get();

		bool bChg_Pick = false;//to concentrate callbacks once
		bool bChg_HSB = false;//to concentrate callbacks once

		//--

		// 0. box color

		ImGuiColorEditFlags _flags = ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoTooltip;

		ImGui::ColorButton("##cPreview", *(ImVec4 *)&cTmp, _flags, ImVec2(_w100, _h));
		//ImGui::ColorButton("##cPreview", *(ImVec4 *)&cTmp, _flags, ImVec2(_w100, COLOR_STRIP_COLOR_HEIGHT));

		//-

		if (ImGui::CollapsingHeader("COLOR PICKERS", ImGuiWindowFlags_NoCollapse))
		{
			//TODO:
			//ImGui::SetItemDefaultFocus();
			//ImGui::SetWindowFocus();

			ImGuiColorEditFlags _flagw;
			ImGuiColorEditFlags _flags;

			float __spacing = 5;

			//--

			ImGui::Indent();

			// 1. color picker

			if (default_wheel) _flagw = ImGuiTreeNodeFlags_DefaultOpen;
			else _flagw = ImGuiTreeNodeFlags_None;
			_flagw |= ImGuiTreeNodeFlags_Framed;

			if (ImGui::TreeNodeEx("WHEEL", _flagw))
			{
				ofxImGuiSurfing::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);
				_h *= 2;

				// 1.1 circled
				_flags =
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

				ImGui::PushItemWidth(-__spacing);
				if (ImGui::ColorPicker4("##PickerWheel01", (float *)&cTmp, _flags))
				{
					ofLogNotice(__FUNCTION__) << "Wheel Picker : moved";

					bChg_Pick = true;
				}
				ImGui::PopItemWidth();

				ImGui::TreePop();
			}

			//--

			// 2. square

			if (!default_wheel) _flagw = ImGuiTreeNodeFlags_DefaultOpen;
			else _flagw = ImGuiTreeNodeFlags_None;
			_flagw |= ImGuiTreeNodeFlags_Framed;

			//flickering bug..
			if (ImGui::TreeNodeEx("SQUARE", _flagw))
			{
				ofxImGuiSurfing::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);
				_h *= 2;

				_flags =
					ImGuiColorEditFlags_NoSmallPreview |
					ImGuiColorEditFlags_NoTooltip |
					ImGuiColorEditFlags_NoLabel |
					ImGuiColorEditFlags_NoSidePreview |
					ImGuiColorEditFlags_HSV |
					ImGuiColorEditFlags_RGB |
					ImGuiColorEditFlags_HEX |
					ImGuiColorEditFlags_HDR |
					ImGuiColorEditFlags_NoAlpha |
					ImGuiColorEditFlags_PickerHueBar;

				ImGui::PushItemWidth(-__spacing);
				if (ImGui::ColorPicker4("##PickerSquare01", (float *)&cTmp, _flags))
				{
					ofLogNotice(__FUNCTION__) << "Square Picker : moved";

					bChg_Pick = true;
				}
				ImGui::PopItemWidth();

				ImGui::TreePop();
			}

			ImGui::Unindent();
		}

		//----

		// 2. HSB
		bool bOpen = false;
		ImGuiColorEditFlags _flagw = (bOpen ? ImGuiWindowFlags_NoCollapse : ImGuiWindowFlags_None);
		if (ImGui::CollapsingHeader("HSB", _flagw))
		{
			//TODO:
			//flickering bug..
			ImGui::PushID("##BUGFLICK");
			ImGui::PushItemWidth(-35);
			{
				//TODO:
				//if (ofxImGui::AddParameter(color_HUE)) {}
				//if (ofxImGui::AddParameter(color_SAT)) {}
				//if (ofxImGui::AddParameter(color_BRG)) {}

				//TODO:
				//flickering bug..
				//this seems crashing on debug mode
				int i = 0;
				ImGui::PushID(i++);//avoid repeat names and flicking bug
				if (ofxImGui::AddParameter(color_HUE)) {}
				ImGui::PopID();
				ImGui::PushID(i++);
				if (ofxImGui::AddParameter(color_SAT)) {}
				ImGui::PopID();
				ImGui::PushID(i++);
				if (ofxImGui::AddParameter(color_BRG)) {}
				ImGui::PopID();// ?? only once ?

				//invert
				ImGui::PushID(i++);
				if (ImGui::Button("INVERT", ImVec2(_w50, _h / 2)))
				{
					ofColor c = color_Picked.get();
					invert(c);
					color_Picked.set(c);
				}
				ImGui::PopID();
				ImGui::SameLine();
				ImGui::PushID(i++);
				if (ImGui::Button("COMPLEMENT", ImVec2(_w50, _h / 2)))
				{
					ofColor c = color_Picked.get();
					color_Picked.set(getComplement(c));
				}
				ImGui::PopID();
			}
			ImGui::PopItemWidth();
			ImGui::PopID();
		}

		//--

		if (ImGui::CollapsingHeader("RANDOMIZER"))
		{
			ofxImGuiSurfing::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);
			_h *= 2;

			if (bColor_rHUE || bColor_rSAT || bColor_rBRG) ofxImGuiSurfing::AddBigButton(bRandomColor, _w100, _h);

			if (bColor_rHUE) color_HUE_0 = color_HUE;
			if (bColor_rSAT) color_SAT_0 = color_SAT;
			if (bColor_rBRG) color_BRG_0 = color_BRG;

			//ImGui::PushItemWidth(-60);
			{ if (ofxImGui::AddParameter(bColor_rHUE)) {}ImGui::SameLine(); }
			{ if (ofxImGui::AddParameter(bColor_rSAT)) {}ImGui::SameLine(); }
			{ if (ofxImGui::AddParameter(bColor_rBRG)) {} }
			//ImGui::PopItemWidth();

			//ImGui::PushItemWidth(170);
			ImGui::PushItemWidth(-80);

			if (bColor_rHUE)
				if (ofxImGui::AddParameter(color_HUE_Power))
				{
					ofLogNotice(__FUNCTION__) << "HUE P : moved";
				}

			if (bColor_rSAT)
				if (ofxImGui::AddParameter(color_SAT_Power))
				{
					ofLogNotice(__FUNCTION__) << "SAT P : moved";
				}

			if (bColor_rBRG)
				if (ofxImGui::AddParameter(color_BRG_Power))
				{
					ofLogNotice(__FUNCTION__) << "BRG P : moved";
				}

			ImGui::PopItemWidth();
		}

		//----

		//to concentrate callbacks once

		if (bChg_Pick)
		{
			color_Picked = cTmp;

			//bFlag_refresh_EnginesFromPalette = true;
		}

		//----

		//if (ImGui::CollapsingHeader("Advanced"))
		//{
		//	ImGui::Checkbox("Auto-Resize", &auto_resize);
		//}

		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		ImGui::Separator();
		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		ofxImGuiSurfing::AddBigToggle(SHOW_Library, _w100, _h / 2);
	}

	//ofxImGui::EndWindow(mainSettings);
	guiManager.endWindow();

	ImGui::PopStyleVar();
}

//--------------------------------------------------------------
void ofxColorManager::gui_EnginesPanel()
{
	static bool auto_resize = true;
	//static bool auto_resize = false;

	ImGuiWindowFlags flagsw;
	flagsw = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;
	flagsw |= flagsWindowsLocked;

	//--

	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(400, BUTTON_BIG_HEIGHT + 20));

	guiManager.beginWindow(SHOW_Engines, flagsw);
	//if (ofxImGui::BeginWindow("ENGINES", mainSettings, flagsw))

	{
		const int NUM_WIDGETS = 4;

		float _spcx;
		float _spcy;
		float _w100;
		float _h100;
		float _w99;
		float _w50;
		float _w33;
		float _w25;
		float _h;
		ofxImGuiSurfing::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);
		_h *= 2;

		float _w = _w100 / NUM_WIDGETS - _spcx;

		ofxImGuiSurfing::AddBigToggle(SHOW_Theory, _w, _h); ImGui::SameLine();
		ofxImGuiSurfing::AddBigToggle(SHOW_Range, _w, _h); ImGui::SameLine();
		ofxImGuiSurfing::AddBigToggle(SHOW_ColourLovers, _w, _h); ImGui::SameLine();
		ofxImGuiSurfing::AddBigToggle(SHOW_Quantizer, _w, _h);
	}

	//ofxImGui::EndWindow(mainSettings);
	guiManager.endWindow();

	ImGui::PopStyleVar();
}

#ifndef USE_MINIMAL_GUI
//--------------------------------------------------------------
void ofxColorManager::gui_Advanced()
{
	static bool auto_resize = true;

	ImGuiWindowFlags flags;
	flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;
	flags |= flagsWindowsLocked;

	//----

	//ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(325, PANEL_WIDGETS_HEIGHT));

	guiManager.beginWindow(SHOW_Advanced, flags);
	//if (ofxImGui::BeginWindow("ADVANCED", mainSettings, flags))

	{
		float _spcx;
		float _spcy;
		float _w100;
		float _h100;
		float _w99;
		float _w50;
		float _w33;
		float _w25;
		float _h;
		ofxImGuiSurfing::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);
		_h *= 2;
		//--

		if (ImGui::CollapsingHeader("PANELS", ImGuiWindowFlags_None))
		{
			ofxImGui::AddParameter(SHOW_Advanced);
			ofxImGui::AddParameter(SHOW_Layouts);
			ofxImGui::AddParameter(bForceLayoutPosition);
			ofxImGui::AddParameter(SHOW_Panels);
			ofxImGui::AddParameter(SHOW_MINI_Preview);
			ofxImGui::AddParameter(SHOW_MenuBar);
			ImGui::Checkbox("SHOW ABOUT", &SHOW_About);
		}

		if (ImGui::CollapsingHeader("THEME", ImGuiWindowFlags_None))
		{
			ImGui::Dummy(ImVec2(0.0f, 2.0f));
			ImGui::Checkbox("Edit Theme", &SHOW_EditTheme);
			ofxImGui::AddParameter(Lock_DockingLayout);
#ifdef USE_DEBUG_LAYOUT 
			if (ImGui::Button("Mouse Ruler", ImVec2(_w100, _h / 2)))
			{
				mouseRuler.toggleVisibility();
			}
#endif
		}

		//ImGui::Dummy(ImVec2(0.0f, 2.0f));
		//ImGui::Separator();
		//ImGui::Text("PRESET NAME");
		if (ImGui::CollapsingHeader("PRESET NAME", ImGuiWindowFlags_None))
		{
			//ofxImGui::AddParameter(SHOW_Name);
			ofxImGui::AddParameter(bAutoHide_Name);

			ImGui::PushItemWidth(85);
			ofxImGui::AddStepper(fontSizeParam);
			ofxImGui::AddStepper(fontSizeBigParam);
			ImGui::PopItemWidth();
		}

		//ImGui::Dummy(ImVec2(0.0f, 2.0f));
		//ImGui::Separator();
		//ImGui::Text("DEBUG");
		if (ImGui::CollapsingHeader("DEBUG", ImGuiWindowFlags_None))
		{
			ofxImGui::AddParameter(ENABLE_keys);
			ofxImGui::AddParameter(bDebugRectCentral);

			bool bOpen = true;
			ImGuiColorEditFlags _flagw = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
			_flagw |= ImGuiTreeNodeFlags_Framed;

			if (ImGui::TreeNodeEx("LOCK KEYCOMMANDS", _flagw))
			{
				ImGui::Checkbox("Blocked Keys", &bBlockedKeys);
				ImGui::Checkbox("Locked All", &bLockAllKeysByGui);
				ImGui::Checkbox("Mouse Over Gui", &mouseLockedByGui);
				ImGui::Checkbox("Text Input Lovers", &bCheckMouseOverTextInputLovers);
				ImGui::Checkbox("Text Input", &bTextInputActive);
				//ImGui::Checkbox("Locked Mouse2", &bLockMouseByImGui);

				ImGui::TreePop();
			}
		}
	}
	//ofxImGui::EndWindow(mainSettings);
	guiManager.endWindow();

	//ImGui::PopStyleVar();
}
#endif

//--------------------------------------------------------------
void ofxColorManager::gui_LayoutsPresets()
{
	static bool auto_resize = false;
	ImGuiWindowFlags flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;
	flags |= ImGuiWindowFlags_NoSavedSettings;//exclude from restore layouts
	//flags |= flagsWindowsLocked;

	static float CURRENT_WINDOW_MIN_HEIGHT = 220;

	//blink save button to clarify workflow
	float a = ofxSurfingHelpers::getFadeBlink();

	//----

	// center
	//is excluded from .ini
	float xw, yw, ww, hw;
	ww = LAYOUT_WINDOW_WIDTH;
	hw = CURRENT_WINDOW_MIN_HEIGHT;
	//xw = ofGetWidth() / 2 - ww / 2;
	//yw = ofGetHeight() / 2 - hw / 2;
	ImGuiCond flag;
	if (bForceLayoutPosition) {
		////upper left
		//glm::vec2 p = rectangle_Central_MAX.getTopLeft() + glm::vec2(-1, -1);
		//center upper left
		int _pad = 10;
		int _xx = rectangle_Central_MAX.getTopLeft().x + _pad;
		int _yy = rectangle_Central_MAX.getTopLeft().y + rectangle_Central_MAX.getHeight() / 2 - hw / 2;
		glm::vec2 p = glm::vec2(_xx, _yy);

		xw = p.x;
		yw = p.y;
		flag = ImGuiCond_Always;
	}
	else {
		xw = positionGuiLayout.get().x;
		yw = positionGuiLayout.get().y;
		flag = ImGuiCond_Once;
	}

	ImGui::SetNextWindowPos(ofVec2f(xw, yw), flag);
	ImGui::SetNextWindowSize(ofVec2f(ww, hw), flag);

	//----

	//ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(ww, CURRENT_WINDOW_MIN_HEIGHT));

	guiManager.beginWindow(SHOW_Layouts, flags);
	//if (ofxImGui::BeginWindow("LAYOUT PRESETS", mainSettings, flags))

	{
		//get window position for advanced layout paired position
		auto posx = ImGui::GetWindowPos().x;
		auto posy = ImGui::GetWindowPos().y;
		auto w = ImGui::GetWindowWidth();
		positionGuiLayout = glm::vec2(posx, posy);
		//positionGuiLayout = glm::vec2(posx + w, posy);
		widthGuiLayout = w;

		//--

		const int NUM_WIDGETS = APP_LAYOUTS_AMOUNT + 4;

		float _spcx = ImGui::GetStyle().ItemSpacing.x;
		float _spcy = ImGui::GetStyle().ItemSpacing.y;
		float _w100 = ImGui::GetContentRegionAvail().x;
		float _h100 = ImGui::GetContentRegionAvail().y;
		float _w99 = _w100 - _spcx;
		float _h99 = _h100 - _spcy;
		float _w50 = _w99 / 2;

		//float _h99 = _h100 - _spcy;

		// buttons size
		float _w;
		float _h;

		// responsive boxed
		float maxbox = 470;
		bool bbox = false;
		if (_w100 < maxbox) bbox = true;

		if (bbox)// squared (three full height + 2 x 1/2 height rows)
		{
			_w = _w99 / 2;
			//float _units = 4.f;
			float _units = 4.5f;
			_h = _h99 / _units - _spcy;
		}
		else// panoramic (one row)
		{
			_w = _w100 / NUM_WIDGETS - _spcx;
			_h = _h99 + _spcy / 2;
		}

		// layouts
		ofxImGuiSurfing::AddBigToggle(b0, _w, _h); ImGui::SameLine();
		ofxImGuiSurfing::AddBigToggle(b1, _w, _h); if (!bbox) ImGui::SameLine();
		ofxImGuiSurfing::AddBigToggle(b2, _w, _h); ImGui::SameLine();
		ofxImGuiSurfing::AddBigToggle(b3, _w, _h); if (!bbox) ImGui::SameLine();
		ofxImGuiSurfing::AddBigToggle(b4, _w, _h); ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.0f, a));
		ImGui::PushID("##saveLayout");
		if (ImGui::Button("SAVE", ImVec2(_w, _h)))
		{
			saveAppLayout(AppLayouts(appLayoutIndex.get()));
		}
		ImGui::PopID();
		ImGui::PopStyleColor();

		if (!bbox)// a. one row
		{
			CURRENT_WINDOW_MIN_HEIGHT = 90;
			ImGui::SameLine();
		}
		else if (bbox)// b. boxes multi row
		{
			CURRENT_WINDOW_MIN_HEIGHT = 250;
			_w = _w100;
			_h = _h / 2;
		}

		//one row
		ofxImGuiSurfing::AddBigToggle(SHOW_Panels, _w, _h);
		if (!bbox) ImGui::SameLine();
		ofxImGuiSurfing::AddBigToggle(SHOW_LayoutsAdvanced, _w, _h);
		if (!bbox) ImGui::SameLine();
		ofxImGuiSurfing::AddBigToggle(Lock_DockingLayout, _w, _h);
	}
	//ofxImGui::EndWindow(mainSettings);
	guiManager.endWindow();

	//ImGui::PopStyleVar();
}

//--------------------------------------------------------------
void ofxColorManager::Changed_LayoutPanels(ofAbstractParameter &e)
{
	std::string name = e.getName();

	ofLogNotice(__FUNCTION__) << name << " : " << e;

	//----

	if (false) {}

	else if (name == b0.getName())
	{
		if (b0) {
			b1 = b2 = b3 = b4 = false;
			appLayoutIndex = 0;
		}
		else
		{
			if (!b0 && !b1 && !b2 && !b3 && !b4) b0 = true;
		}
	}

	else if (name == b1.getName())
	{
		if (b1) {
			b0 = b2 = b3 = b4 = false;
			appLayoutIndex = 1;
		}
		else
		{
			if (!b0 && !b1 && !b2 && !b3 && !b4) b1 = true;
		}
	}

	else if (name == b2.getName())
	{
		if (b2) {
			b0 = b1 = b3 = b4 = false;
			appLayoutIndex = 2;
		}
		else
		{
			if (!b0 && !b1 && !b2 && !b3 && !b4) b2 = true;
		}
	}

	else if (name == b3.getName())
	{
		if (b3) {
			b0 = b1 = b2 = b4 = false;
			appLayoutIndex = 3;
		}
		else
		{
			if (!b0 && !b1 && !b2 && !b3 && !b4) b3 = true;
		}
	}

	else if (name == b4.getName())
	{
		if (b4) {
			b0 = b1 = b2 = b3 = false;
			appLayoutIndex = 4;
		}
		else
		{
			if (!b0 && !b1 && !b2 && !b3 && !b4) b4 = true;
		}
	}
}

//--------------------------------------------------------------
void ofxColorManager::gui_LayoutsAdvanced()
{
	static bool auto_resize = true;

	ImGuiWindowFlags flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;
	flags |= ImGuiWindowFlags_NoSavedSettings; // exclude from restore layouts
	//flags |= flagsWindowsLocked;

	bool bMin = false; // hide load buttons to simplify
	float max = (bMin ? 150 : 175);

	//----

	static float CURRENT_WINDOW_MIN_HEIGHT = 100;

	// position is linked to main layout window
	// is excluded from .ini
	float xw, yw, ww, hw;
	ww = LAYOUT_WINDOW_WIDTH;
	hw = CURRENT_WINDOW_MIN_HEIGHT;
	xw = ofGetWidth() / 2 - ww / 2;
	yw = ofGetHeight() / 2 - hw / 2;

	ImGuiCond _flagc;
	//_flagc = ImGuiCond_Appearing;
	//ImGui::SetNextWindowPos(ofVec2f(xw + ww + 2, yw), _flagc);
	_flagc = ImGuiCond_Always;

	int _pad = 10;
	ImGui::SetNextWindowPos(ofVec2f(positionGuiLayout.get().x + widthGuiLayout + _pad, positionGuiLayout.get().y), _flagc);
	//ImGui::SetNextWindowSize(ofVec2f(ww, hw), _flagc);

	//----

	//ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(max, CURRENT_WINDOW_MIN_HEIGHT));

	guiManager.beginWindow(SHOW_LayoutsAdvanced, flags);
	//if (ofxImGui::BeginWindow("LAYOUTS", mainSettings, flags))

	{
		float _spcx;
		float _spcy;
		float _w100;
		float _h100;
		float _w99;
		float _w50;
		float _w33;
		float _w25;
		float _h;
		ofxImGuiSurfing::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);
		//_h = _h / 2;
		int i = 0;

		//----

		ofxImGuiSurfing::AddBigToggle(SHOW_Panels, _w100, _h);
		ofxImGuiSurfing::AddBigToggle(bAutoSave_Layout, _w100, _h);
		ofxImGuiSurfing::AddBigToggle(bForceLayoutPosition, _w100, _h);
		ofxImGuiSurfing::AddBigToggle(Lock_DockingLayout, _w100, _h);

		//----

		// label current layout
		//std::string str;
		//switch (appLayoutIndex)
		//{
		//case APP_DEFAULT: str = "DEFAULT"; break;
		//case APP_PRESETS: str = "PRESETS"; break;
		//case APP_ENGINES: str = "ENGINES"; break;
		//case APP_MINIMAL: str = "MINIMAL"; break;
		//case APP_USER: str = "USER"; break;
		//}
		//ImGui::Text(str.c_str());

		//ImGui::Dummy(ImVec2(0.0f, 2.0f));

		if (ImGui::CollapsingHeader("TOOLS", ImGuiWindowFlags_None))
		{
			ImVec2 bb{ (bMin ? _w100 : _w50), _h };

			ImGui::Text("Default");
			if (!bMin)
			{
				ImGui::PushID(i++);
				if (ImGui::Button("Load", bb))
				{
					appLayoutIndex = APP_DEFAULT;
				}
				ImGui::SameLine();
			}
			ImGui::PushID(i++);
			if (ImGui::Button("Save", bb))
			{
				ini_to_save_Str = path_ImGui + "imgui_DEFAULT.ini";
				ini_to_save = ini_to_save_Str.c_str();
			}

			ImGui::Text("Presets");
			if (!bMin)
			{
				ImGui::PushID(i++);
				if (ImGui::Button("Load", bb))
				{
					appLayoutIndex = APP_PRESETS;
				}
				ImGui::SameLine();
			}
			ImGui::PushID(i++);
			if (ImGui::Button("Save", bb))
			{
				ini_to_save_Str = path_ImGui + "imgui_PRESETS.ini";
				ini_to_save = ini_to_save_Str.c_str();
			}

			ImGui::Text("Engines");
			if (!bMin)
			{
				ImGui::PushID(i++);
				if (ImGui::Button("Load", bb))
				{
					appLayoutIndex = APP_ENGINES;
				}
				ImGui::SameLine();
			}
			ImGui::PushID(i++);
			if (ImGui::Button("Save", bb))
			{
				ini_to_save_Str = path_ImGui + "imgui_ENGINES.ini";
				ini_to_save = ini_to_save_Str.c_str();
			}

			ImGui::Text("Minimal");
			if (!bMin)
			{
				ImGui::PushID(i++);
				if (ImGui::Button("Load", bb))
				{
					appLayoutIndex = APP_MINIMAL;
				}
				ImGui::SameLine();
			}
			ImGui::PushID(i++);
			if (ImGui::Button("Save", bb))
			{
				ini_to_save_Str = path_ImGui + "imgui_MINIMAL.ini";
				ini_to_save = ini_to_save_Str.c_str();
			}

			ImGui::Text("User");
			if (!bMin)
			{
				ImGui::PushID(i++);
				if (ImGui::Button("Load", bb))
				{
					appLayoutIndex = APP_USER;
				}
				ImGui::SameLine();
			}
			ImGui::PushID(i++);
			if (ImGui::Button("Save", bb))
			{
				ini_to_save_Str = path_ImGui + "imgui_USER.ini";
				ini_to_save = ini_to_save_Str.c_str();
			}

			ImGui::PopID();

			ImGui::Dummy(ImVec2(0.0f, 2.0f));

			if (ImGui::Button("RESET", ImVec2(_w100, _h)))
			{
				//all params to false
				SHOW_Palette = false;
				SHOW_Panels = false;
				SHOW_PanelEngines = false;
				SHOW_Theory = false;
				SHOW_Range = false;
				SHOW_ColourLovers = false;
				SHOW_Quantizer = false;
				SHOW_Presets = false;
				SHOW_Kit = false;
				SHOW_Editor = false;
				SHOW_Picker = false;
				SHOW_Library = false;
				SHOW_Demos = false;
				gradientEngine.SHOW_CurveEditor = false;
				SHOW_MINI_Preview = false;
				SHOW_LinkExport = false;
				SHOW_Advanced = false;
				SHOW_MenuBar = false;
				Lock_DockingLayout = false;

				saveAppLayout(AppLayouts(appLayoutIndex.get()));
			}
		}

		//--


		if (ImGui::CollapsingHeader("EXTRA", ImGuiWindowFlags_None))
		{
			ofxImGuiSurfing::AddBigToggle(SHOW_Engines, _w100, _h);
			ofxImGuiSurfing::AddBigToggle(SHOW_MenuBar, _w100, _h);
			ofxImGuiSurfing::AddBigToggle(SHOW_Advanced, _w100, _h);
			//ofxImGuiSurfing::AddBigToggle(SHOW_LayoutsAdvanced, _w100, _h);

			ImGui::Checkbox("Auto-Resize", &auto_resize);
		}
	}

	//ofxImGui::EndWindow(mainSettings);
	guiManager.endWindow();

	//ImGui::PopStyleVar();
}

//--------------------------------------------------------------
void ofxColorManager::gui_Panels()
{
	static bool auto_resize = false;

	ImGuiWindowFlags flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;
	flags |= flagsWindowsLocked;

	//ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(625, PANEL_WIDGETS_HEIGHT));

	//----

	guiManager.beginWindow(SHOW_Panels, flags);
	//if (ofxImGui::BeginWindow("PANELS", mainSettings, flags))

	{
		int NUM_WIDGETS = 11;//expected num widgets

#ifndef USE_MINIMAL_GUI
		NUM_WIDGETS++;//extra advanced toggle
#endif
		float _spcx = ImGui::GetStyle().ItemSpacing.x;
		float _spcy = ImGui::GetStyle().ItemSpacing.y;
		float _h100 = ImGui::GetContentRegionAvail().y;
		float _w100 = ImGui::GetContentRegionAvail().x;
		float _w99 = _w100;// -_spcx;

		float _h = _h100 / 2 - _spcy;
		//float _h = BUTTON_BIG_HEIGHT;

		//layout
		float maxw = 850;
		//bool bMini = _w100 < maxw;
		bool bMini = true;

		float _w;
		if (!bMini)_w = _w99 / NUM_WIDGETS - _spcx;
		else _w = _w99 / 6 - _spcx;

		ofxImGuiSurfing::AddBigToggle(SHOW_Palette, _w, _h); ImGui::SameLine();
		ofxImGuiSurfing::AddBigToggle(SHOW_Presets, _w, _h); ImGui::SameLine();
		ofxImGuiSurfing::AddBigToggle(SHOW_Kit, _w, _h); ImGui::SameLine();
		ofxImGuiSurfing::AddBigToggle(SHOW_Editor, _w, _h); ImGui::SameLine();
		ofxImGuiSurfing::AddBigToggle(SHOW_Picker, _w, _h); ImGui::SameLine();

#ifdef MODE_BACKGROUND
		ofxImGuiSurfing::AddBigToggle(SHOW_BackGround, _w, _h);
		ImGui::SameLine();
#endif
		ofxImGuiSurfing::AddBigToggle(SHOW_Library, _w, _h);
		if (!bMini) ImGui::SameLine();//split if mini

		//ImGui::Separator();

		ofxImGuiSurfing::AddBigToggle(SHOW_PanelEngines, _w, _h); ImGui::SameLine();
		ofxImGuiSurfing::AddBigToggle(SHOW_Gradient, _w, _h); ImGui::SameLine();
		//ofxImGuiSurfing::AddBigToggle(gradientEngine.SHOW_CurveEditor, _w, _h); ImGui::SameLine();
		//ofxImGuiSurfing::AddBigToggle(SHOW_MINI_Preview, _w, _h);//ImGui::SameLine();

		//-

		// link blink
		float a;
		bool b;
#ifdef LINK_TCP_MASTER_CLIENT_KU
		b = TCP_Sender.connected() && TCP_Sender.enabled() && bExportByTCP;
#endif
#ifdef LINK_TCP_MASTER_CLIENT_OF
		b = TCP_Sender.isConnected() && bExportByTCP && (TCP_Sender.getNumClients() > 0);
#endif
		if (b) a = ofxSurfingHelpers::getFadeBlink();
		else a = 1.0f;
		if (b) ImGui::PushStyleColor(ImGuiCol_Border, (ImVec4)ImColor::HSV(0.5f, 0.0f, 1.0f, a));

		ofxImGuiSurfing::AddBigToggle(SHOW_LinkExport, _w, _h, false); ImGui::SameLine();

		if (b) ImGui::PopStyleColor();

		//-

		ofxImGuiSurfing::AddBigToggle(SHOW_Demos, _w, _h); ImGui::SameLine();
		ofxImGuiSurfing::AddBigToggle(SHOW_Layouts, _w, _h);

#ifndef USE_MINIMAL_GUI
		ImGui::SameLine();
		ofxImGuiSurfing::AddBigToggle(SHOW_Advanced, _w, _h);
#endif
	}
	//ofxImGui::EndWindow(mainSettings);
	guiManager.endWindow();

	//ImGui::PopStyleVar();
}

//--------------------------------------------------------------
void ofxColorManager::gui_Range()
{
	static bool auto_resize = false;

	float ww, hh;
	ww = PANEL_WIDGETS_WIDTH;
	hh = PANEL_WIDGETS_HEIGHT;
	ImGui::SetWindowSize(ImVec2(ww, hh));

	ImGuiWindowFlags flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;
	flags |= flagsWindowsLocked;

	//--

	//ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(PANEL_WIDGETS_WIDTH, 600));

	guiManager.beginWindow(SHOW_Range, flags);
	//if (ofxImGui::BeginWindow("RANGE", mainSettings, flags))

	{
		//float _h = float(COLOR_STRIP_COLOR_HEIGHT);
		float _hSz = int(BUTTON_COLOR_SIZE) * scale_ColRange.get();//color boxes
		float _hSz2;

		//TODO:
		float _spcx;
		float _spcy;
		float _w100;
		float _h100;
		float _w99;
		float _w50;
		float _w33;
		float _w25;
		float _h;
		ofxImGuiSurfing::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);

		float _w49 = _w50 - 2;

		float _szLabel = 70;//width label text
		float _wSz = (_w99 - _szLabel - 8) / numColors_Range.get();//color boxes

		//-

		bool bChanged = false;// some picker changed

		auto _c1 = color_1_Range.get();
		auto _c2 = color_2_Range.get();

		//-

		ImGui::Dummy(ImVec2(0.0f, 2.0f));

		// amount of colors

		//ImGui::PushItemWidth(-50);
		ImGui::PushItemWidth(110);
		if (ofxImGuiSurfing::AddIntStepped(numColors_Range)) {}
		ImGui::PopItemWidth();

		//-

		ImGuiColorEditFlags _flags;
		_flags =
			ImGuiColorEditFlags_NoSmallPreview |
			ImGuiColorEditFlags_NoLabel |
			ImGuiColorEditFlags_NoTooltip |
			ImGuiColorEditFlags_NoLabel |
			ImGuiColorEditFlags_NoSidePreview |
			ImGuiColorEditFlags_HSV |
			ImGuiColorEditFlags_RGB |
			ImGuiColorEditFlags_NoAlpha |
			ImGuiColorEditFlags_NoInputs |
			ImGuiColorEditFlags_NoDragDrop |
			ImGuiColorEditFlags_PickerHueBar;// square
			//ImGuiColorEditFlags_PickerHueWheel;// wheel

		//----

		// 1. colors
		{
			int i = 0;

			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			ImGui::Columns(2);
			ImGui::PushItemWidth(_w50);
			// 1.1.a toggles auto pick
			ImGui::PushID(i++);//avoid repeat names and flicking bug
			ImGui::Text("Color 1");
			// 1.2 mini box
			ImGui::PushID(i++);//avoid repeat names and flicking bug
			if (ImGui::ColorButton("##C1", _c1, _flags, ImVec2(_w50, _h))) {}
			ofxImGuiSurfing::AddBigToggle(bAuto_Color1_FromPicker_Range, _w50, BUTTON_BIG_HEIGHT / 2);
			ImGui::PopItemWidth();

			ImGui::NextColumn();
			ImGui::PushItemWidth(_w50);
			// 1.1.a toggles auto pick
			ImGui::PushID(i++);//avoid repeat names and flicking bug
			ImGui::Text("Color 2");
			// 1.2.a mini box
			ImGui::PushID(i++);//avoid repeat names and flicking bug
			if (ImGui::ColorButton("##C2", _c2, _flags, ImVec2(_w50, _h))) {}
			ImGui::PushID(i++);//avoid repeat names and flicking bug
			ofxImGuiSurfing::AddBigToggle(bAuto_Color2_FromPicker_Range, _w50, BUTTON_BIG_HEIGHT / 2);
			ImGui::PopItemWidth();
			ImGui::Columns(1);

			ImGui::PopID();

			//----

			if (ImGui::CollapsingHeader("PICKERS", ImGuiWindowFlags_NoCollapse))
			{
				int i = 0;

				// picker c1

				ImGui::Columns(2);
				//ImGui::PushItemWidth(-1);//too big sometimes
				//ImGui::PushItemWidth(-25);

				float _www;
				if (_w100 >= 2 * MAX_PICKER_WIDTH - 10)
				{
					_www = MAX_PICKER_WIDTH;
				}
				else if (_w100 < 2 * MAX_PICKER_WIDTH)
				{
					_www = _w49;
				}
				ImGui::PushItemWidth(_www);

				ImGui::PushID(i++);//avoid repeat names and flicking bug
				if (ImGui::ColorPicker3("##cPicker001", (float *)&_c1, _flags))
				{
					if (bAuto_Color1_FromPicker_Range) color_Picked.setWithoutEventNotifications(_c1);
					color_1_Range.set(_c1);
					bChanged = true;
				}

				ImGui::PopItemWidth();
				ImGui::NextColumn();

				//----

				// picker c2

				ImGui::PushItemWidth(_www);

				ImGui::PushID(i++);
				if (ImGui::ColorPicker3("##cPicker002", (float *)&_c2, _flags))
				{
					if (bAuto_Color2_FromPicker_Range) color_Picked.setWithoutEventNotifications(_c2);
					color_2_Range.set(_c2);
					bChanged = true;
				}
				ImGui::PopItemWidth();

				//--

				ImGui::PopID();

				ImGui::Columns(1);
			}

			//----

			// set picked colors

			if (bChanged)
			{
				refresh_Range_AutoUpdate();

				// autogenerate
				if (autoGenerate_Range) generate_Range(color_1_Range.get(), color_2_Range.get());
			}

			if (!autoGenerate_Range)
			{
				if (ImGui::Button("GENERATE", ImVec2(_w99, 0.5 * BUTTON_BIG_HEIGHT)))
				{
					generate_Range(color_1_Range.get(), color_2_Range.get());
				}
			}
			//ofxImGuiSurfing::AddSmallButton(bGetPalette_From_Range, 150, 30);

			//-

			// advanced

			if (SHOW_Advanced)
			{
				if (ImGui::CollapsingHeader("Advanced"))
				{
					ofxImGui::AddParameter(bAuto_Build_Palette);
					ofxImGui::AddParameter(autoGenerate_Range);

					if (ImGui::CollapsingHeader("Layout"))
					{
						// layout
						ImGui::Checkbox("Auto-Resize", &auto_resize);
						ImGui::InputFloat(scale_ColRange.getName().c_str(), (float *)&scale_ColRange.get(), 0.02f, 0.1f);
						//ofxImGui::AddParameter(scale_ColRange);
					}
				}
				ImGui::Dummy(ImVec2(0.0f, 5.0f));
			}
		}

		//----

		// 2. draw all palette colors grid

		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		//----

		int __sz = int(NUM_TYPES_RANGES);
#ifndef USE_MINIMAL_GUI
		if (SHOW_Advanced) __sz++;
#endif
		_hSz2 = ImGui::GetContentRegionAvail().y / __sz - ImGui::GetStyle().ItemSpacing.y;

		//----

		int ii = 0;

		for (int t = 0; t < int(NUM_TYPES_RANGES); t++)
		{
			// 2.1 each labels itself for each row (1st box)
			{
				// 2.1.1 label button

				//-

				// border to selected
				bool bDrawBorder = false;

				if (t == last_Index_Range && bLast_Index_Range)
				{
					bDrawBorder = true;

					ImGui::PushStyleColor(ImGuiCol_Border, borderLineColor);
					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, borderLineWidth);
					const ImVec4 colorActive = style->Colors[ImGuiCol_ButtonHovered];
					ImGui::PushStyleColor(ImGuiCol_Button, colorActive);
				}

				//-

				// 2.1.2 label name 

				//which range lab type selected

				//align
				//ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(labelPadding, 0.5f));
				if (ofxImGuiSurfing::AddSmallButton(types_Range[t], _szLabel, _hSz2))
				{
					//ofLogNotice(__FUNCTION__) << types_Range[t].getName() << " #" << t;// ? allways triggering..
				}
				//ImGui::PopStyleVar();

				//-

				if (bDrawBorder)
				{
					ImGui::PopStyleVar();
					ImGui::PopStyleColor(2);
				}

				ImGui::SameLine();

				//----

				// 2.2 colors boxes

				for (int c = 0; c < numColors_Range.get(); c++)
				{
					if (c == 0) ImGui::SameLine(0, ImGui::GetStyle().ItemSpacing.y);//vertical inter line
					else ImGui::SameLine(0, 0);//vertical inter line

					//int cc = t * numColors_Range.get() + c;
					int cc = ii;

					ofFloatColor _cc = palette_Range[cc];

					std::string ss;

					////ii = t + c * t;
					//ss = "##cRg" + ofToString(ii);
					//ImGui::PushID(cc);

					ss = ofToString(ii);
					ImGui::PushID(ii);
					ii++;

					if (cc < palette_Range.size())
					{
						if (ImGui::ColorButton(ss.c_str(), _cc, _flags, ImVec2(_wSz, _hSz2)))
						{
							ofLogNotice(__FUNCTION__) << "Range Box: " << t << "," << c << " " << ii;

							//--

							//using these pickers
							if (bAuto_Color1_FromPicker_Range || bAuto_Color2_FromPicker_Range)
							{
								color_Picked.set(_cc);
							}

							if (bAuto_Color1_FromPicker_Range) color_1_Range = _cc;
							else if (bAuto_Color2_FromPicker_Range) color_2_Range = _cc;

							// autogenerate
							if (autoGenerate_Range)
							{
								generate_Range(color_1_Range.get(), color_2_Range.get());
							}
						}
					}

					ImGui::PopID();
				}
			}
		}
	}
	//ofxImGui::EndWindow(mainSettings);
	guiManager.endWindow();

	//ImGui::PopStyleVar();
}

#ifdef MODE_BACKGROUND
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

	//ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(PANEL_WIDGETS_WIDTH, PANEL_WIDGETS_HEIGHT));

	guiManager.beginWindow(SHOW_BackGround, flagsw);
	//if (ofxImGui::BeginWindow("BACKGROUND", mainSettings, flagsw))

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
			//ImGui::Dummy(ImVec2(0.0f, 5.0f));

			//-

			// 0. color big

			float _w = ImGui::GetWindowContentRegionWidth();
			float _h = float(COLOR_STRIP_COLOR_HEIGHT);

			ImGuiColorEditFlags _flags;
			_flags = ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoTooltip;

			if (ImGui::ColorButton("##ColorBgPicker", *(ImVec4 *)&color, _flags, ImVec2(_w, _h)))
			{
				bChanged = true;
			}

			ImGui::Dummy(ImVec2(0.0f, 5.0f));

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

			ImGui::Dummy(ImVec2(0.0f, 5.0f));

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
						ofxImGuiSurfing::AddBigToggle(color_BackGround_DarkerMode);
						ofxImGuiSurfing::AddBigToggle(color_BackGround_GradientMode);

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

				//ImGui::Dummy(ImVec2(0.0f, 5.0f));
				//ImGui::Checkbox("Auto-Resize", &auto_resize);
			}

			//-

			//TODO: 
			//? should update only when changed
			if (bChanged) color_BackGround = color;
		}
	}
	//ofxImGui::EndWindow(mainSettings);
	guiManager.endWindow();

	//ImGui::PopStyleVar();
}
#endif
//--------------------------------------------------------------
void ofxColorManager::gui_Kit()
{
	static bool auto_resize = false;

	ImGuiWindowFlags flags;
	flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;
	flags |= flagsWindowsLocked;

	//blink when a new preset is editing
	float a = ofxSurfingHelpers::getFadeBlink();

	//----

	//add kit palettes browser
	if (SHOW_Kit)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(PANEL_WIDGETS_WIDTH, PANEL_WIDGETS_WIDTH));

		float x = ofGetWindowPositionX() + 10;
		float y = ofGetWindowPositionY() + 10;
		float w = PANEL_WIDGETS_WIDTH;
		//float w = ofGetWindowWidth();
		float h = ofGetWindowHeight() - 40;
		ImVec4 vViewport(x, y, w, h);
		ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_Once);

		guiManager.beginWindow(SHOW_Kit, flags);
		//if (ofxImGui::BeginWindow("KIT", mainSettings, flags))

		{
			// populate widgets
			bool bfocus = false;
			int indexPreset = gui_GridPalettes(kit, last_Index_Preset, AutoScroll.get(), bfocus);

			//--

			// workflow
			if (indexPreset != -1)
			{
				last_Index_Preset = indexPreset;
				preset_Load(files_Names[last_Index_Preset]);
			}
		}

		//ofxImGui::EndWindow(mainSettings);
		guiManager.endWindow();

		ImGui::PopStyleVar();

		//--

		//disable wheel scroll
		auto &io = ImGui::GetIO();
		if (io.MouseHoveredViewport)
		{
			float wheel = io.MouseWheel;
			if (wheel != 0)
			{
				if (AutoScroll) AutoScroll = false;
			}
		}
	}
}

#ifdef MODE_TEXT_INPUT_WORKAROUND
//--------------------------------------------------------------
void ofxColorManager::gui_TextInput()
{
	static bool auto_resize = false;

#ifdef INCLUDE_IMGUI_CUSTOM_THEME_AND_FONT
	ImGui::PushFont(customFontBig);
#endif

	ImVec2 _wt = ImGui::CalcTextSize(textInput_New.c_str());
	float _spc = ImGui::GetStyle().ItemSpacing.y + ImGui::GetStyle().ItemInnerSpacing.y + 50;

	// a. fit size to text width
	float _ww = _wt.x + 130;
	float _hh = fontSizeBigParam + _spc;
	ImGui::SetNextWindowSize(ImVec2(_ww, _hh));

	// force fit text input window to free space
	//glm::vec2 p = rectangle_Central_MAX.getTopLeft() + glm::vec2(0, -8);//upper left
	glm::vec2 p = rectangle_Central_MAX.getTopLeft() +
		glm::vec2(rectangle_Central_MAX.getWidth() / 2 - _ww / 2, -8);//centered
	ImGui::SetNextWindowPos(ImVec2(p.x, p.y), ImGuiCond_Always);

	//-

	// b. locked size
	//float _ww = 800;
	//float _hh = 75;

	//-

	ImGuiWindowFlags flagsw;
	flagsw = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;

	//flagsw |= flagsWindowsLocked;
	flagsw |= ImGuiWindowFlags_NoTitleBar;
	flagsw |= ImGuiWindowFlags_NoBackground;
	flagsw |= ImGuiWindowFlags_NoDecoration;

	//-

	// make it transparent
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0.1));
	//ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0.05));

	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(_ww, _hh));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0);

	// blink when a new preset is editing
	float a;
	if (MODE_NewPreset) a = ofxSurfingHelpers::getFadeBlink();
	else a = 1.0f;

	//--

	enum THEME_COLORS
	{
		THEME_DAY = 0,
		THEME_NIGHT
	};
	THEME_COLORS _THEME;

	// select one
	if (DEMO3_Svg.bEnable) _THEME = THEME_DAY;//this demo has white bg
	else _THEME = THEME_NIGHT;

	//--

	// label tittle

	int _alpha = 240;
	ofColor c0, c1;//shadow and letter colors
	if (_THEME == THEME_NIGHT)
	{
		c0.set(0, _alpha * a * 0.9);
		c1.set(255, _alpha * a);
	}
	else if (_THEME == THEME_DAY)
	{
		c0.set(164, _alpha * a);
		c1.set(0, _alpha * a * 0.9);
	}

	//--

	guiManager.beginWindow("PRESET NAME", flagsw);
	//if (ofxImGui::BeginWindow("PRESET NAME", mainSettings, flagsw))

	{
		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		float _hb = BUTTON_BIG_HEIGHT;
		float _spcx = ImGui::GetStyle().ItemSpacing.x;
		float _h = ImGui::GetContentRegionAvail().y;
		float _w100 = ImGui::GetContentRegionAvail().x;
		float _w50 = _w100 / 2;

		{
			//--

			// b. input text
			{
				// load string into char array
				char tab[32] = "type name";
				// this breakes the mouse cursor inside text input..
				// copy textInput_New to tab chars
				strncpy(tab, textInput_New.c_str(), sizeof(tab));
				tab[sizeof(tab) - 1] = 0;

				ImGui::PushStyleColor(ImGuiCol_Text, c1);
				ImGui::PushItemWidth(-1);

				//if (ImGui::InputText("", tab, IM_ARRAYSIZE(tab), ImGuiInputTextFlags_CharsUppercase))
				if (ImGui::InputText("", tab, IM_ARRAYSIZE(tab)))
				{
					textInput_New = ofToString(tab);
					if (name_TARGET != nullptr) name_TARGET[0] = &textInput_New[0];
					ofLogVerbose(__FUNCTION__) << "text:" << textInput_New;
				}

				ImGui::PopItemWidth();
				ImGui::PopStyleColor();

				//--

				//to disable all other key commands
				bool b = bTextInputActive;
				bTextInputActive = ImGui::IsItemActive();
				//changed
				if (bTextInputActive != b) ofLogNotice(__FUNCTION__) << "TextInput : " << (bTextInputActive ? "ACTIVE" : "DISABLED");

			}

			//--

			//	////TODO: ??
			//	//has_focus = 0;
			//	//if (focus_1) ImGui::SetKeyboardFocusHere();
			//	//ImGui::InputText("", tab, IM_ARRAYSIZE(tab));
			//	//if (ImGui::IsItemActive())
			//	//{
			//	//	has_focus = 1;
			//	//	textInput_New = ofToString(tab);
			//	//	ofLogNotice(__FUNCTION__) << "textInput_New:" << textInput_New;
			//	//}

			//--

#define HIDE_SAVE_BUTTON
#ifndef HIDE_SAVE_BUTTON
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.0f, a));

			_h = ImGui::GetContentRegionAvail().y;

			if (ImGui::Button("SAVE NEW", ImVec2(_w100, _h)))
			{
				//NOTE: preset filename must to not include any extra '.' char
				//clean all extra '.' chars
				ofStringReplace(textInput_New, ".", "");

				//has_focus = 0;
				MODE_NewPreset = false;
				if (textInput_New != "")
				{
					ofLogNotice(__FUNCTION__) << "textInput_New: " << textInput_New;

					preset_Save(textInput_New);
					preset_RefreshFiles();

					//--

					refresh_FilesSorting(textInput_New);
				}
				else ofLogError(__FUNCTION__) << "Empty name on textInput !";
			}
			ImGui::PopStyleColor();
#endif
		}

		ImGui::Dummy(ImVec2(0.0f, 5.0f));
	}

	//ofxImGui::EndWindow(mainSettings);
	guiManager.endWindow();

#ifdef INCLUDE_IMGUI_CUSTOM_THEME_AND_FONT
	ImGui::PopFont();
#endif

	ImGui::PopStyleVar();
	ImGui::PopStyleVar();

	ImGui::PopStyleColor();
}
#endif

//--------------------------------------------------------------
void ofxColorManager::gui_Presets()
{
	static bool auto_resize = true;

	float ww, hh;
	ww = PANEL_WIDGETS_WIDTH + 50;
	hh = PANEL_WIDGETS_HEIGHT;

	ImGuiWindowFlags flags;
	flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;
	flags |= flagsWindowsLocked;

	//blink when a new preset is editing
	float a = ofxSurfingHelpers::getFadeBlink();

	if (!auto_resize) ImGui::SetWindowSize(ImVec2(ww, hh));//not doing nothing?

	//--

	//ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(PANEL_WIDGETS_WIDTH, PANEL_WIDGETS_HEIGHT));

	//--

	////TODO:
	////snap engine
	//auto snap = [=](float value, float snap_threshold) -> float {
	//	float modulo = std::fmodf(value, snap_threshold);
	//	float moduloRatio = std::fabsf(modulo) / snap_threshold;
	//	if (moduloRatio < 0.5f)
	//		value -= modulo;
	//	else if (moduloRatio > (1.f - 0.5f))
	//		value = value - modulo + snap_threshold * ((value < 0.f) ? -1.f : 1.f);
	//	return value;
	//};
	////ImGui::Begin(name.data());
	////if (ImGui::IsItemActive()) 
	//{
	//	auto p = ImGui::GetWindowPos();
	//	auto size = ImGui::GetWindowSize();

	//	float x = snap(p.x, 16.f);
	//	float y = snap(p.y, 16.f);
	//	float sizex = snap(size.x, 16.f);
	//	float sizey = snap(size.y, 16.f);
	//	ImGui::SetWindowPos(ImFloor(ImVec2(x, y)));
	//}

	//--

	guiManager.beginWindow(SHOW_Presets, flags);
	//if (ofxImGui::BeginWindow("PRESETS", mainSettings, flags))

	{
		float _spcx;
		float _spcy;
		float _w100;
		float _h100;
		float _w99;
		float _w50;
		float _w33;
		float _w25;
		float _h;

		ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);
		_h *= 2;

		//-

		// KITS
		{
			bool bOpen = false;
			ImGuiTreeNodeFlags _flagt = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
			_flagt |= ImGuiTreeNodeFlags_Framed;

			if (ImGui::TreeNodeEx(path_Kit.c_str(), _flagt))
			{
				ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);
				_h *= 2;

				// scrollable list

				if (!files_KitNames.empty())
				{
					int _i = last_Index_Kit;

					string n = "##kits";
					//ImGui::PushItemWidth(_w50);
					ImGui::PushItemWidth(_w100 - 20);
					//ImGui::PushItemWidth(-10);
					ImGui::PushID(n.c_str());

					if (ofxImGui::VectorCombo(" ", &_i, files_KitNames))
					{
						ofLogNotice(__FUNCTION__) << "_i: " << ofToString(_i);

						if (_i < files_KitNames.size())
						{
							last_Index_Kit = _i;

							//path_Kit = files_KitNames[_i];
							////path_Kit = "USER KIT"; // default

							//preset_RefreshFiles(true);
						}
					}

					ImGui::PopID();
					ImGui::PopItemWidth();
				}

				if (ImGui::Button("REFRESH KITS", ImVec2(_w100, _h / 2)))
				{
					refresh_Kits();
				}

				ImGui::TreePop();
			}
		}

		//-

		//ImGui::Dummy(ImVec2(0.0f, 5.0f));

		//----

		// 1. Palette presets. scrollable list

		if (!files_Names.empty())
		{
			ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);
			_h *= 2;

			int _i = last_Index_Preset;

			string n = "##presets";
			//ImGui::PushItemWidth(-10);
			//ImGui::PushItemWidth(100);
			ImGui::PushItemWidth(_w100 - 20);
			ImGui::PushID(n.c_str());

			if (ofxImGui::VectorCombo(" ", &_i, files_Names))
			{
				ofLogNotice(__FUNCTION__) << "_i: " << ofToString(_i);

				if (_i < files_Names.size())
				{
					last_Index_Preset = _i;

					PRESET_Name = files_Names[last_Index_Preset];
					preset_Load(PRESET_Name);
				}

				// workflow
				if (MODE_NewPreset) MODE_NewPreset = false;
				MODE_ReadyToUpdate = true;
			}

			ImGui::PopID();
			ImGui::PopItemWidth();
		}

		ImGui::Dummy(ImVec2(0.0f, 2.0f));

		ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);
		_h *= 2;

		//----

		int counter = last_Index_Preset.get();

		// 2. index preset / total

		int _sz = files_Names.size() - 1;

		ImGui::Text("%d/%d", counter + 1, _sz + 1);//start on 1 not 0
		//ImGui::Text("%d/%d", counter, _sz);

		//----

		ImGui::Dummy(ImVec2(0.0f, 2.0f));

		ImGui::PushButtonRepeat(true);

		if (ImGui::Button("<", ImVec2(_w50, _h)))
		{
			preset_Previous();
		}

		//----

		ImGui::SameLine();

		if (ImGui::Button(">", ImVec2(_w50, _h)))
		{
			preset_Next();
		}

		ImGui::PopButtonRepeat();

		ImGui::Dummy(ImVec2(0.0f, 2.0f));

		//----

		// 3. new preset 

		// manual toggle

		if (ofxImGuiSurfing::AddBigToggle(MODE_NewPreset, _w100, _h))
		{
			if (name_TARGET != nullptr) textInput_New = name_TARGET[0];
			if (textInput_New == "") textInput_New = "type name";
		}

		if (MODE_NewPreset.get())
		{
#ifndef MODE_TEXT_INPUT_WORKAROUND
			//TODO:
			// load string into char array
			char tab[32] = "type text";

			// this breakes the mouse cursor inside text input..
			strncpy(tab, textInput_New.c_str(), sizeof(tab));
			tab[sizeof(tab) - 1] = 0;

			//ImGui::PushItemWidth(_w50);
			ImGui::PushItemWidth(-1);

			if (ImGui::InputText("", tab, IM_ARRAYSIZE(tab)))
			{
				textInput_New = ofToString(tab);
				if (name_TARGET != nullptr) name_TARGET[0] = &textInput_New[0];
				ofLogNotice(__FUNCTION__) << "textInput_New:" << textInput_New;
			}

			ImGui::PopItemWidth();

			//--

			//to disable all other key commands
			bool b = bTextInputActive;
			bTextInputActive = ImGui::IsItemActive();
			//changed
			if (bTextInputActive != b) ofLogNotice(__FUNCTION__) << "TextInput : " << (bTextInputActive ? "ACTIVE" : "DISABLED");

			//--

			//	////TODO: ??
			//	//has_focus = 0;
			//	//if (focus_1) ImGui::SetKeyboardFocusHere();
			//	//ImGui::InputText("", tab, IM_ARRAYSIZE(tab));
			//	//if (ImGui::IsItemActive())
			//	//{
			//	//	has_focus = 1;
			//	//	textInput_New = ofToString(tab);
			//	//	ofLogNotice(__FUNCTION__) << "textInput_New:" << textInput_New;
			//	//}

#endif
			//--

			// 4. save new

			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.0f, a));
			if (ImGui::Button("SAVE NEW", ImVec2(_w100, _h * 2)))
			{
				ofLogNotice(__FUNCTION__) << "SAVE NEW: " << textInput_New;

				//NOTE: preset filename must to not include any extra '.' char
				//clean all extra '.' chars
				ofStringReplace(textInput_New, ".", "");

				//has_focus = 0;
				MODE_NewPreset = false;
				MODE_ReadyToUpdate = true;

				if (textInput_New != "")
				{
					ofLogNotice(__FUNCTION__) << "textInput_New: " << textInput_New;

					//--

					//TODO:
					//not working

					// check overwrite

					string _pathfile = path_Presets + textInput_New + ".json";
					bAlertFileOverWrite = ofxSurfingHelpers::ofxKuFileExists(_pathfile);
					ofLogNotice(__FUNCTION__) << "File " << _pathfile << " " << (bAlertFileOverWrite ? "FOUND" : "NOT FOUND");
					bool bSkipSave = false;

					if (bAlertFileOverWrite) ImGui::OpenPopup("OVERWRITE");

					if (ImGui::BeginPopupModal("OVERWRITE", NULL, ImGuiWindowFlags_AlwaysAutoResize))
					{
						ImGui::Text("Current Preset will be overwritten.\nThis operation cannot be undone!\n\n");
						ImGui::Separator();

						if (ImGui::Button("OK", ImVec2(_w50, 0)))
						{
							ofLogNotice(__FUNCTION__) << "OVERWRITE";
							bSkipSave = false;
							bAlertFileOverWrite = false;

							ImGui::CloseCurrentPopup();
						}

						ImGui::SetItemDefaultFocus();
						ImGui::SameLine();

						if (ImGui::Button("CANCEL", ImVec2(_w50, 0)))
						{
							ofLogNotice(__FUNCTION__) << "CANCEL";
							bSkipSave = true;
							bAlertFileOverWrite = false;

							ImGui::CloseCurrentPopup();
						}

						ImGui::EndPopup();
					}

					//--

					if (!bSkipSave)
					{
						preset_Save(textInput_New);
						preset_RefreshFiles(false);
					}

					//--

					// workflow
					//go to ne index after adding a new preset
					refresh_FilesSorting(textInput_New);
				}
				else ofLogError(__FUNCTION__) << "Empty name on textInput !";
			}
			ImGui::PopStyleColor();
	}
		else
		{
			if (MODE_ReadyToUpdate)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.0f, a));

				if (ImGui::Button("SAVE", ImVec2(_w100, _h)))
				{
					//TODO:
					//should re load by same name and get what position on vector
					//is to reload current preset number
					//textInput_temp = ofToString(tab2);
					//ofLogNotice(__FUNCTION__) << "textInput_temp:" << textInput_temp;

					//PRESET_Name = textInput_temp;
					textInput_New = PRESET_Name;

					ofLogNotice(__FUNCTION__) << "SAVE : " << PRESET_Name;

					//save/update
					preset_Save(PRESET_Name);
					preset_RefreshFiles(false);
				}
				ImGui::PopStyleColor();
			}
		}

		//----

		// 2. presets

		ImGui::Dummy(ImVec2(0.0f, 2.0f));

		if (ImGui::CollapsingHeader("EXTRA"))
		{
			ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);
			_h *= 2;

			/*
			if (ImGui::Button("UPDATE", ImVec2(_w50, _h / 2)))
			{
				//TODO:
				//should re load by same name and get what position on vector
				//is to reload current preset number
				//textInput_temp = ofToString(tab2);
				//ofLogNotice(__FUNCTION__) << "textInput_temp:" << textInput_temp;

				//PRESET_Name = textInput_temp;
				textInput_New = PRESET_Name;

				ofLogNotice(__FUNCTION__) << "UPDATE : " << PRESET_Name;

				//save/update
				preset_Save(PRESET_Name);
				preset_RefreshFiles(false);
			}

			//ImGui::SameLine();
			*/

			//----

			if (ImGui::Button("RELOAD", ImVec2(_w100, _h / 2)))//not required..
			{
				ofLogNotice(__FUNCTION__) << "RELOAD : " << PRESET_Name;

				preset_Load(PRESET_Name);
			}

			//ImGui::SameLine();

			if (ImGui::Button("DELETE", ImVec2(_w50, _h / 2))) ImGui::OpenPopup("DELETE?");

			if (ImGui::BeginPopupModal("DELETE?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("Current Preset will be deleted.\nThis operation cannot be undone!\n\n");
				ImGui::Separator();

				static bool dont_ask_me_next_time = false;
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
				ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
				ImGui::PopStyleVar();

				if (!dont_ask_me_next_time) {
					if (ImGui::Button("OK", ImVec2(_w50, 0)))
					{
						ofLogNotice(__FUNCTION__) << "DELETE";
						files[last_Index_Preset].remove();
						preset_RefreshFiles(false);

						ImGui::CloseCurrentPopup();
					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();

					if (ImGui::Button("CANCEL", ImVec2(_w50, 0))) { ImGui::CloseCurrentPopup(); }
				}
				else {
					ofLogNotice(__FUNCTION__) << "DELETE";
					files[last_Index_Preset].remove();
					preset_RefreshFiles();

					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

			//----

			ImGui::SameLine();

			if (ImGui::Button("CLONE", ImVec2(_w50, _h / 2)))
			{
				ofLogNotice(__FUNCTION__) << "CLONE";

				textInput_New = PRESET_Name + "_";

				preset_Save(textInput_New);
				preset_RefreshFiles(false);

				//--

				refresh_FilesSorting(textInput_New);
			}

			//export user palette colors to live reload from another parallel app!

			if (ImGui::Button("EXPORT", ImVec2(_w50, _h / 2))) ImGui::OpenPopup("EXPORT ");
			if (ImGui::BeginPopupModal("EXPORT ", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				//ImGui::Text("Pick export path..\n\n");
				//ImGui::Separator();

				static bool dont_ask_me_next_time = false;
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
				ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
				ImGui::PopStyleVar();

				if (!dont_ask_me_next_time)
				{
					if (ImGui::Button("OK", ImVec2(_w50, 0)))
					{
						ofLogNotice(__FUNCTION__) << "EXPORT";
						//path_Folder_ExportColor_Custom = ;

						exportPalette();

						ImGui::CloseCurrentPopup();
					}

					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(_w50, 0))) { ImGui::CloseCurrentPopup(); }

					if (ImGui::Button("SET PATH", ImVec2(_w100, 0)))
					{
						ofLogNotice(__FUNCTION__) << "SET EXPORT PATH";
						ImGui::Text("Pick export path..\n\n");

						//TODO:
						//path_Folder_ExportColor_Custom = ;
						bOpen = true;

						ImGui::CloseCurrentPopup();
					}
				}
				//else
				//{
				//	//exportPalette();
				//}

				ImGui::EndPopup();
			}

			ImGui::SameLine();

			if (ImGui::Button("REFRESH KIT", ImVec2(_w50, _h / 2)))
			{
				ofLogNotice(__FUNCTION__) << "REFRESH KIT";

				preset_RefreshFiles();
			}

			//--

			// workflow

			ImGui::Dummy(ImVec2(0.0f, 2.0f));

			// palette colors mini preview
			// auto browse presets. to testing and auto export
			//ImGui::PushItemWidth(_w99);
			//if (ofxImGui::AddParameter(bPlaySlideShow)) {}
			guiManager.Add(bPlaySlideShow, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
			//ImGui::PopItemWidth();

			//ImGui::PushItemWidth(-35);
			if (bPlaySlideShow) guiManager.Add(auto_pilot_Duration, OFX_IM_DEFAULT);
			//if (bPlaySlideShow) ofxImGui::AddParameter(auto_pilot_Duration);
			guiManager.Add(bAutoHide_Name, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
			//ofxImGui::AddParameter(bAutoHide_Name);
			//ImGui::PopItemWidth();
		}

		//----

		ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);
		_h *= 2;

		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		ImGui::Separator();
		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		guiManager.Add(SHOW_Editor, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
		guiManager.Add(SHOW_Kit, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
		//ofxImGuiSurfing::AddBigToggle(SHOW_Editor, _w100, _h / 2);
		//ofxImGuiSurfing::AddBigToggle(SHOW_Kit, _w100, _h / 2);

		//ImGui::Dummy(ImVec2(0.0f, 2.0f));
		//if (SHOW_Kit) ofxImGui::AddParameter(AutoScroll);
		//ImGui::Checkbox("Auto-Resize", &auto_resize);
}

	guiManager.endWindow();
	//ofxImGui::EndWindow(mainSettings);

	//ImGui::PopStyleVar();
}

//--------------------------------------------------------------
void ofxColorManager::gui_Gradient()
{
	static bool auto_resize = true;

	ImGuiWindowFlags flagsw;
	flagsw = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;
	flagsw |= flagsWindowsLocked;

	//--

	//ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(PANEL_WIDGETS_WIDTH, PANEL_WIDGETS_HEIGHT));

	guiManager.beginWindow(SHOW_Gradient, flagsw);
	//if (ofxImGui::BeginWindow("GRADIENT", mainSettings, flagsw))

	{
		float _spcx;
		float _spcy;
		float _w100;
		float _h100;
		float _w99;
		float _w50;
		float _w33;
		float _w25;
		float _h;
		ofxImGuiSurfing::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);
		_h *= 2;

		//-

		//TODO:
		//should make all this internal in the addon
		//to avoid use gradientEngine. with public variables
		//add void drawGui()

		//if (ImGui::CollapsingHeader("EDIT", ImGuiWindowFlags_NoCollapse))
		{
			//ImGui::Dummy(ImVec2(0.0f, 5.0f));;

			//ofxImGuiSurfing::AddBigToggle(SHOW_Gradient, _w100, _h / 2);
			guiManager.Add(gradientEngine.SHOW_CurveEditor, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
			//ofxImGuiSurfing::AddBigToggle(gradientEngine.SHOW_CurveEditor, _w100, _h);
			//ofxImGuiSurfing::AddBigToggle(gradientEngine.SHOW_Gradient_Mini, _w100, _h);

			//TODO:
			// get and draw the widgets to draw inside ImGui
			//if (bDEMO3) colorClient.drawGradientToolWidgets();
			//gradientEngine.draw_GradientTool_ImGui_Widgets();

			//---

			if (ImGui::Button(gradientEngine.bResetGradient.getName().c_str(), ImVec2(_w100, _h)))
			{
				gradientEngine.bResetGradient = true;
			}

			//ImGui::Dummy(ImVec2(0.0f, 5.0f));

			ImGui::PushItemWidth(-40);
			ofxImGui::AddParameter(gradientEngine.pickIn);
			ofxImGui::AddParameter(gradientEngine.expTweak);
			ImGui::PopItemWidth();

			ImGui::Dummy(ImVec2(0.0f, 2.0f));

			if (gradientEngine.color_BackGround_GradientMode)
			{
				ofxImGui::AddParameter(color_BackGround);
				ofxImGui::AddParameter(gradientEngine.bAutoSet_PickColor);
			}
			ofxImGuiSurfing::AddBigToggle(gradientEngine.color_BackGround_GradientMode, _w100, _h / 2);
			if (ImGui::Button(gradientEngine.bPalettize.getName().c_str(), ImVec2(_w100, _h / 2)))
			{
				gradientEngine.bPalettize = true;
			}
			//ofxImGui::AddParameter(gradientEngine.bAutoPaletteFromGradient);
		}

		//--

		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		ImGuiWindowFlags _flagw;
		_flagw |= ImGuiTreeNodeFlags_None;

		if (ImGui::CollapsingHeader("EXTRA", _flagw))
		{
			ofxImGui::AddParameter(gradientEngine.gradient_HardMode);

			ImGui::PushItemWidth(-40);
			ofxImGui::AddParameter(gradientEngine.expTweak);
			ImGui::PopItemWidth();

			//-

			ImGui::Dummy(ImVec2(0.0f, 5.0f));
			if (ofxImGuiSurfing::AddBigToggle(gradientEngine.bEditLayout, _w100, _h / 2))
			{
			}

			//--

			// curve Test

			if (ImGui::CollapsingHeader("Curve TEST"))
			{
				ImGui::PushItemWidth(-40);
				ofxImGui::AddParameter(gradientEngine.bTEST_Enable);
				//ImGui::Checkbox("Enable", &TEST_Mode); 
				if (gradientEngine.bTEST_Enable)
				{
					ImGui::SameLine();
					ImGui::Checkbox("LFO", &gradientEngine.bTEST_LFO_Mode);
				}
				ImGui::SliderFloat("Speed", &gradientEngine.TEST_Speed, 0.0f, 1.0f);
				ImGui::PopItemWidth();
			}

			ImGui::Checkbox("Auto-Resize", &auto_resize);
		}
	}
	//ofxImGui::EndWindow(mainSettings);
	guiManager.endWindow();

	//ImGui::PopStyleVar();
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
	flagsw |= flagsWindowsLocked;

	//-

	//ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(ww, hh));

	guiManager.beginWindow(SHOW_Demos, flagsw);
	//if (ofxImGui::BeginWindow("DEMO", mainSettings, flagsw))

	{
		float _spcx;
		float _spcy;
		float _w100;
		float _h100;
		float _w99;
		float _w50;
		float _w33;
		float _w25;
		float _h;
		float _h50;
		float _pad = -80;
		//float _pad = _w33;

		ofxImGuiSurfing::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);
		_h *= 2;

		//--

		// DEMO 1

		//demo bubbles
		ofxImGuiSurfing::AddBigToggle(DEMO1_Enable, _w100, _h);
		if (DEMO1_Enable)
			//if (ImGui::CollapsingHeader("DEMO 1", ImGuiWindowFlags_None))
		{
			if (DEMO1_Enable)
			{
				ImGui::Indent();
				ofxImGuiSurfing::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);
				_h50 = _h;;
				_h *= 2;

				ImGui::PushItemWidth(_pad);
				ofxImGui::AddParameter(DEMO1_Alpha);
				ofxImGui::AddParameter(DEMO1_Auto);
				ofxImGui::AddParameter(DEMO1_Timer);
				if (ofxImGui::AddParameter(DEMO1_Cam))
				{
					DEMO1_Bubbles.setEnableMouseCamera(DEMO1_Cam);
				}
				ImGui::PopItemWidth();

				if (ImGui::Button("Reset", ImVec2(_w100, _h50))) {
					DEMO1_Bubbles.setEnableMouseCamera(DEMO1_Cam);
					DEMO1_Bubbles.resetCamera();
					DEMO1_Alpha = 0.7;
					DEMO1_Timer = 0.2;
				}
				if (ImGui::Button("Load", ImVec2(_w50, _h50))) {
					DEMO1_Bubbles.load();
				}
				ImGui::SameLine();
				if (ImGui::Button("Save", ImVec2(_w50, _h50))) {
					DEMO1_Bubbles.save();
				}

				ImGui::Unindent();
			}
		}
		ImGui::Dummy(ImVec2(0, 5));

		//-

		//ImGui::Dummy(ImVec2(0.0f, 2.0f));
		//ImGui::Separator();
		//ImGui::Dummy(ImVec2(0.0f, 2.0f));

		//--

		// DEMO 2

		// demo spheres
		ofxImGuiSurfing::AddBigToggle(DEMO2_Enable, _w100, _h);
		if (DEMO2_Enable)
			//if (ImGui::CollapsingHeader("DEMO 2", ImGuiWindowFlags_None))
		{
			if (DEMO2_Enable)
			{
				ImGui::Indent();
				ofxImGuiSurfing::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);
				_h50 = _h;
				_h *= 2;

				_pad = -100;
				ImGui::PushItemWidth(_pad);
				ofxImGui::AddParameter(DEMO2_Spheres.alpha_);
				ofxImGui::AddParameter(DEMO2_Spheres.zoom);
				ofxImGui::AddParameter(DEMO2_Spheres.speed);
				if (ofxImGui::AddParameter(DEMO2_Spheres.mouseCam))
				{
					DEMO2_Spheres.setEnableMouseCamera(DEMO2_Spheres.mouseCam);
				}
				ImGui::PopItemWidth();

				if (ImGui::Button("Reset", ImVec2(_w100, _h50))) {
					DEMO2_Spheres.resetCamera();
					DEMO2_Spheres.alpha_ = 0.8;
					DEMO2_Spheres.zoom = 0.5;
					DEMO2_Spheres.speed = 0.5;
				}
				if (ImGui::Button("Load", ImVec2(_w50, _h50))) {
					DEMO2_Spheres.load();
				}
				ImGui::SameLine();
				if (ImGui::Button("Save", ImVec2(_w50, _h50))) {
					DEMO2_Spheres.save();
				}

				ImGui::Unindent();
			}
		}
		ImGui::Dummy(ImVec2(0, 5));

		//-

		//ImGui::Dummy(ImVec2(0.0f, 2.0f));
		//ImGui::Separator();
		//ImGui::Dummy(ImVec2(0.0f, 2.0f));

		//--

		// DEMO 3

		// svg demo
		ofxImGuiSurfing::AddBigToggle(DEMO3_Svg.bEnable, _w99, _h);
		if (DEMO3_Svg.bEnable)
			//if (ImGui::CollapsingHeader("DEMO 3", ImGuiWindowFlags_None))
		{
			if (DEMO3_Svg.bEnable)
			{
				ImGui::Indent();
				ofxImGuiSurfing::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);
				_h50 = _h;
				_h *= 2;

				//ImGui::PushItemWidth(-80);
				ImGui::PushItemWidth(_pad);

				ofxImGui::AddParameter(DEMO3_Svg.bEdit);
				if (DEMO3_Svg.bEdit) ofxImGui::AddParameter(DEMO3_Svg.scaleSvg);
				ofxImGui::AddParameter(DEMO3_Svg.alphaSvg);
				ofxImGui::AddStepper(DEMO3_Svg.blendMode);
				ofxImGui::AddParameter(DEMO3_Svg.blendModeName);
				ofxImGui::AddStepper(DEMO3_Svg.fileIndex);
				ofxImGui::AddParameter(DEMO3_Svg.fileIndexName);
#ifdef USE_SVG_MASK
				ofxImGui::AddParameter(DEMO3_Svg.enable_Mask);
				ofxImGui::AddParameter(DEMO3_Svg.DEMO2_BgWhite);
#endif		
				ofxImGui::AddParameter(DEMO3_Svg.bKeys);

				ImGui::PopItemWidth();

				if (ImGui::Button("Reset", ImVec2(_w99, _h50))) {
					DEMO3_Svg.reset();
				}
				if (ImGui::Button("Load", ImVec2(_w50, _h50))) {
					DEMO3_Svg.load();
				}
				ImGui::SameLine();
				if (ImGui::Button("Save", ImVec2(_w50, _h50))) {
					DEMO3_Svg.save();
				}

				ImGui::Unindent();
			}
		}
		ImGui::Dummy(ImVec2(0, 5));
	}
	//ofxImGui::EndWindow(mainSettings);
	guiManager.endWindow();

	//ImGui::PopStyleVar();
}

//--------------------------------------------------------------
void ofxColorManager::setupGui()
{
	// using Daan ImGui fork

	ImGuiConfigFlags flags;
	flags = ImGuiConfigFlags_DockingEnable;

	// allow move panels out-of-OF-window
	//#ifdef USE_VIEWPORTS
	//	flags |= ImGuiConfigFlags_ViewportsEnable;
	//#endif

	//--

	// setup

	//bool bMouse = false;//false to auto show on window workflow
	//bool bAutoDraw = true;
	//bool bRestore = true;
	//gui.setup(nullptr, bAutoDraw, flags, bRestore, bMouse);
	guiManager.setImGuiAutodraw(true); // -> required when only one single ImGui instance is instantiated on all the oF project.
	guiManager.setup();

	// io
	auto &io = ImGui::GetIO();
	io.ConfigDockingWithShift = true;
	//io.ConfigInputTextCursorBlink = true;

	//--

	// fonts

#ifdef INCLUDE_IMGUI_CUSTOM_THEME_AND_FONT

	//size = 11.f;
	//"telegrama_render.otf";

	fontSizeParam.set("Font Size", 14, 6, 20);
	//fontName = "Ruda-Bold.ttf";

	fontSizeBigParam.set("Font Big Size", 100, 20, 200);
	//fontBigName = "Kazesawa-Extrabold.ttf";

#endif

	//--

//	// theme
//#ifdef INCLUDE_IMGUI_CUSTOM_THEME_AND_FONT
//	ofxImGuiSurfing::ImGui_ThemeMoebiusSurfing();
//
//	//ofxSurfingHelpers::ImGui_ThemeModernDark();
//	//ImGui::StyleColorsDark();
//#endif

	//-

	style = &ImGui::GetStyle();
	style->WindowMenuButtonPosition = ImGuiDir_None;
	//style->TabMinWidthForCloseButton = 200;

	mainSettings = ofxImGui::Settings();

	// for all window panels. lock
	//TODO:
	//flagsWindowsLocked = ImGuiWindowFlags_NoMove;
	//Lock_DockingLayout = Lock_DockingLayout;//refresh callback

	//-

	guiVisible = true;
}

//--------------------------------------------------------------
bool ofxColorManager::draw_Gui()
{
	//bLockMouseByImGui = false;

	guiManager.begin();
	//gui.begin();
	{

		//----

		if (SHOW_MenuBar)
		{
			gui_MenuBar();
		}

		//----

		// a. Define the ofWindow as a docking space

		//ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0)); // Fixes imgui to expected behaviour. Otherwise add in ImGui::DockSpace() [±line 14505] : if (flags & ImGuiDockNodeFlags_PassthruCentralNode) window_flags |= ImGuiWindowFlags_NoBackground;
		//ImGuiID dockNodeID = ImGui::DockSpaceOverViewport(NULL, ImGuiDockNodeFlags_PassthruCentralNode);
		//ImGui::PopStyleColor();

		//-

		// b. Lockable settings 

		// Fixes imgui to expected behaviour. Otherwise add in ImGui::DockSpace() [±line 14505] : if (flags & ImGuiDockNodeFlags_PassthruCentralNode) window_flags |= ImGuiWindowFlags_NoBackground;
		//ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0));
		ImGuiDockNodeFlags flagsDock;
		flagsDock = ImGuiDockNodeFlags_PassthruCentralNode;
		if (Lock_DockingLayout)
		{
			flagsDock |= ImGuiDockNodeFlags_NoResize;
			flagsDock |= ImGuiDockNodeFlags_NoCloseButton;
			//flagsDock |= ImGuiDockNodeFlags_NoTabBar;
			//flagsDock |= ImGuiDockNodeFlags_NoWindowMenuButton;
			//flagsDock |= ImGuiDockNodeFlags_NoMove__;
		}
		ImGuiID dockNodeID = ImGui::DockSpaceOverViewport(NULL, flagsDock);
		//ImGui::PopStyleColor();

		//----

		// check free space
		// central inter docks rectangle

		//TODO:
		ImGuiDockNode* dockNode = ImGui::DockBuilderGetNode(dockNodeID);
		if (dockNode) {
			ImGuiDockNode* centralNode = ImGui::DockBuilderGetCentralNode(dockNodeID);
			// Verifies if the central node is empty (visible empty space for oF)
			if (centralNode && centralNode->IsEmpty())
			{
				ImRect availableSpace = centralNode->Rect();
				//availableSpace.Max = availableSpace.Min + ImGui::GetContentRegionAvail();
				//ImGui::GetForegroundDrawList()->AddRect(availableSpace.GetTL() + ImVec2(8, 8), availableSpace.GetBR() - ImVec2(8, 8), IM_COL32(255, 50, 50, 255));

				ImVec2 viewCenter = availableSpace.GetCenter();
				// Depending on the viewports flag, the XY is either absolute or relative to the oF window.
				if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) viewCenter = viewCenter - ImVec2(ofGetWindowPositionX(), ofGetWindowPositionY());

				float ww = availableSpace.GetSize().x;
				float hh = availableSpace.GetSize().y;
				rectangle_Central_MAX = ofRectangle(viewCenter.x, viewCenter.y, ww, hh);

				bool bDebug = bDebugRectCentral;
				if (bDebug)
				{
					int _wl = 3;
					ofPushStyle();
					ofSetRectMode(OF_RECTMODE_CENTER);
					int g = 0;
					//int g = 255 * ofxSurfingHelpers::Bounce(0.5);
					int a = 255.f * ofMap(ofxSurfingHelpers::Bounce(1), 0.0f, 1.0f, 0.2f, 1.0f, true);
					ofColor c = ofColor(g, a);
					ofSetColor(c);
					ofNoFill();
					ofSetLineWidth(_wl);

					ofRectangle rDebug;
					int pad = 3;
					float ww = availableSpace.GetSize().x - pad;
					float hh = availableSpace.GetSize().y - pad;
					rDebug = ofRectangle(viewCenter.x, viewCenter.y, ww, hh);
					ofDrawRectangle(rDebug);

					//ofDrawRectangle(rectangle_Central_MAX);
					ofSetRectMode(OF_RECTMODE_CORNER);
					ofPopStyle();
				}
				// move to left corner mode
				rectangle_Central_MAX.translate(-ww / 2, -hh / 2);

				//-

				static ofRectangle rectangle_Central_MAX_PRE;
				if (rectangle_Central_MAX_PRE != rectangle_Central_MAX) {//update when layout canges..
					rectangle_Central_MAX_PRE = rectangle_Central_MAX;

					// fit exact rectangle to borders and scaled to fit
					rectangle_Central = DEMO3_Svg.getRect();
					rectangle_Central.scaleTo(rectangle_Central_MAX, OF_ASPECT_RATIO_KEEP, OF_ALIGN_HORZ_CENTER, OF_ALIGN_VERT_CENTER);

					// rescaled rectangle a bit
					float _scale = 0.7f;
					rectangle_Central_Transposed = rectangle_Central;
					rectangle_Central_Transposed.scaleFromCenter(_scale, _scale);//scale down to fit layout spacing better
					rectangle_Central_Transposed.translateY(rectangle_Central.getHeight() * 0.07);//move down a bit
					DEMO3_Svg.setRect(rectangle_Central_Transposed);
				}
			}
		}

		//----

#ifdef INCLUDE_IMGUI_CUSTOM_THEME_AND_FONT
		ImGui::PushFont(customFont);
#endif
		//--

		if (SHOW_ALL_GUI)
		{
			if (SHOW_Panels) gui_Panels();

			if (SHOW_Palette) gui_Palette();
			if (SHOW_Editor) gui_Editor();
			if (SHOW_Presets) gui_Presets();

#ifdef MODE_TEXT_INPUT_WORKAROUND
			if ((MODE_NewPreset && bAutoHide_Name) || !bAutoHide_Name)//hide text input
			//if (MODE_NewPreset)//hide text input
			//if (SHOW_Presets /*&& MODE_NewPreset*/) gui_TextInput();
				gui_TextInput();
#endif
			if (SHOW_Kit) gui_Kit();
			if (SHOW_Picker) gui_Picker();
			if (SHOW_Library) gui_Library();
#ifdef MODE_BACKGROUND
			if (SHOW_BackGround) gui_Background();
#endif
			if (SHOW_PanelEngines)
			{
				//gui_EnginesPanel();
				if (SHOW_Engines) gui_EnginesPanel();
				if (SHOW_Theory) gui_Theory();
				if (SHOW_Range) gui_Range();

#ifdef USE_COLOR_LOVERS
				if (SHOW_ColourLovers)
				{
					//bCheckMouseOverTextInputLovers = colourLoversHelper.draw();
					bCheckMouseOverTextInputLovers = colourLoversHelper.isTextInputTyping();
				}
				else
				{
					bCheckMouseOverTextInputLovers = false;
				}
#endif
				if (SHOW_Quantizer) colorQuantizer.draw_Gui();
			}
			if (SHOW_LinkExport) gui_LinkExport();
			if (SHOW_Demos) gui_Demo();

#ifndef USE_MINIMAL_GUI
			if (SHOW_Advanced) gui_Advanced();
#endif
			if (SHOW_LayoutsAdvanced) gui_LayoutsAdvanced();
			if (SHOW_Layouts) gui_LayoutsPresets();

			if (SHOW_Gradient) gui_Gradient();
			//if (gradientEngine.SHOW_CurveEditor) gui_Gradient();
			//if (gradientEngine.SHOW_Gradient) gui_Gradient();
		}

		//--

#ifdef USE_VERSION_CHECKER
		// app version checker
		if (_isAnUpdateAvailable)
		{
			//const float x = 20;
			//const float y = 20;
			//const float padding = 20;
			//ofDrawBitmapString("A new version is available !", x, y);
			//ofDrawBitmapString("\tdisplayNumber :\t" + _newVersion.displayNumber, x, y + padding);
			//ofDrawBitmapString("\tnumber :\t" + ofToString(_newVersion.number), x, y + padding * 2);
			//ofDrawBitmapString("\tisCritical :\t" + ofToString(_newVersion.isCritical), x, y + padding * 3);
			//ofDrawBitmapString("\turl :\t\t" + _newVersion.url, x, y + padding * 4);

			std::string str;
			str += "A new version is available!";
			str += "\n";
			str += "\n";
			str += "\tDisplay Number :\t" + ofToString(_newVersion.displayNumber);
			str += "\n";
			str += "\tNumber :\t" + ofToString(_newVersion.number);
			str += "\n";
			str += "\tis Critical :\t" + ofToString(_newVersion.isCritical ? "TRUE" : "FALSE");
			str += "\n";
			str += "\turl :\t\t" + ofToString(_newVersion.url);

			bool p_open = true;
			{
				ImGuiWindowFlags flags;
				flags = ImGuiWindowFlags_AlwaysAutoResize;
				//flags = ImGuiWindowFlags_None;

				ImGui::Begin("APP VERSION CHECKER", &p_open, flags);
				{
					ImGui::Text(APP_RELEASE_NAME);
					//ImGui::Separator();
					ImGui::Dummy(ImVec2(0.0f, 10.0f));
					ImGui::Text(str.c_str());
					ImGui::Dummy(ImVec2(0, 10.0f));
				}
				ImGui::End();
			}
	}
#endif

		//--

#ifdef INCLUDE_IMGUI_CUSTOM_THEME_AND_FONT
		ImGui::PopFont();
#endif

		//--

		//extra advanced panels
		if (SHOW_EditTheme)
		{
			guiManager.beginWindow(SHOW_EditTheme, ImGuiWindowFlags_None);
			//if (ofxImGui::BeginWindow("THEME", mainSettings, ImGuiWindowFlags_None))
			{
				ImGui::ShowStyleEditor();
			}
			//ofxImGui::EndWindow(mainSettings);
			guiManager.endWindow();

			guiManager.beginWindow("STYLE SLECTOR", ImGuiWindowFlags_None);
			//if (ofxImGui::BeginWindow("STYLE SLECTOR", mainSettings, ImGuiWindowFlags_None))
			{
				ImGui::ShowStyleSelector(" ");
			}
			//ofxImGui::EndWindow(mainSettings);
			guiManager.endWindow();

			//if (ofxImGui::BeginWindow("VIEW PORT THUMBS", mainSettings, ImGuiWindowFlags_None))
			//{
			//	ImGui::ShowViewportThumbnails();
			//}
			//ofxImGui::EndWindow(mainSettings);
		}

		if (SHOW_About) { gui_About(&SHOW_About); }


		//----

		// keyboard disabler

		////snippet
		////mouse lockers
		//bLockMouseByImGui = bLockMouseByImGui | ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
		//bLockMouseByImGui = bLockMouseByImGui | ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);
		//bLockMouseByImGui = bLockMouseByImGui | ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);

		//--
}
	//gui.end();
	guiManager.end();

	mouseLockedByGui = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);

	/*
	//// two different modes:

	//a.
	//// mouse over any panel or over text input only
	//if (bCheckMouseOverTextInput) return bTextInputActive;
	//else return mainSettings2.mouseLockedByGui;

	//b.
	//return ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
	//bool b = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && bTextInputActive;

	//bool b =
	//	ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) &&
	//	bTextInputActive &&
	//	bCheckMouseOverTextInputLovers;
	*/

	//-

	bLockAllKeysByGui =
		//bLockMouseByImGui &&
		ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) &&
		bTextInputActive &&
		bCheckMouseOverTextInputLovers;

	return bLockAllKeysByGui;
}

//--------------------------------------------------------------
void ofxColorManager::palette_FromTheory(int p)
{
	ofLogNotice(__FUNCTION__) << p;
	if (p < 0) return;

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
	//	bExportFlag = true;
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
	complementTriad.generateComplementaryTriad(triadSpread);
	//complementTriad.generateComplementaryTriad();
	//complementTriad.generateComplementaryTriad(analogSpread);
	monochrome.generateMonoChromatic(ofxColorPalette::SATURATION, numColors_Theory_G2);
	monochromeBrightness.generateMonoChromatic(ofxColorPalette::BRIGHTNESS, numColors_Theory_G2);
	analogue.generateAnalogous(numColors_Theory_G2, analogSpread);
	//analogue.generateAnalogous(numColors_Theory_G2, 0.2);

	//TODO:
	//add randoms...
	//add more tweakers like in analog

	//---

	// G2

	refresh_Theory_G2_2();
	//TODO: reduce calls...
}

//--------------------------------------------------------------
void ofxColorManager::palettes_Resize()
{
	//TODO: not required ?
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
		ofLogVerbose(__FUNCTION__) << i;// << " " << _scheme->;
	}
}

//--------------------------------------------------------------
void ofxColorManager::setupTheory_G1()
{
	ofLogNotice(__FUNCTION__);

	numColors_Engines.set("Amount Colors", 8, 2, MAX_PALETTE_COLORS);
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
	params_ColorTheory_G1.add(numColors_Theory_G1.set("Amnt Colors", 8, 2, MAX_PALETTE_COLORS));
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
	ofLogNotice(__FUNCTION__) << ofToString(palette.size()) << " : " << ofToString(c);

	palette.push_back(c);

	gradientEngine.addColor(c);

	//--

	// workflow 
	last_Index_ColorPalette.setMax(palette.size() - 1);

	// workflow 
	//TODO: could be better to run less times..
	//update flag? on build palette?
	doGetPaletteToTweaker();
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
		for (unsigned i = 0; i < palette.size(); ++i) ofLogNotice(__FUNCTION__) << palette[i];
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

	if (DEMO1_Enable || SHOW_About) DEMO1_Bubbles.clear();

	//--

	//TODO: ?
	refresh_Palette_TARGET(palette);

	last_Index_ColorPalette.setMax(palette.size() - 1);
}

//----

// callbacks

//--------------------------------------------------------------
void ofxColorManager::Changed_ColorPicked(ofFloatColor &c)
{
	ofLogNotice(__FUNCTION__) << "color_Picked : " << ofToString(c);// color_Picked

	// update TARGET color pointer (ofApp)
	if (color_TARGET != nullptr)
	{
		color_TARGET->set(c);
	}

	ENABLE_CALLBACKS_Pickers = false;
	refresh_Pick_ToHSB();
	ENABLE_CALLBACKS_Pickers = true;

	//--

	// workflow
	if (ENABLE_CALLBACKS_Engines) refresh_Pick_ToEngines();

	//--

	////TODO: ?
	//if (SHOW_Presets)
	//{
	//	// workflow
	//	bFlag_refresh_EnginesFromPalette = true;
	//}
}

//--------------------------------------------------------------
void ofxColorManager::Changed_ColorClicked(ofFloatColor &c)
{
	ofLogNotice(__FUNCTION__) << "color_Clicked : " << ofToString(c);// color_Clicked

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

		ENABLE_CALLBACKS_Engines = false;
		build_Palette_SortFlip();
		ENABLE_CALLBACKS_Engines = true;
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
		refresh_Theory_G2();
	}

	//else if (name == colorScheme.getName())
	//{
	//	refresh_Theory_G1();
	//}

	else if (name == last_Index_Theory_PickPalette.getName())
	{
		ofLogWarning(__FUNCTION__) << "last_Index_Theory_PickPalette : " << last_Index_Theory_PickPalette;

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

		if (bLast_Index_Theory)
		{
			//enable
			if (last_Index_Theory_PickPalette >= 0 &&
				last_Index_Theory_PickPalette < NUM_COLOR_THEORY_TYPES_G1)
			{
				theory_Types_G1[last_Index_Theory_PickPalette] = true;
			}
			else if (last_Index_Theory_PickPalette >= NUM_COLOR_THEORY_TYPES_G1 &&
				last_Index_Theory_PickPalette < NUM_COLOR_THEORY_TYPES_G1 + NUM_COLOR_THEORY_TYPES_G2)
			{
				types_Theory_G2[last_Index_Theory_PickPalette - NUM_COLOR_THEORY_TYPES_G1] = true;
			}
		}

		//// workflow
		//// -1: disable all
		//if (last_Index_Theory_PickPalette == -1)
		//{
		//	for (int i = 0; i < NUM_COLOR_THEORY_TYPES_G1; i++)
		//	{
		//		theory_Types_G1[i] = false;
		//	}
		//	for (int i = NUM_COLOR_THEORY_TYPES_G1; i < NUM_COLOR_THEORY_TYPES_G1 + NUM_COLOR_THEORY_TYPES_G2; i++)
		//	{
		//		types_Theory_G2[last_Index_Theory_PickPalette - NUM_COLOR_THEORY_TYPES_G1] = false;
		//	}
		//}
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

	//// library
	//else if (name == SHOW_Library.getName())
	//{
	//	colorBrowser.setEnableKeys(SHOW_Library);
	//}

	// kit
	else if (name == last_Index_Kit.getName())
	{
		int _i = ofClamp(last_Index_Kit, last_Index_Kit.getMin(), last_Index_Kit.getMax());
		if (_i != last_Index_Kit.get()) last_Index_Kit.setWithoutEventNotifications(_i);

		//int _i = MAX(last_Index_Kit, files_KitNames.size() - 1);
		path_Kit = files_KitNames[_i];
		//path_Kit = "USER KIT"; // default

		preset_RefreshFiles(true);
	}

	// layout
	else if (name == SHOW_Layouts.getName())
	{
		// workflow
		if (!SHOW_Layouts)
		{
			SHOW_LayoutsAdvanced = false;
		}
	}

	// link workflow
	else if (name == SHOW_LinkExport.getName())
	{
		if (SHOW_LinkExport)

			// workflow
			if (!bExportByTCP)
			{
				bExportByTCP = true;
			}
	}

	//-

#ifdef LINK_TCP_MASTER_CLIENT_KU

	// tcp link
	else if (name == bExportByTCP.getName())
	{
		//reconnect
		if (bExportByTCP)
		{
			setupLink();
		}
		else
		{
			//auto srv = TCP_Sender.TCPServerRef();
			//srv.

			//is not closing..
			TCP_Sender.close();
		}
}
#endif

	//-

	// layout
	else if (name == Lock_DockingLayout.getName())
	{
		if (!Lock_DockingLayout)
		{
			flagsWindowsLocked = ImGuiWindowFlags_None;
		}
		else
		{
			flagsWindowsLocked = ImGuiWindowFlags_NoMove;
			//flagsWindowsLocked |= ImGuiWindowFlags_NoResize;
			//flagsWindowsLocked |= ImGuiWindowFlags_NoTitleBar;
			//flagsWindowsLocked |= ImGuiWindowFlags_NoCollapse;
			//flagsWindowsLocked |= ImGuiWindowFlags_NoDecoration;
			//flagsWindowsLocked |= ImGuiWindowFlags_NoBackground;
			//flagsWindowsLocked |= ImGuiDockNodeFlags_AutoHideTabBar;
			//flagsWindowsLocked |= ImGuiDockNodeFlags_NoTabBar;
			//flagsWindowsLocked |= ImGuiDockNodeFlags_NoCloseButton;
		}
	}

	// layout
	else if (name == bResponsive_Panels.getName())
	{
		if (bResponsive_Panels)
		{
			bFitLayout = false;
		}
	}

	// layout modes
	else if (name == appLayoutIndex.getName())
	{
		appLayoutIndex = ofClamp(appLayoutIndex.get(), appLayoutIndex.getMin(), appLayoutIndex.getMax());

		if (appLayoutIndex != appLayoutIndex_PRE /*&& appLayoutIndex_PRE != -1*/) //changed
		{
			// 1. autosave

			if (bAutoSave_Layout)
			{
				// workaround:
				// must save here bc usually we use the fallged on update save...
				// only once per cycle allowed this way.
				//force to ensure save bc update chain load and save below
				//saveAppLayout(AppLayouts(appLayoutIndex_PRE));
				std::string __ini_to_save_Str = getLayoutName(AppLayouts(appLayoutIndex_PRE));
				const char* __ini_to_save = NULL;
				__ini_to_save = __ini_to_save_Str.c_str();//flags to save on update
				ImGui::SaveIniSettingsToDisk(__ini_to_save);
				ofxSurfingHelpers::saveGroup(params_LayoutPanelsState, __ini_to_save_Str + ".json");
				//if (ini_to_save)
				//{
				//	ImGui::SaveIniSettingsToDisk(ini_to_save);
				//	ini_to_save = NULL;
				//}
			}

			appLayoutIndex_PRE = appLayoutIndex;

			//loadAppLayout(AppLayouts(appLayoutIndex.get()));
		//}

			// 2. load layout
			loadAppLayout(AppLayouts(appLayoutIndex.get()));
		}
	}

	//--

	// font preset name size
#ifdef INCLUDE_IMGUI_CUSTOM_THEME_AND_FONT
	else if (name == fontSizeParam.getName())
	{
		auto &io = ImGui::GetIO();
		auto normalCharRanges = io.Fonts->GetGlyphRangesDefault();

		fontName = "Ruda-Bold.ttf";

		std::string _path = "assets/fonts/";//assets folder
		customFont = guiManager.getGui().addFont(_path + fontName, fontSizeParam, nullptr, normalCharRanges);
		//customFont = gui.addFont(_path + fontName, fontSizeParam, nullptr, normalCharRanges);

		io.FontDefault = customFont;
	}
	else if (name == fontSizeBigParam.getName())
	{
		auto &io = ImGui::GetIO();
		auto normalCharRanges = io.Fonts->GetGlyphRangesDefault();

		fontBigName = "Kazesawa-Extrabold.ttf";

		std::string _path = "assets/fonts/";//assets folder
		customFontBig = guiManager.getGui().addFont(_path + fontBigName, fontSizeBigParam, nullptr, normalCharRanges);
		//customFontBig = gui.addFont(_path + fontBigName, fontSizeBigParam, nullptr, normalCharRanges);
	}
#endif

	//--

	// advanced
	else if (name == SHOW_Advanced.getName())
	{
		colorQuantizer.SHOW_AdvancedLayout = SHOW_Advanced;
		colourLoversHelper.SHOW_AdvancedLayout = SHOW_Advanced;

		if (SHOW_Advanced)
		{
			//flagsWindowsLocked = ImGuiWindowFlags_None;

			//// workflow
			////if (!SHOW_MenuBar) SHOW_MenuBar = true;
		}
		else
		{
			// workflow
			//if (SHOW_MenuBar) SHOW_MenuBar = false;
			if (SHOW_EditTheme) SHOW_EditTheme = false;
		}

		//// workflow
		//if (!SHOW_MenuBar) SHOW_MenuBar = true;

		// workflow
		buildHelpInfo();
	}

	// layout
	else if (name == bFitLayout.getName())
	{
		if (bFitLayout)
		{
			bResponsive_Panels = false;
		}
	}

	// app modes
	else if (name == AppEnginesMode.getName())
	{
		//clamp
		AppEnginesMode = ofClamp(
			AppEnginesMode.get(),
			AppEnginesMode.getMin(),
			AppEnginesMode.getMax());

		////cycle
		//if (AppEnginesMode > AppEnginesMode.getMax()) AppEnginesMode.setWithoutEventNotifications(AppEnginesMode.getMin());
		//else if (AppEnginesMode < 0) AppEnginesMode.setWithoutEventNotifications(AppEnginesMode.getMax());
		//if (current_element != AppEnginesMode) current_element = AppEnginesMode.get();
		//AppMode_name = ofToString(element_names[AppEnginesMode.get()]);

		switch (AppEnginesMode.get())
		{
			// none
		case 0:
		{
			SHOW_Theory = false;
			SHOW_Range = false;
			SHOW_ColourLovers = false;
			SHOW_Quantizer = false;
		}
		break;

		// theory
		case 1:
		{
			SHOW_Theory = true;
			SHOW_Range = false;
			SHOW_ColourLovers = false;
			SHOW_Quantizer = false;
			bLast_Index_Theory = true;
		}
		break;

		// range
		case 2:
		{
			SHOW_Theory = false;
			SHOW_Range = true;
			SHOW_ColourLovers = false;
			SHOW_Quantizer = false;
			bLast_Index_Range = true;
		}
		break;

		// lovers
		case 3:
		{
			SHOW_Theory = false;
			SHOW_Range = false;
			SHOW_ColourLovers = true;
			SHOW_Quantizer = false;
		}
		break;

		// quantizer
		case 4:
		{
			SHOW_Theory = false;
			SHOW_Range = false;
			SHOW_ColourLovers = false;
			SHOW_Quantizer = true;
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
		//// workflow
		//focus_1 = MODE_NewPreset;

		if (!MODE_NewPreset) {
			bBlockedKeys = false;
			bTextInputActive = false;
		}
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
			last_Index_ColorPalette.get(),
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
		//TODO: ?
		//build_Palette_Engine();

		last_Index_Theory = (int)ofClamp(
			last_Index_Theory,
			last_Index_Theory.getMin(),
			last_Index_Theory.getMax());

		last_Index_Type = 2;

		if (!bLast_Index_Theory) bLast_Index_Theory = true;
	}

	else if (name == last_Index_Range.getName())
	{
		last_Index_Range = (int)ofClamp(
			last_Index_Range.get(),
			last_Index_Range.getMin(),
			last_Index_Range.getMax());

		last_Index_Type = 3;

		if (!bLast_Index_Range) bLast_Index_Range = true;
	}

	//----

	// num colors

	else if (name == numColors_Engines.getName())
	{
		numColors_Engines = ofClamp(numColors_Engines.get(), numColors_Engines.getMin(), numColors_Engines.getMax());

		numColors_Theory_G1.setWithoutEventNotifications(numColors_Engines);
		numColors_Theory_G2.setWithoutEventNotifications(numColors_Engines);

		numColors_Range = numColors_Engines;

		if (SHOW_Quantizer) colorQuantizer.setNumColors(numColors_Engines);

		palettes_Resize();

		//workflow
		//when no type is enabled
		if (!bLast_Index_Theory) {
			last_Index_Theory = 0;
			bLast_Index_Theory = true;
		}
		if (!bLast_Index_Range) {
			last_Index_Range = 0;
			bLast_Index_Range = true;
		}
	}

	else if (name == numColors_Theory_G2.getName())
	{
		numColors_Theory_G1.setWithoutEventNotifications(numColors_Theory_G2);
		numColors_Engines.setWithoutEventNotifications(numColors_Theory_G2);

		palettes_Resize();
	}

	else if (name == analogSpread.getName())
	{
		//TODO: ?
		refresh_Theory_G1();
		refresh_Theory_G2();

		// workflow
		if (bAuto_Build_Palette)
		{
			//if (SHOW_Theory) palette_FromTheory(last_Index_Theory);
			build_Palette_Engine();
		}
	}

	else if (name == triadSpread.getName())
	{
		//TODO: ?
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

	if (ENABLE_CALLBACKS_Pickers)
	{
		if (name == color_HUE.getName())
		{
			ofLogNotice(__FUNCTION__) << "" << name << " : " << e;
			ofFloatColor c;
			c.set(color_Picked.get());
			c.setHue((color_HUE + 0.5f) / 255.f);
			color_Picked.set(c);
		}
		else if (name == color_SAT.getName())
		{
			ofLogNotice(__FUNCTION__) << "" << name << " : " << e;
			ofFloatColor c;
			c.set(color_Picked.get());
			c.setSaturation((color_SAT + 0.5f) / 255.f);
			color_Picked.set(c);
		}
		else if (name == color_BRG.getName())
		{
			ofLogNotice(__FUNCTION__) << "" << name << " : " << e;
			ofFloatColor c;
			c.set(color_Picked.get());
			c.setBrightness((color_BRG + 0.5f) / 255.f);
			color_Picked.set(c);
		}
	}

	//----

	//engine panels

	// theory

	if (name == SHOW_Theory.getName())
	{
		bLast_Index_Theory = SHOW_Theory;

		if (SHOW_Theory.get())
		{
			AppEnginesMode = 1;

			// workflow
			//if (SHOW_ColourLovers || SHOW_Range)
			{
				refresh_EnginesFromPalette();
			}

			// workflow
			if (bEditPalette) bEditPalette = false;

			// workflow
			if (bAuto_Theory_FromPicker)
			{
				if (bAuto_Color1_FromPicker_Range)
				{
					color_Picked.set(color_1_Range.get());
				}
				else if (bAuto_Color2_FromPicker_Range)
				{
					color_Picked.set(color_2_Range.get());
				}
			}

			//TODO:
			//if (name_TARGET != nullptr) textInput_New = name_TARGET[0];//set

			int i = last_Index_Theory_PickPalette;
			if (i < NUM_COLOR_THEORY_TYPES_G1)
				name_Theory = theory_Types_G1[i].getName();
			else if (i > NUM_COLOR_THEORY_TYPES_G1 && i < NUM_COLOR_THEORY_TYPES_G1 + NUM_COLOR_THEORY_TYPES_G2)
				name_Theory = types_Theory_G2[i - NUM_COLOR_THEORY_TYPES_G1].getName();

		}
	}

	// range

	else if (name == SHOW_Range.getName())
	{
		bLast_Index_Range = SHOW_Range;

		if (SHOW_Range.get())
		{
			AppEnginesMode = 2;

			//// workflow
			//if (SHOW_ColourLovers || SHOW_Range)
			{
				refresh_EnginesFromPalette();
			}

			// workflow
			if (bEditPalette) bEditPalette = false;

			// workflow
			refresh_Pick_ToEngines();
		}
	}

	// lovers

	else if (name == SHOW_ColourLovers.getName())
	{
		colourLoversHelper.setEnableKeys(SHOW_ColourLovers);

		if (SHOW_ColourLovers)
		{
			AppEnginesMode = 3;

			//// workflow
			//if (bEditPalette) bEditPalette = false;

			// workflow
			refresh_EnginesFromPalette();

			// workflow
			colourLoversHelper.refreshPalette();
		}
	}

	// picture quantizer

	else if (name == SHOW_Quantizer.getName())
	{
		if (SHOW_Quantizer)
		{
			AppEnginesMode = 4;

			//// workflow
			//if (bEditPalette) bEditPalette = false;

			// workflow
			colorQuantizer.build();
		}

		colorQuantizer.setActive(SHOW_Quantizer);
	}

	//----

	// editor

	else if (name == bRandomColor.getName() && bRandomColor.get())
	{
		bRandomColor = false;

		doRandomizeColorPicker();
	}

	//----

	// edit manager

	else if (name == bEditPalette.getName())
	{
		if (bEditPalette)
		{
			// selected color from palette
			last_Index_ColorPalette = ofClamp(last_Index_ColorPalette, 0, last_Index_ColorPalette.getMax());

			// workflow
			{
				saturationTweak = 0;
				brigthnesTweak = 0;
				palette_AUX = palette;
			}
		}
	}

	else if (name == bAddColor.getName())
	{
		if (bAddColor)
		{
			bAddColor = false;

			////TODO:
			//// ColorUtil::interpolate(const std::vector<ofColor> & colors, int steps) {
			//palette = ColorUtil::interpolate(palette, palette.size() + 1);

			//-

			palette_AddColor(ofColor(color_Picked.get()));

			//--

			refresh_Palette_TARGET(palette);

			// export
			if (bAutoExportPreset)
			{
				bExportFlag = true;
			}
		}
	}

	else if (name == bRemoveColor.getName())
	{
		if (bRemoveColor)
		{
			bRemoveColor = false;

			if (palette.size() > 1) {
				// remove selected
				palette_RemoveColor(last_Index_ColorPalette);

				// workflow
				//build_Palette_Engine();
				last_Index_ColorPalette = last_Index_ColorPalette.getMax();//select last
			}
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

			if (palette.size() < 1) {
				palette_AddColor(color_Picked.get());
			}

			//last_Index_ColorPalette.setMax(palette.size() - 1);
			last_Index_ColorPalette = last_Index_ColorPalette.getMax();
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

		// set background color from first/last palette color
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

	//--

	// demos
	else if (name == SHOW_Demos.getName())
	{
		// bubbles
		DEMO1_Cam = false;
		DEMO1_Bubbles.setEnableMouseCamera(DEMO1_Cam);

		if (!SHOW_Demos) DEMO3_Svg.bEdit = false;

		// svg
		DEMO3_Svg.setEnableKeys(SHOW_Demos);
		//DEMO3_Svg.DEMO2_Edit = SHOW_Demos;
		////if (SHOW_Demos) DEMO3_Svg.DEMO2_Edit = true;

		//// spheres
		//DEMO2_Spheres.mouseCam = DEMO2_Enable;
		////DEMO2_Spheres.mouseCam = false;
		////if (SHOW_Demos) DEMO2_Spheres.mouseCam = true;
		//DEMO2_Spheres.setEnableMouseCamera(DEMO2_Spheres.mouseCam);
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
	if (bRange_Intitiated && ENABLE_CALLBACKS_Range)
	{
		std::string name = e.getName();
		ofLogNotice(__FUNCTION__) << name << " : " << e;

		if (false) {}

		//-

		// num colors

		else if (name == numColors_Range.getName())
		{
			// all the num colors are linked
			numColors_Engines.setWithoutEventNotifications(numColors_Range);
			numColors_Theory_G1.setWithoutEventNotifications(numColors_Range);
			numColors_Theory_G2.setWithoutEventNotifications(numColors_Range);

			if (autoGenerate_Range)
				generate_Range(color_1_Range.get(), color_2_Range.get());

			//auto create palette
			refresh_Range_AutoUpdate();
		}

		//-

		else if (name == bAuto_Color1_FromPicker_Range.getName())
		{
			if (bAuto_Color1_FromPicker_Range)
			{
				bAuto_Color2_FromPicker_Range = false;
				if (!bEditPalette) color_Picked.set(color_1_Range.get());
			}
		}
		else if (name == bAuto_Color2_FromPicker_Range.getName())
		{
			if (bAuto_Color2_FromPicker_Range)
			{
				bAuto_Color1_FromPicker_Range = false;
				if (!bEditPalette) color_Picked.set(color_2_Range.get());
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

	// workflow
	//TODO:
	if (palette.size() > 0)
	{
		color_Picked = palette[0];
	}
}

//--------------------------------------------------------------
void ofxColorManager::palette_FromRange(int index)
{
	ofLogNotice(__FUNCTION__) << index;

	if (index < 0 || !bLast_Index_Range) return;

	//--

	last_Index_Range = index;

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

				ofLogVerbose(__FUNCTION__) << "[" << last_Index_Range << "][" << (j - st) << "] > " << ofToString(c);
			}
		}

		//--

		// workflow
		refresh_Background();

		//--

		// export
		if (bAutoExportPreset)
		{
			ofLogNotice(__FUNCTION__) << "Auto EXPORT";
			bExportFlag = true;
		}
	}
}

//--------------------------------------------------------------
void ofxColorManager::refresh_Range_AutoUpdate()
{
	ofLogNotice(__FUNCTION__);

	if (last_Index_Range < 0 || !bLast_Index_Range) return;//exclude

	//---

	// auto create palette
	if (autoGenerate_Range)
	{
		int i = last_Index_Range;

		types_Range[i] = true;//toggle the active range type label button
		name_Range = types_Range[i].getName();
		last_Index_Type = 3;

		//--

		//ofColor c;
		//if (bAuto_Color1_FromPicker_Range) c.set(color_1_Range.get());
		//else if (bAuto_Color2_FromPicker_Range) c.set(color_2_Range.get());
		//if (color_TARGET != nullptr)
		//{
		//	color_TARGET->set(c);
		//}

		//myColor_BACK.set(c);
		////if (myColor_BACK != nullptr)
		////{
		////	myColor_BACK->set(c);
		////}
		////if (myColor_BACK) {
		////myColor_BACK
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

	bool bDebug = true;
	if (bDebug)
	{
		if (!ENABLE_keys || bBlockedKeys) {
			ofLogError(__FUNCTION__) << "Key commands are disabled!";
			ofLogError(__FUNCTION__) << "ENABLE_keys: " << (ENABLE_keys ? "TRUE" : "FALSE");
			ofLogError(__FUNCTION__) << "bBlockedKeys : " << (bBlockedKeys ? "TRUE" : "FALSE");
		}
		else
			ofLogNotice(__FUNCTION__)
			<< "keycode: " << key
			<< " char: " << (char)key
			<< (mod_CONTROL ? " + CONTROL" : "")
			<< (mod_SHIFT ? " + SHIFT" : "")
			<< (mod_ALT ? " + ALT" : "");
	}

	if (key == 'K') ENABLE_keys = !ENABLE_keys;

	if (ENABLE_keys && !bBlockedKeys)
	{
		//----

		// presets

		if ((!SHOW_Theory && !SHOW_Range && !SHOW_ColourLovers && !SHOW_Quantizer)
			|| (SHOW_Presets && !SHOW_Library))
		{
			// browse presets
			if (key == OF_KEY_LEFT && !mod_CONTROL)
			{
				preset_Previous();
				return;
			}

			else if ((key == OF_KEY_RIGHT || key == ' ') && !mod_CONTROL)
			{
				preset_Next();
				return;
			}

			//select a preset by random
			else if ((key == OF_KEY_RETURN && !mod_CONTROL))
			{
				preset_RandomIndex();
				return;
			}

			// shift
			else if (key == OF_KEY_LEFT && mod_CONTROL)
			{
				build_Palette_SortShift(true);
				return;
			}
			else if ((key == OF_KEY_RIGHT || key == ' ') && mod_CONTROL)
			{
				build_Palette_SortShift();
				return;
			}
		}
		else if (SHOW_Presets)
			if (key == ' ' && !mod_CONTROL)
			{
				preset_Next();
				return;
			}

		//--

		// general key commands

		if (false) {}

		// help
		else if ((key == 'h' || key == 'H') && !mod_CONTROL)
		{
			SHOW_HelpInfo = !SHOW_HelpInfo;
		}

		// about
		else if ((key == 'a' && !mod_CONTROL && !mod_ALT) /*&& mod_CONTROL*/)
		{
			SHOW_About = !SHOW_About;
		}

		// gui
		else if (key == 'g' && !mod_CONTROL)
		{
			setToggleVisible();

			// workflow
			if (!SHOW_ALL_GUI) SHOW_HelpInfo = false;
		}

		// advanced Ctrl+Alt+a
		else if (key == 'a' && mod_CONTROL && mod_ALT)
		{
			SHOW_Advanced = !SHOW_Advanced;
		}

		// splash
		if (key == 'A')
		{
			splash.start();
		}

		// unlock dock Ctrl+Alt+a
		else if ((key == 'L' || key == 'l')) //&& mod_CONTROL && !mod_ALT)
		{
			Lock_DockingLayout = !Lock_DockingLayout;
		}

		//----

		// extra

#ifdef USE_DEBUG_LAYOUT
		else if (key == 'M')
		{
			mouseRuler.toggleVisibility();
			//DEMO1_Bubbles.toggleMouseCamera();
		}
#endif
		// TEST
		else if (key == 'T')
		{
			gradientEngine.setToggleTest();
		}

		else if (key == 'e' || key == 'E')
		{
			bEditPalette = !bEditPalette;
		}

		//--

		// workflow
		if (bEditPalette)
		{
			// randomize color
			if ((key == 'r' || key == OF_KEY_RETURN) && !mod_CONTROL)
			{
				doRandomizeColorPicker();
			}

			// randomize sort palette
			else if (key == OF_KEY_BACKSPACE && !mod_CONTROL && !SHOW_Quantizer && !SHOW_ColourLovers)
			{
				// workflow
				if (bEditPalette) bEditPalette = false;

				build_Palette_SortRandom();
			}

			// flip palette
			else if (key == OF_KEY_BACKSPACE && mod_CONTROL && !SHOW_Quantizer && !SHOW_ColourLovers)
			{
				ENABLE_CALLBACKS_Engines = false;
				build_Palette_SortFlip();
				ENABLE_CALLBACKS_Engines = true;
			}
		}

		// amount colors

		if (!SHOW_Theory && !SHOW_Range)
		{
			if (key == '-' && !mod_CONTROL)
			{
				bRemoveColor = true;
			}
			else if (key == '+' && !mod_CONTROL)
			{
				bAddColor = true;
			}
		}

		//--

		// amount colors

		if (key == '-' && !mod_CONTROL)
		{
			if (numColors_Engines > 0) numColors_Engines--;
		}
		else if (key == '+' && !mod_CONTROL)
		{
			if (numColors_Engines < numColors_Engines.getMax()) numColors_Engines++;
		}

		//-----

		// app modes: browse engines

		//TODO fix

		else if (key == OF_KEY_TAB && mod_CONTROL && SHOW_Engines)
		{
			if (AppEnginesMode < AppEnginesMode.getMax()) AppEnginesMode++;
			else if (AppEnginesMode == AppEnginesMode.getMax()) AppEnginesMode = 0;
		}
		//else if (key == OF_KEY_TAB && !mod_CONTROL)
		//{
		//	if (AppEnginesMode > AppEnginesMode.getMin()) AppEnginesMode--;
		//	else if (AppEnginesMode == AppEnginesMode.getMin()) AppEnginesMode = AppEnginesMode.getMax();
		//}

		//--

		// layout modes

		else if (key == OF_KEY_TAB && !mod_CONTROL)
		{
			if (appLayoutIndex > appLayoutIndex.getMin()) appLayoutIndex++;
			else if (appLayoutIndex == appLayoutIndex.getMin()) appLayoutIndex = appLayoutIndex.getMax();

			//if (appLayoutIndex < 3) loadAppLayout(AppLayouts(appLayoutIndex + 1));
			//else if (appLayoutIndex == 3) loadAppLayout(AppLayouts(0));
		}

		//-----

		// editor

		if (bEditPalette && !SHOW_Library)
			// && SHOW_Presets && !SHOW_Theory && !SHOW_Range && !SHOW_ColourLovers && !SHOW_Quantizer)
		{
			if (!SHOW_Presets)
			{
				if (key == OF_KEY_LEFT && !mod_CONTROL)
				{
					if (!bEditPalette) bEditPalette = true;
					if (last_Index_ColorPalette > 0) last_Index_ColorPalette--;
				}
				else if (key == OF_KEY_RIGHT && !mod_CONTROL)
				{
					if (!bEditPalette) bEditPalette = true;
					if (last_Index_ColorPalette < last_Index_ColorPalette.getMax()) last_Index_ColorPalette++;
				}
			}
		}

		//-----

		// presets

		if (SHOW_Presets && !SHOW_Theory && !SHOW_Range && !SHOW_ColourLovers && !SHOW_Quantizer && !SHOW_Library)
		{
			// get some presets by index random 
			if ((key == 'R' || key == OF_KEY_RETURN) && !mod_CONTROL)
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

		// theory

		if (SHOW_Theory && !SHOW_Range && !SHOW_ColourLovers && !SHOW_Quantizer && !SHOW_Library)
		{
			//if (!SHOW_Presets)
			{
				if (key == OF_KEY_UP && !mod_CONTROL)
				{
					if (last_Index_Theory_PickPalette == 0) last_Index_Theory_PickPalette = last_Index_Theory_PickPalette.getMax();
					else if (last_Index_Theory_PickPalette > 0) last_Index_Theory_PickPalette = last_Index_Theory_PickPalette.get() - 1;

					bLast_Index_Theory = true;
				}

				else if (key == OF_KEY_DOWN && !mod_CONTROL || (key == ' ' && !SHOW_Presets))
				{
					if (last_Index_Theory_PickPalette == last_Index_Theory_PickPalette.getMax())//cycle 
					{
						last_Index_Theory_PickPalette = last_Index_Theory_PickPalette.getMin();
					}
					else
					{
						last_Index_Theory_PickPalette = last_Index_Theory_PickPalette.get() + 1;
					}

					bLast_Index_Theory = true;
				}
			}

			if (!bEditPalette)
			{
				// randomize the base color
				if (key == OF_KEY_RETURN && !mod_CONTROL)
				{
					doRandomizeColorPicker();
				}

				// randomize a theory type
				if (key == OF_KEY_RETURN && mod_CONTROL)
				{
					// workflow
					if (bEditPalette) bEditPalette = false;
					//last_Index_Theory = (int)ofRandom(0, last_Index_Theory.getMax()+1);
					last_Index_Theory_PickPalette = (int)ofRandom(0, last_Index_Theory_PickPalette.getMax() + 1);
					bLast_Index_Theory = true;
					//refresh_FromPicked();
				}
			}
		}

		//----

		// range

		if (SHOW_Range && !SHOW_Theory && !SHOW_ColourLovers && !SHOW_Quantizer)// && !SHOW_Library)
		{
			//if (!SHOW_Presets)
			{
				if (key == OF_KEY_UP && !mod_CONTROL)
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

					bLast_Index_Range = true;
				}

				else if ((key == OF_KEY_DOWN && !mod_CONTROL) || (key == ' ' && !SHOW_Presets))
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

					bLast_Index_Range = true;
				}
			}

			//switch color c1/c2 selectors
			if (key == OF_KEY_BACKSPACE && !mod_CONTROL)
			{
				// workflow
				if (bEditPalette) bEditPalette = false;

				if (!bAuto_Color1_FromPicker_Range && !bAuto_Color2_FromPicker_Range) bAuto_Color1_FromPicker_Range = true;
				else if (bAuto_Color1_FromPicker_Range && !bAuto_Color2_FromPicker_Range) bAuto_Color2_FromPicker_Range = true;
				else if (!bAuto_Color1_FromPicker_Range && bAuto_Color2_FromPicker_Range) bAuto_Color1_FromPicker_Range = true;

				// workflow
				//get main color from range picker
				if (bAuto_Color1_FromPicker_Range) color_Picked.set(color_1_Range.get());
				else if (bAuto_Color2_FromPicker_Range) color_Picked.set(color_2_Range.get());
			}

			if (!bEditPalette)
			{
				// randomize color
				if ((key == 'r' || key == OF_KEY_RETURN) && !mod_CONTROL)
				{
					doRandomizeColorPicker();
				}

				//randomize a range type
				if ((key == 'r' || key == OF_KEY_RETURN) && mod_CONTROL)
				{
					last_Index_Range = (int)ofRandom(0, last_Index_Range.getMax() + 1);
					int i = last_Index_Range;
					types_Range[i] = true;//on button
				}
			}
		}

		//--

		// quantizer

		if (SHOW_Quantizer && !SHOW_Presets && key == ' ' && !mod_CONTROL)
			colorQuantizer.loadNext();

		//--

		// colour lover

		if (SHOW_ColourLovers && !SHOW_Presets && key == ' ' && !mod_CONTROL)
			colourLoversHelper.nextPalette();

		//-----

		// library 
		// Pantone

		if (SHOW_Library)
		{
			if (key == OF_KEY_RIGHT && !mod_CONTROL)
			{
				// index
				int n = last_ColorPicked_Lib;
				n++;
				n = ofClamp(n, 0, lib_TotalColors - 1);
				last_ColorPicked_Lib = n;
				last_Lib_Index = n;
				// color name
				if (n < palette_Lib_Names.size())
				{
					last_Lib_NameColor = palette_Lib_Names[n];
				}
				color_Picked = ofColor(palette_Lib_Cols[n]);
				//go to page
				int pag = n / lib_Page_NumColors;
				if (lib_Page_Index != pag) lib_Page_Index = pag;
			}

			else if (key == OF_KEY_LEFT && !mod_CONTROL)
			{
				// index
				int n = last_ColorPicked_Lib;
				n--;
				n = ofClamp(n, 0, lib_TotalColors - 1);
				last_ColorPicked_Lib = n;
				last_Lib_Index = n;
				// color name
				if (n < palette_Lib_Names.size())
				{
					last_Lib_NameColor = palette_Lib_Names[n];
				}
				color_Picked = ofColor(palette_Lib_Cols[n]);
				//go to page
				int pag = n / lib_Page_NumColors;
				if (lib_Page_Index != pag) lib_Page_Index = pag;
			}

			else if (key == OF_KEY_DOWN && !mod_CONTROL)
			{
				// index
				int n = last_ColorPicked_Lib;
				if (!lib_Responsive_ModeFit) n = n + lib_RowSize;
				else  n = n + lib_MaxColumns;
				n = ofClamp(n, 0, lib_TotalColors - 1);
				last_ColorPicked_Lib = n;
				last_Lib_Index = n;
				// color name
				if (n < palette_Lib_Names.size())
				{
					last_Lib_NameColor = palette_Lib_Names[n];
				}
				color_Picked = ofColor(palette_Lib_Cols[n]);
				// go to page
				int pag = n / lib_Page_NumColors;
				if (lib_Page_Index != pag) lib_Page_Index = pag;
			}

			else if (key == OF_KEY_UP && !mod_CONTROL)
			{
				// index
				int n = last_ColorPicked_Lib;
				if (!lib_Responsive_ModeFit) n = n - lib_RowSize;
				else  n = n - lib_MaxColumns;
				n = ofClamp(n, 0, lib_TotalColors - 1);
				last_ColorPicked_Lib = n;
				last_Lib_Index = n;
				// color name
				if (n < palette_Lib_Names.size())
				{
					last_Lib_NameColor = palette_Lib_Names[n];
				}
				color_Picked = ofColor(palette_Lib_Cols[n]);
				// go to page
				int pag = n / lib_Page_NumColors;
				if (lib_Page_Index != pag) lib_Page_Index = pag;
			}

			else if (key == OF_KEY_LEFT && mod_CONTROL)
			{
				lib_Page_Index--;
				if (lib_Page_Index < lib_Page_Index.getMin()) lib_Page_Index = lib_Page_Index.getMax();
				lib_Page_Index = ofClamp(lib_Page_Index, lib_Page_Index.getMin(), lib_Page_Index.getMax());

				// index
				int n = lib_Page_Index * lib_Page_NumColors;
				n = ofClamp(n, 0, lib_TotalColors - 1);
				last_ColorPicked_Lib = n;
				last_Lib_Index = n;
				// color name
				if (n < palette_Lib_Names.size())
				{
					last_Lib_NameColor = palette_Lib_Names[n];
				}
				color_Picked = ofColor(palette_Lib_Cols[n]);
			}

			else if (key == OF_KEY_RIGHT && mod_CONTROL)
			{
				lib_Page_Index++;
				if (lib_Page_Index > lib_Page_Index.getMax()) lib_Page_Index = lib_Page_Index.getMin();
				lib_Page_Index = ofClamp(lib_Page_Index, lib_Page_Index.getMin(), lib_Page_Index.getMax());

				// index
				int n = lib_Page_Index * lib_Page_NumColors;
				n = ofClamp(n, 0, lib_TotalColors - 1);
				last_ColorPicked_Lib = n;
				last_Lib_Index = n;
				// color name
				if (n < palette_Lib_Names.size())
				{
					last_Lib_NameColor = palette_Lib_Names[n];
				}
				color_Picked = ofColor(palette_Lib_Cols[n]);
			}

			else if (key == OF_KEY_RETURN && !mod_CONTROL)//random a color from the full library
			{
				//TODO:
				//crash
				//doRandomizeColorLibrary(false);

				doRandomizeColorLibrary();
				int n = ofRandom(0, lib_TotalColors);
				n = ofClamp(n, 0, lib_TotalColors - 1);
				last_ColorPicked_Lib = n;
				last_Lib_Index = n;
				// color name
				if (n < palette_Lib_Names.size()) last_Lib_NameColor = palette_Lib_Names[n];
				else last_Lib_NameColor = "ERROR EMPTY_COLOR";//error
				color_Picked = ofColor(palette_Lib_Cols[n]);
				// go to page
				int pag = n / lib_Page_NumColors;
				if (lib_Page_Index != pag) lib_Page_Index = pag;
			}

			else if (key == OF_KEY_RETURN && mod_CONTROL)//random a color from current page
			{
				//TODO:
				//crash
				//doRandomizeColorLibrary(true);

				int nmin = lib_Page_NumColors * lib_Page_Index;
				int nmax = lib_Page_NumColors * (lib_Page_Index + 1);
				nmax = ofClamp(nmax, 0, lib_TotalColors - 1);
				int n = ofRandom(nmin, nmax);
				last_ColorPicked_Lib = n;
				last_Lib_Index = n;
				// color name
				if (n < palette_Lib_Names.size()) last_Lib_NameColor = palette_Lib_Names[n];
				else last_Lib_NameColor = "ERROR EMPTY_COLOR";//error
				color_Picked = ofColor(palette_Lib_Cols[n]);
				// go to page
				int pag = n / lib_Page_NumColors;
				if (lib_Page_Index != pag) lib_Page_Index = pag;
			}
		}

		//----

		// panels

		if (mod_CONTROL)
		{
			if (key == OF_KEY_F1)//floating palette
			{
				SHOW_Palette = !SHOW_Palette;
			}
			else if (key == OF_KEY_F2)//presets
			{
				SHOW_Presets = !SHOW_Presets;
			}
			else if (key == OF_KEY_F3)//kit
			{
				SHOW_Kit = !SHOW_Kit;
			}
			else if (key == OF_KEY_F4)//palette editor
			{
				SHOW_Editor = !SHOW_Editor;
			}
			else if (key == OF_KEY_F5)//picker
			{
				SHOW_Picker = !SHOW_Picker;
			}
			else if (key == OF_KEY_F6)//library
			{
				SHOW_Library = !SHOW_Library;
			}
			else if (key == OF_KEY_F7)//gradient
			{
				//gradientEngine.SHOW_CurveEditor = !gradientEngine.SHOW_CurveEditor;
				SHOW_Gradient = !SHOW_Gradient;
			}
			else if (key == OF_KEY_F8)//demo
			{
				SHOW_Demos = !SHOW_Demos;
			}
			else if (key == OF_KEY_F9)//layouts
			{
				SHOW_Layouts = !SHOW_Layouts;
			}
			else if (key == OF_KEY_F10)//export
			{
				SHOW_LinkExport = !SHOW_LinkExport;
			}

			//else if (key == OF_KEY_F9)//mini
			//{
			//	SHOW_MINI_Preview = !SHOW_MINI_Preview;
			//}

			//-

//			else if (key == OF_KEY_F11 && !mod_CONTROL)//restore all
//			{
//				SHOW_Palette = true;
//				SHOW_Editor = true;
//				SHOW_Presets = true;
//				SHOW_Kit = true;
//				SHOW_Picker = true;
//				SHOW_Library = false;
//				SHOW_Demos = false;
//				SHOW_MINI_Preview = false;
//				gradientEngine.SHOW_Gradient = false;
//#ifdef MODE_BACKGROUND
//				SHOW_BackGround = true;
//#endif
//				SHOW_Theory = false;
//				SHOW_Range = false;
//				SHOW_ColourLovers = false;
//				SHOW_Quantizer = false;
//
//				SHOW_Advanced = false;
//			}

			else if (key == OF_KEY_F11)//
			{
				SHOW_Advanced = !SHOW_Advanced;
			}

			//#ifdef MODE_BACKGROUND
			//		else if (key == OF_KEY_F5)//bg
			//		{
			//			SHOW_BackGround = !SHOW_BackGround;
			//		}
			//#endif
		}

		//--

		// layouts

		else //!mod_CONTROL
		{
			switch (key)
			{
			case OF_KEY_F1: appLayoutIndex = APP_DEFAULT; break;
			case OF_KEY_F2: appLayoutIndex = APP_PRESETS; break;
			case OF_KEY_F3: appLayoutIndex = APP_ENGINES; break;
			case OF_KEY_F4: appLayoutIndex = APP_MINIMAL; break;
			case OF_KEY_F5: appLayoutIndex = APP_USER; break;
			default: break;
			}

			if (key == OF_KEY_F9)//layouts
			{
				SHOW_Layouts = !SHOW_Layouts;
			}

			if (key == OF_KEY_F10)//panels
			{
				SHOW_Panels = !SHOW_Panels;
			}
			else if (key == OF_KEY_F11)//
			{
				SHOW_Advanced = !SHOW_Advanced;
			}
		}

		//----

		//TODO:
		//// undo engine
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
	}
}

//--------------------------------------------------------------
void ofxColorManager::keyReleased(ofKeyEventArgs &eventArgs)
{
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

	//--

	// DEMO

	//TODO:
	//mouseLockedByGui_PRE not working..

	if (!mouseLockedByGui)
	{
		//if (/*SHOW_About &&*/ !mouseLockedByGui_PRE)
		if (DEMO1_Enable)
		{
			// workflow
			//disable mouse on auto mode
			//if (!DEMO1_Auto)
			{
				//second mouse button cleans DEMO
				if (button == 2) DEMO1_Bubbles.clear();
				else DEMO1_Bubbles.start();//trig DEMO start
			}
		}

		if (DEMO2_Enable || SHOW_About) DEMO2_Spheres.start();
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
	ofLogVerbose(__FUNCTION__) << "scrollX: " << scrollX << "  scrollY: " << scrollY;
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
void ofxColorManager::refresh_FilesSorting(std::string name)
{
	ofLogNotice(__FUNCTION__) << "----------------- SORTING FILES-----------------";
	ofLogNotice(__FUNCTION__) << "Search Iindex for file:";
	ofLogNotice(__FUNCTION__) << name;

	// workflow

	//go to ne index after adding a new preset
	ofLogNotice(__FUNCTION__) << "Rearrenge index file";
	//std::string path = path_Presets + textInput_New + ".json";

	//locate new position of old (saved) preset
	int ii = -1;

	for (int i = 0; i < files.size() && ii == -1; i++)
	{
		std::string _name = files_Names[i];
		//std::string _name = files[i].getBaseName();

		ofLogNotice(__FUNCTION__) << i << " " << files_Names[i];

		if (_name == name)//found file index of the preset
		{
			ii = i;
			ofLogNotice(__FUNCTION__) << "Found " << _name << " at index " << i << " !";

			//continue;
		}
	}

	//reload the same preset at newer index
	if (ii != -1)//found the name
	{
		//preset_Load(textInput_New);;

		last_Index_Preset = ii;
	}
	else//not found
	{
		ofLogError(__FUNCTION__) << name << " filename not Found !";

		last_Index_Preset = 0;
		//load first preset by default if not found the same name
	}

	//load
	PRESET_Name = files_Names[last_Index_Preset];
	preset_Load(PRESET_Name);
}

//--------------------------------------------------------------
void ofxColorManager::refresh_Paths()
{
	// initialize
	//path_Kit = "USER KIT";
	path_Global = "ofxColorManager/";
	path_Kits = "kits/";
	path_Presets = path_Global + path_Kits + path_Kit;
	//path_Palettes = path_Global + path_Kits + "palettes/"; // not used
	ofxSurfingHelpers::CheckFolder(path_Global + path_Kits);

	//std::string _path = path_Presets;
	ofxSurfingHelpers::CheckFolder(path_Presets);
	ofDirectory dataDirectory(ofToDataPath(path_Presets, true));
	ofxSurfingHelpers::CheckFolder(path_Presets);
	ofLogNotice(__FUNCTION__) << path_Presets;

	// export for live reload preset from client app
	//path_Name_ExportColor = "ofxColorManager";
}

//--------------------------------------------------------------
bool ofxColorManager::preset_RefreshFiles(bool bForeLoadFirst)
{
	ofLogNotice(__FUNCTION__) << "----------------- REFRESH FILES-----------------";

	bool b = false;//returns true if amount files changed.
	//bc sometimes we couild b overwritting a file

	//-

	refresh_Paths();

	//-

	int _sz = files.size();

	// clear files and filenames vectors
	files.clear();
	files_Names.clear();

	// load all folder files in one call

	ofxSurfingHelpers::CheckFolder(path_Presets);
	ofDirectory dataDirectory(ofToDataPath(path_Presets, true));
	ofxSurfingHelpers::CheckFolder(path_Presets);
	ofLogNotice(__FUNCTION__) << path_Presets;

	files = dataDirectory.getFiles();

	for (size_t i = 0; i < files.size(); i++)
	{
		files_Names.push_back(files[i].getBaseName());

		ofLogNotice(__FUNCTION__) << i << " " << files_Names[i];
	}

	last_Index_Preset.setMax(files.size() - 1);
	if (last_Index_Preset > files.size() - 1) last_Index_Preset = -1;


	//TODO:
	PRESET_Temp.setPathKit(path_Kit + "/");

	//-

	// workflow

	//TODO:
	//void to go to 1st presets...
	if (bForeLoadFirst)
	{
		// 1. load same position preset
		// if preset is deleted goes to nextone..
		// should check names because sorting changes..
		if (files_Names.size() > 0)
		{
			if (last_Index_Preset > files_Names.size() - 1) last_Index_Preset = files_Names.size() - 1;

			//else if (last_Index_Preset > files_Names.size() - 1)

			PRESET_Name = files_Names[last_Index_Preset];
			preset_Load(PRESET_Name);
		}
		else
		{
			ofLogError(__FUNCTION__) << "FILE PRESET NOT FOUND!";
		}
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

	//-

	if (_sz != files.size()) b = true;
	return b;
}

//--------------------------------------------------------------
void ofxColorManager::preset_Load(std::string p, bool absolutePath)
{
	ofLogNotice(__FUNCTION__) << "----------------- PRESET LOAD -----------------";
	ofLogNotice(__FUNCTION__) << "PRESET_Name: " << PRESET_Name;

	//--

	ENABLE_CALLBACKS_Engines = false;

	//--

	PRESET_Temp.setLinkName(p);

#ifndef USE_SIMPLE_PRESET_PALETTE	
	PRESET_Temp.setNameCurve_TARGET(PRESET_Name_Gradient);
#endif

	//--

	last_Index_Type = 0;

	//--

	if (!absolutePath) p = p + ".json";

	//--

	// 1. load palette preset (target will be the above pointers) //TODO: should (late?) improve this..
	bool b = PRESET_Temp.preset_Load(p, absolutePath);
	if (!b)
	{
		ofLogError(__FUNCTION__) << "Preset file " << p << " not found! ";
		return;
	}
	// palette and name will we auto updated here bc are pointer-back-referenced
	// preset loaded succesfully here!

	//--

	// set name to text input

	if (name_TARGET != nullptr) textInput_New = name_TARGET[0];
	textInput_New = PRESET_Name;
	//textInput_New = p;
	//textInput_New = PRESET_Temp.name;

	//--

	//TODO:
	// include curve gradient into the preset system ?

	//--

	// populate colors to palette

	palette_Clear();
	vector<ofColor> _p = PRESET_Temp.getPalette();
	for (int i = 0; i < _p.size(); i++)
	{
		ofLogNotice(__FUNCTION__) << "Color #" << ofToString(i) << " " << ofToString(_p[i]);
		palette_AddColor(_p[i]);
	}

	//--

	// workflow

	// new preset
	if (MODE_NewPreset) MODE_NewPreset = false;
	MODE_ReadyToUpdate = true;

	// load first color
	if (palette.size() > 0) color_Picked = ofFloatColor(palette[0]);

	//--

	build_Palette_Preset();

	// export
	if (bAutoExportPreset)
	{
		bExportFlag = true;
	}

	//--

	ENABLE_CALLBACKS_Engines = true;

	//--

	// engines

	// workflow
	// untick engines toggles
	bLast_Index_Theory = false;
	bLast_Index_Range = false;

	// workflow
	bFlag_refresh_EnginesFromPalette = true;

	// workflow
	if (DEMO1_Enable || SHOW_About) DEMO1_Bubbles.reStart();
	if (DEMO2_Enable || SHOW_About) DEMO2_Spheres.start();
}

//--------------------------------------------------------------
void ofxColorManager::preset_Save(std::string p, bool absolutePath)
{
	ofLogNotice(__FUNCTION__) << "----------------- PRESET SAVE -----------------";

	//NOTE: preset filename must to not include any extra '.' char
	//clean all extra '.' chars
	ofStringReplace(p, ".", "");

	ofLogNotice(__FUNCTION__) << p;

	PRESET_Temp.setLinkName(p);
#ifndef USE_SIMPLE_PRESET_PALETTE	
	PRESET_Temp.setNameCurve_TARGET(PRESET_Name_Gradient);
#endif
	PRESET_Temp.setBackgroundColor(color_BackGround.get());//no pointer now
	PRESET_Temp.preset_Save(p, false);
}

//----

//--------------------------------------------------------------
void ofxColorManager::refresh_FromPicked()
{
	ofLogNotice(__FUNCTION__);

	color_TheoryBase.set(color_Picked.get());
}

//--------------------------------------------------------------
void ofxColorManager::refresh_ColorPickedFromEngines()
{
	ofLogNotice(__FUNCTION__);

	//TODO:
	ENABLE_CALLBACKS_Engines = false;
	if (palette.size() > 0)
	{
		color_Picked = palette[0];
	}
	ENABLE_CALLBACKS_Engines = true;
}

//--------------------------------------------------------------
void ofxColorManager::refresh_EnginesFromPalette()
{
	if (palette.size() > 0)
	{
		ofLogNotice(__FUNCTION__);

		// theory

		//if (SHOW_Theory.get())
		{
			if (bAuto_Theory_FromPicker)
			{
				color_TheoryBase = palette[0];//first
				//color_TheoryBase = palette[last_Index_ColorPalette];//last
			}
		}

		//--

		// range

		//if (SHOW_Range.get())
		{
			color_1_Range.setWithoutEventNotifications(palette[0]);//first color
			color_2_Range.setWithoutEventNotifications(palette[palette.size() - 1]);//last color

			if (autoGenerate_Range) generate_Range(color_1_Range.get(), color_2_Range.get());
		}
	}
}

//--------------------------------------------------------------
void ofxColorManager::refresh_Pick_ToHSB()
{
	ofLogNotice(__FUNCTION__);

	//--

	////TODO:
	////using ImGui helper
	//ENABLE_CALLBACKS_Pickers = false;
	//float out_h, out_s, out_v;
	//ofColor c = color_Picked.get();

	////ImGui::ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v)
	//ImGui::ColorConvertRGBtoHSV(c.r, c.g, c.b, out_h, out_s, out_v);

	//color_HUE = out_h * 255.0f;
	//color_SAT = out_s * 255.0f;
	//color_BRG = out_v * 255.0f;
	//ENABLE_CALLBACKS_Pickers = true;

	//--

	//TODO:
	//using OF
	//sometimes differs a bit... Must round float/int..
	color_HUE = int(255.f * color_Picked.get().getHue());
	color_SAT = int(255.f * color_Picked.get().getSaturation());
	color_BRG = int(255.f * color_Picked.get().getBrightness());

	//color_HUE = int(255.f * color_Picked.get().getHue() + 0.5f);
	//color_SAT = int(255.f * color_Picked.get().getSaturation() + 0.5f);
	//color_BRG = int(255.f * color_Picked.get().getBrightness() + 0.5f);

	//https://forum.openframeworks.cc/t/convert-rgb-colour/1350/3
}

//--------------------------------------------------------------
void ofxColorManager::refresh_Pick_ToEngines()
{
	ofLogNotice(__FUNCTION__) << "Spread color_Picked : " << ofToString(color_Picked);

	//--

	if (bEditPalette)
	{
		// autosave edited color
		if (last_Index_ColorPalette >= 0 &&
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

		// export
		if (bAutoExportPreset)
		{
			ofLogNotice(__FUNCTION__) << "Auto EXPORT";
			bExportFlag = true;
		}

		return;
	}
	// we skip enignes on edit mode!

	//----

	ENABLE_CALLBACKS_Pickers = false; //disable callbacks
	{
		//-

		// theory 

		// color base

		if (SHOW_Theory)
		{
			if (bAuto_Theory_FromPicker)
			{
				color_TheoryBase = color_Picked.get();
			}
		}

		//-

		// range

		if (SHOW_Range)
		{
			bool bDoGen = false;

			// 2. range color1
			if (bAuto_Color1_FromPicker_Range)
			{
				color_1_Range = color_Picked.get();
				bDoGen = true;
			}

			//-

			// 3. range color2
			else if (bAuto_Color2_FromPicker_Range)
			{
				color_2_Range = color_Picked.get();
				bDoGen = true;
			}

			if (autoGenerate_Range && bDoGen) generate_Range(color_1_Range.get(), color_2_Range.get());
		}

		//--

		//TODO:
		// mirror clicked/picked colors
		//color_Clicked2.set(color_Picked.get());
	}
	ENABLE_CALLBACKS_Pickers = true;

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
void ofxColorManager::setLinkName(std::string &s)
{
	ofLogNotice(__FUNCTION__) << s;
	name_TARGET = &s;
}

//--------------------------------------------------------------
void ofxColorManager::setLinkColorPick(ofColor &c)
{
	ofLogNotice(__FUNCTION__) << ofToString(c);
	color_TARGET = &c;
}

//--------------------------------------------------------------
void ofxColorManager::setLinkColorBg(ofColor &c)
{
	ofLogNotice(__FUNCTION__) << ofToString(c);
	colorBg_TARGET = &c;
}

//--------------------------------------------------------------
void ofxColorManager::setLinkPalette(vector<ofColor> &p)
{
	ofLogNotice(__FUNCTION__) << ofToString(p);
	palette_TARGET = &p;
}

//--

//--------------------------------------------------------------
void ofxColorManager::setPickControl(float control)
{
	ofLogNotice(__FUNCTION__) << control;

	gradientEngine.pickIn = control;
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

	if (SHOW_Gradient)
		//if (gradientEngine.SHOW_CurveEditor)
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
void ofxColorManager::setupRange()
{
	color_1_Range.set("Color 1", ofColor::red, ofColor(0), ofColor(1));
	color_2_Range.set("Color 2", ofColor::blue, ofColor(0), ofColor(1));

	// 12 types
	names_Types_Range = { "RGB", "HSL", "HSV ", "HSB", "LUV ", "LAB", "HLAB", "LCH", "CMY", "CMYK", "YXY", "XYZ" };

	autoGenerate_Range.set("Auto Generate", true);
	bAuto_Color1_FromPicker_Range.set("Auto Pick C1", true);
	bAuto_Color2_FromPicker_Range.set("Auto Pick C2", false);
	bGetPalette_From_Range.set("To User Palette", false);

	numColors_Range.set("Amount Colors ", (int)MAX_PALETTE_COLORS, 3, (int)MAX_PALETTE_COLORS);
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
		ofLogNotice(__FUNCTION__) << "----------------- GENERATE RANGE -----------------";

		ofLogNotice(__FUNCTION__) << "col1:" << ofToString(col1) << " > col2:" << ofToString(col2);

		int _max = 400;
		int _step;
		int _div;

		ofLogNotice(__FUNCTION__) << numColors_Range.getName() << " : " << numColors_Range.get();

		_div = int((numColors_Range.get() - 1));
		if (_div > 0)
		{
			_step = _max / _div;

			ofLogVerbose(__FUNCTION__) << "_div : " << _div << " / " << "_step: " << _step;

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

						ofLogVerbose(__FUNCTION__) << i << " : " << j << " > " << color;

						palette_Range.push_back(color);
					}
				}

				ofLogVerbose(__FUNCTION__) << "palette_Range size: " << palette_Range.size();
			}
			else
			{
				ofLogWarning(__FUNCTION__) << "skip bc params wrong defined yet..";
			}
		}
		else
		{
			ofLogWarning(__FUNCTION__) << "skip bc params wrong defined yet..";
		}
	}
}

//----

//--------------------------------------------------------------
void ofxColorManager::setImage()
{
	ofLogNotice(__FUNCTION__) << "----------------- SET IMAGE -----------------";

	//Open the Open File Dialog
	std::string str = "Select image file ";
	str += "to extract dominant colors ";
	str += "and create Palette\n";

	ofFileDialogResult openFileResult = ofSystemLoadDialog(str, false);

	//Check if the user opened a file
	if (openFileResult.bSuccess)
	{
		string path = openFileResult.getPath();
		colorQuantizer.addImage(path);

		ofLogNotice(__FUNCTION__) << "Image file to extract: " << path;

		// workflow
		SHOW_Quantizer = true;
	}
	else
	{
		ofLogNotice(__FUNCTION__) << "User hit cancel";
	}
}

//--------------------------------------------------------------
void ofxColorManager::setPathPresetWatch()
{
	ofLogNotice(__FUNCTION__) << "----------------- SET PRESET PATH -----------------";

	//Open the Open File Dialog
	std::string str = "Select the preset export path.\n";
	str += "Typically you should set something like:\n";
	str += "'OF_ADDON/ofxColorClient/3-example_AdvancedScenes/bin/data'\n";
	str += "Where YOUR_OF_CLIENT_APP will be watching for changing on the preset file.\n";
	str += "And to reload. This is the alternative LINK-MODE to the TCP based one.\n";
	ofFileDialogResult openFileResult = ofSystemLoadDialog(str, true);

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
		bExportPreset_DefaultPath = true;// set back default OF_APP/bin/data
	}

	bOpen = false;
}

//--------------------------------------------------------------
void ofxColorManager::exportKit()
{
	ofLogNotice(__FUNCTION__) << "----------------- EXPORT KIT -----------------";

	// refresh
	preset_RefreshFiles(false);
	std::string str = "Select the Export Path for the Kit preset files.\n";
	str += "Typically you should set something like:\n";
	str += "YOUR_OF_CLIENT_APP/bin/data/ofxColorManager/kits/presets\n";
	str += "Where your client app will scan the folder\n";
	str += "and list the preset files.\n";

	ofFileDialogResult openFileResult = ofSystemLoadDialog(str, true);

	std::string path_export;

	//Check if the user opened a file
	if (openFileResult.bSuccess)
	{
		path_export = openFileResult.getPath();
		//path_export += "\\";// windows

		ofLogNotice(__FUNCTION__) << "User selected a path: " << path_export;

		//save all files
		for (size_t i = 0; i < files.size(); i++)
		{
			const filesystem::path path_From = files[i].getAbsolutePath();
			const filesystem::path path_To = path_export + "/" + files[i].getFileName();
			ofLogNotice(__FUNCTION__) << "Copy " << path_From << " > " << path_To;

			ofFile _file;
			bool b = _file.copyFromTo(path_From, path_To, false, true);
			if (b) ofLogNotice(__FUNCTION__) << "Saved";
			else ofLogError(__FUNCTION__) << "Saving Failed!";
		}

		//TODO:
		//open system file browser
		//ofxNative
	}
	else
	{
		ofLogNotice(__FUNCTION__) << "User hit cancel";
	}
}

//--------------------------------------------------------------
void ofxColorManager::exportPalette()
{
	ofLogNotice(__FUNCTION__) << "----------------- EXPORT PALETTE -----------------";

	ofJson j;

	// check if requires create folder
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

	// a. FILE WATCH

	if (bExportByFile)
	{
#ifdef USE_SIMPLE_PRESET_PALETTE
		// save palette colors only (without background neither gradient, bg or name)
		//using ofxSerializer
		j = palette;
		std::string _path = path_FileExport + "_Palette.json";
		ofLogNotice(__FUNCTION__) << "\n" << ofToString(j);

		ofSavePrettyJson(_path, j);
#endif
	}

	//--

	// b. TCP-LINK

	if (bExportByTCP)
	{
		j = palette;
		//TODO: 
		//add bundle json too
		//j = PRESET_Temp.getJson();
		//j = load(path_FileExport + "_Bundled"
		std::stringstream ss;
		ss << j;
		//ss << "[/TCP]";

		ofLogNotice(__FUNCTION__) << "LINK: " + ss.str();

#ifdef LINK_TCP_MASTER_CLIENT_OF
		TCP_Sender.sendToAll(ss.str());//send to all clients
		//for (int i = 0; i < TCP.getLastID(); i++)//many clients can be connected 
		//{
		//	//TCP.send(i, ss.str());
		//}
#endif

#ifdef LINK_TCP_MASTER_CLIENT_KU
		if (TCP_Sender.connected()) {
			TCP_Sender.clearBuffer();
			TCP_Sender.putString(ss.str());
			TCP_Sender.send();
	}
#endif

		storeText.push_back(ss.str() + "\n");
}

	//--
}

//--------------------------------------------------------------
void ofxColorManager::savePresetFile()
{
	ofLogNotice(__FUNCTION__) << "----------------- SAVE PALETTE -----------------";

	//Open the Open File Dialog
	std::string str = "Select the Preset Save path.\n";
	str += "Typically you use this saving type to BACKUP file.\n";
	//str += "'OF_ADDON/ofxColorClient/3-example_AdvancedScenes/bin/data'\n";
	//str += "Where YOUR_OF_CLIENT_APP will be watching for changing on the preset file.\n";
	//str += "And to reload. This is the alternative LINK-MODE to the TCP based one.\n";
	ofFileDialogResult openFileResult = ofSystemLoadDialog(str, true);

	std::string _path;

	//Check if the user opened a file
	if (openFileResult.bSuccess)
	{
		_path = openFileResult.getPath();
		ofStringReplace(textInput_New, ".", "");
		_path += "\\";// windows
		_path += textInput_New;

		ofLogNotice(__FUNCTION__) << ": " << _path;

		PRESET_Temp.preset_Save(_path, true);
	}
	else
	{
		ofLogError(__FUNCTION__) << "File '" << _path << "' NOT FOUND";
	}
}

//--------------------------------------------------------------
void ofxColorManager::loadPresetFile()
{
	ofLogNotice(__FUNCTION__) << "----------------- LOAD PALETTE -----------------";

	std::string str = "Select the Preset Load path.\n";
	str += "Typically you use this loading type to load a BACKUP file.\n";
	//str += "'OF_ADDON/ofxColorClient/3-example_AdvancedScenes/bin/data'\n";
	//str += "Where YOUR_OF_CLIENT_APP will be watching for changing on the preset file.\n";
	//str += "And to reload. This is the alternative LINK-MODE to the TCP based one.\n";

	//Open the Open File Dialog
	ofFileDialogResult openFileResult = ofSystemLoadDialog(str, false);

	std::string _path;
	std::string _name;

	//Check if the user opened a file
	if (openFileResult.bSuccess)
	{
		_path = openFileResult.getPath();
		_name = openFileResult.getName();

		auto _nameExt = ofSplitString(_name, ".");//remove ext
		if (_nameExt.size() == 2) _name = _nameExt[0];
		PRESET_Name = _name;
		if (name_TARGET != nullptr) name_TARGET[0] = _name;
		PRESET_Temp.setLinkName(_name);

		ofLogNotice(__FUNCTION__) << ": " << _path << " | " << _name;

		preset_Load(_path, true);

		// new preset
		// workflow
		//if (!MODE_NewPreset) MODE_NewPreset = true;
		MODE_ReadyToUpdate = true;
		textInput_New = _name;
	}
	else
	{
		ofLogError(__FUNCTION__) << "File '" << _path << "' NOT FOUND";
	}
}

//--

#ifdef LINK_TCP_MASTER_CLIENT_OF
//--------------------------------------------------------------
void ofxColorManager::setupLink() {

	// setup the server to listen on port
	ofxTCPSettings settings(port);

	// set other options
	settings.blocking = false;
	//settings.reuse = true;
	//settings.messageDelimiter = "\n";

	TCP_Sender.setup(settings);

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
	if (now - lastCheckLink >= 4000)
	{
		for (int i = 0; i < TCP_Sender.getLastID(); i++)
		{
			if (!TCP_Sender.isClientConnected(i)) continue;

			ofLogVerbose(__FUNCTION__) << "Connected on port: " + ofToString(TCP_Sender.getClientPort(i));

			//TCP.send(i, "hello client - you are connected on port - " + ofToString(TCP.getClientPort(i)));
		}
		lastCheckLink = now;
	}
}

//--------------------------------------------------------------
void ofxColorManager::draw_Link(int x, int y)
{
	std::string ss = "TCP SERVER\n\port: " + ofToString(TCP_Sender.getPort()) + "\n";

	// for each connected client lets get the data being sent and lets print it to the screen
	for (unsigned int i = 0; i < (unsigned int)TCP_Sender.getLastID(); i++) {

		if (!TCP_Sender.isClientConnected(i))continue;

		// get the ip and port of the client
		std::string port = ofToString(TCP_Sender.getClientPort(i));
		std::string ip = TCP_Sender.getClientIP(i);
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
			tmp = TCP_Sender.receive(i);
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

	ofxSurfingHelpers::drawTextBoxed(font, ss, x, y);
}
#endif

//--

#ifdef LINK_TCP_MASTER_CLIENT_KU
//--------------------------------------------------------------
void ofxColorManager::setupLink() {

	TCP_Sender.close();
	TCP_Sender.setup(host, port, 1024);

	// optionally set the delimiter to something else.  
	//The delimiter in the client and the server have to be the same, default being [/TCP]
	//TCP.setMessageDelimiter("\n");
	//lastCheckLink = 0;
}

//--------------------------------------------------------------
void ofxColorManager::updateLink() {
	//TODO:
	//add reconnect engine

	//// for each client lets send them a message letting them know what port they are connected on
	//// we throttle the message sending frequency to once every 100ms
	//uint64_t now = ofGetElapsedTimeMillis();
	//if (now - lastCheckLink >= 4000)
	//{
	//	for (int i = 0; i < TCP_Sender.getLastID(); i++)
	//	{
	//		if (!TCP_Sender.isClientConnected(i)) continue;
	//		ofLogVerbose(__FUNCTION__) << "Connected on port: " + ofToString(TCP_Sender.getClientPort(i));
	//		//TCP.send(i, "hello client - you are connected on port - " + ofToString(TCP.getClientPort(i)));
	//	}
	//	lastCheckLink = now;
	//}
}

//--------------------------------------------------------------
void ofxColorManager::draw_Link(int x, int y)
{
	//std::string ss = "TCP SERVER\n\port: " + ofToString(TCP_Sender.getPort()) + "\n";
	//// for each connected client lets get the data being sent and lets print it to the screen
	//for (unsigned int i = 0; i < (unsigned int)TCP_Sender.getLastID(); i++) {
	//	if (!TCP_Sender.isClientConnected(i))continue;
	//	// get the ip and port of the client
	//	std::string port = ofToString(TCP_Sender.getClientPort(i));
	//	std::string ip = TCP_Sender.getClientIP(i);
	//	std::string info = "client " + ofToString(i) + " " + ip + ":" + port;
	//	// if we don't have a string allocated yet
	//	// lets create one
	//	if (i >= storeText.size())
	//	{
	//		storeText.push_back(std::string());
	//	}
	//	// receive all the available messages, separated by \n
	//	// and keep only the last one
	//	std::string str;
	//	std::string tmp;
	//	do
	//	{
	//		str = tmp;
	//		tmp = TCP_Sender.receive(i);
	//	} while (tmp != "");
	//	// if there was a message set it to the corresponding client
	//	if (str.length() > 0)
	//	{
	//		storeText[i] = str;
	//	}
	//	//// draw the info text and the received text bellow it
	//	//ofDrawBitmapString(info, xPos, yPos);
	//	//ofDrawBitmapString(storeText[i], 25, yPos + 20);
	//	ss += info;
	//	ss += storeText[i];
	//}
	//ofxSurfingHelpers::drawTextBoxed(font, ss, x, y);
}
#endif

//--------------------------------------------------------------
void ofxColorManager::doRandomizeColorPicker()
{
	float _hue;
	float _sat;
	float _brg;

	if (bColor_rHUE) color_HUE_0 = color_HUE;
	if (bColor_rSAT) color_SAT_0 = color_SAT;
	if (bColor_rBRG) color_BRG_0 = color_BRG;

	int maxTimes = 5;//limit retries to avoid hanging..

	if (bColor_rHUE)
	{
		bool rDone = false;
		int times = 0;
		while (!rDone && times < maxTimes)
		{
			_hue = ofRandom(color_HUE_0 - 128.f * color_HUE_Power, color_HUE_0 + 128.f * color_HUE_Power);
			if (color_HUE_0 != _hue) color_HUE = ofClamp(_hue, 0, 255);
			if (color_HUE != color_HUE_0) rDone = true;
			times++;
		}
	}

	if (bColor_rSAT)
	{
		bool rDone = false;
		int times = 0;
		while (!rDone && times < maxTimes)
		{
			_sat = ofRandom(color_SAT_0 - 128.f * color_SAT_Power, color_SAT_0 + 128.f * color_SAT_Power);
			if (color_SAT_0 != _sat) color_SAT = ofClamp(_sat, 0, 255);
			if (color_SAT != color_SAT_0) rDone = true;
			times++;
		}
	}

	if (bColor_rBRG)
	{
		bool rDone = false;
		int times = 0;
		while (!rDone && times < maxTimes)
		{
			_brg = ofRandom(color_BRG_0 - 128.f * color_BRG_Power, color_BRG_0 + 128.f * color_BRG_Power);
			if (color_BRG_0 != _brg) color_BRG = ofClamp(_brg, 0, 255);
			if (color_BRG != color_BRG_0) rDone = true;
			times++;
		}
	}
}

//--------------------------------------------------------------
void ofxColorManager::buildHelpInfo()
{

	helpInfo = "";
	helpInfo += "      HELP KEY COMMANDS\n\n";
	//helpInfo += "           HELP\n\n";
	//helpInfo += "\n";
	helpInfo += "\n";

	helpInfo += "H            HELP\n";
	helpInfo += "G            GUI\n";
	helpInfo += "F            FULL SCREEN\n";
	if (SHOW_Advanced) helpInfo += "L            UNLOCK DOCK\n";
	if (SHOW_Advanced) helpInfo += "SHIFT        DOCK EDIT\n";
	helpInfo += "A            ABOUT\n";
	helpInfo += "\n";
	helpInfo += "\n";

	helpInfo += "         LAYOUTS\n";
	helpInfo += "\n";
	helpInfo += "TAB          >>\n";
	helpInfo += "F1           DEFAULT\n";
	helpInfo += "F2           PRESETS\n";
	helpInfo += "F3           ENGINES\n";
	helpInfo += "F4           MINIMAL\n";
	helpInfo += "F5           USER\n";
	helpInfo += "\n";
	helpInfo += "F9           LAYOUTS\n";
	helpInfo += "F10          PANELS\n";
	helpInfo += "F11          ADVANCED\n";

	helpInfo += "\n";
	helpInfo += "\n";

	helpInfo += "         ACTIONS\n";
	helpInfo += "\n";
	helpInfo += "SPACE        >> PRESET\n";
	helpInfo += "Left|Right   <> PRESET\n";
	helpInfo += "+Ctrl        <> SHIFT COLORS\n";
	//helpInfo += "\n";

	helpInfo += "-|+          AMOUNT COLORS\n";
	//if (SHOW_Advanced) helpInfo += "\n";
	helpInfo += "RETURN       RANDOM\n";
	//if (SHOW_Advanced) helpInfo += "+Ctrl        \n";
	//if (SHOW_Advanced) helpInfo += "\n";
	helpInfo += "BACKSPACE    AUX\n";
	//if (SHOW_Advanced) helpInfo += "  +Ctrl      \n";
	if (SHOW_Advanced) helpInfo += "\n";
	if (SHOW_Advanced) {
		//helpInfo += "\n";
		helpInfo += "         LIBRARY\n";
		helpInfo += "Arrows       COLORS\n";
		helpInfo += "+Ctrl        PAGE\n";
	}
	helpInfo += "\n\n";

	helpInfo += "         ENGINES\n";
	helpInfo += "\n";
	helpInfo += "TAB+Ctrl     >>\n";
	helpInfo += "             THEORY\n";
	helpInfo += "             RANGE\n";
	helpInfo += "             LOVERS\n";
	helpInfo += "             PICTURE\n";
	//helpInfo += "\n";
	helpInfo += "Down|Up      SUB-TYPES\n";
	helpInfo += "\n\n";

	if (SHOW_Advanced)
	{
		helpInfo += "         PANELS\n";
		helpInfo += "\n";
		helpInfo += "Ctrl+\n";
		helpInfo += "F1           PALETTE\n";
		helpInfo += "F2           PRESETS\n";
		helpInfo += "F3           KIT\n";
		helpInfo += "F4           EDITOR\n";
		helpInfo += "F5           PICKER\n";
		helpInfo += "F6           LIBRARY\n";
		helpInfo += "F7           GRADIENT\n";
		helpInfo += "F8           DEMO\n";
		helpInfo += "F9           LAYOUTS\n";
		helpInfo += "F10          EXPORT\n";
		helpInfo += "\n\n";
	}
	else
	{
		//helpInfo += "More keys on ADVANCED MODE\n";
	}

	//helpInfo += "TEST\n";		 
	//helpInfo += "D              DEMO SCENE\n";
	//helpInfo += "T              GRADIENT TEST\n";
	//helpInfo += "m              MINI PREVIEW\n";
	//helpInfo += "M              DEBUG MOUSE RULER\n";
}

//--------------------------------------------------------------
void ofxColorManager::setupDemos()
{
	// DEMO Bubbles

	DEMO1_Bubbles.setup();
	DEMO1_Bubbles.setPalette(palette);
	//DEMO1_Bubbles.setEnableMouseCamera(true);

	//--

	// DEMO Spheres

	DEMO2_Spheres.setup();
	DEMO2_Spheres.setPalette(palette);
	//DEMO1_Bubbles.setEnableMouseCamera(true);

	//-

	// DEMO Svg

	DEMO3_Svg.setLinkPalette(palette);
	DEMO3_Svg.setVisible(false);
#ifdef USE_SVG_MASK
	//#ifndef USE_SVG_MASK
	DEMO3_Svg.enable_Mask = false;
#endif	
}

//--------------------------------------------------------------
void ofxColorManager::updateDemos()
{
	// DEMO Bubbles
	if (DEMO1_Enable || SHOW_About)
	{
		DEMO1_Bubbles.update();
		if (DEMO1_Auto) {}
		if ((ofGetElapsedTimeMillis() - Demo_Timer) > MAX(Demo_Timer_Max * (1 - DEMO1_Timer), 10))
		{
			Demo_Timer = ofGetElapsedTimeMillis();

			DEMO1_Bubbles.start();
		}
	}

	// DEMO Spheres
	if (DEMO2_Enable || SHOW_About) DEMO2_Spheres.update();
}

//--------------------------------------------------------------
void ofxColorManager::draw_Demos()
{
	//glm::vec3 offset = ofGetCurrentViewport().getCenter() - rectangle_Central_MAX.getCenter();

	// DEMO Svg
	DEMO3_Svg.draw();//is centered to free space

	//-

	// DEMO Bubbles
	if (DEMO1_Enable)
	{
		DEMO1_Bubbles.draw(rectangle_Central_MAX, DEMO1_Alpha);
	}

	//-

	// DEMO Spheres
	if (DEMO2_Enable)
	{
		DEMO2_Spheres.draw(rectangle_Central_MAX);
	}
}

//--------------------------------------------------------------
void ofxColorManager::refresh_DemoFboAbout()
{
	ofLogNotice(__FUNCTION__);

	bool b = ofGetUsingArbTex();
	ofDisableArbTex();

	ofFbo::Settings settingsBig;
	settingsBig.numSamples = 16;
	settingsBig.useStencil = true;
	settingsBig.useDepth = false;
	settingsBig.width = ofGetWidth();
	settingsBig.height = ofGetHeight();
	fboBig.allocate(settingsBig);

	float _ww = wAboutDemo;

	ofFbo::Settings settings;
	settings.numSamples = 16;
	settings.useStencil = true;
	settings.useDepth = false;
	settings.width = _ww;
	settings.height = _ww * (9 / 16.f);
	fbo.allocate(settings);

	fbo.createAndAttachTexture(GL_RGB, 0); //Position
	fbo.createAndAttachTexture(GL_RGBA32F, 1); //velocity
	fbo.createAndAttachRenderbuffer(GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT);
	fbo.checkStatus();

	fboBig.begin();
	ofClear(0, 0);
	fboBig.end();

	fbo.begin();
	ofClear(0, 0);
	fbo.end();

	if (b) ofEnableArbTex();
}

//--------------------------------------------------------------
void ofxColorManager::draw_DemoFbo()
{
	float _spc = ImGui::GetStyle().ItemSpacing.x;
	float _w100 = ImGui::GetContentRegionAvail().x;
	float w = wAboutDemo;
	float h = w * (9 / 16.f);

	fbo.begin();
	{
		ofClear(0, 0);

		fboBig.draw(0, 0, w, h);
	}
	fbo.end();

	if (ImGui::ImageButton(
		(ImTextureID)(uintptr_t)fbo.getTexture(0).getTextureData().textureID,
		ImVec2(w, h)))
	{
		ofLogNotice(__FUNCTION__) << "Image Pressed";

		preset_Next();
	}
}

//--------------------------------------------------------------
void ofxColorManager::gui_MenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::BeginMenu("Export Linked Preset"))
			{
				if (ImGui::MenuItem("Set Export Path", "")) { setPathPresetWatch(); }
				if (ImGui::MenuItem("Export Preset", "")) { exportPalette(); }

				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Export Kit", "")) { exportKit(); }
			if (ImGui::MenuItem("Load Preset File", "")) { loadPresetFile(); }
			if (ImGui::MenuItem("Save Preset File", "")) { savePresetFile(); }

			if (ImGui::MenuItem("Open Image to extract", "")) { setImage(); }

			ImGui::Separator();

			if (ImGui::MenuItem("Quit", "")) { ofExit(); }

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			// coolors.com import export
			//https://coolors.co/f53c65-470229-05a66e

			if (ImGui::MenuItem("Import from Coolors.co", "paste url")) {

				doImportPaletteCoolors(ofGetClipboardString());
			}

			if (ImGui::MenuItem("Export to Coolors.co", "copy url")) {

				doExportPaletteCoolors();
			}

			////TODO:
			////add other types
			//if (ImGui::MenuItem("Copy HEX Color", "")) { 
			//	//TODO: convert
			//	const char* text = ofToString(color_Picked.get().getHex()).c_str();
			//	ImGui::SetClipboardText(text);
			//};

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			static bool pref = SHOW_MenuBar.get();
			if (ImGui::MenuItem("Show Menu Bar (This!)", NULL, &pref))
			{
				SHOW_MenuBar = pref;
			}

			if (ImGui::MenuItem("Load Default Layout", NULL))
			{
				appLayoutIndex = APP_DEFAULT;
			}

			bool bFull = true;
			if (ofGetWindowMode() == OF_WINDOW) bFull = false;
			if (ImGui::MenuItem("Full Screen Mode", NULL, &bFull))
			{
				ofSetFullscreen(bFull);

				int offset = 20;
				if (!bFull && ofGetWindowPositionY() < offset) {
					glm::vec2 p{ ofGetWindowPositionX(), ofGetWindowPositionY() };
					ofSetWindowPosition(p.x, offset);
				}
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			static bool pref = SHOW_HelpInfo.get();
			if (ImGui::MenuItem("Key Commands", NULL, &pref))
			{
				SHOW_HelpInfo = pref;
			}
			if (ImGui::MenuItem("GitHub", NULL)) {
				std::string url = "https://github.com/moebiussurfing/ofxColorManager";
				ofLaunchBrowser(url);
			}
			if (ImGui::MenuItem("itch.io", NULL)) {
				std::string url = "https://moebiussurfing.itch.io/paletto";
				ofLaunchBrowser(url);
			}
			ImGui::MenuItem("About", NULL, &SHOW_About);

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

//--------------------------------------------------------------
void ofxColorManager::gui_About(bool* p_open)
{
	ImGuiWindowFlags flags;
	flags = ImGuiWindowFlags_AlwaysAutoResize;
	//flags = ImGuiWindowFlags_None;

	//TODO:
	// draw texture inside transparent backgroundg

	if (!ImGui::Begin("About", p_open, flags))
	{
		ImGui::End();
		return;
	}
	//ImGui::Image(bg_tex_id, ImGui::GetContentRegionAvail());

	//draw_DemoFbo();
	ImGui::Text(APP_RELEASE_NAME);

	//ImGui::Separator();
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	ImGui::Text("Author:");
	ImGui::Text("moebiusSurfing. ManuMolina");
	ImGui::Text("Barcelona / Buenos Aires.");
	ImGui::Text("2019 - 2021");
	ImGui::Dummy(ImVec2(0.0f, 5.0f));
	ImGui::Text("GitHub:");
	ImGui::Text("https://github.com/moebiussurfing");
	ImGui::Dummy(ImVec2(0.0f, 5.0f));
	ImGui::Text("Email:");
	ImGui::Text("moebiusSurfing@gmail.com");
	ImGui::Dummy(ImVec2(0, 10.f));
	ImGui::Text("Thanks to:");
	ImGui::Text("All the openFrameworks");
	ImGui::Text("community, coders and contributors of");
	ImGui::Text("included libraries and add-ons.");
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	ImGui::Text("MIT License");
	ImGui::Dummy(ImVec2(0, 10.0f));

	draw_DemoFbo();
	ImGui::Text("Demo Sketches by junkiyoshi.com. Thanks!");

	ImGui::End();

	//ImGui::PopStyleVar(2);
}

//--------------------------------------------------------------
void ofxColorManager::saveAppLayout(AppLayouts mode)
{
	ini_to_save_Str = getLayoutName(mode);

	////TODO:
	////workaround
	////avoid empty name
	////force save
	//if (ini_to_save_Str == "")//when unknown name
	//{
	//	appLayoutIndex_PRE = -1;
	//	appLayoutIndex = 0;
	//	ini_to_save_Str = path_ImGui + "imgui_DEFAULT.ini";
	//	ini_to_save = ini_to_save_Str.c_str();
	//	ImGui::SaveIniSettingsToDisk(ini_to_save);
	//}

	ini_to_save = ini_to_save_Str.c_str();//flags to save on update

	//save group
	ofxSurfingHelpers::saveGroup(params_LayoutPanelsState, ini_to_save_Str + ".json");
}

//--------------------------------------------------------------
void ofxColorManager::loadAppLayout(AppLayouts mode)
{
	std::string _label = APP_RELEASE_NAME; // Paletto v1.0
	_label += "       "; // spacing

	bool bEnableHardcode = false; // false makes all presets customizable!

	switch (mode)
	{
	case ofxColorManager::APP_DEFAULT:
		if (!b0) b0 = true;
		b1 = b2 = b3 = b4 = false;
		ofSetWindowTitle(_label + " F1  " + "DEFAULT");
		ini_to_load_Str = path_ImGui + "imgui_DEFAULT.ini";
		ini_to_load = ini_to_load_Str.c_str();
		ofxSurfingHelpers::loadGroup(params_LayoutPanelsState, ini_to_load_Str + ".json");
		//hardcoded forced
		if (bEnableHardcode) {
			SHOW_Palette = true;
			//SHOW_Panels = true;
			SHOW_PanelEngines = true;
			SHOW_Theory.setWithoutEventNotifications(true);
			SHOW_Range = false;
			SHOW_ColourLovers = false;
			SHOW_Quantizer = false;
			SHOW_Presets = true;
			SHOW_Editor = true;
			SHOW_Picker = true;
			//SHOW_Kit = true;
			//SHOW_Library = false;
			//SHOW_Demos = false;
			//gradientEngine.SHOW_Gradient = false;
			//SHOW_MINI_Preview = false;
			//SHOW_LinkExport = false;
			//SHOW_Advanced = false;
			//SHOW_MenuBar = true;
			//Lock_DockingLayout = true;
		}
		break;

	case ofxColorManager::APP_PRESETS:
		if (!b1) b1 = true;
		b0 = b2 = b3 = b4 = false;
		ofSetWindowTitle(_label + " F2  " + "PRESETS");
		ini_to_load_Str = path_ImGui + "imgui_PRESETS.ini";
		ini_to_load = ini_to_load_Str.c_str();
		ofxSurfingHelpers::loadGroup(params_LayoutPanelsState, ini_to_load_Str + ".json");
		//hardcoded forced
		if (bEnableHardcode) {
			SHOW_Palette = true;
			//SHOW_Panels = false;
			SHOW_PanelEngines = false;
			SHOW_Theory = false;
			SHOW_Range = false;
			SHOW_ColourLovers = false;
			SHOW_Quantizer = false;
			SHOW_Presets = true;
			SHOW_Editor = false;
			SHOW_Picker = false;
			//SHOW_Kit = true;
			//SHOW_Library = false;
			//SHOW_Demos = false;
			//gradientEngine.SHOW_Gradient = false;
			//SHOW_MINI_Preview = false;
			//SHOW_LinkExport = false;
			//SHOW_Advanced = false;
			//SHOW_MenuBar = false;
			//Lock_DockingLayout = true;
		}
		break;

	case ofxColorManager::APP_ENGINES:
		if (!b2) b2 = true;
		b0 = b1 = b3 = b4 = false;
		ofSetWindowTitle(_label + " F3  " + "ENGINES");
		ini_to_load_Str = path_ImGui + "imgui_ENGINES.ini";
		ini_to_load = ini_to_load_Str.c_str();
		ofxSurfingHelpers::loadGroup(params_LayoutPanelsState, ini_to_load_Str + ".json");
		//hardcoded forced
		if (bEnableHardcode) {
			SHOW_Palette = true;
			//SHOW_Panels = false;
			SHOW_PanelEngines = true;
			SHOW_Theory = true;
			SHOW_Range = false;
			SHOW_ColourLovers = false;
			SHOW_Quantizer = false;
			SHOW_Presets = false;
			SHOW_Editor = true;
			SHOW_Picker = true;
			//SHOW_Kit = false;
			//SHOW_Library = false;
			//SHOW_Demos = false;
			//gradientEngine.SHOW_Gradient = false;
			//SHOW_MINI_Preview = false;
			//SHOW_LinkExport = false;
			//SHOW_Advanced = false;
			//SHOW_MenuBar = false;
			//Lock_DockingLayout = true;
		}
		break;

	case ofxColorManager::APP_MINIMAL:
		if (!b3) b3 = true;
		b0 = b1 = b2 = b4 = false;
		ofSetWindowTitle(_label + " F4  " + "MINIMAL");
		ini_to_load_Str = path_ImGui + "imgui_MINIMAL.ini";
		ini_to_load = ini_to_load_Str.c_str();
		ofxSurfingHelpers::loadGroup(params_LayoutPanelsState, ini_to_load_Str + ".json");
		//hardcoded forced
		//if (bEnableHardcode) {
		//	SHOW_Palette = true;
		//	SHOW_Panels = false;
		//	SHOW_PanelEngines = false;
		//	SHOW_Theory = false;
		//	SHOW_Range = false;
		//	SHOW_ColourLovers = false;
		//	SHOW_Quantizer = false;
		//	SHOW_Presets = true;
		//	SHOW_Kit = false;
		//	SHOW_Editor = true;
		//	SHOW_Picker = true;
		//	SHOW_Library = false;
		//	SHOW_Demos = false;
		//	gradientEngine.SHOW_Gradient = false;
		//	SHOW_MINI_Preview = false;
		//	SHOW_LinkExport = false;
		//	SHOW_Advanced = false;
		//	SHOW_MenuBar = false;
		//}
		//Lock_DockingLayout = false;
		break;

	case ofxColorManager::APP_USER:
		if (!b4) b4 = true;
		b0 = b1 = b2 = b3 = false;
		ofSetWindowTitle(_label + " F5  " + "USER");
		ini_to_load_Str = path_ImGui + "imgui_USER.ini";
		ini_to_load = ini_to_load_Str.c_str();
		ofxSurfingHelpers::loadGroup(params_LayoutPanelsState, ini_to_load_Str + ".json");
		//hardcoded forced
		//if (bEnableHardcode) {
		//	SHOW_Palette = true;
		//	SHOW_Panels = false;
		//	SHOW_PanelEngines = false;
		//	SHOW_Theory = false;
		//	SHOW_Range = false;
		//	SHOW_ColourLovers = false;
		//	SHOW_Quantizer = false;
		//	SHOW_Presets = true;
		//	SHOW_Kit = false;
		//	SHOW_Editor = true;
		//	SHOW_Picker = true;
		//	SHOW_Library = false;
		//	SHOW_Demos = false;
		//	gradientEngine.SHOW_Gradient = false;
		//	SHOW_MINI_Preview = false;
		//	SHOW_LinkExport = false;
		//	SHOW_Advanced = false;
		//	SHOW_MenuBar = false;
		//}
		//Lock_DockingLayout = false;
		break;
	}

	ofLogNotice(__FUNCTION__) << "------------------------------------";
	ofLogNotice(__FUNCTION__) << "ini_to_load    : " << ini_to_load;
	ofLogNotice(__FUNCTION__) << "ini_to_load_Str: " << ini_to_load_Str;
}

//--------------------------------------------------------------
void ofxColorManager::preset_RandomIndex() {

	int i = ofRandom(last_Index_Preset.getMin(), last_Index_Preset.getMax());
	last_Index_Preset = i;

	if (last_Index_Preset < files.size() && files.size() > 0)
	{
		PRESET_Name = files_Names[last_Index_Preset];
		ofLogNotice(__FUNCTION__) << "PRESET_Name: [" + ofToString(last_Index_Preset) + "] " << PRESET_Name;

		preset_Load(PRESET_Name);
	}

	if (!AutoScroll) AutoScroll = true;
}

//--------------------------------------------------------------
void ofxColorManager::preset_Next() {
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

	if (!AutoScroll) AutoScroll = true;
}

//--------------------------------------------------------------
void ofxColorManager::preset_Previous() {
	if (last_Index_Preset > 0)
	{
		last_Index_Preset--;
	}
	else
	{
		if (last_Index_Preset == 0) last_Index_Preset = files.size() - 1;
	}

	if (last_Index_Preset < files.size() && files.size() > 0)
	{
		PRESET_Name = files_Names[last_Index_Preset];
		ofLogNotice(__FUNCTION__) << "PRESET_Name: [" + ofToString(last_Index_Preset) + "] " << PRESET_Name;

		preset_Load(PRESET_Name);
	}

	if (!AutoScroll) AutoScroll = true;
}

//TODO:
//GL scene test
//#include <algorithm>
//#define V ImVec2
//#define R(u,v,r) {float U=u;u=cos(r)*u-sin(r)*v;v=sin(r)*U+cos(r)*v;}
//#define S(T) 1.f/(1+exp(-(T)))*IM_PI*2
//#define B(T) sin((T)*3)*exp(-(T))
//int W = 11;
//unsigned c[] = { 0xD9000080,0x750A2B18,0xDC2A2A17,0x0200025D,0x5AB1E800,0x26EAB555,0x01800100 };
//struct T { float x, y, z; int i; bool operator<(T&o) { return z > o.z; } };
//void FX(ImDrawList *d, V a, V b, V s, ImVec4 m, float t) {
//	float L = fmod(t, 6) * 10; T v[221];
//	for (int n = 3; n < 224; n++) {
//		int i = n - 3;
//		float x = i % 17 * W - 8.5*W, y = i / 17 * W - 6.5*W, z = 0, D = sqrt(x*x + y * y) / 32,
//			X = S(L - 4 - D) + cos(t / 3) / 4, Y = S(L - 12 - D) + cos(t / 7) / 4, Z = S(L - 20 - D) + cos(t / 2) / 4;
//		R(x, y, Z)R(y, z, X)R(x, z, Y)
//			z -= L - D > 28 ? B((L - 28 - D) / 2) * 50 : 0;
//		z = c[n / 32] & 1 << n % 32 ? z / 100 + 1 : 0;
//		v[i] = { a.x + s.x*.5f + x / z,a.y + s.y*.5f + y / z,z,i };
//	}
//	std::sort(v, &v[221]);
//	for (int i = 0; i < 221; i++)
//		if (v[i].z != 0)
//			d->AddCircleFilled(V(v[i].x, v[i].y), W*.8 / v[i].z, ImColor(v[i].i > 102 ? 0.f : 1.f, v[i].i < 102 ? 0.f : 1.f, 3.f - v[i].z*2.5f, 1.f));
//}

//TODO:
//crash
//--------------------------------------------------------------
void ofxColorManager::doRandomizeColorLibrary(bool bPageOnly)
{
	//if (bPageOnly == true)
	//{
	//	int n = ofRandom(0, lib_Page_NumColors);
	//	n = ofClamp(n, 0, lib_Page_NumColors - 1);
	//	n = n + lib_Page_NumColors * lib_Page_Index;
	//	last_ColorPicked_Lib = n;
	//	last_Lib_Index = n;
	//	// color name
	//	if (n < palette_Lib_Names.size())
	//	{
	//		last_Lib_NameColor = palette_Lib_Names[n];
	//	}
	//	color_Picked = ofColor(palette_Lib_Cols[n]);
	//	// go to page
	//	int pag = n / lib_Page_NumColors;
	//	if (lib_Page_Index != pag) lib_Page_Index = pag;
	//}
	//else
	//{
	//	doRandomizeColorLibrary();
	//	int n = ofRandom(0, lib_TotalColors);
	//	n = ofClamp(n, 0, lib_TotalColors - 1);
	//	last_ColorPicked_Lib = n;
	//	last_Lib_Index = n;
	//	// color name
	//	if (n < palette_Lib_Names.size())
	//	{
	//		last_Lib_NameColor = palette_Lib_Names[n];
	//	}
	//	color_Picked = ofColor(palette_Lib_Cols[n]);
	//	// go to page
	//	int pag = n / lib_Page_NumColors;
	//	if (lib_Page_Index != pag) lib_Page_Index = pag;
	//}
}

//--------------------------------------------------------------
void ofxColorManager::doImportPaletteCoolors(std::string url)
{
	ofLogNotice(__FUNCTION__) << "----------------- IMPORT COOLORS -----------------" << endl;
	ofLogNotice(__FUNCTION__) << url;

	//avoid exception
	if (!ofIsStringInString(url, "https://coolors.co/"))
	{
		ofLogError(__FUNCTION__) << "Wrong url format! " << url;
		return;
	}

	//static int i = 0;

	// browse into https://coolors.co/palettes/trending
	// select palette > export > url
	// https://coolors.co/3d5a80-98c1d9-e0fbfc-ee6c4d-293241
	// https://coolors.co/f4f1de-e07a5f-3d405b-81b29a-f2cc8f

	ofStringReplace(url, "https://coolors.co/", "");
	auto ss = ofSplitString(url, "-");

	palette_Clear();

	for (auto hex : ss) {
		ofLogNotice(__FUNCTION__) << hex;
		ofColor col;
		hexToColor(col, hex);
		palette_AddColor(col);
	}

	// workflow
	//if (!MODE_NewPreset) MODE_NewPreset = true;

	textInput_New = "coolors_";
	textInput_New += ofGetTimestampString();
	//textInput_New += "_";
	//textInput_New += ofToString(++i);

	// link
	exportPalette();
}

//--------------------------------------------------------------
void ofxColorManager::resetLibraryLayout()
{
	ofLogNotice(__FUNCTION__) << "----------------- RESET LIBRARY -----------------" << endl;

	lib_Responsive_ModeGrid = false;
	lib_CardsMode = false;
	bPagerized = true;
	lib_Page_Index = 0;
	scale_LibCol = 25;
	lib_NumRows = 14;
	lib_MaxColumns = 7;
	lib_Responsive_ModeFit = true;
}

//--------------------------------------------------------------
void ofxColorManager::doExportPaletteCoolors()
{
	ofLogNotice(__FUNCTION__) << "----------------- EXPORT COOLORS -----------------" << endl;

	std::string url = "https://coolors.co/";

	for (int i = 0; i < palette.size(); i++)
	{
		ofColor c = palette[i];
		std::string hex = hexToWeb(c);
		ofLogNotice(__FUNCTION__) << i << " : " << hex;

		//remove #
		ofStringReplace(hex, "#", "");

		url += hex;
		if (i < palette.size() - 1) url += "-";
	}

	ofSetClipboardString(url);
	//https://coolors.co/f53c65-470229-05a66e

	ofLogNotice(__FUNCTION__) << url;

	// workflow
	ofLaunchBrowser(url);

	// link
	exportPalette();

	//----

#ifdef USE_ASE_MODULE
	//TODO:
	createAse();
#endif
}

//--------------------------------------------------------------
void ofxColorManager::doGetPaletteToTweaker() {
	hueTweak = 0;
	saturationTweak = 0;
	brigthnesTweak = 0;
	palette_AUX = palette;
};

//--------------------------------------------------------------
void ofxColorManager::doApplyTweaker() {

	palette = palette_AUX;

	gradientEngine.build_FromPaletteLinked(palette);

	refresh_Palette_TARGET(palette);// ?

	//palette_Clear();
	//for (int i = 0; i < palette_AUX.size(); i++)
	//{
	//	palette_AddColor(palette_AUX[i]);
	//}


	//build_Palette_Preset();

	// export
	if (bAutoExportPreset)
	{
		bExportFlag = true;
	}
};


#ifdef USE_VERSION_CHECKER
//--------------------------------------------------------------
void ofxColorManager::onNewVersionAvailable(AppUpdateNotifier::Version& version)
{
	_isAnUpdateAvailable = true;
	_newVersion = version;
}
#endif