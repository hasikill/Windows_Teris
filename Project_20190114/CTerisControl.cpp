#include "CTerisControl.h"

CTerisControl::CTerisControl()
{
  this->m_CurBlock = nullptr;
  this->m_NextBlock = nullptr;

  //初始分数
  m_nScore = 0;

  //创建方块
  createBlock();

  for (int i = 0; i < HEIGHT; i++)
  {
    for (int j = 0; j < WIDTH; j++)
    {
      if (j == 0 || j == WIDTH - 1 || i == HEIGHT - 1)
      {
        //数据写入
        g_Maps[i][j] = BORDER;
      }
    }
  }
  
  //游戏开始
  isGameStart = true;
}


CTerisControl::~CTerisControl()
{
}

void CTerisControl::Drop()
{
  //移动
  emStatus mvStatus = moveBlock(MV_DOWN);

  if (mvStatus == ST_STOP)
  { //不能再移动了,就是碰到东西停止了

    //消行并且计分
    clearLine();

    //游戏结束判断
    if (isGameOver())
    {
      MessageBox(NULL, TEXT("游戏结束!"), TEXT("GameOver"), MB_OK);
      return;
    }
    else
    {
      //重新创建方块
      createBlock();
    }
  }
  else if (mvStatus == ST_PARALLEL)
  {

  }
}

CTerisControl::emStatus CTerisControl::moveBlock(emMoveDirection emDir)
{
  switch (emDir)
  {
  case CTerisControl::MV_UP:
  {
    //暂不提供
  }
  break;
  case CTerisControl::MV_DOWN:
  {
    //给新Pos赋值
    tagPos newPos = m_CurBlockPos;

    //赋予新值
    newPos.nY++;
     
    //测试位置有效性
    if (isOkPosition(newPos))
    {
      //如果有效刷新新Pos
      m_CurBlockPos = newPos;
    }
    else
    {
      int nBlockSize = m_CurBlock->m_block.nBlockSize;
      char *pBlock = m_CurBlock->m_block.block_map;

      int nX = newPos.nX;
      int nY = newPos.nY;

      if (nY <= 0)
      {
        isGameStart = false;
        return ST_STOP;
      }

      for (int y = 0; y < nBlockSize; y++)
      {
        for (int x = 0; x < nBlockSize; x++)
        {
          int n = pBlock[y * nBlockSize + x];
          if (n == BLOCK)
          {
            g_Maps[(nY + y - 1)][(nX + x)] = n;
          }
        }
      }
      m_CurBlockPos = newPos;

      return ST_STOP;
    }
  }
  break;
  case CTerisControl::MV_LEFT:
  {
    //给新Pos赋值
    tagPos newPos = m_CurBlockPos;

    //赋予新值
    newPos.nX--;

    //测试位置有效性
    if (isOkPosition(newPos))
    {
      //如果有效刷新新Pos
      m_CurBlockPos = newPos;
    }

    return ST_PARALLEL;
  }
  case CTerisControl::MV_RIGHT:
  {
    //给新Pos赋值
    tagPos newPos = m_CurBlockPos;
    //赋予新值
    newPos.nX++;

    //测试位置有效性
    if (isOkPosition(newPos))
    {
      //如果有效刷新新Pos
      m_CurBlockPos = newPos;
    }
  }
  return ST_PARALLEL;
  }

  return ST_NONE;
}

void CTerisControl::createBlock()
{
  if (this->m_NextBlock != NULL)
  {
    //创建一个方块
    if (this->m_CurBlock != nullptr)
    {
      delete this->m_CurBlock;
      this->m_CurBlock = nullptr;
    }
    this->m_CurBlock = m_NextBlock;
    this->m_NextBlock = new CTetrisBlock();
  }
  else
  {
    //第一次
    if (this->m_CurBlock == nullptr)
    {
      this->m_NextBlock = new CTetrisBlock();
      this->m_CurBlock = new CTetrisBlock();
    }
  }

  int nBlockSize = this->m_CurBlock->m_block.nBlockSize;

  //方块位置
  this->m_CurBlockPos.nY = -nBlockSize;
  this->m_CurBlockPos.nX = (WIDTH - nBlockSize) / 2;
}

void CTerisControl::rotate()
{
  int nSize = m_CurBlock->m_block.nBlockSize;

  if (m_CurBlockPos.nX <= 0)
  {
    m_CurBlockPos.nX = 1;
  }
  if (m_CurBlockPos.nX >= WIDTH - nSize)
  {
    m_CurBlockPos.nX = WIDTH - nSize - 1;
  }

  CTetrisBlock tBlock = *m_CurBlock;
  tBlock.rotate(CTetrisBlock::RO_LEFT);

  //检查有效性
  if (isOkPosition(m_CurBlockPos, tBlock.m_block))
  {
    //调用方块的旋转函数
    m_CurBlock->rotate(CTetrisBlock::RO_LEFT);
  }
}

void CTerisControl::clearLine()
{

  int nBottom = HEIGHT - 2;

  int LineNums[HEIGHT] = { 0 };
  int nIndex = HEIGHT - 1;

  //
  for (int i = nBottom; i > 0; i--)
  {
    bool bFlag = true;

    for (int j = 1; j < WIDTH - 1; j++)
    {
      if (g_Maps[i][j] == SPACE || g_Maps[nBottom][j] == BORDER) //底部判断
      {
        bFlag = false;
        break;
      }
    }

    //添加数组
    if (bFlag)
    {
      LineNums[nIndex--] = i;
    }

  }
  //完成消行
  for (int i = nIndex + 1; i < HEIGHT; i++)
  {
    int nSize = WIDTH;
    m_nScore = m_nScore + 100;

    //取出行号
    int nNum = LineNums[i];

    for (int k = nNum - 1; k > 0; k--)
    {
      //数据清除
      ::memcpy(g_Maps[(k + 1)], g_Maps[k], nSize);
    }

  }
}

bool CTerisControl::isGameOver()
{
  return !isGameStart;
}

int CTerisControl::getScore()
{
  return m_nScore;
}

bool CTerisControl::isOkPosition(tagPos newPos, CTetrisBlock::tagBlockMap map)
{
  //方块宽度
  int nBlockSize = map.nBlockSize;

  //方块边界
  int nX = newPos.nX;
  int nY = newPos.nY;
  int nBottom = nY + nBlockSize;
  int nRight = nX + nBlockSize;

  //地图
  char *pBlock = map.block_map;

  //越界
  if (nBottom > HEIGHT || nX > WIDTH)
  {
    return false;
  }

  for (int i = 0, tY = nY; i < nBlockSize; i++, tY++)
  {
    for (int j = 0; j < nBlockSize; j++)
    {
      if (pBlock[i * nBlockSize + j] == BLOCK)
      {
        if (tY < 0)
        {
          continue;
        }
        emFlags emF = (emFlags)g_Maps[(nY + i)][(nX + j)];
        //底部判断
        if (emF == BORDER || emF == BLOCK)
        {
          //通知上层调用已到底部
          return false;
        }
      }
    }

  }

  return true;
}

bool CTerisControl::isOkPosition(tagPos newPos)
{
  return isOkPosition(newPos, m_CurBlock->m_block);
}
