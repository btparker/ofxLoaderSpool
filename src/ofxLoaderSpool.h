//
//  ofxLoaderSpool.h
//  ofxLoaderSpool
//
//  Created by  Tyler Parker on 11/14/14.
//
//

#ifndef __ofxLoaderSpool__
#define __ofxLoaderSpool__

#include "ofMain.h"
#include "ProgressiveTextureLoadQueue.h"
#include "ofxMSATimer.h"
#include "ofxLoaderBatch.h"


class ofxLoaderSpool{
    public:
        ofxLoaderSpool();
        ~ofxLoaderSpool();
    
        void update();
    
        ofxLoaderBatch* addBatch(string _batchId);
        ofxLoaderBatch* addBatch(ofxLoaderBatch batch);
    
        ofxLoaderBatch* getBatch(string _batchId);
        int getBatchesSize();
        vector<string> getBatchIds();
    
        void loadBatch(string _batchId);
    
        bool isBatchReady(string _batchId);
        bool isBatchDrawable(string _batchId);

        void clearBatch(string _batchId);
        void removeBatch(string _batchId);
    
    private:
        ProgressiveTextureLoadQueue * q;
        map<string, ofxLoaderBatch> batches;
};

#endif /* defined(__ofxLoaderSpool__) */
