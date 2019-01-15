#include "CTerisControl.h"

CTerisControl::CTerisControl()
{
  this->m_CurBlock = nullptr;
  this->m_NextBlock = nullptr;

  //��ʼ����
  m_nScore = 0;

  //��������
  createBlock();

  for (int i = 0; i < HEIGHT; i++)
  {
    for (int j = 0; j < WIDTH; j++)
    {
      if (j == 0 || j == WIDTH - 1 || i == HEIGHT - 1)
      {
        //����д��
        g_Maps[i][j] = BORDER;
      }
    }
  }
  
  //��Ϸ��ʼ
  isGameStart = true;
}


CTerisControl::~CTerisControl()
{
}

void CTerisControl::Drop()
{
  //�ƶ�
  emStatus mvStatus = moveBlock(MV_DOWN);

  if (mvStatus == ST_STOP)
  { //�������ƶ���,������������ֹͣ��

    //���в��ҼƷ�
    clearLine();

    //��Ϸ�����ж�
    if (isGameOver())
    {
      MessageBox(NULL, TEXT("��Ϸ����!"), TEXT("GameOver"), MB_OK);
      return;
    }
    else
    {
      //���´�������
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
    //�ݲ��ṩ
  }
  break;
  case CTerisControl::MV_DOWN:
  {
    //����Pos��ֵ
    tagPos newPos = m_CurBlockPos;

    //������ֵ
    newPos.nY++;
     
    //����λ����Ч��
    if (isOkPosition(newPos))
    {
      //�����Чˢ����Pos
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
    //����Pos��ֵ
    tagPos newPos = m_CurBlockPos;

    //������ֵ
    newPos.nX--;

    //����λ����Ч��
    if (isOkPosition(newPos))
    {
      //�����Чˢ����Pos
      m_CurBlockPos = newPos;
    }

    return ST_PARALLEL;
  }
  case CTerisControl::MV_RIGHT:
  {
    //����Pos��ֵ
    tagPos newPos = m_CurBlockPos;
    //������ֵ
    newPos.nX++;

    //����λ����Ч��
    if (isOkPosition(newPos))
    {
      //�����Чˢ����Pos
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
    //����һ������
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
    //��һ��
    if (this->m_CurBlock == nullptr)
    {
      this->m_NextBlock = new CTetrisBlock();
      this->m_CurBlock = new CTetrisBlock();
    }
  }

  int nBlockSize = this->m_CurBlock->m_block.nBlockSize;

  //����λ��
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

  //�����Ч��
  if (isOkPosition(m_CurBlockPos, tBlock.m_block))
  {
    //���÷������ת����
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
      if (g_Maps[i][j] == SPACE || g_Maps[nBottom][j] == BORDER) //�ײ��ж�
      {
        bFlag = false;
        break;
      }
    }

    //�������
    if (bFlag)
    {
      LineNums[nIndex--] = i;
    }

  }
  //�������
  for (int i = nIndex + 1; i < HEIGHT; i++)
  {
    int nSize = WIDTH;
    m_nScore = m_nScore + 100;

    //ȡ���к�
    int nNum = LineNums[i];

    for (int k = nNum - 1; k > 0; k--)
    {
      //�������
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
  //������
  int nBlockSize = map.nBlockSize;

  //����߽�
  int nX = newPos.nX;
  int nY = newPos.nY;
  int nBottom = nY + nBlockSize;
  int nRight = nX + nBlockSize;

  //��ͼ
  char *pBlock = map.block_map;

  //Խ��
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
        //�ײ��ж�
        if (emF == BORDER || emF == BLOCK)
        {
          //֪ͨ�ϲ�����ѵ��ײ�
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
