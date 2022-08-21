// Right now I'm largely following the tutorials at:
// https://antongerdelan.net/opengl/

// Don't let GLFW include anything. I'll handle everything myself.
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
// I generated an OpenGL loader profile from the Glad web service.
#include <glad/gl.h>

#include "stdio.h"
#include "string.h"

const int WIDTH = 640;
const int HEIGHT = 480;

void error_callback(int error, const char *description) {
  // Print out any errors with GLFW.
  fprintf(stderr, "Error (%d): %s\n", error, description);
}

static void key_callback(GLFWwindow *pWindow, int key, int scancode, int action,
                         int mods) {
  // Handle Input.
  printf("%d-%d(%d)\n", mods, key, scancode);
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(pWindow, GLFW_TRUE);
  }
}

int main() {
  printf("System initialized.\n");
  // Initialize GLFW.
  if (!glfwInit()) {
    printf("GLFW initialization failed.\n");
    return 0;
  }
  printf("GLFW initialized.\n");
  glfwSetErrorCallback(error_callback);

  // Create window.
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  // The next two lines are necessary for MacOS to access modern OpenGL
  // functions, and also because the program won't initialize with legacy
  // OpenGL because Apple forbids it (at least that's my take away).
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  GLFWwindow *pWindow =
      glfwCreateWindow(WIDTH, HEIGHT, "Project 1", NULL, NULL);
  if (!pWindow) {
    printf("GLFW window creation failed.\n");
    return 0;
  }
  glfwSetKeyCallback(pWindow, key_callback);
  printf("GLFW window creation succeeded.\n");

  // Create OpenGL context.
  glfwMakeContextCurrent(pWindow);
  gladLoadGL(glfwGetProcAddress);
  glfwSwapInterval(1);
  // Depth testing will set the renderer to only render pixels that are not
  // obscured by other pixels.
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // Set up triangle vertices.
  float triagleVertices[] = {
       0.0f,  0.5f,  0.0f,
       0.5f, -0.5f,  0.0f,
      -0.5f, -0.5f,  0.0f,
  };

  // Dump triangle vertices into a vertex buffer.
  GLuint vertexBuffer = 0;
  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), triagleVertices,
               GL_STATIC_DRAW);

  // Set up a vertex buffer array.
  GLuint vertexArray = 0;
  glGenVertexArrays(1, &vertexArray);
  glBindVertexArray(vertexArray);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  // gl positions are 4 dimensional (the last dimension is perspective). This
  // has to do with "Homogeneous Coordinates" (look that up on wikipedia for an
  // explanation). Which has something to do with projecting 3D objects onto a
  // 2D plane, but I sure as HELL did not learn about this in my linear algebra
  // class when I learned about projection. The 1.0 means "don't use
  // perspective".
  
  // Define the shader to apply to the vertices. This will convert the 3
  // dimensional coordinates that I use to define the triangle into the 4
  // dimensional coordinates needed to render the triangle.
  const char* vertexShaderSource = 
    "#version 400\n"
    "in vec3 vp;"
    "void main() {"
    "  gl_Position = vec4(vp, 1.0);"
    "}";
  const int vertexShaderSourceLength = strlen(vertexShaderSource);

  // Define the fragment shader (a fragment is a pixel that defines the surface
  // of the object). This will run on every pixel on the surface of the
  // triangle and set the color.
  const char* fragmentShaderSource =
    "#version 400\n"
    "out vec4 frag_color;"
    "void main() {"
    "  frag_color = vec4(0.5, 0.0, 0.5, 1.0);"
    "}";
  const int fragmentShaderSourceLength = strlen(fragmentShaderSource);
  
  // Compile the shaders.
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource,
                 &vertexShaderSourceLength);
  glCompileShader(vertexShader);
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource,
                 &fragmentShaderSourceLength);
  glCompileShader(fragmentShader);

  // Combine the shaders into a GPU executable and link.
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // Main loop.
  while (!glfwWindowShouldClose(pWindow)) {
    // Clear the buffer.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw the triangle.
    glUseProgram(shaderProgram);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Handle events.
    glfwPollEvents();
    // Swap the buffer onto the screen.
    glfwSwapBuffers(pWindow);
  }

  // Clean up.
  glfwDestroyWindow(pWindow);
  printf("GLFW window destroyed.\n");
  glfwTerminate();
  printf("GLFW terminated.\n");

  return 0;
}
