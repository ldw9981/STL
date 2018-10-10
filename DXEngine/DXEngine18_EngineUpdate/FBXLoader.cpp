#include "FBXLoader.h"

namespace FBXLoader
{
	HRESULT LoadFBX(const char * fileName, std::vector<Vertex>* pOutVertices, std::vector<DWORD>* pOutIndices)
	{
		// fbxManager�� �������� �ʾ����� ����.
		if (fbxManager == NULL)
		{
			// �ʱ� SDK Manager ����.
			fbxManager = FbxManager::Create();

			FbxIOSettings* pIOSettings = FbxIOSettings::Create(fbxManager, IOSROOT);
			fbxManager->SetIOSettings(pIOSettings);
		}

		FbxGeometryConverter gConverter(fbxManager);
		FbxImporter* importer
			= FbxImporter::Create(fbxManager, "");
		FbxScene* fbxScene
			= FbxScene::Create(fbxManager, "");

		// ������ �ʱ�ȭ.
		if (importer->Initialize(fileName, -1, fbxManager->GetIOSettings()) == false)
		{
			return E_FAIL;
		}

		// 3D �� ����Ʈ.
		if (importer->Import(fbxScene) == false)
		{
			return E_FAIL;
		}

		// ������ ����.
		importer->Destroy();

		// FBX Node �б�.
		FbxNode* fbxRootNode = fbxScene->GetRootNode();
		if (fbxRootNode == NULL)
			return E_FAIL;

		// �𵨸� ������ �⺻ ������ �ﰢ������ ����.
		gConverter.Triangulate(fbxScene, true);

		int childCount = fbxRootNode->GetChildCount();
		for (int ix = 0; ix < childCount; ++ix)
		{
			// �ڽ� ���� ��� �о����.
			FbxNode* fbxChildNode
				= fbxRootNode->GetChild(ix);

			// �Ӽ��� ���� ���� �н�.
			if (fbxChildNode->GetNodeAttribute() == NULL)
				continue;

			// ��� �Ӽ� Ÿ�� Ȯ��.
			FbxNodeAttribute::EType arrtType
				= fbxChildNode->GetNodeAttribute()->GetAttributeType();

			// ��� Ÿ���� �޽ð� �ƴ� ��� �н�.
			if (arrtType != FbxNodeAttribute::EType::eMesh)
				continue;

			// �޽� ���� �б�.
			FbxMesh* fbxMesh = fbxChildNode->GetMesh();

			// ��Ʈ�� ����Ʈ �迭 ������ (����).
			FbxVector4* vertices = fbxMesh->GetControlPoints();
			// ������ �� Ȯ��.
			int polyCount = fbxMesh->GetPolygonCount();
			int vertexCounter = 0;

			for (int jx = 0; jx < polyCount; ++jx)
			{
				// ������ ���� ���� ���� Ȯ��.
				int vertexCount = fbxMesh->GetPolygonSize(jx);

				for (int kx = 0; kx < vertexCount; ++kx)
				{
					// �ﰢ��(������) ��ȣ, ���� ��ȣ�� ���� ���� �б�.
					int vertexIndex = fbxMesh->GetPolygonVertex(jx, kx);

					// ���� ���� �б�.
					Vertex vertex;
					vertex.position.x = (float)vertices[vertexIndex].mData[0];
					vertex.position.y = (float)vertices[vertexIndex].mData[1];
					vertex.position.z = (float)vertices[vertexIndex].mData[2];

					// UV �б�.
					vertex.texCoord = ReadUV(fbxMesh, vertexIndex, vertexCounter);

					// ��� �б�.
					vertex.normal = ReadNormal(fbxMesh, vertexIndex, vertexCounter);

					// ź��Ʈ �б�.
					bool hasNoTangent
						= fbxMesh->GetElementTangentCount() < 1;
					if (hasNoTangent)
					{
						MessageBox(NULL, L"ź��Ʈ ����", L"����", MB_OK);
					}

					vertex.tangent = hasNoTangent ? XMFLOAT3(0.0f, 0.0, 0.0f) : ReadTangent(fbxMesh, vertexIndex, vertexCounter);

					// ���̳�� �б�.
					bool hasNoBinormal
						= fbxMesh->GetElementBinormalCount() < 1;
					if (hasNoBinormal)
						MessageBox(NULL, L"���̳�� ����", L"����", MB_OK);

					vertex.binormal = hasNoBinormal ? XMFLOAT3(0.0f, 0.0f, 0.0f) :
						ReadBinormal(fbxMesh, vertexIndex, vertexCounter);

					// ���� �迭�� ������ �߰�.
					pOutVertices->push_back(vertex);
					// �ε��� �迭�� ������ �߰�.
					pOutIndices->push_back(vertexCounter);
					++vertexCounter;
				}
			}
		}


		return S_OK;
	}

