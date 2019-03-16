#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	ofSetVerticalSync(true);
	//ofBackgroundGradient(ofColor::black, ofColor::blue);
	w = ofGetWidth();
	h = ofGetHeight();
	ofSetFrameRate(30);
	
	// ------ ------ ------ ------ ------ ------ | dsp |
	sampleRate = 44100;
	initialBufferSize = 512;
	lAudioOut = new float[initialBufferSize];
	rAudioOut = new float[initialBufferSize];
	lAudioIn = new float[initialBufferSize];
	rAudioIn = new float[initialBufferSize];
	memset(lAudioOut, 0, initialBufferSize * sizeof(float));
	memset(rAudioOut, 0, initialBufferSize * sizeof(float));
	memset(lAudioIn, 0, initialBufferSize * sizeof(float));
	memset(rAudioIn, 0, initialBufferSize * sizeof(float));

	fftSize = 1024;
	mfft.setup(fftSize, 512, 256);
	ifft.setup(fftSize, 512, 256);
	nAverages = 12;
	oct.setup(sampleRate, fftSize / 2, nAverages);
	mfccs = (double*)malloc(sizeof(double) * 13);
	mfcc.setup(512, 42, 13, 20, 20000, sampleRate);
	ofxMaxiSettings::setup(sampleRate, 2, initialBufferSize);
	ofSoundStreamSetup(2, 2, this, sampleRate, initialBufferSize, 4); // Call this last ! 
	// fft :: 512 , mfft.magnitudes[i] , pitch/timbral/vol
	// MFCC :: 13 , mfccs[i] , timbre/vocal
	// CosQ :: 104, oct.averages[i] , 12 bands/oct
	// peak :: peakFreq, spectral centroid :: centroid, rms::RMS
	nBands = 104;
	dBuffSize = 8;
	d0.resize(nBands);
	mB.resize(nBands);
	dataBuff.resize(dBuffSize);
	for (int n = 0; n < dBuffSize; n++) { dataBuff[n].resize(nBands); }


	// ------ ------ ------ ------	------ ------ | gui |
	showGui = true;
	gui.setup();
	mixGroup.setup("mix");
	mixGroup.setName("mixer");
	mixGroup.setHeaderBackgroundColor(ofColor::black);
	mixGroup.setBorderColor(ofColor::darkGray);
	mixGroup.setTextColor(ofColor::darkGray);
	mixGroup.add(dsp_enable.setup("dsp", true));
	mixGroup.add(send_syph.setup("send_SPOUT", false));
	gui.add(&mixGroup);

	mov1Group.setup("mov1");
	mov1Group.setName("1.[][-][].1");
	mov1Group.setHeaderBackgroundColor(ofColor::black);
	mov1Group.setBorderColor(ofColor::black);
	mov1Group.setTextColor(ofColor::darkGray);
	mov1Group.add(mov1_en.setup("enable", false));
	mov1Group.add(mov1_fade.setup("fade", 255, 0, 255));
	mov1Group.add(mov1_mov.setup("movement", false));
	mov1Group.add(mov1_vel.setup("veloc", 0.001, 0, 0.06));
	mov1Group.add(mov1_wall.setup("wall", false));
	mov1Group.add(mov1_wdir.setup("wall_direction", 3, 1, 100));
	mov1Group.add(mov1_multi.setup("multi", false));
	mov1Group.add(mov1_trail.setup("trails", false));
	gui.add(&mov1Group);

	mov2Group.setup("mov2");
	mov2Group.setName("2.[--][][--].2");
	mov2Group.setHeaderBackgroundColor(ofColor(0,0,0,100));
	mov2Group.setBorderColor(ofColor::black);
	mov2Group.setTextColor(ofColor::darkGray);
	mov2Group.add(mov2_en.setup("enable", false));
	mov2Group.add(mov2_fade.setup("fade", 255, 0, 255));
	mov2Group.add(mov2_mov.setup("movement", false));
	mov2Group.add(mov2_ran.setup("range", 10, 0, 500));
	//mov2Group.add(mov2_mop.setup("mov particles", 0.1, 0, 50));
	mov2Group.add(mov2_wall.setup("wall", false));
	mov2Group.add(mov2_par.setup("wall_reactive", false));
	gui.add(&mov2Group);

	mov3Group.setup("mov3");
	mov3Group.setName("3.[�][�][�].3");
	mov3Group.setHeaderBackgroundColor(ofColor::black);
	mov3Group.setBorderColor(ofColor::black);
	mov3Group.setTextColor(ofColor::darkGray);
	mov3Group.add(mov3_en.setup("enable", false));
	mov3Group.add(mov3_fade.setup("fade", 255, 0, 255));
	mov3Group.add(mov3_mov.setup("movement", false));
	mov3Group.add(mov3_max_ang.setup("range", 0, -20, 20));
	mov3Group.add(mov3_wall.setup("wall", false));
	gui.add(&mov3Group);

	mov4Group.setup("mov4");
	mov4Group.setName("4.[//][�][//].4");
	mov4Group.setHeaderBackgroundColor(ofColor::black);
	mov4Group.setBorderColor(ofColor::black);
	mov4Group.setTextColor(ofColor::darkGray);
	mov4Group.add(mov4_en.setup("enable", false));
	mov4Group.add(mov4_fade.setup("fade", 255, 0, 255));
	mov4Group.add(mov4_mov.setup("movement", false));
	mov4Group.add(mov4_noise.setup("veloc", 0.00001, 0, 0.03));
	//mov4Group.add(mov4_wall.setup("wall", false));
	gui.add(&mov4Group);

	mov5Group.setup("mov5");
	mov5Group.setName("5.[-][---][-].5");
	mov5Group.setHeaderBackgroundColor(ofColor::black);
	mov5Group.setBorderColor(ofColor::black);
	mov5Group.setTextColor(ofColor::darkGray);
	mov5Group.add(mov5_en.setup("enable", false));
	mov5Group.add(mov5_fade.setup("fade", 255, 0, 255));
	mov5Group.add(mov5_mov.setup("movement", false));
	mov5Group.add(mov5_vel.setup("veloc", 0.00001, 0, 1));
	mov5Group.add(mov5_grow.setup("grow", false));
	mov5Group.add(mov5_orbit.setup("orbit", false));
	//mov5Group.add(mov5_wall.setup("wall", false));
	gui.add(&mov5Group);

	mov6Group.setup("mov6");
	mov6Group.setName("6.[--][--][--].6");
	mov6Group.setHeaderBackgroundColor(ofColor::black);
	mov6Group.setBorderColor(ofColor::black);
	mov6Group.setTextColor(ofColor::darkGray);
	mov6Group.add(mov6_en.setup("enable", false));
	mov6Group.add(mov6_fade.setup("fade", 255, 0, 255));
	mov6Group.add(mov6_fal.setup("fall", false));
	mov6Group.add(mov6_per.setup("period", 50, 1, 1000));
	mov6Group.add(mov6_ext.setup("extrusion", 2, 0, 100));
	gui.add(&mov6Group);

	mov7Group.setup("mov7");
	mov7Group.setName("7.[...][+][...].7");
	mov7Group.setHeaderBackgroundColor(ofColor::black);
	mov7Group.setBorderColor(ofColor::black);
	mov7Group.setTextColor(ofColor::darkGray);
	mov7Group.add(mov7_en.setup("enable", false));
	mov7Group.add(mov7_fade.setup("fade", 255, 0, 255));
	mov7Group.add(mov7_fal.setup("fall", false));
	mov7Group.add(mov7_per.setup("period", 50, 1, 1000));
	mov7Group.add(mov7_ext.setup("extrusion", 2, 0, 100));
	gui.add(&mov7Group);
	// ------ ------ ------ ------ ------ ------ | fbo |
	fbo_glitch.allocate(w, h, GL_RGBA);
	fbo_glitch.begin(); ofClear(0, 0); fbo_glitch.end();
	tImg.allocate(w, h, OF_IMAGE_COLOR_ALPHA);

	// ------ ------ ------ ------ ------ ------ | shaders |
	//shader_kale.load("kaleido");
	//myGlitch.setup(&fbo4a);
	//myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE, true);


	// ------ ------ ------ ------ ------ ------ | rec |
	recording = false;
	recorder.setPrefix(ofToDataPath("react2/reac_"));
	recorder.setFormat("jpg");

	// ------ ------ ------ ------ ------ ------ | colors |
	ofLoadImage(pix, "im2.jpg");
	ofPoint point1 = ofPoint(0, 0, 0);
	c1 = ofColor(254, 0, 55);
	ofSetLineWidth(4);
	//ofNoFill();
	
	// ------ ------ ------ ------ ------ ------ | agents |
	//A1 = Agent01();
	A1.setup();
	A2.setup();
	A3.setup();
	A4.setup();
	A5.setup();
	A6.setup();
	A7.setup();

	/*// --- SYPHON
	individualTextureSyphonServer.setName("Texture Output");
	//mainOutputSyphonServer.setName("Screen Output");
	//mClient.setup();
	//mClient.set("", "Simple Server");
	*/
	sender.init("xato");
}


