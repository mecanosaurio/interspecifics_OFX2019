#include "ofApp.h"
#include "maximilian.h"
#include "time.h"

//--------------- --------------- ---------------  ------          ----------------------------//
//---------------****** SETUP ****** -------------------          ----------------------------//
void ofApp::setup() {
	receiver.setup(IN_PORT);
	cout << "listening for osc messages on port " << IN_PORT << "\n";

	// --- --- .. _..  --- _.. --- --- ---	_.. 	--- --- --- audio
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

	// --- --- .. _..  --- _.. --- --- ---	_.. 	--- --- --- maximilian
	fftSize = 1024;
	mfft.setup(fftSize, 512, 256);
	ifft.setup(fftSize, 512, 256);
	nAverages = 12;
	oct.setup(sampleRate, fftSize / 2, nAverages);
	mfccs = (double*)malloc(sizeof(double) * 13);
	mfcc.setup(512, 42, 13, 20, 20000, sampleRate);
	ofxMaxiSettings::setup(sampleRate, 2, initialBufferSize);
	ofSoundStreamSetup(2, 2, this, sampleRate, initialBufferSize, 4);			/* Call this last ! */

	// --- --- .. _..  --- _.. --- --- ---	_.. 	--- --- --- maximilian
	nBands = 104;
	dBuffSize = 32;
	d0.resize(nBands);
	mB.resize(nBands);
	dataBuff.resize(dBuffSize);
	for (int n = 0; n < dBuffSize; n++) {
		dataBuff[n].resize(nBands);
	}


	// --- --- .. _..  --- _.. --- --- ---	_.. 	--- --- --- kinect
	kinect.setRegistration(true);
	kinect.init();
	kinect.open();
	angle = 9;
	kinect.setCameraTiltAngle(angle);

	// --- --- .. _..  --- _.. --- --- ---	_.. 	--- --- --- imgs
	colorImg.allocate(kinect.width, kinect.height);
	gImg.allocate(kinect.width, kinect.height);
	fbo.allocate(kinect.width, kinect.height, GL_RGBA);
	gThrNea.allocate(kinect.width, kinect.height);
	gThrFar.allocate(kinect.width, kinect.height);

	// --- --- .. _..  --- _.. --- --- ---	_.. 	--- --- --- gral
	ofSetFrameRate(60);
	ofBackgroundGradient(ofColor(0, 0, 0), ofColor(20, 20, 20));

	// --- --- .. _..  --- _.. --- --- ---	_.. 	--- --- --- gui
	gui.setup();
	gui.add(meditate.setup("[[[M]]]:", 10, 0, 100));
	gui.add(concentrate.setup("[[[C]]]:", 10, 0, 100));
	gui.add(nearThreshold.setup("[[[NEAR]]]:", 254, 1, 255));
	gui.add(farThreshold.setup("[[[FAR]]]:", 105, 1, 255));
	//gui.add(chromagramToggle.setup("Const-Q analyser (12 bands/oct) (104 inputs)", false));
	bHide = true;
	myfont.loadFont("Pixeled.ttf", 10);
	myFont2.loadFont("Pixeled.ttf", 8);
	//ofSetVerticalSync(true);
	med = 0.0;
	con = 0.0;

	// --- --- .. _..  --- _.. --- --- ---	_.. 	--- --- --- video
	//video01.loadMovie("port.mp4");
	//video01.play();

	// --- --- .. _..  --- _.. --- --- ---	_.. 	--- --- --- particles
	h = ofGetHeight();
	w = ofGetWidth();
	nP = 128;
	float sphR = 300;
	int gap = int( w / 4);  

	points.resize(nP);
	colors.resize(nP);
	particles.resize(nP);

	//create fixed points
	for (int i = 0; i < nP; i++) {
		int mod = i % 3;
		if (mod == 0) {
			ofPoint cp(w/2, 130, -40*int(i/3));
			points[i] = cp;
			particles[i].setup(20, cp);
		} else if (mod == 1) {
			ofPoint cp(w/2 - 410, 130, -40*int(i / 3));
			points[i] = cp;
			particles[i].setup(20, cp);
		}
		else if (mod == 2) {
			ofPoint cp(w / 2 + 410, 130, -40*int(i / 3));
			points[i] = cp;
			particles[i].setup(20, cp);
		}
	}
	//atractor.setup( ofPoint(ofGetWidth()/2,ofGetHeight()/2,0) );
}





