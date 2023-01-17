#include "FBXLoader.h"

CFBXLoader::CFBXLoader()	:
	m_Manager(nullptr),
	m_Scene(nullptr),
	m_Mixamo(false)
{
}

CFBXLoader::~CFBXLoader()
{
	size_t	Size = m_vecMaterial.size();

	for (size_t i = 0; i < Size; ++i)
	{
		size_t	Size1 = m_vecMaterial[i].size();

		for (size_t j = 0; j < Size1; ++j)
		{
			SAFE_DELETE(m_vecMaterial[i][j]);
		}
	}

	Size = m_vecMeshContainer.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecMeshContainer[i]);
	}

	Size = m_vecBone.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecBone[i]);
	}

	for (int i = 0; i < m_NameArray.Size(); ++i)
	{
		m_NameArray[i]->Clear();
		SAFE_DELETE(m_NameArray[i]);
	}

	Size = m_vecClip.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecClip[i]);
	}

	if (m_Scene)
	{
		m_Scene->Clear();
		m_Scene->Destroy();
	}

	if (m_Manager)
		m_Manager->Destroy();
}

bool CFBXLoader::LoadFBX(const char* FullPath, bool Static)
{
	m_Manager = FbxManager::Create();

	FbxIOSettings* IOS = FbxIOSettings::Create(m_Manager,
		IOSROOT);

	m_Manager->SetIOSettings(IOS);

	m_Scene = FbxScene::Create(m_Manager, "");

	FbxImporter* Importer = FbxImporter::Create(m_Manager, "");

	Importer->Initialize(FullPath, -1, m_Manager->GetIOSettings());

	Importer->Import(m_Scene);

	if (m_Scene->GetGlobalSettings().GetAxisSystem() != FbxAxisSystem::Max)
		m_Scene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::Max);

	if (!Static)
	{
		m_Scene->FillAnimStackNameArray(m_NameArray);

		if (m_NameArray.GetCount() > 0)
		{
			LoadAnimationClip();

			// �� ������ �о�´�.
			LoadBone(m_Scene->GetRootNode());

			// Ŭ���� ������ �ִ� �������� �� ������ŭ resize ���ش�.
			// ���� Animation Clip���� �ϴ����̴�.
			for (size_t i = 0; i < m_vecClip.size(); ++i)
			{
				m_vecClip[i]->vecBoneKeyFrame.resize(m_vecBone.size());
			}
		}
	}

	Triangulate(m_Scene->GetRootNode());

	LoadMesh(m_Scene->GetRootNode(), Static);

	Importer->Destroy();

	return true;
}

void CFBXLoader::Triangulate(FbxNode* Node)
{
	// FbxNodeAttribute : FbxNode�� �Ӽ��� ��Ÿ����.
	FbxNodeAttribute* Attr = Node->GetNodeAttribute();

	if (Attr && (Attr->GetAttributeType() == FbxNodeAttribute::eMesh ||
		Attr->GetAttributeType() == FbxNodeAttribute::eNurbs ||
		Attr->GetAttributeType() == FbxNodeAttribute::eNurbsSurface))
	{
		FbxGeometryConverter	Converter(m_Manager);

		Converter.Triangulate(Attr, true);
	}

	// ���� ������ �д´�.
	int	MtrlCount = Node->GetMaterialCount();

	if (MtrlCount > 0)
	{
		std::vector<FbxMaterial*>	vecMtrl;

		m_vecMaterial.push_back(vecMtrl);

		for (int i = 0; i < MtrlCount; ++i)
		{
			FbxSurfaceMaterial* Mtrl = Node->GetMaterial(i);

			if (!Mtrl)
				continue;

			LoadMaterial(Mtrl);
		}
	}

	int	ChildCount = Node->GetChildCount();

	for (int i = 0; i < ChildCount; ++i)
	{
		Triangulate(Node->GetChild(i));
	}
}

