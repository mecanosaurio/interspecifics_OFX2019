#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		// los tipos de variable dice qué clase de valor contienen
		// un "int" almacena un valor entero
		// el tipo "float" almacenan valores fraccionarios o reales
		// el tipo "bool" almacena valores lógicos: verdadero o falso
		// el tipo "ofPoint" almacena las coordenadas de un punto
		// el tipo "ofVec3f" almacena las 3 coordenadas a las que apunta un vector
		int origenX, origenY;
		ofPoint inicioEjeX, finEjeX, inicioEjeY, finEjeY;



};
