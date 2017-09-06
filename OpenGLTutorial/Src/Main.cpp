/**
*@file Main.cpp
*/

#include "GLFWEW.h"
#include <iostream>
#include <vector>
#include "glm/gtc/matrix_transform.hpp"
#include"Texture.h"
#include "Shader.h"
#include "OffscreenBuffer.h"
#include "UniformBuffer.h"
#include "Mesh.h"
#include "Entity.h"
#include <random>
#include <time.h>


//3D�x�N�^�[�^
struct Vector3
{
	float x, y, z;
};

//�q�f�a�`�J���[�^
struct Color
{
	float r, g, b, a;
};

//���_�f�[�^�^
struct Vertex
{
	glm::vec3 position;
	glm::vec4 color;
	glm::vec2 texCoord;
};

//���_�f�[�^
const Vertex vertices[] = {
	{ { -0.5f,-0.3f,0.5f },{ 0.0f,0.0f,1.0f,1.0f },{0.0f,0.0f} },
	{ { 0.3f,-0.3f, 0.5f },{ 0.0f,1.0f,0.0f,1.0f },{ 1.0f,0.0f } },
	{ { 0.3f,0.5f,0.5f },{ 0.0f,0.0f,1.0f,1.0f },{ 1.0f,1.0f } },
	{ { -0.5f,0.5f,0.5f },{ 1.0f,0.0f,0.0f,1.0f },{ 0.0f,1.0f } },

	{ { -0.3f,0.3f,0.1f },{ 0.0f,0.0f,1.0f,1.0f },{ 0.0f,1.0f } },
	{ { -0.3f,-0.5f, 0.1f},{ 0.0f,1.0f,1.0f,1.0f },{ 0.0f,0.0f } },
	{ { 0.5f,-0.5f,0.1f },{ 0.0f,0.0f,1.0f,1.0f },{ 1.0f,0.0f } },
	{ { 0.5f,-0.5f,0.1f },{ 1.0f,0.0f,0.0f,1.0f },{ 1.0f,0.0f } },
	{ { 0.5f,0.3f,0.1f },{ 1.0f,1.0f,0.0f,1.0f },{ 1.0f,1.0f } },
	{ { -0.3f,0.3f,0.1f },{ 1.0f,0.0f,1.0f,1.0f },{ 0.0f,1.0f } },

	{ { -1.0f,-1.0f,0.5f },{ 1.0f,1.0f,1.0f,1.0f },{ 1.0f,0.0f } },
	{ { 1.0f,-1.0f, 0.5f },{ 1.0f,1.0f,1.0f,1.0f },{ 0.0f,0.0f } },
	{ { 1.0f,1.0f,0.5f },{ 1.0f,1.0f,1.0f,1.0f },{ 0.0f,1.0f } },
	{ { -1.0f,1.0f,0.5f },{ 1.0f,1.0f,1.0f,1.0f },{ 1.0f,1.0f } },

};

//�C���f�b�N�X�f�[�^
const GLuint indices[] = {
	0,1,2,2,3,0,
	4,5,6,7,8,9,
	10,11,12,12,13,10,
};


//���_�V�F�[�_�̃p�����[�^�^
struct VertexData {

	glm::mat4 matMVP;
	glm::mat4 matModel;
	glm::mat3x4 matNormal;
};

/**
*���C�g�f�[�^�i�_�����j
*/
struct PointLight {

	glm::vec4 position;
	glm::vec4 color;
};

const int maxLightCount = 4;

/**
*���C�e�B���O�p�����[�^�[
*/
struct LightData {

	glm::vec4 ambientColor;
	PointLight light[maxLightCount];
};

/**
*�|�X�g�G�t�F�N�g�f�[�^
*/
struct PostEffectData {
	glm::mat4x4 matColor;
};

/**
*�����`��f�[�^
*/
struct RenderingPart
{
	GLsizei size;
	GLvoid* offset;
};

/*
*RenderingPart���쐬����
*
*@param	size	�`�悷��C���f�b�N�X��
*@param	offset	�`��J�n�C���f�b�N�X�̃I�t�Z�b�g�i�C���f�b�N�X�P�ʁj
*
*@return	�쐬���������`��I�u�W�F�N�g
*/
constexpr RenderingPart MakeRenderingPart(GLsizei size, GLsizei offset) {

	return{ size,reinterpret_cast<GLvoid*>(offset * sizeof(GLuint)) };
}