void CFBXLoader::LoadMaterial(FbxSurfaceMaterial* Mtrl)
{
	// ���� ������ ������ ����ü�� �����Ѵ�.
	FbxMaterial*	MtrlInfo = new FbxMaterial;

	// ���� ������ �����̳ʿ� ���� ������ �߰��Ѵ�.
	m_vecMaterial[m_vecMaterial.size() - 1].push_back(MtrlInfo);

	// Diffuse ������ �о�´�.
	MtrlInfo->BaseColor = GetMaterialColor(Mtrl, FbxSurfaceMaterial::sDiffuse,
		FbxSurfaceMaterial::sDiffuseFactor);

	MtrlInfo->AmbientColor = GetMaterialColor(Mtrl, FbxSurfaceMaterial::sAmbient,
		FbxSurfaceMaterial::sAmbientFactor);

	MtrlInfo->SpecularColor = GetMaterialColor(Mtrl, FbxSurfaceMaterial::sSpecular,
		FbxSurfaceMaterial::sSpecularFactor);

	MtrlInfo->EmissiveColor = GetMaterialColor(Mtrl, FbxSurfaceMaterial::sEmissive,
		FbxSurfaceMaterial::sEmissiveFactor);

	MtrlInfo->SpecularPower = (float)GetMaterialFactor(Mtrl, FbxSurfaceMaterial::sSpecularFactor);
	MtrlInfo->Shininess = (float)GetMaterialFactor(Mtrl, FbxSurfaceMaterial::sShininess);
	MtrlInfo->TransparencyFactor = (float)GetMaterialFactor(Mtrl, FbxSurfaceMaterial::sTransparencyFactor);

	MtrlInfo->DiffuseTexture = GetMaterialTexture(Mtrl, FbxSurfaceMaterial::sDiffuse);
	MtrlInfo->BumpTexture = GetMaterialTexture(Mtrl, FbxSurfaceMaterial::sNormalMap);

	if (MtrlInfo->BumpTexture.empty())
		MtrlInfo->BumpTexture = GetMaterialTexture(Mtrl, FbxSurfaceMaterial::sBump);

	MtrlInfo->SpecularTexture = GetMaterialTexture(Mtrl, FbxSurfaceMaterial::sSpecular);
}

Vector4 CFBXLoader::GetMaterialColor(FbxSurfaceMaterial* Mtrl, const char* PropertyName, const char* PropertyFactorName)
{
	FbxDouble3	Result(0, 0, 0);
	double		Factor = 0;

	// ���ڷ� ���� �������� �ش� �̸��� ���� ������Ƽ�� �ִ����� �Ǵ��Ѵ���
	// ���´�. ���������� �ش� �̸��� ������Ƽ ���Ͱ� �ִ����� �Ǵ��Ѵ���
	// ���´�.
	FbxProperty	Property = Mtrl->FindProperty(PropertyName);
	FbxProperty	PropertyFactor = Mtrl->FindProperty(PropertyFactorName);

	// ��ȿ���� üũ�Ѵ�.
	if (Property.IsValid() && PropertyFactor.IsValid())
	{
		Result = Property.Get<FbxDouble3>();
		Factor = PropertyFactor.Get<FbxDouble>();

		// Factor�� 1�� �ƴ� ��� ������ ���������� ����Ͽ�
		// ���� vResult �� ��� �����ش�.
		if (Factor != 1)
		{
			Result[0] *= Factor;
			Result[1] *= Factor;
			Result[2] *= Factor;
		}
	}

	return Vector4((float)Result[0], (float)Result[1], (float)Result[2], (float)Factor);
}

double CFBXLoader::GetMaterialFactor(FbxSurfaceMaterial* Mtrl, 
	const char* PropertyName)
{
	FbxProperty	Property = Mtrl->FindProperty(PropertyName);
	double	Factor = 0.0;

	if (Property.IsValid())
		Factor = Property.Get<FbxDouble>();

	return Factor;
}

std::string CFBXLoader::GetMaterialTexture(
	FbxSurfaceMaterial* Mtrl, const char* PropertyName)
{
	FbxProperty	Property = Mtrl->FindProperty(PropertyName);

	std::string	str = "";

	if (Property.IsValid())
	{
		// FbxFileTexture �� Ÿ���� ��� �ִ����� ������ �Լ��̴�.
		int	TexCount = Property.GetSrcObjectCount<FbxFileTexture>();

		if (TexCount > 0)
		{
			FbxFileTexture* FileTex = Property.GetSrcObject<FbxFileTexture>(0);

			if (FileTex)
				str = FileTex->GetFileName();
		}
	}

	return str;
}

