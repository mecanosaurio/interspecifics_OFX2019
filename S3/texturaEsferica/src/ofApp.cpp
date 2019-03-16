#include "ofApp.h"
#include "maximilian.h"
#include "time.h"


// ---------- ---------- ----------   ----------   ---------- ---------- -------- - ..............
//---------- ---------- ----------   ----------   ---------- ---------- ---------- | >> SETUP  //
// ---------- ---------- ----------   ----------   ---------- ---------- -------- - ··············

void ofApp::setup() {
	h = ofGetHeight();
	w = ofGetWidth();
	ofSetFrameRate(60);
	ofBackground(0);


	// .. __ .. __  .. __ .. __ .. __ .. __ .. __ .. __ .. __ .. __ --- --- --- OSC
	receiver.setup(OSC_IN_PORT);
	cout << "listening for osc messages on port " << OSC_IN_PORT << "\n";


	// .. __ .. __  .. __ .. __ .. __ .. __ .. __ .. __ .. __ .. __ --- --- --- AUDIO
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

	nBands = 104;
	dBuffSize = 64;
	d0.resize(nBands);
	mB.resize(nBands);
	dataBuff.resize(dBuffSize);
	for (int n = 0; n < dBuffSize; n++) {
		dataBuff[n].resize(nBands);
	}


	// .. __ .. __  .. __ .. __ .. __ .. __ .. __ .. __ .. __ .. __ --- --- --- GUI
	showGui = true;
	gui.setup();

	sceneGroup.setup("scene");
	sceneGroup.setHeaderBackgroundColor(ofColor::white);
	sceneGroup.setTextColor(ofColor::black);
	sceneGroup.setBorderColor(ofColor::white);
	sceneGroup.add(sc0.setup("SC -00", false));
	sceneGroup.add(sc1.setup("SC -01", false));
	sceneGroup.add(sc2.setup("SC -02", false));
	sceneGroup.add(sc3.setup("SC -03", false));
	sceneGroup.add(sc4.setup("SC -04", false));

	gui.add(&sceneGroup);

	mixerGroup.setup("mixer");
	mixerGroup.setHeaderBackgroundColor(ofColor::blue);
	mixerGroup.setBorderColor(ofColor::blue);
	mixerGroup.add(show2D.setup("show2D", 255, 0, 255));
	mixerGroup.add(show3D.setup("show3D", 255, 0, 255));
	mixerGroup.add(rad.setup("rad", 155, 0, 512));
	mixerGroup.add(extrude.setup("extrude", 0.75, 0, 1));
	mixerGroup.add(fx1.setup("fx1", false));
	gui.add(&mixerGroup);

	generalGroup.setup("general");
	generalGroup.setHeaderBackgroundColor(ofColor::blueSteel);
	generalGroup.setBorderColor(ofColor::blueSteel);
	generalGroup.add(dspEnabled.setup("DSPEnable", true));
	generalGroup.add(img1Enabled.setup("img1_en", false));
	generalGroup.add(invContourSpacing.setup("invContour Space", 127, 1, 256));
	gui.add(&generalGroup);

	mindGroup.setup("mindwave");
	mindGroup.setHeaderBackgroundColor(ofColor::lightBlue);
	mindGroup.setBorderColor(ofColor::lightBlue);
	mindGroup.add(mindEnabled.setup("mindEnable", false));
	mindGroup.add(mConc.setup("concentrate", 127, 1, 255));
	mindGroup.add(mMedit.setup("meditate", 127, 1, 255));
	gui.add(&mindGroup);

	kinectGroup.setup("kinect");
	kinectGroup.setHeaderBackgroundColor(ofColor::forestGreen);
	kinectGroup.setBorderColor(ofColor::forestGreen);
	kinectGroup.add(kinEnabled.setup("kinEnabled", true));
	kinectGroup.add(nearThreshold.setup("nearTHRSH:", 254, 1, 255));
	kinectGroup.add(farThreshold.setup("farTHRSH:", 105, 1, 255));
	gui.add(&kinectGroup);

	camGroup.setup("camera");
	camGroup.setHeaderBackgroundColor(ofColor::gray);
	camGroup.setBorderColor(ofColor::darkGray);
	camGroup.add(camEnabled.setup("camEnabled", false));
	camGroup.add(camThreshold.setup("camTHRSH:", 50, 1, 255));
	camGroup.add(bLearnBackground.setup("bLearnBackground", true));
	gui.add(&camGroup);

	kaleidoGroup.setup("kaleidoscope");
	kaleidoGroup.setHeaderBackgroundColor(ofColor::darkRed);
	kaleidoGroup.setBorderColor(ofColor::darkRed);
	kaleidoGroup.add(kenabled.setup("kaleidoEnabled", false));
	kaleidoGroup.add(ksectors.setup("ksectors", 19, 1, 50));
	kaleidoGroup.add(kangle.setup("kangle", 101.02, -180, 180));
	kaleidoGroup.add(kx.setup("kx", 0.459184, 0, 1));
	kaleidoGroup.add(ky.setup("ky", 0.673469, 0, 1));
	gui.add(&kaleidoGroup);

		
	// .. __ .. __  .. __ .. __ .. __ .. __ .. __ .. __ .. __ .. __ --- --- --- KINECT
	kinect.setRegistration(true);
	kinect.init();
	kinect.open();
	angle = 9;
	kinect.setCameraTiltAngle(angle);

	colorImg.allocate(kinect.width, kinect.height);
	gImg.allocate(kinect.width, kinect.height);
	bgImg.allocate(kinect.width, kinect.height);
	gThrNea.allocate(kinect.width, kinect.height);
	gThrFar.allocate(kinect.width, kinect.height);

			
	// .. __ .. __  .. __ .. __ .. __ .. __ .. __ .. __ .. __ .. __ --- --- --- FBO
	fbo_00.allocate(w, h, GL_RGBA);
	fbo_01.allocate(w, h, GL_RGBA);
	fbo_02a.allocate(w, h, GL_RGBA);
	fbo_02b.allocate(w, h, GL_RGBA);
	fbo_03.allocate(w, h, GL_RGB);
	fbo_04a.allocate(w, h, GL_RGBA32F_ARB);
	fbo_04b.allocate(w, h, GL_RGBA);

	fbo_00.begin();
	ofClear(0, 0);
	fbo_00.end();

	fbo_01.begin();
	ofClear(0, 0);
	fbo_01.end();

	fbo_02a.begin();
	ofClear(0, 0);
	fbo_02a.end();
	fbo_02b.begin();
	ofClear(0, 0);
	fbo_02b.end();

	fbo_03.begin();
	ofClear(0, 0);
	fbo_03.end();

	fbo_04a.begin();
	ofClear(0, 0);
	fbo_04a.end();
	fbo_04b.begin();
	ofClear(0, 0);
	fbo_04b.end();

	glow.allocate(w, h, GL_RGBA);
	myGlitch.setup(&fbo_02a);

	// .. __ .. __  .. __ .. __ .. __ .. __ .. __ .. __ .. __ .. __ --- --- --- IMGS
	n_imgs_00 = dir.listDir("sc00V");
	seq_00.resize(n_imgs_00);
	for (int i = 0; i < n_imgs_00; i++) {
		string fn = dir.getPath(i);
		ofLoadImage(seq_00[i], fn);
		cout << "Reading_00: " << i << "/" << n_imgs_00 << "\n";
	}

	n_imgs_03 = dir.listDir("sc03V");
	seq_03.resize(n_imgs_03);
	for (int i = 0; i < n_imgs_03; i++) {
		string fn = dir.getPath(i);
		ofLoadImage(seq_03[i], fn);
		cout << "Reading_03: " << i << "/" << n_imgs_03 << "\n";
	}


	// .. __ .. __  .. __ .. __ .. __ .. __ .. __ .. __ .. __ .. __ --- --- --- SHADERS
	shader_kale.load("kaleido");
	shaderMoon.load("kaleido.vert", "hex2.frag");


	// .. __ .. __  .. __ .. __ .. __ .. __ .. __ .. __ .. __ .. __ --- --- --- DRAWING
	ofVec3f gravity(0, -200, 0);
	nP = 128;
	float sphR = 300;
	int gap = int(w / 4);
	points.resize(nP);
	colors.resize(nP);
	//create fixed points


	// .. __ .. __  .. __ .. __ .. __ .. __ .. __ .. __ .. __ .. __ --- --- --- CAMERA
	cam.setDeviceID(0);
	cam.setDesiredFrameRate(60);
	cam.initGrabber(kinect.width, kinect.height);


	//silho.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	silho.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
	silho2.setMode(OF_PRIMITIVE_LINE_LOOP);
	serp.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
	moonCenter = ofPoint(-1200, 0, -1000);


	// .. __ .. __  .. __ .. __ .. __ .. __ .. __ .. __ .. __ .. __ --- --- --- SPHERE
	sphere.set(250, 200);
	vertices0 = sphere.getMesh().getVertices();
	sphere.mapTexCoords(0, h, w, 0);
	sphere.rotate(90, 0, 1, 0);


	// .. __ .. __  .. __ .. __ .. __ .. __ .. __ .. __ .. __ .. __ --- --- --- FX
	myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE, true);


	// .. __ .. __  .. __ .. __ .. __ .. __ .. __ .. __ .. __ .. __ --- --- --- VIDEO IMGS
	index_00 = 0;
	index_03 = 0;
	fNo_00 = 0;
	fNo_01 = 0;
	fNo_02 = 0;
	fNo_03 = 0;
	fNo_04 = 0;

	stars.resize(1);
	tt0 = ofGetElapsedTimeMillis();
	ofHideCursor();
}




