﻿/*
Title: Specular Maps
File Name: main.cpp
Copyright � 2016
Author: David Erbelding
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
#include "../header/mesh.h"
#include "../header/fpsController.h"
#include "../header/transform3d.h"
#include "../header/material.h"
#include "../header/texture.h"
#include "../header/cubeMap.h"
#include <iostream>




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


int main(int argc, char **argv)
{
	// Initialize GLFW
	glfwInit();

	// Initialize window
	GLFWwindow* window = glfwCreateWindow(viewportDimensions.x, viewportDimensions.y, "Shiny", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set window callbacks
	glfwSetFramebufferSizeCallback(window, resizeCallback);
    glfwSetCursorPosCallback(window, mouseMoveCallback);

	// Initialize glew
	glewInit();

    // The mesh loading code has changed slightly, we now have to do some extra math to take advantage of our normal maps.
    // Here we pass in true to calculate tangents.
    Mesh* model = new Mesh("../assets/ironbuckler.obj", true);
    Mesh* cube = new Mesh("../assets/cube.obj", true);

    // The transform being used to draw our second shape.
    Transform3D transform;
    transform.SetPosition(glm::vec3(0, 0, -2));
    transform.RotateX(1.5);


    // Make a first person controller for the camera.
    FPSController controller = FPSController();


	// Create Shaders
    Shader* vertexShader = new Shader("../shaders/vertex.glsl", GL_VERTEX_SHADER);
    Shader* fragmentShader = new Shader("../shaders/fragment.glsl", GL_FRAGMENT_SHADER);


    // Create A Shader Program
    // The class wraps all of the functionality of a gl shader program.
    ShaderProgram* shaderProgram = new ShaderProgram();
    shaderProgram->AttachShader(vertexShader);
    shaderProgram->AttachShader(fragmentShader);


    // Create a material using a texture for our model
    Material* material = new Material(shaderProgram);
    material->SetTexture("diffuseMap", new Texture("../assets/iron_buckler_diffuse.png"));
    material->SetTexture("normalMap", new Texture("../assets/iron_buckler_normal.png"));
    material->SetTexture("specularMap", new Texture("../assets/iron_buckler_specular.png"));
    material->SetInt("specularExponent", 64);


    Shader* skyboxVertexShader = new Shader("../shaders/skyboxvertex.glsl", GL_VERTEX_SHADER);
    Shader* skyboxfragmentShader = new Shader("../shaders/skyboxfragment.glsl", GL_FRAGMENT_SHADER);


    // Create A Shader Program
    // The class wraps all of the functionality of a gl shader program.
    ShaderProgram* skyboxShaderProgram = new ShaderProgram();
    skyboxShaderProgram->AttachShader(skyboxVertexShader);
    skyboxShaderProgram->AttachShader(skyboxfragmentShader);


    Material* skyMat = new Material(skyboxShaderProgram);
    std::vector<char*> faceFilePaths;
    faceFilePaths.push_back("../assets/skyboxLeft.png");
    faceFilePaths.push_back("../assets/skyboxRight.png");
    faceFilePaths.push_back("../assets/skyboxBottom.png");
    faceFilePaths.push_back("../assets/skyboxTop.png");
    faceFilePaths.push_back("../assets/skyboxBack.png");
    faceFilePaths.push_back("../assets/skyboxFront.png");
    skyMat->SetCubeMap("cubeMap", new CubeMap(faceFilePaths));

    // Print instructions to the console.
    std::cout << "Use WASD to move, and the mouse to look around." << std::endl;
    std::cout << "Press escape or alt-f4 to exit." << std::endl;



	// Main Loop
	while (!glfwWindowShouldClose(window))
	{
        // Exit when escape is pressed.
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;

        // Calculate delta time.
        float dt = glfwGetTime();
        // Reset the timer.
        glfwSetTime(0);
        

        // Update the player controller
        controller.Update(window, viewportDimensions, mousePosition, dt);
        

        // rotate cube transform
        transform.RotateY(1.0f * dt);



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
        material->SetMatrix("cameraView", viewProjection);
        material->SetMatrix("worldMatrix", transform.GetMatrix());

        // For specularity, we also need the position of the camera to calculate reflections
        material->SetVec3("cameraPosition", controller.GetTransform().Position());

        // Bind the material
        material->Bind();
        model->Draw();
        material->Unbind();

        // Draw the skybox
        glm::mat4 specialView = projection * glm::mat4(glm::mat3(view));
        skyMat->SetMatrix("cameraView", specialView);
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

    delete model;
    delete cube;

    // Free material should free all objects used by material
    delete material;
    delete skyMat;

	// Free GLFW memory.
	glfwTerminate();

	// End of Program.
	return 0;
}