#include "game_logic.h"

void GameLogic::checkInteraction(std::unordered_map<std::string, bool> clickable_objects_collision, bool is_interacting)
{
    //std::cout << "is player interacting? " << is_interacting << std::endl;
    for (auto obj : clickable_objects_collision)
    {
        if (obj.second && is_interacting)
        {
            handleInteraction(obj.first);
        }
    }
}

void GameLogic::handleInteraction(std::string object_id)
{
    std::cout << "Interacting with object!!!" << std::endl;
}