
//
//  Kinect.cpp
//
#include "Kinect.h"

//---------
Kinect::Kinect(int index) {
	this->index = index;
	this->kinect.setRegistration(true);
	this->kinect.init();
	this->kinect.open(index);
	this->loadTransform();
	this->parameters.add(angle);
	
	angle.set("Angle", 0, -30, 30);
	angle.addListener(this, & Kinect::updateAngle);
	
	this->coloredMesh.getVertices().resize(kinect.getWidth()*kinect.getHeight() / (STEP*STEP));
	this->coloredMesh.getColors().resize(kinect.getWidth()*kinect.getHeight() / (STEP*STEP));
}

//---------
void Kinect::updatePointCloud() {
	int w = kinect.getWidth();
	int h = kinect.getHeight();
	this->coloredMesh.setMode(OF_PRIMITIVE_POINTS);
	ofFloatColor * color = coloredMesh.getColorsPointer();
	ofVec3f * position = coloredMesh.getVerticesPointer();
	
	for(int y = 0; y < h; y += STEP) {
		for(int x = 0; x < w; x += STEP) {
			if(kinect.getDistanceAt(x, y) > 0) {
				*color++ = kinect.getColorAt(x,y);
				*position++ = kinect.getWorldCoordinateAt(x, y);
			}
		}
	}
	
	whiteMesh.getVertices().assign(this->coloredMesh.getVertices().begin(), this->coloredMesh.getVertices().end());
}

//---------
void Kinect::update() {
	this->kinect.update();
	if (this->kinect.isFrameNew()) {
		this->updatePointCloud();
	}
}

//---------
void Kinect::draw() {
	
	this->getNode().transformGL();
	

	//colored point mesh
	glPointSize(this->pointSize);
	ofPushMatrix();
	// the projected points are 'upside down' and 'backwards'
	ofScale(1, -1, -1);
	ofScale(0.001, 0.001, 0.001);
	coloredMesh.drawVertices();
	ofPopMatrix();

	
	ofPushMatrix();
	ofScale(0.01, 0.01,0.01);
	origin.draw();
	ofPopMatrix();
	
	ofPushMatrix();
	ofScale(0.4f, 0.4f, 0.4f);
	ofTranslate(-1.0f, 1.0f);
	ofScale(2.0f/(float)kinect.getWidth(), -2.0f/(float)kinect.getHeight());
	ofTranslate(0.0f, 0.0f, -2.0f);
	this->getRgbTexture().draw(0.0f, 0.0f);
	ofPopMatrix();
	
	this->getNode().restoreTransformGL();
}

//---------
void Kinect::drawWhitePoints() {
	ofPushMatrix();
	ofPushStyle();
	
	this->getNode().transformGL();
	
	// the projected points are 'upside down' and 'backwards'
	ofScale(1, -1, -1);
	ofScale(0.001, 0.001, 0.001);
	
	ofPushStyle();
	glPointSize(1.0f);
	whiteMesh.drawVertices();
	ofPopMatrix();
	
	this->getNode().restoreTransformGL();
	ofPopStyle();
	ofPopMatrix();
}

//---------
void Kinect::close() {
	this->kinect.close();
}

//---------
ofTexture & Kinect::getDepthTexture() {
	return this->kinect.getDepthTextureReference();
}

//---------
ofTexture & Kinect::getRgbTexture() {
	return this->kinect.getTextureReference();
}

//---------
void Kinect::updateAngle(float & angle) {
	this->kinect.setCameraTiltAngle(angle);
}