#include "lnk_tool.h"

#include <QtWin>

QPixmap loadIcon(const QString &file_path)
{
    HICON hicon = 0;
    auto filePathWideChars = new wchar_t[file_path.length() + 1];
    QString windows_file_path = file_path;
    windows_file_path = windows_file_path.replace(QChar('/'), QChar('\\'));
    windows_file_path.toWCharArray(filePathWideChars);
    filePathWideChars[windows_file_path.length()] = 0;
    auto fileInfo = new SHFILEINFO();
    if (SHGetFileInfoW(filePathWideChars, FILE_ATTRIBUTE_NORMAL, fileInfo, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_LARGEICON))
    {
        hicon = fileInfo->hIcon;
    }
    delete[] filePathWideChars;
    delete fileInfo;
    QPixmap pixmap = QtWin::fromHICON(hicon);
    DestroyIcon(hicon);
    return pixmap;
}
