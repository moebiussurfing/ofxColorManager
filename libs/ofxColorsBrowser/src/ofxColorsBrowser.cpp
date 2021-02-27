#include "ofxColorsBrowser.h"

//--------------------------------------------------------------
// comparing colors to sorting methods

bool compareName(const colorMapping_STRUCT &s1, const colorMapping_STRUCT &s2)
{
	return s1.name < s2.name;
}

bool compareBrightness(const colorMapping_STRUCT &s1, const colorMapping_STRUCT &s2)
{
	return s1.color.getBrightness() < s2.color.getBrightness();
}

bool compareHue(const colorMapping_STRUCT &s1, const colorMapping_STRUCT &s2)
{
	return s1.color.getHue() < s2.color.getHue();
}

bool compareSaturation(const colorMapping_STRUCT &s1, const colorMapping_STRUCT &s2)
{
	return s1.color.getSaturation() < s2.color.getSaturation();
}

bool comparePosition(const colorMapping_STRUCT &s1, const colorMapping_STRUCT &s2)
{
	return s1.position < s2.position;
}

//----

//--------------------------------------------------------------
ofxColorsBrowser::ofxColorsBrowser()
{
#ifdef USE_OFX_COLOR_BROWSER_INTERFACE
	addMouseListeners();
#endif
#ifdef USE_OFX_COLOR_BROWSER_KEYS
#ifdef KEY_SHORTCUTS_ENABLE
	setEnableKeys(true);
#else
	setEnableKeys(false);
#endif
#endif

	//-

	path_Global = "ofxColorsBrowser/";
	path_File = path_Global + "pantone-colors.json";
	path_FileSettings = path_Global + "AppSettings.xml";

	ofxSurfingHelpers::CheckFolder(path_Global);

	//--

	helpInfo = "";
	helpInfo += "\n";
	helpInfo += "KEY COMMANDS";
	helpInfo += "\n";
	//helpInfo += "\n";
	helpInfo += "Mouse Click           Browse Colors";
	helpInfo += "\n";
	helpInfo += "Up Down Left Right    Browse Colors";
	helpInfo += "\n";
	helpInfo += "a d w s		       Browse Cards";
	helpInfo += "\n";
	helpInfo += "r                     Random Card";
	helpInfo += "\n";
	helpInfo += "BackSpace             Change Library";
	helpInfo += "\n";
	helpInfo += "Tab                   Change Sorting Type";
	helpInfo += "\n";
	helpInfo += "0 1 2 3 4             Set Sort Type";
	helpInfo += "\n";
	helpInfo += "K                     Enable Keys";
	helpInfo += "\n";
	helpInfo += "D                     Debug";
	helpInfo += "\n";
	helpInfo += "G                     Gui Panel";
	helpInfo += "\n";
	helpInfo += "g                     Gui";
	helpInfo += "\n";
}

