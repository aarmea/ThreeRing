#include <QApplication>
#include "mainwindow.h"

/*
void EnableCrashingOnCrashes()
{
    typedef BOOL (WINAPI *tGetPolicy)(LPDWORD lpFlags);
    typedef BOOL (WINAPI *tSetPolicy)(DWORD dwFlags);
    const DWORD EXCEPTION_SWALLOWING = 0x1;

    HMODULE kernel32 = LoadLibraryA("kernel32.dll");
    tGetPolicy pGetPolicy = (tGetPolicy)GetProcAddress(kernel32,
                "GetProcessUserModeExceptionPolicy");
    tSetPolicy pSetPolicy = (tSetPolicy)GetProcAddress(kernel32,
                "SetProcessUserModeExceptionPolicy");
    if (pGetPolicy && pSetPolicy)
    {
        DWORD dwFlags;
        if (pGetPolicy(&dwFlags))
        {
            // Turn off the filter
            pSetPolicy(dwFlags & ~EXCEPTION_SWALLOWING);
        }
    }
}
*/

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
