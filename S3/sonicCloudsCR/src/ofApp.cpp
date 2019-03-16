#include "ofApp.h"

using namespace cv;
using namespace ofxCv;

//--------------------------------------------------------------
void ofApp::setup(){
	//- base
	ofBackground(0);
	ofSetVerticalSync(true);
	ofSetFrameRate(12);
	h = ofGetHeight();
	w = ofGetWidth();

	//- osc
	osc_sender.setup(OSC_HOST, OSC_OUT_PORT);

	//- no gui
	gridSz = 40;
	cout << "OK1: " << gridSz << endl;

	//- files
	load_dir();
	load_new = false;

	//- rec 
	img_recorder.setPrefix(ofToDataPath("imgs/cc_sphere_"));
	img_recorder.setFormat("png");
	en_record = false;

	// other inits
	actImg = toCv(imgList[0]);
	cvtColor(actImg, grayImg, CV_RGB2GRAY);
	prevImg = grayImg;

	fbo_A.allocate(w, h, GL_RGBA);
	fbo_A.begin(); ofClear(0, 0); fbo_A.end();
	fbo_B.allocate(w, h, GL_RGBA);
	fbo_B.begin(); ofClear(0, 0); fbo_B.end();
	fbo_C.allocate(w, h, GL_RGBA);
	fbo_C.begin(); ofClear(0, 0); fbo_C.end();
	fbo_D.allocate(w, h, GL_RGBA);
	fbo_D.begin(); ofClear(0, 0); fbo_D.end();

	scale = 0;
	isLoad = false;
	blackFlag = false;
	showGui = true;
	index_img = 0;
	index1 = 1;

	en_edges	= true;
	edge_thr1	= 26;
	edge_thr2	= 36;

	lkQ			= 0.01;
	lkMxLv		= 2;
	lkMnDist	= 5;

	cutCM		= 7.0;
	vlThr		= 15;

	// mesh
	meshSize = 10;
	mw = iw / meshSize;
	mh = ih / meshSize;

	for (int y = 0; y < mh; y++) {
		for (int x = 0; x < mw; x++) {
			//mesh.addVertex(ofPoint(x, y, 0));
			mesh.addVertex(ofPoint((x - mw / 2) * meshSize, (y - mh / 2) * meshSize, 0));
			mesh.addTexCoord(ofPoint(x * (iw / mw), y * (ih / mh)));
			mesh.addColor(ofColor(255, 255, 255));
		}
	}

	for (int y = 0; y < mh - 1; y++) {
		for (int x = 0; x < mw - 1; x++) {
			mesh.addIndex(x + y*mw);				// 0
			mesh.addIndex((x + 1) + y*mw);			// 1
			mesh.addIndex(x + (y + 1)*mw);			// 10

			mesh.addIndex((x + 1) + y*mw);			// 1
			mesh.addIndex((x + 1) + (y + 1)*mw);	// 11
			mesh.addIndex(x + (y + 1)*mw);			// 10

		}
	}
	//light.setAmbientColor(ofColor(255));
	//light.enable();
	extrusionAmount = 1500;
	en_3d = true;
	en_dots = false;
	en_faces = true;
	en_wire = false;
	en_bind = false;
}




