#pragma once

#include "ofMain.h" 
#include "ofxGui.h"
#include "ofxXmlSettings.h"
#include "ofxMaxim.h"
#include "maxiMFCC.h"
#include "Agents.h"
#include "ofxSpout.h"
#include "ofxImageSequenceRecorder.h"


/*// --- SYPHON
#include "ofxSyphon.h"
*/


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
		void exit();
		// ------ ------ ------ ------ ------ ------ | audio + maximilian stuff |
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

		// ------ ------ ------ ------ ------ ------ | data buffer stuff |
		vector< vector<float> > dataBuff;
		vector<float> d0;
		vector<float> mB;
		int dBuffSize;
		int nBands;
		int rot;

		// ------ ------ ------ ------ ------ ------ | gui stuff |
		bool showGui;
		ofxPanel gui;

		ofxGuiGroup mixGroup;
		ofxToggle dsp_enable;
		ofxToggle send_syph;

		ofxGuiGroup mov1Group;
		ofxToggle mov1_en, mov1_mov, mov1_wall, mov1_multi, mov1_trail;
		ofxIntSlider mov1_fade, mov1_wdir;
		ofxFloatSlider mov1_vel;

		ofxGuiGroup mov2Group;
		ofxToggle mov2_en, mov2_mov, mov2_wall, mov2_par;
		ofxIntSlider mov2_fade;
		ofxFloatSlider mov2_ran, mov2_mop;

		ofxGuiGroup mov3Group;
		ofxToggle mov3_en, mov3_mov, mov3_wall;
		ofxIntSlider mov3_fade;
		ofxFloatSlider mov3_max_ang;

		ofxGuiGroup mov4Group;
		ofxToggle mov4_en, mov4_mov, mov4_wall;
		ofxIntSlider mov4_fade;
		ofxFloatSlider mov4_noise;

		ofxGuiGroup mov5Group;
		ofxToggle mov5_en, mov5_mov, mov5_wall;
		ofxIntSlider mov5_fade;
		ofxFloatSlider mov5_vel;
		ofxToggle mov5_orbit;
		ofxToggle mov5_grow;

		ofxGuiGroup mov6Group;
		ofxToggle mov6_en, mov6_fal;
		ofxIntSlider mov6_fade;
		ofxFloatSlider mov6_per, mov6_ext;
		ofxToggle mov6_orbit;
		ofxToggle mov6_grow;

		ofxGuiGroup mov7Group;
		ofxToggle mov7_en, mov7_fal;
		ofxIntSlider mov7_fade;
		ofxFloatSlider mov7_per, mov7_ext;
		ofxToggle mov7_orbit;
		ofxToggle mov7_grow;

		// ------ ------ ------ ------ ------ ------ | regular stuff |
		int w, h, t, t0;
		float tt, grav;
		float slowRMS;
		// pixcam and color
		ofPixels pix;
		ofColor c1;
		ofFbo fbo_glitch;
		//ofxPostGlitch myGlitch;
		
		// ------ ------ ------ ------ ------ ------ | recorder stuff |
		bool recording;
		ofImage tImg;
		ofxImageSequenceRecorder recorder;

		// ------ ------ ------ ------ ------ ------ | agents stuff |
		Agent01 A1;
		Agent02 A2;
		Agent03 A3;
		Agent04 A4;
		Agent05 A5;
		Agent06 A6;
		Agent07 A7;

		/*// --- SYPHON
		ofxSyphonServer individualTextureSyphonServer;
		//ofTexture tex;
		//ofxSyphonServer mainOutputSyphonServer;
		//ofxSyphonClient mClient;
		*/
		ofxSpout::Sender sender;
		ofImage rImg;

};

