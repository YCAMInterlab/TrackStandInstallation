//
//  KinectSet.cpp
//

#include "KinectSet.h"

//--------
KinectSet::KinectSet() {
#ifdef USE_GRABSCENE
	height.set("Height", 5, 0, 10);
	width.set("Width", 4, 0, 10);
	depth.set("Depth", 8, 0, 10);
	resolution.set("Resolution", 128, 0, 1024);
	minArea.set("Minimum area", 3, 0, 128);
	maxArea.set("Maximum area", 128, 1, 256);
	threshold.set("Threshold", 50, 0, 255);
	persistence.set("Persistence", 15, 0, 256);
	maxDistance.set("Max position jump", 32, 0, 512);
	
	this->parameters.add(height);
	this->parameters.add(width);
	this->parameters.add(depth);
	this->parameters.add(resolution);
	this->parameters.add(minArea);
	this->parameters.add(maxArea);
	this->parameters.add(threshold);
	this->parameters.add(persistence);
	this->parameters.add(maxDistance);
	
	height.addListener(this, & KinectSet::updateRegion);
	width.addListener(this, & KinectSet::updateRegion);	
	depth.addListener(this, & KinectSet::updateRegion);
	resolution.addListener(this, & KinectSet::updateResolution);
	minArea.addListener(this, & KinectSet::updateTrackingParameters);
	maxArea.addListener(this, & KinectSet::updateTrackingParameters);
	threshold.addListener(this, & KinectSet::updateTrackingParameters);
#endif
	
	float dummy;
	int dummyInt;
	updateRegion(dummy);
	updateResolution(dummyInt);
	updateTrackingParameters(dummyInt);
	
	load();
}

//--------
void KinectSet::updateRegion(float &dummy) {
	region.makeIdentityMatrix();
	region.translate(-width/2.0f, -height/2.0f, depth/2.0f);
	region.scale(2.0f/width, 2.0f/height, 2.0f/depth);
	region.rotate(90, 1.0f, 0.0f, 0.0f);
}

//--------
void KinectSet::updateResolution(int &dummy) {
	if (resolution % 2 != 0)
		resolution = resolution + 1;
	this->fromAbove.allocate(resolution, resolution, GL_RGB);
}

//--------
void KinectSet::updateTrackingParameters(int &dummy) {
	contourFinder.setMinAreaRadius(this->minArea);
	contourFinder.setMaxAreaRadius(this->maxArea);
	contourFinder.setThreshold(this->threshold);
	tracker.setPersistence(this->persistence);
	tracker.setMaximumDistance(this->maxDistance);
}

//--------
void KinectSet::add(Device & device) {
	this->devices.push_back(&device);
	this->combinedPointsInWorldSpace.resize(this->combinedPointsInWorldSpace.size() + device.outputPointCount());
	this->pointsPerDevice.push_back(device.outputPointCount());
}

//--------
void KinectSet::update() {
	vector<Device*>::iterator it;
	for (it = this->devices.begin(); it != this->devices.end(); it++) {
		(**it).update();
	}
	updateTracking();
	updateWorldPoints();
}

