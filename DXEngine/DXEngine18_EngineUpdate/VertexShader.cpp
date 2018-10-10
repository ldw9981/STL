#include "VertexShader.h"
#include "Shader.h"


VertexShader::VertexShader()
{
}

VertexShader::VertexShader(LPCWSTR initFileName):Shader(initFileName)
{
	profile = "vs_5_0";
}

VertexShader::VertexShader(LPCWSTR initFileName, LPCSTR initEntry, LPCSTR initProfile)
	:Shader(initFileName, initEntry, initProfile)
{

}


VertexShader::~VertexShader()
{
}

void VertexShader::SetWVPMatrices(XMMATRIX world, XMMATRIX view, XMMATRIX projection)
{
	cbMatrices.world = XMMatrixTranspose(world);
	cbMatrices.view = XMMatrixTranspose(view);
	cbMatrices.projection = XMMatrixTranspose(projection);
	
}

void VertexShader::SetLightInfo(XMVECTOR lightPos, XMVECTOR cameraPos)
{
	cbLight.cameraPosition = cameraPos;
	cbLight.lightPosition = lightPos;
}

bool VertexShader::CompileShader()
{
	if (!ShaderUtil::CompileShader(fileName, entryPoint, profile, &shaderBuffer))
	{
		return false;
	}
	return true;
}

bool VertexShader::CreateShader(ID3D11Device * device)
{
	if (!ShaderUtil::CreateVertexShader(device,shaderBuffer->GetBufferPointer(),shaderBuffer->GetBufferSize(),NULL, &vertexShader))
	{
		return false;
	}
	return true;
}

void VertexShader::BindShader(ID3D11DeviceContext * deviceContext)
{
	ShaderUtil::BindVertexShader(deviceContext, vertexShader, NULL, NULL);
}

void VertexShader::Release()
{
	Shader::Release();
	Memory::SafeRelease(vertexShader);
}
