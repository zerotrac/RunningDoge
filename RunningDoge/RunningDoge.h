#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <math.h>
#include "resource.h"

/*����������WNDWIDTH��WNDHEIGHTΪ���ڵĿ�͸�*/
#define TIMER_ID             1
#define TIMER_ELAPSE         20
#define WNDWIDTH             720
#define WNDHEIGHT            430
#define BLOCK_COLOR_NUM      4
#define ROOF_COLOR_NUM       2
#define MAX_TERRIAN_NUM      20

//������Դ�ĳߴ�         
#define HERO_SIZE_X          65
#define HERO_SIZE_Y          49
#define HERO_MAX_FRAME_NUM   9
#define BUILDING_SIZE_X      900
#define BUILDING_SIZE_Y      600
#define ROOF_SIZE_X          65
#define ROOF_SIZE_Y          33
#define BLOCK_SIZE_X         65
#define BLOCK_SIZE_Y         65
#define GAME_STATUS_SIZE_X   40
#define GAME_STATUS_SIZE_Y   30

/*Ӣ�۽ṹ�壬Ӣ��λͼ��λ�á���С*/
typedef struct
{
	HBITMAP	hBmp;
	POINT	pos;
	SIZE	size;
	int     curFrameIndex;
	int     maxFrameSize;
}Hero;

/*���������ṹ�壬����λͼ��λ�á���С������*/
typedef struct
{
	HBITMAP	hBmp;
	POINT	pos;
	SIZE	size;
}Building;

/*���νṹ�壬���η���λͼ���ݶ�λͼ��λ�á���С���ݶ��Լ������С���������*/
typedef struct
{
	HBITMAP hBlockBmp;
	HBITMAP hRoofBmp;
	POINT	pos;
	SIZE    size;
	int     roofWidth;
	int     roofHeight;
	int     blockWidth;
	int     blockHeight;
	int     blockNum;
}Terrian;

/*��Ϸ״̬�ṹ��*/
typedef struct
{
	HBITMAP	hBmp;
	POINT   pos;
	SIZE	size;
	BOOL	isPaused;
	int     totalDist;
	int		situation;
}GameStatus;

/*ȫ�ֱ���*/
static TCHAR szWindowClass[] = _T("win32app");
static TCHAR szTitle[] = _T("���ܰɣ�Doge!");

/*����λͼ���*/
HBITMAP m_hBackgroundBmp;
HBITMAP m_hBuildingBmp;
HBITMAP m_hHeroBmp;
HBITMAP m_hGameStatusBmp;
HBITMAP	m_hBlockBmp[BLOCK_COLOR_NUM];
HBITMAP	m_hRoofkBmp[ROOF_COLOR_NUM];
HBITMAP m_hStart;
HBITMAP m_hStoryStartBmp;

/*���巽����ɫ���飬��m_hBlockBmp[BLOCK_COLOR_NUM]������Ӧ��0��ʾ��ɫ���飬1��ʾ��ɫ���飬2��ʾ��ɫ���飬3��ʾ��ɫ����*/
int	m_blockBmpNames[] = { IDB_BLUE_BLOCK, IDB_GREEN_BLOCK, IDB_ORANGE_BLOCK, IDB_PINK_BLOCK };
/*�����ݶ���ɫ���飬��m_hRoofkBmp[ROOF_COLOR_NUM]������Ӧ��0��ʾ��ɫ�ݶ���1��ʾ��ɫ�ݶ�*/
int	m_roofBmpNames[] = { IDB_BLACK_ROOF, IDB_GREY_ROOF };

/*����Ӣ�ۡ����������Ρ���Ϸ״̬*/
Hero          m_hero;
Building      m_building;
Terrian       m_terrian[MAX_TERRIAN_NUM];
GameStatus    m_gameStatus;

/*ȫ�ֺ���*/
//������̺���
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//��ʼ��
VOID Init(HWND hWnd, WPARAM wParam, LPARAM lParam);

/*************************************************
Fucntion : ����Ӣ������
Parameter:
posX��posY��ʾ�������Ͻǵ�����
sizeX��sizeY��ʾ���ε�size
hBmp��ʾλͼ���
curFrameIndex��ǰ֡
maxFrameSize���֡��
*************************************************/
Hero CreateHero(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp, int curFrameIndex, int maxFrameSize);

/*************************************************
Fucntion : ������������
Parameter:
posX��posY��ʾ�������Ͻǵ�����
sizeX��sizeY��ʾ���ε�size
hBmp��ʾλͼ���
*************************************************/
Building CreateBuilding(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);

/*************************************************
Fucntion : ������Ϸ״̬
Parameter:
posX��posY��ʾ�������Ͻǵ�����
sizeX��sizeY��ʾ���ε�size
hBmp��ʾλͼ���
*************************************************/
GameStatus CreateGameStatus(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);

/*************************************************
Fucntion : ������������
Parameter:
posX��posY��ʾ�������Ͻǵ�����
sizeX��sizeY��ʾ���ε�size
hBlockBmp��ʾ����λͼ���
hRoofBmp��ʾ�ݶ�λͼ���
roofHeight�ݶ���Ⱥ͸߶�
blockHeight�����Ⱥ͸߶�
*************************************************/
Terrian CreateTerrian(LONG posX, LONG posY, LONG sizeX, LONG sizeY,
	HBITMAP hBlockBmp, HBITMAP hRoofBmp, int roofHeight, int blockHeight);

//˫�������
VOID Render(HWND hWnd);
//��ʱ���¼�
VOID TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam);
//hero����
VOID HeroUpdate();
//���θ���
VOID TerrianUpdate();
//��Ϸ״̬����
VOID GameStatusUpdate();
//�ж��Ƿ�����ʼ
BOOL Startd(POINT ptMouse);
//�ж��Ƿ���Ŀ�ʼ
BOOL StoryStartd(POINT ptMouse);
//�ж��Ƿ�����ͣ
BOOL Paused(POINT ptMouse);
//���̰����¼�����
VOID KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
//�����ɿ��¼�����
VOID KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam);
//��������¼�
VOID LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
