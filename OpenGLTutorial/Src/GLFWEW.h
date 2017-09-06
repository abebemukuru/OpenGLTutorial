#pragma once


/**
*@file GLFWEW.h
*/
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace GLFWEW {

	/**
	*GLFW��GLEW�̃��b�p�[�N���X
	*/
	class Window {
	public:
		static Window& Instance();
		bool Init(int w, int h, const char* title);
		bool ShouldClose() const;
		void SwapBuffers() const;

	private:
		Window();
		~Window();
		Window(const Window&) = delete;
		Window& operator = (const Window&) = delete;

		bool isGLFWInitialized;
		bool isInitialized;
		GLFWwindow* window;
	};
}	//namespace GLF-WEW


/**
*GLFW����̃G���[�񍐂���������
*
*@param		error �G���[�ԍ�
*@param		desc �G���[�̓��e
*/
void ErrorCallback(int error, const char* desc);

