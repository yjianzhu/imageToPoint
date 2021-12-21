#include "terminal.h"

#ifdef __WINDOWS_
#include <windows.h>
#include <io.h>
#include <fcntl.h>

#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

short terminal_columns()
{
#ifdef WINDOWS
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        int ret;
        ret = GetConsoleScreenBufferInfo(GetStdHandle( STD_OUTPUT_HANDLE ),&csbi);
        return ret?csbi.dwSize.X:0;
#else
        struct winsize win;
        ioctl(1, TIOCGWINSZ, &win);
        return win.ws_col;
#endif
}

short terminal_rows()
{
#ifdef __WINDOWS_
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        int ret;
        ret = GetConsoleScreenBufferInfo(GetStdHandle( STD_OUTPUT_HANDLE ),&csbi);
        return ret?csbi.dwSize.Y:0;
#else
        struct winsize win;
        ioctl(1, TIOCGWINSZ, &win);
        return win.ws_row;
#endif
}