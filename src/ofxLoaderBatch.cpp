//
//  ofxLoaderBatch.cpp
//
//  Created by  Tyler Parker on 11/17/14.
//
//

#include "ofxLoaderBatch.h"

ofxLoaderBatch::ofxLoaderBatch(string _id){
    id = _id;
}

void ofxLoaderBatch::addBatch(string id){
    ofxLoaderBatch* batch = new ofxLoaderBatch(id);
    batches[id] = batch;
}

ofxLoaderBatch* ofxLoaderBatch::getBatch(string id){
    return batches[id];
}


ofxLoaderBatch::~ofxLoaderBatch(){
    for(map<string,ofxLoaderBatch*>::iterator iter = batches.begin(); iter != batches.end(); ++iter)
    {
        string k =  iter->first;
        delete batches[k];
    }
}