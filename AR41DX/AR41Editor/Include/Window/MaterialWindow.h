#pragma once
#include "Editor\EditorWindow.h"
#include "Editor/EditorTree.h"

class CMaterialWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorInput* m_RenderState[3];
	class CEditorCheckBox* m_ReceiveDecal;
	class CEditorCheckBox* m_EnableBump;
	class CEditorCheckBox* m_EnableSpecular;
	class CEditorCheckBox* m_EnableEmissive;
	class CEditorCheckBox* m_Animation3D;
	class CEditorInput* m_BaseColor[4];
	class CEditorInput* m_AmbientColor[4];
	class CEditorInput* m_SpecularColor[4];
	class CEditorInput* m_EmissiveColor[4];
	class CEditorInput* m_Opacity;
	class CEditorInput* m_Shader;
	class CEditorInput* m_Name;
	class CEditorImage* m_Image;
	class CEditorCheckBox* m_IsArray;
	CEditorTree<class CMaterial*>* m_Tree;
	CEditorTree<class CTexture*>* m_TextureTree;
	CEditorTree<class CTexture*>* m_MultiTextureTree;
	CSharedPtr<class CPrimitiveComponent>	m_SelectComponent;
	CSharedPtr<class CMaterial>	m_SelectMaterial;
	//메터리얼 리스트
	class CEditorListBox* m_MaterialList;
	std::string			m_SelectListItem;
	std::string			m_SelectTextureItem;
	void ListClickCallback(int index, const std::string& item);
	void MaterialLoadCallback();
	void MaterialChangeCallback();
	void MaterialCreateCallback();
	void AddMultiTexure();
	void DeleteMultiTexure();
	void TreeCallback(CEditorTreeItem<class CMaterial*>* node, const std::string& item);
	void TextureTreeCallback(CEditorTreeItem<class CTexture*>* node, const std::string& item);
protected:
	CMaterialWindow();
	virtual ~CMaterialWindow();
public:
	void SetSelectComponent(class CPrimitiveComponent* component);
	virtual bool Init();
	virtual void Update(float deltaTime);
	bool AddItem(class CMaterial* material, const std::string& name, const std::string& parentName = "Material");
	bool AddItem(class CTexture* texture, const std::string& name, const std::string& parentName = "Texture");
	bool AddMultiTextureItem(class CTexture* texture, const std::string& name,
		const std::string& parentName = "MultiTexture");
	void Clear();
	void ClearTexture();
	void ClearMultiTexture();
	void ClearSelect();
	void ImgChangeCallback(const std::string& name, const TCHAR* path);
};

