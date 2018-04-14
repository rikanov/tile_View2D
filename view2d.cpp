/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2018  <copyright holder> <email>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "view2d.h"

bool View2D::sdl_initilized = false;

View2D::View2D(const char* name,const int w, const int h)
:win_name(name)
,SCR_WIDTH(w)
,SCR_HEIGHT(h)
{
    if(!sdl_initilized)
    {
        init();
    } 
    createWindow(); 
}

void View2D::init()
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    sdl_initilized = true;
}

void View2D::createWindow()
{
    win = SDL_CreateWindow(win_name,SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_CENTERED, 
                           SCR_WIDTH,SCR_HEIGHT,SDL_WINDOW_SHOWN);
    if(win == nullptr)
    {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() <<std::endl;
        exit(2);
    }
    render = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(render == nullptr)
    {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        exit(3);
    } 
    loadTextureImage();
    cursor = new CursorTile(render); 
}

int View2D::createPieces(const ALLY & A, const int& col, const int& row, const char* png)
{
    tile_pieces.push_back(new PlayerTile(A, png, col, row,render));
    const int handle = tile_pieces.size();
    handles[col][row] = handle;
    return handle;
}

void View2D::loadTextureImage()
{
    int w,h;
    board_background = IMG_LoadTexture(render, "background/background.png");
    board_texture  = IMG_LoadTexture(render, "background/board.png");
    if(board_texture == nullptr)
    {
        std::cerr << "IMG_LoadTexture Error: "<< SDL_GetError() << std::endl;
        exit(5);
    }
    SDL_QueryTexture(board_texture,NULL,NULL, &w,&h);
    
    board_position.x=(SCR_WIDTH-w)/2;
    board_position.y=(SCR_HEIGHT-h)/2;
    board_position.w=w;
    board_position.h=h;
    CursorTile::start_x += board_position.x;
    CursorTile::start_y += board_position.y;
}

void View2D::moveCharacterTo(const int& handle, const int& col, const int& row)
{
    if(handle)
    {
        const int x = tile_pieces[handle-1]->getCol();
        const int y = tile_pieces[handle-1]->getRow();
        if(handles[col][row])
        {
            tile_pieces.at(handles[col][row]-1)->active = false;
        }
        handles[x][y] = 0;
        handles[col][row] = handle;
        tile_pieces[handle-1]->setPos(col,row);
    }
    else
    {
        tile_pieces.at(handles[col][row]-1)->active = false;
        handles[col][row] = 0;
    }
}

void View2D::moveSelection()
{
    for(std::vector<SDL_Point>::reverse_iterator rit = selected.rbegin(); rit+1 != selected.rend(); ++rit)
    {
        const int dest_x = rit->x;
        const int dest_y = rit->y;
        const int handle = handles[(rit+1)->x][(rit+1)->y];
        moveCharacterTo(handle, dest_x, dest_y);
        SDL_Delay(100);
        show();
    }
}

void View2D::redrawBoard()
{
    for(CursorTile * c : tile_pieces)
    {
        if(c->active)
        {
            PlayerTile * t = dynamic_cast<PlayerTile* >(c);
            SDL_RenderCopy(render,t->backgrnd,NULL,&(t->pos));
            SDL_RenderCopy(render,t->mark,NULL,&(t->pos));
        }
        //SDL_RenderCopyEx(render,t->mark,NULL,&(t->pos),t->angle, &(t->middle), t->flip);
    }
}

int View2D::getHandle(int& col, int& row, const bool& not_empty)
{    
    SDL_Event event;
    int mouse_x, mouse_y, result = -1;
    show();   
    while(result < not_empty)
    {  
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT)
        {
            exit(0);
        }
        if(event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
        {
            const bool click = SDL_GetMouseState(&mouse_x,&mouse_y) & SDL_BUTTON(SDL_BUTTON_LEFT);
            if(convertToTilePosition(mouse_x,mouse_y)==false)
            {
                continue;
            }
            if(col != mouse_x || row != mouse_y)
            {               
                col = mouse_x;
                row = mouse_y;
                moveTileCursorTo(col,row);
                show();
            }
            result = click ? handles[col][row] : -1;
        } 
    }
    return result;
}

void View2D::getTiles()
{
    std::vector<int> result;
    SDL_Event event;
    SDL_Point mouse;
    SDL_Point mouse_act;
    show();
    do
    {
        SDL_WaitEvent(&event);
        SDL_GetMouseState(&mouse.x,&mouse.y);
        if(convertToTilePosition(mouse.x,mouse.y) && (mouse.x != mouse_act.x || mouse.y != mouse_act.y))
        {
            mouse_act = mouse;
            moveTileCursorTo(mouse_act.x, mouse_act.y);
            show();
        }
        if(event.type == SDL_QUIT)
        {
            exit(0);
        }
        
    }while(event.type != SDL_MOUSEBUTTONDOWN);
    do
    {
        std::vector<SDL_Point>::reverse_iterator rit = selected.rbegin();
        SDL_PollEvent(&event);
        if((SDL_GetMouseState(&mouse.x, &mouse.y) & SDL_BUTTON(SDL_BUTTON_LEFT)) /*&& event.type == SDL_MOUSEMOTION */&& convertToTilePosition(mouse.x,mouse.y))
        {
            
            if(mouse.x != mouse_act.x || mouse.y != mouse_act.y)
            {
                mouse_act = mouse;
                moveTileCursorTo(mouse_act.x, mouse_act.y);
                show();
            }
           // std::cout<<"exec"<<std::endl;
            switch(selected.size())
            {
                case 0:
                    selected.push_back(mouse);
                    break;
                case 1:
                    if(rit->x != mouse.x || rit->y != mouse.y)
                    {
                        selected.push_back(mouse);
                    }
                    break;
                default:
                    if(rit->x != mouse.x || rit->y != mouse.y)
                    {
                        ++rit;
                        if(rit->x == mouse.x && rit->y == mouse.y)
                        {
                            selected.pop_back();
                        }
                        else
                        {
                            selected.push_back(mouse);
                        }
                    }
            }
            show();
        }
        if(selected.size() > 4)
        {
            selected.clear();
        }
    }while(event.type != SDL_MOUSEBUTTONUP);
   // return std::move(result);
}

void View2D::moveTileCursorTo(const int& row, const int& col)
{
    cursor->setPos(row,col);
}

void View2D::showTileCursor()
{
    SDL_RenderCopy(render,cursor->backgrnd,NULL,&(cursor->pos));
}

void View2D::showSelected()
{
    for(SDL_Point s : selected)
    {
        moveTileCursorTo(s.x,s.y);
        showTileCursor();
    }
}

void View2D::show()
{
    SDL_RenderClear(render);
    SDL_RenderCopy(render, board_background, NULL,NULL);
    SDL_RenderCopy(render,board_texture,NULL,&board_position);
    redrawBoard();
    showTileCursor();
    showSelected();
    SDL_RenderPresent(render);
}

View2D::~View2D()
{
    std::cout << win_name <<" destructor called... tiles: " << tile_pieces.size() << std::endl;
    delete cursor;
    for(CursorTile * t : tile_pieces)
    {
        std::cout<<'.';
        delete t;
    }
    SDL_DestroyTexture(board_background);
    SDL_DestroyTexture(board_texture);
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(win);
    std::cout << std::endl << win_name <<" destructor finished... " << std::endl;
}
