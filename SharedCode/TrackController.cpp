
#include "TrackController.h"
TrackController::TrackController(){
	editMode = false;
}

TrackController::~TrackController(){
	
}

void TrackController::setup(int numTracks){
	timelineWidth = ofGetWidth();
	vector<string> palettes;
	palettes.push_back("../../../SharedAssets/spring.png");
	palettes.push_back("../../../SharedAssets/summer.png");
	palettes.push_back("../../../SharedAssets/autumn.png");
	palettes.push_back("../../../SharedAssets/winter.png");
	
	for(int i = 0; i < numTracks; i++){
		ofxTimeline* timeline = new ofxTimeline();
		
		timeline->setup();
		timeline->setLoopType(OF_LOOP_NORMAL);
		timeline->setMinimalHeaders(true);
		timeline->setFootersHidden(true);
		timeline->setShowTimeControls(false);
		timeline->setDurationInSeconds(60);
		timeline->addColorsWithPalette("primary color", palettes[i]);
		timeline->addColorsWithPalette("secondary color", palettes[i]);
		timeline->addLFO("birth rate", ofRange(0, 1.0));
		timeline->addLFO("life span", ofRange(0, 100));
		timeline->addLFO("perlin amp", ofRange(0, .2));
		timeline->addLFO("perlin density", ofRange(0, .2));
		timeline->addLFO("gravity amp", ofRange(0, .2));
		timeline->addLFO("spin force", ofRange(0, .2));
		timelines.push_back(timeline);

		ParticleRenderer* particleRenderer = new ParticleRenderer();
		particleRenderer->setup(30000);
		
		particleRenderers.push_back(particleRenderer);
		
	}
}

void TrackController::toggleFooters(){
	for(int i = 0; i < timelines.size(); i++){
		timelines[i]->setShowZoomer(timelines[i]->toggleShowFooters());
	}
}

void TrackController::setWidth(float width){
	if(width == timelineWidth){
		return;
	}

	timelineWidth = width;
	for(int i = 0; i < timelines.size(); i++){
		timelines[i]->setWidth(timelineWidth);
	}
}

void TrackController::draw(){
	
	for(int i = 0; i < timelines.size(); i++){
		if(i != 0){
			timelines[i]->setOffset(timelines[i-1]->getBottomLeft() );
		}
		if(!editMode){
			timelines[i]->getZoomer()->setViewRange(ofRange(timelines[i]->getPercentComplete()-.06,
															timelines[i]->getPercentComplete()+.06));
		}
		timelines[i]->draw();
	}
}

void TrackController::drawParticles(){
	for(int i = 0; i < particleRenderers.size(); i++){
		particleRenderers[i]->draw();
	}
}

void TrackController::update(){
	
	for(int i  = 0; i < timelines.size(); i++){
		if(timelines[i]->getIsPlaying()){

			particleRenderers[i]->perlinForce->amplitude = timelines[i]->getValue("perlin amp");
			particleRenderers[i]->perlinForce->density = timelines[i]->getValue("perlin amp");
			particleRenderers[i]->gravityForce->gravity = timelines[i]->getValue("gravity amp");
			particleRenderers[i]->spinForce->power = timelines[i]->getValue("spin force");
			particleRenderers[i]->primaryColor   = timelines[i]->getColor("primary color");
			particleRenderers[i]->secondaryColor = timelines[i]->getColor("secondary color");
			particleRenderers[i]->birthRate = timelines[i]->getValue("birth rate");
			particleRenderers[i]->lifeSpan = timelines[i]->getValue("life span");
			
		}
		particleRenderers[i]->update();
	}
}


void TrackController::setPositions(vector<ofVec2f> positions){
	drawRect = ofRectangle(0,0,0,0);
	for(int i = 0; i < timelines.size(); i++){
		drawRect = drawRect.getUnion(timelines[i]->getDrawRect());
	}
	
	for(int t = 0; t < timelines.size(); t++){
		bool occupied = false;
		for(int i = 0; i < positions.size(); i++){
//			ofVec2f screenPosition = positions[i] * ofVec2f(totalDrawRect.width,totalDrawRect.height);
			ofVec2f screenPosition = positions[i];
			if(timelines[t]->getDrawRect().inside(screenPosition)){
				occupied = true;
				break;
			}
		}
		
		if(!editMode){
			if(occupied && !timelines[t]->getIsPlaying()){
//				cout << t << " is occupied and now playing" << endl;
				timelines[t]->play();
			}
			else if(!occupied && timelines[t]->getIsPlaying()){
//				cout << t << " is no longer occupied" << endl;
				timelines[t]->stop();
			}
		}
	}
}

void TrackController::fitToScreenHeight(){
	float heightPerTimeline = ofGetHeight() / timelines.size();
	for(int i = 0; i < timelines.size(); i++){
		timelines[i]->setHeight(heightPerTimeline);
	}
}

void TrackController::togglePlayForTrackAtPoint(ofVec2f point){
	for(int t = 0; t < timelines.size(); t++){
		if(timelines[t]->getDrawRect().inside(point)){
			timelines[t]->togglePlay();
		}
	}
}