//--------------- --------------- ---------------  ------          ----------------------------//
//---------------****** UPDATE ****** -------------------          ----------------------------//
void ofApp::update() {
	// ----------- .. .. ----------- .. .  ----- OSC
	while (receiver.hasWaitingMessages()) {
		ofxOscMessage m;
		receiver.getNextMessage(m);
		if (m.getAddress() == "/med") {
			// both the arguments are int32's
			met = m.getArgAsInt32(0);
			cout << "med: " << met << "\n";
		}
		else if (m.getAddress() == "/att") {
			// both the arguments are int32's
			kon = m.getArgAsInt32(0);
			cout << "con: " << kon << "\n";
		}
		else if (m.getAddress() == "/blink") {
			// both the arguments are int32's
			blinc = m.getArgAsInt32(0);
			cout << "blink: " << blinc << "\n";
		}
	}


	// ----------- .. .. ----------- .. .  ----- BUFFER
	for (int b = 0; b < nBands; b++) {											/* refill the buff */
		if (oct.averages[b] > 0) {
			d0[b] = oct.averages[b];
			// normalize ?
		}
	}
	for (int n = dBuffSize - 1; n > 0; n--) {
		dataBuff[n] = dataBuff[n - 1];
	}
	dataBuff[0] = d0;

	for (int b = 0; b < nBands; b++) {											/* get the means */
		mB[b] = 0;
		for (int n = 0; n < dBuffSize; n++) {
			mB[b] += dataBuff[n][b];
		}
		mB[b] /= dBuffSize;
	}

	// ----------- .. .. ----------- .. .  ----- PARTICLES
	//atractor.updateM(atractMass);
	for (int i = 0; i < nP; i++) {
		t = ofGetElapsedTimeMillis();
		epoch = i%nBands;
		particles[i].setColor(ofColor(50*mB[epoch] ));
		
	}


	// ----------- .. .. ----------- .. .  ----- KINECT
	kinect.update();
	if (kinect.isFrameNew()) {
		gImg.setFromPixels(kinect.getDepthPixels());
		gThrNea = gImg;
		gThrFar = gImg;
		gThrNea.threshold(nearThreshold, true);
		gThrFar.threshold(farThreshold);
		cvAnd(gThrNea.getCvImage(), gThrFar.getCvImage(), gImg.getCvImage(), NULL);
		gImg.flagImageChanged();
		cFinder.findContours(gImg, 10, (kinect.width*kinect.height) / 2, 20, false);
		//gb = 0;
		//for (int i = 0; i < cFinder.nBlobs; i++) {
		//	if (cFinder.blobs[i].nPts > cFinder.blobs[gb].nPts) {gb = i ;}
		//}
	}

	// ----------- .. .. ----------- .. .  ----- FBO
	fbo.begin();
	ofClear(0, 0, 0, 0);
	ofSetColor(255);
	ofSetLineWidth(4);
	//kinect.drawDepth(10, 10, 400, 300);
	//kinect.draw(420, 10, 400, 300);
	//gImg.draw(0, 0, ofGetWidth(), ofGetHeight());
	if (cFinder.nBlobs > 0) {
		ofPolyline line(cFinder.blobs[0].pts);
		ofSetColor(color);
		line.draw();
	}
	//grayImage.draw(0, 0, ofGetWidth(), ofGetHeight());
	fbo.end();
	smLerp();

	/*
	video01.update();
	if (video01.isFrameNew()) {
		pixels = video01.getPixels();
	}
	*/
}




//--------------- --------------- ---------------  ------          ----------------------------//
//---------------****** DRAW ****** -------------------          -------------------------  ---//
void ofApp::draw() {
	ofBackgroundGradient(ofColor(20, 20, 20), ofColor(0,0,0));
	
	/*for (int x = 0; x < pixels.getWidth(); x++) {
		ofColor coli = pixels.getColor(x, h / 2);
		ofSetColor(coli);
		ofLine(x, 0, x, h);
	}
	*/


	// draw fbo
	fbo.draw(w/2 - 50, h/2 - 50, 100, 100);

	ofEnableAlphaBlending();
	// draw particles
	
	for (int i = 0; i < nP; i++) {
		particles[i].draw();
	}
	
	ofDisableAlphaBlending();

	// gui
	if (bHide) {
		gui.draw();
	}
}

//--------------------------------------------------------------
void ofApp::exit() {
	kinect.close();
}

//--------------------------------------------------------------
void ofApp::smLerp() {
	med = ofLerp(med, meditate, 0.02);
	con = ofLerp(con, concentrate, 0.02);
	//color = ofColor(2.5*med,0,2.5*con, 255);
}




//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'h') {
		bHide = !bHide;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

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