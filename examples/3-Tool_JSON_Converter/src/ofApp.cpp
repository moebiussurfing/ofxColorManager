#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(25);

	ttf.load(OF_TTF_SERIF, 10, true, true, true);

	//--

	// 1.
	if (0)
	{
		// Taken from https://github.com/Jam3/nice-color-palettes
		// A JSON of the top color palettes on ColourLovers.com, as RGB hex strings.

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
	if (1)
	{
		// Taken from
		// https://github.com/sravanrekandar/google-material-color-palette-json

		ofFile file("palette.json");

		string kitName = "GOOGLE MATERIAL";
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

	//for (int p = 0; p < kit.palettes.size(); p++) {
	//	cout << "------------------------" << endl;
	//	cout << "palette #" << p << endl;
	//	for (int c = 0; c < kit.palettes[p].palette.size(); c++) {
	//		cout << "color #" << c << " " << kit.palettes[p].palette[c] << endl;
	//	}
	//}

	//    ofJson j;
	//    vector<ofColor> p = kit.palettes[p].palette;
	//    to_json(j, p);
	//    ofSavePrettyJson ("myPalette", j);
}
