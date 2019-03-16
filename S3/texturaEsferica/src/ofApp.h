#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxGui.h"
#include "ofxXmlSettings.h"
#include "ofxOsc.h"
#include "maxiMFCC.h"
#include "ofxFXObject.h"
#include "ofxGlow.h"
#include "ofxPostGlitch.h"


#define OSC_IN_PORT 12000


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();

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
		
		void draw3D();
		void drawMini();

		//                                            --------------------------
		// ------ ------ ------ ------ ------ ------ | audio + maximilian stuff |
		//                                            --------------------------
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


		//                                            -------------------
		// ------ ------ ------ ------ ------ ------ | data buffer stuff |
		//                                            -------------------
		vector< vector<float> > dataBuff;
		vector<float> d0;
		vector<float> mB;
		int dBuffSize;
		int nBands;
		int rot;


		//                                            ---- ------
		// ------ ------ ------ ------ ------ ------ | gui stuff |
		//                                            --- -------
		bool showGui;
		ofTrueTypeFont myfont, myFont2;

		ofxPanel gui;

		ofxGuiGroup sceneGroup;
		ofxToggle sc0;
		ofxToggle sc1;
		ofxToggle sc2;
		ofxToggle sc3;
		ofxToggle sc4;

		ofxGuiGroup mixerGroup;
		ofxFloatSlider show2D, show3D;
		ofxFloatSlider rad, extrude;
		ofxToggle fx1;

		ofxGuiGroup generalGroup;
		ofxToggle dspEnabled;
		ofxToggle img1Enabled;
		ofxIntSlider invContourSpacing;

		ofxGuiGroup mindGroup;
		ofxToggle mindEnabled;
		ofxIntSlider mMedit;
		ofxIntSlider mConc;

		ofxGuiGroup kinectGroup;
		ofxToggle kinEnabled;
		ofxIntSlider nearThreshold;
		ofxIntSlider farThreshold;

		ofxGuiGroup camGroup;
		ofxToggle camEnabled;
		ofxIntSlider camThreshold;
		ofxToggle bLearnBackground;

		ofxGuiGroup kaleidoGroup;
		ofxToggle kenabled;
		ofxIntSlider ksectors;
		ofxFloatSlider kangle, kx, ky;


		//                                            --------------
		// ------ ------ ------ ------ ------ ------ | kinect stuff |
		//                                            --------------
		float med, con, blink;
		float met, kon, blinc;
		ofColor color;
		ofxOscReceiver receiver;

		ofxKinect kinect;
		ofxCvColorImage colorImg;
		ofxCvGrayscaleImage gImg;
		ofxCvGrayscaleImage bgImg;
		ofxCvGrayscaleImage gThrNea;
		ofxCvGrayscaleImage gThrFar;
	
		ofxCvContourFinder cFinder;
		ofEasyCam easycam;
		ofPolyline line;
		int angle;
		int gb;
		float minBlobSize, maxBlobSize;


		//                                            --------------
		// ------ ------ ------ ------ ------ ------ | video stuff |
		//                                            --------------
		ofVideoPlayer video01;
		ofVideoGrabber cam;
		ofPixels pixels;
		ofImage img1, img2;


		//                                            -------- ------
		ofMesh mesh;
		ofPolyline poly;



		int nP, nPx, nPy;
		int t, h, w, epoch;
		bool nNcon = false;
		float tt;
		int tt0, tt1;
		ofDirectory dir;
		int n_imgs_00, n_imgs_03;

		//                                            -------- ------
		// ------ ------ ------ ------ ------ ------ | SC00           |
		//                                            ----- ---------
		vector<ofTexture> seq_00;
		ofFbo fbo_00;
		int index_00;
		int fNo_00;

		//                                            -------- ------
		// ------ ------ ------ ------ ------ ------ | SC01           |
		//                                            ----- ---------
		ofFbo fbo_01;
		int fNo_01;
		ofMesh silho;
		vector<ofPoint> stars;
		vector<ofColor> colors;
		ofPoint moonCenter;

		//                                            -------- ------
		// ------ ------ ------ ------ ------ ------ | SC02           |
		//                                            ----- ---------
		int fNo_02;
		ofMesh silho2;
		ofFbo fbo_02a, fbo_02b;
		ofxGlow glow;
		ofxPostGlitch myGlitch;

		//                                            -------- ------
		// ------ ------ ------ ------ ------ ------ | SC03           |
		//                                            ----- ---------
		int fNo_03;
		ofFbo fbo_03;
		vector<ofTexture> seq_03;
		int index_03;

		//                                            -------- ------
		// ------ ------ ------ ------ ------ ------ | SC04          |
		//                                            ----- ---------
		int fNo_04;
		ofFbo fbo_04a, fbo_04b;
		ofShader shader_kale;

};
