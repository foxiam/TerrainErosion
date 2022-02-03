#include "DiamondSquare.h"
#include <stdlib.h>
#include <iomanip>
#include <iostream>
#include <Windows.h>
DiamondSquare::DiamondSquare(double** array, int s, int SEED)
{
	srand(SEED);
	map = array;
	size = s;
}

DiamondSquare::~DiamondSquare()
{
	for (int i = 0; i < size; i++)
	{
		delete map[i];
	}
	delete map;
}

void DiamondSquare::process()
{
	init();
	fractal();
}

void DiamondSquare::diamondStep(int sideLength)
{
	int halfSide = (sideLength + 1) / 2;

	for (int y = 0; y < size / sideLength; y++)
	{
		for (int x = 0; x < size / sideLength; x++)
		{
			int center_x = x * sideLength + halfSide;
			int center_y = y * sideLength + halfSide;

			int avg = (map[x * sideLength][y * sideLength] +
				map[x * sideLength][(y + 1) * sideLength] +
				map[(x + 1) * sideLength][y * sideLength] +
				map[(x + 1) * sideLength][(y + 1) * sideLength])
				/ 4.0f;

			map[center_x][center_y] = avg + drand(-range, range);
		}
	}
}

void DiamondSquare::average(int x, int y, int sideLength)
{
	float counter = 0;
	float accumulator = 0;

	int halfSide = sideLength / 2;

	if (x != 0)
	{
		counter += 1.0f;
		accumulator += map[y][x - halfSide];
	}
	if (y != 0)
	{
		counter += 1.0f;
		accumulator += map[y - halfSide][x];
	}
	if (x != size - 1)
	{
		counter += 1.0f;
		accumulator += map[y][x + halfSide];
	}
	if (y != size - 1)
	{
		counter += 1.0f;
		accumulator += map[y + halfSide][x];
	}

	map[y][x] = (accumulator / counter) + drand(-range, range);
}

void DiamondSquare::squareStep(int sideLength)
{
	int halfLength = (sideLength + 1) / 2;

	for (int y = 0; y < size / sideLength; y++)
	{
		for (int x = 0; x < size / sideLength; x++)
		{
			// Top
			average(x * sideLength + halfLength, y * sideLength, (sideLength + 1));
			// Right
			average((x + 1) * sideLength, y * sideLength + halfLength, (sideLength + 1));
			// Bottom
			average(x * sideLength + halfLength, (y + 1) * sideLength, (sideLength + 1));
			// Left
			average(x * sideLength, y * sideLength + halfLength, (sideLength + 1));
		}
	}
}
void DiamondSquare::fractal()
{
	int sideLength = size / 2;

	diamondStep(size - 1);
	squareStep(size - 1);
	range /= 2;

	while (sideLength >= 2)
	{
		diamondStep(sideLength);
		squareStep(sideLength);
		sideLength /= 2;
		range /= 2;
	}
}

int DiamondSquare::drand(int min = 0, int max = 255)
{
	return min + (rand() % static_cast<int>(max - min + 1));
}

void DiamondSquare::init()
{
	
	map[0][0] = drand();
	map[0][size - 1] = drand();
	map[size - 1][0] = drand();
	map[size - 1][size - 1] = drand();
}

