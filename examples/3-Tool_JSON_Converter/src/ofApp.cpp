#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(25);

	ttf.load(OF_TTF_MONO, 8, true, true, true);

	//-

	// uncomment ont of the converters:

	//batch1();
	//batch2();
	//batch3();
	batch4();

	ofLogNotice(__FUNCTION__) << text;
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(255);
	text = ss.str();
	ttf.drawString(text, 20, 20);
}

//--------------------------------------------------------------
void ofApp::batch1() {

	// Taken from https://github.com/Jam3/nice-color-palettes
	// A JSON of the top color palettes on ColourLovers.com, as RGB hex strings.
	/*
	[["#69d2e7","#a7dbd8","#e0e4cc","#f38630","#fa6900"],["#fe4365","#fc9d9a","#f9cdad","#c8c8a9","#83af9b"],["#ecd078","#
	*/
	{
		// set the source file to convert
		ofFile file("1000.json");
		//ofFile file("100.json");

		string kitName = "FACTORY KIT";
		string path = "OUTPUT/" + kitName; // set kit folder name
		string filenameRoot = "Palette"; // set palette names root

		static bool bDebug = true;

		if (file.exists())
		{
			file >> js;

			if (bDebug)
			{
				ss.clear();
				ss << "IMPORTING JSON FILE PALETTES" << endl;
				ss << "to path: " << path << endl;
				ss << "with name: " << filenameRoot << "_xx" << endl << endl;
				ss << js << endl << endl;
			}

			int ip = 0;
			for (auto & jsPalette : js)
			{
				ip++;
				if (bDebug) {
					ss << "------------------------" << endl;
					ss << "palette #" << ip << endl;
				}

				if (!jsPalette.empty())
				{
					paletteType p;

					ofJson jp;

					int ic = 0;
					for (auto & color : jsPalette)
					{
						ofColor myColor;
						string sColor = color;
						ofStringReplace(sColor, "#", ""); // remove "#"

						hexToColor(myColor, sColor);
						if (bDebug) {
							ss << "color #" << ic++ << " " << sColor << endl;
						}

						p.palette.push_back(myColor);

						ofJson jc;
						jc["r"] = myColor.r;
						jc["g"] = myColor.g;
						jc["b"] = myColor.b;
						jc["a"] = myColor.a;
						jp.push_back(jc);
					}

					kit.palettes.push_back(p);

					string suffix = "";
					//suffix += ofToString(ip < 1000 ? "0" : "");
					suffix += ofToString(ip < 100 ? "0" : "");
					suffix += ofToString(ip < 10 ? "0" : "");
					suffix += ofToString(ip);

					string filename = filenameRoot + "_" + suffix + ".json";

					// save json
					ofSavePrettyJson(path + "/" + filename, jp);
				}
			}

			ss << "Total processed palettes: " << ip << endl;
		}
	}
}

