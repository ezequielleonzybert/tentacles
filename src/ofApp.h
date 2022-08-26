#pragma once

#include "ofMain.h"
#include "videoRecorder.h"

class ofApp : public ofBaseApp
{

public:
	void setup();
	void update();
	void draw();
	void exit();

	// void keyPressed(int key);
	// void keyReleased(int key);
	// void mouseMoved(int x, int y);
	// void mouseDragged(int x, int y, int button);
	// void mousePressed(int x, int y, int button);
	// void mouseReleased(int x, int y, int button);
	// void mouseEntered(int x, int y);
	// void mouseExited(int x, int y);
	// void windowResized(int w, int h);
	// void dragEvent(ofDragInfo dragInfo);
	// void gotMessage(ofMessage msg);

	void showFps()
	{
		std::stringstream strm;
		strm << "fps: " << ofGetFrameRate() << " FrameNum: " << ofGetFrameNum();
		ofSetWindowTitle(strm.str());
	};

	int w = ofGetWidth();
	int h = ofGetHeight();
	std::vector<glm::vec3> emitter, end;
	std::vector<ofPolyline> lines;
	std::vector<float> scales;
	std::vector<int> alpha;
	std::vector<float> scalar;
	std::vector<ofMatrix4x4> matrix;
	std::vector<ofColor> color;
	vector<int> counter;
	int c;
	ofCamera camera;

	// paleta
	std::array<ofColor, 6> palette = {
		ofColor(255),
		ofColor(255, 85, 0),
		ofColor(0, 18, 66),
		ofColor(0, 0, 192),
		ofColor(0, 206, 235),
		ofColor(0)};

	VideoRecorder recorder;
};
