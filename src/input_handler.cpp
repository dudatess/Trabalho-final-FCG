#include "input_handler.h"
#include <GLFW/glfw3.h>
#include <iostream>

void InputHandler::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mod)
{
    InputHandler *input_handler = static_cast<InputHandler *>(glfwGetWindowUserPointer(window));

    // Se o usuário pressionar a tecla ESC, fechamos a janela.
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key == GLFW_KEY_W)
    {
        input_handler->state.move_forward = action;
    }
    if (key == GLFW_KEY_S)
    {
        input_handler->state.move_backward = action;
    }
    if (key == GLFW_KEY_A)
    {
        input_handler->state.move_left = action;
    }
    if (key == GLFW_KEY_D)
    {
        input_handler->state.move_right = action;
    }
    else if (key == GLFW_KEY_LEFT_SHIFT)
    {
        input_handler->state.is_running = action;
    }
}

void InputHandler::cursorPosCallback(GLFWwindow *window, double x_pos, double y_pos)
{
    InputHandler *input_handler = static_cast<InputHandler *>(glfwGetWindowUserPointer(window));
    input_handler->state.mouse_x = x_pos;
    input_handler->state.mouse_y = y_pos;
}

InputState InputHandler::getInputState()
{
    return this->state;
}