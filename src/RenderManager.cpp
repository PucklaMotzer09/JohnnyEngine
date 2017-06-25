#include "RenderManager.h"
#include "Shader.h"
#include "Actor.h"
#include "ResourceManager.h"
#include "Light.h"
#include "OBJLoader.h"
#include "MainClass.h"
#include "Camera.h"
#include <iostream>

Shader* RenderManager::DEFAULT_SHADER = nullptr;
Shader* RenderManager::DEFAULT_SHADOWMAP_SHADER = nullptr;
Shader* RenderManager::DEFAULT_POST_PROCESSING_SHADER = nullptr;

void RenderManager::loadDefaultShaders(SDL::ResourceManager* res,Lighting* light)
{
	DEFAULT_SHADER = new Shader("Default Shader");
	DEFAULT_SHADOWMAP_SHADER = new Shader("Default Shadowmap Shader");
	DEFAULT_POST_PROCESSING_SHADER = new Shader("Default Post Processing Shader");

	std::map<std::string, std::string> defineChanges = light->getDefineChanges();

	DEFAULT_SHADER->addVertexShader(res->loadShader("vertexShader.glsl"));
	DEFAULT_SHADER->addFragmentShader(res->loadShader("fragmentShader.glsl", &defineChanges));

	DEFAULT_SHADOWMAP_SHADER->addVertexShader(res->loadShader("vertexShaderShadowMap.glsl"));
	DEFAULT_SHADOWMAP_SHADER->addFragmentShader(res->loadShader("fragmentShaderShadowMap.glsl"));

	DEFAULT_POST_PROCESSING_SHADER->addVertexShader(res->loadShader("vertexShaderFrameBuffer.glsl"));
	DEFAULT_POST_PROCESSING_SHADER->addFragmentShader(res->loadShader("fragmentShaderFrameBuffer.glsl"));


	DEFAULT_SHADER->addAttribute("vertexPosition", 0);
	DEFAULT_SHADER->addAttribute("vertexNormal", 1);
	DEFAULT_SHADER->addAttribute("vertexUV", 2);

	DEFAULT_POST_PROCESSING_SHADER->addAttribute("vertexPosition", 0);
	DEFAULT_POST_PROCESSING_SHADER->addAttribute("vertexNormal", 1);
	DEFAULT_POST_PROCESSING_SHADER->addAttribute("vertexUV", 2);

	DEFAULT_SHADOWMAP_SHADER->addAttribute("vertexPosition", 0);
	DEFAULT_SHADOWMAP_SHADER->addAttribute("vertexNormal", 1);
	DEFAULT_SHADOWMAP_SHADER->addAttribute("vertexUV", 2);


	DEFAULT_SHADER->link();

	DEFAULT_SHADER->addUniform("transform");
	DEFAULT_SHADER->addUniform("ambientLight");
	DEFAULT_SHADER->addUniform("worldMatrix");
	DEFAULT_SHADER->addUniform("eyePosition");
	Material::setUniforms(DEFAULT_SHADER);

	DEFAULT_POST_PROCESSING_SHADER->link();

	DEFAULT_POST_PROCESSING_SHADER->addUniform("frameBuffer");

	DEFAULT_SHADOWMAP_SHADER->link();

	DEFAULT_SHADOWMAP_SHADER->addUniform("lightSpaceMatrix");
	DEFAULT_SHADOWMAP_SHADER->addUniform("worldMatrix");

	DEFAULT_SHADOWMAP_SHADER->setShadowMap(true);

}

void RenderManager::unload()
{
	if(DEFAULT_SHADER)
		delete DEFAULT_SHADER;
	if(DEFAULT_SHADOWMAP_SHADER)
		delete DEFAULT_SHADOWMAP_SHADER;
	if(DEFAULT_POST_PROCESSING_SHADER)
		delete DEFAULT_POST_PROCESSING_SHADER;
}

RenderManager::RenderManager()
{
}


RenderManager::~RenderManager()
{
}

void RenderManager::addShader(Shader* s)
{
	m_shaderActors[s] = std::vector<SDL::Actor*>();
}

void RenderManager::addActor(SDL::Actor* a)
{
	if (!a->getShader())
		return;

	m_shaderActors[a->getShader()].push_back(a);
}

void RenderManager::removeShader(Shader* s)
{
	m_shaderActors.erase(s);
}

void RenderManager::removeActor(SDL::Actor* a)
{
	if (!a->getShader())
		return;

	std::vector<SDL::Actor*>* actors = &m_shaderActors[a->getShader()];

	for (size_t i = 0; i < actors->size(); i++)
	{
		if ((*actors)[i] == a)
		{
			(*actors)[i] = actors->back();
			actors->pop_back();
			break;
		}
	}
}

void RenderManager::render(SDL::MainClass* m)
{
	std::vector<SDL::Actor*>* actors;
	for (std::map < Shader*, std::vector<SDL::Actor*>>::iterator it = m_shaderActors.begin(); it != m_shaderActors.end(); it++)
	{
		actors = &it->second;
		it->first->bind();
		if (it->first->loadDefaultUniforms())
		{
			it->first->setUniformVec4("ambientLight", Lighting::ambientLight);
			it->first->setUniformVec3("eyePosition", m->getCamera()->getPosition());
			m->getLighting()->load(it->first, 3);
		}
		for (size_t i = 0; i < actors->size(); i++)
		{
			(*actors)[i]->setShader(it->first,false);
			(*actors)[i]->m_render_render();
		}
	}
}
