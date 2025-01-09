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
        if (checkLastInteraction())
        {
            this->last_interaction = 0.0f;
            incPasswordInput();
            std::cout << "Incrementing password input... " << password_input << std::endl;
        }
    }
    if (object_id == "CHECK_CUBE")
    {
        if (checkLastInteraction())
        {
            checkPassword(1);
        }
    }
    if (object_id == "CHECK_CUBE2")
    {
        if (checkLastInteraction())
        {
            checkPassword(2);
        }
    }

    if (object_id == "TOILET")
    {
        if (checkLastInteraction())
        {
            toiletInteraction();
        }
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

void GameLogic::checkPassword(int puzzle_number)
{   
    if (puzzle_number == 1)
    {
        if (this->first_password == this->password_input)
        {
            std::cout << "First Password correct!" << std::endl;
            this->is_first_password_ok = !is_first_password_ok;
        }
        else
        {
            std::cout << "Incorrect Password! Try again!" << std::endl;
        }
    }

    if (puzzle_number == 2)
    {
        if (this->second_password == this->password_input)
        {
            std::cout << "Second Password correct!" << std::endl;
            this->is_second_password_ok = !is_second_password_ok;
        }
        else
        {
            std::cout << "Incorrect Password! Try again!" << std::endl;
        }
    }

    std::cout << "Resetting password buffer..." << std::endl;
    this->password_input = 0;
}

bool GameLogic::isFirstPasswordCorrect()
{
    return this->is_first_password_ok;
}

bool GameLogic::isSecondPasswordCorrect()
{
    return this->is_second_password_ok;
}

void GameLogic::toiletInteraction()
{
    this->delete_everything = true;
}
bool GameLogic::shouldDeleteEverything()
{
    return this->delete_everything;
}

void GameLogic::resetDeleteEverything()
{
    this->delete_everything = false;
}

bool GameLogic::checkLastInteraction()
{
    bool is_ok = this->last_interaction > this->cooldown;
    if (is_ok)
    {
        this->last_interaction = 0.0f;
    }
    return is_ok;
}