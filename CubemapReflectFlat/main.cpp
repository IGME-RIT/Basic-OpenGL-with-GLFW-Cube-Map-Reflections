/*
Title: Specular Maps
File Name: main.cpp
Copyright � 2016, 2019
Author: David Erbelding, Niko Procopi
Written under the supervision of David I. Schwartz, Ph.D., and
supported by a professional development seed grant from the B. Thomas
Golisano College of Computing & Information Sciences
(https://www.rit.edu/gccis) at the Rochester Institute of Technology.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include "FreeImage.h"
#include <vector>
#include "mesh.h"
#include "fpsController.h"
#include "transform3d.h"
#include "material.h"
#include "texture.h"
#include "cubeMap.h"
#include <iostream>

// chooses which skybox to draw
int skyboxIndex = 0;

// Store the current dimensions of the viewport.
glm::vec2 viewportDimensions = glm::vec2(800, 600);
glm::vec2 mousePosition = glm::vec2();


// Window resize callback
void resizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
    viewportDimensions = glm::vec2(width, height);
}

// This will get called when the mouse moves.
void mouseMoveCallback(GLFWwindow *window, GLdouble mouseX, GLdouble mouseY)
{
    mousePosition = glm::vec2(mouseX, mouseY);
}

void KeyDown(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == 1 || action == 2)
	{
		if (key == GLFW_KEY_LEFT)
		{
			skyboxIndex--;
			if (skyboxIndex < 0)
				skyboxIndex = 5;
		}

		if (key == GLFW_KEY_RIGHT)
		{
			skyboxIndex++;
			if (skyboxIndex > 5)
				skyboxIndex = 0;
		}
	}
}


int main(int argc, char **argv)
{
	// Initialize GLFW
	glfwInit();

	// cheap anti aliasing
	glfwWindowHint(GLFW_SAMPLES, 16);

	// Initialize window
	GLFWwindow* window = glfwCreateWindow(viewportDimensions.x, viewportDimensions.y, "Change Sky with Arrow Keys", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set window callbacks
	glfwSetFramebufferSizeCallback(window, resizeCallback);
    glfwSetCursorPosCallback(window, mouseMoveCallback);
	glfwSetKeyCallback(window, KeyDown);

	// Initialize glew
	glewInit();

    // The mesh loading code has changed slightly, we now have to do some extra math to take advantage of our normal maps.
    // Here we pass in true to calculate tangents.
    Mesh* model = new Mesh("../Assets/building.obj", true);
    Mesh* cube = new Mesh("../Assets/cube.obj", true);

    // The transform being used to draw our second shape.
    Transform3D transform;
    transform.SetPosition(glm::vec3(0, 0, -2));

    // Make a first person controller for the camera.
    FPSController controller = FPSController();

	// Create Shaders
    Shader* vertexShader = new Shader("../Assets/rockVertex.glsl", GL_VERTEX_SHADER);
    Shader* fragmentShader = new Shader("../Assets/rockFragment.glsl", GL_FRAGMENT_SHADER);

    // Create A Shader Program
    ShaderProgram* shaderProgram = new ShaderProgram();
    shaderProgram->AttachShader(vertexShader);
    shaderProgram->AttachShader(fragmentShader);

	// fields that are used in the shader, on the graphics card
	char cameraViewVS[] = "cameraView";
	char worldMatrixVS[] = "worldMatrix";
	char cameraPosFS[] = "cameraPosition";

	char cubeMapFS[] = "cubeMap";

    // Create a material using a texture for our model
    Material* rockMat = new Material(shaderProgram);

	std::vector<char*> GreenFaceFilePaths;
	char GreenSkyLeftFile[] =	"../Assets/GreenSky/left.png";
	char GreenSkyRightFile[] =	"../assets/GreenSky/right.png";
	char GreenSkyBottomFile[] =	"../Assets/GreenSky/bottom.png";
	char GreenSkyTopFile[] =	"../Assets/GreenSky/top.png";
	char GreenSkyBackFile[] =	"../Assets/GreenSky/back.png";
	char GreenSkyFrontFile[] =	"../Assets/GreenSky/front.png";
	GreenFaceFilePaths.push_back(GreenSkyLeftFile);
	GreenFaceFilePaths.push_back(GreenSkyRightFile);
	GreenFaceFilePaths.push_back(GreenSkyBottomFile);
	GreenFaceFilePaths.push_back(GreenSkyTopFile);
	GreenFaceFilePaths.push_back(GreenSkyBackFile);
	GreenFaceFilePaths.push_back(GreenSkyFrontFile);
	CubeMap* greenSky = new CubeMap(GreenFaceFilePaths);

	std::vector<char*> RedFaceFilePaths;
	char RedSkyLeftFile[] =		"../Assets/RedSky/left.png";
	char RedSkyRightFile[] =	"../assets/RedSky/right.png";
	char RedSkyBottomFile[] =	"../Assets/RedSky/bottom.png";
	char RedSkyTopFile[] =		"../Assets/RedSky/top.png";
	char RedSkyBackFile[] =		"../Assets/RedSky/back.png";
	char RedSkyFrontFile[] =	"../Assets/RedSky/front.png";
	RedFaceFilePaths.push_back(RedSkyLeftFile);
	RedFaceFilePaths.push_back(RedSkyRightFile);
	RedFaceFilePaths.push_back(RedSkyBottomFile);
	RedFaceFilePaths.push_back(RedSkyTopFile);
	RedFaceFilePaths.push_back(RedSkyBackFile);
	RedFaceFilePaths.push_back(RedSkyFrontFile);
	CubeMap* redSky = new CubeMap(RedFaceFilePaths);

	std::vector<char*> BlueFaceFilePaths;
	char BlueSkyLeftFile[] =	"../Assets/BlueSky/left.png";
	char BlueSkyRightFile[] =	"../assets/BlueSky/right.png";
	char BlueSkyBottomFile[] =	"../Assets/BlueSky/bottom.png";
	char BlueSkyTopFile[] =		"../Assets/BlueSky/top.png";
	char BlueSkyBackFile[] =	"../Assets/BlueSky/back.png";
	char BlueSkyFrontFile[] =	"../Assets/BlueSky/front.png";
	BlueFaceFilePaths.push_back(BlueSkyLeftFile);
	BlueFaceFilePaths.push_back(BlueSkyRightFile);
	BlueFaceFilePaths.push_back(BlueSkyBottomFile);
	BlueFaceFilePaths.push_back(BlueSkyTopFile);
	BlueFaceFilePaths.push_back(BlueSkyBackFile);
	BlueFaceFilePaths.push_back(BlueSkyFrontFile);
	CubeMap* blueSky = new CubeMap(BlueFaceFilePaths);

	std::vector<char*> BlackFaceFilePaths;
	char BlackSkyLeftFile[] =	"../Assets/BlackSky/left.png";
	char BlackSkyRightFile[] =	"../assets/BlackSky/right.png";
	char BlackSkyBottomFile[] =	"../Assets/BlackSky/bottom.png";
	char BlackSkyTopFile[] =	"../Assets/BlackSky/top.png";
	char BlackSkyBackFile[] =	"../Assets/BlackSky/back.png";
	char BlackSkyFrontFile[] =	"../Assets/BlackSky/front.png";
	BlackFaceFilePaths.push_back(BlackSkyLeftFile);
	BlackFaceFilePaths.push_back(BlackSkyRightFile);
	BlackFaceFilePaths.push_back(BlackSkyBottomFile);
	BlackFaceFilePaths.push_back(BlackSkyTopFile);
	BlackFaceFilePaths.push_back(BlackSkyBackFile);
	BlackFaceFilePaths.push_back(BlackSkyFrontFile);
	CubeMap* blackSky = new CubeMap(BlackFaceFilePaths);

	std::vector<char*> GrassFaceFilePaths;
	char GrassSkyLeftFile[] =	"../Assets/GrassSky/left.png";
	char GrassSkyRightFile[] =	"../assets/GrassSky/right.png";
	char GrassSkyBottomFile[] = "../Assets/GrassSky/bottom.png";
	char GrassSkyTopFile[] =	"../Assets/GrassSky/top.png";
	char GrassSkyBackFile[] =	"../Assets/GrassSky/back.png";
	char GrassSkyFrontFile[] =	"../Assets/GrassSky/front.png";
	GrassFaceFilePaths.push_back(GrassSkyLeftFile);
	GrassFaceFilePaths.push_back(GrassSkyRightFile);
	GrassFaceFilePaths.push_back(GrassSkyBottomFile);
	GrassFaceFilePaths.push_back(GrassSkyTopFile);
	GrassFaceFilePaths.push_back(GrassSkyBackFile);
	GrassFaceFilePaths.push_back(GrassSkyFrontFile);
	CubeMap* grassSky = new CubeMap(GrassFaceFilePaths);

	std::vector<char*> GalaxyFaceFilePaths;
	char GalaxySkyLeftFile[] =	"../Assets/GalaxySky/left.png";
	char GalaxySkyRightFile[] =	"../assets/GalaxySky/right.png";
	char GalaxySkyBottomFile[] = "../Assets/GalaxySky/bottom.png";
	char GalaxySkyTopFile[] =	"../Assets/GalaxySky/top.png";
	char GalaxySkyBackFile[] =	"../Assets/GalaxySky/back.png";
	char GalaxySkyFrontFile[] =	"../Assets/GalaxySky/front.png";
	GalaxyFaceFilePaths.push_back(GalaxySkyLeftFile);
	GalaxyFaceFilePaths.push_back(GalaxySkyRightFile);
	GalaxyFaceFilePaths.push_back(GalaxySkyBottomFile);
	GalaxyFaceFilePaths.push_back(GalaxySkyTopFile);
	GalaxyFaceFilePaths.push_back(GalaxySkyBackFile);
	GalaxyFaceFilePaths.push_back(GalaxySkyFrontFile);
	CubeMap* galaxySky = new CubeMap(GalaxyFaceFilePaths);


    Shader* skyboxVertexShader = new Shader("../Assets/skyboxVertex.glsl", GL_VERTEX_SHADER);
    Shader* skyboxfragmentShader = new Shader("../Assets/skyboxFragment.glsl", GL_FRAGMENT_SHADER);


    // Create A Shader Program for the skybox
    ShaderProgram* skyboxShaderProgram = new ShaderProgram();
    skyboxShaderProgram->AttachShader(skyboxVertexShader);
    skyboxShaderProgram->AttachShader(skyboxfragmentShader);

    // Create material for skybox
    Material* skyMat = new Material(skyboxShaderProgram);

    // Print instructions to the console.
    std::cout << "Use WASD to move, and the mouse to look around." << std::endl;
    std::cout << "Press escape or alt-f4 to exit." << std::endl;

	// Main Loop
	while (!glfwWindowShouldClose(window))
	{
		// cheap anti aliasing
		glEnable(GL_MULTISAMPLE);

        // Exit when escape is pressed.
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;

        // Calculate delta time.
        float dt = glfwGetTime();
        // Reset the timer.
        glfwSetTime(0);
        
        // Update the player controller
        controller.Update(window, viewportDimensions, mousePosition, dt);

        // View matrix.
        glm::mat4 view = controller.GetTransform().GetInverseMatrix();
        // Projection matrix.
        glm::mat4 projection = glm::perspective(.75f, viewportDimensions.x / viewportDimensions.y, .1f, 100.f);
        // Compose view and projection.
        glm::mat4 viewProjection = projection * view;


        // Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0, 0.0, 0.0, 0.0);


        // Set the camera and world matrices to the shader
        // The string names correspond directly to the uniform names within the shader.
        rockMat->SetMatrix(cameraViewVS, viewProjection);
        rockMat->SetMatrix(worldMatrixVS, transform.GetMatrix());

        // For specularity, we also need the position of the camera to calculate reflections
        rockMat->SetVec3(cameraPosFS, controller.GetTransform().Position());

		CubeMap* activeMap = nullptr;

		if (skyboxIndex == 0) activeMap = redSky;
		if (skyboxIndex == 1) activeMap = greenSky;
		if (skyboxIndex == 2) activeMap = blueSky;
		if (skyboxIndex == 3) activeMap = blackSky;
		if (skyboxIndex == 4) activeMap = grassSky;
		if (skyboxIndex == 5) activeMap = galaxySky;

		skyMat->SetCubeMap(cubeMapFS, activeMap);
		rockMat->SetCubeMap(cubeMapFS, activeMap);

        // Bind the material
        rockMat->Bind();
        model->Draw();
        rockMat->Unbind();

        // Draw the skybox
        glm::mat4 specialView = projection * glm::mat4(glm::mat3(view));
        skyMat->SetMatrix(cameraViewVS, specialView);
        glDepthFunc(GL_LEQUAL);
        skyMat->Bind();
        cube->Draw();
        skyMat->Unbind();
        // Set the depth test back to the default setting.
        glDepthFunc(GL_LESS);

		// Stop using the shader program.

		// Swap the backbuffer to the front.
		glfwSwapBuffers(window);

		// Poll input and window events.
		glfwPollEvents();
	}

    // Delete mesh objects
    delete model;
    delete cube;

    // Free memory used by materials and all sub objects
    delete rockMat;
    delete skyMat;

	// Free GLFW memory.
	glfwTerminate();

	// End of Program.
	return 0;
}
