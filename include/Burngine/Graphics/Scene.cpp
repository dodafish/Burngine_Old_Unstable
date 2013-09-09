/*
 * Scene.cpp
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#include "Scene.h"

namespace burn {

Scene::Scene() {
	// TODO Auto-generated constructor stub

}

Scene::~Scene() {
	// TODO Auto-generated destructor stub
}

StaticMeshNode* Scene::createStaticMeshNode(){
	StaticMeshNode* n = new StaticMeshNode;
	_staticMeshes.push_back(n);
	return n;
}

} /* namespace burn */
