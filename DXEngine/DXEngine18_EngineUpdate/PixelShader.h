#pragma once
#include "Shader.h"
#include <vector>
class PixelShader :
	public Shader
{
public:
	PixelShader();
	PixelShader(LPCWSTR initFileName);
	PixelShader(LPCWSTR initFileName, LPCSTR initEntry, LPCSTR initProfile);

	~PixelShader();

	virtual bool CompileShader() override;
	virtual bool CreateShader(ID3D11Device* device) override;
	virtual void BindShader(ID3D11DeviceContext* deviceContext) override;
	virtual void Release() override;

	bool CreateSamplerState(ID3D11Device* device);
	void BindSamplerState(ID3D11DeviceContext* deviceContext);
	void LoadTextures(ID3D11Device* device);
	void BindTextures(ID3D11DeviceContext* deviceContext);

	//
	ID3D11PixelShader* GetPixelShader() const { return pixelShader;  }
	std::vector<Texture> GetTextureArray() const { return textures;  }
	void AddTexture(Texture newTexture) { textures.push_back(newTexture); }
private:
	ID3D11PixelShader* pixelShader;
	ID3D11SamplerState* samplerState;

	//texture
	std::vector<Texture> textures;
};


struct Texture
{
public:
	Texture() { ZeroMemory(this, sizeof(Texture)); }
	~Texture() { Memory::SafeRelease(textureResource); }

	LPCWSTR fileName;
	ID3D11ShaderResourceView* textureResource = NULL;

};
