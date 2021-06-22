#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(25);

	ttf.load(OF_TTF_SERIF, 10, true, true, true);

	//--

	// 1.
	// Taken from https://github.com/Jam3/nice-color-palettes
	// A JSON of the top color palettes on ColourLovers.com, as RGB hex strings.
	if (0)
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

	//--

	// 2.
	// Taken from
	// https://github.com/sravanrekandar/google-material-color-palette-json
	if (0)
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

	//--

	// 3.
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

	if (1)
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
		text = slog;

		cout << slog << endl;
	}
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(255);
	text = ss.str();
	ttf.drawString(text, 20, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
}
