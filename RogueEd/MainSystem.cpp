#include "MainSystem.h"

MainSystem::MainSystem()
{
	m_modelObjectIndex = -1;
	m_initModelChange = false;
	m_sync = false;
	m_Input = 0;
	m_Graphics = 0;
	m_CameraCtrl = 0;
	m_Timer = 0;
	m_changeLevel = NULL;
}

MainSystem::MainSystem(const MainSystem& other){}

MainSystem::~MainSystem() {}

bool MainSystem::Initialize()
{
	int screenWidth, screenHeight;
	bool result;


	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight);

	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	m_Input = new Input;
	if (!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	m_Input->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight);

	DataIO* dataIO = DataIO::getInstance();
	dataIO->read("DATA.HDR");

	unordered_map<string, FileInfo> hashMap = dataIO->getHashMap();

	unordered_map<string, FileInfo>::iterator it = hashMap.begin();

	while (it != hashMap.end())
	{
		string name = it->first;

		if (name.find("_HOB") != string::npos)
			SendDlgItemMessageA(g_hToolbar, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)name.c_str());

		it++;
	}
	

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_Graphics = new Graphics;
	if (!m_Graphics)
	{
		return false;
	}

	// Initialize the graphics object.
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}

	m_Timer = new Timer;

	result = m_Timer->Initialize();
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the Timer object.", L"Error", MB_OK);
		return false;
	}

	m_Fps = new FpsMonitor;

	m_Fps->Initialize();

	m_CameraCtrl = new CameraControl;

	return true;
}

void MainSystem::Shutdown()
{
	m_sync = true;

	// Release the graphics object.
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// Release the input object.
	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	// Shutdown the window.
	ShutdownWindows();

	return;
}

void MainSystem::Run()
{
	MSG msg;
	bool done, result;
	thread testThread(ViewportFrame, this);

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (m_Input->IsEscapePressed())
		{
			done = true;
			m_sync = true;
			testThread.join();
			continue;
		}

		if (m_initModelChange)
		{
			DataIO* dataIO = DataIO::getInstance();
			int itemIdx = SendDlgItemMessage(g_hToolbar, IDC_LIST1, LB_GETCURSEL, 0, 0);

			vector<char> hobfilecontents;
			vector<char> hmtfilecontents;
			string path = getSelectedModelPath(itemIdx);

			dataIO->getFileContents(path, &hobfilecontents);

			path.replace(path.end() - 3, path.end(), "HMT");

			dataIO->getFileContents(path, &hmtfilecontents);

			m_sync = true;
			testThread.join();

			vector<string> names = getObjectNames(&hobfilecontents);

			SendDlgItemMessage(g_hModelOptions, IDC_COMBO1, CB_RESETCONTENT, 0, 0);

			for (int i = 0; i < names.size(); i++)
			{
				SendDlgItemMessageA(g_hModelOptions, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)names[i].c_str());
			}

			SendDlgItemMessage(g_hModelOptions, IDC_COMBO1, CB_SETCURSEL, 0, 0);

			m_Graphics->setCurrentObjectIndex(0);
			m_Graphics->setCurrentModel(hobfilecontents, hmtfilecontents);

			m_CameraCtrl->Reset();

			m_sync = false;
			testThread = thread(ViewportFrame, this);

			m_initModelChange = false;
		}

		if (m_initSubModelChange)
		{
			int itemIdx = SendDlgItemMessage(g_hModelOptions, IDC_COMBO1, CB_GETCURSEL, 0, 0);
			m_Graphics->setCurrentObjectIndex(itemIdx);

			m_CameraCtrl->Reset();

			m_initSubModelChange = false;
		}
		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			done = true;
			m_sync = true;
			testThread.join();
			continue;
		}


		//Frame();
		//else
		//{
		//	// Otherwise do the frame processing.
		//	result = Frame();
		//	if (!result)
		//	{
		//		done = true;
		//	}
		//}

	}

}

string MainSystem::getSelectedModelPath(int selIdx)
{
	if (selIdx == LB_ERR)
	{
		// No selection
		return NULL;
	}

	int textLen = SendDlgItemMessage(g_hToolbar, IDC_LIST1, LB_GETTEXTLEN, selIdx, 0);

	CHAR* textBuffer = new CHAR[textLen + 1];

	SendDlgItemMessageA(g_hToolbar, IDC_LIST1, LB_GETTEXT, (WPARAM)selIdx, (LPARAM)textBuffer);

	string path(textBuffer);

	delete[] textBuffer;

	return path;
}

bool SHIT()
{
	return 0;
}

