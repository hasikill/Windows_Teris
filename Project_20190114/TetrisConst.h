#pragma once

#define BLOCKCOUNT  7     //������������

#define WIDTH       22    //��ͼ���
#define HEIGHT      26
#define GAMESPEED   250   //��������ٶ�

#define SCR_WIDTH   800
#define SCR_HEIGHT  600
#define BLOCK_SIZE  SCR_HEIGHT / 20

extern char g_Blocks[BLOCKCOUNT][8][8];
extern char g_Block_sizes[BLOCKCOUNT];

extern char g_Maps[HEIGHT][WIDTH];

//��ɫ��
extern const float colorTable[8][4];

//���ε�ɫ������ͻ������Ч��
extern const float lightColorTable[8][4];

//������ɫ������ͻ������Ч��
extern const float darkColorTable[8][4];