//--------------------------------------------------------------
void ofApp::update(){
	//- timings
	t = ofGetElapsedTimeMillis();
	tt = ofGetElapsedTimef();
	
	//- title and data
	std::stringstream strm;
	strm << "CE_exTraktor // fps: " << ofGetFrameRate();
	ofSetWindowTitle(strm.str());

	// check state
	if (load_new == true) {
		load_dir();
		load_new = false;
		}

	//- no sequence, just update index and flag
	t0 = ofGetElapsedTimeMillis();
	if ((t0 - t1) > 80) {
		if (index_img < nImgs - 3) {
			index_img++;
			blackFlag = true;
		}
		else {
			index_img = 0;
			if (en_write == true) {
				// escribe a archivo
				if (!db.save("db.json", true)) {
					ofLogNotice("ofApp::setup") << "db.json written unsuccessfully.";
				} else {
					ofLogNotice("ofApp::setup") << "db.json written successfully.";
				}
				// y desactiva write mode
				en_write = false;
			}
			if (en_record== true) {
				// escribe a archivo
				ofLogNotice("ofApp::setup") << "image sequence written successfully.";
				en_record = false;
			}
		}
		t1 = ofGetElapsedTimeMillis();
	}
	
	//- if flagged, process
	if (blackFlag==true)
	{
		if (en_3d==false) 
		{
			// get image, get bw
			actImg = toCv(imgList[index_img]);
			cvtColor(actImg, grayImg, CV_RGB2GRAY);
			nexImg = toCv(imgList[index_img + 1]);
			cvtColor(nexImg, grayImg2, CV_RGB2GRAY);
			// if edges
			if (en_edges == true) {
				Canny(grayImg, edge, edge_thr1, edge_thr2);
				edge.update();
				Canny(grayImg2, edge2, edge_thr1, edge_thr2);
				edge2.update();
			}
			// then optoflow
			LK.setWindowSize(10);
			LK.setQualityLevel(lkQ);
			LK.setMinDistance(lkMnDist);
			LK.setMaxLevel(lkMxLv);
			LK.setFeaturesToTrack(feats);
			if (en_edges == true) LK.calcOpticalFlow(edge);
			else LK.calcOpticalFlow(imgList[index_img]);
			new_feats = LK.getMotion();
			// and draw on FBO
			fbo_B.begin();
			ofClear(0, 0);
			// do lines -use LK.draw(0, 0) alternatively-
			for (int i = 0; i < new_feats.size(); i++) {
				ofNoFill();
				if (new_feats[i].length() <= vlThr) {
					scale = ofMap(new_feats[i].length(), 0, 20, 0, 255);
					ofSetColor(162, 255, 30, 150);
					ofSetLineWidth(2);
					acc_states[i] = ofLerp(acc_states[i], new_feats[i].length(), 0.5);
					ofDrawCircle(ofVec2f(feats[i].x - 1, feats[i].y - 1), acc_states[i]);
					//ofDrawCircle(ofVec2f(feats[i].x - 1, feats[i].y - 1), new_feats[i].length());
					ofSetColor(255, 165); // originale scale line
					ofSetLineWidth(4);
					ofLine(ofVec2f(feats[i].x, feats[i].y), ofVec2f(feats[i].x, feats[i].y) + new_feats[i] * 2);
				}
				else {
					scale = new_feats[i].length() / vlThr;
					ofSetColor(40, 90, 255, 152);
					ofSetLineWidth(2);
					acc_states[i] = ofLerp(acc_states[i], new_feats[i].length() / scale, 0.5);
					ofDrawCircle(ofVec2f(feats[i].x - 1, feats[i].y - 1), acc_states[i]);
					ofSetColor(255, 165);
					ofSetLineWidth(4);
					ofLine(ofVec2f(feats[i].x, feats[i].y), ofVec2f(feats[i].x, feats[i].y) + 2 * new_feats[i] / scale);
				}
				ofFill();
				ofSetLineWidth(1);
				ofSetColor(10, 255, 10, 107);// (1 + sin((ofGetFrameNum() % 36)*TWO_PI / 9)) * 128);
				ofDrawRectangle(ofVec2f(feats[i].x - 2, feats[i].y - 2), 4, 4);
				;

			}
			fbo_B.end();

			// draw the other fBO
			fbo_A.begin();
			ofClear(0, 0);
			if (en_edges == true) { edge.draw(0, 0); }
			else { imgList[index_img].draw(0, 0); }
			fbo_A.end();

			// if writing data
			if (en_write == true) {
				entry.clear();
				entry["filename"] = fList[index_img];
				entry["edges"] = ofToString(en_edges == true ? " on" : "off");
				entry["edgThrA"] = edge_thr1;
				entry["edgThrB"] = edge_thr2;
				entry["lkQ"] = lkQ;
				entry["vlThr"] = vlThr;
				entry["index"] = index_img;
				ofxJSONElement klist;
				klist.append("x");
				klist.append("y");
				klist.append("dx");
				klist.append("dy");
				klist.append("angle");
				klist.append("lenght");
				entry["optoKeys"] = klist;
				delist.clear();
				for (int i = 0; i < new_feats.size(); i++) {
					mini.clear();
					/*mini["x"] = feats[i].x;
					mini["y"] = feats[i].y;
					mini["dx"] = new_feats[i].x;
					mini["dy"] = new_feats[i].y;
					//ofVec2f dapos;
					//dapos = ofVec2f(new_feats[i]) + ofVec2f(float(feats[i].x), float(feats[i].y));
					mini["angle"] = new_feats[i].angle(ofVec2f(0, 1));
					mini["distance"] = new_feats[i].distance(ofVec2f(0, 0));
					mini["lenght"] = new_feats[i].length();*/
					mini.append(feats[i].x);
					mini.append(feats[i].y);
					mini.append(new_feats[i].x);
					mini.append(new_feats[i].y);
					mini.append(new_feats[i].angle(ofVec2f(0, 1)));
					mini.append(new_feats[i].length());
					delist.append(mini);
				}
				entry["opticflow"] = delist;
				db.append(entry);
			}
		}
		else										// -here comes 3D 
		{
			//actImg = toCv(imgList[index_img]);
			//cvtColor(actImg, grayImg, CV_RGB2GRAY);
			//cv::Mat nuImg;
			//normalize(grayImg, nuImg, 0.0, 1.0, cv::NORM_MINMAX);
			//toOf(nuImg, pixs);
			
			pixs = imgList[index_img].getPixels();
			for (int y = 0; y < mh; y++) {
				for (int x = 0; x < mw; x++) {

					//Vertex index
					int i = x + mw * y;
					//ofPoint p = mesh.getVertex(i);

					float scaleX = iw / mw;
					float scaleY = ih / mh;
					float offs_x = 0;
					float offs_y = 0;
					float offs_z = 0;

					ofVec2f p1(x*scaleX, y*scaleY);
					ofVec2f p2(iw / 2, ih / 2);
					if (p1.distance(p2) < 500) 
					{
						// get brightness
						int index = int((x * scaleX) + iw * (y * scaleY)); //
						ofFloatColor cColor(pixs[3 * index + offs_z] / 255.f, 			// r
							pixs[3 * index + 1 + offs_x] / 255.f,			// g
							pixs[3 * index + 2 + offs_y] / 255.f);			// b

						float bb = cColor.getBrightness();

						ofVec3f tmpVec = mesh.getVertex(i);
						//float mm = MIN(a_rms, 1);
						float mm = 1;
						tmpVec.z = ofLerp(tmpVec.z, -extrusionAmount/2 + (bb-0.5) * extrusionAmount, 0.9);// ;
						mesh.setVertex(i, tmpVec);
						//mesh.setColor(i, ofColor(cColor, 127 + int(mm * 150)));
						mesh.setColor(i, cColor);
					}
					else 
					{
						ofVec3f tmpVec = mesh.getVertex(i);
						tmpVec.z = 1000;// ;
						mesh.setVertex(i, tmpVec);
						mesh.setColor(i, ofColor(0, 0));
					}

					/*
					int brightness = pixs[index];
					//Change z-coordinate of vertex
					p.z = brightness*2;//+20 * ofNoise(x * 0.05, y * 0.05, ofGetElapsedTimef() * 0.5);
					//p.z = ofNoise(x * 0.05, y * 0.05, ofGetElapsedTimef() * 0.5) * 100;
					mesh.setVertex(i, p);
					//Change color of vertex
					//mesh.setColor(i, pixs[index]);
					*/
				}
			}
			if (en_dots == true) mesh.setMode(OF_PRIMITIVE_POINTS);
			else mesh.setMode(OF_PRIMITIVE_TRIANGLES);

			fbo_C.begin();
			fbo_C.end();
		}
		//blackFlag = false;
	} // end blackflag activity (now on draw)
}



