#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxXmlSettings.h"
#include "ofxOsc.h"
#include "ofxPostGlitch.h"

#define OSC_IN_PORT 57110

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void draw_spiro_in(int cx, int cy, float r1, float r2, float d, float e);
		void draw_spiro_ex(int cx, int cy, float r1, float r2, float d, float e);
		void draw_star(int cx, int cy, int r1, int r2, int np);
		void keyPressed(int key);
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
		
		bool showGui;
		ofxPanel gui;
		ofxGuiGroup mindGroup;
		ofxFloatSlider med, att;

		ofxOscReceiver receiver;
		float cc1, cc2;
		int note;

		ofFbo fbo1, fbo2;
		ofxPostGlitch myGlitch;

		int h, w, t, f;
		float tt;

		int cX, cY;
		float pX, pY, nX, nY;
		float a01, a02;
		float b01, b02, b03, b04, b05;

		int pCount;
		int steps;
		float inRad, outRad;
		float outRatio, inRatio;
		float shadeRatio, rotRatio;
		float rCol;
};
