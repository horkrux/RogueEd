// RogueEd.cpp : Defines the entry point for the application.
//

//#include "framework.h"
#include "RogueEd.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    MainSystem* System;
    bool result;


    // Create the system object.
    System = new MainSystem;
    if (!System)
    {
        return 0;
    }

    // Initialize and run the system object.
    result = System->Initialize();
    if (result)
    {
        System->Run();
    }

    // Shutdown and release the system object.
    System->Shutdown();
    delete System;
    System = 0;


    return 0;
}
