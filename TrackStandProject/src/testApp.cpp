#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(0);
	ofEnableAlphaBlending();
	ofSetFrameRate(60);
	ofEnableSmoothing();
	ofToggleFullscreen();
	ofxTimeline::removeCocoaMenusFromGlut("TrackStand");

	trackController.setup(4);
	trackController.setWidth(ofGetScreenWidth()/2);
	
	useTestRecording = false;
	//set up test
	if(useTestRecording){
		recordingTest.setup();
		recordingTest.setShowTimeControls(false);
		string testSequencePath = "/Users/focus/Desktop/__RGBD_Bins/YCAM_SPIN/YCAM_Y_CAM1/TAKE_09_21_16_01_16/depth";
		depthSequence.loadSequence(testSequencePath);
		recordingTest.addTrack("depth sequence",&depthSequence);

//		particleRenderer.meshBuilder.setDepthPixels(depthSequence.getDepthImageSequence()->getPixels());
		recordingTest.setDurationInMillis(depthSequence.getDepthImageSequence()->getDurationInMillis());
		float widthPerSection = 640 / 4;
//		for(int i = 0; i < 4; i++){
//			trackController.particleRenderers[i]->minX = widthPerSection;
//			trackController.particleRenderers[i]->points = &points;
//		}
	}
	else{
		kinect[0] = new Kinect(0, false);
		kinect[1] = new Kinect(1, false);
		kinects.add(*kinect[0]);	
		kinects.add(*kinect[1]);
		
		trackController.particleRenderer1->points = &kinect[0]->getObjectPoints();
		trackController.particleRenderer2->points = &kinect[1]->getObjectPoints();
		trackController.particleRenderer1->kinect = kinect[0];
		trackController.particleRenderer2->kinect = kinect[1];
		
		float widthPerSection = kinects.getWidth() / 4;
//		for(int i = 0; i < 4; i++){
//			trackController.particleRenderers[i]->minX = widthPerSection;
//			trackController.particleRenderers[i]->points = &kinects.highFiveTagTeam();
//		}
		
	}
	cam.setup();
	cam.setFarClip(100);
	cam.setNearClip(.01);
	cam.autosavePosition = true;
	cam.loadCameraPosition();
	cam.speed *= .05;
}

//--------------------------------------------------------------
void testApp::update(){

	kinects.update();
	
	vector<ofVec2f> positions;
	positions.push_back( ofVec2f(mouseX, mouseY) );
	trackController.setPositions(positions);

	if(useTestRecording){
		if(depthSequence.isFrameNew()){
//			particleRenderer.meshBuilder.update();
		}
	}

	trackController.setPositions(this->kinects.getPeopleInScreenSpace());
	trackController.update();
	
}

//--------------------------------------------------------------
void testApp::draw(){
	trackController.draw();
	if(useTestRecording){
//		recordingTest.setOffset(trackController.drawRect.getBottomLeft());
//		recordingTest.draw();
	}
	
	previewRect = ofRectangle(trackController.drawRect.getTopRight(),
										  trackController.drawRect.width,
										  ofGetHeight());
	cam.begin(previewRect);
//	particleRenderer.draw();
	trackController.drawParticles();
	cam.end();
	
	ofDrawBitmapString(ofToString(ofGetFrameRate(),2), ofGetWidth() - 100, ofGetHeight()-40);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	if(key == 'b'){
		trackController.editMode = !trackController.editMode;
	}
	if(key == ' '){
		trackController.togglePlayForTrackAtPoint(ofVec2f(mouseX,mouseY));
	}
	if(key == 'p' && useTestRecording){
		recordingTest.togglePlay();
	}
	if(key == 'F'){
		trackController.toggleFooters();
	}
	if(key == 'f'){
		ofToggleFullscreen();
	}
	if(key == '1'){
		trackController.fitToScreenHeight();
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
	cam.applyRotation = cam.applyTranslation = previewRect.inside(mouseX, mouseY);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	trackController.setWidth(ofGetWidth()/2);
	if(useTestRecording){
		recordingTest.setWidth(ofGetWidth()/2);
	}

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void testApp::exit(){
	depthSequence.disable();
}