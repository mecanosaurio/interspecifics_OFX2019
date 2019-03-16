#pragma once
#include "ofMain.h"
class Particle
{
public:
	Particle();
	~Particle();

	void setup(float _m, ofPoint _loc);
	void applyForce(ofVec3f force);
	void update();
	void updateVeloc(ofVec3f veloc);
	void updateS(float s);
	void updateLoc(ofPoint _loc);
	void setColor(ofColor _color);
	void draw();

	ofPoint loc;
	ofVec3f velocity;
	ofVec3f acceleration;
	ofColor color;
	float mass;
	float size;
	int ww, hh;
};

