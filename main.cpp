#include <iostream>
#include "view2d.h"

using namespace std;

View2D * teszt = nullptr;

struct Position 
{
    const char * crew;
    const int col;
    const int row;
} positions[] =
{
    { "hunter"      , 1, 0},
    { "hunter"      , 2, 0},
    { "hunter"      , 3, 0},
    { "hunter"      , 3, 1},
    { "trooper"     , 4, 0},
    { "assassin"    , 4, 1},
    { "transporter" , 6, 1},
    { "auradin"     , 5, 0},
    { "auradin"     , 6, 0},
    { "auradin"     , 7, 0},
    { "auradin"     , 7, 1},    
    { "teleporter"  , 5, 2},
    { "teleporter"  , 6, 2},
    { "teleporter"  , 7, 2},
    { "teleporter"  , 7, 3},
    { "commander"   , 5, 1},
} ;

void exit()
{
    delete teszt;
    cout << "Finishing SDL..." << endl;
    IMG_Quit();
    SDL_Quit();
}

void loop(View2D * T)
{
    while(true)
    {
        T->getTiles();
        T->moveSelection();
        T->selected.clear();
        continue;
        int col=0, row=0;
        int pick_handle = T->getHandle(col, row, true);
        SDL_Delay(120);
        int put_handle=T->getHandle(col,row);
        if(put_handle == 0)
        {
            T->moveCharacterTo(pick_handle,col,row);
        }
    }
}

int main(int argc, char *argv[])
{
    atexit(exit); 
    teszt = new View2D("First board", 1920, 1080);
    for(Position t : positions)
    {
        teszt->createPieces(FOE, t.col, t.row, t.crew);
        teszt->createPieces(OWN,8 + 2 * t.row - t.col, 7 - t.row, t.crew);
    }
    loop(teszt);
    return EXIT_SUCCESS;
}
