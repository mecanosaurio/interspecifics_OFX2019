#include "ofApp.h"
/*
 * 
 *
 *
 */


//--------------------------------------------------------------
void ofApp::setup(){
	w = ofGetWidth();
	h = ofGetHeight();
	ofSetFrameRate(24);
	ofSetVerticalSync(true);
	//- - - | dsp |
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
	nBands = 512;
	dBuffSize = 16;
	d0.resize(nBands);
	mB.resize(nBands);
	dataBuff.resize(dBuffSize);
	for (int n = 0; n < dBuffSize; n++) { dataBuff[n].resize(nBands); }
	softRMS = 0;
	//- - -|fbos|-
	fboSceneA.allocate(w, h, GL_RGBA);
	fboSceneA.begin(); ofClear(0, 0); fboSceneA.end();
	fboSceneB.allocate(w, h, GL_RGBA);
	fboSceneB.begin(); ofClear(0, 0); fboSceneB.end();
	fboSceneC.allocate(w, h, GL_RGBA);
	fboSceneC.begin(); ofClear(0, 0); fboSceneC.end();
	fboSceneD.allocate(w, h, GL_RGBA);
	fboSceneD.begin(); ofClear(0, 0); fboSceneD.end();
	fboSceneTmp.allocate(w, h, GL_RGBA);
	fboSceneTmp.begin(); ofClear(0, 0); fboSceneTmp.end();
	
	//- - -| gui |
	gui.setup();
	gui.setName("[INT E R SPEC I F I CS]");
	//-
	sceneA_group.setup();
	sceneA_group.setName("[escena_A]");
	sceneA_group.setBorderColor(ofColor::greenYellow);
	sceneA_group.setFillColor(ofColor::black);
	sceneA_group.setTextColor(ofColor::whiteSmoke);
	sceneA_group.setHeaderBackgroundColor(ofColor::darkGray);
	sceneA_group.setBackgroundColor(ofColor(0, 0));
	sceneA_group.add(iFA1.setup("fqIx-A1", 45, 0, 128));
	sceneA_group.add(amA1.setup("amp-A1", 4, 00.001, 20.0));
	sceneA_group.add(iFA2.setup("fqIx-A2", 31, 0, 128));
	sceneA_group.add(amA2.setup("amp-A2", 4, 0.001, 20.0));
	sceneA_group.add(iFA3.setup("fqIx-A3", 71, 0, 128));
	sceneA_group.add(amA3.setup("amp-A3", 4, 0.001, 20.0));
	sceneA_group.add(bgClearA.setup("bg Clear", true));
	sceneA_group.add(thrBlendScreenA.setup("thr BlendSCREEN", 0.5, 0.01, 5.0));
	//sceneA_group.add(cA1.setup("c-A1", ofColor(127, 0, 0, 92), ofColor(0, 0), ofColor(255, 255)));
	gui.add(&sceneA_group);
	//-
	sceneB_group.setup();
	sceneB_group.setName("[escena_B]");
	sceneB_group.setBorderColor(ofColor::greenYellow);
	sceneB_group.setFillColor(ofColor::black);
	sceneB_group.setHeaderBackgroundColor(ofColor::slateGray);
	sceneB_group.setTextColor(ofColor::whiteSmoke);
	sceneB_group.add(indexFreqB1.setup("fqIx-B1 [vel]", 47, 0, 128));
	sceneB_group.add(indexFreqB2.setup("fqIx-B2 [size]", 11, 0, 128));
	sceneB_group.add(indexFreqB3.setup("fqIx-B3 [color]", 5, 0, 128));
	sceneB_group.add(amplitudB1.setup("[vel]", 500, 1, 5000));
	sceneB_group.add(amplitudB2.setup("[size]", 1000, 1, 10000));
	sceneB_group.add(amplitudB3.setup("[hue]", 58, 0, 255));
	gui.add(&sceneB_group);
	//-
	sceneC_group.setup();
	sceneC_group.setName("[escena_C]");
	sceneC_group.setBorderColor(ofColor::greenYellow);
	sceneC_group.setFillColor(ofColor::black);
	sceneC_group.setHeaderBackgroundColor(ofColor::darkSlateGray);
	sceneC_group.setTextColor(ofColor::whiteSmoke);
	sceneC_group.add(indexFreqC1.setup("fqIx-C1", 2, 0, 128));
	sceneC_group.add(indexFreqC2.setup("fqIx-C2", 4, 0, 128));
	sceneC_group.add(indexFreqC3.setup("fqIx-C3", 8, 0, 128));
	sceneC_group.add(amplitudC1.setup("[ampC1]", 100, 1, 5000));
	sceneC_group.add(amplitudC2.setup("[ampC2]", 200, 1, 5000));
	sceneC_group.add(amplitudC3.setup("[ampC3]", 400, 0, 5000));
	gui.add(&sceneC_group);
	//.
	mixer_group.setup();
	mixer_group.setName("[_ .mixer. _]");
	mixer_group.add(enA.setup("enable-A", false));
	mixer_group.add(alphaA.setup("alpha-A", 0, 0, 255));
	mixer_group.add(enB.setup("enable-B", false));
	mixer_group.add(alphaB.setup("alpha-B", 0, 0, 255));
	mixer_group.add(enC.setup("enable-C", false));
	mixer_group.add(alphaC.setup("alpha-C", 255, 0, 255));
	gui.add(&mixer_group);

	//- - -|other stuff|-
	enDsp = true;
	enGui = true;
	cout << "OK1: INIT"<< endl;
	
	// init scene A
	box1.set(100);
	box2.set(100);
	box3.set(100);

	// init scene B
	cbX = 0;
	cbY = 0;

	// init scene C
	nImgs = 100;
	for (int i = 0; i < nImgs; i++) {
		float r = ofRandom(200, 700);
		listaPos[i] = ofPoint(r * cos(2 * r * PI / 100), r * sin(2 * r * PI / 100),	ofRandom(-100, -500));
		listaVelocs[i] = ofPoint(0, 0, ofRandom(1,10));
		//cout << listaPos[i] << "-" << listaVelocs[i] << "   " << endl;
	}
	imageC1.load("imgs/object073.png");
	imageC2.load("imgs/object146.png");
	imageC3.load("imgs/object157.png");
	imageC4.load("imgs/objectRO.png");
	imageC5.load("imgs/objectSU.png");
}




