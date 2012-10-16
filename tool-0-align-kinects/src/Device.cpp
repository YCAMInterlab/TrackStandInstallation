//
//  Device.cpp
//

#include "Device.h"

//---------
Device::Device() {
#ifdef USE_GRABSCENE
	this->pointSize.set("Point size", 2.0f, 1.0f, 10.0f);
	this->parameters.add(this->pointSize);
#endif
}
//---------
string Device::getFilename() const {
	return "../../../common-data/transform" + ofToString(this->index) + ".bin";
}

//---------
void Device::saveTransform() const {
	ofFile file(this->getFilename(), ofFile::WriteOnly, true);
	ofMatrix4x4 transform = this->getGlobalTransformMatrix();
	file.write( (char *) & transform, sizeof(ofMatrix4x4));
	file.close();
}

//---------
void Device::loadTransform() {
	const string filename = this->getFilename();
	if (!ofFile::doesFileExist(filename)) {
		ofLogError() << "Cannot load matrix as file " << filename << " does not exist";
		return;
	}
	
	ofMatrix4x4 transform;
	ofFile file(this->getFilename(), ofFile::ReadOnly, true);
	file.read( (char *) & transform, sizeof(ofMatrix4x4));
	file.close();
	this->setTransformMatrix(transform);
}