//--------------------------------------------------------------
void ofxColorsBrowser::buildColors()
{
	ofLogNotice(__FUNCTION__);

	colors_STRUCT.clear();
	colorNameMap.clear();

	//TODO: seems is not erasing last name colors..

	//-

	cardPos = glm::vec2(235, 170);
	cardNum = 0;
	currColor = 0;

	//--

	// 0. OFX_PANTONE_COLORS

	if (LibraryColors_Index == OFX_PANTONE_COLORS)
	{
		ofLogNotice(__FUNCTION__) << "OFX_PANTONE_COLORS";

		//ideal card size and layout
		cardSize = 7;
		cardsPerRow = 10;
		boxSize = 15;
		boxPad = 0.5;

		cardColor_size = 100;
		cardColor_pad = 20;

		//-

		std::string name;
		ofColor c;

		for (int i = 0; i < colors_PantoneNames.size(); i++)
		{
			name = colors_PantoneNames[i];
			c = colors_Pantone[i];

			// 1. names map
			colorNameMap[name] = c;

			// 2. struct
			colorMapping_STRUCT myColor;
			myColor.name = name;
			myColor.color = c;
			myColor.position = i;

			// 3. add color to vector
			colors_STRUCT.push_back(myColor);
		}
		ofLogNotice(__FUNCTION__);
	}

	//----

	// 1. OFX_COLOR_NATIVE

	else if (LibraryColors_Index == OFX_COLOR_NATIVE)
	{
		// dessired distribution for this palette
		cardSize = 12;
		cardsPerRow = 1;
		boxSize = 40;
		boxPad = 2;

		cardColor_size = 70;
		cardColor_pad = 10;

		// build a map from name to ofColor of all the named OF colors;
		colorNameMap["white"] = ofColor::white;
		colorNameMap["gray"] = ofColor::gray;
		colorNameMap["black"] = ofColor::black;
		colorNameMap["red"] = ofColor::red;
		colorNameMap["green"] = ofColor::green;
		colorNameMap["blue"] = ofColor::blue;
		colorNameMap["cyan"] = ofColor::cyan;
		colorNameMap["magenta"] = ofColor::magenta;
		colorNameMap["yellow"] = ofColor::yellow;
		colorNameMap["aliceBlue"] = ofColor::aliceBlue;
		colorNameMap["antiqueWhite"] = ofColor::antiqueWhite;
		colorNameMap["aqua"] = ofColor::aqua;
		colorNameMap["aquamarine"] = ofColor::aquamarine;
		colorNameMap["azure"] = ofColor::azure;
		colorNameMap["beige"] = ofColor::beige;
		colorNameMap["bisque"] = ofColor::bisque;
		colorNameMap["blanchedAlmond"] = ofColor::blanchedAlmond;
		colorNameMap["blueViolet"] = ofColor::blueViolet;
		colorNameMap["brown"] = ofColor::brown;
		colorNameMap["burlyWood"] = ofColor::burlyWood;
		colorNameMap["cadetBlue"] = ofColor::cadetBlue;
		colorNameMap["chartreuse"] = ofColor::chartreuse;
		colorNameMap["chocolate"] = ofColor::chocolate;
		colorNameMap["coral"] = ofColor::coral;
		colorNameMap["cornflowerBlue"] = ofColor::cornflowerBlue;
		colorNameMap["cornsilk"] = ofColor::cornsilk;
		colorNameMap["crimson"] = ofColor::crimson;
		colorNameMap["darkBlue"] = ofColor::darkBlue;
		colorNameMap["darkCyan"] = ofColor::darkCyan;
		colorNameMap["darkGoldenRod"] = ofColor::darkGoldenRod;
		colorNameMap["darkGray"] = ofColor::darkGray;
		colorNameMap["darkGrey"] = ofColor::darkGrey;
		colorNameMap["darkGreen"] = ofColor::darkGreen;
		colorNameMap["darkKhaki"] = ofColor::darkKhaki;
		colorNameMap["darkMagenta"] = ofColor::darkMagenta;
		colorNameMap["darkOliveGreen"] = ofColor::darkOliveGreen;
		colorNameMap["darkorange"] = ofColor::darkorange;
		colorNameMap["darkOrchid"] = ofColor::darkOrchid;
		colorNameMap["darkRed"] = ofColor::darkRed;
		colorNameMap["darkSalmon"] = ofColor::darkSalmon;
		colorNameMap["darkSeaGreen"] = ofColor::darkSeaGreen;
		colorNameMap["darkSlateBlue"] = ofColor::darkSlateBlue;
		colorNameMap["darkSlateGray"] = ofColor::darkSlateGray;
		colorNameMap["darkSlateGrey"] = ofColor::darkSlateGrey;
		colorNameMap["darkTurquoise"] = ofColor::darkTurquoise;
		colorNameMap["darkViolet"] = ofColor::darkViolet;
		colorNameMap["deepPink"] = ofColor::deepPink;
		colorNameMap["deepSkyBlue"] = ofColor::deepSkyBlue;
		colorNameMap["dimGray"] = ofColor::dimGray;
		colorNameMap["dimGrey"] = ofColor::dimGrey;
		colorNameMap["dodgerBlue"] = ofColor::dodgerBlue;
		colorNameMap["fireBrick"] = ofColor::fireBrick;
		colorNameMap["floralWhite"] = ofColor::floralWhite;
		colorNameMap["forestGreen"] = ofColor::forestGreen;
		colorNameMap["fuchsia"] = ofColor::fuchsia;
		colorNameMap["gainsboro"] = ofColor::gainsboro;
		colorNameMap["ghostWhite"] = ofColor::ghostWhite;
		colorNameMap["gold"] = ofColor::gold;
		colorNameMap["goldenRod"] = ofColor::goldenRod;
		colorNameMap["grey"] = ofColor::grey;
		colorNameMap["greenYellow"] = ofColor::greenYellow;
		colorNameMap["honeyDew"] = ofColor::honeyDew;
		colorNameMap["hotPink"] = ofColor::hotPink;
		colorNameMap["indianRed "] = ofColor::indianRed;
		colorNameMap["indigo "] = ofColor::indigo;
		colorNameMap["ivory"] = ofColor::ivory;
		colorNameMap["khaki"] = ofColor::khaki;
		colorNameMap["lavender"] = ofColor::lavender;
		colorNameMap["lavenderBlush"] = ofColor::lavenderBlush;
		colorNameMap["lawnGreen"] = ofColor::lawnGreen;
		colorNameMap["lemonChiffon"] = ofColor::lemonChiffon;
		colorNameMap["lightBlue"] = ofColor::lightBlue;
		colorNameMap["lightCoral"] = ofColor::lightCoral;
		colorNameMap["lightCyan"] = ofColor::lightCyan;
		colorNameMap["lightGoldenRodYellow"] = ofColor::lightGoldenRodYellow;
		colorNameMap["lightGray"] = ofColor::lightGray;
		colorNameMap["lightGrey"] = ofColor::lightGrey;
		colorNameMap["lightGreen"] = ofColor::lightGreen;
		colorNameMap["lightPink"] = ofColor::lightPink;
		colorNameMap["lightSalmon"] = ofColor::lightSalmon;
		colorNameMap["lightSeaGreen"] = ofColor::lightSeaGreen;
		colorNameMap["lightSkyBlue"] = ofColor::lightSkyBlue;
		colorNameMap["lightSlateGray"] = ofColor::lightSlateGray;
		colorNameMap["lightSlateGrey"] = ofColor::lightSlateGrey;
		colorNameMap["lightSteelBlue"] = ofColor::lightSteelBlue;
		colorNameMap["lightYellow"] = ofColor::lightYellow;
		colorNameMap["lime"] = ofColor::lime;
		colorNameMap["limeGreen"] = ofColor::limeGreen;
		colorNameMap["linen"] = ofColor::linen;
		colorNameMap["maroon"] = ofColor::maroon;
		colorNameMap["mediumAquaMarine"] = ofColor::mediumAquaMarine;
		colorNameMap["mediumBlue"] = ofColor::mediumBlue;
		colorNameMap["mediumOrchid"] = ofColor::mediumOrchid;
		colorNameMap["mediumPurple"] = ofColor::mediumPurple;
		colorNameMap["mediumSeaGreen"] = ofColor::mediumSeaGreen;
		colorNameMap["mediumSlateBlue"] = ofColor::mediumSlateBlue;
		colorNameMap["mediumSpringGreen"] = ofColor::mediumSpringGreen;
		colorNameMap["mediumTurquoise"] = ofColor::mediumTurquoise;
		colorNameMap["mediumVioletRed"] = ofColor::mediumVioletRed;
		colorNameMap["midnightBlue"] = ofColor::midnightBlue;
		colorNameMap["mintCream"] = ofColor::mintCream;
		colorNameMap["mistyRose"] = ofColor::mistyRose;
		colorNameMap["moccasin"] = ofColor::moccasin;
		colorNameMap["navajoWhite"] = ofColor::navajoWhite;
		colorNameMap["navy"] = ofColor::navy;
		colorNameMap["oldLace"] = ofColor::oldLace;
		colorNameMap["olive"] = ofColor::olive;
		colorNameMap["oliveDrab"] = ofColor::oliveDrab;
		colorNameMap["orange"] = ofColor::orange;
		colorNameMap["orangeRed"] = ofColor::orangeRed;
		colorNameMap["orchid"] = ofColor::orchid;
		colorNameMap["paleGoldenRod"] = ofColor::paleGoldenRod;
		colorNameMap["paleGreen"] = ofColor::paleGreen;
		colorNameMap["paleTurquoise"] = ofColor::paleTurquoise;
		colorNameMap["paleVioletRed"] = ofColor::paleVioletRed;
		colorNameMap["papayaWhip"] = ofColor::papayaWhip;
		colorNameMap["peachPuff"] = ofColor::peachPuff;
		colorNameMap["peru"] = ofColor::peru;
		colorNameMap["pink"] = ofColor::pink;
		colorNameMap["plum"] = ofColor::plum;
		colorNameMap["powderBlue"] = ofColor::powderBlue;
		colorNameMap["purple"] = ofColor::purple;
		colorNameMap["rosyBrown"] = ofColor::rosyBrown;
		colorNameMap["royalBlue"] = ofColor::royalBlue;
		colorNameMap["saddleBrown"] = ofColor::saddleBrown;
		colorNameMap["salmon"] = ofColor::salmon;
		colorNameMap["sandyBrown"] = ofColor::sandyBrown;
		colorNameMap["seaGreen"] = ofColor::seaGreen;
		colorNameMap["seaShell"] = ofColor::seaShell;
		colorNameMap["sienna"] = ofColor::sienna;
		colorNameMap["silver"] = ofColor::silver;
		colorNameMap["skyBlue"] = ofColor::skyBlue;
		colorNameMap["slateBlue"] = ofColor::slateBlue;
		colorNameMap["slateGray"] = ofColor::slateGray;
		colorNameMap["slateGrey"] = ofColor::slateGrey;
		colorNameMap["snow"] = ofColor::snow;
		colorNameMap["springGreen"] = ofColor::springGreen;
		colorNameMap["steelBlue"] = ofColor::steelBlue;
		colorNameMap["tan"] = ofColor::tan;
		colorNameMap["teal"] = ofColor::teal;
		colorNameMap["thistle"] = ofColor::thistle;
		colorNameMap["tomato"] = ofColor::tomato;
		colorNameMap["turquoise"] = ofColor::turquoise;
		colorNameMap["violet"] = ofColor::violet;
		colorNameMap["wheat"] = ofColor::wheat;
		colorNameMap["whiteSmoke"] = ofColor::whiteSmoke;
		colorNameMap["yellowGreen"] = ofColor::yellowGreen;

		int i = 0;
		for (auto const &x : colorNameMap)
		{
			// std::cout << x.first  // string (key)
			// << ':'
			// << x.second // string's value
			// << std::endl ;

			colorMapping_STRUCT myColor;
			myColor.name = x.first;
			myColor.color = x.second;
			myColor.position = i;

			colors_STRUCT.push_back(myColor);

			i++;
		}
	}

	//----

	// 2. OFX_OPEN_COLOR

	else if (LibraryColors_Index == OFX_OPEN_COLOR)
	{
		ofLogNotice(__FUNCTION__) << "OFX_OPEN_COLOR";

#define NUM_COLORS_ROW 10
		//that's the ideal distribution for open color palette

		// dessired distribution for this palette
		cardSize = 13;
		cardsPerRow = 1;
		boxSize = 30;
		boxPad = 1;

		cardColor_size = 65;
		cardColor_pad = 5;

		bool flipOrder = true;
		int iFlip;
		int pos = 0;//-1? to set correlative positions...

		for (int i = 0; i < NUM_COLORS_ROW; i++)
		{
			// flip order: iFlip
			if (flipOrder) iFlip = (NUM_COLORS_ROW - 1) - i;
			else iFlip = i;

			// 1.
			colorNameMap["GREY " + ofToString(i)] = oc_gray_[iFlip];
			colorNameMap["RED " + ofToString(i)] = oc_red_[iFlip];
			colorNameMap["PINK " + ofToString(i)] = oc_pink_[iFlip];
			colorNameMap["GRAPE " + ofToString(i)] = oc_grape_[iFlip];
			colorNameMap["VIOLET " + ofToString(i)] = oc_violet_[iFlip];
			colorNameMap["INDIGO " + ofToString(i)] = oc_indigo_[iFlip];
			colorNameMap["BLUE " + ofToString(i)] = oc_blue_[iFlip];
			colorNameMap["CYAN " + ofToString(i)] = oc_cyan_[iFlip];
			colorNameMap["TEAL " + ofToString(i)] = oc_teal_[iFlip];
			colorNameMap["GREEN " + ofToString(i)] = oc_green_[iFlip];
			colorNameMap["LIME " + ofToString(i)] = oc_lime_[iFlip];
			colorNameMap["YELLOW " + ofToString(i)] = oc_yellow_[iFlip];
			colorNameMap["ORANGE " + ofToString(i)] = oc_orange_[iFlip];

			// 2. vector
			colorMapping_STRUCT colorGREY;
			colorMapping_STRUCT colorRED;
			colorMapping_STRUCT colorPINK;
			colorMapping_STRUCT colorGRAPE;
			colorMapping_STRUCT colorVIOLET;
			colorMapping_STRUCT colorINDIGO;
			colorMapping_STRUCT colorBLUE;
			colorMapping_STRUCT colorCYAN;
			colorMapping_STRUCT colorTEAL;
			colorMapping_STRUCT colorGREEN;
			colorMapping_STRUCT colorLIME;
			colorMapping_STRUCT colorYELLOW;
			colorMapping_STRUCT colorORANGE;

			colorGREY.name = "GREY " + ofToString(i);
			colorRED.name = "RED " + ofToString(i);
			colorPINK.name = "PINK " + ofToString(i);
			colorGRAPE.name = "GRAPE " + ofToString(i);
			colorVIOLET.name = "VIOLET " + ofToString(i);
			colorINDIGO.name = "INDIGO " + ofToString(i);
			colorBLUE.name = "BLUE " + ofToString(i);
			colorCYAN.name = "CYAN " + ofToString(i);
			colorTEAL.name = "TEAL " + ofToString(i);
			colorGREEN.name = "GREEN " + ofToString(i);
			colorLIME.name = "LIME " + ofToString(i);
			colorYELLOW.name = "YELLOW " + ofToString(i);
			colorORANGE.name = "ORANGE " + ofToString(i);

			colorGREY.color = oc_gray_[iFlip];
			colorRED.color = oc_red_[iFlip];
			colorPINK.color = oc_pink_[iFlip];
			colorGRAPE.color = oc_grape_[iFlip];
			colorVIOLET.color = oc_violet_[iFlip];
			colorINDIGO.color = oc_indigo_[iFlip];
			colorBLUE.color = oc_blue_[iFlip];
			colorCYAN.color = oc_cyan_[iFlip];
			colorTEAL.color = oc_teal_[iFlip];
			colorGREEN.color = oc_green_[iFlip];
			colorLIME.color = oc_lime_[iFlip];
			colorYELLOW.color = oc_yellow_[iFlip];
			colorORANGE.color = oc_orange_[iFlip];

			colorGREY.position = pos++;
			colorRED.position = pos++;
			colorPINK.position = pos++;
			colorGRAPE.position = pos++;
			colorVIOLET.position = pos++;
			colorINDIGO.position = pos++;
			colorBLUE.position = pos++;
			colorCYAN.position = pos++;
			colorTEAL.position = pos++;
			colorGREEN.position = pos++;
			colorLIME.position = pos++;
			colorYELLOW.position = pos++;
			colorORANGE.position = pos++;

			//-

			colors_STRUCT.push_back(colorGREY);
			colors_STRUCT.push_back(colorRED);
			colors_STRUCT.push_back(colorPINK);
			colors_STRUCT.push_back(colorGRAPE);
			colors_STRUCT.push_back(colorVIOLET);
			colors_STRUCT.push_back(colorINDIGO);
			colors_STRUCT.push_back(colorBLUE);
			colors_STRUCT.push_back(colorCYAN);
			colors_STRUCT.push_back(colorTEAL);
			colors_STRUCT.push_back(colorGREEN);
			colors_STRUCT.push_back(colorLIME);
			colors_STRUCT.push_back(colorYELLOW);
			colors_STRUCT.push_back(colorORANGE);
		}
	}
}