void CFBXLoader::LoadMesh(FbxNode* Node, bool Static)
{
	FbxNodeAttribute* Attr = Node->GetNodeAttribute();

	if (Attr && Attr->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		FbxMesh* Mesh = Node->GetMesh();

		if (Mesh)
			LoadMesh(Mesh, Static);
	}

	int	ChildCount = Node->GetChildCount();

	for (int i = 0; i < ChildCount; ++i)
	{
		LoadMesh(Node->GetChild(i), Static);
	}
}

void CFBXLoader::LoadMesh(FbxMesh* Mesh, bool Static)
{
	// MeshContainer�� ����� �����̳ʺ� ������ �����̳�&����� ��
	// �ε��� ������ ���ͼ� �޽��� �����ؾ� �Ѵ�.
	FbxMeshContainer* Container = new FbxMeshContainer;

	m_vecMeshContainer.push_back(Container);

	// ControlPoint �� ��ġ������ ��� �ִ� �迭�̴�.
	// �� �迭�� ������ �� ������ ������ �ȴ�.
	int	VtxCount = Mesh->GetControlPointsCount();

	// ���������� FbxVector4Ÿ���� �迭�� �����ϰ� �ֱ� ������ �迭�� 
	// �����ּҸ� ���´�.
	FbxVector4* VtxPos = Mesh->GetControlPoints();

	// �����̳ʰ� ������ �ִ� ���� �������� ��������ŭ resize ���ش�.
	Container->vecPos.resize(VtxCount);
	Container->vecNormal.resize(VtxCount);
	Container->vecUV.resize(VtxCount);
	Container->vecTangent.resize(VtxCount);
	Container->vecBinormal.resize(VtxCount);

	for (int i = 0; i < VtxCount; ++i)
	{
		// y�� z���� �ٲ�� �ֱ� ������ �������ش�.
		Container->vecPos[i].x = (float)VtxPos[i].mData[0];
		Container->vecPos[i].y = (float)VtxPos[i].mData[2];
		Container->vecPos[i].z = (float)VtxPos[i].mData[1];
	}

	// ������ ���� ���´�.
	int	PolygonCount = Mesh->GetPolygonCount();

	UINT	VtxID = 0;

	// ���� ���� ���´�.(���� �� == ����� ��)
	int	MtrlCount = Mesh->GetNode()->GetMaterialCount();

	MtrlCount = MtrlCount == 0 ? 1 : MtrlCount;

	// ���� ���� �� ����� ���̱� ������ ���� ����ŭ resize �Ѵ�.
	Container->vecIndex.resize(MtrlCount);

	// ���� ������ ���´�.
	FbxGeometryElementMaterial* Material = Mesh->GetElementMaterial();
	int iCount = Mesh->GetElementMaterialCount();
	// �ﰢ�� ����ŭ �ݺ��Ѵ�.
	for (int i = 0; i < PolygonCount; ++i)
	{
		// �� �������� �����ϴ� ������ ���� ���´�.
		// �ﰢ���� ��� 3���� ���´�.
		int	PolygonSize = Mesh->GetPolygonSize(i);

		int	Idx[3] = {};

		for (int j = 0; j < PolygonSize; ++j)
		{
			// ���� �ﰢ���� �����ϰ� �ִ� ���ؽ����� �������� �ε�����
			// ���Ѵ�.
			int	ControlIndex = Mesh->GetPolygonVertex(i, j);

			Idx[j] = ControlIndex;

			LoadNormal(Mesh, Container, VtxID, ControlIndex);

			LoadUV(Mesh, Container, Mesh->GetTextureUVIndex(i, j),
				ControlIndex);

			LoadTangent(Mesh, Container, VtxID, ControlIndex);

			LoadBinormal(Mesh, Container, VtxID, ControlIndex);

			++VtxID;
		}

		// �ﰢ���� �Ҽӵ� ����� �ε����� ���´�.
		int	MtrlID = 0;

		if (Material)
			MtrlID = Material->GetIndexArray().GetAt(i);

		Container->vecIndex[MtrlID].push_back(Idx[0]);
		Container->vecIndex[MtrlID].push_back(Idx[2]);
		Container->vecIndex[MtrlID].push_back(Idx[1]);
	}

	if (!Static)
		LoadAnimation(Mesh, Container);
}

