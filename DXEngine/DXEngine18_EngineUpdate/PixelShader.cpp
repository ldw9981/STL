#include "PixelShader.h"



PixelShader::PixelShader()
{
}

PixelShader::PixelShader(LPCWSTR initFileName) :Shader(initFileName)
{
	profile = "ps_5_0";
}

PixelShader::PixelShader(LPCWSTR initFileName, LPCSTR initEntry, LPCSTR initProfile)
	: Shader(initFileName, initEntry, initProfile)
{

}


PixelShader::~PixelShader()
{
}

bool PixelShader::CompileShader()
{
	if (!ShaderUtil::CompileShader(fileName, entryPoint, profile, &shaderBuffer))
	{
		return false;
	}
	return true;
}

bool PixelShader::CreateShader(ID3D11Device * device)
{
	if(!ShaderUtil::CreatePixelShader(device, shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, &pixelShader))
	{
		return false;
	}
	return true;
}

void PixelShader::BindShader(ID3D11DeviceContext * deviceContext)
{
	ShaderUtil::BindPixelShader(deviceContext, pixelShader, NULL, NULL);
}

void PixelShader::Release()
{
	Shader::Release();
	Memory::SafeRelease(pixelShader);
}

bool PixelShader::CreateSamplerState(ID3D11Device * device)
{
	// ���÷� ������Ʈ.
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// ���÷� ������Ʈ ����.
	HRESULT hr = device->CreateSamplerState(&samplerDesc, &samplerState);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"���÷� ������Ʈ ���� ����", L"����", MB_OK);
		return false;
	}
	return false;
}

void PixelShader::BindSamplerState(ID3D11DeviceContext * deviceContext)
{
	deviceContext->PSSetSamplers(0, 1, &samplerState);
}

void PixelShader::LoadTextures(ID3D11Device * device)
{
	for (size_t i = 0; i < textures.size(); i++)
	{
		// �ؽ�ó ���� �ε�.
		HRESULT hr = D3DX11CreateShaderResourceViewFromFile(
				device,textures[i].fileName, NULL, NULL, &textures[i].textureResource, NULL);
		if (FAILED(hr))
		{
			MessageBox(NULL, L"�ؽ�ó �ε� ����", L"����", MB_OK);
			return;
		}
	}
	
}

void PixelShader::BindTextures(ID3D11DeviceContext * deviceContext)
{
	int size = textures.size();
	for (size_t i = 0; i < size; i++)
	{
		deviceContext->PSSetShaderResources(i, 1, &textures[i].textureResource);
	}
}
