#pragma once
#include <chrono>
#include <vector>
#include "glm/glm.hpp"
#include <iostream>
#include "DiamondSquare.h"
class World {
public:
	void generate();
	void erode(int cycles);
	glm::vec3 surfaceNormal(int i, int j);
	World(int _seed);
	~World();

	int SEED = 10;
	std::chrono::milliseconds tickLength = std::chrono::milliseconds(1000);
	glm::vec2 dim = glm::vec2(512, 512);
	bool updated = false;

	double scale = 60.0;
	double** heightmap;

	//Erosion
	bool active = false;
	int remaining = 5;
	int erosionstep = 1;
	//Particle Properties
	float dt = 1.2;
	float density = 1.0;
	float evapRate = 0.001;
	float depositionRate = 0.1;
	float minVol = 0.01;
	float friction = 0.05;
};

struct Particle {
	Particle(glm::vec2 _pos) { pos = _pos; }

	glm::vec2 pos;
	glm::vec2 speed = glm::vec2(0.0);

	float volume = 1.0;
	float sediment = 0.0;
};

