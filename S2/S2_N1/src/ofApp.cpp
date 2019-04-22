#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(255);
	inicioEjeX = ofPoint(100, 500);
	finEjeX = ofPoint(700, 500);
	inicioEjeY = ofPoint(100, 100);
	finEjeY = ofPoint(100, 500);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	// dibujar referencia
	ofSetColor(0);
	ofSetLineWidth(3);
	ofLine(inicioEjeX, finEjeX);
	ofLine(inicioEjeY, finEjeY);
	/*
	el bucle "for" usa tres elementos:
	1. condicion de inicio
	2. condición de paro
	3. actualización
	se usa para realizar una serie de tareas repetidamente
	*/

	// función constante (x cambia, y se mantien)
	ofSetColor(0, 0, 255);
	for (int i = 0; i < 100; i++) {
		ofCircle((i*6)+103, 300, 3);
	}

	// función identidad (x cambia, y = x)
	ofSetColor(255, 0,0);
	for (int j = 0; j < 100; j++) {
		ofCircle(((j*6)+103), ofGetHeight()-((j*6)+ 103), 3);
	}

	// funcion senoidal simple
	for (int k = 0; k < 100; k++) {
		//ofCircle((k * 6) + 103, sin(k), 3);
		// baja la gráfica con un offset
		ofSetColor(0, 64, 0);
		ofCircle((k * 6) + 103, sin(k) + (ofGetHeight()-100), 3);
		// aumentamos la amplitud
		ofSetColor(0, 127, 0);
		ofCircle((k * 6) + 103, 50 * sin(k) + (ofGetHeight() - 100), 3);
		// bajamos la frecuencia multiplicando por un factor muy pequeño (PI/25)
		ofSetColor(0, 255, 0);
		ofCircle((k * 6) + 103, 50 * sin((PI/25)*(k)) + (ofGetHeight() - 100), 3);
		ofSetColor(0, 255, 127);
		ofCircle((k * 6) + 103, 50 * sin((PI/50)*(k)) + (ofGetHeight() - 100), 3);
	}
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
