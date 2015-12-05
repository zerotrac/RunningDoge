#include "RunningDoge.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	WNDCLASSEX wcex;
	HWND hWnd;
	MSG msg;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Running Doge"),
			MB_OK);

		return 1;
	}

	// The parameters to CreateWindow explained:
	// szWindowClass: the name of the application
	// szTitle: the text that appears in the title bar
	// WS_OVERLAPPEDWINDOW: the type of window to create,~WS_THICKFRAME  fixed window size
	// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
	// WNDWIDTH, WNDHEIGHT: initial size (width, length)
	// NULL: the parent of this window
	// NULL: this application does not have a menu bar
	// hInstance: the first parameter from WinMain
	// NULL: not used in this application
	hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, //���ô�����ʽ�����ɸı��С���������
		CW_USEDEFAULT, CW_USEDEFAULT,
		WNDWIDTH, WNDHEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Running Doge"),
			MB_OK);

		return 1;
	}

	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain
	ShowWindow(hWnd,
		nCmdShow);
	UpdateWindow(hWnd);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		//��ʼ��
		Init(hWnd, wParam, lParam);
		break;
	case WM_PAINT:
		//����
		Render(hWnd);
		break;
	case WM_KEYDOWN:
		//���̰����¼�
		KeyDown(hWnd, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
		//������¼�
		LButtonDown(hWnd, wParam, lParam);
		break;
	case WM_TIMER:
		//��ʱ���¼�
		TimerUpdate(hWnd, wParam, lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}

VOID Init(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	//���ؿ�ʼ����λͼ
	m_hStartBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_START));
	//���ع��½���λͼ
	m_hStoryStartBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_STORY_START));
	//���ر���λͼ
	m_hBackgroundBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_BACKGROUND));
	//����Ӣ��λͼ
	m_hHeroBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_HERODOGE_GRAY));
	//������Ϸ״̬λͼ
	m_hGameStatusBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_GAME_STATUS));
	//���ص��ο�λͼ
	m_hStepBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_STEP));
	//����Ӣ�ۡ�����
	m_hero = CreateHero(200, 238, HERO_SIZE_X, HERO_SIZE_Y, m_hHeroBmp, 0, HERO_MAX_FRAME_NUM);
	//��������
	int k;
	for (k = 0; k < MAX_TERRIAN_NUM; ++k)
	{
		if (k % 4 == 0)
		{
			continue;
		}
		m_terrian[k] = CreateTerrian(k * 64, 220 + 64 * (k % 2), STEP_SIZE_X, STEP_SIZE_Y, m_hStepBmp, STEP_SIZE_Y);
	}
	//������Ϸ״̬
	m_gameStatus = CreateGameStatus(0, 0, GAME_STATUS_SIZE_X, GAME_STATUS_SIZE_Y, m_hGameStatusBmp);
	jump_status = 0;
	//������ʱ��
	//SetTimer(hWnd, TIMER_ID, TIMER_ELAPSE, NULL);
}

