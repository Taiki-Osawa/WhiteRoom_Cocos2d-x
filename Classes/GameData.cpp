#include "GameData.h"

bool GameData::isEqualPositionIndex(PositionIndex positionIndexA, PositionIndex positionIndexB)
{
  return positionIndexA.x == positionIndexB.x && positionIndexA.y == positionIndexB.y;
}
