//
//  PersonCentroid.cpp
//
#include "PersonCentroid.h"

using namespace ofxCv;
using namespace cv;

const float dyingTime = 0.2f;

//---------
void PersonCentroid::setup(const cv::Rect& track) {
	color.setHsb(ofRandom(0, 255), 255, 255);
	cur = toOf(track).getCenter();
	smooth = cur;
}

//---------
void PersonCentroid::update(const cv::Rect& track) {
	cur = toOf(track).getCenter();
	smooth.interpolate(cur, .5);
	all.addVertex(smooth);
}

//---------
void PersonCentroid::kill() {
	float curTime = ofGetElapsedTimef();
	if(startedDying == 0) {
		startedDying = curTime;
	} else if(curTime - startedDying > dyingTime) {
		dead = true;
	}
}

//---------
void PersonCentroid::draw() {
	ofPushStyle();
	
	float size = 16;
	ofSetColor(200, 100, 100);
	if(startedDying) {
		ofSetColor(ofColor::red);
		size = ofMap(ofGetElapsedTimef() - startedDying, 0, dyingTime, size, 0, true);
	}
	
	ofSetLineWidth(2.0f);
	ofLine(smooth.x, smooth.y, 0.0f, smooth.x, smooth.y, 1.0f);
	ofCircle(smooth.x, smooth.y, 1.0f, 0.5f);
	
	ofSetLineWidth(1.0f);
	all.draw();
	
	ofNoFill();
	ofSetColor(255);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
	ofDrawBitmapString(ofToString(label), cur);
	
	
	ofPopStyle();
}