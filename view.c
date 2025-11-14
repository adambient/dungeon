#include "globals.h"
#include "map.h"
#include "gfx/gfx.h"
#include "grid.h"
#include "screen.h"
#include "enemy.h"

#define BLOCK_BYTE 0b0001000 // set this bit (OR byte) to mark a tile as blocking

// the 3*4 grid in front of the player
//3xxx
//2xxx
//1xxx
//0xxx
//012
static unsigned char view[3][4];

unsigned char view_tile_get(unsigned char x, unsigned char y)
{
    if (x < MAP_SIZE && y < MAP_SIZE)
    {  
        grid.x = x;
        grid.y = y;
        grid_get();

        if (enemy_is_located(x, y))
        {
            return ENEMY | BLOCK_BYTE;
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
            return (YELLOW | (screen.colour & 0b00000001)) | BLOCK_BYTE;
        case CRATE:
        case WALL:
            return grid.tile | BLOCK_BYTE;
        default:
            return grid.tile;
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
        view[0][0] = view_tile_get(globals.player_x, globals.player_y-1);
        view[1][0] = view_tile_get(globals.player_x, globals.player_y);
        view[2][0] = view_tile_get(globals.player_x, globals.player_y+1);
        view[0][1] = view_tile_get(globals.player_x-1, globals.player_y-1);
        view[1][1] = view_tile_get(globals.player_x-1, globals.player_y);
        view[2][1] = view_tile_get(globals.player_x-1, globals.player_y+1);
        view[0][2] = view_tile_get(globals.player_x-2, globals.player_y-1);
        view[1][2] = view_tile_get(globals.player_x-2, globals.player_y);
        view[2][2] = view_tile_get(globals.player_x-2, globals.player_y+1);
        view[0][3] = view_tile_get(globals.player_x-3, globals.player_y-1);
        view[1][3] = view_tile_get(globals.player_x-3, globals.player_y);
        view[2][3] = view_tile_get(globals.player_x-3, globals.player_y+1);
        break;
    case DIR_DOWN:
        view[0][0] = view_tile_get(globals.player_x, globals.player_y+1);
        view[1][0] = view_tile_get(globals.player_x, globals.player_y);
        view[2][0] = view_tile_get(globals.player_x, globals.player_y-1);
        view[0][1] = view_tile_get(globals.player_x+1, globals.player_y+1);
        view[1][1] = view_tile_get(globals.player_x+1, globals.player_y);
        view[2][1] = view_tile_get(globals.player_x+1, globals.player_y-1);
        view[0][2] = view_tile_get(globals.player_x+2, globals.player_y+1);
        view[1][2] = view_tile_get(globals.player_x+2, globals.player_y);
        view[2][2] = view_tile_get(globals.player_x+2, globals.player_y-1);
        view[0][3] = view_tile_get(globals.player_x+3, globals.player_y+1);
        view[1][3] = view_tile_get(globals.player_x+3, globals.player_y);
        view[2][3] = view_tile_get(globals.player_x+3, globals.player_y-1);
        break;
    case DIR_RIGHT:
        view[0][0] = view_tile_get(globals.player_x-1, globals.player_y);
        view[1][0] = view_tile_get(globals.player_x, globals.player_y);
        view[2][0] = view_tile_get(globals.player_x+1, globals.player_y);
        view[0][1] = view_tile_get(globals.player_x-1, globals.player_y+1);
        view[1][1] = view_tile_get(globals.player_x, globals.player_y+1);
        view[2][1] = view_tile_get(globals.player_x+1, globals.player_y+1);
        view[0][2] = view_tile_get(globals.player_x-1, globals.player_y+2);
        view[1][2] = view_tile_get(globals.player_x, globals.player_y+2);
        view[2][2] = view_tile_get(globals.player_x+1, globals.player_y+2);
        view[0][3] = view_tile_get(globals.player_x-1, globals.player_y+3);
        view[1][3] = view_tile_get(globals.player_x, globals.player_y+3);
        view[2][3] = view_tile_get(globals.player_x+1, globals.player_y+3);
        break;
    case DIR_LEFT:
        view[0][0] = view_tile_get(globals.player_x+1, globals.player_y);
        view[1][0] = view_tile_get(globals.player_x, globals.player_y);
        view[2][0] = view_tile_get(globals.player_x-1, globals.player_y);
        view[0][1] = view_tile_get(globals.player_x+1, globals.player_y-1);
        view[1][1] = view_tile_get(globals.player_x, globals.player_y-1);
        view[2][1] = view_tile_get(globals.player_x-1, globals.player_y-1);
        view[0][2] = view_tile_get(globals.player_x+1, globals.player_y-2);
        view[1][2] = view_tile_get(globals.player_x, globals.player_y-2);
        view[2][2] = view_tile_get(globals.player_x-1, globals.player_y-2);
        view[0][3] = view_tile_get(globals.player_x+1, globals.player_y-3);
        view[1][3] = view_tile_get(globals.player_x, globals.player_y-3);
        view[2][3] = view_tile_get(globals.player_x-1, globals.player_y-3);
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
    gfx.ink = DARKNESS;
    gfx.paper = DARKNESS;
    gfx_attr();       

    // 0,3 (left)
    gfx.width = 6; // lhs
    gfx.y = 19;
    gfx.paper = view[0][3] & BG_BYTES;
    if (view[0][3] & BLOCK_BYTE)
    {
        gfx.x = 5;
        gfx.height = 2;
        gfx.ink = view[0][3] & BG_BYTES;
    }
    else
    {
        gfx.x = 6;
        gfx.height = 1;
        gfx.ink = DARKNESS;
    }
    gfx_attr();    

    // 2,3 (right)
    gfx.width = 6; // rhs
    gfx.y = 26;
    gfx.paper = view[2][3] & BG_BYTES;
    if (view[2][3] & BLOCK_BYTE)
    {
        gfx.x = 5;    
        gfx.height = 2;    
        gfx.ink = view[2][3] & BG_BYTES;
    }   
    else
    {
        gfx.x = 6;
        gfx.height = 1;
        gfx.ink = DARKNESS;
    }     
    gfx_attr();

    // 1,3 (middle)
    if (view[1][3] & BLOCK_BYTE)
    {
        gfx.x = 5;
        gfx.y = 24;
        gfx.height = 2;
        gfx.width = 3;
        gfx.ink = view[1][3] & BG_BYTES; // tile block
        gfx.paper = view[1][3] & BG_BYTES; // tile block
        gfx_attr();
    }
    else
    {
        // top
        gfx.x = 5;
        gfx.y = 25;
        gfx.height = 1;
        gfx.width = 1;
        gfx.ink = DARKNESS;
        gfx.paper = DARKNESS;
        gfx_attr();

        // bottom
        gfx.x = 6;
        gfx.y = 25;
        gfx.height = 1;
        gfx.width = 1; // full width    
        gfx.ink = DARKNESS;
        gfx.paper = view[1][3] & BG_BYTES; // tile floor                                    
        gfx_attr();
    }   

    // 0,2 (left)
    gfx.width = 5;
    gfx.y = 19;
    gfx.paper = view[0][2] & BG_BYTES;
    gfx.ink = view[0][2] & BG_BYTES;
    if (view[0][2] & BLOCK_BYTE)
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
    gfx.paper = view[2][2] & BG_BYTES;
    gfx.ink = view[2][2] & BG_BYTES;
    if (view[2][2] & BLOCK_BYTE)
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
    if (view[1][2] & BLOCK_BYTE)
    {
        gfx.x = 3;
        gfx.y = 22;
        gfx.height = 5;
        gfx.width = 7;
        gfx.ink = view[1][2] & BG_BYTES; // tile wall
        gfx.paper = view[1][2] & BG_BYTES; // tile wall
        gfx_attr();
    }
    else
    {
        // top
        if (view[0][2] & BLOCK_BYTE)
        {
            gfx.x = 4;
            gfx.y = 23;
            gfx.height = 1;
            gfx.width = 1;
            gfx.ink = DARKNESS;
            gfx.paper = view[0][2] & BG_BYTES; // lhs tile wall
            gfx_attr();
        }
    
        gfx.x = 4;
        gfx.y = 24;
        gfx.height = 1;
        gfx.width = 3;
        gfx.ink = DARKNESS;
        gfx.paper = DARKNESS;
        gfx_attr();

        if (view[2][2] & BLOCK_BYTE)
        {
            gfx.x = 4;
            gfx.y = 27;
            gfx.height = 1;
            gfx.width = 1;
            gfx.ink = DARKNESS;
            gfx.paper = view[2][2] & BG_BYTES; // rhs tile wall
            gfx_attr();
        }

        // bottom
        gfx.x = 7;
        gfx.y = 23;
        gfx.height = 1;
        gfx.width = 1;
        gfx.ink = view[0][2] & BG_BYTES; // tile wall
        gfx.paper = view[1][2] & BG_BYTES; // tile floor
        gfx_attr();

        gfx.x = 7;
        gfx.y = 24;
        gfx.height = 1;
        gfx.width = 3;
        gfx.ink = view[1][2] & BG_BYTES; // tile floor
        gfx.paper = view[1][2] & BG_BYTES; // tile floor
        gfx_attr();

        gfx.x = 7;
        gfx.y = 27;
        gfx.height = 1;
        gfx.width = 1;
        gfx.ink = view[2][2] & BG_BYTES; // tile wall
        gfx.paper = view[1][2] & BG_BYTES; // tile floor
        gfx_attr();
    }

    // 0,1 (left)
    gfx.width = 3;
    gfx.y = 19;
    gfx.paper = view[0][1] & BG_BYTES;
    gfx.ink = view[0][1] & BG_BYTES;
    if (view[0][1] & BLOCK_BYTE)
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
    gfx.ink = view[2][1] & BG_BYTES;
    gfx.paper = view[2][1] & BG_BYTES;
    if (view[2][1] & BLOCK_BYTE)
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
    if (view[1][1] & BLOCK_BYTE)
    {
        gfx.x = 2;
        gfx.y = 20;
        gfx.height = 7;
        gfx.width = 11;
        gfx.ink = view[1][1] & BG_BYTES; // tile wall
        gfx.paper = view[1][1] & BG_BYTES; // tile wall
        gfx_attr();
    }
    else
    {
        // top
        if (view[0][1] & BLOCK_BYTE)
        {
            gfx.x = 3;
            gfx.y = 21;
            gfx.height = 1;
            gfx.width = 1;
            gfx.ink = DARKNESS;
            gfx.paper = view[0][1] & BG_BYTES; // lhs tile wall
            gfx_attr();
        }

        gfx.x = 3;
        gfx.y = 22;
        gfx.height = 1;
        gfx.width = 7;
        gfx.ink = DARKNESS;
        gfx.paper = DARKNESS;
        gfx_attr();

        if (view[2][1] & BLOCK_BYTE)
        {
            gfx.x = 3;
            gfx.y = 29;
            gfx.height = 1;
            gfx.width = 1;
            gfx.ink = DARKNESS;
            gfx.paper = view[2][1] & BG_BYTES; // rhs tile wall
            gfx_attr();
        }

        // bottom
        gfx.x = 8;
        gfx.y = 21;
        gfx.height = 1;
        gfx.width = 1;
        gfx.ink = view[0][1] & BG_BYTES; // tile wall
        gfx.paper = view[1][1] & BG_BYTES; // tile floor    
        gfx_attr();

        gfx.x = 8;
        gfx.y = 22;
        gfx.height = 1;
        gfx.width = 7;
        gfx.ink = view[1][1] & BG_BYTES; // tile floor
        gfx.paper = view[1][1] & BG_BYTES; // tile floor
        gfx_attr();

        gfx.x = 8;
        gfx.y = 29;
        gfx.height = 1;
        gfx.width = 1;
        gfx.ink = view[2][1] & BG_BYTES; // tile wall
        gfx.paper = view[1][1] & BG_BYTES; // tile floor    
        gfx_attr();
    }    

    // 0,0 (left)
    gfx.width = 1;
    gfx.y = 19;
    gfx.ink = view[0][0] & BG_BYTES;
    gfx.paper = view[0][0] & BG_BYTES;
    if (view[0][0] & BLOCK_BYTE)
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
    gfx.ink = view[2][0] & BG_BYTES;
    gfx.paper = view[2][0] & BG_BYTES;
    if (view[2][0] & BLOCK_BYTE)
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
    if (view[0][0] & BLOCK_BYTE)
    {
        gfx.x = 2;
        gfx.y = 19;
        gfx.height = 1;
        gfx.width = 1;
        gfx.ink = DARKNESS;
        gfx.paper = view[0][0] & BG_BYTES; // tile wall
        gfx_attr();
    }

    gfx.x = 2;
    gfx.y = 20;
    gfx.height = 1;
    gfx.width = 11;
    gfx.ink = DARKNESS;
    gfx.paper = DARKNESS;
    gfx_attr();

    if (view[2][0] & BLOCK_BYTE)
    {
        gfx.x = 2;
        gfx.y = 31;
        gfx.height = 1;
        gfx.width = 1;
        gfx.ink = DARKNESS;
        gfx.paper = view[2][0] & BG_BYTES; // tile wall
        gfx_attr();
    }

    // 1,0 (middle) - bottom
    gfx.x = 9;
    gfx.y = 19;
    gfx.height = 1;
    gfx.width = 1;
    if (view[0][0] & BLOCK_BYTE) // is there a block to our immediate left
    {
        gfx.ink = view[0][0] & BG_BYTES; // lhs tile wall
        gfx.paper = view[1][0] & BG_BYTES; // middle floor
    }
    else
    {
        gfx.ink = view[0][1] & BG_BYTES; // next lhs tile wall
        gfx.paper = view[0][0] & BG_BYTES; // lhs tile floor
    }    
    gfx_attr();

    gfx.x = 9;
    gfx.y = 20;
    gfx.height = 1;
    gfx.width = 11;
    gfx.ink = view[1][1] & BG_BYTES; // tile floor (next tile floor looks better)
    gfx.paper = view[1][0] & BG_BYTES; // tile floor
    gfx_attr();

    gfx.x = 9;
    gfx.y = 31;
    gfx.height = 1;
    gfx.width = 1;
    if (view[2][0] & BLOCK_BYTE)
    {
        gfx.ink = view[2][0] & BG_BYTES; // rhs tile wall
        gfx.paper = view[1][0] & BG_BYTES; // middle floor
    }
    else
    {
        gfx.ink = view[2][1] & BG_BYTES; // next rhs tile wall
        gfx.paper = view[2][0] & BG_BYTES; // rhs tile floor
    }    
    gfx_attr();    
}