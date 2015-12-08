#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <math.h>
#include <time.h>
#include "resource.h"

/*����������WNDWIDTH��WNDHEIGHTΪ���ڵĿ�͸�*/
#define TIMER_ID             1
#define JUMP_TIMER			 2
#define DOWN_TIMER			 3
#define TIMER_ELAPSE         20
#define WNDWIDTH             720
#define WNDHEIGHT            430
#define MAX_TERRIAN_NUM      24

//������Դ�ĳߴ�         
#define HERO_SIZE_X          53
#define HERO_SIZE_Y          50
#define HERO_MAX_FRAME_NUM   9
#define STEP_SIZE_X          64
#define STEP_SIZE_Y          64
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

/*���νṹ�壬���η���λͼ��λ�á���С*/
typedef struct
{
	HBITMAP hStepBmp;
	POINT	pos;
	SIZE    size;
	int     stepWidth;
	int     stepHeight;
}Terrian;

/*��Ϸ״̬�ṹ��*/
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

/*ȫ�ֱ���*/
static TCHAR szWindowClass[] = _T("win32app");
static TCHAR szTitle[] = _T("���ܰɣ�Doge!");

/*����λͼ���*/
HBITMAP m_hBackgroundBmp;
HBITMAP m_hHeroBmp;
HBITMAP m_hGameStatusBmp;
HBITMAP	m_hStepBmp;
HBITMAP m_hStartBmp;
HBITMAP m_hStoryStartBmp;
HBITMAP m_hDeadBmp;
HBITMAP m_hBoss1Bmp;
HBITMAP m_hBoss2Bmp;
HBITMAP m_hBoss3Bmp;
HBITMAP m_hBoss4Bmp;
HBITMAP m_hBoss5Bmp;
HBITMAP m_hBoss6Bmp;
HBITMAP m_hBoss7Bmp;
HBITMAP m_hBoss8Bmp;
HBITMAP m_hStoryEndBmp;
HBITMAP m_hEndBmp;

/*����Ӣ�ۡ����Ρ���Ϸ״̬*/
Hero          m_hero;
Terrian       m_terrian[MAX_TERRIAN_NUM];
GameStatus    m_gameStatus;
int			  jump_status;
int			  down_status;
int			  speed_jump;
int			  terriantype[24][5] = { 0, 0, 0, 0, 0, 
									 5, 0, 0, 0, 5,//1
									 3, 0, 0, 0, 3,//2
									 1, 0, 0, 0, 1,//3
									 5, 0, 0, 0, 0,//4
									 3, 0, 0, 0, 0,//5
									 1, 0, 0, 0, 0,//6
									 0, 0, 0, 5, 5,//7
									 0, 0, 0, 3, 3,//8
									 0, 0, 0, 1, 1,//9
									 0, 5, 5, 5, 5,//10
									 0, 3, 3, 3, 3,//11
									 0, 1, 1, 1, 1,//12
									 0, 5, 4, 3, 3,//13
									 0, 3, 2, 1, 1,//14
									 5, 4, 3, 5, 5,//15
									 3, 2, 1, 3, 3,//16
									 3, 0, 0, 0, 5,//17
									 1, 0, 0, 0, 3,//18
									 3, 5, 0, 0, 3,//19
									 1, 3, 0, 0, 1,//20
									 5, 5, 5, 5, 5,//21
									 3, 3, 3, 3, 3,//22 
									 1, 1, 1, 1, 1 };//23
int			  terriansituation;
int           difficulty;

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
hStepBmp��ʾ����λͼ���
roofHeight�ݶ���Ⱥ͸߶�
blockHeight�����Ⱥ͸߶�
*************************************************/
Terrian CreateTerrian(LONG posX, LONG posY, LONG sizeX, LONG sizeY,
	HBITMAP hStepBmp, int stepHeight);

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
//�ж��Ƿ����¿�ʼ
BOOL Restart(POINT ptMouse);
//�ж�PPT�Ƿ��ӳ
BOOL Continued(POINT ptMouse);
//����ж�
int BeBorn();
//����ײ���
void RightCollision();
//׷�����
void ChaseTest();
//��ؼ��
void DownTest(HWND hWnd);
//��������
void Dead(HWND hWnd);
//�������
void RandTerrian();
//Boss�ط�ӳ��״̬ת��
void BossStart(HWND hWnd);
//���̰����¼�����
VOID KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
//��������¼�
VOID LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
