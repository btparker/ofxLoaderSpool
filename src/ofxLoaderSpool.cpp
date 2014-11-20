//
//  ofxLoaderSpool.cpp
//  ofxLoaderSpool
//
//  Created by  Tyler Parker on 11/14/14.
//
//

#include "ofxLoaderSpool.h"

ofxLoaderSpool::ofxLoaderSpool(){
    
}

void ofxLoaderSpool::setup(){
    q = ProgressiveTextureLoadQueue::instance();
    
    q->setTexLodBias(-0.5); //negative gives you lower mipmaps >> sharper
    q->setScanlinesPerLoop(64);
    q->setTargetTimePerFrame(10.0f);
    q->setNumberSimultaneousLoads(3);
    q->setVerbose(false);
};

void ofxLoaderSpool::update(){
}

ofxLoaderBatch* ofxLoaderSpool::addBatch(string id){
    ofxLoaderBatch* batch = new ofxLoaderBatch(id);
    return addBatch(batch);
}

ofxLoaderBatch* ofxLoaderSpool::addBatch(ofxLoaderBatch* batch){
    batch->setParentLoadQueue(q);
    return batches[batch->getId()] = batch;
}

void ofxLoaderSpool::clearBatch(string id){
    batches[id]->clear();
}

void ofxLoaderSpool::loadBatch(string id){
    batches[id]->load();
}

bool ofxLoaderSpool::isBatchReady(string id){
    return batches[id]->isReady();
}

bool ofxLoaderSpool::isBatchDrawable(string id){
    return batches[id]->isDrawable();
}

ofxLoaderBatch* ofxLoaderSpool::getBatch(string id){
    return batches[id];
}

int ofxLoaderSpool::getBatchesSize(){
    return batches.size();
}

ofxLoaderSpool::~ofxLoaderSpool(){
    for(map<string,ofxLoaderBatch*>::iterator iter = batches.begin(); iter != batches.end(); ++iter)
    {
        string id =  iter->first;
        batches[id]->clear();
        delete batches[id];
    }
}