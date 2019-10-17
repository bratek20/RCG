#include "Window.h"
#include "Input.h"
#include "Globals.h"
#include "Scene.h"
#include "Color.h"
#include "Assets.h"
#include "Config.h"

#include <gli/gli.hpp>
#include <iostream>
#include <algorithm>

#include "Random.h"

using namespace std;

ScenePtr scene;
Config c;

void printCameraPosition(){
	cout << "Camera pos: " << scene->getCamera()->getWorldPosition() << endl;
}

void takePhoto(){
	scene->takePhotoPathTracing(c);
}

void debugRay() {
	scene->debugRay(c);
}

int main(int argc, char* argv[]){
	if(argc < 2 || !c.load(argv[1])){
		cerr << "Bad config! Specify it as first argument." << endl;
		return -1;
	}
    if(!Window::open("Lab1", c)){
        return -1;
    }

	Globals::init(c);

	scene = Scene::create(c);
	
	if(c.debugMode){	
		Assets::init();
		Input::init();
		Input::onKeyPressed(GLFW_KEY_P, takePhoto);
		Input::onKeyPressed(GLFW_KEY_M, printCameraPosition);
		Input::onKeyPressed(GLFW_KEY_L, debugRay);
		while(!Window::shouldClose()){
			Input::handle();

			scene->update();

			Window::clear();
			scene->render();
			Window::swapBuffers();

			Globals::updateTime();
		}	
		Assets::clear();
	}
	else{
		takePhoto();
	}	
	
	Window::close();
	return 0;
}


