#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main()
{
	int n = 2;
	ofSetupOpenGL(3200/n, 1080/n, OF_WINDOW); // <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());
}
