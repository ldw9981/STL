#pragma once
#include "DXUtil.h"
#include "ShaderUtil.h"

class Shader
{
public:
	Shader();
	Shader(LPCWSTR initFileName);
	Shader(LPCWSTR initFileName,LPCSTR initEntry,LPCSTR initProfile);
	virtual ~Shader();

	void SetFileName(LPCWSTR setFileName)
	{
		fileName = setFileName;
	}

	LPCWSTR GetFileName() const { return fileName; }
	LPCSTR GetEntry() const { return entryPoint; }
	LPCSTR GetProfile() const { return profile; }
	ID3DBlob* GetShaderBuffer() const { return shaderBuffer; }
protected:
	// ���̴� ����.
	LPCWSTR fileName;	
	LPCSTR entryPoint;	// ��������
	LPCSTR profile;
	ID3DBlob* shaderBuffer; // �����ϵ� ������ ����Ǵ� ����

	virtual bool CompileShader() = 0;
	virtual bool CreateShader(ID3D11Device* device) = 0;
	virtual void BindShader(ID3D11DeviceContext* deviceContext) = 0;
	virtual void Release();
};

