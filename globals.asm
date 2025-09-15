SECTION bss_user
PUBLIC _globals
_globals:
db %00000000 ;globals.player_tile
db %00000000 ;globals.player_x
db %00000000 ;globals.player_y
db %00000000 ;globals.player_dir
db %00000100 ;globals.player_facing = DIR_NONE
db %00000000 ;globals.is_player_pushing
db %00000000 ;globals.is_player_pulling
