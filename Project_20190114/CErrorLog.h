#pragma once
#include <stdio.h>
#include <string>

using namespace std;
class CErrorLog
{
public:
  CErrorLog(const char *strFile);
  ~CErrorLog();

  void OutLastError(const char* szSource, int nLine);

  void OutLastErrorInFile(const char* fileName, const char* szSource, int nLine);

private:
  FILE * fp;
  string strFileName;
};