//--------------------------------------------------------------
void ofApp::update(){
	t = ofGetElapsedTimeMillis();
	tt = ofGetElapsedTimef();
	ff = ofGetFrameNum();
	std::stringstream strm;
	strm << "[guiTemplate]: " << ofGetFrameRate() << " fps";
	ofSetWindowTitle(strm.str());
	//- - -| osc |
	while (recvOsc.hasWaitingMessages()) {
		ofxOscMessage m;
		recvOsc.getNextMessage(m);
		if (m.getAddress() == "/template/CC0") {
			v1 = m.getArgAsFloat(0);
			cout << "[CC0]: " << v1 << "\n";
		}
	}		
	//- - -| dsp |
	if (enDsp == true) {
		for (int b = 0; b < nBands; b++) { 
			if (mfft.magnitudes[b] > 0) { 
				d0[b] = mfft.magnitudes[b]; 
			} 
		}
		for (int n = dBuffSize - 1; n > 0; n--) { 
			dataBuff[n] = dataBuff[n - 1]; 
		}
		dataBuff[0] = d0;
		for (int b = 0; b < nBands; b++) {
			mB[b] = 0;
			for (int n = 0; n < dBuffSize; n++) { mB[b] += dataBuff[n][b]; }
			mB[b] /= dBuffSize;
		}
		softRMS = ofLerp(softRMS, RMS, 0.2);
	}

	//- - -|actualiza las escenas que estén habilitadas|-
	
	if (enA == true) updateSceneA();
	if (enB == true) updateSceneB();
	if (enC == true) updateSceneC();
	if (enD == true) updateSceneD();

}




