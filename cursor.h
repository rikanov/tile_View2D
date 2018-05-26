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

#ifndef CURSOR_H
#define CURSOR_H


#include "../GameEngine/enums.h"
#include "sdl_dep.h"

class CursorTile
{
protected:
    
    static inline int upDown(const int& col, const int& row)
    {
        return (col + (row > 3)) % 2;
    }
    int row, col;
    SDL_Texture * background[2];
    void calcPosition();
    
    virtual void alignToPosition(const int& col, const int& row);
    
public: 
    
    static int start_x, start_y, step_x, step_y;
    
    CursorTile();
    CursorTile(SDL_Renderer* render);
    virtual ~CursorTile();
    
    SDL_Texture * backgrnd;
    SDL_Rect pos;
    bool active;
    void setRow(const int& r)
    {
        row=r;
        calcPosition();
    }
    const int& getRow() const
    {
        return row;
    }
    void setCol(const int& c)
    {
        col=c;
        calcPosition();
    }
    const int& getCol() const
    {
        return col;
    }
    void setPos(const int& c, const int& r)
    {
        row=r;
        col=c;
        calcPosition();
    }
    static bool convertToTilePosition(int& x, int& y);
};


#endif // CURSOR_H