//--------------------------------------------------------------
void ofxColorsBrowser::load_Pantone_JSON()
{
	ofLogNotice(__FUNCTION__);

	colors_PantoneNames.clear();
	colors_Pantone.clear();

	ofFile file(path_File);
	if (file.exists())
	{
		file >> js;
		ofLogNotice(__FUNCTION__) << js;
		ofLogNotice(__FUNCTION__) << endl;

		int i = 0;
		for (auto &jsName : js["names"])
		{
			//ofLogNotice(__FUNCTION__) << "NAMES  ["<<i<<"] "<<jsName<<endl;
			colors_PantoneNames.push_back(jsName);
			i++;
		}

		i = 0;
		for (auto &jsValues : js["values"])
		{
			//ofLogNotice(__FUNCTION__) << "VALUES ["<<i<<"] "<<jsValues<<endl;
			ofColor c;
			std::string colorHEXcode = ofToString(jsValues);
			vector<std::string> colorHEXcode_VEC = ofSplitString(colorHEXcode, "#");
			std::string myCol = colorHEXcode_VEC[1];
			vector<std::string> myCol2 = ofSplitString(myCol, "\"");
			hexToColor(c, myCol2[0]);

			colors_Pantone.push_back(c);
			i++;
		}
	}
	else
	{
		ofLogNotice(__FUNCTION__) << "FILE '" << path_File << "' NOT FOUND!";
	}
}


//--------------------------------------------------------------
void ofxColorsBrowser::setup()
{
	setPosition(glm::vec2(220, 350));//call before setup

	//--

#ifdef USE_OFX_COLOR_BROWSER_INTERFACE
	std::string _path;
	_path = "assets/fonts/Kazesawa-Extrabold.ttf";
	font.setup(_path, 1.0, 1024, false, 8, 1.5);
	font.setCharacterSpacing(0);

	_path = "assets/fonts/mono.ttf";
	font2.load(_path, 8, true, true);
#endif

	//-

	MODE_SORTING_name.setSerializable(false);
	LibraryColors_Index_name.setSerializable(false);

	params.setName("ofxColorsBrowser");
	params.add(LibraryColors_Index);
	params.add(LibraryColors_Index_name);
	params.add(MODE_SORTING);
	params.add(MODE_SORTING_name);
	params.add(ENABLE_keys);

#ifndef USE_OFX_COLOR_BROWSER_INTERFACE
	LibraryColors_Index.setSerializable(false);
#endif

	//-

	gui.setup("ofxColorsBrowser");
	gui.add(params);
	gui.setPosition(ofGetWidth() * 0.5 - 100, 10);

	//-


	//--------------------------------------------------------------
	listener_ModeSorting = MODE_SORTING.newListener([this](int &i) {
		ofLogNotice("MODE_SORTING: ") << i;

		if (MODE_SORTING == 5) MODE_SORTING = 0;
		MODE_SORTING = ofClamp(MODE_SORTING, 0, 4);

		if (MODE_SORTING == 0) { ofSort(colors_STRUCT, comparePosition); MODE_SORTING_name = "Original"; }
		else if (MODE_SORTING == 1) { ofSort(colors_STRUCT, compareName); MODE_SORTING_name = "Name"; }
		else if (MODE_SORTING == 2) { ofSort(colors_STRUCT, compareHue); MODE_SORTING_name = "Hue"; }
		else if (MODE_SORTING == 3) { ofSort(colors_STRUCT, compareBrightness); MODE_SORTING_name = "Brightness"; }
		else if (MODE_SORTING == 4) { ofSort(colors_STRUCT, compareSaturation); MODE_SORTING_name = "Saturation"; }

		clearInterface();
		grid_create_boxes();

	});

	//-

	//--------------------------------------------------------------
	listener_Library = LibraryColors_Index.newListener([this](int &i) {
		ofLogNotice("LibraryColors_Index: ") << i;

		LibraryColors_Index = LibraryColors_Index % 3;

		switch (LibraryColors_Index)
		{
		case OFX_PANTONE_COLORS:
			ofLogNotice(__FUNCTION__) << "OFX_PANTONE_COLORS";
			LibraryColors_Index_name = "Pantone";
			break;

		case OFX_COLOR_NATIVE:
			ofLogNotice(__FUNCTION__) << "OFX_COLOR_NATIVE";
			LibraryColors_Index_name = "OF Native";
			break;

		case OFX_OPEN_COLOR:
			ofLogNotice(__FUNCTION__) << "OFX_OPEN_COLOR";
			LibraryColors_Index_name = "Open Color";
			break;
		}

		//-

		clearInterface();

		buildColors();

		grid_create_boxes();

	});

	//----

	// pantone colors
	load_Pantone_JSON();

	//--

	// 1. default sorting
	MODE_SORTING = SORTING_ORIGINAL;//
	// by name, at the start

	// 2. default palette mode
	LibraryColors_Index = OFX_PANTONE_COLORS;

	//--

	buildColors();

	//--

#ifdef USE_OFX_COLOR_BROWSER_INTERFACE
	grid_create_boxes();
#endif	

	//-----

	// startup

	if (isVisible()) {
		setEnableInterfaceClicks(true);
		setVisibleDebug(true);
	}

	//currColor = 0;
	//cardNum = 0;
	//refresh_Clicks();

	ofxSurfingHelpers::loadGroup(params, path_FileSettings);
}


