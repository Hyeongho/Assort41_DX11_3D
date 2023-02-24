#include "Client3DManager.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    CClient3DManager  mgr;

    if (!mgr.Init(hInstance))
        return 0;

    int Ret = mgr.Run();

    return Ret;
}
