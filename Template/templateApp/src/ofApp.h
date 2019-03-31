#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGui.h"
#include "ofxMaxim.h"
#include "maxiMFCC.h"
#include "ofxPostGlitch.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void updateSceneA();
		void drawSceneA();
		void updateSceneB();
		void drawSceneB();
		void updateSceneC();
		void drawSceneC();
		void updateSceneD();
		void drawSceneD();
		
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
		
		// ---| audio + maxim stuff |
		void audioRequested(float * input, int bufferSize, int nChannels);
		void audioReceived(float * input, int bufferSize, int nChannels);
		float 	* lAudioOut;
		float   * rAudioOut;
		float * lAudioIn;
		float * rAudioIn;
		int		initialBufferSize;
		int		sampleRate;
		double wave, sample, outputs[2], ifftVal;
		maxiMix mymix;
		maxiOsc osc;
		ofxMaxiFFTOctaveAnalyzer oct;
		int nAverages;
		float *ifftOutput;
		int ifftSize;
		float peakFreq = 0;
		float centroid = 0;
		float RMS = 0;
		ofxMaxiIFFT ifft;
		ofxMaxiFFT mfft;
		int fftSize;
		int bins, dataSize;
		maxiMFCC mfcc;
		double *mfccs;
		maxiSample samp;
		vector< vector<float> > dataBuff;
		vector<float> d0;
		vector<float> mB;
		int dBuffSize;
		int nBands;
		int rot;
		float softRMS;

		// ---| osc|
		ofxOscReceiver recvOsc;
		ofxOscSender sendOsc;
		int v1, v2;

		// ---| gui|-
		bool showGui;
		ofxPanel gui;
		ofxGuiGroup mixer_group;
		ofxGuiGroup sceneA_group, sceneB_group;
		ofxGuiGroup sceneC_group, sceneD_group;
		ofxToggle enA, enB, enC, enD;
		ofxIntSlider alphaA, alphaB, alphaC, alphaD;

		// ---| base stuff |
		int w, h, t, ff;
		float tt;
		bool enDsp, enGui, enRec;
		ofFbo fboSceneA, fboSceneB, fboSceneC, fboSceneD;
		ofFbo fboSceneTmp;

		// --stuff scene A
		ofxColorSlider cA1, cA2, cA3;
		ofxIntSlider iFA1, iFA2, iFA3;
		ofxFloatSlider amA1, amA2, amA3;
		ofxToggle bgClearA;
		ofxFloatSlider thrBlendScreenA;
		ofBoxPrimitive box, box1, box2, box3;
		ofColor colorBox1, colorBox2, colorBox3;
		float szBox1, szBox2, szBox3;
		float tmp01, tmp02;

		// stuff scene B
		ofxIntSlider vaB1, vaB2;
		ofxIntSlider indexFreqB1, indexFreqB2, indexFreqB3;
		ofxFloatSlider amplitudB1, amplitudB2, amplitudB3;
		int cbX, cbY;

		// stuff scene C
		ofxIntSlider vaC1, vaC2;
		ofxIntSlider indexFreqC1, indexFreqC2, indexFreqC3;
		ofxFloatSlider amplitudC1, amplitudC2, amplitudC3;
		int nImgs;
		ofPoint listaPos[100];
		ofPoint listaVelocs[100];
		ofImage imageC1, imageC2, imageC3, imageC4, imageC5;
};