//--------------------------------------------------------------
void ofApp::batch2() {

	// Taken from
	// https://github.com/sravanrekandar/google-material-color-palette-json
	/*
	{
	  "red": {
		"shade_50": "#ffebee",
		"shade_100": "#ffcdd2",
		"shade_200": "#ef9a9a",
		"shade_300": "#e57373",
		"shade_400": "#ef5350",
		"shade_500": "#f44336",
		"shade_600": "#e53935",
		"shade_700": "#d32f2f",
		"shade_800": "#c62828",
		"shade_900": "#b71c1c",
		"shade_A100": "#ff8a80",
		"shade_A200": "#ff5252",
		"shade_A400": "#ff1744",
		"shade_A700": "#d50000"
	  },
	  "pink": {
		"shade_50": "#fce4ec",
		"shade_100": "#f8bbd0",
	*/

	// TODO:
	// https://github.com/frizeiro/material-colors
	// https://github.com/frizeiro/material-colors/blob/master/base/material-colors.txt
	// another alternative to make user palettes like:
	//    "name": "Teal",
	/*
	"weight": "300",
		"color" : "#4DB6AC",
		"light" : "#82E9DE",
		"dark" : "#00867D",
		"text" : "#000000"
	*/
	{
		ofFile file("palette.json");

		string kitName = "GOOGLE MATERIAL 2";
		string path = "OUTPUT/" + kitName; // set kit folder name
		string filenameRoot = "Palette"; // set palette names root

		static bool bDebug = true;

		if (file.exists())
		{
			file >> js;

			if (bDebug)
			{
				ss.clear();
				ss << "IMPORTING JSON FILE PALETTES" << endl;
				ss << "to path: " << path << endl;
				ss << "with name: " << filenameRoot << "_xx" << endl << endl;
				ss << js << endl << endl;
			}

			//--

			// 2.1 read and build kit data

			int ip = 0;
			for (ofJson & jsPalette : js)
			{
				ip++;
				if (bDebug) {
					ss << "----------------------------------------------------------------------" << endl;
					ss << "palette #" << ip << endl;
				}

				if (!jsPalette.empty())
				{
					paletteType p;

					int ic = 0;

					for (ofJson & color : jsPalette)
					{
						if (!color.empty())
						{
							//TODO:
							//this is an nugly workaround that can not get the name..

							string hex = color;
							ofStringReplace(hex, "#", ""); // remove "#"

							ofColor myColor;
							hexToColor(myColor, hex);
							p.palette.push_back(myColor);
						}
					}

					kit.palettes.push_back(p);
				}
			}

			ss << "Total processed palettes: " << ip << endl;

			//--

			// 2.2 save each palette json

			int ipj = 0;

			for (int p = 0; p < kit.palettes.size(); p++)
			{
				if (p != 1 && p != 19) // exclude
				{
					ipj++;
					ofJson jp; // palette

					for (int c = 0; c < kit.palettes[p].palette.size(); c++)
					{
						ofColor myColor = kit.palettes[p].palette[c];

						ofJson jc; // color
						jc["r"] = myColor.r;
						jc["g"] = myColor.g;
						jc["b"] = myColor.b;
						jc["a"] = myColor.a;
						jp.push_back(jc);
					}

					//--

					string suffix = "";
					//suffix += ofToString(ipj < 1000 ? "0" : "");
					//suffix += ofToString(ipj < 100 ? "0" : "");
					suffix += ofToString(ipj < 10 ? "0" : "");
					suffix += ofToString(ipj);

					string filename = filenameRoot + "_" + suffix + ".json";

					// save json
					ofSavePrettyJson(path + "/" + filename, jp);
				}
			}
		}
	}

}

