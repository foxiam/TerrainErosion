#pragma once
#include <time.h>

class DiamondSquare
{
private:
	double** map;
	int size;
	int range = 173;

	void init();
	void average(int, int, int);
	void diamondStep(int);
	void squareStep(int);
	void fractal();
	int drand(int min, int max);

public:
	DiamondSquare(double** array, int s, int SEED = time(NULL));
	~DiamondSquare();

	void process();
};