/**
 *
	creating 3d visuals from image sequences
 */

#pragma once

#include "ofMain.h"
#include "ofxImageSequenceRecorder.h"
#include "ofxMaxim.h"
#include "maxiMFCC.h"
#include "ofxPostGlitch.h"
#include "ofxOsc.h"

#define OSC_IN_PORT 6666

class ofApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
	ofEasyCam cam; // add mouse controls for camera movement
	float extrusionAmount;
	ofVboMesh mesh;
	ofVideoGrabber vidGrabber;
	
	// ------ ------ ------ ------ ------ ------ | general stuff |
	int h, w, t;
	int ih, iw, t0, t1;
	float tt;

	// ------ ------ ------ ------ ------ ------ | rec stuff |
	ofxImageSequenceRecorder recorder;
	bool isRec;
	ofImage tImg;

	// ------ ------ ------ ------ ------ ------ | images stuff |
	vector<ofImage> seq_a;
	int nImgs;
	ofFbo fbo_a;
	ofDirectory dir;
	ofPixels pixs;
	ofColor cc;
	int i_1, i_2;
	int fp;
	int scale, npm_y, npm_x;
};
