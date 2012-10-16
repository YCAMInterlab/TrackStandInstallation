/**
 *
 * TrackStand interactive installation
 * researching creative uses of ofxTimeline in installation context
 */
 
#pragma once

#include "ofMain.h"
#include "ofxTimeline.h"
#include "ParticleRenderer.h"

class TrackController {
  public:
	TrackController();
	~TrackController();
	
	void setup(int numTracks);
	void update();
	void draw();
	
	void setPositions(vector<ofVec2f> positions);
	vector<ofxTimeline*> timelines;
	void setWidth(float width);
	bool editMode;
	ofRectangle drawRect;
	ParticleRenderer* particles;
	
	void toggleFooters();
	void togglePlayForTrackAtPoint(ofVec2f point);
  protected:

	float timelineWidth;
};

