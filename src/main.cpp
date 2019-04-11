#include "Window.h"
#include "Input.h"
#include "Globals.h"
#include "Scene.h"
#include "Color.h"
#include "Assets.h"
#include "Config.h"

#include <iostream>
#include <algorithm>

using namespace std;

int main(int argc, char* argv[]){
	Config c;
	if(argc < 2 || !c.load(argv[1])){
		cerr << "Bad config!" << endl;
		return -1;
	}
    if(!Window::open("Lab1", c)){
        return -1;
    }

	Globals::init();

	ScenePtr scene = Scene::create(c);
	
	if(c.debugMode){	
		Assets::init();
		Input::init();
		Input::onKeyPressed(GLFW_KEY_P, bind(&Scene::takePhoto, scene, std::ref(c)));
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
		scene->takePhoto(c);
	}
	
	
	Window::close();
	return 0;
}


