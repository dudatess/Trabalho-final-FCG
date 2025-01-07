#ifndef _GAME_LOGIC_H
#define _GAME_LOGIC_H


#include <iostream>
#include "gameObject.h"
class GameLogic
{
    private:
    //std::unordered_map<std::string, GameObject> clickable_objects;
    int password = 3;
    int password_input = 0;
    bool is_password_ok = false;

    float cooldown = 0.5f;
    float last_interaction = 0.0f;

    public:
    GameLogic();
    void updateLastInteraction(float delta_time);
    void checkInteraction(std::unordered_map<std::string, bool> clickable_objects_collision, bool current_state);
    void handleInteraction(std::string object_id);

    void setPassword(int password);
    void setPasswordInput(int password_input);
    void incPasswordInput();
    void checkPassword();

    bool isPasswordCorrect();
};

#endif