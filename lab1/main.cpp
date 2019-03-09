#include "Window.h"
#include "Input.h"
#include "Globals.h"
#include "Mesh.h"
#include "Scene.h"
#include "Color.h"
#include "Assets.h"

#include <iostream>
#include <algorithm>

using namespace std;

ScenePtr createScene(){
	auto scene = Scene::create();
	auto a1 = Actor::create(Mesh::create(Assets::CUBE));
	a1->move({0, 0, 3});
	scene->addChild(a1);
	return scene;
}	

int main(){
    if(!Window::open("Lab1")){
        return -1;
    }
	Input::init();
	Assets::init();
    Mesh::init();

	Globals::init();
	ScenePtr scene = createScene();

	while(!Window::shouldClose()){
		Input::handle();

		scene->update();

        Window::clear();
		scene->render();
		Window::swapBuffers();

		Globals::updateTime();
	}

    Mesh::clear();
	Assets::clear();
	Window::close();
	return 0;
}


