#pragma once
#include "CTetrisBlock.h"

class CTerisControl
{
public:
  CTerisControl();
  ~CTerisControl();

  /*
  方块的位置
  */
  struct tagPos
  {
    int nX;
    int nY;
  };

  /*
  地图数据标识符
  */
  enum emFlags
  {
    SPACE,  //空地
    BLOCK,  //方块
    BORDER  //边界
  };

  //移动方向
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

  //下坠
  void Drop();
  //移动
  emStatus moveBlock(emMoveDirection emDir);
  //创建方块
  void createBlock();
  //旋转
  void rotate();
  //消行
  void clearLine();
  //游戏是否结束
  bool isGameOver();
  //得分
  int getScore();

  /*
  测试指定位置是否有效
  */
  bool isOkPosition(tagPos newPos, CTetrisBlock::tagBlockMap map);
  bool isOkPosition(tagPos newPos);

  //分数
  int m_nScore;
  //当前方块位置
  tagPos m_CurBlockPos;
  //当前方块
  CTetrisBlock* m_CurBlock;
  //下一个方块
  CTetrisBlock* m_NextBlock;
  //游戏是否结束
  bool isGameStart;
};

