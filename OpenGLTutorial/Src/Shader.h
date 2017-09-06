/**
*@file	Shader.h
*/
#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED
#include <GL/glew.h>
#include <string>
#include <memory>

namespace Shader {

	class Program;
	typedef std::shared_ptr<Program> ProgramPtr;

	/**
	*�V�F�[�_�v���O�����N���X
	*/
	class Program {
	public:
		static ProgramPtr Create(const char* vsFilename, const char* fsFilename);

		bool UniformBlockBinding(const char* blockName, GLuint bindingPoint);
		void UseProgram();
		void BindTexture(GLenum uint, GLenum type, GLuint texture);

	private:
		Program() = default;
		~Program();
		Program(const Program&) = delete;
		Program& operator=(const Program&) = delete;

	private:
		GLuint program = 0;
		GLuint samplerLocation = -1;
		int samplerCount = 0;
		std::string name;
	};
	GLuint CreateProgramFromFile(const char* vsFileName, const char* fsFilename);
}


#endif // !SHADER_H_INCLUDED