bool MainSystem::Frame()
{
	bool result, keyDown;

	//m_Timer->Frame();

	result = m_Input->Frame();
	if (!result)
	{
		return false;
	}
	m_Timer->Frame();
	m_CameraCtrl->SetFrameTime(m_Timer->GetTime());

	keyDown = m_Input->IsWPressed();
	m_CameraCtrl->MoveForwards(keyDown);

	keyDown = m_Input->IsSPressed();
	m_CameraCtrl->MoveBackwards(keyDown);

	keyDown = m_Input->IsAPressed();
	m_CameraCtrl->MoveLeft(keyDown);

	keyDown = m_Input->IsDPressed();
	m_CameraCtrl->MoveRight(keyDown);

	keyDown = m_Input->IsRMBPressed();
	m_CameraCtrl->TurnX(keyDown, m_Input->GetMouseDirectionY());
	m_CameraCtrl->TurnY(keyDown, m_Input->GetMouseDirectionX());

	keyDown = m_Input->IsLeftArrowPressed();
	m_CameraCtrl->TurnLeft(keyDown);
	keyDown = m_Input->IsRightArrowPressed();
	m_CameraCtrl->TurnRight(keyDown);

	keyDown = m_Input->IsLMBPressed();
	m_CameraCtrl->AdjustTerrainHeight(keyDown, m_Input->GetMouseDirectionY());

}

bool ViewportFrame(MainSystem* system)
{
	bool result;
	XMFLOAT3 camPosition, camRotation;
	float terrainPositionY;
	char test[256];

	do {
		if (ApplicationHandle->getSyncVar())
		{
			return false;
		}
		ApplicationHandle->Frame();

		//ApplicationHandle->getTimer()->Frame();
		ApplicationHandle->getFpsMonitor()->Frame();
		int fps = ApplicationHandle->getFpsMonitor()->GetFps();
		//ApplicationHandle->getEntityCtrl()->SetFrameTime(ApplicationHandle->getTimer()->GetTime());

		/*if (ApplicationHandle->getInitSubModelChangeVar())
		{

		}*/

		// Check if the user pressed escape and wants to exit the application.
		if (system->getInput()->IsEscapePressed())
		{
			return false;
		}

		ApplicationHandle->getCameraCtrl()->GetPosition(camPosition);
		ApplicationHandle->getCameraCtrl()->GetRotation(camRotation);
		ApplicationHandle->getCameraCtrl()->GetPositionTerrainY(terrainPositionY);

		if (char levelId = ApplicationHandle->getIsLevelChange())
		{
			ApplicationHandle->setLevelChange(0);

			ApplicationHandle->getGraphics()->ChangeMap(levelId);

		}

		// Do the frame processing for the graphics object.
		result = ApplicationHandle->getGraphics()->Frame(camPosition, camRotation, terrainPositionY);
		if (!result)
		{
			return false;
		}
	} while (result);

	return true;
}

LRESULT CALLBACK MainSystem::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	//switch (umsg)
	//{
	//	// Check if a key has been pressed on the keyboard.
	//case WM_KEYDOWN:
	//{
	//	// If a key is pressed send it to the input object so it can record that state.
	//	m_Input->KeyDown((unsigned int)wparam);
	//	return 0;
	//}

	//// Check if a key has been released on the keyboard.
	//case WM_KEYUP:
	//{
	//	// If a key is released then send it to the input object so it can unset the state for that key.
	//	m_Input->KeyUp((unsigned int)wparam);
	//	return 0;
	//}

	//// Any other messages send to the default message handler as our application won't make use of them.
	//default:
	//{
	//	return DefWindowProc(hwnd, umsg, wparam, lparam);
	//}
	//}
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

void MainSystem::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;
	g_hToolbar = NULL;

	// Get an external pointer to this object.
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"RogueEd";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = MAKEINTRESOURCEW(IDC_ROGUEED);
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);
	InitializeFilesWindow();
	g_hToolbar = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_TOOLBAR), m_hwnd, ToolDlgProc);
	if (g_hToolbar != NULL)
	{
		ShowWindow(g_hToolbar, SW_SHOW);
	}

	g_hMenu = GetMenu(m_hwnd);

	g_hModelOptions = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_MODELOPTIONS), m_hwnd, MdlOptDlgProc);
	if (g_hModelOptions != NULL)
	{
		//ShowWindow(g_hModelOptions, SW_SHOW);
	}

	// Hide the mouse cursor.
	ShowCursor(true);
	return;
}

HWND MainSystem::getToolbarWnd()
{
	return g_hToolbar;
}

Graphics* MainSystem::getGraphics()
{
	return m_Graphics;
}

Timer* MainSystem::getTimer()
{
	return m_Timer;
}

Input* MainSystem::getInput()
{
	return m_Input;
}

