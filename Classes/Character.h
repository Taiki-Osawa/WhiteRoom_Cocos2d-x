#ifndef _WhiteRoom_Character_
#define _WhiteRoom_Character_

#include "cocos2d.h"
#include "GameData.h"
#include "Item.h"

class Character : public cocos2d::Sprite, GameData
{
public:
  enum class CharacterType
  {
    SantaGirl,
    Undefined,
  };
  enum class Direction
  {
    Front,
    Left,
    Right,
    Back,
  };
  enum class CharacterAction
  {
    Front,
    Left,
    Right,
    Back,
    MoveFront,
    MoveLeft,
    MoveRight,
    MoveBack,
  };
  enum ActionTag
  {
    AT_Animation = 0,
  };

  Character();
  static Character* create(CharacterType characterType);
  virtual bool init(CharacterType characterType);

  CC_SYNTHESIZE(ItemContainer, _itemContainer, ItemContainer);

  CC_PROPERTY(Direction, _direction, Direction);
  CC_PROPERTY(CharacterType, _characterType, CharacterType);
  CC_PROPERTY(PositionIndex, _positionIndex, PositionIndex);

  void resetPosition();
  void setPositionIndexAndChangePosition(PositionIndex positionIndex);
  static cocos2d::Point getPositionForPositionIndex(PositionIndex positionIndex);

  void update(float dt) override;
  void addActionStack(ActionButtonType actionButtonType);
  void TappedAction();
  void TappedActionForInternalData(ActionButtonType actionButtonType);

protected:
  std::vector<CharacterAction> _actionStack;
  bool _isSequenceActionStack;
  void resetAnimation();
};

#endif /* defined(_WhiteRoom_Character_) */
