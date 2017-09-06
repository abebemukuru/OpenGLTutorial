/**
*@file	Texture.h
*/
#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED
#include <GL/glew.h>
#include <memory>

class Texture;
//テクスチャポインタ
typedef std::shared_ptr<Texture> TexturePtr;

/**
*テクスチャクラス
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
	
	//デストラクタ
	~Texture();

	//コンストラクタ
	Texture();

	Texture(const Texture&) = delete;
	Texture& operator= (const Texture&) = delete;
	
	GLuint texId;
	int width;
	int height;
};
#endif // !TEXTURE_H_INCLUDED