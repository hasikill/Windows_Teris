#include <iostream>
#include <Windows.h>
#include "CTerisWindow.h"
#include "TetrisConst.h"

int WINAPI wWinMain(HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPWSTR lpCmdLine,
  int iCmdShow)
{
  //��������
  CTerisWindow ui = CTerisWindow(hInstance, "����˹���� v1.0", SCR_WIDTH, SCR_HEIGHT);
  //ѭ��
  ui.loop();
  return 0;
}