//--------------------------------------------------------------
void ofApp::updateSceneA() {
	// 0.comienza a dibujar en el frameBuffer de la escena_A
	fboSceneA.begin();
	// 1. limpia o no fondo
	if (bgClearA == true) {
		ofClear(0, 192);
	}
	// 2. cambia blendMode si RMS>thr
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	if(RMS>thrBlendScreenA)	ofEnableBlendMode(OF_BLENDMODE_SCREEN);
	// 3. actualiza la posiciones de cajas
	box1.setPosition(w / 2, h / 2, 20);
	box2.setPosition(w / 2, h / 2, 20);
	box3.setPosition(w / 2, h / 2, 20);
	// 4. actualiza tamaño de cajas
	szBox1 = 1 + mB[iFA1] * w * amA1;
	szBox2 = 1 + mB[iFA2] * w * amA2;
	szBox3 = ofLerp(szBox3, 1 + d0[iFA3] * w * amA3, 0.4);
	box1.set(szBox1);
	box2.set(szBox2);
	box3.set(szBox3);
	// 5. actualiza cantidad y eje de rotacion de cajas
	box1.rotate(ofMap(mB[iFA3], 0, 1, -2.5, 2.5), 0.0, -1.0, -1.0);
	box2.rotate(ofMap(mB[iFA3], 0, 1, -1.0, 1.0), 1.0, -1.0, 0.0);
	box3.rotate(ofMap(mB[iFA3], 0, 1, -0.3, 0.3), -1.0, 1.0, -1.0);
	// 6. colorea cajas
	ofNoFill();
	ofSetLineWidth(5);
	tmp01 = ofLerp(tmp01, peakFreq, 0.3);
	colorBox1 = ofColor::fromHsb(128 + ofMap(tmp01, 0, 500, 28, -108), 255, 255, 64);
	colorBox2 = ofColor::fromHsb(ofMap(mB[iFA1] * amA1, 0.0001, 1, 255, 128), 255, 255, 64);
	colorBox3 = ofColor::fromHsb((ff % 255), 255, 255, 64);
	// 7. dibuja objetos en el buffer
	ofSetColor(colorBox1);
	box1.draw();
	ofSetColor(colorBox2);
	box2.draw();
	ofSetColor(colorBox3);
	box3.draw();
	// termina blendMode y frameBuffer de escena_A
	ofDisableBlendMode();
	fboSceneA.end();
}


//--------------------------------------------------------------
void ofApp::updateSceneB() {
	// 0. comienza a dibujar en el frameBuffer de la escena_B
	fboSceneB.begin();
	// 1. calcula velocidad y tamaño a partir de los coeficientes de frecuencia 1 y 2
	// d0[] son los datos en tiempo real, mB[] son datos acumulados que cambian mas lento
	float vel = (0.01+d0[indexFreqB1]) * amplitudB1;
	float siz = (0.01+mB[indexFreqB2]) * amplitudB2;
	// 2. calcula desplazamiento a la derecha de la coordenada cbX
	// cuando llega al borde (cbx>=w), regresa al extremo izquierdo
	if (cbX < w) {
		cbX += vel;
	} else {
		cbX -= (w+100);
	}
	// 3. calcula el color en funcion del control hue y el tercer indice de frecuencia
	int hue = amplitudB3 + ofNoise(tt) * 128;
	int bright = 255;
	int saturation = int(ofMap(d0[indexFreqB3], 0, 2, 255, 192));
	int alpha = int(ofMap(d0[indexFreqB3], 0, 0.5, 32, 255));
	// 4. establece el color de los objetos
	ofSetColor(ofColor::fromHsb(hue, saturation, bright, alpha));
	// 5. dibuja rectangulo de altura h, y amplitud variable siz
	ofDrawRectangle(cbX, 0, siz, h);
	// 6. dibuja minirectangulos en borde superior e inferior
	ofDrawRectangle(w - cbX, 0, -siz, 6);
	ofDrawRectangle(w - cbX, h, -siz, -6);
	// termina de dibujar en el frameBuffer de la escena_B
	fboSceneB.end();
}


