
#include<ExGLFWwindow.h>

int main()
{
    GLFWGuard glfw_guard;
    ExGLFWwindow window;
    window.Init("GL Window");

    return window.Show();
}