// ---------- ---------- ----------   ----------   ---------- ---------- -------- - ..............
//---------- ---------- ----------   ----------   ---------- ---------- ---------- | >> UPDATE  //
// ---------- ---------- ----------   ----------   ---------- ---------- -------- - ··············

void ofApp::update() {

	t = ofGetElapsedTimeMillis();
	tt = ofGetElapsedTimef();

	// .. __ .. __  .. __ .. __ .. __ .. __ .. __ .. __ .. __ .. __ --- --- --- OSC
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


	// .. __ .. __  .. __ .. __ .. __ .. __ .. __ .. __ .. __ .. __ --- --- --- AUDIO
	if (dspEnabled) {
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
	}

	//                                                                           ...........
	// .. __ .. __  .. __ .. __ .. __ .. __ .. __ .. __ .. __ .. __ --- --- --- |SCENE 00  <<
	//                                                                           ···········
	// video + glitch 
	if (sc0 && index_00<n_imgs_00-10) {
		tt1 = ofGetElapsedTimeMillis();
		if ((tt1 - tt0) > 33) {
			index_00++;
			fbo_00.begin();
				seq_00[index_00].draw(0, 0, w, h);
			fbo_00.end();
			tt0 = ofGetElapsedTimeMillis();
		}
		fNo_00++;
	}// end sc00


	 //                                                                           ...........
	 // .. __ .. __  .. __ .. __ .. __ .. __ .. __ .. __ .. __ .. __ --- --- --- |SCENE 01  <<
	 //                                                                           ···········
	 // audio + kinect + moon
	if (sc1) {
		// get the contour
		kinect.update();
		if (kinect.isFrameNew()) {
			gImg.setFromPixels(kinect.getDepthPixels());
			gThrNea = gImg;
			gThrFar = gImg;
			gThrNea.threshold(nearThreshold, true);
			gThrFar.threshold(farThreshold);
			cvAnd(gThrNea.getCvImage(), gThrFar.getCvImage(), gImg.getCvImage(), NULL);
			gImg.flagImageChanged();
			cFinder.findContours(gImg, 10, (kinect.width*kinect.height) / 2, 5, false, true);
		}
		
		// transform
		fbo_01.begin();
		ofClear(0, 0, 0, 0);
		ofSetColor(255);
		ofSetLineWidth(3);

		if (cFinder.nBlobs > 0) {
			ofPolyline line(cFinder.blobs[0].pts);
			line = line.getResampledBySpacing(invContourSpacing);
			line.close();

			for (int i = 0; i < line.size(); i++) {
				ofPoint silOffset(320, 180, -500 + 100 * fNo_01 % 5);
				line[i] = line[i] + silOffset;
				silho.addVertex(line[i]);
				silho.addColor(ofColor(255, ofRandom(20, 255)));
			}
			if ((fNo_01 % 17 == 0) && (stars.size() < 1044)) {
				ofPoint nwSt(ofRandom(-w / 2, 2 * w), ofRandom(-h / 2, h / 2), ofRandom(-600, 100));
				stars.push_back(nwSt);
				//cout << "pushed back: " << nwSt << "\t" << "len starts:" << stars.size() << "\n";
			}
			silho.draw();
		}
		fbo_01.end();
		if (fNo_01 % 4 == 0) silho.clear();
		
		// draw stars
		for (int i = 0; i < stars.size(); i++) {
			stars[i].z = -500 + 100 * mB[i % (nBands / 2)];
		}
		moonCenter.x = -1540 + fNo_01*.21;
		moonCenter.y = h - fNo_01*.17;
		fNo_01++;
	}// end sc01
	

	 //                                                                           ...........
	 // .. __ .. __  .. __ .. __ .. __ .. __ .. __ .. __ .. __ .. __ --- --- --- |SCENE 02  <<
	 //                                                                           ···········
	 // kinect + rectangles + glow && lines
	if (sc2) {
		if (fNo_02 > 900) {
			myGlitch.setFx(OFXPOSTGLITCH_SLITSCAN, true);
		} else {
			myGlitch.setFx(OFXPOSTGLITCH_SLITSCAN, false);
		}
		if (fNo_02 > 1800) {
			myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER, true);
		} else {
			myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER, false);
		}
		// get the contour
		kinect.update();
		if (kinect.isFrameNew()) {
			gImg.setFromPixels(kinect.getDepthPixels());
			gThrNea = gImg;
			gThrFar = gImg;
			gThrNea.threshold(nearThreshold, true);
			gThrFar.threshold(farThreshold);
			cvAnd(gThrNea.getCvImage(), gThrFar.getCvImage(), gImg.getCvImage(), NULL);
			gImg.flagImageChanged();
			cFinder.findContours(gImg, 10, (kinect.width*kinect.height) / 2, 5, false, true);
		}

		// transform
		fbo_01.begin();
		ofClear(0, 0, 0, 0);
		ofSetColor(ofClamp(fNo_02, 0, 250));
		ofSetLineWidth(3);

		if (cFinder.nBlobs > 0) {
			ofPolyline line(cFinder.blobs[0].pts);
			//invContourSpacing = 10;
			line = line.getResampledBySpacing(10);
			line.close();

			for (int i = 0; i < line.size(); i++) {
				ofPoint silOffset(320, 240, -400 + 100 * fNo_01 % 5);
				line[i] = line[i] + silOffset;
				silho2.addVertex(line[i]);
				silho2.addColor(ofColor(255, ofRandom(20, 255)));
			}
			silho2.draw();
		}
		fbo_01.end();

		if (fNo_02 % 4 == 0) silho2.clear();

		fbo_02a.begin();
		ofSetColor(0, 20);
		ofFill();
		ofRect(0, 0, w, h);

		ofSetColor(0, 255, 55, 207);
		ofNoFill();
		for (int i = 0; i < 20; i++) {
			ofSetColor(200, 227);
			float r = ofRandom(1);
			//ofTranslate(0, 0, );
			//if (r > 0.83) {
			ofSetColor(mB[i * 5]*30, 255);
			ofRect(w / 2 - 300, h / 2 - 300 - ((20 - i)*sin(i*PI / 6 + t*.01)), 100 - 100 * i, 600, 600);
			//}
			if (r > 0.73) {
				ofPushMatrix();
				ofTranslate(0, 0, 100 - 50 * i);
				ofSetLineWidth(ofRandom(2, 5));
				ofSetColor(255, ofRandom(50, 250));
				fbo_01.draw(0, 0, w, h);
				ofPopMatrix();
			}
		}
		fbo_02a.end();

		glow << fbo_02a;
		glow.update();

		// lines
		ofSetColor(255, 255);
		fbo_02b.begin();
		for (int i = h / 2 - 200; i < h / 2; i++) {
			float r = ofNoise(20000 - t % 20000 * i*0.001);
			if (r < 0.33) {
				ofSetColor(255);
				ofLine(0, i, 340, i);
				ofLine(w - 340, i, w, i);
			}
			else if (r < 0.66) {
				ofSetColor(20, 0);
				ofLine(0, i, 340, i);
				ofLine(w - 340, i, w, i);
			}
			else {
				ofSetColor(0);
				ofLine(0, i, 340, i);
				ofLine(w - 340, i, w, i);

			}
		}

		for (int i = h / 2; i < h / 2 + 200; i++) {
			float r = ofNoise(t % 10000 * (h / 2 + i)*0.0001);
			if (r < 0.43) {
				ofSetColor(255);
				ofLine(0, i, 340, i);
				ofLine(w - 340, i, w, i);
			}
			else {
				ofSetColor(0);
				ofLine(0, i, 340, i);
				ofLine(w - 340, i, w, i);

			}
		}

		fbo_02b.end();	
		fNo_02++;
	}// end sc02


	 //                                                                           ...........
	 // .. __ .. __  .. __ .. __ .. __ .. __ .. __ .. __ .. __ .. __ --- --- --- |SCENE 03  <<
	 //                                                                           ···········
	 // video + glitch
	if (sc3 && index_03<n_imgs_03-10) {
		tt1 = ofGetElapsedTimeMillis();
		if ((tt1 - tt0) > 33) {
			index_03++;
			fbo_03.begin();
				seq_03[index_03].draw(0, 0, w, h);
			fbo_03.end();
			tt0 = ofGetElapsedTimeMillis();
		}
		fNo_03++;
	}// end sc03


	 //                                                                           ...........
	 // .. __ .. __  .. __ .. __ .. __ .. __ .. __ .. __ .. __ .. __ --- --- --- |SCENE 04  <<
	 //                                                                           ···········
	 // video + glitch
	if (sc4) {
		cam.update();
		fbo_04a.begin();
		shader_kale.begin();
		shader_kale.setUniform1i("ksectors", ksectors);
		shader_kale.setUniform1f("kangleRad", ofDegToRad(kangle));
		shader_kale.setUniform2f("kcenter", kx*w, ky*h);
		shader_kale.setUniform2f("screenCenter", 0.5*w, 0.5*h);
		if (cam.isFrameNew()) {
			cam.draw(0, 0, w, h);
		}
		shader_kale.end();
		fbo_04a.end();

		fbo_04b.begin();
		ofBackground(ofColor::black);
		draw3D();
		fbo_04b.end();

		vector<ofPoint> &vertices = sphere.getMesh().getVertices();
		ofPixels pixels;

		fbo_04a.readToPixels(pixels);
		for (int i = 0; i<vertices.size(); i++) {
			ofPoint v = vertices0[i];
			v.normalize();
			if (fx1) {
				vertices[i] = v * (40 + (20 * mB[i % (nBands)] - 20));
			} else {
				v *= rad;
				vertices[i] = v;
				ofVec2f t = sphere.getMesh().getTexCoords()[i];
				t.x = ofClamp(t.x, 0, pixels.getWidth() - 1);
				t.y = ofClamp(t.y, 0, pixels.getHeight() - 1);
				float br = pixels.getColor(t.x, t.y).getBrightness();
				//vertices[i] = v * (mB[i % (nBands)]) + br/255.0*extrude;
				vertices[i] *= 1 + br / 255.0 * extrude;
			}
		}
		fNo_04++;
	}// end sc04
}


