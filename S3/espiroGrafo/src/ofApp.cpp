#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	h = ofGetHeight();
	w = ofGetWidth();
	ofSetFrameRate(300);
	ofBackground(0);
	ofSetVerticalSync(true);


	// .. __ .. __  .. __ .. __ .. __ .. __ .. __ .. __ .. __ .. __ --- --- --- OSC
	receiver.setup(OSC_IN_PORT);
	cout << "d[OSC]b :: " << OSC_IN_PORT << "\n";

	// .. __ .. __  .. __ .. __ .. __ .. __ .. __ .. __ .. __ .. __ --- --- --- GUI
	showGui = true;
	gui.setup();

	mindGroup.setup("mindGroup");
	mindGroup.setHeaderBackgroundColor(ofColor::blue);
	mindGroup.setBorderColor(ofColor::blue);
	mindGroup.add( med.setup("med", 1, 1, 100) );
	mindGroup.add( att.setup("att", 1, 1, 100) );
	gui.add(&mindGroup);

	// .. __ .. __  .. __ .. __ .. __ .. __ .. __ .. __ .. __ .. __ --- --- --- FBO
	fbo1.allocate(w, h, GL_RGBA);
	fbo2.allocate(w, h, GL_RGBA);
	fbo1.begin();
	ofClear(0, 0);
	fbo1.end();
	fbo2.begin();
	ofClear(0, 0);
	fbo2.end();

	// custom limits
	cX = w / 2;
	cY = h / 2;
	b01 = int(ofRandom(1, 10));
	b02 = int(ofRandom(155, 165));
	b03 = int(ofRandom(40, 60));
	b04 = int(ofRandom(60, 80));
	b05 = int(ofRandom(80, 100));

	//staring
	pCount = 3;
	steps = 4;
	outRad = w - 300;
	inRad = outRad * 0.5;

	outRatio = outRad / steps;
	inRatio = inRad / steps;
	rCol = ofRandom(225, 255);
	shadeRatio = rCol / steps;
	rotRatio = ofRandom(90, 200) / steps;
}

//--------------------------------------------------------------
void ofApp::update(){
	t = ofGetElapsedTimeMillis();
	tt = ofGetElapsedTimef();
	f = ofGetFrameNum();
	// .. __ .. __  .. __ .. __ .. __ .. __ .. __ .. __ .. __ .. __ --- --- --- OSC
	while (receiver.hasWaitingMessages()) {
		ofxOscMessage m;
		receiver.getNextMessage(m);
		if (m.getAddress() == "/cc/1") {
			cc1 = m.getArgAsFloat(0);
			cout << "cc1: " << cc1 << "\n";
		}
		else if (m.getAddress() == "/cc/2") {
			cc2 = m.getArgAsFloat(0);
			cout << "cc2: " << cc2 << "\n";
		}
		else if (m.getAddress() == "/note") {
			note = m.getArgAsInt32(1);
			cout << "note: " << note<< "\n";
		}
	}

	// lerpink kween   
	a01 = ofLerp(a01, med, 0.1);
	a02 = ofLerp(a02, att, 0.1);

	// drawink
	fbo1.begin();
	ofClear(0, 0);

	if (a01>0) {
		float r1 = ofMap(ofClamp(a01, 0, 20), 0, 20, 40, 20);
		float r2 = ofMap(ofClamp(a01, 0, 20), 0, 20, 40, b02);
		float d0 = att;
		float e0 = 50;
		draw_spiro_in(cX, cY, r1, r2, d0, 4);

		if (a01>20) {
			r1 = ofMap(ofClamp(a01, 20, 40), 20, 40, 40, b02);;
			r2 = r1*ofMap(r1, 40, b02, 0.5, 2.5);
			d0 = 10 * 20;
			draw_star(cX, cY, r1, r2, pCount);
			
		}
		if (a01>40) {
			r1 = ofMap(ofClamp(a01, 40, 60), 40, 60, 1, 7 * b03);
			r2 = b01;
			d0 = 80;
			draw_spiro_in(cX, cY, r1, r2, d0, e0);
		}
		if (a01>60) {
			r1 = ofMap(ofClamp(a01, 40, 60), 40, 60, 1, 7 * b03);
			r2 = 300;
			d0 = a02;
			draw_spiro_ex(cX, cY, r1, r2, d0, e0);
		}
		else if (a01>80) {
		}
		else if (a01>95) {
		}
	}
	fbo1.end();

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(ofColor::black);
	ofSetColor(255, 255);

	fbo1.draw(0, 0, w, h);
	
	if (showGui) {
		gui.draw();
	}
}