//--------------------------------------------------------------
void ofApp::update(){
	t = ofGetElapsedTimeMillis();
	tt = ofGetElapsedTimef();

	std::stringstream strm;
	strm << "> i n t e r s p e c i f i c s < :: reactSynth :: [fps]: " << ofGetFrameRate();
	ofSetWindowTitle(strm.str());

	// ------ ------ ------ ------ ------ ------ | dsp |
	if (dsp_enable==true) {
		for (int b = 0; b < nBands; b++) { if (oct.averages[b] > 0) { d0[b] = oct.averages[b]; } }
		for (int n = dBuffSize - 1; n > 0; n--) { dataBuff[n] = dataBuff[n - 1]; }
		dataBuff[0] = d0;
		for (int b = 0; b < nBands; b++) {
			mB[b] = 0;
			for (int n = 0; n < dBuffSize; n++) { mB[b] += dataBuff[n][b]; }
			mB[b] /= dBuffSize;
		}
		slowRMS = ofLerp(slowRMS, RMS, 0.1);
	}

	// ------ ------ ------ ------ ------ ------ | agents |
	if (mov1_en == true) {
		A1.update(mov1_mov, mov1_vel, mov1_wall, mov1_wdir, mov1_multi, mov1_trail, RMS);
	}
	if (mov2_en == true) {
		A2.update(mov2_mov, mov2_ran, mov2_wall, (mov2_par==true), mB);
	}
	if (mov3_en == true) {
		A3.update(mov3_mov, mov3_max_ang, mov3_wall, mB);
	}
	if (mov4_en == true) {
		A4.update(mov4_mov, mov4_noise, mov4_wall, mB);
	}
	if (mov5_en == true) {
		A5.update(mov5_mov, mov5_vel, mov5_grow, mov5_wall, mB);
	}
	if (mov6_en == true) {
		A6.update(mov6_fal, mov6_per, mov6_ext, mB);
	}
	if (mov7_en == true) {
		A7.update(mov7_fal, mov7_per, mov7_ext, mB, d0);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetVerticalSync(true);
	if (mov1_en == true) {
		ofSetColor(int(mov1_fade), 255);
		A1.draw();
	}
	if (mov2_en == true) {
		ofSetColor(int(mov2_fade), 255);
		A2.draw();
	}
	if (mov3_en == true) {
		ofSetColor(int(mov3_fade), 255);
		A3.draw();
	}
	if (mov4_en == true) {
		ofSetColor(int(mov4_fade), 255);
		A4.draw();
	}
	if (mov5_en == true) {
		ofSetColor(int(mov5_fade), 255);
		A5.draw();
	}
	if (mov6_en == true) {
		ofSetColor(int(mov6_fade), 255);
		A6.draw();
	}
	if (mov7_en == true) {
		ofSetColor(int(mov7_fade), 255);
		A7.draw();
	}
	ofDisableDepthTest();
	/*// --- SYPHON
	if (send_syph == true) {
		tImg.grabScreen(0, 0, w, h);
		individualTextureSyphonServer.publishTexture(tImg.getTexture());
		//mainOutputSyphonServer.publishScreen();
	}
	*/

	//evident gui
	if (showGui) gui.draw();

	rImg.grabScreen(0, 0, w, h);
	if (recording) recorder.addFrame(rImg);
	sender.send(rImg.getTexture());
	// gui original
	//if (showGui) gui.draw();
}

//--------------------------------------------------------------
void ofApp::exit() {
	gui.saveToFile("settings.xml");
	recorder.waitForThread();
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'h') {
		showGui = !showGui;
	}
	if (key == 'f') {
		ofToggleFullscreen();
	}
	if (key == 'r') {
		recording = !recording;
	}
	if (key == 't') {
		if (recorder.isThreadRunning()) {
			recorder.stopThread();
		}
		else {
			recorder.startThread(false, true);
		}
	}
	if (key == '5' && mov7_en == true) 	A7.colorPick.setup("cImgs/img_s05.jpg");
	if (key == '6' && mov7_en == true) 	A7.colorPick.setup("cImgs/img_s06.jpg");
	if (key == '7' && mov7_en == true) 	A7.colorPick.setup("cImgs/img_s07.jpg");
	if (key == '8' && mov7_en == true) 	A7.colorPick.setup("cImgs/img_s08.jpg");
	if (key == '9' && mov7_en == true) 	A7.colorPick.setup("cImgs/img_s09.jpg");
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
void ofApp::mousePressed(int x, int y, int button) {
	if (mov7_en == true) {
		for (int i = 0; i < 36 - 1; i++) {
			A7.theBlocks[i].dest = ofPoint(w / 2 + ofRandom(-w / 2, w / 2), 10 + i * 20, 0);
			A7.theBlocks[i].spc_td = ofRandom(0.8, 1.1) * w / A7.theBlocks[i].nt;
			//theBlocks[i].cwd = ofRandom(0.01, 1.9) * w / theBlocks[i].nt;
			A7.theBlocks[i].cwd = ofRandom(0.01, 1.9) * w / A7.theBlocks[i].nt;

			if (i > 11 && i < 23) {
				A7.theBlocks[i].v = ofRandom(PI / 8, PI / 6);
			}
			else {
				A7.theBlocks[i].v = ofRandom(0.01, 0.5);
			}
		}

	}

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

//--------------------------------------------------------------
void ofApp::audioRequested(float * output, int bufferSize, int nChannels) {
	for (int i = 0; i < bufferSize; i++) {
		wave = lAudioIn[i];
		if (mfft.process(wave)) {
			mfft.magsToDB();
			oct.calculate(mfft.magnitudesDB);

			float sum = 0;
			float maxFreq = 0;
			int maxBin = 0;
			for (int i = 0; i < fftSize / 2; i++) {
				sum += mfft.magnitudes[i];
				if (mfft.magnitudes[i] > maxFreq) {
					maxFreq = mfft.magnitudes[i];
					maxBin = i;
				}
			}
			centroid = sum / (fftSize / 2);
			peakFreq = (float)maxBin / fftSize * 44100;

			mfcc.mfcc(mfft.magnitudes, mfccs);
		}
		lAudioOut[i] = 0;
		rAudioOut[i] = 0;
	}
}


//--------------------------------------------------------------
void ofApp::audioReceived(float * input, int bufferSize, int nChannels) {
	/* You can just grab this input and stick it in a double, then use it above to create output*/
	float sum = 0;
	for (int i = 0; i < bufferSize; i++) {
		/* you can also grab the data out of the arrays*/
		lAudioIn[i] = input[i * 2];
		rAudioIn[i] = input[i * 2 + 1];

		sum += input[i * 2] * input[i * 2];
	}
	RMS = sqrt(sum);
}