//--------
void KinectSet::updateTracking() {
	
	this->fromAbove.begin();
	ofClear(0);
	
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(region.getPtr());
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(this->getGlobalTransformMatrix().getInverse().getPtr());
	
	ofPushStyle();
	glBlendFunc(GL_ONE, GL_ONE);
	ofColor(1);
	vector<Device*>::iterator it;
	for (it = this->devices.begin(); it != this->devices.end(); it++) {
		(**it).drawWhitePoints();
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ofPopStyle();
	
	this->fromAbove.end();
	
	fromAbove.readToPixels(pixelsCol);
	cv::cvtColor(ofxCv::toCv(pixelsCol), pixelsMono, CV_RGB2GRAY);
	contourFinder.findContours(pixelsMono);
	tracker.track(contourFinder.getBoundingRects());

}

//--------
void KinectSet::updateWorldPoints() {
	activePoints.clear();
	
	//copy from kinects
	ofVec3f *worldOut =  & this->combinedPointsInWorldSpace[0];
	int offset = 0;
	for (int i=0; i<this->devices.size(); i++) {
		Device & kinect = * this->devices[i];
		const vector<ofVec3f> world = kinect.getWorldPoints();
		memcpy(worldOut + offset * sizeof(ofVec3f), & world[0], world.size() * sizeof(ofVec3f));
		worldOut += world.size();
		offset += world.size();
	}
	
	//check if inside region
	worldOut =  & this->combinedPointsInWorldSpace[0];
#pragma omp parallel for
	for (int i=0; i<this->combinedPointsInWorldSpace.size(); i++) {
		const ofVec3f regionSpace = *worldOut * this->getNode().getGlobalTransformMatrix().getInverse() * this->region;
		
		if (regionSpace.x >= -1.0f && regionSpace.x <= 1.0f &&
			regionSpace.y >= -1.0f && regionSpace.y <= 1.0f &&
			regionSpace.z >= -1.0f && regionSpace.z <= 1.0f) {
#pragma omp atomic
			this->activePoints.push_back(worldOut);
		}
		worldOut++;
	}
}

//--------
void KinectSet::customDraw() {
	ofPushStyle();
	ofPushMatrix();
	glMultMatrixf(region.getInverse().getPtr());
	ofSetLineWidth(3.0f);
	ofNoFill();
	ofBox(2.0f);
	ofTranslate(-1.0f, -1.0f, -1.0f);
	ofScale(2.0f/(float)fromAbove.getWidth(), 2.0f/(float)fromAbove.getHeight());
	
	ofEnableAlphaBlending();
	ofSetColor(255, 255, 255, 150);
	fromAbove.draw(0.0f, 0.0f);
	
	ofTranslate(0.0f, 0.1f, 0.0f);
	
	vector<PersonCentroid>& followers = tracker.getFollowers();
	ofColor(255, 0, 0);
	ofSetLineWidth(10.0f);
	ofScale(1.0f, 1.0f, height);
	for(int i = 0; i < followers.size(); i++) {
		followers[i].draw();
	}
	
	ofPopMatrix();
	ofPopStyle();
}

//-----------
void KinectSet::load() {
	if (!ofFile::doesFileExist(this->getFilename()))
		return;
	
	ofxXmlSettings xml;

	
#ifdef USE_GRABSCENE
	xml.load(this->getFilename());
	xml.getValue(height.getName(), height);
	xml.getValue(width.getName(), width);
	xml.getValue(depth.getName(), depth);
	xml.getValue(resolution.getName(), resolution);
	xml.getValue(minArea.getName(), minArea);
	xml.getValue(maxArea.getName(), maxArea);
	xml.getValue(threshold.getName(), threshold);
	xml.getValue(persistence.getName(), persistence);
#else
	xml.loadFile(this->getFilename());
	xml.getValue("Height", height);
	xml.getValue("Width", width);
	xml.getValue("Depth", depth);
	xml.getValue("Resolution", resolution);
	xml.getValue("Minimum area", minArea);
	xml.getValue("Maximum area", maxArea);
	xml.getValue("Threshold", threshold);
	xml.getValue("Persistence", persistence);
	xml.getValue("Max position jump", maxDistance);
#endif
}

//-----------
void KinectSet::save() {
#ifdef USE_GRABSCENE
	ofxXmlSettings xml;
	
	xml.addValue(height.getName(), height);
	xml.addValue(width.getName(), width);
	xml.addValue(depth.getName(), depth);
	xml.addValue(resolution.getName(), resolution);
	xml.addValue(minArea.getName(), minArea);
	xml.addValue(maxArea.getName(), maxArea);
	xml.addValue(threshold.getName(), threshold);
	xml.addValue(persistence.getName(), persistence);
	
	xml.save(this->getFilename());
#endif
}

//--------
void KinectSet::close(){
	for (vector<Device*>::iterator it = this->devices.begin(); it != this->devices.end() ; it++) {
		(**it).close();
	}
}

//--------
vector<ofVec2f> KinectSet::getPeopleInScreenSpace() {
	vector<ofVec2f> result;
	vector<PersonCentroid> people = this->tracker.getFollowers();
	for (int i=0; i<people.size(); i++) {
		ofVec2f pos = people[i].getPosition();
		pos.x /= 5.20f * (1920.0f / 1200.0f);
		pos.y /= 5.20f;
		
		pos.y = 1200.0f - (pos.y * 1200.0f);
		pos.x = 1920.0f - (pos.x * 1920.0f);
		
		result.push_back(pos);
	}
	
	return result;
}

//--------
void KinectSet::drawWorldSpacePoints() {
	ofMesh mesh;
	
	//mesh.getVertices().assign(this->combinedPointsInWorldSpace.begin(), this->combinedPointsInWorldSpace.end());
	
	vector<ofVec3f*>::const_iterator it;
	for(it = this->activePoints.begin(); it != this->activePoints.end(); it++) {
		mesh.addVertex(**it);
	}
	
	ofPushStyle();
	ofColor(200, 100, 100);
	mesh.drawVertices();
	ofPopStyle();
}