	XMFLOAT2 ReadUV(FbxMesh * mesh, int controlPointIndex, int vertexCounter)
	{
		// UV�� �ִ��� Ȯ��.
		if (mesh->GetElementUVCount() < 1)
		{
			MessageBox(NULL, L"UV�� �����ϴ�.", L"����", MB_OK);
			return NULL;
		}

		// ��ȯ�� ������ ����.
		XMFLOAT2 texCoord(0.0f, 0.0f);

		// UV ��ü �迭 �б�.
		FbxGeometryElementUV* vertexUV = mesh->GetElementUV(0);
		const bool isUsingIndex
			= vertexUV->GetReferenceMode() != FbxGeometryElement::eDirect;
		const int indexCount = isUsingIndex ? vertexUV->GetIndexArray().GetCount() : 0;

		// ��� Ȯ��.
		switch (vertexUV->GetMappingMode())
		{
			// ���� ������ ������ ������ ���.
		case FbxGeometryElement::eByControlPoint:
		{
			// ���� UV ���� �о�� �ε��� ������.
			int index = isUsingIndex ? vertexUV->GetIndexArray().GetAt(controlPointIndex) : controlPointIndex;

			/*if (isUsingIndex == true)
				index = vertexUV->GetIndexArray().GetAt(controlPointIndex);
			else
				index = controlPointIndex;*/

				// UV �� �о����. (DirectX�� FBX�� UV�� V��ǥ�� ���� �ݴ�.)
			texCoord.x = (float)vertexUV->GetDirectArray().GetAt(index).mData[0];
			texCoord.y = 1.0f - (float)vertexUV->GetDirectArray().GetAt(index).mData[1];

			// UV �� ��ȯ.
			return texCoord;
		}

		case FbxGeometryElement::eByPolygonVertex:
		{
			// ���� UV ���� �о�� �ε��� ������.
			int index = isUsingIndex ? vertexUV->GetIndexArray().GetAt(vertexCounter) : vertexCounter;

			// UV �� �о����. (DirectX�� FBX�� UV�� V��ǥ�� ���� �ݴ�.)
			texCoord.x = (float)vertexUV->GetDirectArray().GetAt(index).mData[0];
			texCoord.y = 1.0f - (float)vertexUV->GetDirectArray().GetAt(index).mData[1];

			// UV �� ��ȯ.
			return texCoord;
		}

		default:
		{
			MessageBox(NULL, L"UV ���� ��ȿ���� �ʽ��ϴ�", L"����", MB_OK);
			return NULL;
		}
		}

		return NULL;
	}

	XMFLOAT3 ReadNormal(FbxMesh * mesh, int controlPointIndex, int vertexCounter)
	{
		XMFLOAT3 normal(0.0f, 0.0f, 0.0f);

		// ����� �ִ��� Ȯ��.
		if (mesh->GetElementNormalCount() < 1)
		{
			MessageBox(NULL, L"����� �����ϴ�.", L"����", MB_OK);
			return NULL;
		}

		FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);
		const bool isUsingIndex
			= vertexNormal->GetReferenceMode() != FbxGeometryElement::eDirect;
		const int indexCount = isUsingIndex ? vertexNormal->GetIndexArray().GetCount() : 0;

		switch (vertexNormal->GetMappingMode())
		{
			// ���� ������ ������ ������ ���.
		case FbxGeometryElement::eByControlPoint:
		{
			// ���� ��� ���� �о�� �ε��� ������.
			int index = isUsingIndex ? vertexNormal->GetIndexArray().GetAt(controlPointIndex) : controlPointIndex;

			// ��� �� �о����. (DirectX�� FBX�� UV�� V��ǥ�� ���� �ݴ�.)
			normal.x = (float)vertexNormal->GetDirectArray().GetAt(index).mData[0];
			normal.y = (float)vertexNormal->GetDirectArray().GetAt(index).mData[1];
			normal.z = (float)vertexNormal->GetDirectArray().GetAt(index).mData[2];

			// ��� �� ��ȯ.
			return normal;
		}

		case FbxGeometryElement::eByPolygonVertex:
		{
			// ���� ��� ���� �о�� �ε��� ������.
			int index = isUsingIndex ? vertexNormal->GetIndexArray().GetAt(vertexCounter) : vertexCounter;

			// ��� �� �о����. (DirectX�� FBX�� UV�� V��ǥ�� ���� �ݴ�.)
			normal.x = (float)vertexNormal->GetDirectArray().GetAt(index).mData[0];
			normal.y = (float)vertexNormal->GetDirectArray().GetAt(index).mData[1];
			normal.z = (float)vertexNormal->GetDirectArray().GetAt(index).mData[2];

			// ��� �� ��ȯ.
			return normal;
		}

		default:
		{
			MessageBox(NULL, L"��� ���� ��ȿ���� �ʽ��ϴ�", L"����", MB_OK);
			return NULL;
		}
		}

