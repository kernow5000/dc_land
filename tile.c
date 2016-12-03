#include <kos.h>
#include <dc_land/dc_land.h>

// BROKEN , needs v_clr and pad, hmm possibly fixed actually now
//
// kernow 02/11/02


//draw a tilegrid_t and its tiles..

void draw_tilegrid (struct tilegrid_t *tileGrid) {
  struct tile_t *currentTile = tileGrid -> tiles;

  //clear tileGrid area..  clear screen for now..

  v_clr (tileGrid -> xPos, tileGrid -> yPos,
	       tileGrid -> xPos + (tileGrid -> gridWidth * tileGrid -> tileWidth),
	       tileGrid -> yPos + (tileGrid -> gridHeight * tileGrid -> tileHeight),
	       0, 0, 0);


  //draw a box for the boundaries of the tileGrid
  v_box (tileGrid -> xPos -1, tileGrid -> yPos -1,
       (tileGrid -> gridWidth * tileGrid -> tileWidth) +2,
       (tileGrid -> gridHeight * tileGrid -> tileHeight) +2,
       255, 255, 255);   //colour

  
  if (currentTile == NULL)     //no tiles to draw
    return;

  do {
    draw_tile (tileGrid, currentTile -> id);
    currentTile = currentTile -> next;
  } while (currentTile != NULL);
    
  return;
}


//find a tile by its id and return a pointer to it returns NULL if id doesnt exist

struct tile_t *get_tile (struct tilegrid_t *tileGrid, TILE_ID id) {
  struct tile_t *tile = tileGrid -> tiles;

  if (tile != NULL)
    while ( (tile -> id != id) && (tile != NULL) )
      tile = tile -> next;
  
  return tile;
}


//draw an individual tile
//returns NO_SUCH_TILE if tile doesnt exist..

int draw_tile (struct tilegrid_t *tileGrid, TILE_ID id) {
  struct tile_t *tile;

  if ( (tile = get_tile (tileGrid, id) ) == NULL )
    return NO_SUCH_TILE;
  
  //draw tile

  v_box ( (tileGrid -> xPos + (tileGrid -> tileWidth * tile -> x)) + 1,
       (tileGrid -> yPos + (tileGrid -> tileHeight * tile -> y)) + 1,
       tileGrid -> tileWidth - 1, tileGrid -> tileHeight - 1,
       255, 255, 255);    //colour
  
  return 0;    //ok
}


//clear a given tile

int clear_tile (struct tilegrid_t *tileGrid, TILE_ID id) {
  struct tile_t *tile;
  
  if ( (tile = get_tile (tileGrid, id) ) == NULL )
    return NO_SUCH_TILE;
  
  //clear tile

  v_box ( (tileGrid -> xPos + (tileGrid -> tileWidth * tile -> x)) + 1,
       (tileGrid -> yPos + (tileGrid -> tileHeight * tile -> y)) + 1,
       tileGrid -> tileWidth - 1, tileGrid -> tileHeight - 1,
       0, 0, 0);   //colour

  return 0;    //ok
}



//return 0 if posn is empty, or -1 if out of limits of grid
//if there is a tile in pos x, y
//  return the TILE_ID of the tile

TILE_ID check_tilegrid (struct tilegrid_t *tileGrid, int x, int y) {
  struct tile_t *currentTile = tileGrid -> tiles;
  int id = 0;
  
  if ( (x > tileGrid -> gridWidth) || (y > tileGrid -> gridHeight) )
    return INVALID_TILE_POS;

  if (currentTile != NULL) {

    do {
      if ( (x == currentTile -> x) && (y == currentTile -> y) )
	return id;
      
      currentTile = currentTile -> next;
      id++;
    } while (currentTile != NULL);
    
  }

  return TILE_SLOT_FREE;
} 


//create a new tileGrid *

