#include <stdio.h>
#include <windows.h>
#include <winuser.h>
#include <conio.h>

#pragma comment(lib, "User32.lib")

void moveMouse(int, int);
void setupHotkeyOutput();
void printSucc(char[256]);
void printHotkey(BOOL, int);
void printError(int);
void printErrorStr(char[256]);
void printInfo(char[256]);
void printDivider(void);
void printRec(char[256]);
void printDBG(int);

void turnOn();
void turnOff();

int rowsInConsole;
int colsInConsole;
BOOL WINAPI consoleHandler(DWORD signal)
{
    if (signal == CTRL_C_EVENT)
    {
        printf("\x1b[?25h \x1b[?12h \x1b[2S\x1b[%d;5H\x1b[31mB\x1b[35my\x1b[34me \x1b[36mB\x1b[32my\x1b[33me\x1b[0m\x1b[1S\x1b[r\x1b[%d;0H", rowsInConsole, rowsInConsole); // set SGR to default, show cursor, enable cursor blinking
    }
    ExitProcess(1);
    return TRUE;
}

int currentHeaderLine = 0;
int hotkeyHeaderLine = 0;
char fstr[256];

BOOL numlockActive;
BOOL leftActive;
BOOL upActive;
BOOL rightActive;
BOOL downActive;

int main(int argc, char const *argv[])
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    rowsInConsole = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    colsInConsole = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    if (!SetConsoleCtrlHandler(consoleHandler, TRUE))
    {
        printf("\x1b[32m\nERROR: Could not set control handler\x1b[0m");
        return 1;
    }

    printf("\x1b[2J\x1b[0;0H\x1b[?25l");                                                                                                  // clear disply \ cursor to 00 \ hide cursor
    printf("\x1b[32m");                                                                                                                   // set foreground color to green
    printf("\x1b[%dC-=-=-=-=-=-=-=-=- \x1b[4mDefinitive Virtual Stylus\x1b[24m -=-=-=-=-=-=-=-=-\x1b[0m\x1b[2E", colsInConsole / 2 - 31); // \x1b[4m so set underlined
    currentHeaderLine += 2;

    if (RegisterHotKey(
            NULL,
            0,
            0,
            VK_NUMLOCK))
    {
        printSucc("Hotkey Numlock registered");
    }
    else
    {
        printErrorStr("Hotkey Numlock couldn't be registered");
        printErrorStr("Exiting...");
        ExitProcess(1);
        return 1;
    }

    hotkeyHeaderLine = currentHeaderLine;
    setupHotkeyOutput();

    printDivider();
    printf("\x1b[%d;%dr", currentHeaderLine, rowsInConsole);
    printf("\x1b[%d;0H", rowsInConsole);

    numlockActive = GetKeyState(VK_NUMLOCK) & 1 ? TRUE : FALSE;
    numlockActive ? turnOff() : turnOn();
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        if (msg.message == WM_HOTKEY)
        {
            if (GetKeyState(VK_NUMLOCK) & 0x8000)
            {
                if (numlockActive)
                {
                    numlockActive = FALSE;
                    turnOn();
                }
                else
                {
                    numlockActive = TRUE;
                    turnOff();
                }
            }
            else
            {
                if (GetKeyState(VK_LEFT) & 0x8000)
                {
                    moveMouse(-5, 0);
                }
                if (GetKeyState(VK_UP) & 0x8000)
                {
                    moveMouse(0, -5);
                }
                if (GetKeyState(VK_RIGHT) & 0x8000)
                {
                    moveMouse(5, 0);
                }
                if (GetKeyState(VK_DOWN) & 0x8000)
                {
                    moveMouse(0, 5);
                }
            }
        }
    }

    printf("\x1b[0m\x1b[?25h\x1b[?12h\x1b[r\x1b[%d;0H", rowsInConsole - 1);
    return 0;
}

void moveMouse(int x, int y)
{
    // if (GetKeyState(VK_SHIFT) & 0x8000)
    // {
    x *= 4;
    y *= 4;
    // }

    POINT p;
    GetCursorPos(&p);
    INPUT inp;
    inp.type = INPUT_MOUSE;
    inp.mi.mouseData = 0;
    inp.mi.time = 0;
    inp.mi.dx = x;
    inp.mi.dy = y;
    inp.mi.dwFlags = MOUSEEVENTF_MOVE;
    SendInput(1, &inp, sizeof(inp));
}

