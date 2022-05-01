#include <iostream>
#include <cmath>

#include <stdlib.h>
#include <time.h>

using namespace std;

const int exponent = 6;
const int screenW = pow(2, exponent);
const int screenH = screenW / 2;

class MapClass
{
    public:
        void initMap();
        void drawScreen();


        void initMap(char screen[screenW][screenH])
        {
            cout << "InitMap\n";
            for(int i = 0; i < screenH; i++)
            {
                for(int j = 0; j < screenW; j++)
                {
                    if(i == 0 || i == screenH - 1 || j == 0 || j == screenW - 1)
                        screen[j][i] = '#';
                    else
                        screen[j][i] = ' ';
                }
            }
        }


        void drawScreen(char screen[screenW][screenH])
        {
            for(int i = 0; i < screenH; i++)
            {
                for(int j = 0; j < screenW; j++)
                {
                    cout << screen[j][i];
                }
                cout << endl;
            }
        }
};


class PlayerClass
{
    public:
        void spawnPlayer();
        void movePlayer();
        
        void spawnPlayer(char screen[screenW][screenH])
        {
            int x = (rand() % screenW - 2) + 1;
            int y = (rand() % screenH - 2) + 1;
        
            screen[x][y] = '-';
        }


        void movePlayer(char screen[screenW][screenH])
        {
            //Create player class and put all funcs in there, then define playerPos and make functions to read and write
            cout << "moving player\n";
        }
};

int main()
{
    srand(time(NULL));

    cout << "Starting program\n";
    char screen[screenW][screenH];
    bool playing = true;
    MapClass mapClass;
    PlayerClass playerClass;

    mapClass.initMap(screen);
    playerClass.spawnPlayer(screen);
    while(playing)
    {
        playerClass.movePlayer(screen);
        mapClass.drawScreen(screen);
        sleep(250);
    }
    cout << "Finished tasks\n";
    return 0;
}
