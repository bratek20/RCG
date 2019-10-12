#include "LightSampler.h"
#include "Random.h"
#include <iostream>

using namespace std;

LightSampler::LightSampler(const vector<TrianglePtr>& triangles, const Config& c){
    for(auto& tri : triangles){
        if(tri->mat.isLightSource()){
            lightSources.push_back(tri);
            thresholds.push_back(tri->calcArea() * tri->mat.emissive.length());
        }
    }
    for(auto& lc : c.lights){
        lightSources.push_back(&lc.triangle);
        thresholds.push_back(lc.triangle.calcArea());
    }
    for(unsigned i = 1; i < thresholds.size(); i++){
        thresholds[i] += thresholds[i-1];
    }
    cout << "Found " << lightSources.size() << " light sources" << endl;
}

LightSampler::SampleData LightSampler::sample(){
    float randomValue = Random::uniform(0, thresholds.back());
    int idx = lower_bound(thresholds.begin(), thresholds.end(), randomValue) - thresholds.begin();

    SampleData ans;
    ans.source = lightSources[idx];
    ans.point = Random::pointInTriangle(ans.source);
    ans.probability = idx == 0 ? thresholds[0] : thresholds[idx] - thresholds[idx-1];
    ans.probability /= thresholds.back();
    return ans;
}