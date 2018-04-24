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

#include "players.h"
#include "../Engine/enums.h"
#include <string.h>

PlayerTile::PlayerTile(const Ally & A, const std::string& tile_name, const int& c, const int& r, SDL_Renderer * render)
{
    std::string ally;
    if(A == Ally::FOE)
    {
        ally = "foe";
    }
    else if (A == Ally::OWN)
    {
        ally = "own";
    }
    else
    {
        ally ="pointer";
    }
    const std::string bgrnd1 = "tiles/tile_backgrounds/"+ally+"_up.png";
    const std::string bgrnd2 = "tiles/tile_backgrounds/"+ally+"_down.png";
    const std::string ico_base("tiles/marks/"+tile_name+"/"+std::string(A == Ally::OWN ? "own_" : "foe_"));
  
    background[0] = IMG_LoadTexture(render, bgrnd1.c_str());
    background[1] = IMG_LoadTexture(render, bgrnd2.c_str());
    
    if(background[0] == nullptr || background[1] == nullptr)
    {
        std::cerr << "IMG_LoadTexture Error: " << SDL_GetError() << std::endl;
    }
    for(int phase = 0; phase < 6; ++phase)
    {
        std::string ico = ico_base + std::to_string(phase)+".png";
        
        phase_mark[phase] = IMG_LoadTexture(render, ico.c_str());
        if(phase_mark[phase] == nullptr)
        {
            std::cerr << "IMG_LoadTexture Error: " << SDL_GetError() << std::endl;
        }
    }
    pos.w = 150;
    pos.h = 130;
    row = r;
    col = c;
    calcPosition();
}

void PlayerTile::alignToPosition(const int& col, const int& row)
{
    int direction = row < 4 ? row % 3 : (7 - row) % 3;
    direction += 2 - (col % 3);
    direction %= 3;
    // +/- 120 degree, each flip change the rotation!    
    if(row < 4 && (col % 2))
    {
        direction += 3;
    }
    else if(row > 3 && (col % 2 == 0))
    {    
        direction += 3;
    }
    backgrnd = background[ upDown(col,row)];
    mark = phase_mark[direction];
}

PlayerTile::~PlayerTile()
{
    SDL_DestroyTexture(mark);
    for(int i = 0; i < 6 ; ++i)
    {
        SDL_DestroyTexture(phase_mark[i]);
    }
    for(int i = 0; i < 4 ; ++i)
    {
        SDL_DestroyTexture(background[i]);
    }
}
