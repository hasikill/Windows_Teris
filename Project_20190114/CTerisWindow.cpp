#include "CTerisWindow.h"
#include <tchar.h>
#include "TetrisConst.h"
#include "CTerisControl.h"

CTerisControl g_Control;

CTerisWindow::CTerisWindow(HINSTANCE hInstance, string strTitle, int nWidth, int nHeight) : m_objLog("./ErrorLog.log")
{
  //�Ƿ��ʼ��
  m_bInit = false;
  m_hWnd = nullptr;
  m_pCs = nullptr;
  m_hDc = nullptr;
  m_hInstance = hInstance;

  //��ȡ�ṹ���С
  int nSize = sizeof(m_tagWcs);

  //��0
  ::memset(&m_tagWcs, 0, nSize);

  //���WNDCLASS
  m_tagWcs.cbSize = nSize;
  m_tagWcs.hInstance = hInstance;
  m_tagWcs.lpfnWndProc = MyWinProc;
  m_tagWcs.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  m_tagWcs.lpszClassName = "C Teris";

  //ע�ᴰ����
  ATOM atom = RegisterClassEx(&m_tagWcs);
  if (atom == 0)
  {
    m_objLog.OutLastError(__FILE__, __LINE__);
    m_bInit = false;
    return;
  }

  //��������
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

  //�������
  m_bInit = true;
}

CTerisWindow::~CTerisWindow()
{
}

void CTerisWindow::loop()
{
  if (m_bInit != false)
  {
    //��ʾ����
    ShowWindow(m_hWnd, SW_NORMAL);

    //���´���
    if (!UpdateWindow(m_hWnd))
    {
      m_objLog.OutLastError(__FILE__, __LINE__);
      m_bInit = false;
      return;
    }

    //��Ϣ�ṹ��
    MSG msg = { 0 };

    //��������
    BOOL bRet = true;

    //��Ϣѭ��
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
  ���ڹ���
*/
LRESULT WINAPI MyWinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT ret = 1;
  switch (uMsg)
  {
    case WM_CREATE:
    {
      //��Ϣ�ṹ
      LPCREATESTRUCT cs = (LPCREATESTRUCT)lParam;

      //����ʱ��
      SetTimer(hWnd, 1, 300, NULL);
    }
    break;
    case WM_PAINT:
    {
      //��ȡ�ͻ���DC
      HDC hdc = GetDC(hWnd);
      //�����ڴ�DC
      HDC hMemDc = CreateCompatibleDC(hdc);
      RECT rc;
      GetClientRect(hWnd, &rc);
      //����λͼ
      HBITMAP hBitmap = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
      //ѡ�����
      SelectObject(hMemDc, hBitmap);
      //���ڴ�DCд������
      DrawMap(hMemDc);
      //��Ļ����
      BitBlt(hdc, 0, 0, rc.right, rc.bottom, hMemDc, 0, 0, SRCCOPY);

      //BITMAPINFO bmi;
      //GetDIBits(hMemDc, hBitmap, 0, rc.bottom, NULL, &bmi, DIB_RGB_COLORS);

      //�ͷ�DC
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
      //����Ч��
      ValidateRect(hWnd, NULL);
    }
    break;
    case WM_KEYDOWN:
    {
      if (!g_Control.isGameOver())
      {
        //��ȡ����ֵ
        int nKey = (int)wParam;
        //����������
        KeyDown(hWnd, nKey);
      }
      else
      {
        if (MessageBox(hWnd, TEXT("��Ϸ�ѽ���,�Ƿ��˳�?"), TEXT("Teris"), MB_OKCANCEL) == IDOK)
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
      int opt = MessageBox(hWnd, _T("�����Ҫ�˳���?"), _T("����һ��"), MB_OKCANCEL);
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

  //��ǰ�����
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

  //��ȡ����
  char szBuf[64] = { 0 };
  snprintf(szBuf, 64, "���� : %d", g_Control.getScore());
  //��������
  TextOutA(hdc, 530, 50, szBuf, strlen(szBuf));

}

//����ָ�����Ե�ֱ��
void DrawLine(HDC hDC, int x0, int y0, int x1, int y1, int style, int width, COLORREF color)
{
  HPEN hPen = CreatePen(style, width, color);
  HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

  MoveToEx(hDC, x0, y0, NULL);
  LineTo(hDC, x1, y1);

  SelectObject(hDC, hOldPen);
  DeleteObject(hPen);
}

//����ʵ��Բ
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

//����������
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

//����λͼ������
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