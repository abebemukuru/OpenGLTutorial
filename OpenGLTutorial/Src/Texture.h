/**
*@file	Texture.h
*/
#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED
#include <GL/glew.h>
#include <memory>

class Texture;
//�e�N�X�`���|�C���^
typedef std::shared_ptr<Texture> TexturePtr;

/**
*�e�N�X�`���N���X
*/
class Texture {
public:
	static TexturePtr Create(
		int width, int height, GLenum iformat, GLenum format, const void* data);

	static TexturePtr LoadFromFile(const char*);

	GLint Id()const { return texId; }
	GLsizei Width() const { return width; }
	GLsizei Height()const { return height; }

private:
	
	//�f�X�g���N�^
	~Texture();

	//�R���X�g���N�^
	Texture();

	Texture(const Texture&) = delete;
	Texture& operator= (const Texture&) = delete;
	
	GLuint texId;
	int width;
	int height;
};
#endif // !TEXTURE_H_INCLUDED