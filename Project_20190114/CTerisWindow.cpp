#include "CTerisWindow.h"
#include <tchar.h>
#include "TetrisConst.h"
#include "CTerisControl.h"

CTerisControl g_Control;

CTerisWindow::CTerisWindow(HINSTANCE hInstance, string strTitle, int nWidth, int nHeight) : m_objLog("./ErrorLog.log")
{
  //是否初始化
  m_bInit = false;
  m_hWnd = nullptr;
  m_pCs = nullptr;
  m_hDc = nullptr;
  m_hInstance = hInstance;

  //获取结构体大小
  int nSize = sizeof(m_tagWcs);

  //置0
  ::memset(&m_tagWcs, 0, nSize);

  //填充WNDCLASS
  m_tagWcs.cbSize = nSize;
  m_tagWcs.hInstance = hInstance;
  m_tagWcs.lpfnWndProc = MyWinProc;
  m_tagWcs.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  m_tagWcs.lpszClassName = "C Teris";

  //注册窗口类
  ATOM atom = RegisterClassEx(&m_tagWcs);
  if (atom == 0)
  {
    m_objLog.OutLastError(__FILE__, __LINE__);
    m_bInit = false;
    return;
  }

  //创建窗口
  m_hWnd = CreateWindow(_T("C Teris"),
    strTitle.c_str(),
    WS_OVERLAPPEDWINDOW,
    0,
    0,
    nWidth,
    nHeight,
    NULL,
    NULL,
    hInstance,
    NULL);
  if (m_hWnd == NULL)
  {
    m_objLog.OutLastError(__FILE__, __LINE__);
    m_bInit = false;
    return;
  }

  //创建完毕
  m_bInit = true;
}

CTerisWindow::~CTerisWindow()
{
}

void CTerisWindow::loop()
{
  if (m_bInit != false)
  {
    //显示窗口
    ShowWindow(m_hWnd, SW_NORMAL);

    //更新窗口
    if (!UpdateWindow(m_hWnd))
    {
      m_objLog.OutLastError(__FILE__, __LINE__);
      m_bInit = false;
      return;
    }

    //消息结构体
    MSG msg = { 0 };

    //函数检索
    BOOL bRet = true;

    //消息循环
    while ((bRet = GetMessage(&msg, NULL, 0, 0)) != false)
    {
      if (bRet == -1)
      {
        m_objLog.OutLastError(__FILE__, __LINE__);
        m_bInit = false;
        break;
      }
      else
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
  }
}


/*
  窗口过程
*/
LRESULT WINAPI MyWinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT ret = 1;
  switch (uMsg)
  {
    case WM_CREATE:
    {
      //信息结构
      LPCREATESTRUCT cs = (LPCREATESTRUCT)lParam;

      //设置时钟
      SetTimer(hWnd, 1, 300, NULL);
    }
    break;
    case WM_PAINT:
    {
      //获取客户区DC
      HDC hdc = GetDC(hWnd);
      //创建内存DC
      HDC hMemDc = CreateCompatibleDC(hdc);
      RECT rc;
      GetClientRect(hWnd, &rc);
      //创建位图
      HBITMAP hBitmap = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
      //选择对象
      SelectObject(hMemDc, hBitmap);
      //往内存DC写入数据
      DrawMap(hMemDc);
      //屏幕交换
      BitBlt(hdc, 0, 0, rc.right, rc.bottom, hMemDc, 0, 0, SRCCOPY);

      //BITMAPINFO bmi;
      //GetDIBits(hMemDc, hBitmap, 0, rc.bottom, NULL, &bmi, DIB_RGB_COLORS);

      //释放DC
      if (hdc != NULL)
      {
        ReleaseDC(hWnd, hdc);
      }
      if (hMemDc != NULL)
      {
        DeleteDC(hMemDc);
      }
      if (hBitmap != NULL)
      {
        DeleteObject(hBitmap);
      }
      //置无效区
      ValidateRect(hWnd, NULL);
    }
    break;
    case WM_KEYDOWN:
    {
      if (!g_Control.isGameOver())
      {
        //获取按键值
        int nKey = (int)wParam;
        //按键被按下
        KeyDown(hWnd, nKey);
      }
      else
      {
        if (MessageBox(hWnd, TEXT("游戏已结束,是否退出?"), TEXT("Teris"), MB_OKCANCEL) == IDOK)
        {
          PostQuitMessage(0);
        }
      }
    }
    break;
    case WM_TIMER:
    {
      if (!g_Control.isGameOver())
      {
        g_Control.Drop();
        InvalidateRect(hWnd, NULL, true);
      }
      
    }
    break;
    case WM_DESTROY:
    {
      PostQuitMessage(0);
    }
    break;
    case WM_CLOSE:
    {
      int opt = MessageBox(hWnd, _T("您真的要退出吗?"), _T("挽留一下"), MB_OKCANCEL);
      if (opt == IDOK)
      {
        DestroyWindow(hWnd);
      }
    }
    break;
    default:
    {
      ret = DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    break;
  }
  return ret;
}

void KeyDown(HWND hWnd, int nKey)
{
  if (nKey == VK_LEFT)
  {
    g_Control.moveBlock(CTerisControl::MV_LEFT);
    InvalidateRect(hWnd, NULL, true);
  }
  else if (nKey == VK_RIGHT)
  {
    g_Control.moveBlock(CTerisControl::MV_RIGHT);
    InvalidateRect(hWnd, NULL, true);
  }
  else if (nKey == VK_UP)
  {
    g_Control.rotate();
    InvalidateRect(hWnd, NULL, true);
  }
  else if (nKey == VK_DOWN)
  {
    g_Control.Drop();
    InvalidateRect(hWnd, NULL, true);
  }
}

void DrawMap(HDC hdc)
{
  int nSize = 20;
  int nXOffset = 50;

  CTerisControl::tagPos pos = g_Control.m_CurBlockPos;

  int size = g_Control.m_CurBlock->m_block.nBlockSize;
  char *map = g_Control.m_CurBlock->m_block.block_map;

  DrawRect(hdc, 0, 0, 800, 600, 0, RGB(255, 255, 255));

  //当前活动方块
  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
    {
      if (map[i * size + j] == CTerisControl::BLOCK)
      {
        int x = (pos.nX + j) * nSize + nXOffset;
        int y = (pos.nY + i) * nSize;
        DrawRect(hdc, x, y, nSize, nSize, 0, RGB(100, 10, 20));
      }
    }
  }

  for (int i = 0; i < HEIGHT; i++)
  {
    for (int j = 0; j < WIDTH; j++)
    {
      if (g_Maps[i][j] == CTerisControl::BLOCK)
      {
        DrawRect(hdc, j * nSize + nXOffset, i * nSize, nSize, nSize, 0, RGB(100, 10, 20));
      }
      else if (g_Maps[i][j] == CTerisControl::BORDER)
      {
        DrawRect(hdc, j * nSize + nXOffset, i * nSize, nSize, nSize, 0, RGB(155, 199, 20));
      }
    }
  }

  //获取分数
  char szBuf[64] = { 0 };
  snprintf(szBuf, 64, "分数 : %d", g_Control.getScore());
  //设置坐标
  TextOutA(hdc, 530, 50, szBuf, strlen(szBuf));

}

