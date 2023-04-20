#include "Screenshot.h"
#include <iostream>


CScreenshot::CScreenshot() {
    hdc = GetDC(NULL); // get the desktop device context
    hDest = CreateCompatibleDC(hdc); // create a device context to use yourself
}

CScreenshot::~CScreenshot() {
    // after the recording is done, release the desktop context you got..
    ReleaseDC(NULL, hdc);

    // ..and delete the context you created
    DeleteDC(hDest);
}

const wchar_t* CScreenshot::CaptureScreen() {
    HBITMAP hbmScreen = NULL;
    DWORD dwBmpSize = 0;
    HANDLE hDIB = NULL;
    char* lpbitmap = NULL;
    HANDLE hFile = NULL;
    DWORD dwSizeofDIB = 0;
    DWORD dwBytesWritten = 0;

    // get the height and width of the screen
    int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);

    // create a bitmap
    HBITMAP hbDesktop = CreateCompatibleBitmap(hdc, width, height);

    // use the previously created device context with the bitmap
    //SelectObject(hDest, hbDesktop);
    HBITMAP hBitmapOld = (HBITMAP)SelectObject(hDest, hbDesktop);

    // copy from the desktop device context to the bitmap device context
    // call this once per 'frame'
    BitBlt(hDest, 0, 0, width, height, hdc, 0, 0, SRCCOPY);

    // Get the HBITMAP
    hbmScreen = (HBITMAP)SelectObject(hDest, hBitmapOld);

    // Create a CImage object and attach it to the HBITMAP object
    CImage image;
    image.Attach(hbmScreen);

    // Create a temp file to store the image
    wchar_t fullpath[L_tmpnam_s];
    errno_t err = _wtmpnam_s(fullpath, L_tmpnam_s);
    if (err != 0)
    {
        std::cout << "Error creating temporary filename.\n";
        return NULL;
    }

    // Save the image as a JPEG file
    image.Save(fullpath, Gdiplus::ImageFormatJPEG);

    // ..delete the bitmap you were using to capture frames..
    DeleteObject(hbDesktop);

    size_t len = wcslen(fullpath) + 1;
    wchar_t* filename2 = (wchar_t*)malloc(len * sizeof(wchar_t));
    wcscpy_s(filename2, len, fullpath);
    return filename2;
};

