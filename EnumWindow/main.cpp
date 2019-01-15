#include <Windows.h>
#include <iostream>

BOOL CALLBACK EnumWindowsProc(_In_ HWND hwnd,
  _In_ LPARAM lParam)
{
  char buf[64] = { 0 };
  GetWindowTextA(hwnd, buf, 64);
  printf("%d\t%s\r\n", (int)hwnd, buf);
  return true;
}

int main()
{
  //第一种
  EnumWindows(EnumWindowsProc, 0);

  //第二种遍历方式
  HWND hWnd = GetDesktopWindow();
  char buf[64] = { 0 };
  GetWindowTextA(hWnd, buf, 64);
  printf("%d\t%s\r\n", (int)hWnd, buf);

  HWND hCur = GetWindow(hWnd, GW_CHILD);

  do
  {
    char buf[64] = { 0 };
    GetWindowTextA(hCur, buf, 64);
    printf("%d\t%s\r\n", (int)hCur, buf);
    hCur = GetWindow(hCur, GW_HWNDNEXT);
  } while (hCur != NULL);

  getchar();
}