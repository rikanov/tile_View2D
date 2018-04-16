#include "view2d.h"
#include <algorithm>

static bool operator == (const SDL_Point& a, const SDL_Point& b)
{
    return a.x == b.x && a.y == b.y;
}
static bool operator != (const SDL_Point& a, const SDL_Point& b)
{
    return a.x != b.x || a.y != b.y;
}
void View2D::select()
{
    show();
    do
    {
        SDL_WaitEvent(&event);
        if(event.type == SDL_QUIT)
        {
            exit(0);
        }
        SDL_GetMouseState(&mouse.x,&mouse.y);
        if(convertToTilePosition(mouse.x,mouse.y) == false)
        {
            continue;
        }
        if(mouse.x != mouse_act.x || mouse.y != mouse_act.y)
        {
            mouse_act = mouse;
            moveTileCursorTo(mouse_act.x, mouse_act.y);
            show();
        }
    }while(event.type != SDL_MOUSEBUTTONDOWN);
    
    if(SDL_GetMouseState(&mouse.x,&mouse.y) & SDL_BUTTON(SDL_BUTTON_RIGHT))
    {
        selectJump();
    }
    else
    {
        selectGroup();
    }
}

void View2D::selectGroup()
{
    do
    {
        bool solid_line = true;
        if(selected.size() > 4)
        {
            selected.clear();
            show();
        }
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT)
        {
            exit(0);
        }
        if(SDL_GetMouseState(&mouse.x, &mouse.y) & SDL_BUTTON(SDL_BUTTON_LEFT))
        {
            if(convertToTilePosition(mouse.x,mouse.y) == false)
            {
                selected.clear();
                continue;
            }
            if(mouse != mouse_act)
            {
                mouse_act = mouse;
                moveTileCursorTo(mouse_act.x, mouse_act.y);
            }
            if(std::find(selected.begin(),selected.end(),mouse_act) != selected.end())
            {
                continue;
            }
            if(handles[mouse_act.x][mouse_act.y] == 0)
            {
                if(solid_line && selected.size() > 0)
                {
                    solid_line = false;
                    selected.push_back(mouse_act);
                    show();
                }
                continue;
            } 
            selected.push_back(mouse_act);
            show();
        }
    }while(event.type != SDL_MOUSEBUTTONUP);    
}

void View2D::selectJump()
{
    if(convertToTilePosition(mouse.x,mouse.y))
    {
        selected.push_back(mouse);
        show();
        SDL_Delay(120);
        while(true)
        {
            SDL_PollEvent(&event);
            if(event.type == SDL_QUIT)
            {
                exit(0);
            }
            const unsigned int mstate = SDL_GetMouseState(&mouse.x, &mouse.y);
            if(mstate)
            {
                if(convertToTilePosition(mouse.x,mouse.y) == false)
                {
                    selected.clear();
                    break;
                }
                if(mouse != mouse_act)
                {
                    mouse_act = mouse;
                    moveTileCursorTo(mouse_act.x, mouse_act.y);
                }
                if(mstate & (SDL_BUTTON(SDL_BUTTON_LEFT) | SDL_BUTTON(SDL_BUTTON_RIGHT)))
                {
                    if(selected.back() == mouse_act)
                    {
                        selected.clear();
                        break;
                    }
                    if(mstate & SDL_BUTTON(SDL_BUTTON_LEFT))
                    {
                        selected.push_back(mouse_act);
                        break;
                    }
                    else
                    {
                        selected[0] = mouse_act;
                        show();
                        SDL_Delay(120);
                        continue;
                    }
                }
            }
        }; // end while
    }
}
