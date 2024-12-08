#include "scene.h"

Scene::~Scene()
{
    game_objects.clear();
}

void Scene::Update(float deltaTime)
{

    for (const auto &game_object : game_objects)
    {
        game_object->Update(deltaTime);
    }
}

void Scene::AddGameObject(GameObject *game_object)
{
    game_objects.push_back(game_object);
}

void Scene::Render()
{

    for (const auto &game_object : game_objects)
    {
        game_object->Render();
    }
}

void Scene::RemoveGameObject(GameObject *game_object)
{
    auto iter = std::find(game_objects.begin(), game_objects.end(), game_object);

    if (iter != game_objects.end())
        game_objects.erase(iter);
}