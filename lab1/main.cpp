#include "Window.h"
#include "Input.h"
#include "Globals.h"
#include "MyMesh.h"
#include "Scene.h"
#include "Color.h"
#include "Assets.h"

#include "Model.h"
#include "Shader.h"

#include <iostream>
#include <algorithm>

using namespace std;

ScenePtr createScene(){
	auto scene = Scene::create();
	auto a1 = Actor::create(MyMesh::create(Assets::CUBE));
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
    MyMesh::init();

	Globals::init();
	ScenePtr scene = createScene();

	Model ourModel("assets/nanosuit/nanosuit.obj");
	Program3D prog("Program3D.vs", "Program3D.fs");
	while(!Window::shouldClose()){
		Input::handle();

		scene->update();

        Window::clear();
		scene->render();
		ourModel.Draw(prog);
		Window::swapBuffers();

		Globals::updateTime();
	}

    MyMesh::clear();
	Assets::clear();
	Window::close();
	return 0;
}


