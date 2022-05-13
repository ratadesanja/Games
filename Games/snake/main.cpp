#include <iostream>
#include <cmath>

#include <stdlib.h>
#include <time.h>

#include <Windows.h>
#include <thread>
#include <chrono>

using namespace std;

const int screenW = 64;
const int screenH = screenW / 2;

const char playerHeadChar = 'O';
const char playerBodyChar = 'o';

const char wallChar = '#';
const char appleChar = 'x';
const char airChar = ' ';
//const char noneChar = '⠀';

enum Direction {left = 0, right = 1, up = 2, down = 3};


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


class Keyboard
{
    public:
        void getKeys()
        {

            if(GetAsyncKeyState(0x41) & 0x01)
            {
                leftKey = true;
                last_pressed = Direction::left;
            }
            else
                leftKey = false;
            
            if(GetAsyncKeyState(0x44) & 0x01)
            {
                rightKey = true;
                last_pressed = Direction::right;
            }
            else
                rightKey = false;

            if(GetAsyncKeyState(0x57) & 0x01)
            {
                upKey = true;
                last_pressed = Direction::up;
            }
            else
                upKey = false;

            if(GetAsyncKeyState(0x53) & 0x01)
            {
                downKey = true;
                last_pressed = Direction::down;
            }
            else
                downKey = false;
        }

        Direction get_last_pressed()
        {
            return last_pressed;
        }
    
    private:    
        bool leftKey;
        bool rightKey;
        bool upKey;
        bool downKey;

        Direction last_pressed = Direction::right;
};


class MapClass
{
    public:
        char array[screenW][screenH];

        void modifyArray(int x, int y, char c)
        {
            array[x][y] = c;
        }

        char getArray(int x, int y)
        {
            return array[x][y];
        }

        int getColor(char c)
        {
            switch (c)
            { 
                case wallChar:
                //case noneChar:
                case airChar:
                    return 0;
                    break;

                case playerHeadChar:
                    return 1;
                    break;

                case playerBodyChar:
                    return 2;
                    break;

                case appleChar:
                    return 3;
                    break;
            }
            return 5;
        }

        void initMap()
        {
            //cout << "InitMap\n";
            char c;
            for(int y = 0; y < screenH; y++)
            {
                for(int x = 0; x < screenW; x++)
                {
                    if(y == 0 || y == screenH - 1 || x == 0 || x == screenW - 1)
                    {
                        c = wallChar;
                        modifyArray(x, y, c);
                    }
                    else
                    {
                        c = airChar;
                        modifyArray(x, y, c);
                    }
                }
            }
        }

        void drawScreen(Console& console)
        {
            for(int y = 0; y < screenH; y++)
            {
                for(int x = 0; x < screenW; x++)
                {
                    console.setColor(getColor(array[x][y]));
                    cout << array[x][y] << flush;
                    shown_array[x][y] = array[x][y];
                }
                cout << endl;
            }
        }

        void updateScreen(Console& console)
        {             
            for(int y = 0; y < screenH; y++)
            {
                for(int x = 0; x < screenW; x++)
                {
                    if (shown_array[x][y] != array[x][y])
                    {
                        console.setCursorPosition(x, y);                        
                        console.setColor(getColor(array[x][y]));

                        cout << array[x][y] << flush;
                        shown_array[x][y] = array[x][y];
                    }
                }
            }
        }
    
    private:
        char shown_array[screenW][screenH];
    
};


class PlayerClass
{
    public:
        void spawnPlayer();
        void movePlayer();
        
        void spawnPlayer(MapClass& map)
        {
            isAlive = true;
            int tempX, tempY;
            do
            {
                tempX = (rand() % (screenW / 2) - 4) + screenW / 2 + 4;
                tempY = (rand() % (screenH / 2) - 4) + screenH / 2 + 4;
            } while (map.getArray(tempX, tempY) != airChar);

            x = tempX;
            y = tempY;

            map.modifyArray(x, y, playerHeadChar);
        }


        void movePlayer(MapClass& map, Keyboard& keyboard)
        {
            map.modifyArray(x, y, airChar);

            int newX = x;
            int newY = y;

            switch (keyboard.get_last_pressed())
            {
                case Direction::left:
                    newX -= 1;
                    break;

                case Direction::right:
                    newX += 1;
                    break;
                    
                case Direction::up:
                    newY -= 1;
                    break;
                    
                case Direction::down:
                    newY += 1;
                    break;

            }

            char nextChar;
            nextChar = map.getArray(newX, newY);

            switch(nextChar)
            { 
                case airChar:
                    x = newX;
                    y = newY;
                    map.modifyArray(x, y, playerHeadChar);
                    break;

                case wallChar:
                case playerHeadChar:
                case playerBodyChar:
                    isAlive = false;
                    break;
            }

        }

        bool getAlive()
        {
            return isAlive;
        }
    
    private:
        int x, y;
        bool isAlive;
};


void setup(Console& console, MapClass& map, PlayerClass& player)
{
    console.setFullscreen();
    console.ShowConsoleCursor(false);

    map.initMap();
    player.spawnPlayer(map);
    map.drawScreen(console);

}


void keyboardThread(Keyboard& keyboard, Console& console, PlayerClass& player)
{
    //using namespace std::literals::chrono_literals;

    while(player.getAlive())
    {
        keyboard.getKeys();
        
        this_thread::sleep_for(67ms);
    }
}


//TO DO: Center game screen, add text, add apple. Get ideas for new game. FIX COMMENTING ON MY VSCODE
int main()
{
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    srand(time(NULL));

    //cout << "Starting program\n";
    Sleep(1000);

    Console console;
    Keyboard keyboard;
    MapClass map;
    PlayerClass player;


    setup(console, map, player);
    thread keyboardWorker(keyboardThread, ref(keyboard), ref(console), ref(player));

    while(player.getAlive())
    {
        player.movePlayer(map, keyboard);
        map.updateScreen(console);

        console.setColor(0);
        console.setCursorPosition(screenW + 5, 0);
        cout << keyboard.get_last_pressed();
        
        Sleep(1000/10);
    }

    console.setCursorPosition(screenW + 5, screenH / 2);
    cout << "Player is dead" << endl;
    Sleep(5000);

    keyboardWorker.join();

    console.setCursorPosition(0, screenH + 2);
    system("pause");

    return 0;
}