//--------------------------------------------------------------
void ofApp::draw_spiro_in(int cx, int cy, float r1, float r2, float d, float e) {
	ofPolyline line;
	
	ofPushMatrix();
	ofTranslate(cx, cy);
	ofRotateZ(f * (PI / 360));
	ofSetColor(255, 200);
	ofSetLineWidth(1);
	for (int t = 0; t<180 * e; t++) {
		float px = (r1 - r2) * cos(t);
		float py = (r1 - r2) * sin(t);
		float nx = px - d * cos(t*(r2 - r1) / r2);
		float ny = py - d * sin(t*(r2 - r1) / r2);
		//line.addVertex(ofVec2f(nx, ny));
		
		ofLine(px, py, nx, ny);
	
 	}
	line.close();
	ofPopMatrix();

	//line.draw();
	
}

void ofApp::draw_spiro_ex(int cx, int cy, float r1, float r2, float d, float e) {
	ofPolyline line;
	for (int t = 0; t<360 * e; t++) {
		float px = (r1 + r2) * cos(t);
		float py = (r1 + r2) * sin(t);
		float nx = px + d * cos(t*(r2 + r1) / r2);
		float ny = py + d * sin(t*(r2 + r1) / r2);
		ofPushMatrix();
		ofTranslate(cx, cy);
		ofRotateZ(f * (-PI / 3600));
		ofSetColor(255, 100);
		ofSetLineWidth(0.2);
		ofLine(px, py, 0, nx, ny, 0);
		//stroke(0,255,0, 50);
		//strokeWeight(1);
		//ellipse(nx, ny, 2,2);
		ofPopMatrix();
	}
	line.close();
}

void ofApp::draw_star(int cx, int cy, int r1, int r2, int np) {
	float a = 0.0;
	int nVert = np*2;
	float rot = TWO_PI / nVert;
	float tRad = 0.0;
	float x = 0.0, y = 0.0;
	ofPushMatrix();
	ofTranslate(cx, cy);
	ofRotateZ(f * PI/360);
	//ofScale(0.5, 0.5, 0);
	ofSetLineWidth(2);
	for (int k = 0; k<steps; k++) {
		float nRatio = rotRatio - (f / (k + 1))*PI / 180;
		ofPushMatrix();
		ofScale(0.8, 0.8, 0);
		ofRotateZ(nRatio * k * PI / 180);
		ofPolyline line;
		inRad = r1 - inRatio*k;
		outRad = r2 - outRatio*k;
		for (int i = 0; i<np; i++) {
			for (int j = 0; j<2; j++) {
				tRad = inRad;
				// true if j is even
				if (j % 2 == 0) tRad = outRad;
				x = cos(a)*tRad;
				y = sin(a)*tRad;
				line.addVertex(ofVec2f(x, y));
				a += rot;
			}
		}
		line.close();
		ofSetColor(shadeRatio * k, 200);
		ofFill();
		line.draw();
		ofPopMatrix();
	}
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	switch (key) {
	case 'h':
		showGui = !showGui;
		break;
	case 'n':
		b01 = int(ofRandom(1, 10));
		b02 = int(ofRandom(155, 165));
		b03 = int(ofRandom(40, 60));
		b04 = int(ofRandom(60, 80));
		b05 = int(ofRandom(80, 100));
		break;
	case 'g':
		ofSaveScreen(ofToDataPath(ofToString(ofGetUnixTime()) + ".png"));
		break;
	case 'r':
		pCount = int(ofRandom(3, 9));
		steps = int(ofRandom(5, 19));
		outRad = w- 400;
		inRad = outRad * ofRandom(.45, .6);

		outRatio = outRad / steps;
		inRatio = inRad / steps;

		rCol = ofRandom(225, 255);
		shadeRatio = rCol / steps;
		rotRatio = ofRandom(90, 300) / steps;
		break;
	default:
		break;
	}
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
