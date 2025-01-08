#ifndef _GAME_LOGIC_H
#define _GAME_LOGIC_H


#include <iostream>
#include "gameObject.h"
class GameLogic
{
    private:
    //std::unordered_map<std::string, GameObject> clickable_objects;
    int first_password = 3;
    int second_password = 2;
    int password_input = 0;
    bool is_first_password_ok = false;
    bool is_second_password_ok = false;

    float cooldown = 0.5f;
    float last_interaction = 0.0f;

    public:
    GameLogic();
    void updateLastInteraction(float delta_time);
    void checkInteraction(std::unordered_map<std::string, bool> clickable_objects_collision, bool current_state);
    void handleInteraction(std::string object_id);

    void setPasswordInput(int password_input);
    void incPasswordInput();
    void checkPassword();

    bool isFirstPasswordCorrect();
    bool isSecondPasswordCorrect();
};

#endif