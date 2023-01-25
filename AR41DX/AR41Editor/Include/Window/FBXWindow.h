#pragma once
#include "Editor\EditorWindow.h"

class CFBXWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorInput* m_FBXName;
	class CEditorCheckBox* m_FBX_CB;
	void FBXConvertCallback();
	void SequenceConvertCallback();
protected:
	CFBXWindow();
	virtual ~CFBXWindow();
public:
	virtual bool Init();
	virtual void Update(float deltaTime);
};

