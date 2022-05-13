#include <iostream>
#include <string>
#include <vector>

#include <Windows.h>

using std::cout;
using std::endl;
using std::string;
using std::vector;

class Console
{
    public:
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        void ShowConsoleCursor(bool showFlag)
        {

        CONSOLE_CURSOR_INFO     cursorInfo;

        GetConsoleCursorInfo(handle, &cursorInfo);
        cursorInfo.bVisible = showFlag; // set the cursor visibility
        SetConsoleCursorInfo(handle, &cursorInfo);
        }

        void setCursorPosition(int x, int y)
        {
            COORD Position;


            Position.X = x;
            Position.Y = y;
            SetConsoleCursorPosition(handle, Position);
        }

        COORD getCursorPosition()
        {
            CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo; 
            GetConsoleScreenBufferInfo(handle, &screenBufferInfo);

            COORD Position = screenBufferInfo.dwCursorPosition;
            return Position;

        }

        void setColor(int color)
        {
            switch (color)
            {
                case 0: 
                    SetConsoleTextAttribute(handle, 15);
                    break;

                case 1:
                    SetConsoleTextAttribute(handle, 9);
                    break;

                case 2:
                    SetConsoleTextAttribute(handle, 10);
                    break;

                case 3:
                    SetConsoleTextAttribute(handle, 4);
                    break;
            } 
        }

        void setFullscreen()
        {
            HWND hwnd = GetConsoleWindow();
            SetWindowLong(hwnd, GWL_STYLE, WS_POPUP);

            CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo; 
            GetConsoleScreenBufferInfo(handle, &screenBufferInfo);

            COORD new_screen_buffer_size;
            new_screen_buffer_size.X = screenBufferInfo.srWindow.Right - screenBufferInfo.srWindow.Left + 1;
            new_screen_buffer_size.Y = screenBufferInfo.srWindow.Bottom - screenBufferInfo.srWindow.Top + 1;

            SetConsoleScreenBufferSize(handle, new_screen_buffer_size);

            system("mode 650");
            ShowWindow(hwnd, SW_MAXIMIZE);
        }
};

void setup(Console& console)
{
    console.ShowConsoleCursor(false);
}

int main()
{
    Console console;

    return 0;
}