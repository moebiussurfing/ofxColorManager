#include "ofApp.h"

//A JSON of the top color palettes on ColourLovers.com, as RGB hex strings.
//https://github.com/Jam3/nice-color-palettes

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(25);

	ttf.load(OF_TTF_SERIF, 10, true, true, true);

	// set the source file to convert
	ofFile file("1000.json");
	//ofFile file("100.json");

	string kitName = "FactoryKit";
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

		ss << "Total converted palettes: " << ip << endl;
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
