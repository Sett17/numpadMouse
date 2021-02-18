# Numpad Mouse
Program for you use your keyboard to move your mouse cursor and click mouse buttons.

## How to use

Use a console windows that is at least 35 characters wide and 28 characters tall *(defaults for most are probably around 100x30)*.

**!Numlock has to be active!**

Keys:
|          Key          | Funtion                                   |
| :-------------------: | ----------------------------------------- |
|       Capslock        | Toggles whole functionality               |
|         Num4          | Moves mouse to left                       |
|         Num8          | Moves mouse up                            |
|         Num6          | Moves mouse right                         |
|         Num5          | Moves mouse down                          |
|         Num1          | Hold left mouse button down until keyup   |
|         Num2          | Hold middle mouse button down until keyup |
|         Num3          | Hold right mouse button down until keyup  |
|         Ctrl          | Makes mouse move faster                   |
| *Ctrl-Alt-Shift-Num5* | *Exits Program (to be implemented)*       |

## Remarks
- You cant use the aforementioned numpad keys while the program is active, aka. when capslock is on the keys are captured.
  - to write numbers with the numpad again just turn off capslock or terminate the program
- until Ctrl-Alt-Shift-Num5 is implemented use Ctrl-C to close program
- this project is going to stay windows only
- if the visuals lok completely broken, check if your using a terminal with support for virtual terminal sequences
  - If you are using Windows, I recommend [Windows Terminal](https://github.com/microsoft/terminal)