//--------------------------------------------------------------
void ofApp::draw(){
	if (en_3d) 
	{
		//ofEnableDepthTest();
		ofSetColor(255, 255);
		ofClear(0, 0);
		ofEnableDepthTest();
		cam.begin();

		//ofPushMatrix();
		//ofTranslate(-iw / 2, -ih / 2, 0);

		if (en_bind) imgList[index_img].bind();
		if (en_faces) mesh.drawFaces();
		else if (en_wire) mesh.drawWireframe();
		else mesh.draw();
		if (en_bind) imgList[index_img].unbind();

		//ofPopMatrix();

		cam.end();
		ofDisableDepthTest();
		//ofDisableDepthTest();

	}
	else 
	{
		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
		ofSetColor(255, 255);
		fbo_A.draw(0, 0);
		ofSetColor(255, 255);
		fbo_B.draw(0, 0);
		ofDisableBlendMode();
	}

	// -the flag and the save
	if (blackFlag == true) {
		if (en_record == true) {
			tImg.grabScreen(0, 0, w, h);
			img_recorder.addFrame(tImg);
		}
		blackFlag = false;
	}

	if (en_3d)
	{
	}
	else
	{
		ofDrawBitmapStringHighlight("[edG]: " + ofToString(en_edges == true ? " on" : "off")
							+ "\t[write]: " + ofToString(en_write == true ? " on" : "off")
							+ "\t[edgThrA]: " + ofToString(edge_thr1)
							+ "\t[edgThrB]: " + ofToString(edge_thr2)
							+ "\t[lkQ]: " + ofToString(lkQ)
							+ "\t[vlThr]: " + ofToString(vlThr)
							+ "\t[fN]: " + ofToString(index_img), iw / 2 - 400, 20, 10);
	}

}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'e') { en_edges = !en_edges; }
	if (key == 's') { edge_thr1 = edge_thr1 > 0 ? edge_thr1 - 1 : 0; }
	if (key == 'd') { edge_thr1 = edge_thr1 <= 255 ? edge_thr1 + 1 : 255; }
	if (key == 'x') { edge_thr2 = edge_thr2 > 0 ? edge_thr2 - 1 : 0; }
	if (key == 'c') { edge_thr2 = edge_thr2 <= 255 ? edge_thr2 + 1 : 255; }

	if (key == 'f') { lkQ = lkQ >= 0.005 ? lkQ - 0.005 : 0.005; }
	if (key == 'g') { lkQ = lkQ <= 0.995 ? lkQ + 0.005 : 0.995; }
	if (key == 'v') { vlThr = vlThr > 0 ? vlThr - 1 : 0; }
	if (key == 'b') { vlThr = vlThr < 255 ? vlThr + 1 : 0; }

	if (key == 'l') { load_dir(); }

	if (key == 'k') { 
		//en_write = true;
		index_img = -1;
	}

	if (key == 'z') {
		tImg.grabScreen(0, 0, w, h);
		tImg.saveImage("imgs/frame_" + ofToString(t) + ".png");
	}

	if (key == 'r') {
		if (img_recorder.isThreadRunning()) {
			img_recorder.stopThread();
		}
		else {
			img_recorder.startThread(false, true);
		}
		en_record = true;
		index_img = -1;
		t1 = ofGetElapsedTimeMillis();
	}

	if (key == 'p') { en_3d = !en_3d; }
	if (key == 'o') { en_bind = !en_bind; }
	if (key == 'u') { en_faces = !en_faces; }
	if (key == 'i') { en_wire = !en_wire; }
	if (key == 'y') { en_dots = !en_dots; }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::exit() {
	img_recorder.waitForThread();
}