//--------------------------------------------------------------
void ofxColorsBrowser::grid_create_boxes()
{
#ifdef USE_OFX_COLOR_BROWSER_INTERFACE

	ofLogNotice(__FUNCTION__);

	float x = position.x;
	float y = position.y;

	perRow = cardSize * cardsPerRow;

	isSelecting = false;
	draggingRectPtr = NULL;

	for (int i = 0; i < colors_STRUCT.size(); i++)
	{
		float xBtn = x + (i % perRow) * (boxSize + boxPad);
		float yBtn = y + (i / perRow) * (boxSize + boxPad);

		ofColor c = colors_STRUCT[i].color;

		ofxRectangle rect(ofRectangle(xBtn, yBtn, boxSize, boxSize), c);

		rect.setName(colors_STRUCT[i].name);
		rect.setPosition_inVector(colors_STRUCT[i].position);

		rectangles.push_back(rect);
	}

	selectedRectsBoundingBox = ofxRectangle(ofRectangle(), ofColor(127, 80));
	hAlign = OF_ALIGN_HORZ_LEFT;
	vAlign = OF_ALIGN_VERT_TOP;
	anchorRect = NULL;
	showKeyboardCommands = false;

	// startup
	currColor = 0;
	cardNum = 0;
	refresh_Clicks();

#endif
}


//--------------------------------------------------------------
void ofxColorsBrowser::update()
{
	if (color_BACK != color_BACK_PRE)
	{
		color_BACK_OFAPP->set(color_BACK);
		color_BACK_PRE = color_BACK;
	}

	//-

#ifdef USE_OFX_COLOR_BROWSER_INTERFACE
	rectangles_update();
#endif
}

//--------------------------------------------------------------
void ofxColorsBrowser::draw()
{
	if (SHOW_Gui)
	{
		ofPushMatrix();
		ofPushStyle();

		//TODO: add more pages...

		//-

		// 1. DRAW ALL THE COLORS NAMES

		int maxLinesThatFitsScreen = 42;
		bool bColorizeLabel = false;
		bool bColorizeBg = true;

		int iPadded;
		int line;
		int lineBegin;
		int lineEnd;
		int maxCards = maxLinesThatFitsScreen / cardSize;
		int linesPage = cardSize * maxCards;
		int pageNum;

		pageNum = (int)currColor / linesPage;
		lineBegin = pageNum * linesPage;
		lineEnd = lineBegin + linesPage - 1;//-1

		std::string str;
		ofColor c;

		//--

		// draw all color names marking the one selected

#ifdef USE_OFX_COLOR_BROWSER_INTERFACE

		// 1. left lateral list

		if (SHOW_debugText)
		{
			float _x = 10;
			float _y = 20;

			for (int i = lineBegin; i <= lineEnd; i++)
			{
				line = i;

				if (colors_STRUCT.size() > 0)
					str = colors_STRUCT[line].name;

				if (pageNum == 0)
				{
					iPadded = i;
				}
				else
				{
					iPadded = i - lineBegin;
				}

				// all marked names
				float fontSize = 20;
				float fontMargin = 10;
				float x = _x;
				float y = _y + iPadded * 20;
				float rectWidth = 200;
				int margin = 6;

				//-

				// 1. selected color
				if (i == currColor)
				{
					// rectangle
					if (!bColorizeBg) ofSetColor(0);//black
					else ofSetColor(colors_STRUCT[i].color);
					ofDrawRectangle(x, y - fontSize + margin, rectWidth, fontSize);

					// text

					if (bColorizeLabel) c = colors_STRUCT[i].color;
					else c = ofColor::white;


					// A
					ofSetColor(c);
					font.draw(
						str,
						fontSize,
						x + margin,
						y
					);

					// B
					//ofDrawBitmapStringHighlight(str, x, y, c, ofColor::black);
				}

				// 2. all color not selected
				else
				{
					// back light
					ofSetColor(255);//white
					ofDrawRectangle(x, y - fontSize + margin, rectWidth, fontSize);

					if (bColorizeLabel) c = colors_STRUCT[i].color;
					else c = ofColor::black;

					//A
					//ofDrawBitmapStringHighlight(str, 10, 20 + iPadded * 20, c, ofColor::white);

					//B
					ofSetColor(c);
					font.draw(
						str,
						fontSize,
						x + margin,
						y
					);
				}

				// line to mark first color on each card
				if (i % cardSize == 0)
				{
					int lineSize = 3;
					int px = x + 2;
					int py = y - 4;
					ofSetLineWidth(2.0);
					ofDrawLine(px, py, px + lineSize, py);
				}
			}
		}
#endif

		//--

		// 2. MONITOR COLOR SELECTED: name & position in vector

#ifdef USE_OFX_COLOR_BROWSER_INTERFACE
		if (SHOW_debugText)
		{
			int x = position.x + 5;
			int y = 10;

			std::string str = "";
			str += "DEBUG";
			str += "\n";
			str += "Color         " + ofToString(currColor_OriginalPos) + "/" + ofToString(colors_STRUCT.size() - 1);
			str += "\n";
			str += "Card          " + ofToString(cardNum);
			str += "\n";
			str += "Page          " + ofToString(pageNum);
			str += "\n";
			str += "Card Size     " + ofToString(cardSize);
			str += "\n";
			str += "Cards/Row     " + ofToString(cardsPerRow);
			//str += "\n";

			ofxSurfingHelpers::drawTextBoxed(font2, str, x, y);
		}
#endif

		//--

		// 3. MONITOR APP MODES

		if (SHOW_InterfaceColors)
		{
			if (SHOW_debugText)
			{
				std::string str1;
				str1 = "";
				str1 += "\n";

				str1 += "MONITOR";
				str1 += "\n";

				//str1 += "\n";

				str1 += "LIBRARY: ";

				//-

				switch (LibraryColors_Index)
				{
				case OFX_PANTONE_COLORS:
					str1 += "PANTONE";
					break;
				case OFX_COLOR_NATIVE:
					str1 += "OF NATIVE";
					break;
				case OFX_OPEN_COLOR:
					str1 += "OPEN COLOR";
					break;
				}
				//ofDrawBitmapStringHighlight(str1, positionHelper.x, positionHelper.y + 20, ofColor::black, ofColor::white);

				//-

				str1 += "\n";

				str1 += "SORTING: ";
				switch (MODE_SORTING)
				{
				case 0:
					str1 += "ORIGINAL";
					break;
				case 1:
					str1 += "NAME";
					break;
				case 2:
					str1 += "HUE";
					break;
				case 3:
					str1 += "BRIGHTNESS";
					break;
				case 4:
					str1 += "SATURATION";
					break;
				}
				//ofDrawBitmapStringHighlight(str1, positionHelper.x, positionHelper.y + 50, ofColor::black, ofColor::white);

				//-

				str1 += "\n";

				std::string str2 = str1 + helpInfo;

				ofxSurfingHelpers::drawTextBoxed(font2, str2, positionHelper.x, positionHelper.y);
			}

			//--

			// 4. DRAW COLOR BOXES

#ifdef USE_OFX_COLOR_BROWSER_INTERFACE
			rectangles_draw();
			//rectangles with mouse management and draggables..
#endif
		//--
		}

		ofPopMatrix();
		ofPopStyle();

		//-

		if (bGui) gui.draw();
	}
}

