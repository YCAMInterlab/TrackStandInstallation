
#include "TrackController.h"
TrackController::TrackController(){
	editMode = false;
	zoomPercent = .06*.06;
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
	
	vector<string> sounds;
	sounds.push_back("../../../SharedAssets/sound/spring.wav");
	sounds.push_back("../../../SharedAssets/sound/summer.wav");
	sounds.push_back("../../../SharedAssets/sound/autumn.wav");
	sounds.push_back("../../../SharedAssets/sound/winter.wav");
	
	for(int i = 0; i < numTracks; i++){
		ofxTimeline* timeline = new ofxTimeline();
		
		ofxTLAudioTrack* t = new ofxTLAudioTrack();
		t->loadSoundfile(sounds[i]);
		audioTracks.push_back(t);

		timeline->setup();
		timeline->getZoomer()->setViewExponent(1.0);
		timeline->setLoopType(OF_LOOP_NORMAL);
		timeline->setMinimalHeaders(true);
		timeline->setFootersHidden(true);
		timeline->setShowTimeControls(false);
		timeline->setDurationInSeconds(60);
		timeline->setTimecontrolTrack(t);
		timeline->setDurationInSeconds(t->getDuration());
		timeline->addColorsWithPalette("primary color", palettes[i]);
		timeline->addColorsWithPalette("secondary color", palettes[i]);
		
		timeline->addTrack("sound", t);
		timeline->addLFO("birth rate", ofRange(0, 1.0));
		timeline->addLFO("life span", ofRange(0, 100));
		timeline->addLFO("perlin amp", ofRange(0, .2));
		timeline->addLFO("gravity amp", ofRange(0, .2));
		timelines.push_back(timeline);
	}
	
	particleRenderer1 = new ParticleRenderer();
	particleRenderer1->setup(15000);
	particleRenderer2 = new ParticleRenderer();
	particleRenderer2->setup(15000);

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
			if(timelines[i]->getIsPlaying()){
				timelines[i]->getZoomer()->setViewRange(ofRange(timelines[i]->getPercentComplete()-zoomPercent,
																timelines[i]->getPercentComplete()+zoomPercent));
//				audioTracks[i]->setVolume(1.0*ofGetMouseX()/timelines[i]->getDrawRect().width);
			}
		}
		timelines[i]->draw();
	}
	ofDrawBitmapString(ofToString(particleRenderer1->totalParticles+particleRenderer2->totalParticles), ofGetWidth() - 100, ofGetHeight()-20);

}

void TrackController::drawParticles(){
//	for(int i = 0; i < particleRenderers.size(); i++){
//		particleRenderers[i]->draw();
//	}
	particleRenderer1->draw();
	particleRenderer2->draw();
}

void TrackController::update(){

	for(int i  = 0; i < timelines.size(); i++){
		if(timelines[i]->getIsPlaying()){
//
//			particleRenderers[i]->perlinForce->amplitude = timelines[i]->getValue("perlin amp");
//			particleRenderers[i]->gravityForce->gravity = timelines[i]->getValue("gravity amp");
//			particleRenderers[i]->primaryColor   = timelines[i]->getColor("primary color");
//			particleRenderers[i]->secondaryColor = timelines[i]->getColor("secondary color");
//			particleRenderers[i]->birthRate = timelines[i]->getValue("birth rate");
//			particleRenderers[i]->lifeSpan = timelines[i]->getValue("life span");
//			

			particleRenderer1->perlinForce->amplitude = timelines[i]->getValue("perlin amp");
			particleRenderer1->gravityForce->gravity = timelines[i]->getValue("gravity amp");
			particleRenderer1->primaryColor   = timelines[i]->getColor("primary color");
			particleRenderer1->secondaryColor = timelines[i]->getColor("secondary color");
			particleRenderer1->birthRate = timelines[i]->getValue("birth rate");
			particleRenderer1->lifeSpan = timelines[i]->getValue("life span");

			particleRenderer2->perlinForce->amplitude = timelines[i]->getValue("perlin amp");
			particleRenderer2->gravityForce->gravity = timelines[i]->getValue("gravity amp");
			particleRenderer2->primaryColor   = timelines[i]->getColor("primary color");
			particleRenderer2->secondaryColor = timelines[i]->getColor("secondary color");
			particleRenderer2->birthRate = timelines[i]->getValue("birth rate");
			particleRenderer2->lifeSpan = timelines[i]->getValue("life span");
		}
	}
	
	particleRenderer1->update();
	particleRenderer2->update();

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

