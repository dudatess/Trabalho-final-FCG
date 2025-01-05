#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <GLFW/glfw3.h>
#include <iostream>

struct InputState {
    bool move_forward = false;
    bool move_backward = false;
    bool move_left = false;
    bool move_right = false;
    bool is_running = false;

    bool is_interacting = false;

    bool left_click = false;
    bool right_click = false;
    double mouse_x = 0.0;
    double mouse_y = 0.0;
};


class InputHandler {
private:
    InputState state;

public:
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
    static void cursorPosCallback(GLFWwindow* window, double x_pos, double y_pos);
    InputState getInputState();
};

#endif