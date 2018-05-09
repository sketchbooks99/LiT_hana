//
//  Flower.cpp
//  LiT_Flower_Last
//
//  Created by 木内舜司 on 2018/04/28.
//

#include "Flower.hpp"

Flower::Flower() {
    img.load("flower.png");
}

void Flower::update() {
    time += 0.01;
}

void Flower::draw() {
    if(time > 2) {
        float size = this->getRadius()*4;
        ofPushMatrix();
        ofTranslate(getPosition().x, getPosition().y, 0);
        ofRotateX(60);
        img.draw(0, 0, size, size);
        ofPopMatrix();
    } else {
        ofPushMatrix();
        ofTranslate(getPosition().x, getPosition().y, 0);
        //ofRotate(getRotation(), 0, 0, 1);
        ofDrawCircle(0, 0, this->getRadius());
        
        ofPopMatrix();
    }
}




