#include "stdafx.h"
#include "scene.h"

namespace overdrive {
	Scene::Scene():
		System("Scene") 
	{
	}

	void Scene::initialize() {
		System::initialize();
	}

	void Scene::update() {
	}

	void Scene::shutdown() {
		System::shutdown();
	}
}
