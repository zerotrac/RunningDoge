#include "RunningDoge.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
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
		switch (wParam)
		{
		case TIMER_ID:
			if (m_gameStatus.totalDist > 5000 && m_gameStatus.situation == 2)
				BossStart(hWnd);
			if (m_gameStatus.totalDist > 8000 && m_gameStatus.situation == 9)
				BossStart(hWnd);


			TimerUpdate(hWnd, wParam, lParam);
			break;
		case JUMP_TIMER:
			if (speed_jump >= 1)
				m_hero.pos.y -= speed_jump;
			else if (speed_jump == 0)
				m_hero.curFrameIndex = 5;
			else if (speed_jump <= -1)
				m_hero.pos.y -= speed_jump;
			speed_jump--;
			if (BeBorn() >= 0)
			{
				m_hero.pos.y = m_terrian[BeBorn()].pos.y - m_hero.size.cy;
				jump_status = 0;
				KillTimer(hWnd, JUMP_TIMER);
				//SetTimer(hWnd, TIMER_ID, TIMER_ELAPSE, NULL);
			}
			break;
		case DOWN_TIMER:
			if (jump_status!=0)
			{
				KillTimer(hWnd, DOWN_TIMER);
				down_status = 0;
				break;
			}
			m_hero.pos.y -= speed_jump;
			speed_jump--;
			if (BeBorn() >= 0)
			{
				m_hero.pos.y = m_terrian[BeBorn()].pos.y - m_hero.size.cy;
				KillTimer(hWnd, DOWN_TIMER);
				down_status = 0;
			}
			break;
		default:
			break;
		}
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
	//������������
	m_hDeadBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_DEAD));
	//����BOSS�ؿ�PPT����������
	m_hBoss1Bmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_BOSS1));
	m_hBoss2Bmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_BOSS2));
	m_hBoss3Bmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_BOSS3));
	m_hBoss4Bmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_BOSS4));
	m_hBoss5Bmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_BOSS5));
	m_hBoss6Bmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_BOSS6));
	m_hBoss7Bmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_BOSS7));
	m_hBoss8Bmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_BOSS8));
	m_hStoryEndBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_STORYEND));
	m_hEndBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_END));
	//����Ӣ�ۡ�����
	m_hero = CreateHero(-52, 170, HERO_SIZE_X, HERO_SIZE_Y, m_hHeroBmp, 0, HERO_MAX_FRAME_NUM);
	//��������
	int k;
	for (k = 0; k < MAX_TERRIAN_NUM; ++k)
	{
		m_terrian[k] = CreateTerrian(k * 64, 220, STEP_SIZE_X, STEP_SIZE_Y, m_hStepBmp, STEP_SIZE_Y);
	}
	//������Ϸ״̬
	m_gameStatus = CreateGameStatus(0, 0, GAME_STATUS_SIZE_X, GAME_STATUS_SIZE_Y, m_hGameStatusBmp);
	m_gameStatus.totalDist = 0;
	m_gameStatus.totalPoint = 0;

	//����״̬
	jump_status = 0;
	down_status = 0;
	terriansituation = 11;
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
	case 9:
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

		//���ƾ��뼰����
		TCHAR	szDist[13];//������
		SetTextColor(hdcBuffer, RGB(0, 0, 0));
		SetBkMode(hdcBuffer, TRANSPARENT);
		TextOut(hdcBuffer, WNDWIDTH - 100, 15, szDist, wsprintf(szDist, _T("����:%d"), m_gameStatus.totalDist));

		TCHAR	szPoint[13];
		SetTextColor(hdcBuffer, RGB(0, 0, 0));
		SetBkMode(hdcBuffer, TRANSPARENT);
		TextOut(hdcBuffer, WNDWIDTH - 100, 30, szPoint, wsprintf(szPoint, _T("����:%d"), m_gameStatus.totalPoint));

		TCHAR	refen[20];
		SetTextColor(hdcBuffer, RGB(0, 0, 0));
		SetBkMode(hdcBuffer, TRANSPARENT);
		TextOut(hdcBuffer, WNDWIDTH - 200, 115, refen, wsprintf(refen, _T("�ٶ�:%dλ��:%d %d"), speed_jump, m_hero.pos.x, m_hero.pos.y));

		TCHAR	refen2[20];
		SetTextColor(hdcBuffer, RGB(0, 0, 0));
		SetBkMode(hdcBuffer, TRANSPARENT);
		TextOut(hdcBuffer, WNDWIDTH - 200, 130, refen2, wsprintf(refen2, _T("%d"), terriansituation));

		int i;
		TCHAR   pos[24][10];
		for (i = 0; i < 24;i++)
		{
			SetTextColor(hdcBuffer, RGB(255, 0, 0));
			SetBkMode(hdcBuffer, TRANSPARENT);
			TextOut(hdcBuffer, WNDWIDTH - 300, i * 15, pos[i], wsprintf(pos[i], _T("%d  %d"), m_terrian[i].pos.x, m_terrian[i].pos.y));
		}
		break;
	case 3:
		SelectObject(hdcBmp, m_hDeadBmp);
		BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT,
			hdcBmp, 0, 0, SRCCOPY);

		TCHAR	szFinalDist[13];
		SetTextColor(hdcBuffer, RGB(255, 255, 255));
		SetBkMode(hdcBuffer, TRANSPARENT);
		TextOut(hdcBuffer, 100, 30, szFinalDist, wsprintf(szFinalDist, _T("���վ���:%d"), m_gameStatus.totalDist));

		TCHAR   szFinalPoint[13];
		SetTextColor(hdcBuffer, RGB(255, 255, 255));
		SetBkMode(hdcBuffer, TRANSPARENT);
		TextOut(hdcBuffer, 100, 45, szFinalPoint, wsprintf(szFinalPoint, _T("���շ���:%d"), m_gameStatus.totalPoint));
		break;
	case 4:
		SelectObject(hdcBmp, m_hBoss1Bmp);
		BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT,
			hdcBmp, 0, 0, SRCCOPY);
		break;
	case 5:
		SelectObject(hdcBmp, m_hBoss2Bmp);
		BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT,
			hdcBmp, 0, 0, SRCCOPY);
		break;
	case 6:
		SelectObject(hdcBmp, m_hBoss3Bmp);
		BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT,
			hdcBmp, 0, 0, SRCCOPY);
		break;
	case 7:
		SelectObject(hdcBmp, m_hBoss4Bmp);
		BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT,
			hdcBmp, 0, 0, SRCCOPY);
		break;
	case 8:
		SelectObject(hdcBmp, m_hBoss5Bmp);
		BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT,
			hdcBmp, 0, 0, SRCCOPY);
		break;
	case 10:
		SelectObject(hdcBmp, m_hBoss6Bmp);
		BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT,
			hdcBmp, 0, 0, SRCCOPY);
		break;
	case 11:
		SelectObject(hdcBmp, m_hBoss7Bmp);
		BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT,
			hdcBmp, 0, 0, SRCCOPY);
		break;
	case 12:
		SelectObject(hdcBmp, m_hBoss8Bmp);
		BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT,
			hdcBmp, 0, 0, SRCCOPY);
		break;
	case 13:
		SelectObject(hdcBmp, m_hStoryEndBmp);
		BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT,
			hdcBmp, 0, 0, SRCCOPY);
		break;
	case 14:
		SelectObject(hdcBmp, m_hEndBmp);
		BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT,
			hdcBmp, 0, 0, SRCCOPY);

		TCHAR	szFinalDist0[13];
		SetTextColor(hdcBuffer, RGB(255, 255, 255));
		SetBkMode(hdcBuffer, TRANSPARENT);
		TextOut(hdcBuffer, 100, 30, szFinalDist0, wsprintf(szFinalDist0, _T("���վ���:%d"), m_gameStatus.totalDist));

		TCHAR   szFinalPoint0[13];
		SetTextColor(hdcBuffer, RGB(255, 255, 255));
		SetBkMode(hdcBuffer, TRANSPARENT);
		TextOut(hdcBuffer, 100, 45, szFinalPoint0, wsprintf(szFinalPoint0, _T("���շ���:%d"), m_gameStatus.totalPoint));
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
	gameStatus.isPaused = false;
	gameStatus.situation = 0;
	return gameStatus;
}

