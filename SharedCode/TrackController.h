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
	void drawParticles();
	
	void setPositions(vector<ofVec2f> positions);
	vector<ofxTimeline*> timelines;
	void setWidth(float width);
	bool editMode;
	
	ofRectangle drawRect;
	vector<ParticleRenderer*> particleRenderers;
	
	void toggleFooters();
	void togglePlayForTrackAtPoint(ofVec2f point);
	void fitToScreenHeight();
	
  protected:

	float timelineWidth;
};