void setupHotkeyOutput()
{
    printf("\x1b[32m\x1b[0m");
    printf("\x1b[90m\x1b[5GInsert\x1b[11G/Period\x1b[0m");
    printf("\x1b[1E");
    printf("\x1b[32m\x1b[0m");
    printf("\x1b[90m\x1b[5GLeft\x1b[11G/4\x1b[0m");
    printf("\x1b[1E");
    printf("\x1b[32m\x1b[0m");
    printf("\x1b[90m\x1b[5GUp\x1b[11G/8\x1b[0m");
    printf("\x1b[1E");
    printf("\x1b[32m\x1b[0m");
    printf("\x1b[90m\x1b[5GRight\x1b[11G/6\x1b[0m");
    printf("\x1b[1E");
    printf("\x1b[32m\x1b[0m");
    printf("\x1b[90m\x1b[5GDown\x1b[11G/2\x1b[0m");
    printf("\x1b[1E");
    currentHeaderLine += 5;
}

void printSucc(char str[256])
{
    printf("\x1b[32msucces: \x1b[0m");
    printf(" %s", str);
    printf("\x1b[1E");
    currentHeaderLine++;
}
void printError(int str)
{
    printf("\x1b[31merror: \x1b[0m");
    printf(" %d", str);
    printf("\x1b[1E");
    currentHeaderLine++;
}
void printErrorStr(char str[256])
{
    printf("\x1b[31merror: \x1b[0m");
    printf(" %s", str);
    printf("\x1b[1E");
    currentHeaderLine++;
}
void printInfo(char str[256])
{
    printf("\x1b[33minfo: \x1b[0m");
    printf(" %s", str);
    printf("\x1b[1E");
    currentHeaderLine++;
}
void printDivider()
{
    printf("\x1b[%dC\x1b[32m-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\x1b[2E\x1b[0m", colsInConsole / 2 - 31);
    currentHeaderLine += 2;
}
void printRec(char str[256])
{
    printf("\x1b[1S\x1b[%d;2H", rowsInConsole);
    printf("%s", str);
}
void printDBG(int str)
{
    printf("\x1b[1S\x1b[%d;2H", rowsInConsole);
    printf("%d", str);
}
void printHotkey(BOOL state, int id)
{
    printf("\x1b[%d;0H\x1b[%dm%s\x1b[0m", hotkeyHeaderLine + id, state ? 32 : 31, state ? "ON " : "OFF");
}

// turn on off with other hotkey

void turnOn()
{
    if (RegisterHotKey(
            NULL,
            1,
            0,
            VK_INSERT))
    {
        printHotkey(TRUE, 1);
    }
    else
    {
        printHotkey(FALSE, 1);
    }

    if (RegisterHotKey(
            NULL,
            2,
            0,
            VK_LEFT))
    {
        printHotkey(TRUE, 2);
    }
    else
    {
        printHotkey(FALSE, 2);
    }

    if (RegisterHotKey(
            NULL,
            3,
            0,
            VK_UP))
    {
        printHotkey(TRUE, 3);
    }
    else
    {
        printHotkey(FALSE, 3);
    }

    if (RegisterHotKey(
            NULL,
            4,
            0,
            VK_RIGHT))
    {
        printHotkey(TRUE, 4);
    }
    else
    {
        printHotkey(FALSE, 4);
    }

    if (RegisterHotKey(
            NULL,
            5,
            0,
            VK_DOWN))
    {
        printHotkey(TRUE, 5);
    }
    else
    {
        printHotkey(FALSE, 5);
    }
}

void turnOff()
{
    if (UnregisterHotKey(
            NULL,
            1))
    {
        printHotkey(FALSE, 1);
    }
    else
    {
        printHotkey(FALSE, 1);
    }

    if (UnregisterHotKey(
            NULL,
            2))
    {
        printHotkey(FALSE, 2);
    }
    else
    {
        printHotkey(FALSE, 2);
    }

    if (UnregisterHotKey(
            NULL,
            3))
    {
        printHotkey(FALSE, 3);
    }
    else
    {
        printHotkey(FALSE, 3);
    }

    if (UnregisterHotKey(
            NULL,
            4))
    {
        printHotkey(FALSE, 4);
    }
    else
    {
        printHotkey(FALSE, 4);
    }

    if (UnregisterHotKey(
            NULL,
            5))
    {
        printHotkey(FALSE, 5);
    }
    else
    {
        printHotkey(FALSE, 5);
    }
}