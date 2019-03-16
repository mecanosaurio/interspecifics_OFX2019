#include "Agents.h"


void echoCircleDw(ofPoint p, float r, float am, float st, ofColor co) {
	for (int a = 0; a < am; a += st) {
		ofSetColor(co, ofMap(a, 0, am, 255, 25));
		ofPushMatrix();
		ofTranslate(0, 0, -a);
		ofCircle(p, r);
		ofPopMatrix();
	}
}

void echoCircleUp(ofPoint p, float r, float am, float st, ofColor co) {
	for (int a = 0; a < am; a += st) {
		ofSetColor(co, ofMap(a, 0, am, 255, 25));
		ofPushMatrix();
		ofTranslate(0, 0, a);
		ofCircle(p, r);
		ofPopMatrix();
	}
}

// .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. 0///////////
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -- 0///////////

dropForm::dropForm() {};
dropForm::~dropForm() {};
void dropForm::setup(vector<ofPoint> ps) {
	points.resize(0);
	points = ps;
	shape.clear();
	shape.addVertices(ps);
	shape.close();
	//c = ofColor(255, 59, 59);
	isFresh = true;
};


void dropForm::update(int mode, float vel) {
	if (mode > 0) {
		for (int i = 0; i < shape.size(); i++) {
			shape[i].z = ofLerp(shape[i].z, -200, vel);
			if (shape[i].z < -195) { 
				shape[i].z = 500; 
				isFresh = true;
			}
		}
	}
}


void dropForm::update(int mode) {
	if (mode ==0 ) {
		shape.clear();
		shape.addVertices(points);
		shape.close();
	}
}


void dropForm::draw() {
	shape.draw();
}