//--------------------------------------------------------------
void ofApp::batch3() {

	// This part is to generate palettes. But also can be used to
	// get colors to make a new library like the pantone lib included in Paletto.
	// This is a more manual converter, not using json methods but "tricky" manual string manipulations.
	// Taken from
	// https://github.com/buschtoens/material-colors-json
	/*
	{
		"red-50": "#ffebee",
		"red-100": "#ffcdd2",
		"red-200": "#ef9a9a",
		"red-300": "#e57373",
		"red-400": "#ef5350",
		"red-500": "#f44336",
		"red-600": "#e53935",
		"red-700": "#d32f2f",
		"red-800": "#c62828",
		"red-900": "#b71c1c",
		"red-a100": "#ff8a80",
		"red-a200": "#ff5252",
		"red-a400": "#ff1744",
		"red-a700": "#d50000",

		"pink-50": "#fce4ec",
		"pink-100": "#f8bbd0",
		"pink-200": "#f48fb1",
		"pink-300": "#f06292",
		"pink-400": "#ec407a",
		"pink-500": "#e91e63",
		"pink-600": "#d81b60",
		"pink-700": "#c2185b",
		..
	*/
	{
		ofFile file("colors.json");

		string kitName = "GOOGLE MATERIAL";
		string path = "OUTPUT/" + kitName; // set kit folder name
		string filenameRoot = "Palette"; // set palette names root

		string slog;

		static bool bDebug = true;

		ofBuffer buffer(file);

		vector<string> names;
		bool bNew = false;
		int amtPalettes = 0;
		string namePal;

		// clear
		paletteType p;
		kit.palettes.clear();

		//int numLine = 0;

		bNew = true;
		slog += "-> New \n"; // -> new palette starts here

		// Read file line by line
		for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it)
		{
			//numLine++;

			string line = *it;
			vector<string> words = ofSplitString(line, ":");

			if (line == "  ")
			{
				bNew = true;
				slog += "-> New \n"; // -> new palette starts here
			}

			if (line == "{" || line == "}" || line == "" || line == " " || words.size() != 2) continue; // skip format or line

			//if (/*line == "{" ||*/ numLine == 1)
			//{
			//	bNew = true;
			//	slog += "--\n"; // -> new palette starts here
			//	continue;
			//}

			//--

			string name, hex;

			// hex
			hex = words[1];
			bool bvalid = (ofStringTimesInString(hex, "#") == 1);
			if (!bvalid) continue; // skip line if no # fountd
			ofStringReplace(hex, "\"", "");
			ofStringReplace(hex, "#", "");
			ofStringReplace(hex, ",", "");
			ofStringReplace(hex, " ", "");

			// name
			name = words[0];
			ofStringReplace(name, " ", "");
			ofStringReplace(name, "\"", "");
			if (name == "white" || name == "black") continue; // skip these colors..

			//--

			// a palette starts here
			if (bNew)
			{
				bNew = false;

				// queue last loop iteration
				if (amtPalettes != 0) kit.palettes.push_back(p);

				//-

				p.palette.clear();

				// queue palette name
				amtPalettes++;
				int amt = ofStringTimesInString(name, "-");
				auto snames = ofSplitString(name, "-");
				if (amt == 1) {
					namePal = (snames[0]);
					names.push_back(namePal);
				}
				else if (amt == 2) {
					namePal = (snames[0] + "-" + snames[1]);
					names.push_back(namePal);
				}

			}

			// log
			string stab = "     \t";
			string str = "name:" + name + stab + " hex:" + hex + "\n";
			slog += str;

			//-

			// queue color to current palette
			ofColor myColor;
			hexToColor(myColor, hex);
			p.palette.push_back(myColor);

			//-

			////TODO:
			//// queue
			//if (amtPalettes == 1) kit.palettes.push_back(p);
		}


		//--

		// kit is filled with all the palettes

		// 2.2 save each palette to his json

		for (int p = 0; p < kit.palettes.size() && p < names.size(); p++)
		{
			ofJson jp; // palette

			for (int c = 0; c < kit.palettes[p].palette.size(); c++)
			{
				ofColor myColor = kit.palettes[p].palette[c];

				ofJson jc; // color
				jc["r"] = myColor.r;
				jc["g"] = myColor.g;
				jc["b"] = myColor.b;
				jc["a"] = myColor.a;
				jp.push_back(jc);
			}

			//--

			int i = p + 1;
			string suffix = "";
			//suffix += ofToString(i < 1000 ? "0" : "");
			//suffix += ofToString(i < 100 ? "0" : "");
			suffix += ofToString(i < 10 ? "0" : "");
			suffix += ofToString(i);

			string filename = /*filenameRoot + "_" +*/ suffix + "_" + ofToUpper(names[p]) + ".json";

			// save json
			ofSavePrettyJson(path + "/" + filename, jp);
		}

		//--

		slog += "\n names:" + ofToString(names) + "\n";
		cout << slog << endl;
		//text = slog;

		ss << slog.c_str();
	}
}

