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

#ifndef TILE_PIECES__H
#define TILE_PIECES__H

#include"cursor.h"

class PlayerTile: public CursorTile
{
   
    SDL_Texture * phase_mark[6];
    
    void alignToPosition(const int& col, const int& row) override;
public:
    SDL_Texture * mark;
    PlayerTile(const Ally & A, const std::string& tile_name, const int& col, const int& row, SDL_Renderer* render);
    virtual ~PlayerTile();
};

#endif // TILE_PIECES__H
