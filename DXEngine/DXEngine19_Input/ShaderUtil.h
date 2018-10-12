#pragma once

#include "DXUtil.h"

namespace ShaderUtil
{
	//class Shader;

	// ������.
	//bool CompilShader(Shader shader);
	bool CompileShader(LPCWSTR fileName, LPCSTR entry, LPCSTR profile, ID3DBlob** pOutShaderBuffer);

	// ���ؽ� ���̴� ����.
	bool CreateVertexShader(
		ID3D11Device* device, 
		const void* pointer, 
		SIZE_T size, 
		ID3D11ClassLinkage* linkage, 
		ID3D11VertexShader** ppVertexShader);

	// ���ؽ� ���̴� ���ε�.
	void BindVertexShader(
		ID3D11DeviceContext* deviceContext,
		ID3D11VertexShader *pVertexShader,
		ID3D11ClassInstance *const *ppClassInstances,
		UINT NumClassInstances);

	// �ȼ� ���̴� ����.
	bool CreatePixelShader(
		ID3D11Device* device,
		const void* pointer,
		SIZE_T size,
		ID3D11ClassLinkage* linkage,
		ID3D11PixelShader** ppPixelShader);

	// �ȼ� ���̴� ���ε�.
	void BindPixelShader(
		ID3D11DeviceContext* deviceContext,
		ID3D11PixelShader *pPixelShader,
		ID3D11ClassInstance *const *ppClassInstances,
		UINT NumClassInstances
	);
};

