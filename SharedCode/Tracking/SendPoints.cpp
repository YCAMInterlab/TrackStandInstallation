//
//  SendPoints.cpp
//

#include "SendPoints.h"

//--------
void SendPoints::setup() {
	this->tx.Create();
	
	this->tx.Connect("127.0.0.1", 4444);
	this->tx.SetNonBlocking(true);
}

//---------
void SendPoints::send(const vector<ofVec3f> & points) {

	int segmentSize = tx.GetMaxMsgSize() / sizeof(ofVec3f);
	for (int offset = 0; offset < segmentSize; offset+= segmentSize) {
		char * output = buffer;
		int length = 0;
		
		//count total
		*((int*)output) = points.size();
		output += sizeof(int);
		length += sizeof(int);
		
		//count here
		const int countHere = min((int)points.size() - offset, segmentSize);
		*((int*)output) = countHere;
		output += sizeof(int);
		length += sizeof(int);
		
		//offset here
		*((int*)output) = offset;
		output += sizeof(int);
		length += sizeof(int);
		
		memcpy(output, & points[offset], countHere * sizeof(ofVec3f));
		length += sizeof(ofVec3f) * countHere;
		
		tx.Send(buffer, length);
	}
}

//---------
void SendPoints::send(const vector<ofVec3f*> & points) {

	int segmentSize = 600;
	for (int offset = 0; offset < points.size(); offset+= segmentSize) {
		char * output = buffer;
		int length = 0;
		
		//count total
		*((int*)output) = points.size();
		output += sizeof(int);
		length += sizeof(int);
		
		//count here
		const int countHere = min((int)points.size() - offset, segmentSize);
		*((int*)output) = countHere;
		output += sizeof(int);
		length += sizeof(int);
		
		//offset here
		*((int*)output) = offset;
		output += sizeof(int);
		length += sizeof(int);
		
		for (int i=offset; i<countHere + offset; i++) {
			*(ofVec3f*)output = *points[i];
			output += sizeof(ofVec3f);
			length += sizeof(ofVec3f);
		}
		
		tx.Send(buffer, length);
	}
}