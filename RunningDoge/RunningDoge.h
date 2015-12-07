#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <math.h>
#include <time.h>
#include "resource.h"

/*定义宏变量，WNDWIDTH、WNDHEIGHT为窗口的宽和高*/
#define TIMER_ID             1
#define JUMP_TIMER			 2
#define DOWN_TIMER			 3
#define TIMER_ELAPSE         20
#define WNDWIDTH             720
#define WNDHEIGHT            430
#define MAX_TERRIAN_NUM      24

//定义资源的尺寸         
#define HERO_SIZE_X          53
#define HERO_SIZE_Y          50
#define HERO_MAX_FRAME_NUM   9
#define STEP_SIZE_X          64
#define STEP_SIZE_Y          64
#define GAME_STATUS_SIZE_X   40
#define GAME_STATUS_SIZE_Y   30

/*英雄结构体，英雄位图、位置、大小*/
typedef struct
{
	HBITMAP	hBmp;
	POINT	pos;
	SIZE	size;
	int     curFrameIndex;
	int     maxFrameSize;
}Hero;

/*地形结构体，地形方块位图、位置、大小*/
typedef struct
{
	HBITMAP hStepBmp;
	POINT	pos;
	SIZE    size;
	int     stepWidth;
	int     stepHeight;
}Terrian;

/*游戏状态结构体*/
typedef struct
{
	HBITMAP	hBmp;
	POINT   pos;
	SIZE	size;
	BOOL	isPaused;
	int     totalDist;
	int     totalPoint;
	int		situation;
}GameStatus;

/*全局变量*/
static TCHAR szWindowClass[] = _T("win32app");
static TCHAR szTitle[] = _T("奔跑吧！Doge!");

/*声明位图句柄*/
HBITMAP m_hBackgroundBmp;
HBITMAP m_hHeroBmp;
HBITMAP m_hGameStatusBmp;
HBITMAP	m_hStepBmp;
HBITMAP m_hStartBmp;
HBITMAP m_hStoryStartBmp;
HBITMAP m_hDeadBmp;

/*声明英雄、地形、游戏状态*/
Hero          m_hero;
Terrian       m_terrian[MAX_TERRIAN_NUM];
GameStatus    m_gameStatus;
int			  jump_status;
int			  down_status;
int			  speed_jump;
int			  terriantype[24][5] = { 0, 0, 0, 0, 0, 
									 5, 0, 0, 0, 5,
									 3, 0, 0, 0, 3,
									 1, 0, 0, 0, 1,
									 5, 0, 0, 0, 0,
									 3, 0, 0, 0, 0,
									 1, 0, 0, 0, 0,
									 0, 0, 0, 5, 5,
									 0, 0, 0, 3, 3,
									 0, 0, 0, 1, 1,
									 0, 5, 5, 5, 5,
									 0, 3, 3, 3, 3,
									 0, 1, 1, 1, 1,
									 0, 5, 4, 3, 3,
									 0, 3, 2, 1, 1,
									 5, 4, 3, 5, 5,
									 3, 2, 1, 3, 3,
									 3, 0, 0, 0, 5,
									 1, 0, 0, 0, 3,
									 3, 5, 0, 0, 3,
									 1, 3, 0, 0, 1,
									 5, 5, 5, 5, 5, 
									 3, 3, 3, 3, 3, 
									 1, 1, 1, 1, 1 };
int			  terriansituation;

/*全局函数*/
//窗体过程函数
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//初始化
VOID Init(HWND hWnd, WPARAM wParam, LPARAM lParam);

/*************************************************
Fucntion : 创建英雄人物
Parameter:
posX、posY表示矩形左上角的坐标
sizeX、sizeY表示矩形的size
hBmp表示位图句柄
curFrameIndex当前帧
maxFrameSize最大帧率
*************************************************/
Hero CreateHero(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp, int curFrameIndex, int maxFrameSize);

/*************************************************
Fucntion : 创建游戏状态
Parameter:
posX、posY表示矩形左上角的坐标
sizeX、sizeY表示矩形的size
hBmp表示位图句柄
*************************************************/
GameStatus CreateGameStatus(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);

/*************************************************
Fucntion : 创建单个地形
Parameter:
posX、posY表示矩形左上角的坐标
sizeX、sizeY表示矩形的size
hStepBmp表示方块位图句柄
roofHeight屋顶宽度和高度
blockHeight方块宽度和高度
*************************************************/
Terrian CreateTerrian(LONG posX, LONG posY, LONG sizeX, LONG sizeY,
	HBITMAP hStepBmp, int stepHeight);

//双缓冲绘制
VOID Render(HWND hWnd);
//定时器事件
VOID TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam);
//hero更新
VOID HeroUpdate();
//地形更新
VOID TerrianUpdate();
//游戏状态更新
VOID GameStatusUpdate();
//判断是否点击开始
BOOL Startd(POINT ptMouse);
//判断是否真的开始
BOOL StoryStartd(POINT ptMouse);
//判断是否点击暂停
BOOL Paused(POINT ptMouse);
//判断是否重新开始
BOOL Restart(POINT ptMouse);
//落地判定
int BeBorn();
//右碰撞检测
void RightCollision();
//追及检测
void ChaseTest();
//落地检测
void DownTest(HWND hWnd);
//死亡处理
void Dead(HWND hWnd);
//随机地形
void RandTerrian();
//键盘按下事件处理
VOID KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
//左鼠标点击事件
VOID LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