		return NULL;
	}

	XMFLOAT3 ReadTangent(FbxMesh * mesh, int controlPointIndex, int vertexCounter)
	{
		// UV�� �ִ��� Ȯ��.
		if (mesh->GetElementTangentCount() < 1)
		{
			MessageBox(NULL, L"Tangent ���� ��ȿ���� �ʽ��ϴ�", L"����", MB_OK);
			return NULL;
		}

		// ���Ͽ� ���� ����.
		XMFLOAT3 tangent(0.0f, 0.0f, 0.0f);
		// UV ��ü �迭 ������.
		FbxGeometryElementTangent* vertexTangent = mesh->GetElementTangent(0);
		const bool isUsingIndex = vertexTangent->GetReferenceMode() != FbxGeometryElement::eDirect;
		const int indexCount = isUsingIndex ? vertexTangent->GetIndexArray().GetCount() : 0;

		switch (vertexTangent->GetMappingMode())
		{
			// UV �Ӽ��� ������ ������ ���.
		case FbxGeometryElement::eByControlPoint:
		{
			// ���� UV ���� �о�� �ε��� ������.
			int index = isUsingIndex ? vertexTangent->GetIndexArray().GetAt(controlPointIndex) : controlPointIndex;

			// UV �� �о����. (DirectX�� FBX�� UV V��ǥ�� ���� �ݴ�).
			tangent.x = (float)vertexTangent->GetDirectArray().GetAt(index).mData[0];
			tangent.y = (float)vertexTangent->GetDirectArray().GetAt(index).mData[1];
			tangent.z = (float)vertexTangent->GetDirectArray().GetAt(index).mData[2];

			// UV �� ��ȯ.
			return tangent;
		}

		// UV �Ӽ��� ���� ������ ���.
		case FbxGeometryElement::eByPolygonVertex:
		{
			// ���� UV ���� �о�� �ε��� ������. ���׿�����.
			int index = isUsingIndex == true ? vertexTangent->GetIndexArray().GetAt(vertexCounter) : vertexCounter;

			// UV �� �о����. (DirectX�� FBX�� UV V��ǥ�� ���� �ݴ�).
			tangent.x = (float)vertexTangent->GetDirectArray().GetAt(index).mData[0];
			tangent.y = (float)vertexTangent->GetDirectArray().GetAt(index).mData[1];
			tangent.z = (float)vertexTangent->GetDirectArray().GetAt(index).mData[2];

			// UV �� ��ȯ.
			return tangent;
		}

		default:
		{
			MessageBox(NULL, L"Tangent ���� ��ȿ���� �ʽ��ϴ�", L"����", MB_OK);
			return NULL;
		}
		}

		return NULL;
	}

	XMFLOAT3 ReadBinormal(FbxMesh * mesh, int controlPointIndex, int vertexCounter)
	{
		// ���̳���� �ִ��� Ȯ��.
		if (mesh->GetElementBinormalCount() < 1)
		{
			MessageBox(NULL, L"Binormal ���� ��ȿ���� �ʽ��ϴ�", L"����", MB_OK);
			return NULL;
		}

		// ���Ͽ� ���� ����.
		XMFLOAT3 binormal(0.0f, 0.0f, 0.0f);
		// UV ��ü �迭 ������.
		FbxGeometryElementBinormal* vertexBinormal = mesh->GetElementBinormal(0);
		const bool isUsingIndex = vertexBinormal->GetReferenceMode() != FbxGeometryElement::eDirect;
		const int indexCount = isUsingIndex ? vertexBinormal->GetIndexArray().GetCount() : 0;

		switch (vertexBinormal->GetMappingMode())
		{
			// UV �Ӽ��� ������ ������ ���.
		case FbxGeometryElement::eByControlPoint:
		{
			// ���� UV ���� �о�� �ε��� ������.
			int index = isUsingIndex ? vertexBinormal->GetIndexArray().GetAt(controlPointIndex) : controlPointIndex;

			// UV �� �о����. (DirectX�� FBX�� UV V��ǥ�� ���� �ݴ�).
			binormal.x = (float)vertexBinormal->GetDirectArray().GetAt(index).mData[0];
			binormal.y = (float)vertexBinormal->GetDirectArray().GetAt(index).mData[1];
			binormal.z = (float)vertexBinormal->GetDirectArray().GetAt(index).mData[2];

			// UV �� ��ȯ.
			return binormal;
		}

		// UV �Ӽ��� ���� ������ ���.
		case FbxGeometryElement::eByPolygonVertex:
		{
			// ���� UV ���� �о�� �ε��� ������. ���׿�����.
			int index = isUsingIndex == true ? vertexBinormal->GetIndexArray().GetAt(vertexCounter) : vertexCounter;

			// UV �� �о����. (DirectX�� FBX�� UV V��ǥ�� ���� �ݴ�).
			binormal.x = (float)vertexBinormal->GetDirectArray().GetAt(index).mData[0];
			binormal.y = (float)vertexBinormal->GetDirectArray().GetAt(index).mData[1];
			binormal.z = (float)vertexBinormal->GetDirectArray().GetAt(index).mData[2];

			// UV �� ��ȯ.
			return binormal;
		}

		default:
		{
			MessageBox(NULL, L"Binormal ���� ��ȿ���� �ʽ��ϴ�", L"����", MB_OK);
			return NULL;
		}
		}

		return NULL;
	}
}