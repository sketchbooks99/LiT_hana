//
//  Flower.hpp
//  LiT_Flower_Last
//
//  Created by 木内舜司 on 2018/04/28.
//

#pragma once
#define USE_PROGRAMMABLE_GL 1

#include "ofMain.h"
#include "ofxBox2d.h"

class Flower : public ofxBox2dCircle{
private:
    float time; // time
    bool Bloom, Dead;
public:
    typedef struct small_flower {
        ofVec2f p; // position of small flower
        ofVec2f v; // velocity of small flower
    }Small;
    
    ofImage img; // flower image
    ofImage small_img; // small flower image
    Small small[5];
    
    ofShader shader; // shader
    float r; // flower radius
    float alpha; // to use shader
    float dead_timer; // small flower dead timer
    
    Flower();
    void update();
    void draw();
    void draw_small_Flower(); // after delete flower
    
    // to see from ofApp, not to set from ofApp
    bool isDead() { return Dead; }
    bool isFlower() { return Bloom; }
};
