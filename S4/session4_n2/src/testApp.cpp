#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	camera.setDistance(400);
	ofSetCircleResolution(3);

	lenna.loadImage("lenna.png");
	bDrawLenna = false;
	bShowHelp  = true;
	myFbo.allocate(512, 512);

	myGlitch.setup(&myFbo);

	img1.load("eye.png");
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
void testApp::update(){
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

	myFbo.begin();
	ofClear(0, 0, 0,255);
	if (!bDrawLenna){

	int mod;
	mod = ofGetFrameNum() % 100;
	// entintar la imagen
	float col = ofGetFrameNum()%255;
	ofSetColor(col, 0, 255 - col, 200);
	// dibujar la imagen
	//img1.draw(ofGetWidth()/2, ofGetHeight()/2, 500*modSound, 500*modSound);
	img1.draw(ofGetWidth() / 2 - 250 * modSound, ofGetHeight() / 2 - 250 * modSound,
		500 * modSound, 500 * modSound);
		/*camera.begin();
		for (int i = 0;i < 100;i++){
			if		(i % 5 == 0)ofSetColor(50 , 255, 100);
			else if (i % 9 == 0)ofSetColor(255, 50, 100);
			else				ofSetColor(255, 255, 255);

			ofPushMatrix();
			ofRotate(ofGetFrameNum(), 1.0, 1.0, 1.0);
			ofTranslate((ofNoise(i/2.4)-0.5)*1000,
						(ofNoise(i/5.6)-0.5)*1000,
						(ofNoise(i/8.2)-0.5)*1000);
			ofCircle(0, 0, (ofNoise(i/3.4)-0.5)*100+ofRandom(3));
			ofPopMatrix();
		}
		camera.end();*/
	}else{
		ofSetColor(255);
		lenna.draw(0, 0);
	}
	myFbo.end();

}

//--------------------------------------------------------------
void testApp::draw(){
	if (spectrum[2] > 0.2) {
		myGlitch.setFx(OFXPOSTGLITCH_SLITSCAN, true);
	}
	else {
		myGlitch.setFx(OFXPOSTGLITCH_SLITSCAN, false);
	}

	/* draw normal view */
	//ofSetColor(255);
	//myFbo.draw(0, 0);

	/* Apply effects */
	myGlitch.generateFx();

	/* draw effected view */
	ofSetColor(255);
	myFbo.draw(0, 0, ofGetWidth(), ofGetHeight());


	/* show information*/
	string info = "";
	info += "1 - 0 : Apply glitch effects.\n";
	info += "q - u : Apply color remap effects.\n";
	info += "L key : Switch 3Dview / 2DImage.\n";
	info += "H key : Hide or show this information.";

	if (bShowHelp){
		ofSetColor(0, 200);
		ofRect(25, 17, 320, 60);
		ofSetColor(255);
		ofDrawBitmapString(info, 30,30);
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if (key == '1') myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE	, true);
	if (key == '2') myGlitch.setFx(OFXPOSTGLITCH_GLOW			, true);
	if (key == '3') myGlitch.setFx(OFXPOSTGLITCH_SHAKER			, true);
	if (key == '4') myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER		, true);
	if (key == '5') myGlitch.setFx(OFXPOSTGLITCH_TWIST			, true);
	if (key == '6') myGlitch.setFx(OFXPOSTGLITCH_OUTLINE		, true);
	if (key == '7') myGlitch.setFx(OFXPOSTGLITCH_NOISE			, true);
	if (key == '8') myGlitch.setFx(OFXPOSTGLITCH_SLITSCAN		, true);
	if (key == '9') myGlitch.setFx(OFXPOSTGLITCH_SWELL			, true);
	if (key == '0') myGlitch.setFx(OFXPOSTGLITCH_INVERT			, true);

	if (key == 'q') myGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, true);
	if (key == 'w') myGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE	, true);
	if (key == 'e') myGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE	, true);
	if (key == 'r') myGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE	, true);
	if (key == 't') myGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT	, true);
	if (key == 'y') myGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT	, true);
	if (key == 'u') myGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT	, true);

	if (key == 'l') bDrawLenna ^= true;
	if (key == 'h') bShowHelp ^= true;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	if (key == '1') myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE	, false);
	if (key == '2') myGlitch.setFx(OFXPOSTGLITCH_GLOW			, false);
	if (key == '3') myGlitch.setFx(OFXPOSTGLITCH_SHAKER			, false);
	if (key == '4') myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER		, false);
	if (key == '5') myGlitch.setFx(OFXPOSTGLITCH_TWIST			, false);
	if (key == '6') myGlitch.setFx(OFXPOSTGLITCH_OUTLINE		, false);
	if (key == '7') myGlitch.setFx(OFXPOSTGLITCH_NOISE			, false);
	if (key == '8') myGlitch.setFx(OFXPOSTGLITCH_SLITSCAN		, false);
	if (key == '9') myGlitch.setFx(OFXPOSTGLITCH_SWELL			, false);
	if (key == '0') myGlitch.setFx(OFXPOSTGLITCH_INVERT			, false);

	if (key == 'q') myGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, false);
	if (key == 'w') myGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE	, false);
	if (key == 'e') myGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE	, false);
	if (key == 'r') myGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE	, false);
	if (key == 't') myGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT	, false);
	if (key == 'y') myGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT	, false);
	if (key == 'u') myGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT	, false);
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}