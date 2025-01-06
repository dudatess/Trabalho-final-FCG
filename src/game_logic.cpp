#include "game_logic.h"

GameLogic::GameLogic()
{

}

void GameLogic::checkInteraction(std::unordered_map<std::string, bool> clickable_objects_collision, bool is_interacting)
{
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
    if (object_id == "PASSWORD_CUBE")
    {
        //std::cout << "Interacting with password cube!!!" << std::endl;
        if (this->last_interaction > this->cooldown)
        {
            this->last_interaction = 0.0f;
            incPasswordInput();
            std::cout << "Incrementing password input... " << password_input << std::endl;
        }
    }
    if (object_id == "CHECK_CUBE")
    {
        checkPassword();
    }


}

void GameLogic::updateLastInteraction(float delta_time)
{
    this->last_interaction += delta_time;
}

void GameLogic::setPassword(int password)
{
    this->password = password;
}

void GameLogic::setPasswordInput(int password_input)
{
    this->password_input = password_input;
}

void GameLogic::incPasswordInput()
{
    this->password_input++;
}

void GameLogic::checkPassword()
{
    // std::cout << "Checking password... " << password_input << " " << password << std::endl;
    if (this->password == this->password_input)
    {
        std::cout << "Password correct!" << std::endl;
        this->is_password_ok = true;
        this->password_input = 0;
    }
}

bool GameLogic::isPasswordCorrect()
{
    return is_password_ok;
}