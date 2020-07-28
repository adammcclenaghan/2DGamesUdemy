#include <fstream>
#include "./Game.h"
#include "./Map.h"
#include "./EntityManager.h"
#include "./Components/TileComponent.h"

//we know manager comes from game
extern EntityManager manager;
Map::Map(std::string textureId, int scale, int tileSize) {
  this->textureId = textureId;
  this->scale = scale;
  this->tileSize = tileSize;
}

/*
 Read from a file like:
    12, 21, 35
This is translated:
12 = column 1 row 2 
We multiply the column and row values by the tileSize since that will give the 
actual x and y position of the tile in the texture image that we want to add

The first row/column index is assumed to be 0 by this loader
 */
void Map::LoadMap(std::string filePath, int mapSizeX, int mapSizeY) {
  //TODO: read the map tile definitions from the .map file
  std::fstream mapFile;
  mapFile.open(filePath);

  // Loop over character by character in our file to get the tile numbers
  // this only supports tilemaps with 10 rows and columns
  // Because we make 2 character reads, if you had row 11 column 11, that would be
  // 1111 in the map file and wouldn't be supported here ...
  for (int y = 0; y < mapSizeY; y++) {
    for (int x = 0; x < mapSizeX; x++) {
      char ch;
      mapFile.get(ch);
      // This is telling us where to move to in our source image to get the right tile
      int sourceRectY = atoi(&ch) * tileSize;
      mapFile.get(ch);
      int sourceRectX = atoi(&ch) * tileSize;
      AddTile(sourceRectX, sourceRectY, x * (scale * tileSize), y * (scale * tileSize));
      // Ignore the comma
      mapFile.ignore();
    }
  }
  mapFile.close();
}

void Map::AddTile(int sourceRectX, int sourceRectY, int x, int y) {
  // Calling it Tile on the AddEntity call sort of breaks some uniqueness
  // in the names of the entities added to the manager. Should be ok in this instance
  Entity& newTile(manager.AddEntity("Tile"));
  newTile.AddComponent<TileComponent>(sourceRectX, sourceRectY, x, y, tileSize, scale, textureId);
  
  
}
