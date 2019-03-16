#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxGui.h"
#include "ofxOsc.h"
#include "maxiMFCC.h"

#include "Particle.h"

#define IN_PORT 12000

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	void exit();

	void smLerp();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	// ------------ ------------ ------------ audio stuff
	void audioRequested(float * input, int bufferSize, int nChannels);		/* output method */
	void audioReceived(float * input, int bufferSize, int nChannels);		/* input method */

	float 	* lAudioOut;													/* outputs */
	float   * rAudioOut;
	float * lAudioIn;														/* inputs */
	float * rAudioIn;
	int		initialBufferSize;												/* buffer size */
	int		sampleRate;

	// ------------ ------------ ------------ maximilian stuff
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

	// ------------ ------------ ------------ data buffer stuff
	vector< vector<float> > dataBuff;
	vector<float> d0;
	vector<float> mB;
	int dBuffSize;
	int nBands;
	int rot;


	// ------------ ------------ ------------ kinect stuff
	float med, con, blink;
	float met, kon, blinc;
	ofColor color;
	ofxOscReceiver receiver;


	ofxKinect kinect;

	ofxCvColorImage colorImg;
	ofxCvGrayscaleImage gImg;
	ofFbo fbo;
	ofxCvGrayscaleImage gThrNea; // the near thresholded image
	ofxCvGrayscaleImage gThrFar; // the far thresholded image


	ofxCvContourFinder cFinder;
	ofEasyCam cam;
	ofPolyline line;
	int angle;
	int gb;
	float minBlobSize, maxBlobSize;

	ofVideoPlayer video01;
	ofPixels pixels;

	// ------------ ------------ ------------ gui stuff
	bool bHide;
	ofTrueTypeFont myfont, myFont2;

	ofxPanel gui;
	ofxFloatSlider meditate;
	ofxFloatSlider concentrate;
	ofxIntSlider nearThreshold;
	ofxIntSlider farThreshold;

	// ------------ ------------ ------------ particles stuff
	vector<ofPoint> points;
	vector<ofColor> colors;
	vector<Particle> particles;
	int nP;
	int t, h, w, epoch;
};
