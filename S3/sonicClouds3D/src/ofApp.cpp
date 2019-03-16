/**
 *
 Digital 3D collages
 */


#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	ofBackground(0,0,0);
	w = ofGetWidth();
	h = ofGetHeight();

	// ------ ------ ------ ------ ------ ------ | images stuff |
	fbo_a.allocate(w, h, GL_RGBA);
	fbo_a.begin(); ofClear(0, 0); fbo_a.end();

	nImgs = dir.listDir("../../../../../../python/clouds/clouds_10");
	dir.sort();
	seq_a.resize(nImgs);
	for (int i = 0; i < nImgs; i++) {
		string fn = dir.getPath(i);
		seq_a[i].load(fn);
		cout << "[img]: " << i << "/" << nImgs << "\t";
	}
	iw = seq_a[0].getWidth();
	ih = seq_a[0].getHeight();
	cout << "\n[images]" << endl;

	// ------ ------ ------ ------ ------ ------ | mesh stuff |
	scale = 5;
	npm_x = iw / scale;
	npm_y = ih / scale;
	cout << "\n[npm:]" << npm_x << ',' << npm_y << endl;

	// placeholder for pix and colour data
	for (int y = 0; y < npm_y; y+=1){
		for (int x = 0; x <npm_x; x+=1){
			int ii_x = x * scale;
			int ii_y = y * scale;
			mesh.addVertex(ofPoint(ii_x, ii_y,0));	
			mesh.addColor(ofFloatColor(0, 0, 0)); 
			//cout << "[v]:" << ii_x << ',' << ii_y << "\t";
		}
	}
	cout << "\n[placeholders]" << endl;

	for (int y = 0; y < npm_y-1; y++){
		for (int x=0; x < npm_x-1; x++){
			mesh.addIndex(x+y*npm_x);				// 0
			mesh.addIndex((x+1)+y*npm_x);			// 1
			mesh.addIndex(x+(y+1)*npm_x);			// 10
			
			mesh.addIndex((x+1)+y*npm_x);			// 1
			mesh.addIndex((x+1)+(y+1)*npm_x);		// 11
			mesh.addIndex(x+(y+1)*npm_x);			// 10
			//cout << "[i]:" << x << ',' << y << "\t";
		}
	}
	cout << "\n[mesh]" << endl;
	//camflip
	//cam.setScale(1,-1,1);
	extrusionAmount = 500.0;
	t0 = ofGetElapsedTimeMillis();
	t1 = t0;
	i_1 = 0;
	fp = 40;

	// ... ... ... --- --- ... ... ... --- --- ... ... --- | rec |
	isRec = false;
	recorder.setPrefix(ofToDataPath("rec/frame_"));
	recorder.setFormat("jpg");
}

//--------------------------------------------------------------
void ofApp::update(){
	// nums
	t = ofGetElapsedTimeMillis();
	tt = ofGetElapsedTimef();

	std::stringstream strm;
	strm << "[sonicClouds3D]: " << ofGetFrameRate() << "fps.";
	ofSetWindowTitle(strm.str());

	//grab a new frame
	//vidGrabber.update();

	t1 = ofGetElapsedTimeMillis();
	if ((t1 - t0) > fp) {
		i_1++;
		if (i_1 > nImgs - 1) i_1 = 0;
		pixs = seq_a[i_1].getPixels();
		for (int y = 0; y < npm_y; y+=1) {
			for (int x = 0; x < npm_x; x+=1) {
				int ii_x = x * scale;
				int ii_y = y * scale;
				int ii = ii_x + iw * ii_y;
				ofFloatColor cColor(pixs[ii*3] / 255.f,				// r
									pixs[ii*3 + 1] / 255.f,			// g
									pixs[ii*3 + 2] / 255.f);			// b

				int iii = x + npm_x * y;
				ofVec3f tmpVec = mesh.getVertex(iii);
				float bb = cColor.getBrightness();

				if (bb < 0.40) {
					tmpVec.z = -300;
					mesh.setVertex(iii, tmpVec);
					mesh.setColor(iii, ofColor(0,0,0,0));
				}
				else {
					tmpVec.z = -300 + bb * 1024;
					mesh.setVertex(iii, tmpVec);
					float c = ofMap(bb, 0.4, 0.95, 0, 255);
					mesh.setColor(iii, ofColor(c,c,c));
				}
			}
		}
		t0 = ofGetElapsedTimeMillis();
	}

	// ------ ------ ------ ------ ------ ------ | drawing stuff |
	fbo_a.begin();
	ofClear(0, 0);

	ofEnableDepthTest();
	cam.begin();
	ofPushMatrix();
	ofTranslate(-iw / 2, -ih / 2, 0);

	//seq_a[i_1].bind();
	mesh.drawFaces();

	ofPopMatrix();
	cam.end();
	ofDisableDepthTest();
	
	fbo_a.end();
}

//--------------------------------------------------------------
void ofApp::draw(){

	fbo_a.draw(0, 0);

	// r + sh
	if (isRec) {
		tImg.grabScreen(0, 0, w, h);
		recorder.addFrame(tImg);
	}

	//draw framerate for fun
	ofSetColor(255);
	string msg = "fps: " + ofToString(ofGetFrameRate(), 2);
	ofDrawBitmapString(msg, 10, 20);
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key) {
		case 'f':
			ofToggleFullscreen();
			break;
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == ' ') {
		fbo_a.begin();
		ofClear(0, 0);
		fbo_a.end();
	}
	if (key == 't') {
		if (recorder.isThreadRunning()) {
			recorder.stopThread();
		}
		else {
			recorder.startThread(false, true);
		}
	}
	if (key == 'r') isRec = !isRec;
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
