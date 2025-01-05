#ifndef _GAME_LOGIC_H
#define _GAME_LOGIC_H


#include <iostream>
#include "gameObject.h"


class GameLogic
{
    private:
    //std::unordered_map<std::string, GameObject> clickable_objects;

    public:
    void checkInteraction(std::unordered_map<std::string, bool> clickable_objects_collision, bool current_state);
    void handleInteraction(std::string object_id);
};

#endif