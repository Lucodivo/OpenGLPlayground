//
// Created by Connor on 2/8/2020.
//
#pragma once

#include <windows.h>
#include "../LearnOpenGLPlatform.h"
#include <iostream>

uint32 getFileLastWriteTime(const char* filePath) {
  WIN32_FIND_DATA FindFileData = {};
  HANDLE hFind;

  hFind = FindFirstFile(filePath, &FindFileData);
  if (hFind == INVALID_HANDLE_VALUE)
  {
    std::cout << "Can't find file: " << filePath;
  }
  else
  {
    FindClose(hFind);
  }

  return FindFileData.ftLastWriteTime.dwLowDateTime;
}