// ---------- ---------- ----------   ----------   ---------- ---------- -------- - ..............
//---------- ---------- ----------   ----------   ---------- ---------- ---------- | >> DRAWWwW  //
// ---------- ---------- ----------   ----------   ---------- ---------- -------- - ··············
void ofApp::draw() {
	ofBackground(ofColor::black);
	ofSetColor(255, 255);

	//                                                                           ...........
	// .. __ .. __  .. __ .. __ .. __ .. __ .. __ .. __ .. __ .. __ --- --- --- |SCENE 00  <<
	//                                                                           ···········
	if (sc0) {
		ofSetColor(ofClamp(fNo_00, 0, 255));
		ofSetColor(255, 255);
		fbo_00.draw(0, 0, w, h);
	}
	
	//                                                                           ...........
	// .. __ .. __  .. __ .. __ .. __ .. __ .. __ .. __ .. __ .. __ --- --- --- |SCENE 01  <<
	//                                                                           ···········
	if (sc1) {
		ofSetColor(ofClamp(fNo_01, 0, 255));
		ofLine(-10 * w, h, -1000, 20 * w, h, -1000);
		for (int i = 0; i < stars.size(); i++) {
			ofRect(stars[i], 3, 3);
		}
		shaderMoon.begin();
		shaderMoon.setUniform1f("time", ofGetElapsedTimef());
		shaderMoon.setUniform2f("resolution", w, h);
		ofCircle(moonCenter, 144);
		shaderMoon.end();
		fbo_01.draw(0, 0, w, h);

	}

	//                                                                           ...........
	// .. __ .. __  .. __ .. __ .. __ .. __ .. __ .. __ .. __ .. __ --- --- --- |SCENE 02  <<
	//                                                                           ···········
	if (sc2) {
		ofSetColor(255, 255);

		glow.setRadius(10);
		glow.draw(0, 0, w, h);

		myGlitch.generateFx();
		fbo_02b.draw(0, 0, w, h);
	}

	//                                                                           ...........
	// .. __ .. __  .. __ .. __ .. __ .. __ .. __ .. __ .. __ .. __ --- --- --- |SCENE 3  <<
	//                                                                           ···········
	if (sc3) {
		ofSetColor(ofClamp(fNo_03, 0, 255));
		fbo_03.draw(0, 0, w, h);
	}

	//                                                                           ...........
	// .. __ .. __  .. __ .. __ .. __ .. __ .. __ .. __ .. __ .. __ --- --- --- |SCENE 04  <<
	//                                                                           ···········
	if (sc4) {
		ofSetColor(ofClamp(fNo_04, 0, 255));
		fbo_04b.draw(0, 0);
	}
	
	if (showGui) {
		gui.draw();
	}
}