/**
*�����`��f�[�^���X�g
*/
static const RenderingPart renderingparts[] = {
	MakeRenderingPart(12,0),
	MakeRenderingPart(6,12),
};

/**
*Vertex Buffer Object���쐬����
*
*@param	size ���_�f�[�^�̃T�C�Y
*@param	data�@���_�f�[�^�ւ̃|�C���^
*
@return	�쐬�����u�a�n
*/
GLuint CreateVBO(GLsizeiptr size, const GLvoid* data) {

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return vbo;
}

/**
*Index Buffer Object ���쐬����
*
*@param	size	�C���f�b�N�X�f�[�^�̃T�C�Y
*@param	data	�C���f�b�N�X�f�[�^�ւ̃|�C���^
*
*@return	�쐬����IBO
*/
GLuint CreateIBO(GLsizeiptr size, const GLvoid* data) {

	GLuint ibo = 0;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return ibo;
}

/**
*���_�A�g���r���[�g��ݒ肷��
*
*@param	index	���_�A�g���r���[�g�̃C���f�b�N�X
*@param	cls		���_�f�[�^�^��
*@param mbr		���_�A�g���r���[�g�ɐݒ肷��cls�Ƀ����o�ϐ���
*/
#define SetVertexArreibPointer(index,cls,mbr) SetVertexAttribPointerI(\
index,\
sizeof (cls::mbr) / sizeof(float),\
sizeof(cls),\
reinterpret_cast<GLvoid*>(offsetof(cls,mbr)))

void SetVertexAttribPointerI(
	GLuint index, GLuint size, GLsizei stride, const GLvoid* pointer) {

	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, pointer);
}

/**
*Vertex Array Object���쐬����
*
*@param	vbo	VAO�Ɋ֘A�Â�����u�a�n
*@param	ibo	VAO�Ɋ֘A�Â�����IBO
*
*@return	�쐬�����u�`�n
*/
GLuint CreateVAO(GLint vbo,GLuint ibo) {

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	SetVertexArreibPointer(0, Vertex, position);
	SetVertexArreibPointer(1, Vertex, color);
	SetVertexArreibPointer(2, Vertex, texCoord);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBindVertexArray(0);

	return vao;
}

/**
*�G�̉~�Ղ̏�Ԃ��X�V����
*/
struct UpdateToroid {

	explicit UpdateToroid(Entity::Buffer& buffer) : entityBuffer(buffer){}


	void operator()(Entity::Entity& entity, void* ubo, double delta, const glm::mat4& matView, const glm::mat4& matProj) {

		//�͈͊O�ɏo����폜����
		const glm::vec3 pos = entity.Position();
		if (std::abs(pos.x) > 40.0f || std::abs(pos.z) > 40.0f) {

			entityBuffer.RemoveEntity(&entity);

			return;
		}

		//�~�Ղ���]������
		float rot = glm::angle(entity.Rotation());
		rot += glm::radians(15.0f) * static_cast<float>(delta);
		if (rot > glm::pi<float>() * 2.0f) {
			rot -= glm::pi<float>() * 2.0f;
		}

		entity.Rotation(glm::angleAxis(rot, glm::vec3(0, 1, 0)));

		//���_�V�F�[�_�̃p�����[�^���t�a�n�ɃR�s�[����
		VertexData data;
		data.matModel = entity.CalcModelMatrix();
		data.matNormal = glm::mat4_cast(entity.Rotation());
		data.matMVP = matProj * matView * data.matModel;
		memcpy(ubo, &data, sizeof(VertexData));
	}

	Entity::Buffer& entityBuffer;
};

