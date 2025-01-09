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
    if (object_id == "PASSWORD_CUBE" || object_id == "PASSWORD_CUBE2")
    {
        //std::cout << "Interacting with password cube!!!" << std::endl;
        if (this->last_interaction > this->cooldown)
        {
            this->last_interaction = 0.0f;
            incPasswordInput();
            std::cout << "Incrementing password input... " << password_input << std::endl;
        }
    }
    if (object_id == "CHECK_CUBE" || object_id == "CHECK_CUBE2")
    {
        checkPassword();
    }


}

void GameLogic::updateLastInteraction(float delta_time)
{
    this->last_interaction += delta_time;
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
    if (this->first_password == this->password_input)
    {
        std::cout << "First Password correct!" << std::endl;
        this->is_first_password_ok = !is_first_password_ok;
        this->password_input = 0;
    }

    if (this->second_password == this->password_input)
    {
        std::cout << "Second Password correct!" << std::endl;
        this->is_second_password_ok = !is_second_password_ok;
        this->password_input = 0;
    }
}

bool GameLogic::isFirstPasswordCorrect()
{
    return this->is_first_password_ok;
}

bool GameLogic::isSecondPasswordCorrect()
{
    return this->is_second_password_ok;
}