//                                                                                  ............
//---------- ---------- ----------   ----------   ---------- ---------- ---------- | >> EXIT  //
//                                                                                  ············
void ofApp::exit() {
	gui.saveToFile("settings.xml");
	kinect.close();
}



//                                                                                  ..................
//---------- ---------- ----------   ----------   ---------- ---------- ---------- | >> OTHER_DRaWzZ //
//                                                                                  ··················
void ofApp::draw3D() {
	float time = ofGetElapsedTimef();
	float longitude = -5 * time;
	float latitude = 10 * sin(time*0.031);
	float radius = 600 - 20 * sin(time*0.1052);
	if (fx1) {
		latitude = 90;
		radius = 200 + 200 * sin(time*0.1052);
	}

	fbo_04a.getTextureReference().bind();

	//light.setPosition(w / 2, h / 2, 200);
	//light.enable();
	material.begin();
	ofEnableDepthTest();

	eCam.orbit(longitude, latitude, radius, ofPoint(0, 0, 0));
	eCam.begin();


	ofSetColor(ofColor::white);
	ofSetLineWidth(1);
	if (fx1) {
		serp = sphere.getMesh();
		serp.setMode(OF_PRIMITIVE_POINTS);
		serp.drawWireframe();
	}
	else {
		sphere.drawWireframe();
	}

	eCam.end();

	ofDisableDepthTest();
	material.end();
	//light.disable();
	//ofDisableLighting();

	fbo_04b.getTextureReference().unbind();
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'h') {
		showGui = !showGui;
	}
	if (key == 'f') {
		ofToggleFullscreen();
	}
	if (key == 'x') {
		fx1 = !fx1;
	}
	if (key == '0') {
		sc0 = true;
		sc1 = false;
		sc2 = false;
		sc3 = false;
		sc4 = false;
		fNo_00 = 0;
		index_00 = 0;
		tt0 = ofGetElapsedTimeMillis();
	}

	if (key == '1') {
		sc0 = false;
		sc1 = true;
		sc2 = false;
		sc3 = false;
		sc4 = false;
		fNo_01 = 0;
		stars.resize(1);
		tt0 = ofGetElapsedTimeMillis();
		show2D = 255;
	}

	if (key == '2') {
		sc0 = false;
		sc1 = false;
		sc2 = true;
		sc3 = false;
		sc4 = false;
		fNo_02 = 0;
		tt0 = ofGetElapsedTimeMillis();
	}

	if (key == '3') {
		sc0 = false;
		sc1 = false;
		sc2 = false;
		sc3 = true;
		sc4 = false;
		fNo_03 = 0;
		index_03 = 0;
		tt0 = ofGetElapsedTimeMillis();
	}

	if (key == '4') {
		sc0 = false;
		sc1 = false;
		sc2 = false;
		sc3 = false;
		sc4 = true;
		fNo_04 = 0;
		tt0 = ofGetElapsedTimeMillis();

	}

	if (key == '7') {
		sc0 = false;
		sc1 = false;
		sc2 = false;
		sc3 = false;
		sc4 = false;
		fNo_04 = 0;
		tt0 = ofGetElapsedTimeMillis();

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



//                                                                                  ............
//---------- ---------- ----------   ----------   ---------- ---------- ---------- | >> AUDIO //
//                                                                                  ············
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