struct tilegrid_t *create_tilegrid (int xPos, int yPos, int tileWidth, int tileHeight, int gridWidth, int gridHeight/*, Tcolour col*/) {
  struct tilegrid_t *tileGrid = (struct tilegrid_t *) malloc (sizeof (struct tilegrid_t));

  tileGrid -> xPos = xPos;
  tileGrid -> yPos = yPos;

  tileGrid -> tileWidth = tileWidth;
  tileGrid -> tileHeight = tileHeight;

  tileGrid -> gridWidth = gridWidth;
  tileGrid -> gridHeight = gridHeight;

  //tileGrid -> col = col;

  tileGrid -> tiles = NULL;

  return tileGrid;
}


//add a tile to a tileGrid_t *

TILE_ID addTile (struct tilegrid_t *tileGrid, int xPos, int yPos) {
  struct tile_t *newTile = (struct tile_t *) malloc (sizeof (struct tile_t));
  struct tile_t *lastTile;
  TILE_ID newTile_id = 0;

  if (check_tilegrid (tileGrid, xPos, yPos) != TILE_SLOT_FREE)
    return TILE_SLOT_TAKEN;     //theres a tile there already..


  newTile -> x = xPos;
  newTile -> y = yPos;
  //newTile -> col = col;
  newTile -> next = NULL;


  //put the new tile at the end of tileGrid -> tiles, and return the tile_id   =>

  lastTile = tileGrid -> tiles;

  if (lastTile != NULL) {
    while ( lastTile -> next != NULL ) {
      newTile_id ++;
      lastTile = lastTile -> next;
    }

    newTile -> id = newTile_id + 1;
    lastTile -> next = newTile;      //add new tile to end of tileGrid -> tiles
  }
  else {    //first tile
    newTile -> id = 0;
    tileGrid -> tiles = newTile;
  }
  
  return newTile -> id;
}



//puts an existing tile in a new location..

int putTile (struct tilegrid_t *tileGrid, TILE_ID id, int xPos, int yPos) {
  struct tile_t *tile;

  if (check_tilegrid (tileGrid, xPos, yPos) != TILE_SLOT_FREE)
    return TILE_SLOT_TAKEN;     //theres a tile there already..

  tile = tileGrid -> tiles;

  while (tile -> id != id) {
    if (tile == NULL)
      return NO_SUCH_TILE;      //tile not found

    tile = tile -> next;
  }

  tile -> x = xPos;
  tile -> y = yPos;

  return 0;    //ok
}



//moves an existing tile in a given direction

int moveTile (struct tilegrid_t *tileGrid, TILE_ID id, int direction) {
  struct tile_t *tile;

  tile = tileGrid -> tiles;


  //find the tile

  while (tile -> id != id) {
    if (tile == NULL)
      return NO_SUCH_TILE;        //tile not found..

    tile = tile -> next;
  }

  
  //clear the tiles current location

  clear_tile (tileGrid, tile -> id);


  //move tile in specified direction

  switch (direction) {
  case tileUP: {
    if (tile -> y > 0)
      if (check_tilegrid (tileGrid, tile -> x, tile -> y  -1) == TILE_SLOT_FREE)
	tile -> y --;
    break;
  }
  case tileDOWN: {
    if ( tile -> y < (tileGrid -> gridHeight -1) )
      if (check_tilegrid (tileGrid, tile -> x, tile -> y  +1) == TILE_SLOT_FREE)
	tile -> y ++;
    break;
  }
  case tileLEFT: {
    if (tile -> x > 0)
      if (check_tilegrid (tileGrid, tile -> x -1, tile -> y) == TILE_SLOT_FREE)
	tile -> x --;
    break;
  }
  case tileRIGHT: {
    if ( tile -> x < (tileGrid -> gridWidth -1) )
      if (check_tilegrid (tileGrid, tile -> x +1, tile -> y) == TILE_SLOT_FREE)
	tile -> x ++;
    break;
  }
  }


  //draw tile in its new position

  draw_tile (tileGrid, tile -> id);

  return 0;     //ok
} 
