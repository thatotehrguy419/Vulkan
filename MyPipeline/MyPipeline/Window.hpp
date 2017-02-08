#pragma once
#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "DebugMode.hpp"
#include <Vulkan/vulkan.h>
#include <vector>
#include "GLFW/glfw3.h"
#include "Renderer.hpp"

class Window
{
public:
	Window();
	~Window();

	void Update();

	int ShouldClose();

private:

	GLFWwindow* window = nullptr;

	Renderer renderer;

	void CreateWindow();

	static bool initialized;
};

#endif // WINDOW_HPP