//--------------------------------------------------------------
void ofApp::updateSceneC() {
	// 0. comienza a dibujar en el frameBuffer de la escena_C
	fboSceneC.begin();
	ofEnableAlphaBlending();
	ofClear(0, 0);
	// 1 actualizar posiciones, 
	// si un objeto sale del campo visual reinicialo con nuevas pos y veloc
	for (int i = 0; i < nImgs; i++) {
		listaPos[i].z -= listaVelocs[i].z;
		if (listaPos[i].z < -1000) {
			listaPos[i].z = 100;
			listaVelocs[i].z = softRMS * 250;
		}
	}
	// 2. recorre la lista de elementos
	for (int i = 0; i < nImgs; i++) {
		ofPushMatrix();
		// 3. trasladate a la posición z de cada elemento
		ofTranslate(w / 2, h / 2, listaPos[i].z);
		// 4. calcula nuevos tamaños en función de los coeficientes de frecuencia mB y d0
		float sImgC1 = mB[indexFreqC1] * amplitudC1;
		float sImgC2 = mB[indexFreqC2] * amplitudC2;
		float sImgC3 = d0[indexFreqC3] * amplitudC3;
		float sImgC4 = mB[indexFreqC3] * amplitudC3;
		float sImgC5 = mB[indexFreqC2] * amplitudC1;
		// 5. calcula posiciones centradass
		ofPoint posCentradaImgC1 = listaPos[i] - ofPoint(sImgC1 / 2, sImgC1 / 2, 0);
		ofPoint posCentradaImgC2 = listaPos[i] - ofPoint(sImgC2 / 2, sImgC2 / 2, 0);
		ofPoint posCentradaImgC3 = listaPos[i] - ofPoint(sImgC3 / 2, sImgC3 / 2, 0);
		ofPoint posCentradaImgC4 = listaPos[i] - ofPoint(sImgC4 / 2, sImgC4 / 2, 0);
		ofPoint posCentradaImgC5 = listaPos[i] - ofPoint(sImgC5 / 2, sImgC5 / 2, 0);
		// 6. dibuja imagenes distintas imageC1, imageC2... en cada caso
		if (i % 5 == 0) {
			imageC1.draw(posCentradaImgC1, sImgC1, sImgC1); 
		}
		else if (i % 4 == 1) { 
			imageC2.draw(posCentradaImgC2, sImgC2, sImgC2); 
		}
		else if(i%5 == 2){ 
			imageC3.draw(posCentradaImgC3, sImgC3, sImgC3); 
		}
		else if (i % 4 == 3) {
			ofPushMatrix();
			ofRotateZ(-mB[indexFreqC3]*PI*2);
			imageC4.draw(posCentradaImgC4, sImgC4, sImgC4);
			ofPopMatrix();
		}
		else {
			imageC5.draw(posCentradaImgC5, sImgC5, sImgC5);
		}
		ofPopMatrix();
	}
	// desabilita blending y termina dibujo enframeBuffer de escena_B
	ofDisableAlphaBlending();
	fboSceneC.end();
}


//--------------------------------------------------------------
void ofApp::updateSceneD() {
	fboSceneD.begin();
	ofClear(0, 0);
	// dibuja tu propia escena aqui
	// -.+
	// -.+
	fboSceneD.end();
}








//--------------------------------------------------------------
void ofApp::draw() {
	ofBackgroundGradient(ofColor::black, ofColor::darkSlateBlue);
	// dibuja aqui en el frame final con los framebuffers de cada escena aplicando alphaBlending
	if (enA == true) {
		ofSetColor(255, alphaA);
		fboSceneA.draw(0, 0);
	}
	if (enB == true) {
		ofSetColor(255, alphaB);
		fboSceneB.draw(0, 0);
	}
	if (enC == true) {
		ofSetColor(255, alphaC);
		fboSceneC.draw(0, 0);
	}
	//if (enD == true) {
	//	ofSetColor(255, alphaD);
	//	fboSceneD.draw(0, 0);
	//}

	// al final dibuja la gui cuando esté habilitada
	if (enGui) {
		gui.draw();
	}
}






//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
	case 'h':
		enGui = !enGui;
		cout << "enGui is " << enGui << endl;
		break;
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
	w = ofGetWidth();
	h = ofGetHeight();
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