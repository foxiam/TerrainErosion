#include "World.h"

#include "SFML/include/SFML/Graphics.hpp"

World::World(int _seed) : SEED(_seed) 
{
    heightmap = new double* [dim.x];
    for (int i = 0; i < dim.x; ++i)
    {
        heightmap[i] = new double[dim.x];
    }
}

World::~World() 
{
    for (int i = 0; i < dim.x; ++i)
        delete[] heightmap[i];
    delete[] heightmap;
}

double** gen_table(int size)
{
    double** map = new double* [size];
    for (int i = 0; i < size; i++)
    {
        map[i] = new double[size];
        for (int j = 0; j < size; j++)
            map[i][j] = 0.0;
    }
    return map;
}

void World::generate() 
{
    double min = 256.0; 
    double max = -256.0;

    std::cout << "Size : " << dim.x + 1 << std::endl;

    double** map = gen_table(dim.x + 1);

    DiamondSquare ds(map, dim.x + 1);
    ds.process();

    for (int i = 0; i < dim.x; i++) 
    {
        for (int j = 0; j < dim.y; j++) 
        {
            heightmap[i][j] = map[i][j];
            if (heightmap[i][j] > max) max = heightmap[i][j];
            if (heightmap[i][j] < min) min = heightmap[i][j];
        }
    }

    for (int i = 0; i < dim.x; i++) 
    {
        for (int j = 0; j < dim.y; j++) 
        {
            heightmap[i][j] = (heightmap[i][j] - min) / (max - min);
        }
    }
    
    updated = true;
}

glm::vec3 World::surfaceNormal(int i, int j) 
{
    glm::vec3 n;
    if (i < dim.x - 1) n = glm::vec3(0.5) * glm::normalize(glm::vec3(scale * (heightmap[i][j] - heightmap[i + 1][j]), 1.0, 0.0));
    else n = glm::vec3(0.15) * glm::normalize(glm::vec3(scale * (heightmap[i][j] - heightmap[i][j]), 1.0, 0.0));
    if (i > 0) n += glm::vec3(0.15) * glm::normalize(glm::vec3(scale * (heightmap[i - 1][j] - heightmap[i][j]), 1.0, 0.0));
    else n += glm::vec3(0.15) * glm::normalize(glm::vec3(scale * (heightmap[i][j] - heightmap[i][j]), 1.0, 0.0));
    if (j < dim.x - 1) n += glm::vec3(0.25) * glm::normalize(glm::vec3(0.0, 1.0, scale * (heightmap[i][j] - heightmap[i][j + 1])));
    else n += glm::vec3(0.15) * glm::normalize(glm::vec3(0.0, 1.0, scale * (heightmap[i][j] - heightmap[i][j])));
    if (j > 0) n += glm::vec3(0.15) * glm::normalize(glm::vec3(0.0, 1.0, scale * (heightmap[i][j - 1] - heightmap[i][j])));
    else n += glm::vec3(0.15) * glm::normalize(glm::vec3(0.0, 1.0, scale * (heightmap[i][j] - heightmap[i][j])));

    if (i < dim.x - 1 && j < dim.x - 1) n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale * (heightmap[i][j] - heightmap[i + 1][j + 1]) / sqrt(2), sqrt(2), scale * (heightmap[i][j] - heightmap[i + 1][j + 1]) / sqrt(2)));
    else if (i < dim.x - 1) n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale * (heightmap[i][j] - heightmap[i + 1][j]) / sqrt(2), sqrt(2), scale * (heightmap[i][j] - heightmap[i + 1][j]) / sqrt(2)));
    else if (j < dim.x - 1) n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale * (heightmap[i][j] - heightmap[i][j + 1]) / sqrt(2), sqrt(2), scale * (heightmap[i][j] - heightmap[i][j + 1]) / sqrt(2)));
    else n += glm::vec3(0.1) * glm::normalize(glm::vec3(0, sqrt(2), 0));
    if (i < dim.x - 1 && j > 0) n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale * (heightmap[i][j] - heightmap[i + 1][j - 1]) / sqrt(2), sqrt(2), scale * (heightmap[i][j] - heightmap[i + 1][j - 1]) / sqrt(2)));
    else if (j > 0) n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale * (heightmap[i][j] - heightmap[i][j - 1]) / sqrt(2), sqrt(2), scale * (heightmap[i][j] - heightmap[i][j - 1]) / sqrt(2)));
    else if (i < dim.x - 1) n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale * (heightmap[i][j] - heightmap[i + 1][j]) / sqrt(2), sqrt(2), scale * (heightmap[i][j] - heightmap[i + 1][j]) / sqrt(2)));
    else n += glm::vec3(0.1) * glm::normalize(glm::vec3(0, sqrt(2), 0));
    if (j < dim.x - 1 && i > 0) n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale * (heightmap[i][j] - heightmap[i - 1][j + 1]) / sqrt(2), sqrt(2), scale * (heightmap[i][j] - heightmap[i - 1][j + 1]) / sqrt(2)));
    else if (j < dim.x - 1) n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale * (heightmap[i][j] - heightmap[i][j + 1]) / sqrt(2), sqrt(2), scale * (heightmap[i][j] - heightmap[i][j + 1]) / sqrt(2)));
    else if (i > 0) n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale * (heightmap[i][j] - heightmap[i - 1][j]) / sqrt(2), sqrt(2), scale * (heightmap[i][j] - heightmap[i - 1][j]) / sqrt(2)));
    else n += glm::vec3(0.1) * glm::normalize(glm::vec3(0, sqrt(2), 0));
    if (i > 0 && j > 0) n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale * (heightmap[i][j] - heightmap[i - 1][j - 1]) / sqrt(2), sqrt(2), scale * (heightmap[i][j] - heightmap[i - 1][j - 1]) / sqrt(2)));
    else if (i > 0) n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale * (heightmap[i][j] - heightmap[i - 1][j]) / sqrt(2), sqrt(2), scale * (heightmap[i][j] - heightmap[i - 1][j]) / sqrt(2)));
    else if (j > 0) n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale * (heightmap[i][j] - heightmap[i][j - 1]) / sqrt(2), sqrt(2), scale * (heightmap[i][j] - heightmap[i][j - 1]) / sqrt(2)));
    else n += glm::vec3(0.1) * glm::normalize(glm::vec3(0, sqrt(2), 0));

    return n;
}

void World::erode(int cycles)
{
    for (int i = 0; i < cycles; i++)
    {
        glm::vec2 newpos = glm::vec2(rand() % (int)dim.x, rand() % (int)dim.y);
        Particle drop(newpos);

        while (drop.volume > minVol)
        {

            glm::ivec2 ipos = drop.pos;
            glm::vec3 n = surfaceNormal(ipos.x, ipos.y);

            drop.speed += dt * glm::vec2(n.x, n.z) / (drop.volume * density);
            drop.pos += dt * drop.speed;
            drop.speed *= (1.0 - dt * friction);

            if (!glm::all(glm::greaterThanEqual(drop.pos, glm::vec2(0))) ||
                !glm::all(glm::lessThan(drop.pos, dim))) break;

            float maxsediment = drop.volume * glm::length(drop.speed) * (heightmap[ipos.x][ipos.y] - heightmap[(int)drop.pos.x][(int)drop.pos.y]);
            if (maxsediment < 0.0) maxsediment = 0.0;
            float sdiff = maxsediment - drop.sediment;

            drop.sediment += dt * depositionRate * sdiff;
            heightmap[ipos.x][ipos.y] -= dt * drop.volume * depositionRate * sdiff;

            drop.volume *= (1.0 - dt * evapRate);
        }
    }
}