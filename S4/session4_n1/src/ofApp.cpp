#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	img1.load("eye.png");
	ofSetFrameRate(30);
	ofSetBackgroundAuto(false);
	// sonido
	sonidero.loadSound("byetone.mp3");
	sonidero.setLoop(true);
	sonidero.play();
	//inicializa el espectro
	for (int i = 0; i<256; i++) {
		spectrum[i] = 0.0;
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	//actualiza info de sonido
	ofSoundUpdate();
	//Get current spectrum with N bands
	float *val = ofSoundGetSpectrum(256);
	for (int i = 0; i<256; i++) {
		spectrum[i] *= 0.97;
		spectrum[i] = max(spectrum[i], val[i]);
	}
	modSound = spectrum[2];
	// imprime el valor de modsound
	cout << modSound << endl;
}

//--------------------------------------------------------------
void ofApp::draw(){
	//img1.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	int mod;
	mod = ofGetFrameNum() % 100;
	// entintar la imagen
	float col = ofMap(x, 0, ofGetWidth(), 0, 255);
	ofSetColor(col, 0, 255-col, 200);
	// dibujar la imagen
	//img1.draw(ofGetWidth()/2, ofGetHeight()/2, 500*modSound, 500*modSound);
	img1.draw(ofGetWidth() / 2 - 250 * modSound, ofGetHeight() / 2 - 250 * modSound, 
					500 * modSound, 500 * modSound);
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
