//
//  Flower.hpp
//  LiT_Flower_Last
//
//  Created by 木内舜司 on 2018/04/28.
//

#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"

class Flower : public ofxBox2dCircle{
private:
    float time; //時間
    ofImage img; //画像
public:
    
    Flower();
    void update(); //状態の更新
    virtual void draw(); //描画
};
