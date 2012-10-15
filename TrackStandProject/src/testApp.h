#pragma once

#include "ofMain.h"
#include "ofxTimeline.h"
#include "TrackController.h"
#include "ParticleRenderer.h"
#include "ofxTLDepthImageSequence.h"
#include "ofxGameCamera.h"

class testApp : public ofBaseApp{
  public:
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void exit();

	ofxGameCamera cam;
	
	TrackController trackController;
	ParticleRenderer particleRenderer;
	
	ofRectangle previewRect;
	
	bool useTestRecording;
	ofxTimeline recordingTest;
	ofxTLDepthImageSequence depthSequence;
};