void dropForm::draw_echo(int sEcho, int step) {
	for (int sz = 0; sz < sEcho; sz += step) {
		ofSetColor(c, ofMap(sz, 0, sEcho, 255, 25));

		ofPushMatrix();
		ofTranslate(0, 0, sz);
		shape.draw();
		ofPopMatrix();

		ofPushMatrix();
		ofTranslate(0, 0, -sz);
		shape.draw();
		ofPopMatrix();
	}
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --

colorImgPix::colorImgPix() {};
colorImgPix::~colorImgPix() {};
void colorImgPix::setup(std::string fn) {
	fileName = fn;
	ofLoadImage(pix, fileName);
	pc = ofPoint(0, 0, 0);
}
ofColor colorImgPix::pick() {
	//pos = int(ofRandom(pix.size()));
	pc.x += 2 * ofMap(ofNoise(ofGetElapsedTimef()), 0, 0.5, -2, 2);
	pc.y += 2 * ofMap(ofNoise(ofGetElapsedTimeMillis() / 3), 0, 0.5, -2, 2);
	if (pc.x > 1000) pc.x -= 1000;
	if (pc.x < 0) pc.x += 1000;
	if (pc.y > 1000) pc.y -= 1000;
	if (pc.y < 0) pc.y += 1000;
	int ipp = (3 * (pc.y * 1023 + pc.x));
	color = pix.getColor(pc.x, pc.y);
	return color;
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --

lineWave::lineWave() {};
lineWave::~lineWave() {};

void lineWave::setup(ofPoint position, ofColor co) {
	pos = position;
	vel = ofPoint(0, 0, 0);
	n = 100;
	line.clear();
	for (int i = 0; i < n; i++) {
		line.addVertex(pos.x + i*ofGetWidth()/100, pos.y, 0);
	}
	color = co;
	isDead = false;
};

void lineWave::fall() {
	pos.x += vel.x;
	if (pos.x > ofGetWidth()) pos.x = 0;
	if (pos.x < 0) pos.x = ofGetWidth();
	pos.y += vel.y;
	if (pos.y > ofGetHeight()) isDead = true;
	if (pos.y < 0) pos.y = ofGetHeight();
	pos.z += vel.z;
	if (pos.z > 500) pos.z = 0;
	if (pos.z < -500) pos.z = 500;
};

void lineWave::popWith(vector<float> mB, float sc) {
	for (int i = 0; i < n; i++) {
		line[i].z += mB[i] * sc;
	}
};

void lineWave::update() {
	// -- updt line and mesh
	line.clear();
	line.begin();
	for (int i = 0; i < trail.size(); i++) {
		line.addVertex(pos.x + trail[i].x, pos.y + trail[i].y, pos.z + trail[i].z);
	}
	line.close();
	line.end();
};

void lineWave::draw() {
	float te = abs((ofGetHeight() / 2) - pos.y) / 20;
	//ofSetColor(color, 255 - te*te);
	ofSetColor(color, 255);
	line.draw();
};

void lineWave::redoTrail() {
	for (int i = 0; i < n; i++) {
		line[i].y += vel.y;
	}
};

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --

Block::Block() {}
Block::~Block() {}

void Block::setup(ofPoint position, int ntr) {
	pos = position;
	dest = ofPoint(ofGetWidth() / 2, pos.y, pos.z);
	spc_trail = 0;
	trail.resize(0);
	nt = ntr;
	for (int i = 0; i < nt; i++) {
		trail.push_back(ofPoint(pos.x + i*spc_trail, pos.y, pos.z));
	}
	n = int(ofRandom(3, 8));
	r = ofRandom(20, 50);
	v = ofRandom(0.01, 0.5);
	cW = 1;
	cH = 20;
	cwd = 10;
	//if (int(r) % 2 == 0) { v = -v; }
	gravity = ofMap(r, 20, 50, 5, 30);
	color = ofColor(0, 255, 0, 172);
	shape.clear();
	isDead = false;
	isExpl = false;
}

void Block::go_to() {
	pos.x = ofLerp(pos.x, dest.x, 0.04);
	for (int i = 0; i < trail.size(); i++) {
		trail[i].x = pos.x + i*spc_trail;
	}
	cW = ofLerp(cW, cwd, 0.1);
	spc_trail = ofLerp(spc_trail, spc_td, 0.1);
	// --
	pos.x += v;
	dest.x += v;
	if (pos.x >= ofGetWidth()) {
		pos.x -= (ofGetWidth() + cW);
		dest.x = pos.x;
	}
};

void Block::update(float tt) {
	pos.x = ofGetWidth() / 2 + ofMap(ofNoise(tt), 0, 1, -ofGetWidth() / 2, ofGetWidth() / 2);
}

void Block::draw() {
	//color.setHue(int(ofGetFrameNum()/10) % 255);
	ofFill();
	ofSetColor(color, 112);
	//ofSetLineWidth(4);
	//shape.draw();
	//ofSetColor(ofColor(255,0,0), 205);
	int okR = 3;
	int fn = ofGetFrameNum();
	if (fn % 240 == 0) {
		okR = int(ofRandom(trail.size()));
	}
	for (int i = 0; i < trail.size(); i++) {
		if (i != okR) {
			ofDrawRectangle(trail[i].x, pos.y, 1 + cW, cH);
			if (trail[i].x + cW > ofGetWidth()) {
				ofDrawRectangle(trail[i].x - (ofGetWidth() + cW), pos.y, pos.z, 1 + cW, cH);
			}
		}
	}
};



// .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. 1///////////
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -- 1///////////


Agent01::Agent01() {};
Agent01::~Agent01() {};

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
void Agent01::setup() {
	w = ofGetWidth();
	h = ofGetHeight();
	// ------ ------ ------ ------ ------ ------ | fbos |
	fbo_m1_front.allocate(w, h, GL_RGBA);
	fbo_m1_front.begin(); ofClear(0, 0); fbo_m1_front.end();
	fbo_m1_back.allocate(w, h, GL_RGBA);
	fbo_m1_back.begin(); ofClear(0, 0); fbo_m1_back.end();
	fbo_m1_delay.allocate(w, h, GL_RGBA);
	fbo_m1_delay.begin(); ofClear(0, 0); fbo_m1_delay.end();
	fbo_a1.allocate(w, h, GL_RGBA);
	fbo_a1.begin(); ofClear(0, 0); fbo_a1.end();
	// ------ ------ ------ ------ ------ ------ | shaders |
	string shName = "shader/pSh_02.frag";
	tShader.load("shader/basicVertex.vert", shName);

	// ------ ------ ------ ------ ------ ------ | create forms |
	tPointsA.clear();
	tPointsA.resize(0);
	shapes.resize(0);
	r = 300;
	// -> 01
	tLine.clear();
	tPoints.clear();
	for (int i = 0; i < 3; i++) {
		tPoi = ofPoint(r*cos(-PI / 6 + i*TWO_PI / 3), r*sin(-PI / 6 + i*TWO_PI / 3), 0);
		tPoints.push_back(tPoi);
		tPointsA.push_back(tPoi);
	}
	tLine.addVertices(tPoints);
	tLine.close();
	shapes.push_back(tLine);
	// -> 02
	tLine.clear();
	tPoints.clear();
	for (int i = 0; i < 3; i++) {
		tPoi = ofPoint(r*cos(-PI / 6 + PI + i*TWO_PI / 3), r*sin(-PI / 6 + PI + i*TWO_PI / 3), 0);
		tPoints.push_back(tPoi);
		tPointsA.push_back(tPoi);
	}
	tLine.addVertices(tPoints);
	tLine.close();
	shapes.push_back(tLine);
	// -> 03
	tLine.clear();
	tPoints.clear();
	for (int i = 0; i < 6; i++) {
		tPoi = ofPoint(r*cos(-PI / 6 + i*TWO_PI / 6), r*sin(-PI / 6 + i*TWO_PI / 6), 0);
		tPoints.push_back(tPoi);
		tPointsA.push_back(tPoi);
	}
	tLine.addVertices(tPoints);
	tLine.close();
	shapes.push_back(tLine);
	// -> 04
	r = 150;
	tLine.clear();
	tPoints.clear();
	for (int i = 0; i < 6; i++) {
		tPoi = ofPoint(r*cos(-PI / 6 + i*TWO_PI / 6), r*sin(-PI / 6 + i*TWO_PI / 6), 0);
		tPoints.push_back(tPoi);
		tPointsA.push_back(tPoi);
	}
	tLine.addVertices(tPoints);
	tLine.close();
	shapes.push_back(tLine);
	// -> 050
	r = 300;
	tLine.clear();
	tPoints.clear();
	for (int i = 0; i < 2; i++) {
		tPoi = ofPoint(r*cos(-PI / 6 + i*PI), r*sin(-PI / 6 + i*PI), 0);
		tPoints.push_back(tPoi);
		//tPointsA.push_back(tPoi);
	}
	tLine.addVertices(tPoints);
	tLine.close();
	shapes.push_back(tLine);
	tLine.clear();
	tPoints.clear();
	for (int i = 0; i < 2; i++) {
		tPoi = ofPoint(r*cos(PI / 6 + i*PI), r*sin(PI / 6 + i*PI), 0);
		tPoints.push_back(tPoi);
		//tPointsA.push_back(tPoi);
	}
	tLine.addVertices(tPoints);
	tLine.close();
	shapes.push_back(tLine);
	tLine.clear();
	tPoints.clear();
	for (int i = 0; i < 2; i++) {
		tPoi = ofPoint(r*cos(3 * PI / 6 + i*PI), r*sin(3 * PI / 6 + i*PI), 0);
		tPoints.push_back(tPoi);
		//tPointsA.push_back(tPoi);
	}
	tLine.addVertices(tPoints);
	tLine.close();
	shapes.push_back(tLine);
	// -> 06
	r = 150;
	tLine.clear();
	tPoints.clear();
	for (int i = 0; i < 3; i++) {
		tPoi = ofPoint(r*cos(-PI / 6 + i*TWO_PI / 3), r*sin(-PI / 6 + i*TWO_PI / 3), 0);
		tPoints.push_back(tPoi);
		//tPointsA.push_back(tPoi);
	}
	tLine.addVertices(tPoints);
	tLine.close();
	//tLine.end();
	shapes.push_back(tLine);
	// -=-   ==-=?
	ofSetLineWidth(4);
	ofSetColor(255, 200);
	ofNoFill();
	// ------ ------ ------ ------ ------ ------ | end form |
	cout << "A1: created ;;" << endl;
};

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
void Agent01::update(bool mov, float vel, bool wal, int wdir, bool multi, bool trail, float rms) {
	mo = mov; 	wa = wal;
	mu = multi; tr = trail;
	// ------ ------ ------ ------ ------ ------ | update state |
	for (int i = 0; i < shapes[0].size(); i++) {
		shapes[0][i].z = (mov == true) ? ofLerp(shapes[0][i].z, -5, 4 * vel) : 0;
		if (shapes[0][i].z < 0) { shapes[0][i].z = 500; }
	}
	for (int i = 0; i < shapes[1].size(); i++) {
		shapes[1][i].z = (mov == true) ? ofLerp(shapes[1][i].z, -5, 4 * vel) : 0;
		if (shapes[1][i].z < 0) { shapes[1][i].z = 500; }
	}
	for (int i = 0; i < shapes[2].size(); i++) {
		shapes[2][i].z = (mov == true) ? ofLerp(shapes[2][i].z, -5, vel) : 0;
		if (shapes[2][i].z < 0) { shapes[2][i].z = 500; }
	}
	for (int i = 0; i < shapes[3].size(); i++) {
		shapes[3][i].z = (mov == true) ? ofLerp(shapes[3][i].z, -5, 32 * vel) : 0;
		if (shapes[3][i].z < 0) { shapes[3][i].z = 500; }
	}
	for (int i = 0; i < shapes[7].size(); i++) {
		shapes[7][i].z = (mov == true) ? ofLerp(shapes[7][i].z, -5, 16 * vel) : 0;
		if (shapes[7][i].z < 0) { shapes[7][i].z = 500; }
	}

	// ------ ------ ------ ------ ------ ------ | draw on fbos |
	fbo_m1_front.begin();
	ofClear(0, 0);
	//ofEnableAlphaBlending();
	cam.begin();
	ofPushMatrix();
	ofRotate(180, 0, 0, 1);
	//triangulation.draw();
	//triangulation.triangleMesh.draw();
	//ofTranslate(w / 2, h / 2);
	ofFill();
	ofSetColor(0, 255);
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
	mesh.addVertices(shapes[2].getVertices());
	mesh.draw();
	ofNoFill();
	ofSetColor(255, 255);
	shapes[2].draw();
	for (int i = 0; i < shapes.size(); i++) {
		if ((i >= 3 && i <= 6)) {
			tShader.begin();
			tShader.setUniform1f("time", ofGetElapsedTimef());
			tShader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
			tShader.setUniform1f("param", 1);
			tShader.setUniform1f("mode", 3);
			//ofSetColor(255);
			ofFill();
			ofMesh mesh;
			mesh.setMode(OF_PRIMITIVE_LINE_LOOP);
			mesh.addVertices(shapes[i].getVertices());
			mesh.draw();
			ofNoFill();
			tShader.end();
		}
		else if (i == 7) {
			tShader.begin();
			tShader.setUniform1f("time", ofGetElapsedTimef());
			tShader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
			tShader.setUniform1f("param", 1);
			tShader.setUniform1f("mode", 3);			ofFill();
			ofSetColor(0, 0);
			ofMesh mesh;
			mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
			mesh.addVertices(shapes[i].getVertices());
			mesh.draw();
			tShader.end();
			ofNoFill();
			ofSetColor(255, 255);
			shapes[i].draw();
		}
		else {
			shapes[i].draw();
		}
	}
	ofPopMatrix();
	cam.end();
	//ofDisableAlphaBlending();
	fbo_m1_front.end();

	fbo_m1_back.begin();
	ofClear(0, 0);
	tShader.begin();
	tShader.setUniform1f("time", ofGetElapsedTimef());
	tShader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	tShader.setUniform1f("param", ofMap(rms, 0.1, 0.5, 50, 0.1));
	tShader.setUniform1f("mode", wdir);
	//ofSetColor(255);
	ofFill();
	ofDrawRectangle(0, 0, w, h);
	ofNoFill();
	tShader.end();
	fbo_m1_back.end();

	if (trail == true) {
		fbo_m1_delay.begin();
		cam.begin();
		ofPushMatrix();
		ofRotate(180, 0, 0, 1);
		tShader.begin();
		tShader.setUniform1f("time", ofGetElapsedTimef());
		tShader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
		tShader.setUniform1f("param", 1);
		tShader.setUniform1f("mode", 3);
		ofFill();
		ofMesh mesh;
		mesh.setMode(OF_PRIMITIVE_LINE_LOOP);
		mesh.addVertices(shapes[0].getVertices());
		mesh.draw();
		ofNoFill();
		tShader.end();
		ofPopMatrix();
		cam.end();
		fbo_m1_delay.end();
	}

	// ------ ------ ------ ------ ------ ------ | mix the fbos |
	fbo_a1.begin();
	ofClear(0, 0);
	//ofEnableAlphaBlending();
	if (wa == true) {
		ofSetColor(0, 0, 255, 255);
		fbo_m1_back.draw(0, 0);
	}
	if (tr == true) {
		fbo_m1_delay.draw(0, 0);
	}
	if (mu == true) {
		ofSetColor(0, 0, 255, 255);
		fbo_m1_front.draw(391, 0);
		fbo_m1_front.draw(-391, 0);
		fbo_m1_front.draw(195, 489);
		fbo_m1_front.draw(-195, 489);
		fbo_m1_front.draw(195, -489);
		fbo_m1_front.draw(-195, -489);
	}

	ofSetColor(0, 0, 255, 255);
	fbo_m1_front.draw(0, 0);
	//ofDisableAlphaBlending();
	fbo_a1.end();
};

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
void Agent01::draw() {
	fbo_a1.draw(0,0);
};



// .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. 2///////////
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -- 2///////////



Agent02::Agent02() {};
Agent02::~Agent02() {};

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
void Agent02::setup() {
	w = ofGetWidth();
	h = ofGetHeight();
	// ------ ------ ------ ------ ------ ------ | fbos |
	fbo_m2_front.allocate(w, h, GL_RGBA);
	fbo_m2_front.begin(); ofClear(0, 0); fbo_m2_front.end();
	fbo_m2_back.allocate(w, h, GL_RGBA);
	fbo_m2_back.begin(); ofClear(0, 0); fbo_m2_back.end();
	fbo_m2_trail.allocate(w, h, GL_RGBA);
	fbo_m2_trail.begin(); ofClear(0, 0); fbo_m2_trail.end();
	fbo_a2.allocate(w, h, GL_RGBA);
	fbo_a2.begin(); ofClear(0, 0); fbo_a2.end();
	// ------ ------ ------ ------ ------ ------ | shaders |
	//string shName = "shader/pSh_C6.frag";
	tShader.load("shader/basicVertex.vert", "shader/pSh_C6.frag");
	t2Sh.load("shader/basicVertex.vert", "other_shader/pSh_X7.frag");
	// ------ ------ ------ ------ ------ ------ | set points |
	r = 85;
	daPoints.resize(0);
	lay0.resize(0); 	lay1.resize(0);
	lay2.resize(0); 	lay3.resize(0);
	// -> 00
	tPoi = ofPoint(0, 0, 0);
	daPoints.push_back(tPoi);
	lay0.push_back(tPoi);
	// -> 01
	for (int i = 0; i < 6; i++) {
		tPoi = ofPoint(r*cos(-PI / 6 + i*TWO_PI / 6), r*sin(-PI / 6 + i*TWO_PI / 6), 0);
		daPoints.push_back(tPoi);
		lay1.push_back(tPoi);
		for (int j = -1; j <= 1 ; j++) {
			int k = i + j;
			if (k < 0) k += 6;
			if (k >= 6) k -= 6;
			tPoe = tPoi + ofPoint(r*cos(-PI / 6 + k*TWO_PI / 6), r*sin(-PI / 6 + k*TWO_PI / 6), 0);
			daPoints.push_back(tPoe);
			lay2.push_back(tPoe);
			for (int j = -1; j <= 1; j++) {
				int k = i + j;
				if (k < 0) k += 6;
				if (k >= 6) k -= 6;
				ofPoint tPou;
				tPou = tPoe + ofPoint(r*cos(-PI / 6 + k*TWO_PI / 6), r*sin(-PI / 6 + k*TWO_PI / 6), 0);
				daPoints.push_back(tPou);
				lay3.push_back(tPou);
			}

		}
	}
	cout << "A2: created w/" << daPoints.size() << "points;;" << endl;
	/*cout << "A2: points: " << endl;
	for (int i = 0; i < daPoints.size(); i++) {
		cout << "[" << i << "]: " << daPoints[i].x << "," << daPoints[i].y << endl;
	}*/
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
void Agent02::update(bool mov, float ran, bool wal, bool re, vector<float> mb) {
	wa = wal;
	t = ofGetFrameNum();
	// ------ ------ ------ ------ ------ ------ | update state |
	/*for (int i = 0; i < daPoints.size(); i++) {
		daPoints[i].z = (mov == true) ? ofLerp(daPoints[i].z, -5, 4 * ran) : 0;
		if (daPoints[i].z < 0) { daPoints[i].z = 500; }
	}*/

	// ------ ------ ------ ------ ------ ------ | draw on fbos |
	// front
	fbo_m2_front.begin();
	ofClear(0, 0);
	cam.begin();
	ofEnableAlphaBlending();
	ofSetCircleResolution(60);
	// -0
	//ofPushMatrix();
	//ofRotate(int(ofGetFrameNum() / 2) % 360, 1, 0, 0);
	ofFill();

	ofFill();

	ofSetLineWidth(2);
	if (mov==true) lay0[0].z = ofMap(mb[30], 0, 4, -10, ran);
	else lay0[0].z = 0;
	ofSetColor(0, (32 + 32 * cos(TWO_PI*t / 512)));
	ofCircle(lay0[0], 4.1*r);
	ofNoFill();
	ofSetColor(255, 255);
	ofCircle(lay0[0], 4.1*r);
	//ofPopMatrix();
	// .2
	ofSetLineWidth(2);
	for (int i = 0; i < lay2.size(); i++) {
		ofFill();
		ofSetColor(0, 128 + 128 * sin(TWO_PI*t / 2048));
		if (mov == true) lay2[i].z = ofMap(mb[15], 0, 4, -10, ran);
		else lay2[i].z = 0;
		ofCircle(lay2[i], r);
		ofNoFill();
		tShader.begin();
		tShader.setUniform1f("time", ofGetElapsedTimef());
		tShader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
		tShader.setUniform1f("param", 1);
		ofSetColor(255, 235);
		ofCircle(lay2[i], r);
		tShader.end();
	}
	// .3
	ofNoFill();
	ofSetLineWidth(2);
	for (int i = 0; i < lay3.size(); i++) {
		if (i % 3 == 1) {
			if (mov == true) lay3[i].z = ofMap(mb[60], 0, 3, -10, ran);
			else lay3[i].z = 0;
			tShader.begin();
			tShader.setUniform1f("time", ofGetElapsedTimef());
			tShader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
			tShader.setUniform1f("param", 1);
			//ofFill();
			ofCircle(lay3[i], r);
			tShader.end();

		}
		else {
			if (mov == true) lay3[i].z = ofMap(mb[20], 0, 4, -10, ran);
			else lay3[i].z = 0;
			//ofNoFill();
			ofSetColor(255, 235);
			ofCircle(lay3[i], r);
		}
	}
	// -1
	for (int i = 0; i < lay1.size(); i++) {
		ofFill();
		ofSetLineWidth(2);
		ofSetColor(0, 255);
		if (mov == true) lay1[i].z = ofMap(mb[45], 0, 3.5, -10, ran);
		else lay1[i].z = 0;
		ofCircle(lay1[i], r);
	}
	for (int i = 0; i < lay1.size(); i++) {
		ofNoFill();
		ofSetColor(255, 235);
		ofCircle(lay1[i], r);
	}
	ofDisableAlphaBlending();
	// -0
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofFill();
	ofSetColor(0, (32 + 32 * cos(TWO_PI*t / 512)));
	ofSetLineWidth(2);
	if (mov == true) lay0[0].z = ofMap(mb[70], 0, 3, -10, 300);
	else lay0[0].z = 0;
	ofCircle(lay0[0], r);
	ofNoFill(); //
	ofSetColor(255, 255);
	ofCircle(lay0[0], r);
	ofSetColor(255, 255); //
	ofSetLineWidth(1);
	ofCircle(lay0[0], 4.05*r);
	ofDisableBlendMode();
	cam.end(); //
	fbo_m2_front.end();
	// back
	fbo_m2_back.begin();
	ofClear(0, 0);
	t2Sh.begin();
	t2Sh.setUniform1f("time", ofGetElapsedTimef());
	if (re == true) t2Sh.setUniform1f("control", 0.1+ofMap(mb[50],0,5,0,3));
	else t2Sh.setUniform1f("control", 0.5);
	t2Sh.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	t2Sh.setUniform2f("mouse", ofGetWidth() / 2, ofGetHeight() / 2);
	//ofSetColor(255);
	ofFill();
	ofDrawRectangle(0, 0, w, h);
	ofNoFill();
	t2Sh.end();
	fbo_m2_back.end();

	// ------ ------ ------ ------ ------ ------ | mix the fbos |
	fbo_a2.begin();
	ofClear(0, 0);
	//ofEnableAlphaBlending();
	if (wa == true) {
		fbo_m2_back.draw(0, 0);
	}
	fbo_m2_front.draw(0, 0);
	//ofDisableAlphaBlending();
	fbo_a2.end();
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
void Agent02::draw() {
	fbo_a2.draw(0, 0);
}



// .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. 3///////////
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -- 3///////////


Agent03::Agent03() {};
Agent03::~Agent03() {};

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
void Agent03::setup() {
	w = ofGetWidth();
	h = ofGetHeight();
	// ------ ------ ------ ------ ------ ------ | fbos |
	fbo_m3_front.allocate(w, h, GL_RGBA);
	fbo_m3_front.begin(); ofClear(0, 0); fbo_m3_front.end();
	fbo_m3_back.allocate(w, h, GL_RGBA);
	fbo_m3_back.begin(); ofClear(0, 0); fbo_m3_back.end();
	fbo_m3_trail.allocate(w, h, GL_RGBA);
	fbo_m3_trail.begin(); ofClear(0, 0); fbo_m3_trail.end();
	fbo_a3.allocate(w, h, GL_RGBA);
	fbo_a3.begin(); ofClear(0, 0); fbo_a3.end();
	// ------ ------ ------ ------ ------ ------ | shaders |
	t1Sh.load("shader/basicVertex.vert", "shader/pSh_X0.frag");
	// ------ ------ ------ ------ ------ ------ | create forms |
	//shapes.resize(0);
	r = 300;
	cout << "A3: created w/" << "no init points;;" << endl;
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
void Agent03::update(bool mov, float max_ang, bool wal, vector<float> mb) {
	t = ofGetFrameNum();
	// ------ ------ ------ ------ ------ ------ | update state |
	
	// ------ ------ ------ ------ ------ ------ | draw fbos |
	fbo_m3_front.begin();
	ofClear(0,0);
	cam.begin();
	ofPushMatrix();
	ofRotate(90, 0, 0, 1);
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	//
	float phi = 0;
	float actPh = 0;
	for (int i = 0; i < 7; i++) {
		if (mov == true) past = t;
		else t = past;
		// base
		if (i == 0) {
			int ereeme = int(mb[10]);
			for (int k = 0; k < ereeme; k++) {
				float r2 = 350 + (k + 1) * 20;
				tPoints.clear();
				mesh.clear();
				actPh = (i + 1) * t * -PI / 5760;
				for (int j = 0; j < 3; j++) {
					tPoi = ofPoint(r2*cos(actPh + j*TWO_PI / 3), r2*sin(actPh + j*TWO_PI / 3), ofMap(mb[k * 9 % 103], 0, 3, -150, 200));
					tPoints.push_back(tPoi);
				}
				mesh.addVertices(tPoints);
				if (k % 2 == 0 && k>0) {
					ofNoFill();	ofSetColor(0, ofMap(mb[k * 7 % 104], 0, 2, 0, 55));
				}
				else {
					ofNoFill();	ofSetColor(255, ofMap(mb[k * 7 % 104], 0, 2, 0, 55));
				}
				mesh.draw();
			}
		}
		// then triamgles
		tPoints.clear();
		tLine.clear();
		mesh.clear();
		if (i % 2 == 1) {
			t1Sh.begin();
			t1Sh.setUniform1f("time", ofGetElapsedTimef()/(i+1));
			t1Sh.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
			t1Sh.setUniform2f("mouse", ofGetWidth()/2, ofGetHeight()/2);
		}
		// create each truangle
		if (i == 0) r = 350;
		else r = r * 2 / 3;
		actPh = (i+1) * t * -PI / 5760;
		for (int j = 0; j < 3; j++) {
			tPoi = ofPoint(r*cos(actPh + j*TWO_PI/3), r*sin(actPh + j*TWO_PI/3), 0);
			tPoints.push_back(tPoi);
		}
		mesh.addVertices(tPoints);
		tLine.addVertices(tPoints);
		tLine.close();
		// then draw it
		ofFill();	ofSetColor(0, 192);		ofSetLineWidth(2);
		mesh.draw();
		if (i % 2 == 1) {
			t1Sh.end();
		}
		ofNoFill();	ofSetColor(255, 255);
		tLine.draw();

	}
	//
	ofPopMatrix();
	cam.end();
	fbo_m3_front.end();
	// back
	fbo_m3_back.begin();
	ofClear(0, 0);
	t1Sh.begin();
	t1Sh.setUniform1f("time", ofGetElapsedTimef());
	//t1Sh.setUniform2f("surfacePosition", ofGetWidth()/2, ofGetHeight()/2);
	t1Sh.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	//t1Sh.setUniform2f("mouse", ofGetMouseX(), ofGetMouseY());

	//ofSetColor(255);
	ofFill();
	ofDrawRectangle(0, 0, w, h);
	t1Sh.end();
	fbo_m3_back.end();

	// ------ ------ ------ ------ ------ ------ | mix the fbos |
	fbo_a3.begin();
	ofClear(0, 0);
	//ofEnableAlphaBlending();
	if (wal == true) {
		fbo_m3_back.draw(0, 0);
	}
	fbo_m3_front.draw(0, 0);
	//ofDisableAlphaBlending();
	fbo_a3.end();
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
void Agent03::draw() {
	fbo_a3.draw(0, 0);
}


// .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. 4///////////
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -- 4///////////


Agent04::Agent04() {};
Agent04::~Agent04() {};

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
void Agent04::setup() {
	w = ofGetWidth();
	h = ofGetHeight();
	// ------ ------ ------ ------ ------ ------ | fbos |
	fbo_m4_front.allocate(w, h, GL_RGBA);
	fbo_m4_front.begin(); ofClear(0, 0); fbo_m4_front.end();
	fbo_m4_back.allocate(w, h, GL_RGBA);
	fbo_m4_back.begin(); ofClear(0, 0); fbo_m4_back.end();
	fbo_m4_trail.allocate(w, h, GL_RGBA);
	fbo_m4_trail.begin(); ofClear(0, 0); fbo_m4_trail.end();
	fbo_a4.allocate(w, h, GL_RGBA);
	fbo_a4.begin(); ofClear(0, 0); fbo_a4.end();
	pixs.allocate(w, h, GL_RGBA);
	img.allocate(w, h, OF_IMAGE_COLOR_ALPHA);
	// ------ ------ ------ ------ ------ ------ | shaders |
	t41Sh.load("shader/basicVertex.vert", "shader/pSh_C6.frag");
	ofLoadImage(pixs, "cImgs/img_s06.jpg");
	pColor = ofPoint(0, 0, 0);
	// ------ ------ ------ ------ ------ ------ | create forms |
	forms.resize(0);
	ofNoFill();
	ofSetLineWidth(3);
	ofSetColor(0, 255, 0);
	r = 280;
	// -> 01 cen dw
	tLine.clear();
	tPoints.clear();
	for (int i = 0; i < 3; i++) {
		tPoi = ofPoint(r*cos(-PI / 6 + i*TWO_PI / 3), r*sin(-PI / 6 + i*TWO_PI / 3), 0);
		tPoints.push_back(tPoi);
	}
	tLine.addVertices(tPoints);
	tLine.close();
	tForm.setup(tPoints);
	forms.push_back(tForm);
	// -> 02 cen up
	tLine.clear();
	tPoints.clear();
	for (int i = 0; i < 3; i++) {
		tPoi = ofPoint(r*cos(-PI / 6 + PI + i*TWO_PI / 3), r*sin(-PI / 6 + PI + i*TWO_PI / 3), 0);
		tPoints.push_back(tPoi);
	}
	tLine.addVertices(tPoints);
	tLine.close();
	tForm.setup(tPoints);
	forms.push_back(tForm);
	// -> 03 tri dw
	r = 360;
	tLine.clear();
	tPoints.clear();
	for (int i = 0; i < 3; i++) {
		if (i == 1) {
			tPoi = ofPoint(234*cos(-PI / 6 + i*TWO_PI / 3), 100 + 234*sin(-PI / 6 + i*TWO_PI / 3), 0);
		}
		else {
			tPoi = ofPoint(r*cos(-PI / 6 + i*TWO_PI / 3), 100 + r*sin(-PI / 6 + i*TWO_PI / 3), 0);
		}
		tPoints.push_back(tPoi);
	}
	tLine.addVertices(tPoints);
	tLine.close();
	tForm.setup(tPoints);
	forms.push_back(tForm);
	// -> 04 tri up
	tLine.clear();
	tPoints.clear();
	for (int i = 0; i < 3; i++) {
		if (i == 1) {
			tPoi = ofPoint(234*cos(-PI / 6 + PI + i*TWO_PI / 3), -100 + 234*sin(-PI / 6 + PI + i*TWO_PI / 3), 0);
		}
		else {
			tPoi = ofPoint(r*cos(-PI / 6 + PI + i*TWO_PI / 3), -100 + r*sin(-PI / 6 + PI + i*TWO_PI / 3), 0);
		}
		tPoints.push_back(tPoi);
	}
	tLine.addVertices(tPoints);
	tLine.close();
	tForm.setup(tPoints);
	forms.push_back(tForm);
	// -> 05 iso dw
	r = 240;
	tLine.clear();
	tPoints.clear();
	for (int i = 0; i < 3; i++) {
		if (i == 1) {
			tPoi = ofPoint(253 * cos(-PI / 6 + i*TWO_PI / 3), -135 + 180 * sin(-PI / 6 + i*TWO_PI / 3), 0);
		}
		else {
			tPoi = ofPoint(r*cos(-PI / 6 + i*TWO_PI / 3), -135 + r*sin(-PI / 6 + i*TWO_PI / 3), 0);
		}
		tPoints.push_back(tPoi);
	}
	tLine.addVertices(tPoints);
	tLine.close();
	tForm.setup(tPoints);
	forms.push_back(tForm);
	// -> 06 iso up
	tLine.clear();
	tPoints.clear();
	for (int i = 0; i < 3; i++) {
		if (i == 1) {
			tPoi = ofPoint(253 * cos(-PI / 6 + PI + i*TWO_PI / 3), 135 + 180 * sin(-PI / 6 + PI + i*TWO_PI / 3), 0);
		}
		else {
			tPoi = ofPoint(r*cos(-PI / 6 + PI + i*TWO_PI / 3), 135 + r*sin(-PI / 6 + PI + i*TWO_PI / 3), 0);
		}
		tPoints.push_back(tPoi);
	}
	tLine.addVertices(tPoints);
	tLine.close();
	tForm.setup(tPoints);
	forms.push_back(tForm);

	// -> 07 mini
	r = 40;
	tLine.clear();
	tPoints.clear();
	for (int i = 0; i < 3; i++) {
		if (i == 1) {
			tPoi = ofPoint(r * cos(-PI / 6 + i*TWO_PI / 3), 40 + r* sin(-PI / 6 + i*TWO_PI / 3), 0);
		}
		else {
			tPoi = ofPoint(100*cos(-PI / 6 + i*TWO_PI / 3), 40 + 80*sin(-PI / 6 + i*TWO_PI / 3), 0);
		}
		tPoints.push_back(tPoi);
	}
	tLine.addVertices(tPoints);
	tLine.close();
	tForm.setup(tPoints);
	forms.push_back(tForm);

	/*
	// -> 08 int1 dw
	r = 200;
	tLine.clear();
	tPoints.clear();
	for (int i = 0; i < 3; i++) {
		if (i == 1) {
			tPoi = ofPoint(265 * cos(-PI / 6 + i*TWO_PI / 3), -186 + 265 * sin(-PI / 6 + i*TWO_PI / 3), 0);
		}
		else {
			tPoi = ofPoint(r*cos(-PI / 6 + i*TWO_PI / 3), -186 + r*sin(-PI / 6 + i*TWO_PI / 3), 0);
		}
		tPoints.push_back(tPoi);
	}
	tLine.addVertices(tPoints);
	tLine.close();
	tForm.setup(tPoints);
	forms.push_back(tForm);
	// -> 09 int1 up
	tLine.clear();
	tPoints.clear();
	for (int i = 0; i < 3; i++) {
		if (i == 1) {
			tPoi = ofPoint(265 * cos(-PI / 6 + PI + i*TWO_PI / 3), 186 + 265 * sin(-PI / 6 + PI + i*TWO_PI / 3), 0);
		}
		else {
			tPoi = ofPoint(r*cos(-PI / 6 + PI + i*TWO_PI / 3), 186 + r*sin(-PI / 6 + PI + i*TWO_PI / 3), 0);
		}
		tPoints.push_back(tPoi);
	}
	tLine.addVertices(tPoints);
	tLine.close();
	tForm.setup(tPoints);
	forms.push_back(tForm);*/
	//
	cout << "A4: created w/" << forms.size() << "forms;;" << endl;
	ofSetCircleResolution(60);
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
void Agent04::update(bool mov, float ve, bool wal, vector<float> mb) {
	t = ofGetFrameNum();
	// ------ ------ ------ ------ ------ ------ | update state |
	for (int i = 0; i < forms.size(); i++) {
		if (mov == true) {
			forms[i].update(1, pow(2, i) *ve);
		}
		else {
			forms[i].update(0);
		}
		if (forms[i].isFresh) {
			//pos = int(ofRandom(pix.size()));
			pColor.x += 2 * ofMap(ofNoise(tt), 0, 0.5, -2, 2);
			pColor.y += 2 * ofMap(ofNoise(t / 3), 0, 0.5, -2, 2);
			if (pColor.x > 1000) pColor.x -= 1000;
			if (pColor.x < 0) pColor.x += 1000;
			if (pColor.y > 1200) pColor.y -= 1200;
			if (pColor.y < 0) pColor.y += 1200;
			int ipp = (3 * (pColor.y * 1023 + pColor.x));
			iColor = pixs.getColor(pColor.x, pColor.y);

			forms[i].c = iColor;
			forms[i].isFresh = false;
		}

	}
	// ------ ------ ------ ------ ------ ------ | draw fbos |
	fbo_m4_front.begin();
	ofClear(0, 0);
	cam.begin();
	ofPushMatrix();
	ofRotate(180, 0, 0, 1);
	
	t41Sh.begin();
	t41Sh.setUniform1f("time", ofGetElapsedTimef());
	t41Sh.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	r = 340;
	ofNoFill();
	ofSetLineWidth(1);
	ofSetColor(255, 255);
	ofCircle(0, 0, 0, r);
	t41Sh.end();
	
	r = 330;
	ofNoFill();
	ofSetLineWidth(3);
	ofSetColor(255, 200);
	ofCircle(0,0,0, r);
	
	
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
	/*mesh.addVertices(forms[0].shape.getVertices());
	mesh.draw();
	ofNoFill();
	ofSetColor(255, 255);
	forms[0].draw();
	*/
	for (int i = 0; i < forms.size(); i++) {
	//for (int i = forms.size() - 1; i >= 0; i--) {
		//if (i == 0 || i == 1) {	forms[i].c = ofColor();	}
		forms[i].draw_echo(int(mb[(13*i)%103] * 100), 10);
	}
	ofPopMatrix();
	cam.end();
	fbo_m4_front.end();
	
	
	// back
	fbo_m4_back.begin();
	ofClear(0, 0);
	t41Sh.begin();
	t41Sh.setUniform1f("time", ofGetElapsedTimef());
	//t1Sh.setUniform2f("surfacePosition", ofGetWidth()/2, ofGetHeight()/2);
	t41Sh.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	t41Sh.setUniform2f("mouse", ofGetMouseX(), ofGetMouseY());
	//ofSetColor(255);
	ofFill();
	ofDrawRectangle(0, 0, w, h);
	t41Sh.end();
	fbo_m4_back.end();

	// ------ ------ ------ ------ ------ ------ | mix the fbos |
	fbo_a4.begin();
	ofClear(0, 0);
	//ofEnableAlphaBlending();
	if (wal == true) {
		fbo_m4_back.draw(0, 0);
	}
	fbo_m4_front.draw(0, 0);
	//ofDisableAlphaBlending();
	fbo_a4.end();
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
void Agent04::draw() {
	fbo_a4.draw(0, 0);
}



// .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. 5///////////
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -- 5///////////


Agent05::Agent05() {};
Agent05::~Agent05() {};

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
void Agent05::setup() {
	w = ofGetWidth();
	h = ofGetHeight();
	// ------ ------ ------ ------ ------ ------ | fbos |
	fbo_m5_front.allocate(w, h, GL_RGBA);
	fbo_m5_front.begin(); ofClear(0, 0); fbo_m5_front.end();
	fbo_m5_back.allocate(w, h, GL_RGBA);
	fbo_m5_back.begin(); ofClear(0, 0); fbo_m5_back.end();
	fbo_m5_trail.allocate(w, h, GL_RGBA);
	fbo_m5_trail.begin(); ofClear(0, 0); fbo_m5_trail.end();
	fbo_a5.allocate(w, h, GL_RGBA);
	fbo_a5.begin(); ofClear(0, 0); fbo_a5.end();
	pixs.allocate(w, h, GL_RGBA);
	img.allocate(w, h, OF_IMAGE_COLOR_ALPHA);
	// ------ ------ ------ ------ ------ ------ | shaders |
	t51Sh.load("shader/basicVertex.vert", "shader/pSh_C6.frag");
	colorPick.setup("cImgs/img_s08.jpg");
	// ------ ------ ------ ------ ------ ------ | create forms |
	forms.resize(0);
	ofNoFill();
	ofSetLineWidth(3);
	ofSetColor(0, 255, 0);
	r = 350;
	// -> 01 cen dw
	tPoints.clear();
	for (int i = 0; i < 3; i++) {
		tPoi = ofPoint(r*cos(-PI / 6 + i*TWO_PI / 3), r*sin(-PI / 6 + i*TWO_PI / 3), 0);
		tPoints.push_back(tPoi);
	}
	tForm.setup(tPoints);
	forms.push_back(tForm);
	// -> 02 cen up
	tPoints.clear();
	for (int i = 0; i < 3; i++) {
		tPoi = ofPoint(r*cos(-PI / 6 + PI + i*TWO_PI / 3), r*sin(-PI / 6 + PI + i*TWO_PI / 3), 0);
		tPoints.push_back(tPoi);
	}
	tForm.setup(tPoints);
	forms.push_back(tForm);
	// -03 points
	r = 220;
	pos.resize(0);
	for (int i = 0; i < 6; i++) {
		tPoi = ofPoint(r*cos(-PI/6 + i*TWO_PI / 6), r*sin(-PI / 6 + i*TWO_PI / 6), 0);
		pos.push_back(tPoi);
	}

	//
	cout << "A5: created w/" << forms.size() << "forms;;" << "&/ " << pos.size() << "points;;" << endl;
	ofSetCircleResolution(60);
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
void Agent05::update(bool mov, float ve, bool grow, bool wal, vector<float> mb) {
	t = ofGetFrameNum();
	tt = ofGetElapsedTimef();
	// ------ ------ ------ ------ ------ ------ | update state |
	for (int i = 0; i < forms.size(); i++) {
		if (mov == true) {
			forms[i].update(1, pow(2, i) *ve);
		}
		else {
			forms[i].update(0);
		}
		if (forms[i].isFresh) {
			forms[i].c = colorPick.pick(); // colorPick!!
			forms[i].isFresh = false;
		}

	}
	// ------ ------ ------ ------ ------ ------ | draw fbos |
	fbo_m5_front.begin();
	ofClear(0, 0);
	cam.begin();
	ofPushMatrix();
	ofRotate(180, 0, 0, 1);
	// bottom circles
	t51Sh.begin();
	t51Sh.setUniform1f("time", ofGetElapsedTimef());
	t51Sh.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	r = 340;
	ofNoFill();
	ofSetLineWidth(1);
	//ofSetColor(255, 255);
	//
	ofCircle(0, 0, 0, r);
	t51Sh.end();
	r = 330;
	ofNoFill();
	ofSetLineWidth(3);
	ofSetColor(255, 200);
	echoCircleUp(ofPoint(0, 0, 0), r, int(mb[60] * 50), 5, colorPick.pick());
	ofCircle(0, 0, 0, r);
	// center triangles
	ofPushMatrix();
	ofRotate(tt*5,0,0,1);
	for (int i = 0; i < forms.size(); i++) {
		forms[i].draw_echo(int(mb[(13 * i) % 103] * 100), 10);
	}
	ofPopMatrix();
	// more circles
	r = 100;
	ofSetLineWidth(3);
	ofSetColor(255, 255);
	ofPushMatrix();
	ofRotate(-tt*10, 0, 0, 1);
	for (int j = 0; j < pos.size(); j++) {
		//ofCircle(pos[j], r);
		echoCircleDw(pos[j], grow == true ? r*(0.1 + mb[75]) : r, int(mb[8] * 50), 10, colorPick.pick());
	}
	ofPopMatrix();
	ofPopMatrix();
	cam.end();
	fbo_m5_front.end();


	// back
	fbo_m5_back.begin();
	ofClear(0, 0);
	t51Sh.begin();
	t51Sh.setUniform1f("time", ofGetElapsedTimef());
	t51Sh.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	//ofSetColor(255);
	ofFill();
	ofDrawRectangle(0, 0, w, h);
	t51Sh.end();
	fbo_m5_back.end();

	// ------ ------ ------ ------ ------ ------ | mix the fbos |
	fbo_a5.begin();
	ofClear(0, 0);
	//ofEnableAlphaBlending();
	if (wal == true) {
		fbo_m5_back.draw(0, 0);
	}
	fbo_m5_front.draw(0, 0);
	//ofDisableAlphaBlending();
	fbo_a5.end();
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
void Agent05::draw() {
	fbo_a5.draw(0, 0);
}



// .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. 6///////////
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -- 6///////////


Agent06::Agent06() {};
Agent06::~Agent06() {};

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
void Agent06::setup() {
	w = ofGetWidth();
	h = ofGetHeight();
	// ------ ------ ------ ------ ------ ------ | fbos |
	fbo_m6_front.allocate(w, h, GL_RGBA);
	fbo_m6_front.begin(); ofClear(0, 0); fbo_m6_front.end();
	fbo_m6_back.allocate(w, h, GL_RGBA);
	fbo_m6_back.begin(); ofClear(0, 0); fbo_m6_back.end();
	fbo_m6_trail.allocate(w, h, GL_RGBA);
	fbo_m6_trail.begin(); ofClear(0, 0); fbo_m6_trail.end();
	fbo_a6.allocate(w, h, GL_RGBA);
	fbo_a6.begin(); ofClear(0, 0); fbo_a6.end();
	pixs.allocate(w, h, GL_RGBA);
	img.allocate(w, h, OF_IMAGE_COLOR_ALPHA);
	// ------ ------ ------ ------ ------ ------ | shaders |
	t61Sh.load("shader/basicVertex.vert", "shader/pSh_C6.frag");
	colorPick.setup("cImgs/img_s09.jpg");
	// ------ ------ ------ ------ ------ ------ | create forms |
	waves.resize(0);
	lineWave tWave;
	tWave.setup(ofPoint(0, 0, 0), ofColor(255, 0, 0));
	tWave.vel.y = 2;
	waves.push_back(tWave);
	//
	cout << "A6: created w/" << "forms;;" << endl;
	ofSetCircleResolution(60);
	t0 = ofGetElapsedTimeMillis();
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
void Agent06::update(bool fal, float per, float ext, vector<float> mb) {
	t = ofGetElapsedTimeMillis();
	tt = ofGetElapsedTimef();
	// ------ ------ ------ ------ ------ ------ | update state |
	if (t - t0 > per && fal== true) {
		// set color
		//cout << "na-";
		iColor = colorPick.pick();
		// set agent
		lineWave tWave;
		tWave.setup(ofPoint(0, 0, 200), iColor);
		tWave.popWith(mb, ext);
		tWave.vel.y = 1;
		//tWave.color = ofColor();
		waves.push_back(tWave);
		//cout << waves.size();
		t0 = t;
	}

	for (int i = 0; i < waves.size(); i++) {
		if (waves[i].isDead == true) {
			waves.erase(waves.begin() + i);
		}
		else if (fal == true) {
			waves[i].fall();
			waves[i].redoTrail();
		}

	}
	// ------ ------ ------ ------ ------ ------ | draw fbos |
	fbo_m6_front.begin();
	ofClear(0, 0);

	ofEnableBlendMode(OF_BLENDMODE_SCREEN);
	//ofEnableAlphaBlending();
	for (int i = 0; i < waves.size(); i++) {
		if (mb[50]>1.2) 	ofSetLineWidth(4);
		else if (mb[60]>0.8) 	ofSetLineWidth(3);
		else if (mb[70]>0.1) 	ofSetLineWidth(1);
		else if (mb[80]<0.1) 	ofSetLineWidth(0.0);
		else 	ofSetLineWidth(1);

		waves[i].draw();
	}

	//ofDisableAlphaBlending();
	ofDisableBlendMode();
	fbo_m6_front.end();


	// back
	fbo_m6_back.begin();
	ofClear(0, 0);
	t61Sh.begin();
	t61Sh.setUniform1f("time", ofGetElapsedTimef());
	t61Sh.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	//ofSetColor(255);
	ofFill();
	ofDrawRectangle(0, 0, w, h);
	t61Sh.end();
	fbo_m6_back.end();

	// ------ ------ ------ ------ ------ ------ | mix the fbos |
	fbo_a6.begin();
	ofClear(0, 0);
	//ofEnableAlphaBlending();
	fbo_m6_front.draw(0, 0);
	//ofDisableAlphaBlending();
	fbo_a6.end();
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
void Agent06::draw() {
	fbo_a6.draw(0, 0);
}


// .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. -- .. 7///////////
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -- 7///////////


Agent07::Agent07() {};
Agent07::~Agent07() {};

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
void Agent07::setup() {
	w = ofGetWidth();
	h = ofGetHeight();
	// ------ ------ ------ ------ ------ ------ | fbos |
	fbo_m7_front.allocate(w, h, GL_RGBA);
	fbo_m7_front.begin(); ofClear(0, 0); fbo_m7_front.end();
	fbo_m7_back.allocate(w, h, GL_RGBA);
	fbo_m7_back.begin(); ofClear(0, 0); fbo_m7_back.end();
	fbo_m7_trail.allocate(w, h, GL_RGBA);
	fbo_m7_trail.begin(); ofClear(0, 0); fbo_m7_trail.end();
	fbo_a7.allocate(w, h, GL_RGBA);
	fbo_a7.begin(); ofClear(0, 0); fbo_a7.end();
	pixs.allocate(w, h, GL_RGBA);
	img.allocate(w, h, OF_IMAGE_COLOR_ALPHA);
	// ------ ------ ------ ------ ------ ------ | shaders |
	t71Sh.load("shader/basicVertex.vert", "shader/pSh_C6.frag");	//
	colorPick.setup("cImgs/img_s06.jpg");
	// ------ ------ ------ ------ ------ ------ | create forms |
	p_a = ofPoint(0, 0, 0);
	ofSetLineWidth(4);
	theBlocks.resize(0);
	for (int i = 0; i < 36 - 1; i++) {
		Block tempBlock;
		if (i > 11 && i < 23) {
			int nt = int(ofRandom(6, 16));
			tempBlock.setup(ofPoint(ofRandom(w), 10 + i * 20, 0), nt);
			tempBlock.v = ofRandom(PI / 8, PI / 6);
		}
		else {
			int nt = int(ofRandom(3, 11));
			tempBlock.setup(ofPoint(ofRandom(w), 10 + i * 20, 0), nt);
		}
		theBlocks.push_back(tempBlock);
	}


	//
	cout << "A7: created w/" << theBlocks.size() << "blocks;;" << endl;
	ofSetCircleResolution(60);										//
	t0 = ofGetElapsedTimeMillis();
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
void Agent07::update(bool fal, float per, float ext, vector<float> mb, vector<float> d0) {
	t = ofGetElapsedTimeMillis();
	tt = ofGetElapsedTimef();
	// ------ ------ ------ ------ ------ ------ | update state |

	// ------ ------ ------ ------ ------ ------ | draw fbos |
	fbo_m7_front.begin();
	ofClear(0, 0);

	ofEnableBlendMode(OF_BLENDMODE_ADD);
	for (int i = 0; i < theBlocks.size(); i++) {
		theBlocks[i].go_to();
		
		theBlocks[i].color = colorPick.pick();
		//ofSetColor(iColor);
		theBlocks[i].cwd = (mb[12 + (4 * i) % 80] / 3) * w / theBlocks[i].nt;
		if (i % 4 == 0 && t % 17 < 7) {
			theBlocks[i].cH = int(mb[24 + i % 24]) * 20;
			//cout << "ok:" << int(mb[24 + i % 24]) * 20;
		}
		else {
			theBlocks[i].cH = 20;
		}
		//if (mB[22]>5) {
		if (t % 7>5 || t % 25  <2 || d0[22]>5 || d0[60]>4 || d0[86]>3) {
			theBlocks[i].draw();
		}
	}
	//ofDisableAlphaBlending();
	ofDisableBlendMode();
	fbo_m7_front.end();
	
	// back
	fbo_m7_back.begin();
	ofClear(0, 0);
	t71Sh.begin();
	t71Sh.setUniform1f("time", ofGetElapsedTimef());
	t71Sh.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	//ofSetColor(255);
	ofFill();
	ofDrawRectangle(0, 0, w, h);
	t71Sh.end();
	fbo_m7_back.end();

	// ------ ------ ------ ------ ------ ------ | mix the fbos |
	fbo_a7.begin();
	ofClear(0, 0);
	//ofEnableAlphaBlending();
	fbo_m7_front.draw(0, 0);
	//ofDisableAlphaBlending();
	fbo_a7.end();
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
void Agent07::draw() {
	fbo_a7.draw(0, 0);
}