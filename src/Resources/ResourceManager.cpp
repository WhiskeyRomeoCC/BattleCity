#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"

#include <sstream>
#include <fstream>
#include <iostream>

ResourceManager::ResourceManager(const std::string& executablePath)
{
	size_t found = executablePath.find_last_of("/\\");
	m_path = executablePath.substr(0, found);
}

std::string ResourceManager::getFileString(const std::string& relativePath) const
{
	std::ifstream f;
	f.open(m_path + "/" + relativePath.c_str(), std::ios::binary);
	if (!f.is_open())
	{
		std::cout << "Failed to open file: " << relativePath << std::endl;
		return std::string{};
	}

	std::stringstream buffer;
	buffer << f.rdbuf();
	return buffer.str();
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::loadShaders(const std::string& shaderName,
	const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
	std::string vertexStrring = getFileString(vertexShaderPath);
	if (vertexStrring.empty())
	{
		std::cerr << "No vertex shader!" << std::endl;
		return nullptr;
	}

	std::string fragmentStrring = getFileString(fragmentShaderPath);
	if (fragmentStrring.empty())
	{
		std::cerr << "No fragment shader!" << std::endl;
		return nullptr;
	}

	std::shared_ptr<Renderer::ShaderProgram>& newShader = m_shaderPrograms.emplace(shaderName,
		std::make_shared<Renderer::ShaderProgram>(vertexStrring, fragmentStrring)).first->second;
	if (newShader->isCompled())
	{
		return newShader;
	}

	std::cerr << "Can't load shader program:\n"
		<< "Vertex: " << vertexShaderPath << "\n"
		<< "Fragment:" << fragmentShaderPath << std::endl;
	
	return nullptr;
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::getShaderProgram(const std::string& shaderName)
{
	ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderName);
	if (it != m_shaderPrograms.end())
	{
		return it->second;
	}
	std::cerr << "Can't find the shader program: " << shaderName << std::endl;
	return nullptr;
}