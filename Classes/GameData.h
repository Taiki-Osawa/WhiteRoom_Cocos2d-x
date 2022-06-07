#ifndef _WhiteRoom_GameData_
#define _WhiteRoom_GameData_

#include <algorithm>

class GameData
{
public:
  #define SQUARE_X 30
  #define SQUARE_Y 20
  #define SQUARE_SIZE 200

  struct PositionIndex
  {

    PositionIndex()
    {
      x = 0;
      y = 0;
    }

    PositionIndex(int _x, int _y)
    {
      x = _x;
      y = _y;
    }
    
    #define POSITIONINDEX_CHARACTER PositionIndex(-99, -99)

    PositionIndex inarea()
    {
      auto posX = std::max(std::min(x, SQUARE_X), 1);
      auto posY = std::max(std::min(y, SQUARE_Y), 1);
      return PositionIndex(posX, posY);
    }


    PositionIndex add(int _x, int _y)
    {
      return PositionIndex(x + _x, y + _y);
    }


    bool equal(PositionIndex positionIndex)
    {
      return x == positionIndex.x && y == positionIndex.y;
    }

    int x;
    int y;
  };

  enum class ActionButtonType
  {
    //Main,MainR
    Control,
    Move,

    //Sub
    Animate,
    Harvest,
    NextChoices,
    BackChoices,
    Reload,

    //SubR
    Front,
    Left,
    Right,
    Back,
    //Undefined
    Undefined,
  };

  bool isEqualPositionIndex(PositionIndex positionIndexA, PositionIndex positionIndexB);
};

#endif /* defined(_WhiteRoom_GameData_) */