#ifdef USE_OFX_COLOR_BROWSER_KEYS
//--------------------------------------------------------------
void ofxColorsBrowser::keyPressed(ofKeyEventArgs &eventArgs)
{
	const int &key = eventArgs.key;

	ofLogNotice(__FUNCTION__) << " : " << key;

	if (key == 'K')
	{
		setToggleEnableKeys();
	}

	//-

	if (ENABLE_keys)
	{

		//-

		if (false) {}

		//if (key == 'g')
		//{
		//	setToggleVisible();
		//}

		//else if (key == 'D')
		//{
		//	setToggleVisibleDebug();
		//}

		//else if (key == 'G')
		//{
		//	bGui = !bGui;
		//}

		// 0. card selector

		else if (key == OF_KEY_RIGHT_SHIFT || key == 'd')//prev card
		{
			cardNum++;
			if (cardSize * cardNum + cardSize > colors_STRUCT.size())
				cardNum = 0;

			currColor = cardSize * cardNum;
			refresh_Clicks();
		}
		else if (key == OF_KEY_LEFT_SHIFT || key == 'a')//next card
		{
			cardNum--;
			if (cardNum < 0)
				cardNum = (colors_STRUCT.size() - 1) / cardSize;

			currColor = cardSize * cardNum;
			refresh_Clicks();
		}
		else if (key == 's')//prev card
		{
			currColor += cardsPerRow * cardSize;
			if (currColor > colors_STRUCT.size() - 1)
				currColor = 0;
			cardNum = currColor / cardSize;
			refresh_Clicks();
		}
		else if (key == 'w')//prev card
		{
			currColor -= cardsPerRow * cardSize;
			if (currColor < 0)
				currColor = colors_STRUCT.size() - 1 - cardSize;
			cardNum = currColor / cardSize;
			refresh_Clicks();
		}

		//-

		else if (key == 'r')//random to one card
		{
			cardNum = (int)ofRandom((colors_STRUCT.size()) / cardSize);
			currColor = cardSize * cardNum;
			refresh_Clicks();
		}
		//-

		// 1. slelect colors 
		else if (key == OF_KEY_RIGHT)
		{
			currColor++;
			int sizeCols = colors_STRUCT.size();
			if (currColor > sizeCols - 1)
				currColor = 0;
			cardNum = currColor / cardSize;
			refresh_Clicks();
		}
		else if (key == OF_KEY_LEFT)
		{
			currColor--;
			if (currColor < 0)
				currColor = colors_STRUCT.size() - 1;
			cardNum = currColor / cardSize;
			refresh_Clicks();
		}
		else if (key == OF_KEY_DOWN)
		{
			currColor = currColor + perRow;
			int sizeCols = colors_STRUCT.size();
			if (currColor > sizeCols - 1)
				currColor = sizeCols - 1;
			cardNum = currColor / cardSize;
			refresh_Clicks();
		}
		else if (key == OF_KEY_UP)
		{
			currColor = currColor - perRow;
			if (currColor < 0)
				currColor = 0;
			cardNum = currColor / cardSize;
			refresh_Clicks();
		}
		//#endif

				//-

				// 2. change to next palette

		if (key == OF_KEY_BACKSPACE)
		{
			LibraryColors_Index++;
		}

		//-

		// 3. select sorting

		else if (key == '0')
		{
			if (MODE_SORTING != 0)
			{
				MODE_SORTING = 0;
			}
		}
		else if (key == '1')
		{
			if (MODE_SORTING != 1)
			{
				MODE_SORTING = 1;
			}
		}
		else if (key == '2')
		{
			if (MODE_SORTING != 2)
			{
				MODE_SORTING = 2;
			}
		}
		else if (key == '3')
		{
			if (MODE_SORTING != 3)
			{
				MODE_SORTING = 3;
			}
		}
		else if (key == '4')
		{
			if (MODE_SORTING != 4)
			{
				MODE_SORTING = 4;
			}
		}
		else if (key == OF_KEY_TAB)
		{
			nextSortType();
		}

		//--

		//    // rectangles manager
		//
		//    // some tools to rectangles sorting and align
		//
		////    // show debug
		////else if (key == 'd')
		////{
		////    bShowDebug = !bShowDebug;
		////
		////    for (int i = 0; i < rectangles.size(); i++)
		////    {
		////        rectangles[i].setDebug(bShowDebug);
		////    }
		////}
		////
		////else if (key == 'p')
		////{
		////    RectangleUtils::pack(selectedRects, ofRectangle(0,
		////        0,
		////        ofRandom(500),
		////        ofRandom(500)));
		////}
		//
		//    // debug ofxRectangle handling
		//else if (bShowDebug)
		//{
		//
		//    //    if (key == OF_KEY_UP) {
		//    //        for(size_t i = 0; i < selectedRects.boxSize(); i++) {
		//    //            selectedRects[i]->vAlign = OF_ALIGN_VERT_TOP;
		//    //        }
		//    //    } else if (key == OF_KEY_DOWN) {
		//    //        for(size_t i = 0; i < selectedRects.boxSize(); i++) {
		//    //            selectedRects[i]->vAlign = OF_ALIGN_VERT_BOTTOM;
		//    //        }
		//    //    } else if (key == OF_KEY_LEFT) {
		//    //        for(size_t i = 0; i < selectedRects.boxSize(); i++) {
		//    //            selectedRects[i]->hAlign = OF_ALIGN_HORZ_LEFT;
		//    //        }
		//    //    } else if (key == OF_KEY_RIGHT) {
		//    //        for(size_t i = 0; i < selectedRects.boxSize(); i++) {
		//    //            selectedRects[i]->hAlign = OF_ALIGN_HORZ_RIGHT;
		//    //        }
		//    //    } else if (key == 'c') {
		//    //        for(size_t i = 0; i < selectedRects.boxSize(); i++) {
		//    //            selectedRects[i]->hAlign = OF_ALIGN_HORZ_CENTER;
		//    //        }
		//    //    } else if (key == 'C') {
		//    //        for(size_t i = 0; i < selectedRects.boxSize(); i++) {
		//    //            selectedRects[i]->vAlign = OF_ALIGN_VERT_CENTER;
		//    //        }
		//    //    } else if (key == 'i') {
		//    //        for(size_t i = 0; i < selectedRects.boxSize(); i++) {
		//    //            selectedRects[i]->hAlign = OF_ALIGN_HORZ_IGNORE;
		//    //        }
		//    //    } else if (key == 'I') {
		//    //        for(size_t i = 0; i < selectedRects.boxSize(); i++) {
		//    //            selectedRects[i]->vAlign = OF_ALIGN_VERT_IGNORE;
		//    //        }
		//    //
		//    /*} else */ if (key == 'a')
		//    {
		//        if (hAlign == OF_ALIGN_HORZ_LEFT)
		//        {
		//            hAlign = OF_ALIGN_HORZ_CENTER;
		//        }
		//        else if (hAlign == OF_ALIGN_HORZ_CENTER)
		//        {
		//            hAlign = OF_ALIGN_HORZ_RIGHT;
		//        }
		//        else if (hAlign == OF_ALIGN_HORZ_RIGHT)
		//        {
		//            hAlign = OF_ALIGN_HORZ_IGNORE;
		//        }
		//        else if (hAlign == OF_ALIGN_HORZ_IGNORE)
		//        {
		//            hAlign = OF_ALIGN_HORZ_LEFT;
		//        }
		//    }
		//    else if (key == 'A')
		//    {
		//        if (vAlign == OF_ALIGN_VERT_TOP)
		//        {
		//            vAlign = OF_ALIGN_VERT_CENTER;
		//        }
		//        else if (vAlign == OF_ALIGN_VERT_CENTER)
		//        {
		//            vAlign = OF_ALIGN_VERT_BOTTOM;
		//        }
		//        else if (vAlign == OF_ALIGN_VERT_BOTTOM)
		//        {
		//            vAlign = OF_ALIGN_VERT_IGNORE;
		//        }
		//        else if (vAlign == OF_ALIGN_VERT_IGNORE)
		//        {
		//            vAlign = OF_ALIGN_VERT_TOP;
		//        }
		//    }
		//    else if (key == 'W')
		//    {
		//        RectangleUtils::sortByAbsWidth(selectedRects);
		//    }
		//    else if (key == 'A')
		//    {
		//        RectangleUtils::sortByArea(selectedRects);
		//    }
		//    else if (key == 'H')
		//    {
		//        RectangleUtils::sortByAbsHeight(selectedRects);
		//    }
		//    else if (key == 'c')
		//    {
		//        RectangleUtils::cascade(selectedRects, ofRectangle(0, 0, ofGetWidth(), ofGetHeight()), glm::vec2(30, 30));
		//    }
		//    else if (key == 'v')
		//    {
		//        RectangleUtils::alignVert(selectedRects, vAlign);
		//    }
		//    else if (key == 'h')
		//    {
		//        // horizontal align selection
		//        RectangleUtils::alignHorz(selectedRects, hAlign);
		//    }
		//    else if (key == 'x')
		//    {
		//        // distribute in x
		//        RectangleUtils::distributeHorz(selectedRects, hAlign);
		//    }
		//    else if (key == 'y')
		//    {
		//        RectangleUtils::distributeVert(selectedRects, vAlign);
		//    }
		//    else if (key == 'p')
		//    {
		//        RectangleUtils::pack(selectedRects, ofRectangle(0,
		//            0,
		//            ofGetWidth(),
		//            ofGetHeight()));
		//    }
		//    else if (key == ' ')
		//    {
		//        showKeyboardCommands = !showKeyboardCommands;
		//    }
		//}
	}
}
#endif

