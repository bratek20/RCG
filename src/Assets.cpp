#include "Assets.h"
#include "Light.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>

using namespace std;

const vector<string> Assets::POSSIBLE_PATH_PREFIXES = 
{
    "",
    "../assets/"
};

Program3D Assets::program;

void Assets::init(){
    program = Program3D("Program3D.vs", "Program3D.fs");
}

void Assets::clear(){
}

string Assets::validPath(const string& path){
    for(auto& prefix : POSSIBLE_PATH_PREFIXES){
        string fullPath = prefix + path;
        if(isValidPath(fullPath)){
            return fullPath;
        }
    }

    cerr << "No valid path for: " << path << endl;
    return path;
}

string Assets::photoSavePath(const string& name){
    return "../photos/" + name;
}

bool Assets::isValidPath(const string& path){
    ifstream f(path.c_str());
    return f.good();
}

