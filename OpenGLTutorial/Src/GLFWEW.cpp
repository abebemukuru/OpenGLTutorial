/**
*@file GLDWEW.cpp
*/
#include "GLFWEW.h"
#include <iostream>

//GLFW��GLEW�����b�v���邽�߂̖��O���
namespace GLFWEW {

	/**
	* �V���O���g���C���X�^���X���擾����
	*
	*@return	Window�̃V���O���g���C���X�^���X
	*/
	Window& Window::Instance() {
		
		static Window instance;
		return instance;
	}

	/**
	*�R���X�g���N�^
	*/
	Window::Window() : isGLFWInitialized(false),isInitialized(false),window(nullptr){}

	/**
	* �f�X�g���N�^
	*/
	Window::~Window() {

		//�������������I����Ă�����
		if (isGLFWInitialized) {
			//�I������
			glfwTerminate();
		}
	}

	/**
	* GLFW/GLEW�̏�����
	*
	*@param	w �E�B���h�E�̕`��͈͂̕��i�s�N�Z���j
	*@param	h �E�B���h�E�̕`��͈͂̍����i�s�N�Z���j
	*@param	title �E�B���h�E�^�C�g���iUTF-8��0�I�Օ�����j
	*
	*@retval true ����������
	*@retval false �������s
	*/
	bool Window::Init(int w, int h, const char* title) {

		//��������������������Ă���ꍇ
		if (isInitialized) {

			std::cerr << "ERROR: GLFWEW�͊��ɏ���������Ă��܂�" << std::endl;
			return false;
		}

		//GLFW�̏�����������Ă��Ȃ��ꍇ
		if (!isGLFWInitialized) {
			//�G���[�R�[���o�b�N�̃Z�b�g
			glfwSetErrorCallback(ErrorCallback);

			//GLFW�̏������������ł͂Ȃ�������false��Ԃ�
			if (glfwInit() != GL_TRUE) return false;

			//�����܂ŗ��ď��߂ď�����������
			isGLFWInitialized = true;
		}

		//�E�B���h�E����������Ă��Ȃ�������
		if (!window) {
			
			//�E�B���h�E�𐶐�
			window = glfwCreateWindow(w, h, title, nullptr, nullptr);

			//��������Ȃ�������false��Ԃ�
			if (!window) return false;
			
			//�E�B���h�E�̃R���e�L�X�g��`��Ώۂɐݒ�
			glfwMakeContextCurrent(window);
		}

		//GLEW�̏����������s��������
		if (glewInit() != GLEW_OK) {

			std::cerr << "ERROR: GLEW�̏������Ɏ��s���܂���" << std::endl;
			return false;
		}
		//�`��f�o�C�X�̕\��
		const GLubyte* renderer = glGetString(GL_RENDERER);
		std::cout << "Renderer: " << renderer << std::endl;

		//�Ή��o�[�W�����̕\��
		const GLubyte* version = glGetString(GL_VERSION);
		std::cout << "Version: " << version << std::endl;

		//�������t���O��true�ɂ���
		isInitialized = true;

		return true;
	}

	/**
	* �E�B���h�E�����ׂ������ׂ�
	*
	*@retval true ����
	*@retval false ���Ȃ�
	*/
	bool Window::ShouldClose()const {

		return glfwWindowShouldClose(window) != 0;
	}

	/**
	* �t�����g�o�b�t�@�ƃo�b�N�o�b�t�@��؂�ւ���
	*/
	void Window::SwapBuffers()const {

		//�C�x���g�̏���
		glfwPollEvents();
		//�t�����g�ƃo�b�N�̓���ւ�
		glfwSwapBuffers(window);
	}

}//namespace GLFWEW


 /**
 *GLFW����̃G���[�񍐂���������
 *
 *@param		error �G���[�ԍ�
 *@param		desc �G���[�̓��e
 */
void ErrorCallback(int error, const char* desc) {

	std::cerr << "ERROR :" << desc << std::endl;
}