//--------------------------------------------------------------
void ofxColorsBrowser::clearInterface()
{
#ifdef USE_OFX_COLOR_BROWSER_INTERFACE
	ofLogNotice(__FUNCTION__);

	rectangles.clear();
#endif	
}

#ifdef USE_OFX_COLOR_BROWSER_KEYS
//--------------------------------------------------------------
void ofxColorsBrowser::keyReleased(ofKeyEventArgs &eventArgs)
{
}
//--------------------------------------------------------------
void ofxColorsBrowser::addKeysListeners()
{
	ofAddListener(ofEvents().keyPressed, this, &ofxColorsBrowser::keyPressed);
}

//--------------------------------------------------------------
void ofxColorsBrowser::removeKeysListeners()
{
	ofRemoveListener(ofEvents().keyPressed, this, &ofxColorsBrowser::keyPressed);
}
#endif

//--------------------------------------------------------------
void ofxColorsBrowser::mouseDragged(ofMouseEventArgs &eventArgs)
{
#ifdef USE_OFX_COLOR_BROWSER_INTERFACE

	if (bShowDebug)
	{
		const int &x = eventArgs.x;
		const int &y = eventArgs.y;
		const int &button = eventArgs.button;
		//ofLogNotice(__FUNCTION__) << "mouseDragged " <<  x << ", " << y << ", " << button;

		if (draggingRectPtr != NULL)
		{
			draggingRectPtr->setPosition(ofPoint(x, y) - draggingRectPtr->dragOffset);

			if (draggingRectPtr == &selectedRectsBoundingBox)
			{
				for (size_t i = 0; i < rectangles.size(); i++)
				{
					if (rectangles[i].isSelected)
					{
						rectangles[i].setPosition(ofPoint(x, y) - rectangles[i].dragOffset);
					}
				}
			}
		}
	}

#endif
}


//--------------------------------------------------------------
void ofxColorsBrowser::mousePressed(ofMouseEventArgs &eventArgs)
{
#ifdef USE_OFX_COLOR_BROWSER_INTERFACE
	if (ENABLE_clicks)
	{
		const int &x = eventArgs.x;
		const int &y = eventArgs.y;
		const int &button = eventArgs.button;

		//ofLogNotice(__FUNCTION__) << "mousePressed " <<  x << ", " << y << ", " << button;

		//-

		// 1. rectangles system

		dragStart = glm::vec2(x, y);  // set a new drag start point

		// SHORTCUT MODE

		if (!ofGetKeyPressed('A'))
		{
			bool foundAClickTarget = false;

			// first check to see if we are in the bounding box
			if (!selectedRects.empty() &&
				selectedRectsBoundingBox.inside(dragStart))
			{
				if (bShowDebug)
				{
					draggingRectPtr = &selectedRectsBoundingBox;
					//selectedRectsBoundingBox.dragOffset = dragStart - selectedRectsBoundingBox.getPosition().xy;
					selectedRectsBoundingBox.dragOffset.x = dragStart.x - selectedRectsBoundingBox.getPosition().x;
					selectedRectsBoundingBox.dragOffset.y = dragStart.y - selectedRectsBoundingBox.getPosition().y;

					for (std::size_t i = 0; i < rectangles.size(); i++)
					{
						if (rectangles[i].isSelected)
						{
							//rectangles[i].dragOffset = dragStart - rectangles[i].getPosition().xy;
							rectangles[i].dragOffset.x = dragStart.x - rectangles[i].getPosition().x;
							rectangles[i].dragOffset.y = dragStart.y - rectangles[i].getPosition().y;
						}
					}
					foundAClickTarget = true;
				}
			}

			// RECTANGLE COLOR CLICKED
			else
			{

				selectedRects.clear();
				// otherwise, go through all of the rects and see if we can drag one
				for (size_t i = 0; i < rectangles.size(); i++)
				{
					rectangles[i].isSelected = false; // assume none

					if (!foundAClickTarget && rectangles[i].isOver)
					{
						draggingRectPtr = &rectangles[i];
						rectangles[i].isSelected = true;
						rectangles[i].dragOffset = dragStart - rectangles[i].getPosition();
						foundAClickTarget = true;

						//-

						// TEST
						ofLogNotice(__FUNCTION__) << "mousePressed box clicked  [" << i << "]";
						ofLogNotice(__FUNCTION__) << "mousePressed box name     [" << rectangles[i].name << "]";
						ofLogNotice(__FUNCTION__) << "mousePressed box position ["
							<< rectangles[i].position_inVector << "]";

						//-

						// 1. get rectangle clicked color

						ofColor cRect = rectangles[i].color;

						//-

						// 2. apply color pointer back

						color_BACK = ofColor(cRect);
						// ofColor c = colors_STRUCT[currColor].color;
						// color_BACK = ofColor( c );

						//-

						// 3. update browsing grid

						currColor = i;
						ofLogNotice(__FUNCTION__) << "currColor [" << currColor << "]";

						currName = colors_STRUCT[currColor].name;
						ofLogNotice(__FUNCTION__) << "currName [" << currName << "]";

						currColor_OriginalPos = colors_STRUCT[currColor].position;
						ofLogNotice(__FUNCTION__) << "originalPos[" << currColor_OriginalPos << "]";

						//-

						// 4. update selected card
						cardNum = currColor / cardSize;
						//refresh_Clicks();
					}
				}
			}

			isSelecting = !foundAClickTarget; // means our click did not land on an existing rect
		}
		else
		{
			if (anchorRect != nullptr)
			{
				delete anchorRect;
				anchorRect = nullptr;
			}

			if (bShowDebug)
			{
				anchorRect = new ofRectangle(dragStart, 0, 0);
			}
		}
	}

#endif
}


