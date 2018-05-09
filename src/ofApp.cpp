#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
//    //  window
    ofSetWindowShape(640, 480);
    ofSetVerticalSync(true);
    ofBackground(0, 0, 0);
    ofSetFrameRate(30);
//    //  setup ofxOpenNI
//    kinect.setup();
//    kinect.setRegister(true);
//    kinect.setMirror(true);             //  flip horizontally
//    kinect.addDepthGenerator();         //  required for depth image
//    kinect.addImageGenerator();
//
//    // kinectのRGB画像と震度画像のズレを視点の適応で解消
//    kinect.getDepthGenerator().GetAlternativeViewPointCap().SetViewPoint(kinect.getImageGenerator());
//    kinect.addUserGenerator(); // kinectが人をトラッキングできるように
//    //  start kinect
//    kinect.start();
    
    
    // setup box2d
    box2d.init();
    box2d.setFPS(60.0);
    box2d.setGravity(0, 1);
    box2d.createBounds(0, 0, ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::update(){
    //kinect.update();
    box2d.update();
    
    // 摩擦や、反発などの計算を自分でやろうとしましたが、box2dに剛体の衝突の処理だけを任せるというのはあまりにも非効率だったので、摩擦計算などもbox2dに任せることにしました。なのでFlowerクラスではbox2dCircleを継承させて花の描画、時間の経過、shaderの処理のみを行うことにしました。
    for(int i=0; i<flowers.size(); i++) {
        flowers[i].get()->update();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    //kinect.drawDepth(0, 0, 640, 480);
    //kinect.drawSkeletons(0, 0, 640, 480);
    ofSetColor(255, 255, 255);
    for(int i=0; i<flowers.size(); i++) {
        flowers[i].get()->draw();
    }
    ofDrawBitmapString("flowers.sizs() = " + ofToString(flowers.size()), 10, 10);
    ofDrawBitmapString("fps = " + ofToString(ofGetFrameRate()), 10, 20);
    ofSetColor(255, 0, 0);
    ofDrawBitmapString(ofToString(diff.x), 10, 30);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    diff.set(x-preMouse.x, y-preMouse.y);
    if(diff.x > 20 || diff.x < -20 || diff.y > 20 || diff.y < -20)
        throwFlower(diff, ofVec2f(x, y));
    
    //前のマウスの位置を計算
    preMouse = ofVec2f(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::throwFlower(ofVec2f vec, ofVec2f pos) {
    //atan2の角度がうまくいってない...?
    
    vec.normalize();
    cout << atan2(vec.y, vec.x)*180/PI << endl;
    float radian = atan2(vec.y, vec.x);
    float max_rad = radian+PI/4;
    float min_rad = radian-PI/4;
    for(int i=0; i<3; i++) {
        auto flower = std::make_shared<Flower>();
        flower.get()->setPhysics(1.0, 0.1, 0.5);
        flower.get()->setup(box2d.getWorld(), pos.x, pos.y, ofRandom(5, 10));
        float r_x = ofRandom(sin(min_rad), sin(max_rad));
        float r_y = ofRandom(cos(min_rad), cos(max_rad));
        ofVec2f speed = ofVec2f(r_x, r_y)*5;
        flower.get()->setVelocity(speed);
        flowers.push_back(flower);
    }
}