void CFBXLoader::LoadNormal(FbxMesh* Mesh,
	FbxMeshContainer* Container, int VtxID, int ControlIndex)
{
	// �޽��κ��� ElementNormal ������ ���´�.
	FbxGeometryElementNormal* Normal = Mesh->GetElementNormal();

	int	NormalIndex = VtxID;

	// MappingMode�� ReferenceMode�� ���� �ε����� ����� ������ �޶�����.
	if (Normal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (Normal->GetReferenceMode())
		{
		case FbxGeometryElement::eIndexToDirect:
			NormalIndex = Normal->GetIndexArray().GetAt(VtxID);
			break;
		}
	}

	else if (Normal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		switch (Normal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			NormalIndex = ControlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			NormalIndex = Normal->GetIndexArray().GetAt(ControlIndex);
			break;
		}
	}

	FbxVector4	vNormal = Normal->GetDirectArray().GetAt(NormalIndex);

	Container->vecNormal[ControlIndex].x = (float)vNormal.mData[0];
	Container->vecNormal[ControlIndex].y = (float)vNormal.mData[2];
	Container->vecNormal[ControlIndex].z = (float)vNormal.mData[1];
}

void CFBXLoader::LoadUV(FbxMesh* Mesh,
	FbxMeshContainer* pContainer, int UVID, int ControlIndex)
{
	int Count = Mesh->GetElementUVCount();
	FbxGeometryElementUV* UV = Mesh->GetElementUV(0);

	if (!UV)
		return;

	int	UVIndex = UVID;

	if (UV->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		switch (UV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			UVIndex = ControlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			UVIndex = UV->GetIndexArray().GetAt(ControlIndex);
			break;
		}
	}

	else if (UV->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (UV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		case FbxGeometryElement::eIndexToDirect:
			break;
		}
	}

	FbxVector2	vUV = UV->GetDirectArray().GetAt(UVIndex);

	pContainer->vecUV[ControlIndex].x = (float)vUV.mData[0] - (int)vUV.mData[0];
	pContainer->vecUV[ControlIndex].y = 1.f - (float)(vUV.mData[1] - (int)vUV.mData[1]);
}

void CFBXLoader::LoadTangent(FbxMesh* Mesh,
	FbxMeshContainer* Container, int VtxID, int ControlIndex)
{
	// �޽��κ��� ElementTangent ������ ���´�.
	FbxGeometryElementTangent* Tangent = Mesh->GetElementTangent();

	if (!Tangent)
		return;

	Container->Bump = true;

	int	TangentIndex = VtxID;

	// MappingMode�� ReferenceMode�� ���� �ε����� ����� ������ �޶�����.
	if (Tangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (Tangent->GetReferenceMode())
		{
		case FbxGeometryElement::eIndexToDirect:
			TangentIndex = Tangent->GetIndexArray().GetAt(VtxID);
			break;
		}
	}

	else if (Tangent->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		switch (Tangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			TangentIndex = ControlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			TangentIndex = Tangent->GetIndexArray().GetAt(ControlIndex);
			break;
		}
	}

	FbxVector4	vTangent = Tangent->GetDirectArray().GetAt(TangentIndex);

	Container->vecTangent[ControlIndex].x = (float)vTangent.mData[0];
	Container->vecTangent[ControlIndex].y = (float)vTangent.mData[2];
	Container->vecTangent[ControlIndex].z = (float)vTangent.mData[1];
}

