#pragma comment(lib, "Opengl32.lib")

#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cstdlib> // for std::rand and std::srand
#include <ctime>   // for std::time

const float GRAVITY = -0.001f;
const float FLOOR_Y = -1.0f; // 바닥 y 좌표
const float MOVE_SPEED = 0.01f; // 큐브 이동 속도

struct Cube {
    float x, y, z;
    float size;
    float velocityY;
    float r, g, b;
};

std::vector<Cube> cubes;

void errorCallback(int error, const char* description)
{
    std::cerr << "GLFW Error: " << description << std::endl;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    // 큐브 이동
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        Cube& cube = cubes[0]; // 첫 번째 큐브를 선택하여 이동
        switch (key)
        {
        case GLFW_KEY_UP:
            cube.y += MOVE_SPEED;
            break;
        case GLFW_KEY_DOWN:
            cube.y -= MOVE_SPEED;
            break;
        case GLFW_KEY_LEFT:
            cube.x -= MOVE_SPEED;
            break;
        case GLFW_KEY_RIGHT:
            cube.x += MOVE_SPEED;
            break;
        default:
            break;
        }
    }
}

void initCubes()
{
    std::srand(static_cast<unsigned>(std::time(0))); // Initialize random seed
    for (int i = 0; i < 2; ++i)
    {
        Cube cube;
        cube.x = (std::rand() % 200 - 100) / 100.0f; // Random x position between -1 and 1
        cube.y = (std::rand() % 200 - 100) / 100.0f; // Random y position between -1 and 1
        cube.z = (std::rand() % 200 - 100) / 100.0f; // Random z position between -1 and 1
        cube.size = 0.2f; // Size of the cube
        cube.velocityY = 0.0f; // Initial vertical velocity
        cube.r = static_cast<float>(std::rand() % 100) / 100.0f;
        cube.g = static_cast<float>(std::rand() % 100) / 100.0f;
        cube.b = static_cast<float>(std::rand() % 100) / 100.0f;
        cubes.push_back(cube);
    }
}

void updateCubes()
{
    for (auto& cube : cubes)
    {
        cube.velocityY += GRAVITY;
        cube.y += cube.velocityY;

        // 바닥에 닿으면 멈추게 한다
        if (cube.y - cube.size / 2 < FLOOR_Y)
        {
            cube.y = FLOOR_Y + cube.size / 2;
            cube.velocityY = 0.0f;
        }
    }
}

void drawCube(const Cube& cube)
{
    glColor3f(cube.r, cube.g, cube.b);
    glPushMatrix();
    glTranslatef(cube.x, cube.y, cube.z);
    glBegin(GL_QUADS);

    // Front face
    glVertex3f(-cube.size / 2, -cube.size / 2, cube.size / 2);
    glVertex3f(cube.size / 2, -cube.size / 2, cube.size / 2);
    glVertex3f(cube.size / 2, cube.size / 2, cube.size / 2);
    glVertex3f(-cube.size / 2, cube.size / 2, cube.size / 2);

    // Back face
    glVertex3f(-cube.size / 2, -cube.size / 2, -cube.size / 2);
    glVertex3f(cube.size / 2, -cube.size / 2, -cube.size / 2);
    glVertex3f(cube.size / 2, cube.size / 2, -cube.size / 2);
    glVertex3f(-cube.size / 2, cube.size / 2, -cube.size / 2);

    // Left face
    glVertex3f(-cube.size / 2, -cube.size / 2, -cube.size / 2);
    glVertex3f(-cube.size / 2, -cube.size / 2, cube.size / 2);
    glVertex3f(-cube.size / 2, cube.size / 2, cube.size / 2);
    glVertex3f(-cube.size / 2, cube.size / 2, -cube.size / 2);

    // Right face
    glVertex3f(cube.size / 2, -cube.size / 2, -cube.size / 2);
    glVertex3f(cube.size / 2, -cube.size / 2, cube.size / 2);
    glVertex3f(cube.size / 2, cube.size / 2, cube.size / 2);
    glVertex3f(cube.size / 2, cube.size / 2, -cube.size / 2);

    // Top face
    glVertex3f(-cube.size / 2, cube.size / 2, -cube.size / 2);
    glVertex3f(-cube.size / 2, cube.size / 2, cube.size / 2);
    glVertex3f(cube.size / 2, cube.size / 2, cube.size / 2);
    glVertex3f(cube.size / 2, cube.size / 2, -cube.size / 2);

    // Bottom face
    glVertex3f(-cube.size / 2, -cube.size / 2, -cube.size / 2);
    glVertex3f(-cube.size / 2, -cube.size / 2, cube.size / 2);
    glVertex3f(cube.size / 2, -cube.size / 2, cube.size / 2);
    glVertex3f(cube.size / 2, -cube.size / 2, -cube.size / 2);

    glEnd();
    glPopMatrix();
}

void render()
{
    for (const auto& cube : cubes)
    {
        drawCube(cube);
    }
}

int main(void)
{
    if (!glfwInit())
        return -1;

    GLFWwindow* window;
    window = glfwCreateWindow(800, 600, "Cube Gravity Simulation", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetErrorCallback(errorCallback);
    glfwSetKeyCallback(window, keyCallback);

    glEnable(GL_DEPTH_TEST);
    initCubes();

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        updateCubes();
        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
