
//
//  Kinect.cpp
//
#include "Kinect.h"

//---------
Kinect::Kinect(int index, bool registration) {
	this->index = index;
	this->kinect.setRegistration(registration);
	this->kinect.init(false, registration, registration);
	this->kinect.open(index);
	this->loadTransform();
#ifdef USE_GRABSCENE
	useRgb.set("Use RGB", false);
	this->parameters.add(angle);
	this->parameters.add(useRgb);
	angle.set("Angle", 0, -30, 30);
	angle.addListener(this, & Kinect::updateAngle);
#endif
	
	this->coloredMesh.getVertices().resize(kinect.getWidth()*kinect.getHeight() / (STEP*STEP));
	this->coloredMesh.getColors().resize(kinect.getWidth()*kinect.getHeight() / (STEP*STEP));
	this->whiteMesh.getVertices().resize(kinect.getWidth()*kinect.getHeight() / (STEP*STEP));
	
	_objectPointCount = (this->kinect.getWidth() / STEP) * (this->kinect.getHeight() / STEP);
}

//---------
void Kinect::updatePointCloud() {
	int w = kinect.getWidth();
	int h = kinect.getHeight();
	this->coloredMesh.setMode(OF_PRIMITIVE_POINTS);
	ofFloatColor * color = coloredMesh.getColorsPointer();
	ofVec3f * position = coloredMesh.getVerticesPointer();
	
	const bool useRgb = this->useRgb;
	//#pragma omp parallel for
	for(int y = 0; y < h; y += STEP) {
		for(int x = 0; x < w; x += STEP) {
			if(true || kinect.getDepthPixels()[x + y*w] > 0) {
#ifdef USE_GRABSCENE
				if (useRgb)
					*color++ = kinect.getColorAt(x,y);
				else
					*color++ = ofColor(100);
#else
				*color++ = ofColor(100);//kinect.getColorAt(x,y);
#endif
				*position++ = kinect.getWorldCoordinateAt(x, y);
			} else {
				*color++;
				*position++ = ofVec3f();
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
#ifdef USE_GRABSCENE
	this->getNode().transformGL();
#else
	this->transformGL();
#endif

	//colored point mesh
	glPointSize(pointSize);
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
	ofScale(1.0f, (float)kinect.getHeight() / (float)kinect.getWidth());
	ofTranslate(-1.0f, 1.0f);
	ofScale(2.0f/(float)kinect.getWidth(), -2.0f/(float)kinect.getHeight());
	ofTranslate(0.0f, 0.0f, -2.0f);
	this->getRgbTexture().draw(0.0f, 0.0f);
	ofPopMatrix();

#ifdef USE_GRABSCENE
	this->getNode().restoreTransformGL();
#else
	this->restoreTransformGL();
#endif
}

//---------
void Kinect::drawWhitePoints() {
	ofPushMatrix();
	ofPushStyle();
	
#ifdef USE_GRABSCENE
	this->getNode().transformGL();
#else
	this->transformGL();
#endif
	
	// the projected points are 'upside down' and 'backwards'
	ofScale(1, -1, -1);
	ofScale(0.001, 0.001, 0.001);

	glPointSize(1.0f);
	whiteMesh.drawVertices();
	ofPopMatrix();
	
#ifdef USE_GRABSCENE
	this->getNode().restoreTransformGL();
#else
	this->restoreTransformGL();
#endif
	
	ofPopStyle();
	ofPopMatrix();
}

//---------
void Kinect::close() {
	this->kinect.close();
}

//---------
int Kinect::outputPointCount() const {
	return _objectPointCount;
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
vector<ofVec3f> & Kinect::getObjectPoints() {
	return this->whiteMesh.getVertices();
}

//---------
vector<ofVec3f> Kinect::getWorldPoints() {
	this->getNode().transformGL();
	ofScale(1, -1, -1);
	ofScale(0.001, 0.001, 0.001);
	coloredMesh.drawVertices();
	this->getNode().restoreTransformGL();

	ofMatrix4x4 scale;
	scale.makeScaleMatrix(0.001, -0.001, -0.001);
	
	const int count = this->getObjectPoints().size();
	const ofVec3f * object = & this->getObjectPoints()[0];
	vector<ofVec3f> world(count);
	
	for (int i=0; i<count; i++) {
		world[i] = object[i] * scale * this->getNode().getGlobalTransformMatrix();
	}
	
	return world;
}

//---------
void Kinect::updateAngle(float & angle) {
	this->kinect.setCameraTiltAngle(angle);
}