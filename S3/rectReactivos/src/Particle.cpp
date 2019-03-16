#include "Particle.h"

Particle::Particle(){
}

Particle::~Particle(){
}

void Particle::setup(float _m, ofPoint _loc){
	mass = _m;
	size = 2;
	loc = _loc;
	velocity.set(5, 0, 0);
	acceleration.set(0, 0, 0);
	ww = 350;
	hh = 600;
	color = ofColor::white;
}


void Particle::applyForce(ofVec3f force) {
	ofVec3f f = force / mass;
	acceleration += f/2;
}


void Particle::update() {
	velocity += acceleration;
	loc += velocity/2;
	acceleration *= 0;
}


void Particle::updateVeloc(ofVec3f veloc) {
	velocity = veloc;
	loc += velocity / 2;
}

void Particle::setColor(ofColor _color) {
	color = _color;
}



void Particle::updateS(float s) {
	size = s*10;
}

void Particle::updateLoc(ofPoint _loc) {
	loc = _loc;
}

void Particle::draw() {
	ofSetColor(color);
	ofNoFill();
	ofSetLineWidth(3);
	ofRect(loc.x-ww/2, loc.y / 2, loc.z, ww, hh);
	//ofRect(loc2f, size, size);
	//ofLine(loc.x, -ofGetHeight()/2, loc.x, ofGetHeight()/2);
	//ofLine(-ofGetWidth() / 2, loc.y, ofGetWidth()/2, loc.y);
}

