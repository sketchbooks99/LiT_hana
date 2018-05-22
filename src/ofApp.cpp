#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
//    //  window
    ofSetWindowShape(640, 480);
    ofSetVerticalSync(true);
    ofBackground(0, 0, 0);
    ofSetFrameRate(60);
    
    //  setup ofxOpenNI
    kinect.setup();
    kinect.setRegister(true);
    kinect.setMirror(true);
    kinect.addDepthGenerator();
    kinect.addImageGenerator();

    // to match RGB and Depth
    kinect.getDepthGenerator().GetAlternativeViewPointCap().SetViewPoint(kinect.getImageGenerator());
    kinect.addUserGenerator(); // kinect can track man

    //  start kinect
    kinect.start();
    
    
    // setup box2d
    box2d.init();
    box2d.setFPS(60.0);
    box2d.setGravity(0, 1);
    box2d.createBounds(0, 0, ofGetWidth(), ofGetHeight()-20);
    
    // right hand box2dCircle
    right_hand.setPhysics(0.0, 0.8, 0.0);
    right_hand.setup(box2d.getWorld(), 0, 0, 10);
    
    // right hiji box2dCircle
    right_hiji.setPhysics(0.0, 0.8, 0.0);
    right_hiji.setup(box2d.getWorld(), 0, 0, 10);
    
    //right arm
    for(int i=0; i<10; i++) {
        right_arm[i].setPhysics(0.0, 0.8, 0.0);
        right_arm[i].setup(box2d.getWorld(), 0, 0, 10);
    }

    shader.load("shaders/shader.vert", "shaders/shader.frag");
    
    maskedImage.allocate(640, 480, OF_IMAGE_COLOR_ALPHA);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.update();
    box2d.update();
    
    // cut out limited depth image
    ofPixels maskedImageData = maskedImage.getPixels();
    unsigned char *imageData = kinect.getImagePixels().getData();
    unsigned short *depthData = kinect.getDepthRawPixels().getData();
    for(int k=0; k<640*480; k++) {
        maskedImageData[k*4+0] = imageData[k*3+0];
        maskedImageData[k*4+1] = imageData[k*3+1];
        maskedImageData[k*4+2] = imageData[k*3+2];
        if(1500 <= depthData[k] && depthData[k] < 1800) {
            maskedImageData[k*4+3] = 255;
        } else {
            maskedImageData[k*4+3] = 0;
        }
    }
    maskedImage.setFromPixels(maskedImageData);
    
    for(int i=0; i<flowers.size(); i++) {
        flowers[i].get()->update();
        
        // delete flower
        if(flowers[i].get()->dead_timer <= 0) {
            auto p = flowers.begin();
            flowers.erase(p+i);
        }
    }
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    //kinect.drawDepth(0, 0, 640, 480);
    //kinect.drawSkeletons(0, 0, 640, 480);
    maskedImage.draw(0, 0, 640, 480);

    if(kinect.getNumTrackedUsers() > 0) { // if any man tracked
        ofxOpenNIUser user = kinect.getTrackedUser(0);

        for(int i=0; i<user.getNumJoints(); i++) {
            ofxOpenNIJoint joint = user.getJoint((enum Joint) i);

            float x = joint.getProjectivePosition().x;
            float y = joint.getProjectivePosition().y;

            if(joint.isFound()) {

                switch((enum Joint) i) { // draw ofxBox2dCircle only right hands
                    case JOINT_RIGHT_HAND:
                        ofFill();
                        right_hand_pos.set(x, y);
                        ofSetColor(255, 0, 0, 127);
                        right_hand.setPosition(right_hand_pos);
                        diff.set(right_hand_pos - prePos);
                        if(flowers.size() < 300) {
                            if(diff.x > 20 || diff.x < -20 || diff.y > 20 || diff.y < -20)
                                throwFlower(diff, right_hand_pos);
                        }
                        right_hand.draw();
                        prePos = right_hand_pos;
                        break;
                    case JOINT_LEFT_HAND:
                        if(y < 100) { // if left hands.y < 100, clear flowers
                            flowers.clear();
                        }
                        break;
                    default:
                        break;
                }
            }
        }

        for(int i=0; i<user.getNumLimbs(); i++) {
            ofxOpenNILimb limb = user.getLimb((enum Limb) i);

            if(limb.isFound()) {
                float sX = limb.getStartJoint().getProjectivePosition().x;
                float sY = limb.getStartJoint().getProjectivePosition().y;
                float eX = limb.getEndJoint().getProjectivePosition().x;
                float eY = limb.getEndJoint().getProjectivePosition().y;
                ofVec2f arm_vec = ofVec2f(sX-eX, sY-eY);
                arm_vec.normalize();
                float dist = ofDist(sX, sY, eX, sY)*0.1;

                switch((enum Limb) i) {
                    case LIMB_RIGHT_LOWER_ARM:
                        ofFill();
                        ofSetColor(0, 0, 255, 127);
                        for(int i=0; i<10; i++) { // ten box2dCircle among hands to hiji
                            right_arm[i].setPosition(right_hand_pos+arm_vec*dist*i);
                        }
                        right_hiji.setPosition(sX, sY);
                        right_hiji.draw();
                        break;
                    default:
                        break;
                }
            }
        }
    }
    
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
        diff.set(x-prePos.x, y-prePos.y);
        if(diff.x > 20 || diff.x < -20 || diff.y > 20 || diff.y < -20)
            throwFlower(diff, ofVec2f(x, y));
    }
    
    // previous mouse position
    prePos = ofVec2f(x, y);
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
        flower.get()->setup(box2d.getWorld(), pos.x, pos.y, 2.0);
        
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
