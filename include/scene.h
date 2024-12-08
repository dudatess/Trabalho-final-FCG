#ifndef _SCENE_H
#define _SCENE_H

#include <iostream>
#include "gameObject.h"
#include <vector>
#include <algorithm>
#include <glad/glad.h>

class Scene
{
private:
    std::vector<GameObject *> game_objects;

public:
    Scene();
    ~Scene();

    void Update(float deltaTime);
    void Render();

    void AddGameObject(GameObject *game_object);
    void RemoveGameObject(GameObject *game_object);
};

#endif