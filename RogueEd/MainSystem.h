#ifndef _MAINSYSTEM_H_
#define _MAINSYSTEM_H_

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <thread>

#include "Input.h"
#include "Graphics.h"
#include "Resource.h"
#include "DataIO.h"
#include "HOB.h"
#include "CameraControl.h"
#include "Timer.h"
#include "FpsMonitor.h"
#include "RogueObject.h"

#define ID_FIRSTLEVEL ID_CHOOSELEVEL_0
#define ID_LASTLEVEL ID_CHOOSELEVEL_J

class MainSystem
{
public:
	MainSystem();
	MainSystem(const MainSystem&);
	~MainSystem();

	bool Initialize();
	void Shutdown();
	void Run();
	HWND getToolbarWnd();
	Graphics* getGraphics();
	Timer* getTimer();
	CameraControl* getCameraCtrl();
	FpsMonitor* getFpsMonitor();
	Input* getInput();
	bool getSyncVar();
	bool getInitModelChangeVar();
	void setInitModelChangeVar(bool);
	bool getInitSubModelChangeVar();
	void setInitSubModelChangeVar(bool);
	void setLevelChange(int);
	char getIsLevelChange();
	void CheckLevelMenuItem(int);
	//bool Frame();
	bool Frame();
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	//bool Frame();
	void InitializeWindows(int&, int&);
	void InitializeFilesWindow();
	void ShutdownWindows();
	
	string getSelectedModelPath(int);

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	HWND g_hToolbar;
	HWND g_hModelOptions;
	HMENU g_hMenu;

	HOB* m_TestHOB;
	Input* m_Input;
	Graphics* m_Graphics;
	CameraControl* m_CameraCtrl;
	Timer* m_Timer;
	FpsMonitor* m_Fps;
	char m_changeLevel;
	bool m_sync;
	bool m_initModelChange;
	bool m_initSubModelChange;
	int m_modelObjectIndex;
};

bool ViewportFrame(MainSystem*);

static bool doney = false;

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static LRESULT CALLBACK ToolDlgProc(HWND, UINT, WPARAM, LPARAM);
static LRESULT CALLBACK MdlOptDlgProc(HWND, UINT, WPARAM, LPARAM);

static MainSystem* ApplicationHandle = 0;



#endif