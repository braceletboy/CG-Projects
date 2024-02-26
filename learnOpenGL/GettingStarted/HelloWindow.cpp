#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

/**
 * @brief The call back function for when the GLFW window size is changed
 * 
 * @param window The GLFW window that is getting resized
 * @param width The new width of the window
 * @param height The new height of the window
 */
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

/**
 * @brief Check if the escape key is pressed and close the window if true
 * 
 * @param window The window to be closed
 */
void checkEscapePressed(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

/**
 * @brief Main function
 * 
 * @return int Exit Status
 */
int main()
{
  glfwInit();  // initialize window

  // configure OpenGL version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);  // its OpenGL-4.x
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);  // its OpenGL-y.6
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // core profile

  // create window
  GLFWwindow* window = glfwCreateWindow(
    WINDOW_WIDTH, WINDOW_HEIGHT, "Learn OpenGL", NULL, NULL);
  if(window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);  // make the context of the window the
                                  // current context of the calling thread

  // test the OpenGL function loader
  if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return 1;
  }

  // specify the viewport rectangle inside the window
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

  // handle window resizing;
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // main display loop
  while(!glfwWindowShouldClose(window))
  {
    checkEscapePressed(window);  // close window using the escape key
    glClearColor(1, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);  // clear the screen
    glfwSwapBuffers(window);  // swap back and front buffer contents
    glfwPollEvents();  // listen for user input events
  }

  glfwTerminate();  // free all GLFW resources
}