/**
*�Q�[���̏�Ԃ��X�V����
*
*@param	entityBuffer	�G�G���e�B�e�B�ǉ���̃G���e�B�e�B�o�b�t�@
*@param	meshBuffer		�G�G���e�B�e�B�̃��b�V�����Ǘ����Ă��郁�b�V���o�b�t�@
*@param	tex				�G�G���e�B�e�B�p�̃e�N�X�`��
*@param	prog			�G�G���e�B�e�B�p�̃V�F�[�_�v���O����
*/
void Update(Entity::BufferPtr entityBuffer, Mesh::BufferPtr meshBuffer,
	TexturePtr tex, Shader::ProgramPtr prog) {

	static std::mt19937 rand(time(nullptr));
	static double interval = 0;

	interval -= 1.0 / 60.0;

	if (interval <= 0) {

		const std::uniform_real_distribution<float> posXRange(-15, 15);
		const glm::vec3 pos(posXRange(rand), 0, 40);
		const Mesh::MeshPtr& mesh = meshBuffer->GetMesh("Toroid");
		if (Entity::Entity* p = entityBuffer->AddEntity(pos, mesh, tex, prog, UpdateToroid(*entityBuffer))) {

			p->Velocity(glm::vec3(pos.x < 0 ? 0.1f : -0.1f, 0, -1.0f));
		}

		const std::uniform_real_distribution<double> intervalRange(3.0, 6.0);
		interval = intervalRange(rand);
	}
}

/**
*Uniform Block Object���쐬����
*
*@param	size	Uniform Block�̃T�C�Y
*@param	data	Uniform Block�ɓ]������f�[�^�ւ̃|�C���^
*
*@return	�쐬�����t�a�n
*/
GLuint CreateUBO(GLsizeiptr size, const GLvoid* data = nullptr) {

	GLuint ubo;
	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STREAM_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	return ubo;
}

