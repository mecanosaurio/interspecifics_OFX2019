#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	h = ofGetHeight();
	w = ofGetWidth();

	fbo1.allocate(w, h , GL_RGBA);
	fbo2.allocate(w, h, GL_RGB);
	fbo3D.allocate(w, h, GL_RGBA);

	sphere.set(250, 80);
	vertices0 = sphere.getMesh().getVertices();
	sphere.mapTexCoords(0,h,w,0);
	sphere.rotate(180, 0, 1, 0);

	img1.loadImage("37.jpg");
	//img1.loadImage("img02.png");
	shader.load("kaleido");

	/*
	video01.loadMovie("slime.mp4");
	video01.setLoopState(OF_LOOP_NORMAL);
	video01.play();
	*/

	gui.setup();
	 
	mixerGroup.setup("mixer");
	mixerGroup.setHeaderBackgroundColor(ofColor::blue);
	mixerGroup.setBorderColor(ofColor::blue);
	mixerGroup.add(show2D.setup("show2D", 255, 0, 255));
	mixerGroup.add(show3D.setup("show3D", 255, 0, 255));
	mixerGroup.add(rad.setup("rad", 255, 0, 512));
	mixerGroup.add(deform.setup("deform", 0.0, 0, 1.5));
	mixerGroup.add(deformFreq.setup("deformFreq", 3, 0, 10));
	mixerGroup.add(extrude.setup("extrude", 1, 0, 1));
	mixerGroup.add(tFactor.setup("tFactor", 1, 0, 100));

	gui.add(&mixerGroup);

	kaleidoGroup.setup("kaleidoscope");
	kaleidoGroup.setHeaderBackgroundColor(ofColor::darkRed);
	kaleidoGroup.setBorderColor(ofColor::darkRed);
	kaleidoGroup.add(kenabled.setup("kaleidoEnabled", false));
	kaleidoGroup.add(ksectors.setup("ksectors", 10, 1, 30));
	kaleidoGroup.add(kangle.setup("kangle", 0, -180, 180));
	kaleidoGroup.add(kx.setup("kx", 0.5, 0, 1));
	kaleidoGroup.add(ky.setup("ky", 0.5, 0, 1));
	gui.add(&kaleidoGroup);

	showGui = true;
	//sphere.setGlobalPosition(ofGetWidth() / 2, ofGetHeight() / 2, 0);
}

//--------------------------------------------------------------
void ofApp::update(){
	//video01.update();
	fbo2.begin();
	if (kenabled) {
		shader.begin();
		shader.setUniform1i("ksectors", ksectors);
		shader.setUniform1f("kangleRad", ofDegToRad(kangle));
		shader.setUniform2f("kcenter", kx*w, ky*h);
		shader.setUniform2f("screenCenter", 0.5*w, 0.5*h);
	}
	img1.draw(0, 0, w, h);
	//video01.draw(0, 0, w, h);
	if (kenabled) shader.end();
	fbo2.end();

	fbo3D.begin();
	ofBackground(ofColor::black);
	draw3D();
	fbo3D.end();

	vector<ofPoint> &vertices = sphere.getMesh().getVertices();
	for (int i = 0; i<vertices.size(); i++) {
		ofPoint v = vertices0[i];
		v.normalize();
		float sx = sin(v.x * deformFreq);
		float sy = sin(v.y * deformFreq);
		float sz = sin(v.z * deformFreq);
		v.x += sy * sz * deform;
		v.y += sx * sz * deform;
		v.z += sx * sy * deform;
		v *= rad;
		vertices[i] = v;
	}

	ofPixels pixels;
	fbo2.readToPixels(pixels);
	for (int i = 0; i<vertices.size(); i++) {
		ofVec2f t = sphere.getMesh().getTexCoords()[i];
		t.x = ofClamp(t.x, 0, pixels.getWidth() - 1);
		t.y = ofClamp(t.y, 0, pixels.getHeight() - 1);
		float br = pixels.getColor(t.x, t.y).getBrightness();
		vertices[i] *= 1 + br / 255.0 * extrude;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	//ofEnableBlendMode(OF_BLENDMODE_ADD);
		
	ofBackground(ofColor::black);
	ofSetColor(255, show2D);
	fbo2.draw(0,0);
	ofSetColor(255, show3D);
	fbo3D.draw(0, 0);
	
	//ofDisableBlendMode();

	if (showGui) {
		gui.draw();
	}
}


void ofApp::draw3D() {
	float time = ofGetElapsedTimef();
	float longitude = 10 * time*tFactor;
	float latitude = 10 * sin(time*0.8);
	float radius = 600 + 50 * sin(time*0.4);

	fbo2.getTextureReference().bind();

	light.setPosition(ofGetWidth()/2, ofGetHeight()/2, 200);
	//light.enable();
	material.begin();
	ofEnableDepthTest();

	eCam.orbit(longitude, latitude, radius, ofPoint(0, 0, 0));
	eCam.begin();

	ofSetColor(ofColor::white);
	//sphere.drawWireframe();
	sphere.draw();

	eCam.end();

	ofDisableDepthTest();
	material.end();
	//light.disable();
	ofDisableLighting();

	fbo2.getTextureReference().unbind();
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'h') {
		showGui = !showGui;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	if (showGui && x < 250) eCam.disableMouseInput();
	else eCam.enableMouseInput();
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
