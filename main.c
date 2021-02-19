#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include <winuser.h>

#pragma comment(lib, "User32.lib")

#define TOPBORDER                                                              \
  "==========================================================================" \
  "==========================================================================" \
  "================="
#define BUFFERSIZE 64

#define theme "Standard"  // will later be command line option

typedef enum {
  PART_BODY,
  PART_LEFT,
  PART_RIGHT,
  PART_MIDDLE,
} t_mousePart;

typedef enum {
  ARROW_LEFT = 'l',
  ARROW_RIGHT = 'r',
  ARROW_UP = 'u',
  ARROW_DOWN = 'd',
  ARROW_SPEED = 's',
} t_mouseArrow;

void moveMouse(int, int);
void printMousePart(t_mousePart, BOOL);
int printMouseArrow(t_mouseArrow, BOOL);

void sendMouseKey(int);
void manageMouseKey(t_mousePart, BOOL *);
void manageMouseMove(t_mouseArrow, BOOL *);
void stringToSpaces(char[]);
void turnOn();
void turnOff();

int rowsInConsole;
int colsInConsole;
BOOL WINAPI consoleHandler(DWORD signal) {
  if (signal == CTRL_C_EVENT || signal == CTRL_CLOSE_EVENT) {
    if (GetKeyState(VK_CAPITAL) & 1) {
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
    printf(
        "\x1b[?25h \x1b[?12h \x1b[2S\x1b[%d;5H\x1b[31mB\x1b[35my\x1b[34me "
        "\x1b[36mB\x1b[32my\x1b[33me\x1b[0m\x1b[1S\x1b[r\x1b[%d;0H",
        rowsInConsole, rowsInConsole);  // set SGR to default, show cursor,
                                        // enable cursor blinking
  }
  ExitProcess(1);
  return TRUE;
}

char fstr[BUFFERSIZE];

BOOL active, numlockActive, leftActive, upActive, rightActive, downActive;
int mouseTop, mouseLeft;
int mul = 1;

int main(int argc, char const *argv[]) {
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  rowsInConsole = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
  colsInConsole = csbi.srWindow.Right - csbi.srWindow.Left + 1;

  mouseTop = rowsInConsole / 2 - 11;
  mouseLeft = colsInConsole / 2 - 16;

  // Set output mode to handle virtual terminal sequences
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD dwMode = 0;
  GetConsoleMode(hOut, &dwMode);
  // dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  dwMode |= 0x0004;
  SetConsoleMode(hOut, dwMode);

  printf("\x1b[2J\x1b[0;0H\x1b[?25l");  // clear disply \ cursor to 00 \
                                            // hide cursor
  printf("\x1b[90m\x1b[0;0H%.*s\x1b[%d;0H%.*s", colsInConsole, TOPBORDER,
         rowsInConsole, colsInConsole, TOPBORDER);
  for (int i = 0; i < rowsInConsole; i++) {
    printf("\x1b[%dd\x1b[90m\x1b[0G|\x1b[%dG|", i, colsInConsole);
  }
  printf(
      "\x1b[1;0H    .--\x1b[2;0H  .'\x1b[3;1H '\
            \x1b[1;%dH--.    \x1b[2;%dH'.  \x1b[3;%dH' ",
      colsInConsole - 6, colsInConsole - 3, colsInConsole - 1);

  printf(
      "\x1b[1;%dH-=-=-=-=-=-=-=-=- \x1b[4mNumpad Mouse\x1b[24m "
      "-=-=-=-=-=-=-=-=-\x1b[0m",
      colsInConsole / 2 - 24);

  if (!SetConsoleCtrlHandler(consoleHandler, TRUE)) {
    return 1;
  }

  printMousePart(PART_BODY, FALSE);
  printMousePart(PART_LEFT, FALSE);
  printMousePart(PART_RIGHT, FALSE);
  printMousePart(PART_MIDDLE, FALSE);

  // for (;;) {
  // }

  active = GetKeyState(VK_CAPITAL) & 1;
  BOOL mouseLeftPressed = FALSE;
  BOOL mouseRightPressed = FALSE;
  BOOL mouseMiddlePressed = FALSE;

  BOOL mouseMoveLeftActive = FALSE;
  BOOL mouseMoveUpActive = FALSE;
  BOOL mouseMoveRightActive = FALSE;
  BOOL mouseMoveDownActive = FALSE;
  BOOL mouseMoveSpeedActive = FALSE;
  for (;;) {
    if (GetKeyState(VK_CAPITAL) & 1) {
      if (!active) {
        turnOn();
        printMousePart(PART_BODY, TRUE);
        printMousePart(PART_LEFT, FALSE);
        printMousePart(PART_RIGHT, FALSE);
        printMousePart(PART_MIDDLE, FALSE);
        printMouseArrow(ARROW_LEFT, FALSE);
        printMouseArrow(ARROW_UP, FALSE);
        printMouseArrow(ARROW_RIGHT, FALSE);
        printMouseArrow(ARROW_DOWN, FALSE);
        printMouseArrow(ARROW_SPEED, FALSE);
        active = TRUE;
      }

      mul = GetKeyState(VK_CONTROL) & 0x8000 ? 12 : 4;

      manageMouseKey(PART_LEFT, &mouseLeftPressed);
      manageMouseKey(PART_RIGHT, &mouseRightPressed);
      manageMouseKey(PART_MIDDLE, &mouseMiddlePressed);

      manageMouseMove(ARROW_LEFT, &mouseMoveLeftActive);
      manageMouseMove(ARROW_UP, &mouseMoveUpActive);
      manageMouseMove(ARROW_RIGHT, &mouseMoveRightActive);
      manageMouseMove(ARROW_DOWN, &mouseMoveDownActive);
      manageMouseMove(ARROW_SPEED, &mouseMoveSpeedActive);
    } else if (active) {
      turnOff();
      printMousePart(PART_BODY, FALSE);
      printMousePart(PART_LEFT, FALSE);
      printMousePart(PART_RIGHT, FALSE);
      printMousePart(PART_MIDDLE, FALSE);
      active = FALSE;
    }
    Sleep(1000 / 125);
  }
  printf("\x1b[0m\x1b[?25h\x1b[?12h\x1b[r\x1b[%d;0H", rowsInConsole - 1);
  return 0;
}

void moveMouse(int x, int y) {
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

void printMousePart(t_mousePart part, BOOL state) {
  FILE *art;
  switch (part) {
    case PART_BODY:
      sprintf(fstr, "graphics/body%s", theme);
      art = fopen(fstr, "r");
      break;
    case PART_LEFT:
      sprintf(fstr, "graphics/left%s", theme);
      art = fopen(fstr, "r");
      break;
    case PART_RIGHT:
      sprintf(fstr, "graphics/right%s", theme);
      art = fopen(fstr, "r");
      break;
    case PART_MIDDLE:
      sprintf(fstr, "graphics/middle%s", theme);
      art = fopen(fstr, "r");
      break;
    default:
      break;
  }
  char buffer[BUFFERSIZE];

  printf("\x1b[%dm", state ? 32 : 90);

  int rowOffset, colOffset;
  fgets(buffer, BUFFERSIZE, art);
  sscanf(buffer, "%d,%d", &rowOffset, &colOffset);

  int i = 0;
  while (fgets(buffer, BUFFERSIZE, art)) {
    printf("\x1b[%d;%dH%s", mouseTop + rowOffset + i++, mouseLeft + colOffset,
           buffer);
  }

  printf("\x1b[0m");
  fclose(art);
}

int printMouseArrow(t_mouseArrow arrow, BOOL state) {
  char buffer[BUFFERSIZE];
  sprintf(fstr, "graphics/arrows%s", theme);
  FILE *art = fopen(fstr, "r");

  if (arrow != ARROW_SPEED) {
    printf("\x1b[%dm", state ? 0 : 90);
  } else {
    printf("\x1b[%dm", state ? 31 : 90);
  }

  while (fgets(buffer, BUFFERSIZE, art)) {
    if (buffer[0] == arrow) {
      int size = 0, rowOffset = 0, colOffset = 0;
      char zzz;
      sscanf(buffer, "%c,%d,%d,%d", &zzz, &size, &rowOffset, &colOffset);
      for (int i = 0; i < size; i++) {
        fgets(buffer, BUFFERSIZE, art);
        printf("\x1b[%d;%dH%s", mouseTop + 17 + rowOffset + i,
               mouseLeft + 16 + colOffset, buffer);
      }
      printf("\x1b[0m");
      return 0;
    }
  }
}

void manageMouseKey(t_mousePart part, BOOL *storage) {
  int key, actionUp, actionDown;
  switch (part) {
    case PART_LEFT:
      key = VK_NUMPAD1;
      actionDown = MOUSEEVENTF_LEFTDOWN;
      actionUp = MOUSEEVENTF_LEFTUP;
      break;
    case PART_RIGHT:
      key = VK_NUMPAD3;
      actionDown = MOUSEEVENTF_RIGHTDOWN;
      actionUp = MOUSEEVENTF_RIGHTUP;
      break;
    case PART_MIDDLE:
      key = VK_NUMPAD2;
      actionDown = MOUSEEVENTF_MIDDLEDOWN;
      actionUp = MOUSEEVENTF_MIDDLEUP;
      break;
    default:
      break;
  }

  int keyAction = ((GetKeyState(key) & 0x8000 && !*storage) << 1) |
                  (!(GetKeyState(key) & 0x8000) && *storage);
  if (keyAction == 2) {
    sendMouseKey(actionDown);
    printMousePart(part, TRUE);
    *storage = TRUE;
  } else if (keyAction == 1) {
    sendMouseKey(actionUp);
    printMousePart(part, FALSE);
    *storage = FALSE;
  }
}

void manageMouseMove(t_mouseArrow direction, BOOL *storage) {
  int key, actionUp, actionDown, moveX, moveY;
  switch (direction) {
    case ARROW_LEFT:
      key = VK_NUMPAD4;
      moveX = -1 * mul;
      moveY = 0;
      break;
    case ARROW_UP:
      key = VK_NUMPAD8;
      moveX = 0;
      moveY = -1 * mul;
      break;
    case ARROW_RIGHT:
      key = VK_NUMPAD6;
      moveX = 1 * mul;
      moveY = 0;
      break;
    case ARROW_DOWN:
      key = VK_NUMPAD5;
      moveX = 0;
      moveY = 1 * mul;
      break;
    case ARROW_SPEED:
      key = VK_CONTROL;
      moveX = 0;
      moveY = 0;
    default:
      break;
  }
  int keyAction = ((GetKeyState(key) & 0x8000 && !*storage) << 1) |
                  (!(GetKeyState(key) & 0x8000) && *storage);
  if (keyAction == 2) {
    printMouseArrow(direction, TRUE);
    *storage = TRUE;
  } else if (keyAction == 1) {
    printMouseArrow(direction, FALSE);
    *storage = FALSE;
  }

  if (*storage) {
    moveMouse(moveX, moveY);
  }
}

void sendMouseKey(int flags) {
  INPUT inp;
  inp.type = INPUT_MOUSE;
  inp.mi.mouseData = 0;
  inp.mi.time = 0;
  inp.mi.dwFlags = flags;

  SendInput(1, &inp, sizeof(inp));
}

void turnOn() {
  RegisterHotKey(NULL, 1, 0, VK_NUMPAD4);
  RegisterHotKey(NULL, 2, 0, VK_NUMPAD8);
  RegisterHotKey(NULL, 3, 0, VK_NUMPAD6);
  RegisterHotKey(NULL, 4, 0, VK_NUMPAD2);
  RegisterHotKey(NULL, 5, 0, VK_NUMPAD5);
  RegisterHotKey(NULL, 6, 0, VK_NUMPAD3);
  RegisterHotKey(NULL, 7, 0, VK_NUMPAD1);
}

void turnOff() {
  UnregisterHotKey(NULL, 1);
  UnregisterHotKey(NULL, 2);
  UnregisterHotKey(NULL, 3);
  UnregisterHotKey(NULL, 4);
  UnregisterHotKey(NULL, 5);
  UnregisterHotKey(NULL, 6);
  UnregisterHotKey(NULL, 7);
}
