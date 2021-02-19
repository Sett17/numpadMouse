#include <stdio.h>
#include <windows.h>
#include <winuser.h>
#include <conio.h>

#pragma comment(lib, "User32.lib")

#define TOPBORDER "====================================================================================================================================================================="

void moveMouse(int, int);
void printMouse(BOOL);
void printLeftMouse(BOOL);
void printRightMouse(BOOL);
void printMiddleMouse(BOOL);

void turnOn();
void turnOff();

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

char fstr[256];

BOOL active, numlockActive, leftActive, upActive, rightActive, downActive;
int mouseTop, mouseLeft;

int main(int argc, char const *argv[])
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    rowsInConsole = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    colsInConsole = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    mouseTop = rowsInConsole / 2 - 11;
    mouseLeft = colsInConsole / 2 - 16;

    printf("\x1b[2J\x1b[0;0H\x1b[?25l"); // clear disply \ cursor to 00 \ hide cursor
    printf("\x1b[90m\x1b[0;0H%.*s\x1b[%d;0H%.*s", colsInConsole, TOPBORDER, rowsInConsole, colsInConsole, TOPBORDER);
    for (int i = 0; i < rowsInConsole; i++)
    {
        printf("\x1b[%dd\x1b[90m\x1b[0G|\x1b[%dG|", i, colsInConsole);
    }
    printf("\x1b[1;0H    .--\x1b[2;0H  .'\x1b[3;1H '\
            \x1b[1;%dH--.    \x1b[2;%dH'.  \x1b[3;%dH' ",
           colsInConsole - 6, colsInConsole - 3, colsInConsole - 1);

    printf("\x1b[1;%dH-=-=-=-=-=-=-=-=- \x1b[4mNumpad Mouse\x1b[24m -=-=-=-=-=-=-=-=-\x1b[0m", colsInConsole / 2 - 24);

    printMouse(FALSE);

    if (!SetConsoleCtrlHandler(consoleHandler, TRUE))
    {
        return 1;
    }

    // for (;;)
    // {
    //     //dbg
    // }

    int mul = 1;
    active = GetKeyState(VK_CAPITAL) & 1;
    BOOL mouseLeftPressed = FALSE;
    BOOL mouseRightPressed = FALSE;
    BOOL mouseMiddlePressed = FALSE;
    for (;;)
    {
        if (GetKeyState(VK_CAPITAL) & 1)
        {
            if (!active)
            {
                turnOn();
                printMouse(mouseTop, mouseLeft, TRUE);
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
                printLeftMouse(TRUE);
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
                printLeftMouse(FALSE);
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
                printRightMouse(TRUE);
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
                printRightMouse(FALSE);
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
                printMiddleMouse(TRUE);
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
                printMiddleMouse(FALSE);
            }
        }
        else if (active)
        {
            turnOff();
            printMouse(FALSE);
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

void printMouse(BOOL state)
{
    char buffer[255];
    FILE *art = fopen("mouse.txt", "r");
    printf("\x1b[%dm", state ? 32 : 31);

    int i = 0;
    while (fgets(buffer, 255, art))
    {
        printf("\x1b[%d;%dH%s", mouseTop + i++, mouseLeft, buffer);
    }

    printf("\x1b[0m");
    fclose(art);

    printLeftMouse(FALSE);
    printRightMouse(FALSE);
    printMiddleMouse(FALSE);
}

void printLeftMouse(BOOL state)
{
    printf("\x1b[%dm", state ? 32 : 90);
    printf("\x1b[%d;%dH_.-\"\"\"\"|", mouseTop + 2, mouseLeft + 6);
    printf("\x1b[%d;%dH.'     .'", mouseTop + 3, mouseLeft + 4);
    printf("\x1b[%d;%dH/       |", mouseTop + 4, mouseLeft + 3);
    printf("\x1b[%d;%dH|        |", mouseTop + 5, mouseLeft + 2);
    printf("\x1b[%d;%dH|        |", mouseTop + 6, mouseLeft + 2);
    printf("\x1b[%d;%dH|        |", mouseTop + 7, mouseLeft + 2);
    printf("\x1b[%d;%dH|        |", mouseTop + 8, mouseLeft + 2);
    printf("\x1b[%d;%dH|        '.", mouseTop + 9, mouseLeft + 2);
    printf("\x1b[%d;%dH|          |", mouseTop + 10, mouseLeft + 2);
    printf("\x1b[%d;%dH'._______.'", mouseTop + 11, mouseLeft + 3);
}

void printRightMouse(BOOL state)
{
    printf("\x1b[%dm", state ? 32 : 90);
    printf("\x1b[%d;%dH|\"\"\"\"-._", mouseTop + 2, mouseLeft + 19);
    printf("\x1b[%d;%dH'.     '.", mouseTop + 3, mouseLeft + 20);
    printf("\x1b[%d;%dH|       \\", mouseTop + 4, mouseLeft + 21);
    printf("\x1b[%d;%dH|        |", mouseTop + 5, mouseLeft + 21);
    printf("\x1b[%d;%dH|        |", mouseTop + 6, mouseLeft + 21);
    printf("\x1b[%d;%dH|        |", mouseTop + 7, mouseLeft + 21);
    printf("\x1b[%d;%dH|        |", mouseTop + 8, mouseLeft + 21);
    printf("\x1b[%d;%dH.'        |", mouseTop + 9, mouseLeft + 20);
    printf("\x1b[%d;%dH|         |", mouseTop + 10, mouseLeft + 20);
    printf("\x1b[%d;%dH'.______.'", mouseTop + 11, mouseLeft + 20);
}

void printMiddleMouse(BOOL state)
{
    printf("\x1b[%dm", state ? 32 : 90);
    printf("\x1b[%d;%dH.'''.", mouseTop + 3, mouseLeft + 14);
    printf("\x1b[%d;%dH|     |", mouseTop + 4, mouseLeft + 13);
    printf("\x1b[%d;%dH|     |", mouseTop + 5, mouseLeft + 13);
    printf("\x1b[%d;%dH|     |", mouseTop + 6, mouseLeft + 13);
    printf("\x1b[%d;%dH|     |", mouseTop + 7, mouseLeft + 13);
    printf("\x1b[%d;%dH\\     /", mouseTop + 8, mouseLeft + 13);
    printf("\x1b[%d;%dH'._.'", mouseTop + 9, mouseLeft + 14);
}

void turnOn()
{
    active = TRUE;
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
    active = FALSE;
    UnregisterHotKey(NULL, 1);
    UnregisterHotKey(NULL, 2);
    UnregisterHotKey(NULL, 3);
    UnregisterHotKey(NULL, 4);
    UnregisterHotKey(NULL, 5);
    UnregisterHotKey(NULL, 6);
    UnregisterHotKey(NULL, 7);
}
