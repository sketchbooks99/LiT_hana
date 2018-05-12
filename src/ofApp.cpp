#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
//    //  window
    ofSetWindowShape(1000, 700);
    ofSetVerticalSync(true);
    ofBackground(0, 0, 0);
    ofSetFrameRate(60);
//    //  setup ofxOpenNI
//    kinect.setup();
//    kinect.setRegister(true);
//    kinect.setMirror(true);
//    kinect.addDepthGenerator();
//    kinect.addImageGenerator();
    
//    // to match RGB and Depth
//    kinect.getDepthGenerator().GetAlternativeViewPointCap().SetViewPoint(kinect.getImageGenerator());
//    kinect.addUserGenerator(); // kinect can track man
    
//    //  start kinect
//    kinect.start();
    
    
    // setup box2d
    box2d.init();
    box2d.setFPS(60.0);
    box2d.setGravity(0, 1);
    box2d.createBounds(0, 0, ofGetWidth(), ofGetHeight());
    
    // right hand box2dCircle
    right_hand.setPhysics(0.0, 0.8, 0.0);
    right_hand.setup(box2d.getWorld(), 0, 0, 10);

    // init arm points
    arm_points[0] = ofVec2f(100, ofGetHeight()-100);
    arm_points[1] = ofVec2f(100, ofGetHeight()-120);
    arm_points[2] = ofVec2f(ofGetWidth()-100, ofGetHeight()-120);
    arm_points[3] = ofVec2f(ofGetWidth()-100, ofGetHeight()-100);

    // polygon setting
    polygon = std::make_shared<ofxBox2dPolygon>();
    polygon.get()->addVertex(arm_points[0]);
    polygon.get()->addVertex(arm_points[1]);
    polygon.get()->addVertex(arm_points[2]);
    polygon.get()->addVertex(arm_points[2]);
    polygon.get()->addVertex(arm_points[3]);
    polygon.get()->addVertex(arm_points[1]);
    polygon.get()->setPhysics(0.0, 0.8, 0.0);
    polygon.get()->create(box2d.getWorld());
    
    shader.load("shaders/shader.vert", "shaders/shader.frag");
    
}

//--------------------------------------------------------------
void ofApp::update(){
    //kinect.update();
    box2d.update();
    
    for(int i=0; i<flowers.size(); i++) {
        flowers[i].get()->update();
        
        // delete flower
        if(flowers[i].get()->dead_timer <= 0) {
            auto p = flowers.begin();
            flowers.erase(p+i);
        }
    }
    
    polygon.get()->clear();
    polygon.get()->addVertex(arm_points[0]);
    polygon.get()->addVertex(arm_points[1]);
    polygon.get()->addVertex(arm_points[2]);
    polygon.get()->addVertex(arm_points[2]);
    polygon.get()->addVertex(arm_points[3]);
    polygon.get()->addVertex(arm_points[1]);
    polygon.get()->create(box2d.getWorld());
}

//--------------------------------------------------------------
void ofApp::draw(){
    //kinect.drawDepth(0, 0, 640, 480);
    //kinect.drawSkeletons(0, 0, 640, 480);
    
//    if(kinect.getNumTrackedUsers() > 0) { // if any man tracked
//        ofxOpenNIUser user = kinect.getTrackedUser(0);
//
//        for(int i=0; i<user.getNumJoints(); i++) {
//            ofxOpenNIJoint joint = user.getJoint((enum Joint) i);
//
//            float x = joint.getProjectivePosition().x;
//            float y = joint.getProjectivePosition().y;
//
//            if(joint.isFound()) {
//
//                switch((enum Joint) i) { // draw ofxBox2dCircle only right hands
//                    case JOINT_RIGHT_HAND:
//                        ofSetColor(255, 0, 0, 127);
//                        right_hand.setPosition(x, y);
//                        right_hand.draw();
//                        break;
//                    default:
//                        break;
//                }
//            }
//        }
//
//        for(int i=0; i<user.getNumLimbs(); i++) {
//            ofxOpenNILimb limb = user.getLimb((enum Limb) i);
//
//
//
//            if(limb.isFound()) {
//                float sX = limb.getStartJoint().getProjectivePosition().x;
//                float sY = limb.getStartJoint().getProjectivePosition().y;
//                float eX = limb.getEndJoint().getProjectivePosition().x;
//                float eY = limb.getEndJoint().getProjectivePosition().y;
//
//                switch((enum Limb) i) {
//                        // Polygon or rect
//                        // 4 point need to arm
//                        // s+1, s-1, e+1, e-1 to rect or polygon
//                    case JOINT_LEFT_HAND:
//                        ofSetColor(0, 0, 255, 127);
//                        arm_points[0] = ofVec2f(sX+1, sY+1);
//                        arm_points[1] = ofVec2f(sX-1, sY-1);
//                        arm_points[2] = ofVec2f(eX+1, eY+1);
//                        arm_points[3] = ofVec2f(eX-1, eY-1);
//                        for(int i=0; i<4; i++) {
//                            right_arm[i].get()->setPosition(arm_points[i]);
//                        }
//                        break;
//                    default:
//                        break;
//                }
//            }
//        }
//    }
    
    ofSetColor(255, 255, 255);
    ofDrawBitmapString("flowers.size() = " + ofToString(flowers.size()), 10, 10);
    ofDrawBitmapString("fps = " + ofToString(ofGetFrameRate()), 10, 20);
    
    // draw flowers
    for(int i=0; i<flowers.size(); i++ ) {
        flowers[i].get()->draw();
    }
    
    ofSetColor(0, 255, 0);
    ofBeginShape();
    ofNoFill();
    vector<ofPoint> pos = polygon.get()->getPoints();
    for(int i=0; i<pos.size(); i++) {
        ofVertex(pos[i]);
    }
    ofEndShape();
    
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
    if(flowers.size() < 300) {
        diff.set(x-preMouse.x, y-preMouse.y);
        if(diff.x > 20 || diff.x < -20 || diff.y > 20 || diff.y < -20)
            throwFlower(diff, ofVec2f(x, y));
    }
    
    // previous mouse position
    preMouse = ofVec2f(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
    // delete 3 flowers
    for(int i=0; i<3; i++) {
        if(flowers.size() > 0) {
            auto p = flowers.begin(); // なぜerase(0)だとエラー吐くの？ vector<shared_ptr<Flower> > flowers
            flowers.erase(p);
        }
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    arm_points[0] = ofVec2f(x+1, y+1);
    arm_points[1] = ofVec2f(x-1, y-1);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    arm_points[2] = ofVec2f(x+1, y+1);
    arm_points[3] = ofVec2f(x-1, y-1);
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
    
    vec.normalize();
    float radian;
    radian = atan2(vec.x, vec.y);
    float max_rad = radian+PI/4;
    float min_rad = radian-PI/4;
    
    for(int i=0; i<3; i++) {
        // Flower型を自動で型推論している!!?? なんて便利なんだ...........
        auto flower = std::make_shared<Flower>(); // new Flower
        
        // box2d setting
        flower.get()->setPhysics(1.0, 0.1, 0.5);
        flower.get()->r = (float)ofRandom(5, 10);
        flower.get()->setup(box2d.getWorld(), pos.x, pos.y, 1.0);
        
        // flower setting
        float r_x = ofRandom(sin(min_rad), sin(max_rad));
        float r_y = ofRandom(cos(min_rad), cos(max_rad));
        ofVec2f speed = ofVec2f(r_x, r_y)*5;
        flower.get()->setVelocity(speed);
        flower.get()->shader = this->shader;
        
        // add flowers
        flowers.push_back(flower);
    }
}
