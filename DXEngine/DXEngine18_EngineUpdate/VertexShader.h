#pragma once
#include "Shader.h"
class VertexShader :
	public Shader
{
public:
	VertexShader();
	VertexShader(LPCWSTR initFileName);
	VertexShader(LPCWSTR initFileName, LPCSTR initEntry, LPCSTR initProfile);

	~VertexShader();

	CBPerObject GetWVPMatricex() const { return cbMatrices;  }
	void SetWVPMatrices(XMMATRIX world, XMMATRIX view, XMMATRIX projection);

	CBLight GetLightInfo() const { return cbLight;  }
	void SetLightInfo(XMVECTOR lightPos, XMVECTOR cameraPos);

	virtual bool CompileShader() override;
	virtual bool CreateShader(ID3D11Device* device) override;
	virtual void BindShader(ID3D11DeviceContext* deviceContext) override;
	virtual void Release();
private:
	ID3D11VertexShader* vertexShader;
	CBPerObject cbMatrices;
	CBLight cbLight;
};

