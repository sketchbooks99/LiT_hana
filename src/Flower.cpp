//
//  Flower.cpp
//  LiT_Flower_Last
//
//  Created by 木内舜司 on 2018/04/28.
//

#include "Flower.hpp"

Flower::Flower() {
    ofDisableArbTex();
    img.load("flower.png");
    ofEnableArbTex();
    alpha = 0.0;
    isFlower = false;
}

void Flower::update() {
    
    time += 0.01;
    if(time > 2.00) {
        isFlower = true;
        alpha += 0.02;
        this->setRadius(r);
    }
    
    float x = this->getPosition().x;
    float y = this->getPosition().y;
    
}

void Flower::draw() {
    if(!isFlower) {
        
        // draw circle
        
        ofPushMatrix();
        
        ofTranslate(getPosition().x, getPosition().y, 0);
        ofDrawCircle(0, 0, this->getRadius());
        
        ofPopMatrix();
        
    } else {
        
        //draw flower
        
        ofPushMatrix();
        
        ofTranslate(getPosition().x, getPosition().y, 0);
        ofRotateX(60);
        
        // flower texture with shader
        shader.begin();
        shader.setUniform1f("alpha", alpha);
        shader.setUniformTexture("tex", img.getTexture(), 0);
        float size = this->getRadius()*5; //重なりを作るために半径を5倍する
        img.draw(0, 0, size, size);
        shader.end();
        
        ofPopMatrix();
        
    }
}