Terrian CreateTerrian(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hStepBmp, int stepHeight)
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
	int i;
	if (m_gameStatus.totalDist <= 1000)
		difficulty = 3;
	else if (m_gameStatus.totalDist <= 3000)
		difficulty = 4;
	else if (m_gameStatus.totalDist <= 5000)
		difficulty = 5;
	
	if (BeBorn()>=0)
	{
		HeroUpdate();
	}

	for (i = 1; i <= difficulty; i++)
	{
		TerrianUpdate();
		GameStatusUpdate();
		DownTest(hWnd);
		ChaseTest();
		RightCollision();
	}

	if (m_hero.pos.y >= 430 || m_hero.pos.x + m_hero.size.cx <= 0)
		Dead(hWnd);

	InvalidateRect(hWnd, NULL, FALSE);
}

VOID HeroUpdate()
{
	//���¶���
	++m_hero.curFrameIndex;
	m_hero.curFrameIndex = m_hero.curFrameIndex >= (m_hero.maxFrameSize - 1) ? 0 : m_hero.curFrameIndex;
}

VOID TerrianUpdate()
{
	int k, i, j;
	for (k = 0; k < MAX_TERRIAN_NUM; ++k)
	{
		m_terrian[k].pos.x -= 2;
	}
	
	for (k = 0; k < MAX_TERRIAN_NUM; ++k)
	{
		if (m_terrian[k].pos.x + m_terrian[k].size.cx <= -4 * m_terrian[k].size.cx)
		{
			Terrian	temp_terrian[5];
			RandTerrian();
			int temp[5];
			for (j = 0; j < 5; j++)
				temp[j] = terriantype[terriansituation][j];
			for (j = 0; j < 5;j++)
			{
				if (temp[j] == 0)
					temp[j] = 7;
				temp_terrian[j] = CreateTerrian((MAX_TERRIAN_NUM - 5 + j) * 64, 28 + 64 * temp[j], STEP_SIZE_X, STEP_SIZE_Y, m_hStepBmp, STEP_SIZE_Y);
			}

			for (i = k, j = 0; i <= k + 4; i++, j++)
			{
				if (i >= MAX_TERRIAN_NUM)
				{
					m_terrian[i - MAX_TERRIAN_NUM].pos.x = temp_terrian[j].pos.x;
					m_terrian[i - MAX_TERRIAN_NUM].pos.y = temp_terrian[j].pos.y;
				}
				else
				{
					m_terrian[i].pos.x = temp_terrian[j].pos.x;
					m_terrian[i].pos.y = temp_terrian[j].pos.y;
				}

			}
		}
	}
}

