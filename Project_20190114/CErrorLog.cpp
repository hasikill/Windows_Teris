#include "CErrorLog.h"
#include <Windows.h>
#pragma warning(disable:4996)

CErrorLog::CErrorLog(const char *strFile)
{
  //检查
  if (strFile == nullptr) return;
  //初始化
  fp = nullptr;
  //打开文件
  fp = fopen(strFile, "a");
}

CErrorLog::~CErrorLog()
{
  if (fp != nullptr)
  {
    fclose(fp);
  }
}

void CErrorLog::OutLastError(const char * szSource, int nLine)
{
  if (fp == nullptr)
  {
    return;
  }

  char *pBuffer = nullptr;
  string strLogText = "错误 : ";

  //取消息文本
  FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
    NULL,
    GetLastError(),
    NULL,
    (LPSTR)&pBuffer,
    NULL,
    NULL);

  //写入
  if (pBuffer != nullptr)
  {
    //临时缓冲区
    char buf[20] = { 0 };

    //拼接
    strLogText.append(pBuffer).append(" 源:").append(szSource).append(" 行号:").append(itoa(nLine, buf, 10)).append(" 日期:").append(__DATE__);

    //写入文件
    fputs(strLogText.c_str(), fp);
    //换行
    fputc('\n', fp);

    //释放
    LocalFree(pBuffer);

    //刷新
    fflush(fp);
  }

}

void CErrorLog::OutLastErrorInFile(const char * fileName, const char * szSource, int nLine)
{
  char *pBuffer = nullptr;
  string strLogText = "错误 : ";

  //取消息文本
  FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
    NULL,
    GetLastError(),
    NULL,
    (LPSTR)&pBuffer,
    NULL,
    NULL);

  //打开文件
  FILE *fp = fopen(fileName, "a");
  if (fp == nullptr)
  {
    goto ExitPoint;
  }

ExitPoint:
  if (pBuffer != nullptr)
  {
    //临时缓冲区
    char buf[20] = { 0 };
    //拼接
    strLogText.append(pBuffer).append(" 源:").append(szSource).append(" 行号:").append(itoa(nLine, buf, 10)).append(" 日期:").append(__DATE__);

    //写入文件
    fputs(strLogText.c_str(), fp);
    //换行
    fputc('\n', fp);

    //释放
    LocalFree(pBuffer);
  }

  if (fp != nullptr)
  {
    fclose(fp);
  }
}