//--------------------------------------------------------------
void ofxColorsBrowser::refresh_Clicks()//over rectangles
{
#ifdef USE_OFX_COLOR_BROWSER_INTERFACE

	ofLogNotice(__FUNCTION__) << "make clicked box by keys following currColor: [" << currColor << "]";

	//-

	// handle rectangles

	selectedRects.clear();//clear drag and deselect

	//deselect all
	for (size_t i = 0; i < rectangles.size(); i++)
	{
		rectangles[i].isSelected = false; // assume none
	}

	if (currColor < rectangles.size())
	{
		// select current color
		rectangles[currColor].isSelected = true;
	}

	//-

	if (currColor < colors_STRUCT.size())
	{
		ofColor c = colors_STRUCT[currColor].color;
		color_BACK = ofColor(c);

		ofLogNotice(__FUNCTION__) << "currColor is [" << currColor << "]";

		currName = colors_STRUCT[currColor].name;
		ofLogNotice(__FUNCTION__) << "currName is [" << currName << "]";

		currColor_OriginalPos = colors_STRUCT[currColor].position;
		ofLogNotice(__FUNCTION__) << "originalPos was [" << currColor_OriginalPos << "]";
	}

#endif
}

//--------------------------------------------------------------
void ofxColorsBrowser::mouseReleased(ofMouseEventArgs &eventArgs)
{
#ifdef USE_OFX_COLOR_BROWSER_INTERFACE
	if (ENABLE_clicks)
	{
		const int &x = eventArgs.x;
		const int &y = eventArgs.y;
		const int &button = eventArgs.button;
		//ofLogNotice(__FUNCTION__) << "mouseReleased " <<  x << ", " << y << ", " << button;

		draggingRectPtr = nullptr;
		isSelecting = false;
	}

#endif
}

//--------------------------------------------------------------
void ofxColorsBrowser::addMouseListeners()
{
#ifdef USE_OFX_COLOR_BROWSER_INTERFACE
	ofAddListener(ofEvents().mouseDragged, this, &ofxColorsBrowser::mouseDragged);
	ofAddListener(ofEvents().mousePressed, this, &ofxColorsBrowser::mousePressed);
	ofAddListener(ofEvents().mouseReleased, this, &ofxColorsBrowser::mouseReleased);
#endif
}

//--------------------------------------------------------------
void ofxColorsBrowser::removeMouseListeners()
{
#ifdef USE_OFX_COLOR_BROWSER_INTERFACE
	ofRemoveListener(ofEvents().mouseDragged, this, &ofxColorsBrowser::mouseDragged);
	ofRemoveListener(ofEvents().mousePressed, this, &ofxColorsBrowser::mousePressed);
	ofRemoveListener(ofEvents().mouseReleased, this, &ofxColorsBrowser::mouseReleased);
#endif
}

//--------------------------------------------------------------
void ofxColorsBrowser::exit()
{
#ifdef USE_OFX_COLOR_BROWSER_KEYS
	removeKeysListeners();
#endif

#ifdef USE_OFX_COLOR_BROWSER_INTERFACE
	removeMouseListeners();

	//setEnableInterfaceClicks(false);
#endif
	ofxSurfingHelpers::saveGroup(params, path_FileSettings);
}

//--------------------------------------------------------------
ofxColorsBrowser::~ofxColorsBrowser()
{
	exit();
}

//--------------------------------------------------------------
void ofxColorsBrowser::setup_colorBACK(ofFloatColor &c)
{
	//color_BACK = c;
	color_BACK_OFAPP = &c;
}

//--------------------------------------------------------------
void ofxColorsBrowser::setPosition(glm::vec2 p)
{
	position = p;
}

//--------------------------------------------------------------
void ofxColorsBrowser::setBoxSize(float _size)
{
	boxSize = _size;
}

//--------------------------------------------------------------
void ofxColorsBrowser::setRowsSize(int rows)
{
	perRow = rows;
}

//--------------------------------------------------------------
void ofxColorsBrowser::switch_palette_Type()
{
	LibraryColors_Index = (LibraryColors_Index + 1) % 2;
	ofLogNotice(__FUNCTION__) << LibraryColors_Index;

	clearInterface();

	buildColors();

	grid_create_boxes();
}

//--------------------------------------------------------------
void ofxColorsBrowser::nextSortType()
{
	MODE_SORTING++;
}

//--------------------------------------------------------------
void ofxColorsBrowser::set_palette_Type(int p)
{
	LibraryColors_Index = p;

	clearInterface();
	buildColors();
	grid_create_boxes();
}

//--------------------------------------------------------------
void ofxColorsBrowser::set_sorted_Type(int p)
{
	MODE_SORTING = p;

	switch (MODE_SORTING)
	{
	case 0:
		ofSort(colors_STRUCT, comparePosition);
		break;
	case 1:
		ofSort(colors_STRUCT, compareName);
		break;
	case 2:
		ofSort(colors_STRUCT, compareHue);
		break;
	case 3:
		ofSort(colors_STRUCT, compareBrightness);
		break;
	case 4:
		ofSort(colors_STRUCT, compareSaturation);
		break;
	}

	if (p >= 0 && p <= 4)
	{
		clearInterface();
		grid_create_boxes();
	}
}

//--------------------------------------------------------------
void ofxColorsBrowser::rectangles_update()
{
#ifdef USE_OFX_COLOR_BROWSER_INTERFACE

	if (ENABLE_clicks)
	{
		ofPoint mouse(ofGetMouseX(), ofGetMouseY());


		bool foundIsOver = false;
		bool hasFirstSelection = false;

		if (draggingRectPtr == NULL)
		{
			selectedRects.clear();
		}

		for (size_t i = 0; i < rectangles.size(); ++i)
		{
			// if we are selecting, re-evaluate this each time
			if (isSelecting)
			{
				rectangles[i].isSelected = rectangles[i].intersects(selectionRect);
			}

			// grow the slection box
			if (rectangles[i].isSelected)
			{
				if (draggingRectPtr == NULL)
				{
					if (!hasFirstSelection)
					{
						selectedRectsBoundingBox = rectangles[i];
						hasFirstSelection = true;
					}
					else
					{
						selectedRectsBoundingBox.growToInclude(rectangles[i]);
					}

					selectedRects.push_back(&rectangles[i]);
					hasFirstSelection = true;
				}
			}

			// check is over -- only set isOver if other things aren't happening
			if (!foundIsOver &&
				/*selectedRects.empty() &&
				!rectangles[i].isSelected && */
				(draggingRectPtr == NULL ||
					draggingRectPtr == &rectangles[i] ||
					draggingRectPtr == &selectedRectsBoundingBox) &&
				rectangles[i].inside(mouse))
			{
				rectangles[i].isOver = true;
				foundIsOver = true;
			}
			else
			{
				rectangles[i].isOver = false;
			}
		}

		if (isSelecting)
		{
			selectionRect.set(glm::vec3(dragStart.x, dragStart.y, 0), mouse);
		}
	}

#endif
}

