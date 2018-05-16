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
    
    time = 0.0;
    alpha = 0.0;
    Bloom = false;
    Dead = false;
}

//--------------------------------------------------------------
void Flower::update() {
    
    time += 0.01;
    if(time > 2.00) {
        Bloom = true;
        alpha += 0.02;
        this->setRadius(r);
    }
    
    // break flower and make five small_flower
    if(this->getPosition().y < 100 && !isDead() && isFlower()) {
        Dead = true;
        small_img.load("small flower.png");
        for(int i=0; i<5; i++) {
            float x = this->getPosition().x;
            float y = this->getPosition().y;
            small[i].v.set(ofRandom(-0.5, 0.5), ofRandom(0.8, 1.2));
            small[i].p.set(x, y);
        }
        dead_timer = 2.00;
    }
    
}

//--------------------------------------------------------------
void Flower::draw() {
    if(!isFlower()) {
        
        // draw circle
        
        ofPushMatrix();
        
        ofTranslate(getPosition().x, getPosition().y, 0);
        ofDrawCircle(0, 0, this->getRadius());
        
        ofPopMatrix();
        
    } else if(!isDead()){
        
        //draw flower
        
        ofPushMatrix();
        
        ofTranslate(getPosition().x, getPosition().y, 0);
        ofRotateX(60);
        
        // flower texture with shader
        shader.begin();
        shader.setUniform1f("time", time);
        shader.setUniform1f("alpha", alpha);
        shader.setUniformTexture("tex", img.getTexture(), 0);
        float size = this->getRadius()*5; 
        img.draw(0, 0, size, size);
        shader.end();
        
        ofPopMatrix();
        
    } else {
        draw_small_Flower();
    }
}

//--------------------------------------------------------------
void Flower::draw_small_Flower() {
    
    if(dead_timer > 0) {
        for(int i=0; i<5; i++) {
            ofPushMatrix();
            small[i].p += small[i].v;
            ofTranslate(small[i].p);
            ofRotateX(sin(dead_timer*5.0)*45);
            small_img.draw(0, 0, 20, 20);
            ofPopMatrix();
        }
        dead_timer -= 0.01;
    }
}







