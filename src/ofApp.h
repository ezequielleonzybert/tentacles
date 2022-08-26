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
	std::vector<float> rotations;
	std::vector<int> alphas;
	std::vector<float> scalar;
	std::vector<ofMatrix4x4> matrix;
	vector<int> counter;
	int c;
	ofCamera camera;

	VideoRecorder recorder;
};
