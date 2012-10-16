//
//  PersonCentroid.h
//
//
#pragma once

#include "ofxCv.h"

class PersonCentroid : public ofxCv::RectFollower {
protected:
	ofColor color;
	ofVec2f cur, smooth;
	float startedDying;
	ofPolyline all;
public:
	PersonCentroid()
	:startedDying(0) {
	}
	ofVec2f getPosition() { return this->smooth; }
	void setup(const cv::Rect& track);
	void update(const cv::Rect& track);
	void kill();
	void draw();
};