//--------------------------------------------------------------LOAD DIR
void ofApp::load_dir() {
	// ... ... ... --- --- ... ... ... --- --- ... ... --- files
	ofFileDialogResult result = ofSystemLoadDialog("Selecciona un archivo de la secuencia");
	if (result.bSuccess) {
		filepath = result.getPath();
		path = filepath.substr(0, filepath.rfind("\\"));
		while (path.find("\\") != string::npos) path.replace(path.find("\\"), 1, "/");
		cout << "[file]: " << filepath << endl;
		cout << "[path]: " << path << endl;
		nImgs = dir.listDir(path);
		nImgs = nImgs <= 500 ? nImgs : 500;
		fList.resize(0);
		cout << "[C_C]: " << nImgs << " images: " << endl;
		imgList.resize(nImgs);
		for (int i = 0; i < nImgs - 1; i++) {
			string fn = dir.getPath(i);
			//string fn = dir.getPath(1500 + i);
			imgList[i].load(fn);
			fList.push_back(fn);
			cout << "\t." << i << ".";
		}; cout << endl;
		isLoad = true;
	}
	if (nImgs > 0) {
		iw = imgList[0].getWidth();
		ih = imgList[0].getHeight();
	} else {
		iw = 0;
		ih = 0;
	}

	// features 
	feats.resize(0); // mesh
	acc_states.resize(0);
	for (int ix = 0; ix <= iw; ix += gridSz) {
		for (int iy = 0; iy <= ih; iy += gridSz) {
			feats.push_back(cv::Point2f(ix, iy));
			acc_states.push_back(0.0);
			//LK.setFeaturesToTrack(feats);
		}
	}

	// fbos
	fbo_A.allocate(iw, ih, GL_RGBA);
	fbo_B.allocate(iw, ih, GL_RGBA);
	fbo_C.allocate(iw, ih, GL_RGBA);
	fbo_D.allocate(iw, ih, GL_RGBA);
	buffer.allocate(iw, ih, GL_RGBA);
	rImg.allocate(iw, ih, OF_IMAGE_COLOR_ALPHA);
	optoMap.allocate(iw, ih, OF_IMAGE_COLOR_ALPHA);
	actImg = toCv(imgList[0]);
	cvtColor(actImg, grayImg, CV_RGB2GRAY);
	prevImg = grayImg;
}