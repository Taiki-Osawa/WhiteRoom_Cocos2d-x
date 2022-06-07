#include "Character.h"

USING_NS_CC;

Character::Character()
: _characterType(CharacterType::Undefined)
, _direction(Direction::Front)
, _isSequenceActionStack(false)
, _actionStack({})
, _itemContainer(ItemContainer())
{
}

Character* Character::create(CharacterType characterType)
{
  Character* pRet = new Character;
  if(pRet && pRet->init(characterType))
  {
    pRet->autorelease();
    return pRet;
  }
  else
  {
    delete pRet;
    pRet = nullptr;
    return nullptr;
  }
}

bool Character::init(CharacterType characterType)
{
  if(!Sprite::init()) return false;

  setCharacterType(characterType);
  setPositionIndexAndChangePosition(PositionIndex(8, 4));

  scheduleUpdate();

  return true;
}

void Character::setCharacterType(CharacterType characterType)
{
  _characterType = characterType;

  resetAnimation();
}

void Character::resetAnimation()
{
  switch(_characterType)
  {
    case CharacterType::SantaGirl:
    {
      auto santaGirlAnimationOnce = Animate::create(AnimationCache::getInstance()->getAnimation("SantaGirlFrontAnimation"));
      switch(_direction)
      {
        case Direction::Front:
        {
          santaGirlAnimationOnce = Animate::create(AnimationCache::getInstance()->getAnimation("SantaGirlFrontAnimation"));
          break;
        }
        case Direction::Left:
        {
          santaGirlAnimationOnce = Animate::create(AnimationCache::getInstance()->getAnimation("SantaGirlLeftAnimation"));
          break;
        }
        case Direction::Right:
        {
          santaGirlAnimationOnce = Animate::create(AnimationCache::getInstance()->getAnimation("SantaGirlRightAnimation"));
          break;
        }
        case Direction::Back:
        {
          santaGirlAnimationOnce = Animate::create(AnimationCache::getInstance()->getAnimation("SantaGirlBackAnimation"));
          break;
        }
        default:
        {
          break;
        }
      }
      stopAllActionsByTag(ActionTag::AT_Animation);
      auto santaGirlAnimation = Repeat::create(santaGirlAnimationOnce, -1);
      santaGirlAnimation->setTag(ActionTag::AT_Animation);

      runAction(santaGirlAnimation);
      break;
    }
    default:
    {
    }
  }
}

Character::CharacterType Character::getCharacterType() const
{
  return _characterType;
}

void Character::setDirection(Direction direction)
{
  if(_direction != direction)
  {
    _direction = direction;

    resetAnimation();
  }
}

Character::Direction Character::getDirection() const
{
  return _direction;
}

Character::PositionIndex Character::getPositionIndex() const
{
  return _positionIndex;
}

void Character::setPositionIndex(PositionIndex positionIndex)
{
  _positionIndex = positionIndex;
}

void Character::resetPosition()
{
  setPosition(getPositionForPositionIndex(_positionIndex));
}

void Character::setPositionIndexAndChangePosition(PositionIndex positionIndex)
{
  setPositionIndex(positionIndex);

  resetPosition();
}

Point Character::getPositionForPositionIndex(PositionIndex positionIndex)
{
  return Point(SQUARE_SIZE * (positionIndex.x - 0.5), SQUARE_SIZE * (positionIndex.y - 0.5));
}

void Character::update(float dt)
{
  TappedAction();
}

void Character::addActionStack(ActionButtonType actionButtonType)
{
  //_actionStack.push_back(actionButtonType);
  TappedActionForInternalData(actionButtonType);
}

void Character::TappedAction()
{
  if(!_isSequenceActionStack && !_actionStack.empty())
  {

    FiniteTimeAction* action;

    switch(_actionStack[0])
    {
      case CharacterAction::Front:
      {
        setDirection(Direction::Front);
        _actionStack.erase(_actionStack.begin());
        return;
      }
      case CharacterAction::Left:
      {
        setDirection(Direction::Left);
        _actionStack.erase(_actionStack.begin());
        return;
      }
      case CharacterAction::Right:
      {
        setDirection(Direction::Right);
        _actionStack.erase(_actionStack.begin());
        return;
      }
      case CharacterAction::Back:
      {
        setDirection(Direction::Back);
        _actionStack.erase(_actionStack.begin());
        return;
      }
      case CharacterAction::MoveFront:
      {
        //action = Sequence::create(MoveBy::create(0.1, Point(0, 200)), nullptr);
        setDirection(Direction::Front);
        action = MoveBy::create(0.1, Point(0, -200));
        break;
      }
      case CharacterAction::MoveLeft:
      {
        setDirection(Direction::Left);
        action = MoveBy::create(0.1, Point(-200, 0));
        break;
      }
      case CharacterAction::MoveRight:
      {
        setDirection(Direction::Right);
        action = MoveBy::create(0.1, Point(200, 0));
        break;
      }
      case CharacterAction::MoveBack:
      {
        setDirection(Direction::Back);
        action = MoveBy::create(0.1, Point(0, 200));
        break;
      }
      default:
      {
        action = CallFunc::create([](){
          CCLOG("error : NON-defined Character::TappedAction");
        });
      }
    }

    _isSequenceActionStack = true;
    auto func = CallFunc::create([this](){
      this->_actionStack.erase(_actionStack.begin());
      this->_isSequenceActionStack = false;
    });
    runAction(Sequence::create(action, func, nullptr));
  }
}

void Character::TappedActionForInternalData(ActionButtonType actionButtonType)
{
  switch(actionButtonType)
  {
    case ActionButtonType::Front:
    {
      auto positionIndex = _positionIndex.add(0, -1).inarea();
      if(_positionIndex.equal(positionIndex))
      {
        _actionStack.push_back(CharacterAction::Front);
      }
      else
      {
        _positionIndex = positionIndex;
        _actionStack.push_back(CharacterAction::MoveFront);
      }
      break;
    }
    case ActionButtonType::Left:
    {
      auto positionIndex = _positionIndex.add(-1, 0).inarea();
      if(_positionIndex.equal(positionIndex))
      {
        _actionStack.push_back(CharacterAction::Left);
      }
      else
      {
        _positionIndex = positionIndex;
        _actionStack.push_back(CharacterAction::MoveLeft);
      }
      break;
    }
    case ActionButtonType::Right:
    {
      auto positionIndex = _positionIndex.add(1, 0).inarea();
      if(_positionIndex.equal(positionIndex))
      {
        _actionStack.push_back(CharacterAction::Right);
      }
      else
      {
        _positionIndex = positionIndex;
        _actionStack.push_back(CharacterAction::MoveRight);
      }
      break;
    }
    case ActionButtonType::Back:
    {
      auto positionIndex = _positionIndex.add(0, 1).inarea();
      if(_positionIndex.equal(positionIndex))
      {
        _actionStack.push_back(CharacterAction::Back);
      }
      else
      {
        _positionIndex = positionIndex;
        _actionStack.push_back(CharacterAction::MoveBack);
      }
      break;
    }
    default:
    {
      break;
    }
  }
}
