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

#ifndef VIEW2D_H
#define VIEW2D_H

#include "../BoardView/boardview.h"
#include "players.h"
#include <vector>

class View2D: public BoardView
{
    static bool sdl_initilized;
    static void init();
    
    int SCR_WIDTH;
    int SCR_HEIGHT;
    const char * win_name;
    SDL_Window * win = nullptr;
    SDL_Renderer * render = nullptr;
    SDL_Texture * board_texture = nullptr;
    SDL_Texture * board_background;
    SDL_Rect board_position;
  
    std::vector<CursorTile*> tile_pieces;
    CursorTile * cursor;

    void createWindow();
    
    virtual void showTileCursor() override;
   // virtual void selectGroup() override;
   // virtual void selectJump() override;
    virtual void showSelected() override;
    virtual void redrawBoard() override;
    virtual void moveTileCursorTo(const int& row, const int& col)  override;
    virtual bool convertToTilePosition(int& x, int& y) const  override
    {
        return CursorTile::convertToTilePosition(x,y);
    }
    virtual void show()  override;
    
    void loadTextureImage();
public:
    View2D(const char* name,const int w, const int h);
    ~View2D(); 
    virtual int createPieces(const Ally & A, const int& col, const int& row, const char*) override;
    virtual void moveCharacterTo(const int& handle, const int& col, const int& row) override;
    virtual int moveSelection(const int& revive = 0) override;
    //void /*std::vector<int> &&*/ select() override;
};

#endif // VIEW2D_H
