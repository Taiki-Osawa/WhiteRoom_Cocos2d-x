#ifndef _WhiteRoom_ScrollMap_
#define _WhiteRoom_ScrollMap_

//#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "Square.h"
#include "Character.h"
//#include "GameData.h"

class ScrollMap : public cocos2d::extension::ScrollView, cocos2d::extension::ScrollViewDelegate, GameData
{
protected:
  enum ZOrder
  {
    Z_Square = 0,
    Z_Character,
  };

  std::default_random_engine _engine;

public:
  ScrollMap();
  static ScrollMap* create();
  virtual bool init();

  CC_PROPERTY(Square*, _squareDetailed, SquareDetailed);

  void initSquare();
  void initCharacter();

  Square* newSquare(PositionIndex positionIndex);

  void harvest(PositionIndex positionIndex = POSITIONINDEX_CHARACTER);
  void animate(PositionIndex positionIndex = POSITIONINDEX_CHARACTER);

  Square* getTouchSquare(cocos2d::Point touchPos);
  Square* getSquare(PositionIndex positionIndex);
  Character* getCharacter();

  cocos2d::Point convertTouchToScrollMapSpace(cocos2d::Touch* touch);
  float convertDistanceFromPointToInch(float touchPos);

//  virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
//  virtual void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
//  virtual void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
//  virtual void onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
};

#endif /* defined(_WhiteRoom_ScrollMap_) */
