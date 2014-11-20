//
//  ofxLoaderBatch.h
//
//  Created by  Tyler Parker on 11/17/14.
//
//

#ifndef __ofxLoaderBatch__
#define __ofxLoaderBatch__

#include "ofMain.h"
class ofxLoaderBatch{
public:
    ofxLoaderBatch(string id);
    ~ofxLoaderBatch();
    void addImageFilenames(vector<string> _imageFilenames);
    void addBatch(string id);
    ofxLoaderBatch* getBatch(string id);
    
    string id;
    
    vector<string> imageFilenames;
    map<string, ofxLoaderBatch*> batches;
};


#endif /* defined(__ofxLoaderBatch__) */
