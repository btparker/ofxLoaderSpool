//
//  ofxLoaderSpool.cpp
//  ofxLoaderSpool
//
//  Created by  Tyler Parker on 11/14/14.
//
//

#include "ofxLoaderSpool.h"

ofxLoaderSpool::ofxLoaderSpool(){
    q = ProgressiveTextureLoadQueue::instance();
    
    q->setTexLodBias(0); //negative gives you lower mipmaps >> sharper
    q->setScanlinesPerLoop(128);
    q->setTargetTimePerFrame(10.0f);
    q->setNumberSimultaneousLoads(3);
    q->setVerbose(false);
}

ofxLoaderBatch* ofxLoaderSpool::addBatch(string _batchId){
    ofxLoaderBatch* batch = new ofxLoaderBatch(_batchId);
    return addBatch(batch);
}

void ofxLoaderSpool::update(){
    q->update();
}

ofxLoaderBatch* ofxLoaderSpool::addBatch(ofxLoaderBatch* batch){
    if(batches.count(batch->getId()) > 0){
        ofLogError("ofxLoaderSpool::addBatch", "Already a batch with id "+batch->getId());
        return NULL;
    }
    batch->setParentLoadQueue(q);
    batches[batch->getId()] = batch;
    return batches[batch->getId()];
}

void ofxLoaderSpool::clearBatch(string _batchId){
    if(batches.count(_batchId) == 0){
        ofLogError("ofxLoaderSpool::loadBatch", "No batch found with id "+_batchId);
        return;
    }
    batches[_batchId]->clear();
}

void ofxLoaderSpool::removeBatch(string _batchId){
    if(batches.count(_batchId) == 0){
        ofLogWarning("ofxLoaderSpool::removeBatch", "No batch found with id "+_batchId);
        return;
    }
    batches[_batchId]->clear();
    batches.erase(_batchId);
}

void ofxLoaderSpool::loadBatch(string _batchId){
    if(batches.count(_batchId) == 0){
        ofLogError("ofxLoaderSpool::loadBatch", "No batch found with id "+_batchId);
        return;
    }
    batches[_batchId]->load();
}

bool ofxLoaderSpool::isBatchReady(string _batchId){
    if(batches.count(_batchId) == 0){
        return false;
    }
    return batches[_batchId]->isReady();
}

bool ofxLoaderSpool::isBatchDrawable(string _batchId){
    if(batches.count(_batchId) == 0){
        return false;
    }
    return batches[_batchId]->isDrawable();
}

ofxLoaderBatch* ofxLoaderSpool::getBatch(string _batchId){
    if(batches.count(_batchId) == 0){
        ofLogWarning("ofxLoaderSpool::getBatch", "No batch found with id "+_batchId);
        return NULL;
    }
    return batches[_batchId];
}

int ofxLoaderSpool::getBatchesSize(){
    return batches.size();
}

vector<string> ofxLoaderSpool::getBatchIds(){
    vector<string> batchIds;
    for(map<string,ofxLoaderBatch*>::iterator iter = batches.begin(); iter != batches.end(); ++iter){
        batchIds.push_back(iter->first);
    }
    return batchIds;
}

ofxLoaderSpool::~ofxLoaderSpool(){
    for(map<string,ofxLoaderBatch*>::iterator iter = batches.begin(); iter != batches.end(); ++iter){
        iter->second->clear();
        delete iter->second;
    }
    batches.clear();
}