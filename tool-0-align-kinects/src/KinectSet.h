//
//  KinectSet.h
//
#pragma once

#include "ofxGrabScene.h"

#include "Device.h"
#include "PersonCentroid.h"

class KinectSet : public GrabScene::Node {
public:
	KinectSet();
	vector<Device*> devices;
	void update();
	void draw();
	void load();
	void save();
	string getFilename() { return "../../../common-data/tracking-params.xml"; }
protected:
	void updateRegion(float & dummy);
	void updateResolution(int & dummy);
	void updateTrackingParameters(int & dummy);
	
	ofParameter<float> height;
	ofParameter<float> width;
	ofParameter<float> depth;
	ofParameter<int> resolution;
	ofParameter<int> minArea;
	ofParameter<int> maxArea;
	ofParameter<int> threshold;
	ofParameter<int> persistence;
	ofParameter<int> maxDistance;
	
	ofMatrix4x4 region;
	
	ofFbo fromAbove;
	ofPixels pixelsCol;
	cv::Mat pixelsMono;
	ofxCv::ContourFinder contourFinder;
	ofxCv::RectTrackerFollower<PersonCentroid> tracker;
};