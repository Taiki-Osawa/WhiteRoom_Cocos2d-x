#ifndef _WhiteRoom_Square_
#define _WhiteRoom_Square_

#include "cocos2d.h"
#include "GameData.h"
#include "Item.h"
#include <string>
#include <random>

using SquareType = std::string;

class Square : public cocos2d::Sprite, GameData
{
protected:
  enum ActionTag
  {
    AT_Detailed = 0,
    AT_Highlighted,
    AT_Normal,
    //AT_Noised,
  };

  std::string getStringPositionIndex();
  std::string getStringSquareType();
  bool _highlighted;

  std::default_random_engine _engine;
  std::normal_distribution<float> _distHighlighted;
public:

  Square();
  static Square* create(SquareType type = "White");
  virtual bool init(SquareType type);

  CC_PROPERTY(PositionIndex, _positionIndex, PositionIndex);
  CC_PROPERTY(SquareType, _squareType, SquareType);

  void resetPosition();
  void setPositionIndexAndChangePosition(PositionIndex positionIndex);
  static cocos2d::Point getPositionForPositionIndex(PositionIndex positionIndex);
  static int generateTag(PositionIndex positionIndex);
  std::string getSquareImageFilePath();

  std::map<std::string, std::vector<std::string>> getSquareDetail();

  bool isHighlighted();
  void setHighlighted(bool highlighted);

  void detailedAnimation();
  void stopDetailedAnimation();
};

class SquareList
{
protected:
  SquareList();
  static SquareList* squareList;

  cocos2d::ValueMap _squareList;
  std::map<SquareType, ItemTable> _itemTableList;
public:
  static SquareList* getInstance();

  cocos2d::ValueVector split(const std::string &str, const std::string &delim);

  std::string getFileName(SquareType squareType);
  std::string getFileNameNormal(SquareType squareType);
  std::string getFileNameHighlighted(SquareType squareType);
  std::string getAnimationNormal(SquareType squareType);
  std::string getAnimationHighlighted(SquareType squareType);
  ItemTable getItemTable(SquareType squareType);
};


#endif /* defined(_WhiteRoom_Square_) */
