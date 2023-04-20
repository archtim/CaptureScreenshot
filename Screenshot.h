#pragma once

#include <iostream>
#include <d3d9helper.h>
#include <iostream>
#include <ole2.h>
#include <olectl.h>
#include <time.h>
#include <string>
#include <atlstr.h>
#include <atlimage.h>


class CScreenshot
{
public:
    CScreenshot();
    ~CScreenshot();
    const wchar_t* CaptureScreen();

private:
    HDC hdc;
    HDC hDest;
    //CDGInternet DGInternet;
};

