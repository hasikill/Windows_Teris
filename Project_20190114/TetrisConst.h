#pragma once

#define BLOCKCOUNT  7     //方块种类数量

#define WIDTH       22    //地图宽高
#define HEIGHT      26
#define GAMESPEED   250   //方块掉落速度

#define SCR_WIDTH   800
#define SCR_HEIGHT  600
#define BLOCK_SIZE  SCR_HEIGHT / 20

extern char g_Blocks[BLOCKCOUNT][8][8];
extern char g_Block_sizes[BLOCKCOUNT];

extern char g_Maps[HEIGHT][WIDTH];

//颜色表
extern const float colorTable[8][4];

//矩形淡色，用于突出立体效果
extern const float lightColorTable[8][4];

//矩形深色，用于突出立体效果
extern const float darkColorTable[8][4];