//--------------------------------------------------------------
void ofApp::batch4() {

	// Taken from https://github.com/mattdesl/dictionary-of-colour-combinations
	// A JSON dataset of 348 colour combinations (of 2, 3, and 4 colours) of 159 unique colours, from the book "A Dictionary of Colour Combinations" 
	// compiled by Sanzo Wada (1883 – 1967) and published by Seigensha Art.
	// I noticed than the converted colors looked a bit different than the https://github.com/dblodorn/sanzo-wada/ version.
	// I should verify if something if wrong or if look more similar using CMYK or RGB instead of the used hex.

	/*
	[
		{
		"name": "Hermosa Pink",
		"combinations": [
			176,
			227,
			273
		],
		"swatch": 0,
		"cmyk": [
			0,
			30,
			6,
			0
		],
		"lab": [
			83.42717631799802,
			22.136186770428026,
			1.6381322957198563
		],
		"rgb": [
			249,
			193,
			206
		],
		"hex": "#f9c1ce"
		},

		{
		"name": "Corinthian Pink",
		"combinations": [
			27,
			43,
			254,
			264,
			342
		],
		"swatch": 0,
		"cmyk": [
			0,
			35,
			15,
			0
		],
		"rgb": [
			248,
			182,
		..
		*/
	{
		// set the source file to convert
		ofFile file("colorsDict.json");

		string kitName = "SANZOWADA DICTIONARY"; // to name the container folder
		string path = "OUTPUT/" + kitName; // set kit folder name
		string filenameRoot = "SANZOWADA"; // set palette names root

		vector<colorType> colorsKit;
		vector<combinationType> palettesKit;
		//int icombinations = 0;
		vector<int> icombinations;

		static bool bDebug = true;

		if (file.exists())
		{
			// 1. read json file content:

			file >> js;

			if (bDebug)
			{
				ss.clear();
				ss << "IMPORTING JSON FILE PALETTES" << endl;
				ss << "to path: " << path << endl;
				ss << "with name: " << filenameRoot << "_xx" << endl << endl;
				ss << js << endl << endl;
			}

			int ip = 0;
			for (auto & jc : js) // iterate each color
			{
				ip++;
				if (bDebug) {
					ss << "----------------------------------------------" << endl;
					ss << "Color #" << ip << endl;
				}

				if (!jc.empty())
				{
					//ss << j << endl << endl; // log json item

					string name = jc["name"];
					string hex = jc["hex"];
					vector<int> _colCombinations = jc["combinations"]; // get all the combinations where a color appears
					ss << "Name:" << name << " hex:" << hex << endl;

					for (auto & comb : _colCombinations) // add combination only if not queued
					{
						bool bFound = false;
						if (icombinations.size() == 0) bFound = false; // force queue add
						else
							for (auto & c : icombinations)
							{
								if (comb == c)
								{
									bFound = true;
								}
							}

						// only push for counting purposes 
						if (!bFound) icombinations.push_back(comb); // skip push if already present
					}

					//-

					if (bDebug)
					{
						ss << "Combinations:";
						for (int i = 0; i < _colCombinations.size(); i++)
						{
							ss << _colCombinations[i] << (i != _colCombinations.size() - 1 ? ", " : "");
						}
						ss << endl << endl;
					}

					//cout << ss;
					//ss.clear();

					//-

					// we queue each iterated color (all)

					colorType c;
					c.name = name;
					c.combinations = _colCombinations;

					ofColor _color;
					ofStringReplace(hex, "#", ""); // remove "#"
					hexToColor(_color, hex);
					c.color = _color;

					colorsKit.push_back(c);
				}
			}

			//---

			if (bDebug)
			{
				// we should count 348 colour combinations accumulated
				ss << endl << "Combinations amount:" << icombinations.size() << endl;

				ss << endl << "List Combinations:" << endl;
				for (int _i = 0; _i < icombinations.size(); _i++)
				{
					ss << icombinations[_i] << ", ";
				}
				ss << endl << endl;
				cout << ss;

				std::sort(icombinations.begin(), icombinations.end());
				cout << "Sorted Combinations:" << endl;
				for (auto x : icombinations)
					cout << x << ", ";

				//ss.clear();
			}

			//---

			// 2. build palettes kit:

			// we will generate a palette for each combination re-using all the referenced colors

			for (int p = 0; p < icombinations.size(); p++)
			{
				if (bDebug)
				{
					ss << endl << "Combinations #" << p << endl;
				}

				ofJson jp; // palette

				bool bvalid = false;

				//-

				// 1. iterate each color
				// 2. if the color has the combination listed inside his data, push the color to current palette

				for (int i = 0; i < colorsKit.size(); i++) // all colors
				{
					//int ii = i + 1;

					colorType c = colorsKit[i];

					// notice that we cant use c.combinations[ic] 

					for (int ic = 0; ic < c.combinations.size(); ic++) // iterate listed combinations in the color
					{
						int colcomb = c.combinations[ic];

						// pseudocode:
						// is the color on current i combination?

						// combination is listed in the color
						if (p + 1 == colcomb) // color pos is equals to listed colors con combinator
						{
							ofColor col = c.color;
							ofJson jc; // color
							jc["r"] = col.r;
							jc["g"] = col.g;
							jc["b"] = col.b;
							jc["a"] = col.a;
							jp.push_back(jc); // push then color to palette
							bvalid = true; // almost one color included..
						}
					}
				}

				//--

				// store json if valid (= there's one or more colors)
				if (bvalid)
				{
					string suffix = "";
					int ip = p + 1;
					//suffix += ofToString(ip < 1000 ? "0" : "");
					suffix += ofToString(ip < 100 ? "0" : "");
					suffix += ofToString(ip < 10 ? "0" : "");
					suffix += ofToString(ip);

					// save json
					string filename = filenameRoot + "_" + suffix + ".json";
					ofSavePrettyJson(path + "/" + filename, jp);
				}
			}
		}
	}
}