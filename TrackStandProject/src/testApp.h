#pragma once

#include "ofMain.h"
#include "ofxTimeline.h"
#include "TrackController.h"
#include "ofxTLDepthImageSequence.h"
#include "ofxGameCamera.h"
#include "KinectSet.h"
#include "Kinect.h"
#include "ofxRGBDMeshBuilder.h"
#include "ofxGui.h"

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

	ofxPanel smallGui;
	ofxFloatSlider bottom1;
    ofxFloatSlider bottom2;

	ofxGameCamera cam;
	ofxGrabCam grabcam;
	
	TrackController trackController;
	
	ofRectangle previewRect;
	
	bool useTestRecording;
	ofxTimeline globalparams;
	ofxTimeline recordingTest;
	ofxTLDepthImageSequence depthSequence;
	
	Kinect * kinect[2];
	KinectSet kinects;
	
	vector<ofVec3f*> points;
	ofxRGBDMeshBuilder meshBuilder;
};
