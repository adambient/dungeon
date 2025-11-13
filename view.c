#include "globals.h"
#include "map.h"
#include "gfx/gfx.h"
#include "grid.h"
#include "screen.h"
#include "enemy.h"

// the 3*4 grid in front of the player
//3xxx
//2xxx
//1xxx
//0xxx
//012
static unsigned char view[3][4];

unsigned char tile_get(unsigned char x, unsigned char y)
{
    if (x < MAP_SIZE && y < MAP_SIZE)
    {  
        grid.x = x;
        grid.y = y;
        grid_get();
        if ((grid.tile & SEEN_BYTE) == SEEN_BYTE)
        {
            if (enemy_is_located(x, y))
            {
                return ENEMY;
            }
            if (map_uncovered_holes == 0)
            {
                if ((x == 1 && (y == (MAP_SIZE - 2) || y == 1)) ||
                (x == (MAP_SIZE - 2) && (y == (MAP_SIZE - 2) || y == 1)))
                {
                    // uncovered secret exit, cycle between carpet colours
                    return (CARPET_1 | (screen.colour & 0b00000001));
                }
            }
            grid.tile = grid.tile & BG_BYTES;
            switch (grid.tile)
            {
            case TARGET:
                return screen.colour;
            case PLACED:
                // cycle bettween yellow and white
                return (YELLOW | (screen.colour & 0b00000001));
            default:
                return grid.tile;
            }
        }
    }

    return DARKNESS;
}

void view_update(void)
{
    switch (globals.player_facing)
    {
    default:
    case DIR_UP:
        view[0][0] = tile_get(globals.player_x, globals.player_y-1);
        view[1][0] = tile_get(globals.player_x, globals.player_y);
        view[2][0] = tile_get(globals.player_x, globals.player_y+1);
        view[0][1] = tile_get(globals.player_x-1, globals.player_y-1);
        view[1][1] = tile_get(globals.player_x-1, globals.player_y);
        view[2][1] = tile_get(globals.player_x-1, globals.player_y+1);
        view[0][2] = tile_get(globals.player_x-2, globals.player_y-1);
        view[1][2] = tile_get(globals.player_x-2, globals.player_y);
        view[2][2] = tile_get(globals.player_x-2, globals.player_y+1);
        view[0][3] = tile_get(globals.player_x-3, globals.player_y-1);
        view[1][3] = tile_get(globals.player_x-3, globals.player_y);
        view[2][3] = tile_get(globals.player_x-3, globals.player_y+1);
        break;
    case DIR_DOWN:
        view[0][0] = tile_get(globals.player_x, globals.player_y+1);
        view[1][0] = tile_get(globals.player_x, globals.player_y);
        view[2][0] = tile_get(globals.player_x, globals.player_y-1);
        view[0][1] = tile_get(globals.player_x+1, globals.player_y+1);
        view[1][1] = tile_get(globals.player_x+1, globals.player_y);
        view[2][1] = tile_get(globals.player_x+1, globals.player_y-1);
        view[0][2] = tile_get(globals.player_x+2, globals.player_y+1);
        view[1][2] = tile_get(globals.player_x+2, globals.player_y);
        view[2][2] = tile_get(globals.player_x+2, globals.player_y-1);
        view[0][3] = tile_get(globals.player_x+3, globals.player_y+1);
        view[1][3] = tile_get(globals.player_x+3, globals.player_y);
        view[2][3] = tile_get(globals.player_x+3, globals.player_y-1);
        break;
    case DIR_RIGHT:
        view[0][0] = tile_get(globals.player_x-1, globals.player_y);
        view[1][0] = tile_get(globals.player_x, globals.player_y);
        view[2][0] = tile_get(globals.player_x+1, globals.player_y);
        view[0][1] = tile_get(globals.player_x-1, globals.player_y+1);
        view[1][1] = tile_get(globals.player_x, globals.player_y+1);
        view[2][1] = tile_get(globals.player_x+1, globals.player_y+1);
        view[0][2] = tile_get(globals.player_x-1, globals.player_y+2);
        view[1][2] = tile_get(globals.player_x, globals.player_y+2);
        view[2][2] = tile_get(globals.player_x+1, globals.player_y+2);
        view[0][3] = tile_get(globals.player_x-1, globals.player_y+3);
        view[1][3] = tile_get(globals.player_x, globals.player_y+3);
        view[2][3] = tile_get(globals.player_x+1, globals.player_y+3);
        break;
    case DIR_LEFT:
        view[0][0] = tile_get(globals.player_x+1, globals.player_y);
        view[1][0] = tile_get(globals.player_x, globals.player_y);
        view[2][0] = tile_get(globals.player_x-1, globals.player_y);
        view[0][1] = tile_get(globals.player_x+1, globals.player_y-1);
        view[1][1] = tile_get(globals.player_x, globals.player_y-1);
        view[2][1] = tile_get(globals.player_x-1, globals.player_y-1);
        view[0][2] = tile_get(globals.player_x+1, globals.player_y-2);
        view[1][2] = tile_get(globals.player_x, globals.player_y-2);
        view[2][2] = tile_get(globals.player_x-1, globals.player_y-2);
        view[0][3] = tile_get(globals.player_x+1, globals.player_y-3);
        view[1][3] = tile_get(globals.player_x, globals.player_y-3);
        view[2][3] = tile_get(globals.player_x-1, globals.player_y-3);
        break;
    }
}

