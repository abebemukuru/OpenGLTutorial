/**
*@file GLDWEW.cpp
*/
#include "GLFWEW.h"
#include <iostream>

//GLFWとGLEWをラップするための名前空間
namespace GLFWEW {

	/**
	* シングルトンインスタンスを取得する
	*
	*@return	Windowのシングルトンインスタンス
	*/
	Window& Window::Instance() {
		
		static Window instance;
		return instance;
	}

	/**
	*コンストラクタ
	*/
	Window::Window() : isGLFWInitialized(false),isInitialized(false),window(nullptr){}

	/**
	* デストラクタ
	*/
	Window::~Window() {

		//初期化処理が終わっていたら
		if (isGLFWInitialized) {
			//終了処理
			glfwTerminate();
		}
	}

	/**
	* GLFW/GLEWの初期化
	*
	*@param	w ウィンドウの描画範囲の幅（ピクセル）
	*@param	h ウィンドウの描画範囲の高さ（ピクセル）
	*@param	title ウィンドウタイトル（UTF-8の0終盤文字列）
	*
	*@retval true 初期化成功
	*@retval false 初期失敗
	*/
	bool Window::Init(int w, int h, const char* title) {

		//初期化処理が完了されている場合
		if (isInitialized) {

			std::cerr << "ERROR: GLFWEWは既に初期化されています" << std::endl;
			return false;
		}

		//GLFWの初期化がされていない場合
		if (!isGLFWInitialized) {
			//エラーコールバックのセット
			glfwSetErrorCallback(ErrorCallback);

			//GLFWの初期化が成功ではなかったらfalseを返す
			if (glfwInit() != GL_TRUE) return false;

			//ここまで来て初めて初期化が完了
			isGLFWInitialized = true;
		}

		//ウィンドウが生成されていなかったら
		if (!window) {
			
			//ウィンドウを生成
			window = glfwCreateWindow(w, h, title, nullptr, nullptr);

			//生成されなかったらfalseを返す
			if (!window) return false;
			
			//ウィンドウのコンテキストを描画対象に設定
			glfwMakeContextCurrent(window);
		}

		//GLEWの初期化が失敗だったら
		if (glewInit() != GLEW_OK) {

			std::cerr << "ERROR: GLEWの初期化に失敗しました" << std::endl;
			return false;
		}
		//描画デバイスの表示
		const GLubyte* renderer = glGetString(GL_RENDERER);
		std::cout << "Renderer: " << renderer << std::endl;

		//対応バージョンの表示
		const GLubyte* version = glGetString(GL_VERSION);
		std::cout << "Version: " << version << std::endl;

		//初期化フラグをtrueにする
		isInitialized = true;

		return true;
	}

	/**
	* ウィンドウを閉じるべきか調べる
	*
	*@retval true 閉じる
	*@retval false 閉じない
	*/
	bool Window::ShouldClose()const {

		return glfwWindowShouldClose(window) != 0;
	}

	/**
	* フロントバッファとバックバッファを切り替える
	*/
	void Window::SwapBuffers()const {

		//イベントの処理
		glfwPollEvents();
		//フロントとバックの入れ替え
		glfwSwapBuffers(window);
	}

}//namespace GLFWEW


 /**
 *GLFWからのエラー報告を処理する
 *
 *@param		error エラー番号
 *@param		desc エラーの内容
 */
void ErrorCallback(int error, const char* desc) {

	std::cerr << "ERROR :" << desc << std::endl;
}
