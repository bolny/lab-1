#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "stdio.h"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

void errorCallback(int error, const char* description) {
	fprintf(stderr, "Error (%d): %s\n", error, description);
}

static void keyCallback(
	GLFWwindow *window, int key, int scancode, int action, int mods
) {
	printf("%d : %d\n", scancode, mods);
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

bool createInstance(VkInstance *instance) {
	// Init application info.
	VkApplicationInfo appInfo;
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Project 1";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	// Get extension info.
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	// Get validation layer info.
	uint32_t layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, NULL);
	printf("Available validation layers: %d\n", layerCount);
	VkLayerProperties layersInfo;
	vkEnumerateInstanceLayerProperties(&layerCount, &layersInfo);

	// Choose validation layers to enable.
	// Just enable all available validation layers for now. In reality we'd want
	// to match this against a hard coded list of layer names to enable and
	// filter. I can also set a validation message handling callback, but by
	// default it prints to stdout which is what I want at this stage.
	const char* enabledLayers = layersInfo.layerName;

	// Init instance creation info
	VkInstanceCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;
	createInfo.enabledLayerCount = layerCount;
	createInfo.ppEnabledLayerNames = &enabledLayers;

	// Create the Vulkan instance.
	if (vkCreateInstance(&createInfo, NULL, instance) != VK_SUCCESS) {
		return false;
	}
	return true;
}

bool pickPhysicalDevice(VkInstance *instance, VkPhysicalDevice *physicalDevice) {
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(*instance, &deviceCount, NULL);
	if (deviceCount == 0) {
		return false;
	}
	VkPhysicalDevice *devices = malloc(deviceCount * sizeof(VkPhysicalDevice));
	vkEnumeratePhysicalDevices(*instance, &deviceCount, devices);

	// Just choosing the first available GPU, since I know it is suitable on my
	// machine.
	physicalDevice = &devices[0];

	// Check device properties.
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(*physicalDevice, &deviceProperties);
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(*physicalDevice, &deviceFeatures);

	// In reality I'd have some algorithm to choose the ideal device, but since I
	// only have the one, it's fine.

	free(devices);
	return true;
}

int main() {
	printf("System initialized\n");

	// Test that GLFW is usable;
	if (!glfwInit()) {
		fprintf(stderr, "GLFW init failed\n");
		return -1;
	}
	printf("GLFW init success\n");

	// Test that Vulkan is usable.
	printf("Vulkan version: %d\n", VK_VERSION_1_0);
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);
	printf("%d extensions supported.\n", extensionCount);

	// Test that GLFW can use Vulkan.
	if (!glfwVulkanSupported()) {
		fprintf(stderr, "GLFW Vulkan support not found.\n");
		return -1;
	}
	printf("GLFW Vulkan support found.\n");

	// Create window.
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	GLFWwindow *window = glfwCreateWindow(
		WIDTH, HEIGHT, "Project 1", NULL, NULL
	);
	if (!window) {
		fprintf(stderr, "GLFW window failed\n");
		glfwTerminate();
		return -1;
	}
	printf("GLFW window succeeded init\n");

	// Set window event handling callbacks.
	glfwSetKeyCallback(window, keyCallback);
	glfwSetErrorCallback(errorCallback);

	// Create Vulkan instance.
	printf("Creating Vulkan instance.\n");
	VkInstance instance;
	if (!createInstance(&instance)) {
		fprintf(stderr, "Failed to create Vulkan instance.\n");
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

	// Select Vulkan GPU
	printf("Getting GPU.\n");
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	if (!pickPhysicalDevice(&instance, &physicalDevice)) {
		fprintf(stderr, "Did not find a suitable GPU!\n");
		vkDestroyInstance(instance, NULL);
		glfwDestroyWindow(window);
		glfwTerminate();
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
