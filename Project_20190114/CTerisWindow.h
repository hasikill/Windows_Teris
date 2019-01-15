#pragma once
#include <Windows.h>
#include <string>
#include "CErrorLog.h"

using namespace std;

//���ڹ���
LRESULT WINAPI MyWinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//���̰���
void KeyDown(HWND hWnd, int nKey);

//����ͼ
void DrawMap(HDC hdc);

//����ָ�����Ե�ֱ��
void DrawLine(HDC hDC, int x0, int y0, int x1, int y1, int style, int width, COLORREF color);

//����ʵ��Բ
void DrawCircle(HDC hDC, int x, int y, int len, COLORREF color);

//����������
void DrawRect(HDC hDC, int left, int top, int width, int height, int style, COLORREF color);

//����λͼ������
void DrawBmpRect(HDC hDC, int left, int top, int width, int height, LPCTSTR file);

class CTerisWindow
{
public:
  CTerisWindow(HINSTANCE hInstance, string strTitle, int nWidth, int nHeight);
  ~CTerisWindow();

  //ѭ��
  void loop();

private:
  //������
  WNDCLASSEX m_tagWcs;
  //������־����
  CErrorLog m_objLog;
  //���ھ��
  HWND m_hWnd;
  //ʵ�����
  HINSTANCE m_hInstance;
  //������Ϣ�ṹָ��
  LPCREATESTRUCT m_pCs;
  //�豸�������
  HDC m_hDc;
  //��ʼ��״̬
  bool m_bInit;
};

