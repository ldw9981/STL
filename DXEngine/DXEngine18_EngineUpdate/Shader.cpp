#include "Shader.h"



Shader::Shader()
{
}

Shader::Shader(LPCWSTR initFileName)
{
	fileName = initFileName;
	entryPoint = "main";
}

Shader::Shader(LPCWSTR initFileName, LPCSTR initEntry, LPCSTR initProfile)
{
	fileName = initFileName;
	entryPoint = initEntry;
	profile = initProfile;
}


Shader::~Shader()
{
}

void Shader::Release()
{
	Memory::SafeRelease(shaderBuffer);
}