void CFBXLoader::LoadBinormal(FbxMesh* Mesh,
	FbxMeshContainer* Container, int VtxID, int ControlIndex)
{
	// �޽��κ��� ElementBinormal ������ ���´�.
	FbxGeometryElementBinormal* Binormal = Mesh->GetElementBinormal();

	if (!Binormal)
		return;

	Container->Bump = true;

	int	BinormalIndex = VtxID;

	// MappingMode�� ReferenceMode�� ���� �ε����� ����� ������ �޶�����.
	if (Binormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (Binormal->GetReferenceMode())
		{
		case FbxGeometryElement::eIndexToDirect:
			BinormalIndex = Binormal->GetIndexArray().GetAt(VtxID);
			break;
		}
	}

	else if (Binormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		switch (Binormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			BinormalIndex = ControlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			BinormalIndex = Binormal->GetIndexArray().GetAt(ControlIndex);
			break;
		}
	}

	FbxVector4	vBinormal = Binormal->GetDirectArray().GetAt(BinormalIndex);

	Container->vecBinormal[ControlIndex].x = (float)vBinormal.mData[0];
	Container->vecBinormal[ControlIndex].y = (float)vBinormal.mData[2];
	Container->vecBinormal[ControlIndex].z = (float)vBinormal.mData[1];
}

void CFBXLoader::LoadAnimationClip()
{
	int	Count = m_NameArray.GetCount();

	FbxTime::EMode	TimeMode = m_Scene->GetGlobalSettings().GetTimeMode();

	for (int i = 0; i < Count; ++i)
	{
		// m_NameArr�� ����� �̸����� Scene���κ��� FbxAnimStack ��ü�� ���´�.
		FbxAnimStack* AnimStack = m_Scene->FindMember<FbxAnimStack>(m_NameArray[i]->Buffer());

		if (!AnimStack)
			continue;

		FbxAnimationClip*	Clip = new FbxAnimationClip;

		Clip->Name = AnimStack->GetName();

		if (Clip->Name == "mixamo.com")
			m_Mixamo = true;

		FbxTakeInfo* Take = m_Scene->GetTakeInfo(Clip->Name.c_str());

		Clip->StartTime = Take->mLocalTimeSpan.GetStart();
		Clip->EndTime = Take->mLocalTimeSpan.GetStop();
		// GetFrameCount �Լ��� ȣ���ϰ�  time��带 �־��ָ� �ð��� ����������
		// ��ȯ���ش�. �������� ¥�� �ִϸ��̼� ������ �����ش�.
		Clip->TimeLength = Clip->EndTime.GetFrameCount(TimeMode) -
			Clip->StartTime.GetFrameCount(TimeMode);
		Clip->TimeMode = TimeMode;

		m_vecClip.push_back(Clip);
	}
}

void CFBXLoader::LoadBone(FbxNode* Node)
{
	int ChildCount = Node->GetChildCount();

	for (int i = 0; i < ChildCount; ++i)
	{
		LoadBoneRecursive(Node->GetChild(i), 0, 0, -1);
	}
}

void CFBXLoader::LoadBoneRecursive(FbxNode* Node, int Depth,
	int Index, int Parent)
{
	FbxNodeAttribute* Attr = Node->GetNodeAttribute();

	if (Attr && Attr->GetAttributeType() ==
		FbxNodeAttribute::eSkeleton)
	{
		FbxBone*	Bone = new FbxBone;

		Bone->Name = Node->GetName();
		if (m_Mixamo)
			Bone->Name.erase(0, 10);
		Bone->Depth = Depth;
		Bone->ParentIndex = Parent;

		m_vecBone.push_back(Bone);
	}

	int	ChildCount = Node->GetChildCount();

	for (int i = 0; i < ChildCount; ++i)
	{
		LoadBoneRecursive(Node->GetChild(i), Depth + 1,
			(int)m_vecBone.size(), Index);
	}
}

