
#include "EditorManager.h"

#ifdef _DEBUG

#pragma comment(lib, "AR41Engine_Debug.lib")

#else

#pragma comment(lib, "AR41Engine.lib")

#endif // _DEBUG


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    CEditorManager  mgr;

    if (!mgr.Init(hInstance))
        return 0;

    int Ret = mgr.Run();

    return Ret;
}
