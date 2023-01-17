#pragma once
#include "ComponentWidgetList.h"
class CTileMapWidgetList :
    public CComponentWidgetList
{
	friend class CEditorWindow;

protected:
	CTileMapWidgetList();
	virtual ~CTileMapWidgetList();

private:
	class CEditorComboBox* m_ShapeCombo;

	class CEditorInput* m_CountX;
	class CEditorInput* m_CountY;

	class CEditorInput* m_TileSizeX;
	class CEditorInput* m_TileSizeY;


	class CEditorComboBox* m_TileEditCombo;
	class CEditorComboBox* m_TileOptionCombo;
	class CEditorListBox* m_TileFrameList;
	class CEditorImage* m_TileFrameImage;



	class CTileMapComponent* m_TileMap;

public:
	void SetTileMapComponent(class CTileMapComponent* TileMap);

public:
	virtual bool Init();
	virtual void Render();

private:
	void CreateTileMapClick();
	void SetShapeCallback(int Index, const std::string& Item);
	void SetEditModeCallback(int Index, const std::string& Item);
	void SetFrameListCallback(int Index, const std::string& Item);
};