void CFBXLoader::LoadAnimation(FbxMesh* Mesh, 
	FbxMeshContainer* Container)
{
	int	SkinCount = Mesh->GetDeformerCount(FbxDeformer::eSkin);

	if (SkinCount <= 0)
		return;

	// �޽��� ���� ���� ���´�.
	int	CPCount = Mesh->GetControlPointsCount();

	// ������ ����ġ ������ ���ε��� ������ ���� ����ŭ
	// ��������� �Ѵ�.
	Container->vecBlendWeight.resize(CPCount);
	Container->vecBlendIndex.resize(CPCount);

	Container->Animation = true;

	FbxAMatrix	matTransform = GetTransform(Mesh->GetNode());

	for (int i = 0; i < SkinCount; ++i)
	{
		FbxSkin* Skin = (FbxSkin*)Mesh->GetDeformer(i, FbxDeformer::eSkin);

		if (!Skin)
			continue;

		FbxSkin::EType	SkinningType = Skin->GetSkinningType();

		if (SkinningType == FbxSkin::eRigid ||
			SkinningType == FbxSkin::eLinear ||
			SkinningType == FbxSkin::eBlend)
		{
			// Cluster : ������ �ǹ��Ѵ�.
			int	ClusterCount = Skin->GetClusterCount();

			for (int j = 0; j < ClusterCount; ++j)
			{
				FbxCluster* Cluster = Skin->GetCluster(j);

				if (!Cluster->GetLink())
					continue;

				std::string	BoneName = Cluster->GetLink()->GetName();

				if (m_Mixamo)
					BoneName.erase(0, 10);

				int	BoneIndex = FindBoneFromName(BoneName);

				LoadWeightAndIndex(Cluster, BoneIndex, Container);

				LoadOffsetMatrix(Cluster, matTransform, BoneIndex, Container);

				m_vecBone[BoneIndex]->matBone = matTransform;

				LoadTimeTransform(Mesh->GetNode(), Cluster,
					matTransform, BoneIndex);
			}
		}
	}

	ChangeWeightAndIndices(Container);
}

FbxAMatrix CFBXLoader::GetTransform(FbxNode* Node)
{
	const FbxVector4	vT = Node->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4	vR = Node->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4	vS = Node->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(vT, vR, vS);
}

int CFBXLoader::FindBoneFromName(const std::string& Name)
{
	for (size_t i = 0; i < m_vecBone.size(); ++i)
	{
		if (m_vecBone[i]->Name == Name)
			return (int)i;
	}

	return -1;
}

void CFBXLoader::LoadWeightAndIndex(FbxCluster* Cluster, 
	int BoneIndex, FbxMeshContainer* Container)
{
	int	ControlIndicesCount = Cluster->GetControlPointIndicesCount();

	for (int i = 0; i < ControlIndicesCount; ++i)
	{
		FbxWeight	Weight = {};

		Weight.Index = BoneIndex;
		Weight.Weight = Cluster->GetControlPointWeights()[i];

		int	ClusterIndex = Cluster->GetControlPointIndices()[i];

		// map�� Ư¡ : Ű�� �̿��� �ε���ó�� ������ ��� �ش� Ű�� ���ٸ�
		// ������ش�.
		Container->mapWeights[ClusterIndex].push_back(Weight);
	}
}

void CFBXLoader::LoadOffsetMatrix(FbxCluster* Cluster,
	const FbxAMatrix& matTransform, int BoneIndex,
	FbxMeshContainer* Container)
{
	FbxAMatrix	matCluster;
	FbxAMatrix	matClusterLink;

	Cluster->GetTransformMatrix(matCluster);
	Cluster->GetTransformLinkMatrix(matClusterLink);

	FbxVector4	v1 = { 1.0, 0.0, 0.0, 0.0 };
	FbxVector4	v2 = { 0.0, 0.0, 1.0, 0.0 };
	FbxVector4	v3 = { 0.0, 1.0, 0.0, 0.0 };
	FbxVector4	v4 = { 0.0, 0.0, 0.0, 1.0 };

	FbxAMatrix	matReflect;
	matReflect.mData[0] = v1;
	matReflect.mData[1] = v2;
	matReflect.mData[2] = v3;
	matReflect.mData[3] = v4;

	/*
	1 0 0 0   1 2 3 4   1 0 0 0
	0 0 1 0   5 6 7 8   0 0 1 0
	0 1 0 0 * 9 0 1 2 * 0 1 0 0
	0 0 0 1   3 4 5 6   0 0 0 1

	1 2 3 4   1 0 0 0
	9 0 1 2   0 0 1 0
	5 6 7 8 * 0 1 0 0
	3 4 5 6   0 0 0 1

	1 3 2 4
	9 1 0 2
	5 7 6 8
	3 5 4 6
	*/

	FbxAMatrix	matOffset;
	matOffset = matClusterLink.Inverse() * matCluster * matTransform;
	matOffset = matReflect * matOffset * matReflect;

	m_vecBone[BoneIndex]->matOffset = matOffset;
}