VOID GameStatusUpdate()
{
	++m_gameStatus.totalDist;
	m_gameStatus.totalPoint += 2;
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

BOOL Restart(POINT ptMouse)
{
	RECT rRestart;

	rRestart.left = 509;
	rRestart.top = 361;
	rRestart.right = 650;
	rRestart.bottom = 378;

	return PtInRect(&rRestart, ptMouse);
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

BOOL Continued(POINT ptMouse)
{
	RECT rPause;

	rPause.left = 0;
	rPause.top = 0;
	rPause.right = 720;
	rPause.bottom = 430;

	return PtInRect(&rPause, ptMouse);
}

int BeBorn()
{
	int i;
	for (i = 0; i < MAX_TERRIAN_NUM; i++)
	{
		if(m_hero.pos.x + m_hero.size.cx > m_terrian[i].pos.x && m_hero.pos.x < m_terrian[i].pos.x + m_terrian[i].size.cx)
			if (m_hero.pos.y + m_hero.size.cy >= m_terrian[i].pos.y)
				break;
	}
	if (i == MAX_TERRIAN_NUM)
		return -1;
	else
		return i;
}

void RightCollision()
{
	int i;
	for (i = 0; i < MAX_TERRIAN_NUM; i++)
	{
		if (m_hero.pos.y + m_hero.size.cy <= m_terrian[i].pos.y)
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
	if (m_hero.pos.x < 200)
	{
		type = 1;
		for (i = 0; i < MAX_TERRIAN_NUM; i++)
		{
			if (m_hero.pos.y + m_hero.size.cy <= m_terrian[i].pos.y)
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
		m_hero.pos.x += 1;
	}
}

void DownTest(HWND hWnd)
{
	if (BeBorn() == -1 && jump_status == 0)
	{
		if (down_status==0)
		{
			down_status++;
			speed_jump = 0;
			m_hero.curFrameIndex = 5;
			//KillTimer(hWnd, TIMER_ID);
			SetTimer(hWnd, DOWN_TIMER, TIMER_ELAPSE, NULL);
		}
	}
}

void Dead(HWND hWnd)
{
	m_gameStatus.situation++;
	if (jump_status != 0)
		KillTimer(hWnd, JUMP_TIMER);
	else if (down_status = 1)
		KillTimer(hWnd, DOWN_TIMER);
	KillTimer(hWnd, TIMER_ID);
	PlaySound((LPCWSTR)IDR_DEAD, NULL, SND_RESOURCE | SND_SYNC);
}

void RandTerrian()
{
	srand((unsigned)time(NULL));
	switch (terriansituation)
	{
	case 4:
		if (m_gameStatus.totalDist <= 1000)
		{
			switch (rand() % 2)
			{
			case 0:
				terriansituation = 10;
				break;
			case 1:
				terriansituation = 13;
				break;
			}
		}
		else
		{
			switch (rand() % 3)
			{
			case 0:
				terriansituation = 7;
				break;
			case 1:
				terriansituation = 10;
				break;
			case 2:
				terriansituation = 13;
				break;
			}
		}
		break;
	case 5:
		if (m_gameStatus.totalDist <= 1000)
		{
			switch (rand() % 2)
			{
			case 0:
				terriansituation = 11;
				break;
			case 1:
				terriansituation = 14;
				break;
			}
		}
		else
		{
			switch (rand() % 3)
			{
			case 0:
				terriansituation = 8;
				break;
			case 1:
				terriansituation = 11;
				break;
			case 2:
				terriansituation = 14;
				break;
			}
		}
		break;
	case 6:
		if (m_gameStatus.totalDist <= 1000)
			terriansituation = 12;
		else
		{
			switch (rand() % 2)
			{
			case 0:
				terriansituation = 9;
				break;
			case 1:
				terriansituation = 12;
				break;
			}
		}
		break;
	case 1:
	case 7:
	case 10:
	case 15:
	case 17:
	case 21:
		switch (rand() % 7)
		{
		case 0:
			terriansituation = 1;
			break;
		case 1:
			terriansituation = 4;
			break;
		case 2:
			terriansituation = 7;
			break;
		case 3:
			terriansituation = 10;
			break;
		case 4:
			terriansituation = 13;
			break;
		case 5:
			terriansituation = 15;
			break;
		case 6:
			terriansituation = 21;
			break;
		default:
			break;
		}
		break;
	case 2:
	case 8:
	case 11:
	case 13:
	case 16:
	case 18:
	case 19:
	case 22:
		switch (rand() % 9)
		{
		case 0:
			terriansituation = 2;
			break;
		case 1:
			terriansituation = 5;
			break;
		case 2:
			terriansituation = 8;
			break;
		case 3:
			terriansituation = 11;
			break;
		case 4:
			terriansituation = 14;
			break;
		case 5:
			terriansituation = 16;
			break;
		case 6:
			terriansituation = 17;
			break;
		case 7:
			terriansituation = 19;
			break;
		case 8:
			terriansituation = 22;
			break;
		default:
			break;
		}
		break;
	case 3:
	case 9:
	case 12:
	case 14:
	case 20:
	case 23:
		switch (rand() % 7)
		{
		case 0:
			terriansituation = 3;
			break;
		case 1:
			terriansituation = 6;
			break;
		case 2:
			terriansituation = 9;
			break;
		case 3:
			terriansituation = 12;
			break;
		case 4:
			terriansituation = 18;
			break;
		case 5:
			terriansituation = 20;
			break;
		case 6:
			terriansituation = 23;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void BossStart(HWND hWnd)
{
	if (jump_status != 0)
		KillTimer(hWnd, JUMP_TIMER);
	else if (down_status = 1)
		KillTimer(hWnd, DOWN_TIMER);
	KillTimer(hWnd, TIMER_ID);
	if (m_gameStatus.situation == 2)
	{
		m_gameStatus.situation = 4;
		//PlaySound((LPCWSTR)IDR_BOSS, NULL, SND_RESOURCE | SND_ASYNC | SND_LOOP);
	}
	else if (m_gameStatus.situation == 9)
	{
		m_gameStatus.situation = 10;
	}
	InvalidateRect(hWnd, NULL, FALSE);
}

VOID KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	//TODO
	if ((m_gameStatus.situation == 2 || m_gameStatus.situation == 9) && m_gameStatus.isPaused == 0)
	{
		switch (wParam)
		{
		case VK_UP:
			if (jump_status != 2)
			{
				speed_jump = 12;
				if (jump_status == 0)
				{
					jump_status++;
					SetTimer(hWnd, JUMP_TIMER, TIMER_ELAPSE, NULL);
					m_hero.curFrameIndex = 2;
				}
				else if (jump_status == 1)
				{
					jump_status++;
					m_hero.curFrameIndex = 2;
				}
			}
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
		PlaySound((LPCWSTR)IDR_THEME, NULL, SND_RESOURCE | SND_ASYNC | SND_LOOP);
		SetTimer(hWnd, TIMER_ID, TIMER_ELAPSE, NULL);
		InvalidateRect(hWnd, NULL, FALSE);
	}

	//�������˼�������ͣͼ��
	if (Paused(ptMouse) && (m_gameStatus.situation == 2 || m_gameStatus.situation == 9))
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

	//����������������GO,ִ�����¿�ʼ
	if (Restart(ptMouse) && m_gameStatus.situation == 3)
	{
		//��ʼ����ť
		//����Ӣ�ۡ�����
		m_hero = CreateHero(-52, 170, HERO_SIZE_X, HERO_SIZE_Y, m_hHeroBmp, 0, HERO_MAX_FRAME_NUM);
		//��������
		int k;
		for (k = 0; k < MAX_TERRIAN_NUM; ++k)
		{
			m_terrian[k] = CreateTerrian(k * 64, 220, STEP_SIZE_X, STEP_SIZE_Y, m_hStepBmp, STEP_SIZE_Y);
		}
		//������Ϸ״̬
		m_gameStatus = CreateGameStatus(0, 0, GAME_STATUS_SIZE_X, GAME_STATUS_SIZE_Y, m_hGameStatusBmp);
		m_gameStatus.totalDist = 0;
		m_gameStatus.totalPoint = 0;
		//����״̬
		jump_status = 0;
		down_status = 0;
		terriansituation = 11;

		m_gameStatus.situation = 2;
		PlaySound((LPCWSTR)IDR_THEME, NULL, SND_RESOURCE | SND_ASYNC | SND_LOOP);
		SetTimer(hWnd, TIMER_ID, TIMER_ELAPSE, NULL);
		InvalidateRect(hWnd, NULL, FALSE);
	}

	//�����PPT��ӳ�׶ε���PPT��������λ�ã�������ӳ
	if (Continued(ptMouse) && (m_gameStatus.situation >= 4 && m_gameStatus.situation <= 7))
	{
		m_gameStatus.situation++;
		InvalidateRect(hWnd, NULL, FALSE);
	}

	//����ڵ�����PPT�����Ļ����׶Σ�����boss�ؿ�
	if (Continued(ptMouse) && m_gameStatus.situation == 8)
	{
		m_hero = CreateHero(-52, 170, HERO_SIZE_X, HERO_SIZE_Y, m_hHeroBmp, 0, HERO_MAX_FRAME_NUM);
		//��������
		int k;
		for (k = 0; k < MAX_TERRIAN_NUM; ++k)
		{
			m_terrian[k] = CreateTerrian(k * 64, 220, STEP_SIZE_X, STEP_SIZE_Y, m_hStepBmp, STEP_SIZE_Y);
		}
		//������Ϸ״̬
		m_gameStatus = CreateGameStatus(0, 0, GAME_STATUS_SIZE_X, GAME_STATUS_SIZE_Y, m_hGameStatusBmp);
		//����״̬
		jump_status = 0;
		down_status = 0;
		terriansituation = 11;

		m_gameStatus.situation = 9;
		SetTimer(hWnd, TIMER_ID, TIMER_ELAPSE, NULL);
		InvalidateRect(hWnd, NULL, FALSE);
	}

	//���Boss�أ�����PPT��ӳ
	if (Continued(ptMouse) && (m_gameStatus.situation >= 10 && m_gameStatus.situation <= 13))
	{
		m_gameStatus.situation++;
		InvalidateRect(hWnd, NULL, FALSE);
	}

	//TheEnd���棬����ر�
	if (Continued(ptMouse) && m_gameStatus.situation == 14)
	{
		SendMessage(hWnd, WM_CLOSE, 0, 0);
	}
}