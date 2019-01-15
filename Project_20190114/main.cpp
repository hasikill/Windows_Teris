#include <iostream>
#include <Windows.h>
#include "CTerisWindow.h"
#include "TetrisConst.h"

int WINAPI wWinMain(HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPWSTR lpCmdLine,
  int iCmdShow)
{
  //创建窗口
  CTerisWindow ui = CTerisWindow(hInstance, "俄罗斯方块 v1.0", SCR_WIDTH, SCR_HEIGHT);
  //循环
  ui.loop();
  return 0;
}