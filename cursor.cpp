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

#include "cursor.h"

int CursorTile::start_x = 16;
int CursorTile::start_y = 14;
int CursorTile::step_x = 77;
int CursorTile::step_y = 131;

CursorTile::CursorTile()
 :col(0)
 ,row(0)
 ,active(true)
{
}

CursorTile::CursorTile(SDL_Renderer* render): 
CursorTile()
{
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x,&mouse.y);
    if(convertToTilePosition(mouse.x,mouse.y))
    {
        col = mouse.x;
        row = mouse.y;
    }
    background[0] = IMG_LoadTexture(render, "tiles/tile_backgrounds/pointer_up.png");
    background[1] = IMG_LoadTexture(render, "tiles/tile_backgrounds/pointer_down.png");
    background[2] = IMG_LoadTexture(render, "tiles/tile_backgrounds/pointer_up.png");
    background[3] = IMG_LoadTexture(render, "tiles/tile_backgrounds/pointer_down.png");
    
    if( background[0] == nullptr || background[1] == nullptr)
    {
        std::cerr << "IMG_LoadTexture Error: " << SDL_GetError() << std::endl;
    } //std::cout<<"teszt"<<std::endl;
    pos.w = 150;
    pos.h = 130;
    calcPosition();
}

void CursorTile::calcPosition()
{
    const int offset_x = row < 4 ? 3-row : row-4;
    pos.x = start_x + (offset_x + col) * step_x;
    pos.y = start_y + row * step_y;
    alignToPosition(col,row);
}

void CursorTile::alignToPosition(const int& col, const int& row)
{
    backgrnd = background[upDown(col,row)];
}

bool CursorTile::convertToTilePosition(int& x, int& y)
{
    bool success = false;
    const int real_x = x - start_x;
    const int real_y = y - start_y;
    y = real_y / step_y;
    if(0<=y && y<8)
    {
        x = real_x / step_x;
        const int mod_x = real_x % step_x;
        const int mod_y = real_y % step_y;
        const int offset_x = y < 4 ? 3-y : y-4;
        x -= offset_x;
        if(upDown(x,y))
        {
            x -= (mod_y >  mod_x * step_y / step_x);
        }
        else
        {
            x -= (mod_y < step_y - mod_x * step_y / step_x);
        }
        
        success = (y < 4) ? (2 * y + 9 > x) : (15 - 2 * (y-4) > x) ; 
        success &= (x >= 0);
        success &= (y >= 0);
        success &= (y < 8);        
    }
    return success;
}

CursorTile::~CursorTile()
{
    SDL_DestroyTexture(backgrnd);
    SDL_DestroyTexture(background[0]);
    SDL_DestroyTexture(background[1]);
}
