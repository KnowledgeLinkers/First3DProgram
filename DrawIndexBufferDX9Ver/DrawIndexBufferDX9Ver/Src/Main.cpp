#include <Windows.h>
#include "Engine/Engine.h"
#include "Engine/Window.h"
#include "Engine/Graphics.h"
#include "CubeMesh.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmpLine,
	INT       nCmdShow)
{
	// エンジンの初期化
	if (InitEngine(WINDOW_WIDTH, WINDOW_HEIGHT, "UVマッピング") == false)
	{
		return 0;
	}

	InitCubeMesh();

	while (true)
	{
		bool message_ret = false;
		MSG msg;

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else 
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			SetUpTransform();

			DrawStart();

			DrawCubeMesh();

			DrawEnd();
		}
	}

	ReleaseCubeMesh();
	
	// エンジン終了
	EndEngine();

	return 0;
}