VOID Render(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc;
	//��ʼ����
	hdc = BeginPaint(hWnd, &ps);

	HDC	hdcBmp, hdcBuffer;
	HBITMAP	cptBmp;

	cptBmp = CreateCompatibleBitmap(hdc, WNDWIDTH, WNDHEIGHT);
	hdcBmp = CreateCompatibleDC(hdc);
	hdcBuffer = CreateCompatibleDC(hdc);
	SelectObject(hdcBuffer, cptBmp);

	switch (m_gameStatus.situation)
	{
	case 0:
		SelectObject(hdcBmp, m_hStartBmp);
		BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT,
			hdcBmp, 0, 0, SRCCOPY);
		break;
	case 1:
		SelectObject(hdcBmp, m_hStoryStartBmp);
		BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT,
			hdcBmp, 0, 0, SRCCOPY);
		break;
	case 2:
		//���Ʊ���������
		SelectObject(hdcBmp, m_hBackgroundBmp);
		BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT,
			hdcBmp, 0, 0, SRCCOPY);

		//����Hero������
		SelectObject(hdcBmp, m_hero.hBmp);
		TransparentBlt(
			hdcBuffer, m_hero.pos.x, m_hero.pos.y,
			m_hero.size.cx, m_hero.size.cy,
			hdcBmp, 0, m_hero.size.cy * m_hero.curFrameIndex, m_hero.size.cx, m_hero.size.cy,
			RGB(255, 255, 255)
			);

		//���Ƶ���
		int k;
		for (k = 0; k < MAX_TERRIAN_NUM; ++k)
		{
			Terrian terrian = m_terrian[k];
			SelectObject(hdcBmp, terrian.hStepBmp);
			TransparentBlt(
				hdcBuffer, terrian.pos.x, terrian.pos.y,
				terrian.stepWidth, terrian.stepHeight,
				hdcBmp, 0, 0, terrian.stepWidth, terrian.stepHeight,
				RGB(255, 255, 255)
				);
		}

		//������Ϸ״̬
		//��ͣ�����λͼ
		SelectObject(hdcBmp, m_gameStatus.hBmp);
		TransparentBlt(hdcBuffer, m_gameStatus.pos.x, m_gameStatus.pos.y, m_gameStatus.size.cx, m_gameStatus.size.cy,
			hdcBmp, 0, m_gameStatus.size.cy * m_gameStatus.isPaused,
			m_gameStatus.size.cx, m_gameStatus.size.cy, RGB(255, 255, 255));

		//���Ʒ���
		TCHAR	szDist[13];
		SetTextColor(hdcBuffer, RGB(0, 0, 0));
		SetBkMode(hdcBuffer, TRANSPARENT);
		TextOut(hdcBuffer, WNDWIDTH - 100, 15, szDist, wsprintf(szDist, _T("����:%d"), m_gameStatus.totalDist));
		break;
	default:
		break;
	}

	//������е���Ϣ���Ƶ���Ļ��
	BitBlt(hdc, 0, 0, WNDWIDTH, WNDHEIGHT, hdcBuffer, 0, 0, SRCCOPY);

	//������Դ��ռ���ڴ�
	DeleteObject(cptBmp);
	DeleteDC(hdcBuffer);
	DeleteDC(hdcBmp);

	//��������
	EndPaint(hWnd, &ps);
}

Hero CreateHero(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp, int curFrameIndex, int maxFrameSize)
{
	Hero hero;
	hero.hBmp = hBmp;
	hero.pos.x = posX;
	hero.pos.y = posY;
	hero.size.cx = sizeX;
	hero.size.cy = sizeY;
	hero.curFrameIndex = curFrameIndex;
	hero.maxFrameSize = maxFrameSize;
	return hero;
}

GameStatus CreateGameStatus(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp)
{
	GameStatus gameStatus;
	gameStatus.pos.x = posX;
	gameStatus.pos.y = posY;
	gameStatus.size.cx = sizeX;
	gameStatus.size.cy = sizeY;
	gameStatus.hBmp = hBmp;
	gameStatus.totalDist = 0;
	gameStatus.isPaused = false;
	gameStatus.situation = 0;
	return gameStatus;
}

Terrian CreateTerrian(LONG posX, LONG posY, LONG sizeX, LONG sizeY,
	HBITMAP hStepBmp, int stepHeight)
{
	Terrian terrian;
	terrian.pos.x = posX;
	terrian.pos.y = posY;
	terrian.size.cx = sizeX;
	terrian.size.cy = sizeY;
	terrian.hStepBmp = hStepBmp;
	terrian.stepWidth = sizeX;
	terrian.stepHeight = STEP_SIZE_Y;
	return terrian;
}

VOID TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	int difficulty, i;
	if (m_gameStatus.totalDist <= 10000)
		difficulty = 3;
	else if (m_gameStatus.totalDist <= 30000)
		difficulty = 4;
	else// if (m_gameStatus.totalDist <= 50000)
		difficulty = 5;
	
	HeroUpdate();

	for (i = 1; i <= difficulty; i++)
	{
		TerrianUpdate();
		GameStatusUpdate();
		ChaseTest();
		RightCollision();
	}

	InvalidateRect(hWnd, NULL, FALSE);
}

VOID HeroUpdate()
{
	//TODO
	//����λ��
	//m_hero.pos.x += 1;
	m_hero.pos.x = m_hero.pos.x >= WNDWIDTH ? 0 : m_hero.pos.x;
	//���¶���
	++m_hero.curFrameIndex;
	m_hero.curFrameIndex = m_hero.curFrameIndex >= (m_hero.maxFrameSize - 1) ? 0 : m_hero.curFrameIndex;
}

