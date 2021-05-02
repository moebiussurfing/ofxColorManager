
#include "DEMO_SceneSpheres.h"


//--------------------------------------------------------------
void DEMO_SceneSpheres::setup() {
	ofLogNotice(__FUNCTION__);

	//ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);

	//-

	// a.
	resetCamera();
	//cam.reset();
	ofxLoadCamera(cam, path + _name);//not working

	//// b.
	//cam.setDistance(750);
	//cam.reset();//initialize

	cam.disableMouseInput();
}

//--------------------------------------------------------------
void DEMO_SceneSpheres::update() {
#define ZTW_SPEED 0.05f
	if (tweenD > 0)
	{
		tweenD -= ZTW_SPEED * DEMO5_Speed;
	}
	tweenD = ofClamp(tweenD, 0, 1);

	this->face.clear();
	this->frame.clear();

	float phi_deg_step = 8;
	float theta_deg_step = 8;
	float noise_param = 0.35;
	float threshold = 0.55;

	vector<ofColor> color_list;
	ofColor color;

	//vector<int> hex_list = { 0xef476f, 0xffd166, 0x06d6a0, 0x118ab2, 0x073b4c };
	//for (auto hex : hex_list) {
	//	color.setHex(hex);
	//	color_list.push_back(color);
	//}

	if ((palette != nullptr) && (*palette).size() > 0)
	{
		for (int i = 0; i < (*palette).size(); i++)
		{
			color.set((*palette)[i]);
			color_list.push_back(ofColor(color.r, color.g, color.b, 255 * DEMO5_Alpha));
		}
	}

	int color_index = 0;

	float _rmin = 100;
	float _rmax = 200;
	int _amnt = 4;
	if ((palette != nullptr) && (*palette).size() > 0)
	{
		_amnt = (*palette).size();
	}

	float _step = (_rmax - _rmin) / _amnt;

	for (float radius = _rmin; radius <= _rmax; radius += _step) {

		for (float phi_deg = 0; phi_deg < 360; phi_deg += phi_deg_step) {

			for (float theta_deg = theta_deg_step; theta_deg < 180; theta_deg += theta_deg_step) {

				auto noise_value = ofNoise(cos(theta_deg * DEG_TO_RAD) * noise_param + (ofGetFrameNum() + radius) * 0.015);
				if (noise_value < threshold) { continue; }

				auto noise_1 = ofNoise(cos((theta_deg - theta_deg_step) * DEG_TO_RAD) * noise_param + (ofGetFrameNum() + radius) * 0.015);
				auto noise_2 = ofNoise(cos(theta_deg * DEG_TO_RAD) * noise_param + (ofGetFrameNum() + radius) * 0.015);
				auto noise_3 = ofNoise(cos(theta_deg * DEG_TO_RAD) * noise_param + (ofGetFrameNum() + radius) * 0.015);
				auto noise_4 = ofNoise(cos((theta_deg + theta_deg_step) * DEG_TO_RAD) * noise_param + (ofGetFrameNum() + radius) * 0.015);

				auto index = this->face.getNumVertices();
				vector<glm::vec3> vertices;

				vertices.push_back(glm::vec3(
					radius * sin((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD) * cos((phi_deg + phi_deg_step * 0.5) * DEG_TO_RAD),
					radius * sin((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD) * sin((phi_deg + phi_deg_step * 0.5) * DEG_TO_RAD),
					radius * cos((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD)));
				vertices.push_back(glm::vec3(
					radius * sin((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD) * cos((phi_deg - phi_deg_step * 0.5) * DEG_TO_RAD),
					radius * sin((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD) * sin((phi_deg - phi_deg_step * 0.5) * DEG_TO_RAD),
					radius * cos((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD)));
				vertices.push_back(glm::vec3(
					radius * sin((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD) * cos((phi_deg + phi_deg_step * 0.5) * DEG_TO_RAD),
					radius * sin((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD) * sin((phi_deg + phi_deg_step * 0.5) * DEG_TO_RAD),
					radius * cos((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD)));
				vertices.push_back(glm::vec3(
					radius * sin((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD) * cos((phi_deg - phi_deg_step * 0.5) * DEG_TO_RAD),
					radius * sin((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD) * sin((phi_deg - phi_deg_step * 0.5) * DEG_TO_RAD),
					radius * cos((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD)));

				this->face.addVertices(vertices);

				this->face.addIndex(index + 0); this->face.addIndex(index + 1); this->face.addIndex(index + 3);
				this->face.addIndex(index + 0); this->face.addIndex(index + 3); this->face.addIndex(index + 2);

				for (int k = 0; k < 4; k++) {

					this->face.addColor(color_list[color_index]);
					//this->face.addColor(ofColor(color_list[color_index], 192));

					this->frame.addColor(ofColor(39));
				}

				if (noise_1 < threshold) {

					this->frame.addVertex(vertices[0]);
					this->frame.addVertex(vertices[1]);

					this->frame.addIndex(this->frame.getNumVertices() - 1);
					this->frame.addIndex(this->frame.getNumVertices() - 2);
				}

				if (noise_2 < threshold) {

					this->frame.addVertex(vertices[0]);
					this->frame.addVertex(vertices[2]);

					this->frame.addIndex(this->frame.getNumVertices() - 1);
					this->frame.addIndex(this->frame.getNumVertices() - 2);
				}

				if (noise_3 < threshold) {

					this->frame.addVertex(vertices[1]);
					this->frame.addVertex(vertices[3]);

					this->frame.addIndex(this->frame.getNumVertices() - 1);
					this->frame.addIndex(this->frame.getNumVertices() - 2);
				}

				if (noise_4 < threshold) {

					this->frame.addVertex(vertices[2]);
					this->frame.addVertex(vertices[3]);

					this->frame.addIndex(this->frame.getNumVertices() - 1);
					this->frame.addIndex(this->frame.getNumVertices() - 2);

				}
			}
		}

		color_index = (color_index + 1) % color_list.size();
	}
}

//--------------------------------------------------------------
void DEMO_SceneSpheres::draw(ofRectangle viewport)
{
#define Z_MIN 1.25//zoom
#define Z_MAX 50.0
#define T_MIN 60//speed frames
#define T_MAX 240
#define ZTW_MAX 1.2

	this->cam.begin(viewport);
	{
		float w = ofGetWidth();
		float h = ofGetHeight();

		//faded zoom
		bool bZoomEngine = false;
		if (bZoomEngine)
		{
			bool bSmooth = true;
			static float max1 = 2;
			static int timer1 = 100;
			int frame = ofGetFrameNum() % timer1;
			float s = ofMap(frame, 0, timer1, -1.0, 1.0f);
			if (ofGetFrameNum() % timer1 == 0 && !bSmooth)
			{
				timer1 = ofRandom(T_MIN, T_MAX);
			}
			ofScale(Z_MIN + max1 * abs(0.1 * glm::sin(s)));
		}

		//-

		ofPushMatrix();
		
		ofScale(1 + ZTW_MAX * tweenD * DEMO5_Zoom);
		//fboBig.draw(-w / 2, -h / 2, w, h);

		ofRotateX(ofGetFrameNum() * 0.37);
		ofRotateY(ofGetFrameNum() * 0.72);

		this->frame.draw();
		this->face.draw();

		ofPopMatrix();
	}
	this->cam.end();
}
