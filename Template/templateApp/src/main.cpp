#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofSetupOpenGL(1280, 1280, OF_WINDOW);			// <-------- ventana openGL
	//ofSetupOpenGL(1280, 720, OF_FULLSCREEN);		// <-------- habilitar fullscreen
	ofRunApp(new ofApp());

}
