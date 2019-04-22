#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0, 255, 255);
	origenX = 100;
	origenY = 1;
	//ofSetRectMode(OF_RECTMODE_CENTER);
	y = 0;
	ofSetBackgroundAuto(false);
	ofSetFrameRate(24);
	bSquare = false;
}

//--------------------------------------------------------------
void ofApp::update(){

	alfa = ofRandom(127, 255);
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	//ofBackground(color1*2,0,0);
	
	for (int i = 0; i < 10; i++) {
		x = ofRandom(-30, 30);
		color1 = ofRandom(0, 255);
		ofSetColor(color1, color1 / 2, 100, alfa);
		pos = ofPoint(origenX + (60*i), origenY + y);
		if (bSquare == true) {
			ofRect(pos, 50, 50);
		}
		else {
			ofCircle(pos, 50);
		}
	}
	y = y + 1;
	// condicional "if"
	if (y >= 400) {
		y = 0;
		ofBackground(200, 9, color1);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 't') {
		bSquare = true;
	}
	if (key == 'f') {
		bSquare = false;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