void view_draw(void)
{
    // clear screen
    gfx.x = 2;
    gfx.y = 19;
    gfx.height = 8;
    gfx.width = 13;
    gfx.ink = BLACK;
    gfx.paper = BLACK;
    gfx_attr();

    // 1,3 (middle)
    if (view[1][3] == WALL)
    {
        gfx.x = 5;
        gfx.y = 25;
        gfx.height = 2;
        gfx.width = 1;
        gfx.ink = view[1][3]; // tile block
        gfx.paper = view[1][3]; // tile block
        gfx_attr();
    }
    else
    {
        // top
        gfx.x = 5;
        gfx.y = 25;
        gfx.height = 1;
        gfx.width = 1; // full width
        gfx.ink = BLACK; // black roof
        gfx.paper = BLACK; // black hole
        gfx_attr();

        // bottom
        gfx.x = 6;
        gfx.y = 25;
        gfx.height = 1;
        gfx.width = 1; // full width    
        gfx.ink = BLACK; // black hole
        gfx.paper = view[1][3]; // tile floor                                    
        gfx_attr();
    }            

    // 0,3 (left)
    gfx.width = 6; // lhs
    gfx.y = 19;
    gfx.paper = view[0][3];
    if (view[0][3] == WALL)
    {
        gfx.x = 5;
        gfx.height = 2;
        gfx.ink = view[0][3];
    }
    else
    {
        gfx.x = 6;
        gfx.height = 1;
        gfx.ink = BLACK;
    }
    gfx_attr();    

    // 2,3 (right)
    gfx.width = 6; // rhs
    gfx.y = 26;
    gfx.paper = view[2][3];
    if (view[2][3] == WALL)
    {
        gfx.x = 5;    
        gfx.height = 2;    
        gfx.ink = view[2][3];
    }   
    else
    {
        gfx.x = 6;
        gfx.height = 1;
        gfx.ink = BLACK;
    }     
    gfx_attr();

    // 0,2 (left)
    gfx.width = 5;
    gfx.y = 19;
    gfx.paper = view[0][2];
    gfx.ink = view[0][2];
    if (view[0][2] == WALL)
    {
        gfx.x = 4;    
        gfx.height = 4;            
    }
    else
    {
        gfx.x = 7;    
        gfx.height = 1;
    }        
    gfx_attr();

    // 2,2 (right)
    gfx.width = 5;
    gfx.y = 27;
    gfx.paper = view[2][2];
    gfx.ink = view[2][2];
    if (view[2][2] == WALL)
    {
        gfx.x = 4;      
        gfx.height = 4;
    }
    else
    {
        gfx.x = 7;    
        gfx.height = 1;
    }   
    gfx_attr();

    // 1,2 (middle)
    if (view[1][2] == WALL)
    {
        gfx.x = 3;
        gfx.y = 22;
        gfx.height = 5;
        gfx.width = 7;
        gfx.ink = view[1][2]; // tile wall
        gfx.paper = view[1][2]; // tile wall
        gfx_attr();
    }
    else
    {
        // top
        if (view[0][2] == WALL)
        {
            gfx.x = 4;
            gfx.y = 23;
            gfx.height = 1;
            gfx.width = 1;
            gfx.ink = BLACK; // black roof
            gfx.paper = view[0][2]; // lhs tile wall
            gfx_attr();
        }
    
        gfx.x = 4;
        gfx.y = 24;
        gfx.height = 1;
        gfx.width = 3;
        gfx.ink = BLACK; // black roof
        gfx.paper = BLACK; // black roof
        gfx_attr();

        if (view[2][2] == WALL)
        {
            gfx.x = 4;
            gfx.y = 27;
            gfx.height = 1;
            gfx.width = 1;
            gfx.ink = BLACK; // black roof
            gfx.paper = view[2][2]; // rhs tile wall
            gfx_attr();
        }

        // bottom
        gfx.x = 7;
        gfx.y = 23;
        gfx.height = 1;
        gfx.width = 1;
        gfx.ink = view[0][2]; // tile wall
        gfx.paper = view[1][2]; // tile floor
        gfx_attr();

        gfx.x = 7;
        gfx.y = 24;
        gfx.height = 1;
        gfx.width = 3;
        gfx.ink = view[1][2]; // tile floor
        gfx.paper = view[1][2]; // tile floor
        gfx_attr();

        gfx.x = 7;
        gfx.y = 27;
        gfx.height = 1;
        gfx.width = 1;
        gfx.ink = view[2][2]; // tile wall
        gfx.paper = view[1][2]; // tile floor
        gfx_attr();
    }

    // 0,1 (left)
    gfx.width = 3;
    gfx.y = 19;
    gfx.paper = view[0][1];
    gfx.ink = view[0][1];
    if (view[0][1] == WALL)
    {
        gfx.x = 3;    
        gfx.height = 6;
    }
    else
    {
        gfx.x = 8;    
        gfx.height = 1;
    }             
    gfx_attr();

    // 2,1 (right)
    gfx.width = 3;
    gfx.y = 29;
    gfx.ink = view[2][1];
    gfx.paper = view[2][1];
    if (view[2][1] == WALL)
    {
        gfx.x = 3;    
        gfx.height = 6;
    }
    else
    {
        gfx.x = 8;    
        gfx.height = 1;
    }            
    gfx_attr();

    // 1,1 (middle)
    if (view[1][1] == WALL)
    {
        gfx.x = 2;
        gfx.y = 20;
        gfx.height = 7;
        gfx.width = 11;
        gfx.ink = view[1][1]; // tile wall
        gfx.paper = view[1][1]; // tile wall
        gfx_attr();
    }
    else
    {
        // top
        if (view[0][1] == WALL)
        {
            gfx.x = 3;
            gfx.y = 21;
            gfx.height = 1;
            gfx.width = 1;
            gfx.ink = BLACK; // black roof
            gfx.paper = view[0][1]; // lhs tile wall
            gfx_attr();
        }

        gfx.x = 3;
        gfx.y = 22;
        gfx.height = 1;
        gfx.width = 7;
        gfx.ink = BLACK; // black roof
        gfx.paper = BLACK; // black roof
        gfx_attr();

        if (view[2][1] == WALL)
        {
            gfx.x = 3;
            gfx.y = 29;
            gfx.height = 1;
            gfx.width = 1;
            gfx.ink = BLACK; // black roof
            gfx.paper = view[2][1]; // rhs tile wall
            gfx_attr();
        }

        // bottom
        gfx.x = 8;
        gfx.y = 21;
        gfx.height = 1;
        gfx.width = 1;
        gfx.ink = view[0][1]; // tile wall
        gfx.paper = view[1][1]; // tile floor    
        gfx_attr();

        gfx.x = 8;
        gfx.y = 22;
        gfx.height = 1;
        gfx.width = 7;
        gfx.ink = view[1][1]; // tile floor
        gfx.paper = view[1][1]; // tile floor
        gfx_attr();

        gfx.x = 8;
        gfx.y = 29;
        gfx.height = 1;
        gfx.width = 1;
        gfx.ink = view[2][1]; // tile wall
        gfx.paper = view[1][1]; // tile floor    
        gfx_attr();
    }    

    // 0,0 (left)
    gfx.width = 1;
    gfx.y = 19;
    gfx.ink = view[0][0];
    gfx.paper = view[0][0];
    if (view[0][0] == WALL)
    {
        gfx.x = 2;    
        gfx.height = 8;
    }
    else
    {
        gfx.x = 9;    
        gfx.height = 1;
    }            
    gfx_attr();

    // 2,0 (right)
    gfx.width = 1;
    gfx.y = 31;
    gfx.ink = view[2][0];
    gfx.paper = view[2][0];
    if (view[2][0] == WALL)
    {
        gfx.x = 2;    
        gfx.height = 8;
    }
    else
    {
        gfx.x = 9;    
        gfx.height = 1;
    }            
    gfx_attr();

    // 1,0 (middle) - top
    if (view[0][0] == WALL)
    {
        gfx.x = 2;
        gfx.y = 19;
        gfx.height = 1;
        gfx.width = 1;
        gfx.ink = BLACK; // black roof
        gfx.paper = view[0][0]; // tile wall
        gfx_attr();
    }

    gfx.x = 2;
    gfx.y = 20;
    gfx.height = 1;
    gfx.width = 11;
    gfx.ink = BLACK; // black roof
    gfx.paper = BLACK; // black roof
    gfx_attr();

    if (view[2][0] == WALL)
    {
        gfx.x = 2;
        gfx.y = 31;
        gfx.height = 1;
        gfx.width = 1;
        gfx.ink = BLACK; // black roof
        gfx.paper = view[2][0]; // tile wall
        gfx_attr();
    }

    // 1,0 (middle) - bottom
    gfx.x = 9;
    gfx.y = 19;
    gfx.height = 1;
    gfx.width = 1;
    gfx.ink = view[0][0]; // tile wall
    gfx.paper = view[1][0]; // tile floor
    gfx_attr();

    gfx.x = 9;
    gfx.y = 20;
    gfx.height = 1;
    gfx.width = 11;
    gfx.ink = view[1][1]; // tile floor (next tile floor looks better)
    gfx.paper = view[1][0]; // tile floor
    gfx_attr();

    gfx.x = 9;
    gfx.y = 31;
    gfx.height = 1;
    gfx.width = 1;
    gfx.ink = view[2][0]; // tile wall
    gfx.paper = view[1][0]; // tile floor
    gfx_attr();    
}