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
    
        ofxLoaderBatch* addBatch(string id);
        ofxLoaderBatch* addBatch(ofxLoaderBatch* batch);
    
        ofxLoaderBatch* getBatch(string id);
        int getBatchesSize();
    
        void loadBatch(string id);
    
        bool isBatchReady(string id);
        bool isBatchDrawable(string id);

        void clearBatch(string id);
    
    private:
        ProgressiveTextureLoadQueue * q;
        map<string, ofxLoaderBatch*> batches;
};

#endif /* defined(__ofxLoaderSpool__) */
