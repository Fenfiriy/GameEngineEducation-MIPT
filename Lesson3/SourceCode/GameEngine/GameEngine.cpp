// GameEngine.cpp : Defines the entry point for the application.
//

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "GameEngine.h"
#include "RenderEngine.h"
#include "RenderThread.h"
#include "CubeGameObject.h"
#include "GameTimer.h"
#include "InputHandler.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    // Enable run-time memory check for debug builds.
#if defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    GameTimer timer;

    RenderEngine* renderEngine = new RenderEngine(hInstance);
    RenderThread* renderThread = renderEngine->GetRT();
    InputHandler* inputHandler = new InputHandler();

    CubeGameObject* cubes[100]{};
    for (int i = 0; i < 100; i++)
    {
        cubes[i] = new CubeGameObject();
        cubes[i]->SetPosition((i % 10 - 4.5f) * 3, 0, (i / 10 - 4) * 3);
        renderThread->EnqueueCommand(RC_CreateCubeRenderObject, cubes[i]->GetRenderProxy());
    }
    

    MSG msg = { 0 };

    timer.Start();
    timer.Reset();

    float newPositionX, newPositionY, newPositionZ;

    float velocityX = 0.0f, velocityY = 2.0f, velocityZ = 0.0f;

    // Main message loop:
    while (msg.message != (WM_QUIT | WM_CLOSE))
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            inputHandler->Update();

            float t = 0;
            timer.Tick();

            float dt = timer.DeltaTime();
            if (inputHandler->GetInputState().test(eIC_GoLeft))
                velocityX = -1.0f;
            if (inputHandler->GetInputState().test(eIC_GoRight))
                velocityX = 1.0f;

            velocityZ = int(timer.TotalTime()) % 2 - 0.5f;

            if (velocityY < -2.0f)
                velocityY = -velocityY;
            velocityY -= dt;

            for (auto* gameObj : cubes)
            {
                auto t = gameObj->GetPosition();
                newPositionX = std::get<0>(t);
                newPositionY = std::get<1>(t);
                newPositionZ = std::get<2>(t);
                switch (gameObj->GetType())
                {
                case 0:
                    newPositionX += velocityX * dt;
                    break;
                case 1:
                    newPositionZ += velocityZ * dt;
                    break;
                case 2:
                    newPositionY += velocityY * dt;
                    break;
                default:
                    break;
                }
                
                gameObj->SetPosition(newPositionX, newPositionY, newPositionZ);
            }
            renderThread->OnEndFrame();
        }
    }

    return (int) msg.wParam;
}
