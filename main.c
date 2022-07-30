#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "stdio.h"

void error_callback(int error, const char* description) {
  fprintf(stderr, "Error (%d): %s\n", error, description);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  printf("%d : %d\n", scancode, mods);
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

int main() {
  if (!glfwInit()) {
    printf("GLFW init failed\n");
    return 1;
  }

  printf("GLFW init success\n");
  glfwSetErrorCallback(error_callback);

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow *window = glfwCreateWindow(640, 480, "Title", NULL, NULL);
  if (!window) {
    printf("GLFW window failed\n");
    glfwTerminate();
    return 1;
  }

  printf("GLFW window succeeded init\n");
  glfwSetKeyCallback(window, key_callback);

  VkSurfaceKHR surface;
  VkResult err = glfwCreateWindowSurface(instance, window, NULL, &surface);
  if (err) {
    printf("Vulkan surface creation failed.\n");
    glfwTerminate();
    return 1;
  }

  while (!glfwWindowShouldClose(window)) {
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  vkDestroySurfaceKHR(surface);
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
