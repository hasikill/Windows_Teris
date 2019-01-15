#pragma once
#include <Windows.h>
#include <string>
#include "CErrorLog.h"

using namespace std;

//窗口过程
LRESULT WINAPI MyWinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//键盘按键
void KeyDown(HWND hWnd, int nKey);

//画地图
void DrawMap(HDC hdc);

//绘制指定属性的直线
void DrawLine(HDC hDC, int x0, int y0, int x1, int y1, int style, int width, COLORREF color);

//绘制实心圆
void DrawCircle(HDC hDC, int x, int y, int len, COLORREF color);

//绘制填充矩形
void DrawRect(HDC hDC, int left, int top, int width, int height, int style, COLORREF color);

//绘制位图填充矩形
void DrawBmpRect(HDC hDC, int left, int top, int width, int height, LPCTSTR file);

class CTerisWindow
{
public:
  CTerisWindow(HINSTANCE hInstance, string strTitle, int nWidth, int nHeight);
  ~CTerisWindow();

  //循环
  void loop();

private:
  //窗口类
  WNDCLASSEX m_tagWcs;
  //错误日志对象
  CErrorLog m_objLog;
  //窗口句柄
  HWND m_hWnd;
  //实例句柄
  HINSTANCE m_hInstance;
  //创建信息结构指针
  LPCREATESTRUCT m_pCs;
  //设备环境句柄
  HDC m_hDc;
  //初始化状态
  bool m_bInit;
};

