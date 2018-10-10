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
	// 셰이더 변수.
	LPCWSTR fileName;	
	LPCSTR entryPoint;	// 영문전용
	LPCSTR profile;
	ID3DBlob* shaderBuffer; // 컴파일된 내용이 저장되는 버퍼

	virtual bool CompileShader() = 0;
	virtual bool CreateShader(ID3D11Device* device) = 0;
	virtual void BindShader(ID3D11DeviceContext* deviceContext) = 0;
	virtual void Release();
};

