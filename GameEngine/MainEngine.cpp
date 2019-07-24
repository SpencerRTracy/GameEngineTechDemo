#include "MainEngine.h"
#include <Windows.h>

/**
 * Global Engine variable
 */
MainEngine* ENGINE;


/**
 * This class hold all components together.
 * It is the main driving force behind the entire system cooperating.
 * It manages all Components and coordinates them.
 */
MainEngine::MainEngine(){
	ENGINE = this;
}

MainEngine::~MainEngine(){
	for(unsigned int i=0; i < componentVec.size(); i++){
		delete componentVec[i];
	}
}

/**
 * Add a Component to the Engine
 */
void MainEngine::AddComponent(Component* newComp, std::string componentName){
	componentVec.push_back(newComp);
	componentMap.insert(std::make_pair(componentName, newComp));
}

/**
 * Initialize all components that have been added
 */
void MainEngine::InitializeComponents(){
	for(unsigned int i=0; i < componentVec.size(); i++){
		componentVec[i]->Initialize();
	}
}


/**
 * Update all components
 */
void MainEngine::UpdateComponents(){
	for(unsigned int i=0; i < componentVec.size(); i++){
		componentVec[i]->Update();
	}
}


/**
 * Retrieve a Component from it's name (string form)
 */
Component* MainEngine::GetComponent(std::string name){
	//Find the Component using a hash map.
	ComponentMap::iterator iter = componentMap.find(name);
	if(iter != componentMap.end()){
		return iter->second;
	} else{
		//Component not found in the engine.  It may not have been added.
		char msgbuf[256];
		sprintf(msgbuf, "%s component not found.  Is this component added to the engine?", name.c_str()); 
	}
}
