#ifndef _AGENTS
#define _AGENTS
#include "ofMain.h"

class dropForm {
public:
	dropForm();
	~dropForm();
	void setup(vector<ofPoint> ps);
	void update(int mode);
	void update(int mode, float vel);
	void draw();
	void draw_echo(int sE, int st);
	ofPolyline shape;
	vector<ofPoint> points;
	ofPoint tPoi;
	ofColor c;
	bool isFresh;
};

class colorImgPix{
public:
	colorImgPix();
	~colorImgPix();
	void setup(std::string fn );
	ofColor pick();
	string fileName;
	ofPixels pix;
	ofColor color;
	ofPoint pc;

};

class lineWave {
public:
	lineWave();
	~lineWave();
	void setup(ofPoint position, ofColor co);
	void popWith(vector<float> mB, float sc);
	void update();
	void draw();
	void fall();
	void redoTrail();

	ofPoint pos, vel;
	ofPolyline line;
	ofMesh mesh;
	ofColor color;
	float r, seg;
	int n;
	bool isDead, isDown;

	vector<ofPoint> trail;
	ofTessellator tess;
	float a, b, c;
	bool isExpl;
private:
};


class Block {
public:
	Block();
	~Block();
	void setup(ofPoint position, int ntr);
	void go_to();
	void update(float tt);
	void draw();

	ofPoint pos;
	ofPoint dest;

	vector<ofPoint> trail;
	int spc_trail, spc_td, nt;
	ofPolyline shape;
	float a, b, c, r, v;
	float gravity;
	int n;
	ofColor color;
	bool isDead;
	bool isExpl;
	int cW, cH, cwd;
private:
};


// ----------------------------------------------------------------------------

class Agent01{
public:
	Agent01();
	~Agent01();
	void setup();
	void update(bool mov, float vel, bool wal, int wdir, bool multi, bool trail, float rms);
	void draw();

	ofFbo fbo_m1_front, fbo_m1_back, fbo_m1_delay, fbo_a1;
	vector<ofPolyline> shapes;
	vector<ofPoint> tPoints, tPointsA;
	ofPolyline tLine;
	ofPoint tPoi;
	int r, w, h;
	bool mo, wa, mu, tr;

	ofShader tShader;
	ofEasyCam cam;
private:
};


class Agent02 {
public:
	Agent02();
	~Agent02();
	void setup();
	void update(bool mov, float ran, bool wal, bool re, vector<float> mb);
	void draw();

	ofFbo fbo_m2_front, fbo_m2_back, fbo_m2_trail, fbo_a2;
	vector<ofPoint> daPoints, lay0, lay1, lay2, lay3;
	ofPolyline tLine;
	ofPoint tPoi, tPoe;
	int r, w, h, t;
	float tt;
	bool wa;

	ofShader tShader, t2Sh;
	ofEasyCam cam;
private:
};


class Agent03{
public:
	Agent03();
	~Agent03();
	void setup();
	void update(bool mov, float max_ang, bool wal, vector<float> mb);
	void draw();
	ofFbo fbo_m3_front, fbo_m3_back, fbo_m3_trail, fbo_a3;
	vector<ofPolyline> shapes;
	vector<ofPoint> tPoints;
	ofPolyline tLine;
	ofPoint tPoi, tPoo;
	int r, w, h, t;
	float tt;
	bool wa;

	ofShader t1Sh, t2Sh;
	ofEasyCam cam;
private:
	int past = 0;
};


class Agent04 {
public:
	Agent04();
	~Agent04();
	void setup();
	void update(bool mov, float noise, bool wal, vector<float> mb);
	void draw();
	ofFbo fbo_m4_front, fbo_m4_back, fbo_m4_trail, fbo_a4;
	vector<dropForm> forms;
	vector<ofPoint> tPoints;
	ofPolyline tLine;
	ofPoint tPoi, tPoo;
	int r, w, h, t;
	float tt;
	bool wa;

	ofShader t41Sh, t42Sh;
	ofEasyCam cam;
	ofMesh mesh;
	ofImage img;
	ofPixels pixs;
	dropForm tForm;

	ofPoint pColor;
	ofColor iColor;
	bool isHi;

private:
	int past = 0;
};


class Agent05 {
public:
	Agent05();
	~Agent05();
	void setup();
	void update(bool mov, float vel, bool grow, bool wal, vector<float> mb);
	void draw();
	ofFbo fbo_m5_front, fbo_m5_back, fbo_m5_trail, fbo_a5;
	vector<ofPoint> pos;
	vector<dropForm> forms;
	vector<ofPoint> tPoints;
	ofPolyline tLine;
	ofPoint tPoi, tPoo;
	int r, w, h, t;
	float tt;
	bool wa;

	ofShader t51Sh, t52Sh;
	ofEasyCam cam;
	ofMesh mesh;
	ofImage img;
	ofPixels pixs;
	dropForm tForm;

	ofPoint pColor;
	ofColor iColor;
	bool isHi;

	colorImgPix colorPick;
};



class Agent06 {
public:
	Agent06();
	~Agent06();
	void setup();
	void update(bool fal, float per, float ext, vector<float> mb);
	void draw();
	ofFbo fbo_m6_front, fbo_m6_back, fbo_m6_trail, fbo_a6;
	vector<ofPoint> pos;
	vector<dropForm> forms;
	vector<ofPoint> tPoints;
	ofPolyline tLine;
	ofPoint tPoi, tPoo;
	int r, w, h, t, t0;
	float tt;
	bool wa;

	float grav;
	vector <lineWave> waves;
	lineWave *tWave;

	ofShader t61Sh, t62Sh;
	ofEasyCam cam;
	ofMesh mesh;
	ofImage img;
	ofPixels pixs;
	dropForm tForm;

	ofPoint pColor;
	ofColor iColor;
	bool isHi;

	colorImgPix colorPick;
};


class Agent07 {
public:
	Agent07();
	~Agent07();
	void setup();
	void update(bool fal, float per, float ext, vector<float> mb, vector <float> d0);
	void draw();
	ofFbo fbo_m7_front, fbo_m7_back, fbo_m7_trail, fbo_a7;
	vector<ofPoint> pos;
	vector<dropForm> forms;
	vector<ofPoint> tPoints;
	ofPolyline tLine;
	ofPoint tPoi, tPoo;
	int r, w, h, t, t0;
	float tt;
	bool wa;

	float grav;
	vector <lineWave> waves;
	lineWave *tWave;
	ofPoint dests[72];

	vector <Block> theBlocks;
	Block *tempBlock;
	ofPoint p_a;

	ofShader t71Sh, t72Sh;
	ofEasyCam cam;
	ofMesh mesh;
	ofImage img;
	ofPixels pixs;
	dropForm tForm;

	ofPoint pColor;
	ofColor iColor;
	bool isHi;

	colorImgPix colorPick;
};


#endif