//绘制指定属性的直线
void DrawLine(HDC hDC, int x0, int y0, int x1, int y1, int style, int width, COLORREF color)
{
  HPEN hPen = CreatePen(style, width, color);
  HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

  MoveToEx(hDC, x0, y0, NULL);
  LineTo(hDC, x1, y1);

  SelectObject(hDC, hOldPen);
  DeleteObject(hPen);
}

//绘制实心圆
void DrawCircle(HDC hDC, int x, int y, int len, COLORREF color)
{
  HBRUSH hBrush = CreateSolidBrush(color);
  HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

  HPEN hPen = CreatePen(PS_SOLID, 1, color);
  HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

  Ellipse(hDC, x - len / 2, y - len / 2, x + len / 2, y + len / 2);

  SelectObject(hDC, hOldBrush);
  DeleteObject(hPen);

  SelectObject(hDC, hOldPen);
  DeleteObject(hOldBrush);
}

//绘制填充矩形
void DrawRect(HDC hDC, int left, int top, int width, int height, int style, COLORREF color)
{
  HBRUSH hBrush = CreateSolidBrush(color);
  HBRUSH hOldBrush = NULL;
  if (hBrush != NULL)
  {
    hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
  }

  Rectangle(hDC, left, top, left + width, top + height);

  if (hOldBrush != NULL)
  {
    SelectObject(hDC, hOldBrush);
    DeleteObject(hBrush);
  }
}

//绘制位图填充矩形
void DrawBmpRect(HDC hDC, int left, int top, int width, int height, LPCTSTR file)
{
  HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, file, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
  HBRUSH hBrush = CreatePatternBrush(hBitmap);
  HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

  Rectangle(hDC, left, top, left + width, top + height);

  SelectObject(hDC, hOldBrush);
  DeleteObject(hOldBrush);
  DeleteObject(hBitmap);
}