#pragma once
#define USE_PROGRAMMABLE_GL 1

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
    
    ofVec2f prePos; // 直前の座標
    ofVec2f diff; // 座標の変化量
    ofShader shader; // Shader
    
    ofImage maskedImage;
    
    ofxBox2d box2d; // Box2d field
    ofxBox2dCircle right_hand, right_hiji; // right hands Joint
    ofxBox2dCircle right_arm[10];
    shared_ptr<ofxBox2dPolygon> polygon;
    ofVec2f right_hand_pos;
    vector<shared_ptr<Flower> > flowers; // FlowerクラスのVector
		
};
