#include "LightSampler.h"
#include "Random.h"
#include <iostream>

using namespace std;

LightSampler::LightSampler(const vector<TrianglePtr>& triangles){
    for(auto& tri : triangles){
        if(tri->mat.isLightSource()){
            lightSources.push_back(tri);
            thresholds.push_back(tri->calcArea() * tri->mat.emissive.length());
        }
    }
    for(unsigned i = 1; i < thresholds.size(); i++){
        thresholds[i] += thresholds[i-1];
    }
    cout << "Found " << lightSources.size() << " light sources" << endl;
}

LightSampler::SampleData LightSampler::sample(){
    float randomValue = Random::uniform(0, thresholds.back());
    int idx = upper_bound(thresholds.begin(), thresholds.end(), randomValue) - thresholds.begin();

    SampleData ans;
    ans.source = lightSources[idx];
    ans.point = Random::pointInTriangle(ans.source);
    return ans;
}