void CFBXLoader::LoadTimeTransform(FbxNode* Node, 
	FbxCluster* Cluster, const FbxAMatrix& matTransform,
	int BoneIndex)
{
	FbxVector4	v1 = { 1.0, 0.0, 0.0, 0.0 };
	FbxVector4	v2 = { 0.0, 0.0, 1.0, 0.0 };
	FbxVector4	v3 = { 0.0, 1.0, 0.0, 0.0 };
	FbxVector4	v4 = { 0.0, 0.0, 0.0, 1.0 };

	FbxAMatrix	matReflect;
	matReflect.mData[0] = v1;
	matReflect.mData[1] = v2;
	matReflect.mData[2] = v3;
	matReflect.mData[3] = v4;

	if (m_Mixamo)
	{
		std::vector<FbxAnimationClip*>::iterator	iter;
		std::vector<FbxAnimationClip*>::iterator	iterEnd = m_vecClip.end();

		for (iter = m_vecClip.begin(); iter != iterEnd;)
		{
			if ((*iter)->Name != "mixamo.com")
			{
				SAFE_DELETE((*iter));
				iter = m_vecClip.erase(iter);
				iterEnd = m_vecClip.end();
			}

			else
				++iter;
		}
	}

	for (size_t i = 0; i < m_vecClip.size(); ++i)
	{
		FbxLongLong	Start = m_vecClip[i]->StartTime.GetFrameCount(m_vecClip[i]->TimeMode);
		FbxLongLong	End = m_vecClip[i]->EndTime.GetFrameCount(m_vecClip[i]->TimeMode);

		m_vecClip[i]->vecBoneKeyFrame[BoneIndex].BoneIndex = BoneIndex;

		// ��ü ������ ����ŭ �ݺ��Ѵ�.
		for (FbxLongLong j = Start; j <= End; ++j)
		{
			FbxTime	Time = {};

			// ���� �����ӿ� �ش��ϴ� FbxTime�� ������.
			Time.SetFrame(j, m_vecClip[i]->TimeMode);

			// EvaluateGlobalTransform
			FbxAMatrix	matOffset = Node->EvaluateGlobalTransform(Time) * matTransform;
			FbxAMatrix	matCur = matOffset.Inverse() * Cluster->GetLink()->EvaluateGlobalTransform(Time);

			matCur = matReflect * matCur * matReflect;

			FbxKeyFrame	KeyFrame = {};

			KeyFrame.Time = Time.GetSecondDouble();
			KeyFrame.matTransform = matCur;

			m_vecClip[i]->vecBoneKeyFrame[BoneIndex].vecKeyFrame.push_back(KeyFrame);
		}
	}
}

void CFBXLoader::ChangeWeightAndIndices(FbxMeshContainer* Container)
{
	std::unordered_map<int, std::vector<FbxWeight>>::iterator	iter;
	std::unordered_map<int, std::vector<FbxWeight>>::iterator	iterEnd = Container->mapWeights.end();

	for (iter = Container->mapWeights.begin(); iter != iterEnd; ++iter)
	{
		if (iter->second.size() > 4)
		{
			// ����ġ ���� ���� �������� �����Ѵ�.
			sort(iter->second.begin(), iter->second.end(), [](const FbxWeight& lhs, const FbxWeight& rhs)
				{
					return lhs.Weight > rhs.Weight;
				});

			double	Sum = 0.0;

			for (int i = 0; i < 4; ++i)
			{
				Sum += iter->second[i].Weight;
			}

			double	Interpolate = 1.f - Sum;

			std::vector<FbxWeight>::iterator	iterErase = iter->second.begin() + 4;

			iter->second.erase(iterErase, iter->second.end());
			iter->second[0].Weight += Interpolate;
		}

		Vector4	vWeight;
		Vector4	vIndex;

		for (int i = 0; i < iter->second.size(); ++i)
		{
			vWeight[i] = (float)iter->second[i].Weight;
			vIndex[i] = (float)iter->second[i].Index;
		}

		Container->vecBlendWeight[iter->first] = vWeight;
		Container->vecBlendIndex[iter->first] = vIndex;
	}
}
