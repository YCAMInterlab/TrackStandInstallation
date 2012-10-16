//
//  KinectSet.h
//
#pragma once
#include "ofxXmlSettings.h"

#include "Device.h"
#include "PersonCentroid.h"

#ifdef USE_GRABSCENE
#include "ofxGrabScene.h"
class KinectSet : public GrabScene::Node {
#else
class KinectSet : public ofNode {
#endif
public:
	KinectSet();
	vector<Device*> devices;
	void update();
	void customDraw();
	void load();
	void save();
	string getFilename() { return "../../../common-data/tracking-params.xml"; }
protected:
	void updateRegion(float & dummy);
	void updateResolution(int & dummy);
	void updateTrackingParameters(int & dummy);
	
#ifdef USE_GRABSCENE
	ofParameter<float> height;
	ofParameter<float> width;
	ofParameter<float> depth;
	ofParameter<int> resolution;
	ofParameter<int> minArea;
	ofParameter<int> maxArea;
	ofParameter<int> threshold;
	ofParameter<int> persistence;
	ofParameter<int> maxDistance;
#else
	float height;
	float width;
	float depth;
	int resolution;
	int minArea;
	int maxArea;
	int threshold;
	int persistence;
	int maxDistance;
#endif
	
	ofMatrix4x4 region;
	
	ofFbo fromAbove;
	ofPixels pixelsCol;
	cv::Mat pixelsMono;
	ofxCv::ContourFinder contourFinder;
	ofxCv::RectTrackerFollower<PersonCentroid> tracker;
};