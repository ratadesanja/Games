#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>

#include <Windows.h>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ifstream;

class Console
{
    public:
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD size;


        void ShowConsoleCursor(bool showFlag)
        {
            CONSOLE_CURSOR_INFO     cursorInfo;

            GetConsoleCursorInfo(handle, &cursorInfo);
            cursorInfo.bVisible = showFlag; // set the cursor visibility
            SetConsoleCursorInfo(handle, &cursorInfo);
        }


        void SetCursorPosition(int x, int y)
        {
            COORD Position;


            Position.X = x;
            Position.Y = y;
            SetConsoleCursorPosition(handle, Position);
        }


        COORD GetCursorPosition()
        {
            CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo; 
            GetConsoleScreenBufferInfo(handle, &screenBufferInfo);

            COORD Position = screenBufferInfo.dwCursorPosition;
            return Position;

        }


        void GetConsoleSize()
        {
            CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo; 
            GetConsoleScreenBufferInfo(handle, &screenBufferInfo);

            COORD Size = screenBufferInfo.dwMaximumWindowSize;
            size = Size;
        }


        void SetColor(int color)
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


        void SetFullscreen()
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


        Console()
        {
        }
};

class Hangman
{
    private:
        int fileLength;
        
        void GetFileLength()
        {
            ifstream file("word_list.txt");

            string line;
            int count = 0;
            while(getline(file, line))
            {
                count++;
            }
            fileLength = count;
        }

    public:
        string chosenWord;
        string guessWord;

        void PickWordFromList()
        {
            ifstream file("word_list.txt");
            int lineIndex = rand() % (fileLength + 1);
            
            string line;
            int count = 0;
            while(getline(file, line))
            {
                if(count == lineIndex)
                    break;
        
                count++;
            }
            line[0] = std::tolower(line[0]);

            chosenWord = line;

            guessWord = "";
            for(int i = 0; i < chosenWord.length(); i++)
            { 
                guessWord.append(" ");
            }

        }        

        void IsInWord(char input)
        {
            for(int i = 0; i < chosenWord.length(); i++)
            {
                if(chosenWord[i] == input)
                {
                    guessWord[i] = input;
                }
            }
        }

        void DrawHUD(Console& console)
        {
            int x = console.size.X;
            int y = console.size.Y;

            int minX = console.size.X / 4;
            int minY = console.size.Y / 8;

            int maxX = minX * 2;
            int maxY = minY * 7;

            // console.SetCursorPosition(0, 0);
            // cout << console.size.X << " " << console.size.Y << endl << minX << " " << minX << endl;

            //Vertical lines
            for(int i = 1; i < maxY - minY; i++)
            { 
                console.SetCursorPosition(minX, minY + i);
                cout << "|";
            }
            
            for(int i = 1; i < maxY - minY; i++)
            { 
                console.SetCursorPosition(maxX + minX, minY + i);
                cout << "|";
            }

            //Horizontal lines
            console.SetCursorPosition(minX, minY);
            for(int i = 0; i <= maxX; i++)
            {
                cout << "-";
            }
            
            console.SetCursorPosition(minX, maxY);
            for(int i = 0; i <= maxX; i++)
            {
                cout << "-";
            }


            //ASCII            
            ifstream file("ascii.txt");

            vector<string> title;
            vector<string> gallow;
            vector<string> man;

            string line;
            string reading = "none";
            while(getline(file, line))
            {
                if(line == "title")
                {
                    reading = "title";
                    continue;
                }

                else if(line == "gallow")
                {
                    reading = "gallow";
                    continue;
                }

                else if(line == "man")
                {
                    reading = "man";
                    continue;
                }

                else if(line == "stop")
                {
                    reading = "stop";
                    break;
                }



                else if(reading == "title")
                    title.push_back(line);

                else if(reading == "gallow")
                    gallow.push_back(line);

                else if(reading == "man")
                    man.push_back(line);

            }
            file.close();

            // Draw Title
            DrawAscii(title, console, x, minY, 2, 3);
            
            Sleep(2000);
            // Draw Gallow
            DrawAscii(gallow, console, x, minY, 1.25, 10);

            Sleep(1000);
            // Draw Man
            DrawAscii(man, console, x, minY, 1.25, 10);
        }

        void DrawAscii(vector<string>& list, Console& console, int x, int minY, float marginX , int marginY)
        {
            for(int i = 0; i < list.size(); i++)
            {
                console.SetCursorPosition(x / 2 - (list[i].length() / marginX), minY + marginY + i);
                COORD cursorPos = console.GetCursorPosition();

                string str = list[i];
                for(int j = 0; j < str.length(); j++)
                {
                    if(str[j] == ' ')
                        continue;
                    else
                    {
                        console.SetCursorPosition(cursorPos.X + j, cursorPos.Y);                             
                        cout << str[j];
                    }
                }
            }
        }

        void Draw(Console& console)
        {
            DrawHUD(console);
        }

        Hangman()
        {
            GetFileLength();
        }
    

};

void Setup(Console& console)
{
    console.SetFullscreen();
    console.ShowConsoleCursor(true);    
    console.GetConsoleSize();

}

int main()
{
    srand(time(NULL));

    Console console;
    Hangman hangman;

    Setup(console);

    while(true)
    { 
        hangman.PickWordFromList();
        hangman.DrawHUD(console);
        while(hangman.guessWord != hangman.chosenWord)
        {
            Sleep(200);
            break;
        }
        break;
    }

    console.SetCursorPosition(0, console.size.Y - 5);
    system("pause");
    return 0;
}