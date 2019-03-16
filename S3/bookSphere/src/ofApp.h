#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void draw3D();

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
		
		ofxPanel gui;
		ofxGuiGroup kaleidoGroup;
		ofxToggle kenabled;
		ofxIntSlider ksectors;
		ofxFloatSlider kangle, kx, ky;

		ofxGuiGroup mixerGroup;
		ofxFloatSlider show2D, show3D;
		ofxFloatSlider rad, deform, deformFreq, extrude;
		ofxFloatSlider tFactor;

		ofVideoPlayer video01;
		ofImage img1, img2;
		ofShader shader;

		ofSpherePrimitive sphere;
		ofEasyCam eCam;
		ofLight light;
		ofMaterial material;
		ofFbo fbo1, fbo2, fbo3D;
		vector<ofPoint> vertices0;
		int w, h, t;

		bool showGui;
};
