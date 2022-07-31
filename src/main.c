#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <vulkan/vulkan_core.h>

#include "stdio.h"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

void errorCallback(int error, const char* description) {
  fprintf(stderr, "Error (%d): %s\n", error, description);
}

static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  printf("%d : %d\n", scancode, mods);
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

bool createInstance(VkInstance *instance) {
  // Init application info
  VkApplicationInfo appInfo;
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Project 1";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  // Get extension info
  uint32_t glfwExtensionCount = 0;
  const char** glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  // Get validation layer info.
  uint32_t layerCount = 0;
  VkLayerProperties availableLayers;
  vkEnumerateInstanceLayerProperties(&layerCount, &availableLayers);

  // Init instance creation info
  VkInstanceCreateInfo createInfo;
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  createInfo.enabledExtensionCount = glfwExtensionCount;
  createInfo.ppEnabledExtensionNames = glfwExtensions;
  createInfo.enabledLayerCount = layerCount;
  createInfo.ppEnabledLayerNames = availableLayers.data();

  if (vkCreateInstance(&createInfo, NULL, instance) != VK_SUCCESS) {
    return false;
  }
  return true;
}

int main() {
  printf("System initialized\n");

  // Test that GLFW is usable;
  if (!glfwInit()) {
    printf("GLFW init failed\n");
    return -1;
  }
  printf("GLFW init success\n");


  // Test that GLM is usable.
  mat4 matrix = {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1},
  };
  vec4 vec = {
    1, 0, 0, 0
  };
  mat4 translated;
  glm_translate_to(matrix, vec, translated);
  printf("GLM operation suceeded.\n");

  // Test that Vulkan is usable.
  printf("Vulkan version: %d\n", VK_VERSION_1_0);
  uint32_t extensionCount = 0;
  vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);
  printf("%d extensions supported.\n", extensionCount);


  // Create window.
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow *window = glfwCreateWindow(
    WIDTH, HEIGHT, "Project 1", NULL, NULL
  );
  if (!window) {
    printf("GLFW window failed\n");
    glfwTerminate();
    return -1;
  }
  printf("GLFW window succeeded init\n");
  glfwSetKeyCallback(window, keyCallback);
  glfwSetErrorCallback(errorCallback);

  // Create Vulkan instance.
  printf("Creating Vulkan instance.\n");
  VkInstance instance;
  if (!createInstance(&instance)) {
    printf("Failed to create Vulkan instance.\n");
    return -1;
  }

  // Main loop.
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }

  // Clean up.
  vkDestroyInstance(instance, NULL);
  glfwDestroyWindow(window);
  printf("GLFW window destroyed\n");
  glfwTerminate();
  printf("Terminated\n");
  return 0;
}
