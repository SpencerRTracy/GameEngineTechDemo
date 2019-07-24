#pragma once

#include "Component.h"
#include<vector>
#include <string>
#include <unordered_map>

//Forward declaration
class GraphicsSystem;

/**
 * This class hold all components together.
 * It is the main driving force behind the entire system cooperating.
 * It manages all Components and coordinates them.
 */
class MainEngine{
public:
	/**
	 * This class hold all components together.
	 * It is the main driving force behind the entire system cooperating.
	 * It manages all Components and coordinates them.
	 */
	MainEngine();
	~MainEngine();

	/**
	 * Add a Component to the Engine
	 */
	void AddComponent(Component* newComp, std::string componentName);

	/**
	 * Initialize all components that have been added
	 */
	void InitializeComponents();

	/**
	 * Update all components
	 */
	void UpdateComponents();

	/**
	 * Retrieve a Component from it's name (string form).
	 * Uses a hash map for faster retrieval.
	 */
	Component* GetComponent(std::string name);

private:
	typedef std::unordered_map<std::string, Component*> ComponentMap;

	/**
	 * Hash map using the Component's Name (string) as the key.
	 */
	ComponentMap componentMap;

	/**
	 * Vector for holding all components.
	 * Used to iterate through on the update loop.
	 */
	std::vector<Component*> componentVec;
};

extern MainEngine* ENGINE;