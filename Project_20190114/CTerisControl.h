#pragma once
#include "CTetrisBlock.h"

class CTerisControl
{
public:
  CTerisControl();
  ~CTerisControl();

  /*
  �����λ��
  */
  struct tagPos
  {
    int nX;
    int nY;
  };

  /*
  ��ͼ���ݱ�ʶ��
  */
  enum emFlags
  {
    SPACE,  //�յ�
    BLOCK,  //����
    BORDER  //�߽�
  };

  //�ƶ�����
  enum emMoveDirection
  {
    MV_UP,
    MV_DOWN,
    MV_LEFT,
    MV_RIGHT
  };

  enum emStatus
  {
    ST_STOP,
    ST_PARALLEL,
    ST_NONE
  };

  //��׹
  void Drop();
  //�ƶ�
  emStatus moveBlock(emMoveDirection emDir);
  //��������
  void createBlock();
  //��ת
  void rotate();
  //����
  void clearLine();
  //��Ϸ�Ƿ����
  bool isGameOver();
  //�÷�
  int getScore();

  /*
  ����ָ��λ���Ƿ���Ч
  */
  bool isOkPosition(tagPos newPos, CTetrisBlock::tagBlockMap map);
  bool isOkPosition(tagPos newPos);

  //����
  int m_nScore;
  //��ǰ����λ��
  tagPos m_CurBlockPos;
  //��ǰ����
  CTetrisBlock* m_CurBlock;
  //��һ������
  CTetrisBlock* m_NextBlock;
  //��Ϸ�Ƿ����
  bool isGameStart;
};

