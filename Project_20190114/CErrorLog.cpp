#include "CErrorLog.h"
#include <Windows.h>
#pragma warning(disable:4996)

CErrorLog::CErrorLog(const char *strFile)
{
  //���
  if (strFile == nullptr) return;
  //��ʼ��
  fp = nullptr;
  //���ļ�
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
  string strLogText = "���� : ";

  //ȡ��Ϣ�ı�
  FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
    NULL,
    GetLastError(),
    NULL,
    (LPSTR)&pBuffer,
    NULL,
    NULL);

  //д��
  if (pBuffer != nullptr)
  {
    //��ʱ������
    char buf[20] = { 0 };

    //ƴ��
    strLogText.append(pBuffer).append(" Դ:").append(szSource).append(" �к�:").append(itoa(nLine, buf, 10)).append(" ����:").append(__DATE__);

    //д���ļ�
    fputs(strLogText.c_str(), fp);
    //����
    fputc('\n', fp);

    //�ͷ�
    LocalFree(pBuffer);

    //ˢ��
    fflush(fp);
  }

}

void CErrorLog::OutLastErrorInFile(const char * fileName, const char * szSource, int nLine)
{
  char *pBuffer = nullptr;
  string strLogText = "���� : ";

  //ȡ��Ϣ�ı�
  FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
    NULL,
    GetLastError(),
    NULL,
    (LPSTR)&pBuffer,
    NULL,
    NULL);

  //���ļ�
  FILE *fp = fopen(fileName, "a");
  if (fp == nullptr)
  {
    goto ExitPoint;
  }

ExitPoint:
  if (pBuffer != nullptr)
  {
    //��ʱ������
    char buf[20] = { 0 };
    //ƴ��
    strLogText.append(pBuffer).append(" Դ:").append(szSource).append(" �к�:").append(itoa(nLine, buf, 10)).append(" ����:").append(__DATE__);

    //д���ļ�
    fputs(strLogText.c_str(), fp);
    //����
    fputc('\n', fp);

    //�ͷ�
    LocalFree(pBuffer);
  }

  if (fp != nullptr)
  {
    fclose(fp);
  }
}
