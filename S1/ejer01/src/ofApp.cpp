#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	ofSetFrameRate(1);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetLineWidth(1);

	// cambiar color de relleno
	ofSetColor(255,0,0);
	ofFill();
	ofRect(100, 100, 100, 100);
	
	// dibujar solo contorno
	ofNoFill();
	ofSetColor(0, 255, 0);
	ofRect(300, 100, 100, 100);

	// dibujar contorno y relleno en colores distintos
	ofSetColor(255,255,0);
	ofFill();
	ofRect(500, 100, 100, 100);
	ofSetColor(0, 0, 255);
	ofNoFill();
	ofRect(500, 100, 100, 100);

	// cambiar grosor de contorno
	ofSetLineWidth(5);
	ofSetColor(0, 255, 255);
	ofNoFill();
	//ofRect(700, 100, 100, 100);
	ofDrawRectangle(700, 100, 100, 100);

	ofSetLineWidth(1);
	ofDrawCircle(900, 100, 50);

	// otras primitivas de dibujo
	ofDrawCircle(100, 300, 60);
	ofDrawCurve(300, 300, 350, 350, 300, 400, 400, 400);
	ofDrawEllipse(500, 300, 100, 60);
	ofDrawLine(700, 300, 600, 400);
	ofDrawRectRounded(900, 300, 100, 200, 5);
	ofDrawRectangle(900, 500, 100, 200);
	ofDrawTriangle(300, 400, 100, 200, 500, 500);
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