VOID TerrianUpdate()
{
	//TODO
	int k;
	for (k = 0; k < MAX_TERRIAN_NUM; ++k)
	{
		m_terrian[k].pos.x -= 2;
		if (m_terrian[k].pos.x + m_terrian[k].size.cx < 0)
		{
			m_terrian[k].pos.x += MAX_TERRIAN_NUM * m_terrian[k].size.cx;
		}
	}
}

VOID GameStatusUpdate()
{
	//TODO
	++m_gameStatus.totalDist;
}

BOOL Startd(POINT ptMouse)
{
	RECT rStart;

	rStart.left = 586;
	rStart.top = 303;
	rStart.right = 660;
	rStart.bottom = 332;

	return PtInRect(&rStart, ptMouse);
}

BOOL StoryStartd(POINT ptMouse)
{
	RECT rStoryStart;

	rStoryStart.left = 598;
	rStoryStart.top = 334;
	rStoryStart.right = 651;
	rStoryStart.bottom = 353;

	return PtInRect(&rStoryStart, ptMouse);
}

BOOL Paused(POINT ptMouse)
{
	RECT rPause;

	rPause.left = m_gameStatus.pos.x;
	rPause.top = m_gameStatus.pos.y;
	rPause.right = m_gameStatus.pos.x + m_gameStatus.size.cx;
	rPause.bottom = m_gameStatus.pos.y + m_gameStatus.size.cy;

	return PtInRect(&rPause, ptMouse);
}

void RightCollision()
{
	int i;
	for (i = 0; i < MAX_TERRIAN_NUM; i++)
	{
		if (m_hero.pos.y + m_hero.size.cy <= m_terrian[i].pos.y + 4)
			continue;
		else if (m_hero.pos.x + m_hero.size.cx>m_terrian[i].pos.x - 2 && m_hero.pos.x + m_hero.size.cx <= m_terrian[i].pos.x)
		{
			m_hero.pos.x -= 2;
			break;
		}
	}
}

void ChaseTest()
{
	int i, type;
	type = 0;
	if (m_hero.pos.x + m_hero.size.cx < 200)
	{
		type = 1;
		for (i = 0; i < MAX_TERRIAN_NUM; i++)
		{
			if (m_hero.pos.y + m_hero.size.cy <= m_terrian[i].pos.y + 4)
				continue;
			else if (m_hero.pos.x + m_hero.size.cx>m_terrian[i].pos.x - 2 && m_hero.pos.x + m_hero.size.cx <= m_terrian[i].pos.x)
			{
				type = 0;
				break;
			}
		}
	}
	if (type == 1)
	{
		m_hero.pos.x += 2;
	}
}

VOID KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	//TODO
	if (m_gameStatus.situation == 2 && m_gameStatus.isPaused == 0)
	{
		switch (wParam)
		{
		case VK_UP:
			m_hero.pos.y -= 64;
			/*if (jump_status != 2)
			{
				if (jump_status == 0)
				{
					jump_status++;
					SetTimer(hWnd, JUMP_TIMER, TIMER_ELAPSE, NULL);
				}
			}*/
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case VK_DOWN:
			m_hero.pos.y += 1;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		default:
			break;
		}
	}
}

VOID LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	POINT ptMouse;
	ptMouse.x = LOWORD(lParam);
	ptMouse.y = HIWORD(lParam);

	//����ڿ�ʼ������˿�ʼ��Ϸ
	if (Startd(ptMouse) && m_gameStatus.situation == 0)
	{
		m_gameStatus.situation = 1;
		InvalidateRect(hWnd, NULL, FALSE);
	}

	//����ڹ��½����׶ε���GO
	if (StoryStartd(ptMouse) && m_gameStatus.situation == 1)
	{
		m_gameStatus.situation = 2;
		PlaySound(TEXT("res\\sound\\Theme.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		SetTimer(hWnd, TIMER_ID, TIMER_ELAPSE, NULL);
		InvalidateRect(hWnd, NULL, FALSE);
	}

	//�������˼�������ͣͼ��
	if (Paused(ptMouse) && m_gameStatus.situation == 2)
	{
		if (!m_gameStatus.isPaused)
		{
			KillTimer(hWnd, TIMER_ID);
			m_gameStatus.isPaused = TRUE;
		}
		else
		{
			SetTimer(hWnd, TIMER_ID, TIMER_ELAPSE, NULL);
			m_gameStatus.isPaused = FALSE;
		}
		InvalidateRect(hWnd, NULL, FALSE);
	}
}