//�G���g���[�|�C���g
int main() {

	//GLFEW�̐���
	GLFWEW::Window& window = GLFWEW::Window::Instance();

	//�F
	float color = 0.0f;

	//�����������Ɏ��s������I������
	if (!window.Init(800, 600, "OpenGL Tutorial")) return 1;

	const GLuint vbo = CreateVBO(sizeof(vertices), vertices);
	const GLuint ibo = CreateIBO(sizeof(indices), indices);
	const GLuint vao = CreateVAO(vbo,ibo);
	
	const UniformBufferPtr uboVertex = UniformBuffer::Create(
		sizeof(VertexData), 0, "VertexData");

	const UniformBufferPtr uboLight = UniformBuffer::Create(
		sizeof(LightData), 1, "LightData");

	const UniformBufferPtr uboPostEffect =
		UniformBuffer::Create(sizeof(PostEffectData), 2, "PostEffectData");

	//const GLuint shaderProgram = Shader::CreateProgramFromFile("Res/Turorial.vert", "Res/Tutorial.frag");
	const Shader::ProgramPtr progTutorial = Shader::Program::Create("Res/Turorial.vert", "Res/Tutorial.frag");

	//const Shader::ProgramPtr progColorFilter = Shader::Program::Create("Res/ColorFilter.vert", "Res/ColorFilter.frag");
	const Shader::ProgramPtr progPoster = Shader::Program::Create("Res/Posterization.vert", "Res/Posterization.frag");
	if (!vbo || !ibo || !vao || !uboVertex || !uboLight || !progTutorial || !progPoster) {

		return 1;
	}
	progTutorial->UniformBlockBinding("VertexData", 0);
	progTutorial->UniformBlockBinding("LightData", 1);
	

	//const GLuint uboIndex = glGetUniformBlockIndex(progTutorial, "VertexData");
	
	//if (uboIndex == GL_INVALID_INDEX) return 1;

	//glUniformBlockBinding(shaderProgram, uboIndex, 0);

	//�[�x�o�b�t�@�̐ݒ�
	glEnable(GL_DEPTH_TEST);

	//�e�N�X�`���f�[�^
	static const uint32_t textureData[] = {
		0xffffffff,	0xffcccccc,	0xffffffff,	0xffcccccc,	0xffffffff,
		0xff888888,	0xffffffff,	0xff888888,	0xffffffff,	0xff888888,
		0xffffffff,	0xff444444,	0xffffffff,	0xff444444,	0xffffffff,
		0xff000000,	0xffffffff,	0xff000000,	0xffffffff,	0xff000000,
		0xffffffff,	0xff000000,	0xffffffff,	0xff000000,	0xffffffff,
	};

	//TexturePtr tex = Texture::Create(5, 5, GL_RGBA8, GL_RGBA, textureData);
	TexturePtr texToroid = Texture::LoadFromFile("Res/Toroid.bmp");
	if (!texToroid) return -1;


	Mesh::BufferPtr meshBuffer = Mesh::Buffer::Create(10 * 1024, 30 * 1024);
	meshBuffer->LoadMeshFromFile("Res/Toroid.fbx");

	Entity::BufferPtr entityBuffer = Entity::Buffer::Create(1024, sizeof(VertexData), 0, "VertexData");

	if (!entityBuffer) {

		return 1;
	}

	std::mt19937 rand(static_cast<unsigned int>(time(nullptr)));

	const OffscreenBufferPtr offscreen = OffscreenBuffer::Create(800, 600);
	//�E�B���h�E�����Ȃ����
	while (!window.ShouldClose()){

		Update(entityBuffer, meshBuffer, texToroid, progTutorial);

		glBindFramebuffer(GL_FRAMEBUFFER, offscreen->GetFramebuffer());
		
		//�o�b�N�o�b�t�@����������ۂɎg����F���w��
		glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
		//�o�b�N�o�b�t�@������
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		progTutorial->UseProgram();
		
		//static float degree = 0.0f;
		//degree += 0.1f;
		//if (degree >= 360.0f) { degree -= 360.0f; }

		const glm::vec3 viewPos = glm::vec4(0, 20, -20, 1);

		//const GLint matMVPLoc = glGetUniformLocation(shaderProgram, "matMVP");
		const glm::mat4x4 matProj =
			glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		const glm::mat4x4 matView =
			glm::lookAt(viewPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		VertexData vertexData;
		vertexData.matMVP = matProj * matView;

		uboVertex->BuffferSubData(&vertexData);

		LightData lightData;
		lightData.ambientColor = glm::vec4(0.05f, 0.1f, 0.2f, 1);

		/*
		lightData.light[0].position = glm::vec4(1, 1, 1, 1);
		lightData.light[0].color = glm::vec4(2, 2, 2, 1);
		lightData.light[1].position = glm::vec4(-0.2f, 0, 0.6f, 1);
		lightData.light[1].color = glm::vec4(0.125f, 0.125f, 0.05f, 1);
		*/
		lightData.light[0].color = glm::vec4(12000, 12000, 12000, 1);
		lightData.light[0].position = glm::vec4(40, 100, 20, 1);
		uboLight->BuffferSubData(&lightData);

		//const GLint colorSamplerLoc = glGetUniformLocation(shaderProgram, "colorSampler");

		progTutorial->BindTexture(GL_TEXTURE0, GL_TEXTURE_2D, texToroid->Id());
		meshBuffer->BindVAO();
		meshBuffer->GetMesh("Toroid")->Draw(meshBuffer);

		entityBuffer->Update(1.0 / 60.0, matView, matProj);
		entityBuffer->Draw(meshBuffer);
		glBindVertexArray(vao);

		//glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, renderingparts[0].size,
			GL_UNSIGNED_INT,renderingparts[0].offset);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//progColorFilter->UseProgram();
		progPoster->UseProgram();


		/*
		if (colorSamplerLoc >= 0) {
			glBindTexture(GL_TEXTURE_2D, offscreen->GetTexture());
		}
		*/

		progTutorial->BindTexture(GL_TEXTURE0, GL_TEXTURE_2D, offscreen->GetTexture());

		/*
		//��������

		//aaaa

		//asdfcvb
		//aaaa
		vertexData = {};
		
		uboVertex->BuffferSubData(&vertexData);

		lightData = {};
		lightData.ambientColor = glm::vec4(1);
		uboLight->BuffferSubData(&lightData);
		*/

		PostEffectData postEffect;
		postEffect.matColor[0] = glm::vec4(0.393f, 0.349f, 0.272f, 0);
		postEffect.matColor[1] = glm::vec4(0.769f, 0.686f, 0.534f, 0);
		postEffect.matColor[2] = glm::vec4(0.189f, 0.168f, 0.131f, 0);
		postEffect.matColor[3] = glm::vec4(0,0, 0,1);
		uboPostEffect->BuffferSubData(&postEffect);

		glDrawElements(
			GL_TRIANGLES, renderingparts[1].size,
			GL_UNSIGNED_INT, renderingparts[1].offset);

		//�t�����g�o�b�t�@�ƃo�b�N�o�b�t�@�̓���ւ�
		window.SwapBuffers();
	}

	//glDeleteProgram(progTutorial);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);

	return 0;
}