//--------------------------------------------------------------
void ofxColorsBrowser::rectangles_draw()
{
#ifdef USE_OFX_COLOR_BROWSER_INTERFACE
	// 0. debug rectangles manager

	//ofPoint mouse(ofGetMouseX(), ofGetMouseY());

	//if (bShowDebug)
	//{
	//	ofFill();
	//	ofSetColor(255, showKeyboardCommands ? 255 : 127);
	//	ofDrawBitmapString(showKeyboardCommands ? keyboardCommands : "Press (Spacebar) for help.", 12, 16);
	//}

	//--

	// CARD MODE:

	// 1. draw card rectangles

	if (ENABLE_oneCard_MODE)
	{
		int padding = 15;
		int labelSize = 25;

		int letterPad = 15;
		float fontSize = 12;
		float fontPad = 5;

		float x;
		float y;

		ofColor colorBgCards = ofColor(250);

		if (colors_STRUCT.size() > 0)
		{
			ofPushStyle();

			// 1. white background

			ofSetColor(colorBgCards);

			ofDrawRectangle(
				glm::vec2(
					cardPos.x - padding,
					cardPos.y - padding),
					(cardColor_size + cardColor_pad) * (cardSize)+padding,
				(cardColor_size + cardColor_pad) + 2 * padding + labelSize
			);

			//ofDrawRectRounded(
			//    glm::vec2(
			//        cardPos.x - padding,
			//        cardPos.y - padding),
			//    (cardColor_size + cardColor_pad) * (cardSize) + 2 * padding,
			//    (cardColor_size * 1.1 + cardColor_pad) + 2 * padding + 25,
			//    5,
			//    5,
			//    5,
			//    5
			//);

			//-

			// 2. each color in card

			int colorBegin = cardSize * cardNum;
			int colorEnd = colorBegin + cardSize;

			for (int i = 0; i < cardSize; i++)
			{
				int iPad = i + colorBegin;
				ofSetColor(colors_STRUCT[iPad].color);
				ofFill();

				// 2.1 color box

				//ofDrawRectangle(
				//    cardPos.x + i * (cardColor_size + cardColor_pad),
				//    cardPos.y,
				//    cardColor_size,
				//    cardColor_size);

				ofDrawRectRounded(
					glm::vec2(
						cardPos.x + i * (cardColor_size + cardColor_pad),
						cardPos.y),
					cardColor_size,
					cardColor_size,
					2.5,
					2.5,
					0.0,
					0.0
				);

				//ofDrawRectangle(
				//    cardPos.x + i * (cardColor_size + cardColor_pad),
				//    cardPos.y,
				//    cardColor_size,
				//    cardColor_size*1.1);

				//-

				// 2.2. background text box

				// 2.2.1 text background
				ofSetColor(255);//white
				ofDrawRectangle(
					glm::vec2(
						cardPos.x + i * (cardColor_size + cardColor_pad),
						cardPos.y + cardColor_size),
					cardColor_size,
					50
				);

				//-

				// 2.2.2 text font

				// A.
				//x = cardPos.x + i * (cardColor_size + cardColor_pad) + 4;
				//y = cardPos.y + cardColor_size - 6;

				// B.
				x = cardPos.x + i * (cardColor_size + cardColor_pad) + 3;
				y = cardPos.y + cardColor_size + letterPad;

				// A.
				//ofDrawBitmapStringHighlight(
				//    colors_STRUCT[iPad].name,
				//    x,
				//    y);

				// B.
				//ofColor c = colors_STRUCT[iPad].color;
				ofColor c = 0;//black
				ofSetColor(c);

				std::string str;
				if (LibraryColors_Index == OFX_PANTONE_COLORS)
					str += "PANTONE\n";
				str += colors_STRUCT[iPad].name;

				font.drawMultiLine(
					ofToUpper(str),
					fontSize,
					x + fontPad,
					y
				);

			}

			ofPopStyle();
		}
	}

	//--

	// 1.2 draw all of our rectangles system

	//else if (!ENABLE_oneCard_MODE)
	{
		for (size_t i = 0; i < rectangles.size(); ++i)
		{
			ofRectangle *rect = (ofRectangle *)&rectangles[i];
			unsigned int selectionIndex = ofFind(selectedRects, rect);
			rectangles[i].draw(i, selectionIndex == selectedRects.size() ? -1 : selectionIndex);
		}
	}

	//--

	// rectangles management ?

	// 2. draw border on color
	// draw our bounding box rectangle
	if (!isSelecting && !selectedRects.empty())
	{
		ofNoFill();
		ofSetColor(0, 255);//full black
		ofDrawRectangle(selectedRectsBoundingBox);
	}

	// 3. draw border on selected color box
	if (isSelecting && bShowDebug)
	{
		ofNoFill();
		ofSetColor(ofColor(ofColor::black, 200));
		ofDrawRectangle(selectionRect);
	}

	//--

	// 4. rectangles management debug
	if (bShowDebug)
	{
		stringstream ss;
		ss << "Keyboard [(Spacebar) to hide]" << endl;
		ss << "W: sort by absolute width" << endl;
		ss << "A: sort by area" << endl;
		ss << "H: sort by absolute height" << endl;
		ss << "c: cascade" << endl;
		ss << "v: align vertical with current vAlign" << endl;
		ss << "h: align horizontal with current hAlign" << endl;
		ss << "x: distribute horizontal with current hAlign" << endl;
		ss << "h: distribute vertical with current vAlign" << endl;
		ss << "p: pack rectangles" << endl;
		keyboardCommands = ss.str();

		std::string hAlignString = "";
		switch (hAlign)
		{
		case OF_ALIGN_HORZ_LEFT:
			hAlignString = "OF_ALIGN_HORZ_LEFT";
			break;
		case OF_ALIGN_HORZ_CENTER:
			hAlignString = "OF_ALIGN_HORZ_CENTER";
			break;
		case OF_ALIGN_HORZ_RIGHT:
			hAlignString = "OF_ALIGN_HORZ_RIGHT";
			break;
		case OF_ALIGN_HORZ_IGNORE:
			hAlignString = "OF_ALIGN_HORZ_IGNORE";
			break;
		default:
			hAlignString = "??";
			break;
		}

		std::string vAlignString = "";
		switch (vAlign)
		{
		case OF_ALIGN_VERT_TOP:
			vAlignString = "OF_ALIGN_VERT_TOP";
			break;
		case OF_ALIGN_VERT_CENTER:
			vAlignString = "OF_ALIGN_VERT_CENTER";
			break;
		case OF_ALIGN_VERT_BOTTOM:
			vAlignString = "OF_ALIGN_VERT_BOTTOM";
			break;
		case OF_ALIGN_VERT_IGNORE:
			vAlignString = "OF_ALIGN_VERT_IGNORE";
			break;
		default:
			vAlignString = "??";
			break;
		}

		ofFill();
		ofSetColor(255);
		ofDrawBitmapString("Press (a) to toggle selection hAlign : " + hAlignString, 10, ofGetHeight() - 24);
		ofDrawBitmapString("Press (A) to toggle selection vAlign : " + vAlignString, 10, ofGetHeight() - 10);
	}

	ofNoFill();
	ofSetColor(255, 255, 0);

	for (int i = 0; i < packedRects.size(); i++)
	{
		ofDrawRectangle(packedRects[i]);
	}

	//-
#endif
}

//--------------------------------------------------------------
int ofxColorsBrowser::getSize() {
	ofLogNotice(__FUNCTION__) << colors_STRUCT.size();
	return colors_STRUCT.size();
}

//--------------------------------------------------------------
vector<ofColor> ofxColorsBrowser::getPalette()
{
	ofLogNotice(__FUNCTION__);

	int numColors = colors_STRUCT.size();
	ofLogNotice(__FUNCTION__) << "numColors:" << numColors;

	vector<ofColor> p;
	p.resize(numColors);

	for (int i = 0; i < colors_STRUCT.size(); i++)
	{
		p[i] = colors_STRUCT[i].color;
		ofLogVerbose(__FUNCTION__) << "color [" + ofToString(i) + "]  " + ofToString(p[i]);
	}

	return p;
}

//--------------------------------------------------------------
vector<std::string> ofxColorsBrowser::getNames()
{
	ofLogNotice(__FUNCTION__);

	int numColors = colors_STRUCT.size();
	ofLogNotice(__FUNCTION__) << "numColors:" << numColors;

	vector<std::string> n;
	n.resize(numColors);

	for (int i = 0; i < colors_STRUCT.size(); i++)
	{
		n[i] = colors_STRUCT[i].name;
		ofLogVerbose(__FUNCTION__) << "color [" + ofToString(i) + "]  " + ofToString(n[i]);
	}

	return n;
}

//--------------------------------------------------------------
void ofxColorsBrowser::setVisible(bool b)
{
	ofLogNotice(__FUNCTION__) << "SHOW_Gui: " << SHOW_Gui;

	SHOW_Gui = b;

	SHOW_InterfaceColors = b;

	setEnableInterfaceClicks(b);
}

//--------------------------------------------------------------
void ofxColorsBrowser::setVisibleDebug(bool b)
{
	SHOW_debugText = b;
}
//--------------------------------------------------------------
void ofxColorsBrowser::setToggleVisibleDebug()
{
	SHOW_debugText = !SHOW_debugText;
}
