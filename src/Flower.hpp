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
    float time; //時間
public:
    ofImage img; // 花の画像
    ofShader shader; // shader
    float r; // 花の半径
    bool isFlower;
    float alpha;
    
    Flower();
    void update(); // 状態の更新
    void draw(); // 描画
};