CameraControl* MainSystem::getCameraCtrl()
{
	return m_CameraCtrl;
}

FpsMonitor* MainSystem::getFpsMonitor()
{
	return m_Fps;
}

char MainSystem::getIsLevelChange()
{
	return m_changeLevel;
}

void MainSystem::setLevelChange(int menuItemId)
{
	if (menuItemId >= ID_CHOOSELEVEL_0 && menuItemId <= ID_CHOOSELEVEL_9)
	{
		m_changeLevel = 0x30 + menuItemId - ID_CHOOSELEVEL_0;
	}
	else if (menuItemId > ID_CHOOSELEVEL_9 && menuItemId <= ID_LASTLEVEL){
		m_changeLevel = 0x61 + menuItemId - ID_CHOOSELEVEL_A;
	}
	else {
		m_changeLevel = 0;
	}
}

bool MainSystem::getSyncVar()
{
	return m_sync;
}

bool MainSystem::getInitModelChangeVar()
{
	return m_initModelChange;
}

void MainSystem::setInitModelChangeVar(bool val)
{
	m_initModelChange = val;
}

bool MainSystem::getInitSubModelChangeVar()
{
	return m_initModelChange;
}

void MainSystem::setInitSubModelChangeVar(bool val)
{
	m_initSubModelChange = val;
}

void MainSystem::InitializeFilesWindow()
{

}

void MainSystem::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	HMENU hmenu = GetMenu(hwnd);
	switch (umessage)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wparam);
		// Parse the menu selections:

		if (wmId >= ID_FIRSTLEVEL && wmId <= ID_LASTLEVEL)
		{
			ApplicationHandle->setLevelChange(wmId);
			ApplicationHandle->CheckLevelMenuItem(wmId);
			break;
		}

		switch (wmId)
		{
		case IDM_ABOUT:
			//DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			//DestroyWindow(ApplicationHandle->getToolbarWnd());
			//PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, umessage, wparam, lparam);
		}
		break;
	}
	//break;
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{

		DestroyWindow(ApplicationHandle->getToolbarWnd());
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		DestroyWindow(ApplicationHandle->getToolbarWnd());
		PostQuitMessage(0);
		return 0;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}

LRESULT CALLBACK ToolDlgProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	//case WM_KEYDOWN:
	//{
	//	// If a key is pressed send it to the input object so it can record that state.
	//	ApplicationHandle->getInput()->KeyDown((unsigned int)wparam);
	//	return 0;
	//}

	//// Check if a key has been released on the keyboard.
	//case WM_KEYUP:
	//{
	//	// If a key is released then send it to the input object so it can unset the state for that key.
	//	ApplicationHandle->getInput()->KeyUp((unsigned int)wparam);
	//	return 0;
	//}
	case WM_DESTROY:
	{
		DestroyWindow(hwnd);
		break;
	}
	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
		break;
	}
	case WM_COMMAND:
		switch (LOWORD(wparam))
		{
		case IDC_LIST1:
			switch (HIWORD(wparam))
			{
			case LBN_SELCHANGE:
				ApplicationHandle->setInitModelChangeVar(true);
				return FALSE;
			}
		}
	default:
		return FALSE;
	}
	return TRUE;
}

LRESULT CALLBACK MdlOptDlgProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	//case WM_KEYDOWN:
	//{
	//	// If a key is pressed send it to the input object so it can record that state.
	//	ApplicationHandle->getInput()->KeyDown((unsigned int)wparam);
	//	return 0;
	//}

	//// Check if a key has been released on the keyboard.
	//case WM_KEYUP:
	//{
	//	// If a key is released then send it to the input object so it can unset the state for that key.
	//	ApplicationHandle->getInput()->KeyUp((unsigned int)wparam);
	//	return 0;
	//}
	case WM_DESTROY:
	{
		DestroyWindow(hwnd);
		break;
	}
	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
		break;
	}
	case WM_COMMAND:
		switch (LOWORD(wparam))
		{
		case IDC_COMBO1:
			switch (HIWORD(wparam))
			{
			case CBN_SELCHANGE:
				ApplicationHandle->setInitSubModelChangeVar(true);
				return FALSE;
			}
		}
	default:
		return FALSE;
	}
	return TRUE;
}

void MainSystem::CheckLevelMenuItem(int id)
{
	for (int i = ID_CHOOSELEVEL_0; i < ID_CHOOSELEVEL_J+1; i++)
	{
		if (i == id)
		{
			CheckMenuItem(g_hMenu, i, MF_BYCOMMAND | MF_CHECKED);
		}
		else {
			CheckMenuItem(g_hMenu, i, MF_BYCOMMAND | MF_UNCHECKED);
		}
	}
}