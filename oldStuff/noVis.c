#include <stdio.h>
#include <windows.h>
#include <winuser.h>
#include <conio.h>

#pragma comment(lib, "User32.lib")

void moveMouse(int, int);
void printSucc(char[256]);
void printError(int);
void printErrorStr(char[256]);
void printInfo(char[256]);
void printDivider(void);
void printRec(char[256]);
void printDBG(int);

void turnOn();

int rowsInConsole;
int colsInConsole;
BOOL WINAPI consoleHandler(DWORD signal)
{
    if (signal == CTRL_C_EVENT || signal == CTRL_CLOSE_EVENT)
    {
        if (GetKeyState(VK_CAPITAL) & 1)
        {
            INPUT ip;
            ip.type = INPUT_KEYBOARD;
            ip.ki.wScan = 0;
            ip.ki.time = 0;
            ip.ki.dwExtraInfo = 0;
            ip.ki.wVk = VK_CAPITAL;
            ip.ki.dwFlags = 0;
            SendInput(1, &ip, sizeof(INPUT));
            ip.ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(1, &ip, sizeof(INPUT));
        }
        printf("\x1b[?25h \x1b[?12h \x1b[2S\x1b[%d;5H\x1b[31mB\x1b[35my\x1b[34me \x1b[36mB\x1b[32my\x1b[33me\x1b[0m\x1b[1S\x1b[r\x1b[%d;0H", rowsInConsole, rowsInConsole); // set SGR to default, show cursor, enable cursor blinking
    }
    ExitProcess(1);
    return TRUE;
}

int currentHeaderLine = 0;
int hotkeyHeaderLine = 0;
char fstr[256];

BOOL capslockActive;
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

    printf("\x1b[2J\x1b[0;0H\x1b[?25l");                                                                                                  // clear disply \ cursor to 00 \ hide cursor
    printf("\x1b[32m");                                                                                                                   // set foreground color to green
    printf("\x1b[%dC-=-=-=-=-=-=-=-=- \x1b[4mDefinitive Virtual Stylus\x1b[24m -=-=-=-=-=-=-=-=-\x1b[0m\x1b[2E", colsInConsole / 2 - 31); // \x1b[4m so set underlined
    currentHeaderLine += 2;

    if (!SetConsoleCtrlHandler(consoleHandler, TRUE))
    {
        printErrorStr("Could not set control handler");
        return 1;
    }

    hotkeyHeaderLine = currentHeaderLine;

    printDivider();
    printf("\x1b[%d;%dr", currentHeaderLine, rowsInConsole);
    printf("\x1b[%d;0H", rowsInConsole);
    turnOn();

    int mul = 1;
    BOOL active = GetKeyState(VK_CAPITAL) & 1;
    BOOL mouseLeftPressed = FALSE;
    BOOL mouseRightPressed = FALSE;
    BOOL mouseMiddlePressed = FALSE;
    for (;;)
    {
        if (GetKeyState(VK_CAPITAL) & 1)
        {
            if (!capslockActive)
            {
                turnOn();
            }

            mul = GetKeyState(VK_CONTROL) & 0x8000 ? 8 : 4;
            GetKeyState(VK_NUMPAD4) & 0x8000 ? moveMouse(-1 * mul, 0) : 0;
            GetKeyState(VK_NUMPAD6) & 0x8000 ? moveMouse(1 * mul, 0) : 0;
            GetKeyState(VK_NUMPAD8) & 0x8000 ? moveMouse(0, -1 * mul) : 0;
            GetKeyState(VK_NUMPAD5) & 0x8000 ? moveMouse(0, 1 * mul) : 0;
            if (GetKeyState(VK_NUMPAD1) & 0x8000 && !mouseLeftPressed)
            {
                INPUT inp;
                inp.type = INPUT_MOUSE;
                inp.mi.mouseData = 0;
                inp.mi.time = 0;
                inp.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

                SendInput(1, &inp, sizeof(inp));
                mouseLeftPressed = TRUE;
            }
            else if (!(GetKeyState(VK_NUMPAD1) & 0x8000) && mouseLeftPressed)
            {
                INPUT inp;
                inp.type = INPUT_MOUSE;
                inp.mi.mouseData = 0;
                inp.mi.time = 0;
                inp.mi.dwFlags = MOUSEEVENTF_LEFTUP;

                SendInput(1, &inp, sizeof(inp));
                mouseLeftPressed = FALSE;
            }
            if (GetKeyState(VK_NUMPAD3) & 0x8000 && !mouseRightPressed)
            {
                INPUT inp;
                inp.type = INPUT_MOUSE;
                inp.mi.mouseData = 0;
                inp.mi.time = 0;
                inp.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;

                SendInput(1, &inp, sizeof(inp));
                mouseRightPressed = TRUE;
            }
            else if (!(GetKeyState(VK_NUMPAD3) & 0x8000) && mouseRightPressed)
            {
                INPUT inp;
                inp.type = INPUT_MOUSE;
                inp.mi.mouseData = 0;
                inp.mi.time = 0;
                inp.mi.dwFlags = MOUSEEVENTF_RIGHTUP;

                SendInput(1, &inp, sizeof(inp));
                mouseRightPressed = FALSE;
            }
            if (GetKeyState(VK_NUMPAD2) & 0x8000 && !mouseMiddlePressed)
            {
                INPUT inp;
                inp.type = INPUT_MOUSE;
                inp.mi.mouseData = 0;
                inp.mi.time = 0;
                inp.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;

                SendInput(1, &inp, sizeof(inp));
                mouseMiddlePressed = TRUE;
            }
            else if (!(GetKeyState(VK_NUMPAD2) & 0x8000) && mouseMiddlePressed)
            {
                INPUT inp;
                inp.type = INPUT_MOUSE;
                inp.mi.mouseData = 0;
                inp.mi.time = 0;
                inp.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;

                SendInput(1, &inp, sizeof(inp));
                mouseMiddlePressed = FALSE;
            }
        }
        else if (capslockActive)
        {
            turnOff();
        }
        Sleep(1000 / 125);
    }

    printf("\x1b[0m\x1b[?25h\x1b[?12h\x1b[r\x1b[%d;0H", rowsInConsole - 1);
    return 0;
}

void moveMouse(int x, int y)
{
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

void turnOn()
{
    capslockActive = TRUE;
    RegisterHotKey(NULL, 1, 0, VK_NUMPAD4);
    RegisterHotKey(NULL, 2, 0, VK_NUMPAD8);
    RegisterHotKey(NULL, 3, 0, VK_NUMPAD6);
    RegisterHotKey(NULL, 4, 0, VK_NUMPAD2);
    RegisterHotKey(NULL, 5, 0, VK_NUMPAD5);
    RegisterHotKey(NULL, 6, 0, VK_NUMPAD3);
    RegisterHotKey(NULL, 7, 0, VK_NUMPAD1);
}

void turnOff()
{
    capslockActive = FALSE;
    UnregisterHotKey(NULL, 1);
    UnregisterHotKey(NULL, 2);
    UnregisterHotKey(NULL, 3);
    UnregisterHotKey(NULL, 4);
    UnregisterHotKey(NULL, 5);
    UnregisterHotKey(NULL, 6);
    UnregisterHotKey(NULL, 7);
}