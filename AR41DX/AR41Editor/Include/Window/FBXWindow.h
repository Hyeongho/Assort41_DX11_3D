#pragma once
#include "Editor\EditorWindow.h"

class CFBXWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorCheckBox* m_FBX_CB;
	class CEditorCheckBox* m_Sequence_CB;
	void FBXConvertCallback();
protected:
	CFBXWindow();
	virtual ~CFBXWindow();
public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	void FBXConvert(const TCHAR* path);
};

