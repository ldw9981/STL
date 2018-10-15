#pragma once

#include "DXUtil.h"
#include "Mesh.h"

namespace FBXLoader
{
	//FbxManager* fbxManager;

	HRESULT LoadFBX(const char* fileName, std::vector<Vertex>* pOutVertices, std::vector<DWORD>* pOutIndices);

	// ���� UV �о���� �޼ҵ�.
	XMFLOAT2 ReadUV(FbxMesh* mesh, int controlPointIndex, int vertexCounter);

	// ���� ��� �о���� �޼ҵ�.
	XMFLOAT3 ReadNormal(FbxMesh* mesh, int controlPointIndex, int vertexCounter);

	// ���� ź��Ʈ �б�.
	XMFLOAT3 ReadTangent(FbxMesh* mesh, int controlPointIndex, int vertexCounter);

	// ���� ���̳�� �б�.
	XMFLOAT3 ReadBinormal(FbxMesh* mesh, int controlPointIndex, int vertexCounter);
};