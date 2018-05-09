#pragma once

#include "ofMain.h"
#include "ofxOpenNI.h"
#include "Flower.hpp"
#include "ofxBox2d.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
private:
    ofxOpenNI kinect;

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    void throwFlower(ofVec2f vec, ofVec2f pos);
    
    //vector<Flower> flowers;
    vector<ofVec2f> forces;
    
    ofVec2f preMouse;
    ofVec2f diff;
    
    ofxBox2d box2d;
    vector<shared_ptr<Flower> > flowers;
		
};
