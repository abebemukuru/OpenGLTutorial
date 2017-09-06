/**
*@file	OffscreenBuffer.h
*/
#ifndef OFFSCREENBUFFER_H_INCLUDED
#define OFFSCREENBUFFER_H_INCLUDED

#include "Texture.h"
#include <GL/glew.h>
#include <memory>

class OffscreenBuffer;
typedef std::shared_ptr<OffscreenBuffer> OffscreenBufferPtr;

/**
*�I�t�X�N���[���o�b�t�@
*/
class OffscreenBuffer {
public:
	static OffscreenBufferPtr Create(int w, int h);
	GLuint GetFramebuffer()const { return framebuffer; }///<	�t���[���o�b�t�@���擾����
	GLuint GetTexture()const { return tex->Id(); }	///<	�t���[���o�b�t�@�p�̃e�N�X�`�����擾����

private:
	OffscreenBuffer() = default;
	OffscreenBuffer(const OffscreenBuffer&) = delete;
	OffscreenBuffer& operator=(const OffscreenBuffer&) = delete;
	~OffscreenBuffer();

private:
	TexturePtr tex;
	GLuint depthbuffer = 0;
	GLuint framebuffer = 0;
};

#endif // !OFFSCREENBUFFER_H_INCLUDED