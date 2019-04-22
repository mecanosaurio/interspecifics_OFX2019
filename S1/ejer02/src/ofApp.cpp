#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(35, 65, 135);
	ofSetFrameRate(24);
	posVar = 100;
	//var2 = 1;
}

//--------------------------------------------------------------
void ofApp::update(){
	// actualizamos en nuevo valor
	posVar = posVar + 2;

	// otras actualizaciones
	var2 = 50 * sin(ofGetFrameNum() * PI/200);
	//var2 = 200 * sin(ofGetFrameNum()*PI / 100);
	//var2 = 200 * sin(ofGetFrameNum()*PI / 100);

	// en qué frame nos encontramos?
	ofGetFrameNum();

}

//--------------------------------------------------------------
void ofApp::draw(){
	// modular tamaño
	ofDrawCircle(ofGetWidth()/2, ofGetHeight()/2, var2);
	// modular posicion
	ofDrawCircle(250 + var2*5, ofGetHeight() / 2, 100);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
