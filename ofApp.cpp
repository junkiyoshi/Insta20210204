#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
	ofSetCircleResolution(72);

	this->number_of_targets = 40;
	for (int i = 0; i < this->number_of_targets; i++) {

		this->target_list.push_back(glm::vec2());
		this->color_list.push_back(glm::vec3(1, 1, 1));
	}

	this->shader.load("shader/shader.vert", "shader/shader.frag");
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);


}

//--------------------------------------------------------------
void ofApp::draw() {

	auto noise_seed_deg = ofRandom(1000);
	auto noise_seed_radius = ofRandom(1000);

	int index = 0;
	for (int deg = 0; deg < 360; deg += 18) {

		vector<glm::vec2> vertices;
		for (int i = 0; i < 50; i++) {

			auto noise_deg = deg + ofMap(ofNoise(noise_seed_deg, (ofGetFrameNum() + i) * 0.005), 0, 1, -360, 360);
			auto noise_radius = ofMap(ofNoise(noise_seed_radius, (ofGetFrameNum() + i) * 0.005), 0, 1, -350, 350);
			vertices.push_back(glm::vec2(360 + noise_radius * cos(noise_deg * DEG_TO_RAD), 360 + noise_radius * sin(noise_deg * DEG_TO_RAD)));
		}

		this->target_list[index++] = vertices.back();
		this->target_list[index++] = vertices.front();

		ofNoFill();
		ofBeginShape();
		ofVertices(vertices);
		ofEndShape();
	}

	ofFill();
	this->shader.begin();
	this->shader.setUniform1f("time", ofGetElapsedTimef());
	this->shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	this->shader.setUniform2fv("targets", &this->target_list[0].x, this->number_of_targets);
	this->shader.setUniform3fv("colors", &this->color_list[0].x, this->number_of_targets);

	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	this->shader.end();
}

//--------------------------------------------------------------
bool ofApp::calculate_angle(glm::vec2 location_1, float radius_1, glm::vec2 location_2, float radius_2, double& theta, double& a) {

	auto distance = glm::distance(location_1, location_2);
	if (distance > radius_1 + radius_2 || distance < radius_1 - radius_2) { return false; }

	theta = atan2(location_2.y - location_1.y, location_2.x - location_1.x);
	auto cos_a = (pow(distance, 2) + pow(radius_1, 2) - pow(radius_2, 2)) / (2 * distance * radius_1);
	a = acos(cos_a);
	return true;
}

//--------------------------------------------------------------
int main() {

	ofGLWindowSettings settings;
	settings.setGLVersion(3, 2);
	settings.setSize(720, 720);
	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}