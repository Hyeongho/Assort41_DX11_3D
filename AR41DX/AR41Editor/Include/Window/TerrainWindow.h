#pragma once
#include "Editor\EditorWindow.h"

class CTerrainWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorCheckBox* m_Grid;
	class CEditorInput* m_CountX;
	class CEditorInput* m_CountY;
	class CEditorInput* m_HeightMapX;	//읽기 전용
	class CEditorInput* m_HeightMapY;	//읽기 전용
	class CEditorInput* m_CellSizeX;
	class CEditorInput* m_CellSizeY;
	class CEditorInput* m_SizeX;
	class CEditorInput* m_SizeY;
	class CEditorComboBox* m_TerrainEditCombo;
	//버퍼데이터
	class CEditorInput* m_DetailLevel;
	class CEditorInput* m_SplatCount;
	CSharedPtr<class CTerrainComponent>	m_SelectComponent;
	void CreateTerrainClick();
	void SetFrameListCallback(int index, const std::string& item);
protected:
	CTerrainWindow();
	virtual ~CTerrainWindow();
public:
	void SetSelectComponent(class CTerrainComponent* component);
	virtual bool Init();
	virtual void Update(float deltaTime);
};

