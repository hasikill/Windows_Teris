#pragma once
#include "TetrisConst.h"
#include <Windows.h>

/*
  ������
*/
class CTetrisBlock
{
public: //������
 
  //��ת����
  enum emRotateDirection
  {
    NONE, //��Чֵ
    RO_LEFT, //����
    RO_RIGHT //����
  };

  //�ƶ�����
  enum emMoveDirection
  {
    MV_UP,
    MV_DOWN,
    MV_LEFT,
    MV_RIGHT
  };

  //����
  struct tagBlockMap
  {
    int nBlockSize;
    char *block_map;
  };

public: 
  //ÿһ�������ͼ��
  tagBlockMap m_block;

//----------------------------- �ӿ� -------------------------------

  //���캯��
  CTetrisBlock();
  CTetrisBlock(tagBlockMap block_map);
  CTetrisBlock(CTetrisBlock & block);

  //����
  ~CTetrisBlock();

  //����һ��ָ��ͼ��ָ����ɫ�ķ���
  bool create(tagBlockMap block_map);

  //����һ�����ͼ�������ɫ�ķ���
  bool create();
  
  //��ת
  void rotate(emRotateDirection emDir);

  //�˹���ֻ�ܴ�ӡblock_map��,��ʾ���ܲ��Ǵ˶�����ӵ��
  void display();

  //����
  void destroy();

};