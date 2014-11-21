#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //some path, may be absolute or relative to bin/data
    string imagePath = "images/";
    imageDir = ofDirectory(imagePath);
    //populate the directory object
    imageDir.listDir();
    
    if(imageDir.numFiles() < 2){
        ofLogError("ofApp::setup","Need at least 2 image files in bin/data/images to work!");
    }
    
    //go through and create a batch for each image
    for(int i = 0; i < imageDir.numFiles(); i++){
        //Using file index as batch id
        ofxLoaderBatch* batch = new ofxLoaderBatch(ofToString(i));
        
        // Giving the image file location, and then id'ing the image
        batch->addTexture(imageDir.getPath(i),"single-image");
        
        loaderSpool.addBatch(batch);
    }
    
    vector<string> batchIds = loaderSpool.getBatchIds();
    
    // Loading an image
    currBatchId = batchIds[0];
    nextBatchId = batchIds[1];
    loaderSpool.loadBatch(currBatchId);
}

//--------------------------------------------------------------
void ofApp::update(){
    if(loaderSpool.isBatchDrawable(nextBatchId)){
        loaderSpool.clearBatch(currBatchId);
        currBatchId = nextBatchId;
        nextBatchId = "";
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(loaderSpool.isBatchDrawable(currBatchId)){
        loaderSpool.getBatch(currBatchId)->getTexture("single-image")->draw(0,0, ofGetWidth(), ofGetHeight());
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' '){
        if(loaderSpool.isBatchDrawable(currBatchId)){
            vector<string> batchIds = loaderSpool.getBatchIds();
            nextBatchId = ofToString((ofToInt(currBatchId)+1)%batchIds.size());
            loaderSpool.loadBatch(nextBatchId);
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
