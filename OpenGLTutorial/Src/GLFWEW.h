#pragma once


/**
*@file GLFWEW.h
*/
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace GLFWEW {

	/**
	*GLFWとGLEWのラッパークラス
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
*GLFWからのエラー報告を処理する
*
*@param		error エラー番号
*@param		desc エラーの内容
*/
void ErrorCallback(int error, const char* desc);

