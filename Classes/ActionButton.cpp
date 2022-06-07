#include "ActionButton.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define ACTIONBUTTON_HEIGHT 3
#define MOVE_INCH 7.0f/160.0f

#define WINSIZE Director::getInstance()->getWinSize()

ActionButton::ActionButton()
: _actionButtonClass(ActionButtonClass::Main)
, _actionButtonType(ActionButtonType::Undefined)
, _positionIndex(PositionIndex(0, 0))
, _touchable(true)
, _touchMoved(false)
, _touchPoint(Point(0, 0))
, _page(0)
, _choices({})
{
}

ActionButton* ActionButton::create(ActionButtonClass actionButtonClass, ActionButtonType actionButtonType)
{
  ActionButton* pRet = new ActionButton();
  if(pRet && pRet->init(actionButtonClass, actionButtonType))
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

bool ActionButton::init(ActionButtonClass actionButtonClass, ActionButtonType actionButtonType)
{
  if(!extension::ControlButton::init()) return false;

  initActionButtonClassAndType(actionButtonClass, actionButtonType);
  addTargetWithActionForControlEvents(this, cccontrol_selector(ActionButton::TappedAction), Control::EventType::TOUCH_UP_INSIDE);

  return true;
}

void ActionButton::initActionButtonClassAndType(ActionButtonClass actionButtonClass, ActionButtonType actionButtonType)
{
  initWithLabelAndBackgroundSprite(Label::createWithSystemFont("", "Helvetica", 12), Scale9Sprite::create(), false);
  setActionButtonClass(actionButtonClass);
  setActionButtonType(actionButtonType);

  switch(actionButtonClass)
  {
    case ActionButtonClass::Main:
    {
      std::vector<PositionIndex> actionButtonPositionIndexSet = {PositionIndex(0, -1), PositionIndex(0, 1), PositionIndex(1, 0), PositionIndex(2, -1), PositionIndex(2, 0), PositionIndex(2, 1), PositionIndex(3, -1), PositionIndex(3, 0), PositionIndex(3, 1)};
      for(auto actionButtonPositionIndex : actionButtonPositionIndexSet)
      {
        auto button = ActionButton::create(ActionButtonClass::Sub, ActionButtonType::Undefined);
        button->setPositionIndexAndChangePosition(actionButtonPositionIndex);
        addChild(button, Z_Sub);
      }

      auto reloadButton = ActionButton::create(ActionButtonClass::Sub, ActionButtonType::Reload);
      reloadButton->setPositionIndexAndChangePosition(PositionIndex(-1, 0));
      addChild(reloadButton, Z_Sub);

      auto background = Sprite::create("ActionButton/BackgroundTEST.png");
      background->setPosition(Point(ACTIONBUTTON_SIZE / 2, ACTIONBUTTON_SIZE / 2 + 200));
      addChild(background, Z_Background, T_Background);

      break;
    }
    case ActionButtonClass::MainR:
    {
      std::map<ActionButtonType, PositionIndex> actionButtonTypeAndPositionIndexSet{{ActionButtonType::Front, PositionIndex(-1, 0)}, {ActionButtonType::Left, PositionIndex(0, -1)}, {ActionButtonType::Right, PositionIndex(0, 1)}, {ActionButtonType::Back, PositionIndex(1, 0)}};
      for(auto actionButtonTypeAndPositionIndex : actionButtonTypeAndPositionIndexSet)
      {
        auto button = ActionButton::create(ActionButtonClass::SubR, actionButtonTypeAndPositionIndex.first);
        button->setPositionIndexAndChangePosition(actionButtonTypeAndPositionIndex.second);
        addChild(button, Z_Sub);
      }

      auto background = Sprite::create("ActionButtonR/Background.png");
      background->setPosition(Point(ACTIONBUTTON_SIZE / 2, ACTIONBUTTON_SIZE / 2 + 200));
      addChild(background, Z_Background, T_Background);

      break;
    }
    case ActionButtonClass::Sub:
    {
      setOpacity(0);
      setTouchabled(false);
      break;
    }
    case ActionButtonClass::SubR:
    {
      setOpacity(0);
      setTouchabled(false);
      break;
    }
    default:
    {
      break;
    }
  }
}

void ActionButton::setActionButtonType(ActionButtonType actionButtonType)
{
  auto opacity = getOpacity();

  if(opacity != 0 && getParent() && actionButtonType != _actionButtonType)
  {
    auto actionButtonChangeParticle = Sprite::createWithSpriteFrameName("ChangeParticle001.png");
    actionButtonChangeParticle->setPosition(Point(ACTIONBUTTON_SIZE / 2, ACTIONBUTTON_SIZE / 2));
    addChild(actionButtonChangeParticle);
    auto actionButtonChangeParticleAnimation = Animate::create(AnimationCache::getInstance()->getAnimation("ActionButtonChangeParticle"));
    auto removeSelf = RemoveSelf::create();
    actionButtonChangeParticle->runAction(Sequence::create(actionButtonChangeParticleAnimation, removeSelf, nullptr));
  }

  _actionButtonType = actionButtonType;

  switch(actionButtonType)
  {
    case ActionButtonType::Control:
    {
      switch(_actionButtonClass)
      {
        case ActionButtonClass::Main:
        {
          setBackgroundSpriteForState(Scale9Sprite::create("ActionButton/ControlNormal.png"), Control::State::NORMAL);
          setBackgroundSpriteForState(Scale9Sprite::create("ActionButton/ControlHighlighted.png"), Control::State::HIGH_LIGHTED);
          break;
        }
        case ActionButtonClass::MainR:
        {
          setBackgroundSpriteForState(Scale9Sprite::create("ActionButtonR/ControlNormal.png"), Control::State::NORMAL);
          setBackgroundSpriteForState(Scale9Sprite::create("ActionButtonR/ControlHighlighted.png"), Control::State::HIGH_LIGHTED);
          break;
        }
        default:
        {
          break;
        }
      }
      break;
    }
    case ActionButtonType::Move:
    {
      switch(_actionButtonClass)
      {
        case ActionButtonClass::Main:
        {
          setBackgroundSpriteForState(Scale9Sprite::create("ActionButton/MoveNormal.png"), Control::State::NORMAL);
          setBackgroundSpriteForState(Scale9Sprite::create("ActionButton/MoveHighlighted.png"), Control::State::HIGH_LIGHTED);
          break;
        }
        case ActionButtonClass::MainR:
        {
          setBackgroundSpriteForState(Scale9Sprite::create("ActionButtonR/MoveNormal.png"), Control::State::NORMAL);
          setBackgroundSpriteForState(Scale9Sprite::create("ActionButtonR/MoveHighlighted.png"), Control::State::HIGH_LIGHTED);
          break;
        }
        default:
        {
          break;
        }
      }
      break;
    }
    case ActionButtonType::Front:
    {
      setBackgroundSpriteForState(Scale9Sprite::create("ActionButtonR/FrontNormal.png"), Control::State::NORMAL);
      setBackgroundSpriteForState(Scale9Sprite::create("ActionButtonR/FrontHighlighted.png"), Control::State::HIGH_LIGHTED);
      break;
    }
    case ActionButtonType::Left:
    {
      setBackgroundSpriteForState(Scale9Sprite::create("ActionButtonR/LeftNormal.png"), Control::State::NORMAL);
      setBackgroundSpriteForState(Scale9Sprite::create("ActionButtonR/LeftHighlighted.png"), Control::State::HIGH_LIGHTED);
      break;
    }
    case ActionButtonType::Right:
    {
      setBackgroundSpriteForState(Scale9Sprite::create("ActionButtonR/RightNormal.png"), Control::State::NORMAL);
      setBackgroundSpriteForState(Scale9Sprite::create("ActionButtonR/RightHighlighted.png"), Control::State::HIGH_LIGHTED);
      break;
    }
    case ActionButtonType::Back:
    {
      setBackgroundSpriteForState(Scale9Sprite::create("ActionButtonR/BackNormal.png"), Control::State::NORMAL);
      setBackgroundSpriteForState(Scale9Sprite::create("ActionButtonR/BackHighlighted.png"), Control::State::HIGH_LIGHTED);
      break;
    }
    case ActionButtonType::BackChoices:
    {
      setBackgroundSpriteForState(Scale9Sprite::create("ActionButton/BackChoicesNormal.png"), Control::State::NORMAL);
      setBackgroundSpriteForState(Scale9Sprite::create("ActionButton/BackChoicesHighlighted.png"), Control::State::HIGH_LIGHTED);
      break;
    }
    case ActionButtonType::NextChoices:
    {
      setBackgroundSpriteForState(Scale9Sprite::create("ActionButton/NextChoicesNormal.png"), Control::State::NORMAL);
      setBackgroundSpriteForState(Scale9Sprite::create("ActionButton/NextChoicesHighlighted.png"), Control::State::HIGH_LIGHTED);
      break;
    }
    case ActionButtonType::Reload:
    {
      setBackgroundSpriteForState(Scale9Sprite::create("ActionButton/ReloadNormal.png"), Control::State::NORMAL);
      setBackgroundSpriteForState(Scale9Sprite::create("ActionButton/ReloadHighlighted.png"), Control::State::HIGH_LIGHTED);
      break;
    }
    case ActionButtonType::Animate:
    {
      setBackgroundSpriteForState(Scale9Sprite::create("ActionButton/AnimateNormal.png"), Control::State::NORMAL);
      setBackgroundSpriteForState(Scale9Sprite::create("ActionButton/AnimateHighlighted.png"), Control::State::HIGH_LIGHTED);
      break;
    }
    case ActionButtonType::Harvest:
    {
      setBackgroundSpriteForState(Scale9Sprite::create("ActionButton/HarvestNormal.png"), Control::State::NORMAL);
      setBackgroundSpriteForState(Scale9Sprite::create("ActionButton/HarvestHighlighted.png"), Control::State::HIGH_LIGHTED);
      break;
    }
    default:
    {
      setBackgroundSpriteForState(Scale9Sprite::create("ActionButton/UndefinedNormal.png"), Control::State::NORMAL);
      setBackgroundSpriteForState(Scale9Sprite::create("ActionButton/UndefinedHighlighted.png"), Control::State::HIGH_LIGHTED);
      break;
    }
  }

  if(opacity == 0)
  {
    setOpacity(0);
  }
}

ActionButton::ActionButtonType ActionButton::getActionButtonType() const
{
  return _actionButtonType;
}

void ActionButton::setPositionIndexAndChangePosition(PositionIndex positionIndex)
{
  setPositionIndex(positionIndex);

  resetPosition();
}

bool ActionButton::isTouchabled()
{
  return _touchable;
}

void ActionButton::setTouchabled(bool touchable)
{
  _touchable = touchable;
}

void ActionButton::TappedAction(Ref* sender, Control::EventType controlEvent)
{
  getGameLayer()->squareDetail(nullptr);

  auto actionButtonChangeParticle = Sprite::createWithSpriteFrameName("ChangeParticle001.png");
  actionButtonChangeParticle->setPosition(Point(ACTIONBUTTON_SIZE / 2, ACTIONBUTTON_SIZE / 2));
  addChild(actionButtonChangeParticle);
  auto actionButtonChangeParticleAnimation = Animate::create(AnimationCache::getInstance()->getAnimation("ActionButtonChangeParticle"));
  auto removeSelf = RemoveSelf::create();
  actionButtonChangeParticle->runAction(Sequence::create(actionButtonChangeParticleAnimation, removeSelf, nullptr));

  switch(_actionButtonClass)
  {
    case ActionButtonClass::Main:
    {
      switch(_actionButtonType)
      {
        case ActionButtonType::Control:
        {
          setTouchabled(false);
          setActionButtonType(ActionButtonType::Move);
          for(auto node : getChildren())
          {
            auto actionButtonSub = dynamic_cast<ActionButton*>(node);
            if(actionButtonSub)
            {
              actionButtonSub->fadeOutAnimation();
            }
          }
          auto delay = DelayTime::create(0.1 * ACTIONBUTTON_HEIGHT);
          auto setTouchable = CallFunc::create([this](){
            this->setTouchabled(true);
          });
          runAction(Sequence::create(delay, setTouchable, nullptr));
          break;
        }
        case ActionButtonType::Move:
        {
          setTouchabled(false);
          setActionButtonType(ActionButtonType::Control);
          for(auto node : getChildren())
          {
            auto actionButtonSub = dynamic_cast<ActionButton*>(node);
            if(actionButtonSub)
            {
              actionButtonSub->fadeInAnimation();
            }
          }
          auto delay = DelayTime::create(0.1 * ACTIONBUTTON_HEIGHT);
          auto setTouchable = CallFunc::create([this](){
            this->setTouchabled(true);
            this->pageActionButton();
          });
          runAction(Sequence::create(delay, setTouchable, nullptr));
          break;
        }
        default:
        {
          break;
        }
      }
      break;
    }
    case ActionButtonClass::MainR:
    {
      switch(_actionButtonType)
      {
        case ActionButtonType::Control:
        {
          setTouchabled(false);
          setActionButtonType(ActionButtonType::Move);
          for(auto node : getChildren())
          {
            auto actionButtonSub = dynamic_cast<ActionButton*>(node);
            if(actionButtonSub)
            {
              actionButtonSub->fadeOutAnimation();
            }
          }
          auto delay = DelayTime::create(0.1 * ACTIONBUTTON_HEIGHT);
          auto setTouchable = CallFunc::create([this](){
            this->setTouchabled(true);
          });
          runAction(Sequence::create(delay, setTouchable, nullptr));
          break;
        }
        case ActionButtonType::Move:
        {
          setTouchabled(false);
          setActionButtonType(ActionButtonType::Control);
          for(auto node : getChildren())
          {
            auto actionButtonSub = dynamic_cast<ActionButton*>(node);
            if(actionButtonSub)
            {
              actionButtonSub->fadeInAnimation();
            }
          }
          auto delay = DelayTime::create(0.1 * ACTIONBUTTON_HEIGHT);
          auto setTouchable = CallFunc::create([this](){
            this->setTouchabled(true);
          });
          runAction(Sequence::create(delay, setTouchable, nullptr));
          break;
        }
        default:
        {
          break;
        }
      }
      break;
    }
    case ActionButtonClass::Sub:
    {
      switch(_actionButtonType)
      {
        /*
        case ActionButtonType::Back:
        {
          auto parent = getParent();
          if(parent)
          {
            auto actionButtonParent = dynamic_cast<ActionButton*>(parent);
            if(actionButtonParent)
            {
              actionButtonParent->setActionButtonType(ActionButtonType::Move);
              actionButtonParent->setTouchabled(false);
              for(auto node : actionButtonParent->getChildren())
              {
                auto actionButtonSub = dynamic_cast<ActionButton*>(node);
                if(actionButtonSub)
                {
                  actionButtonSub->fadeOutAnimation();
                }
              }
              auto delay = DelayTime::create(0.1 * ACTIONBUTTON_HEIGHT);
              auto setTouchableParent = CallFunc::create([actionButtonParent](){
                actionButtonParent->setTouchabled(true);
              });
              actionButtonParent->runAction(Sequence::create(delay, setTouchableParent, nullptr));
            }
          }
          break;
        }
        */
        case ActionButtonType::Animate:
        {
          getGameLayer()->getScrollMap()->animate();
          getActionButtonMain()->reloadActionButtonSub();
          break;
        }
        case ActionButtonType::Harvest:
        {
          getGameLayer()->getScrollMap()->harvest();
          getActionButtonMain()->reloadActionButtonSub();
          break;
        }
        case ActionButtonType::BackChoices:
        {
          getActionButtonMain()->pageActionButton(-1);
          break;
        }
        case ActionButtonType::NextChoices:
        {
          getActionButtonMain()->pageActionButton(1);
          break;
        }
        case ActionButtonType::Reload:
        {
          getActionButtonMain()->reloadActionButtonSub();
        }
        default:
        {
          break;
        }
      }
      break;
    }
    case ActionButtonClass::SubR:
    {
      switch(_actionButtonType)
      {
        case ActionButtonType::Front:
        {
          getGameLayer()->getCharacter()->addActionStack(ActionButtonType::Front);
          getGameLayer()->getActionButtonMain()->reloadActionButtonSub();
          break;
        }
        case ActionButtonType::Left:
        {
          getGameLayer()->getCharacter()->addActionStack(ActionButtonType::Left);
          getGameLayer()->getActionButtonMain()->reloadActionButtonSub();
          break;
        }
        case ActionButtonType::Right:
        {
          getGameLayer()->getCharacter()->addActionStack(ActionButtonType::Right);
          getGameLayer()->getActionButtonMain()->reloadActionButtonSub();
          break;
        }
        case ActionButtonType::Back:
        {
          getGameLayer()->getCharacter()->addActionStack(ActionButtonType::Back);
          getGameLayer()->getActionButtonMain()->reloadActionButtonSub();
          break;
        }
        default:
        {
          break;
        }
      }
      break;
    }
    default:
    {
      break;
    }
  }
}

void ActionButton::pageActionButton(int pageDelta)
{
  if(_actionButtonClass != ActionButtonClass::Main)
  {
    CCLOG("error : [pageActionButton] is not used for [ActionButtonType::Main]");
    return;
  }

  _page += pageDelta;

  std::vector<PositionIndex> actionButtonPositionIndexSet = {PositionIndex(2, 0), PositionIndex(3, -1), PositionIndex(1, 0), PositionIndex(3, 0), PositionIndex(3, 1), PositionIndex(2, -1), PositionIndex(2, 1)};

  for(int counter = (_page - 1) * 7; counter < _page * 7; counter++)
  {
    if(counter < _choices.size())
    {
      getActionButtonSub(actionButtonPositionIndexSet[counter % 7])->setActionButtonType(_choices[counter]);
    }
    else
    {
      getActionButtonSub(actionButtonPositionIndexSet[counter % 7])->setActionButtonType(ActionButtonType::Undefined);
    }
  }

  if(_page >= 2)
  {
    getActionButtonSub(PositionIndex(0, -1))->setActionButtonType(ActionButtonType::BackChoices);
  }
  else
  {
    getActionButtonSub(PositionIndex(0, -1))->setActionButtonType(ActionButtonType::Undefined);
  }

  if(_page * 7 < _choices.size())
  {
    getActionButtonSub(PositionIndex(0, 1))->setActionButtonType(ActionButtonType::NextChoices);
  }
  else
  {
    getActionButtonSub(PositionIndex(0, 1))->setActionButtonType(ActionButtonType::Undefined);
  }
}

void ActionButton::reloadActionButtonSub(GameData::PositionIndex positionIndex)
{
  if(_actionButtonClass != ActionButtonClass::Main)
  {
    CCLOG("error : [reloadActionButtonSub] is not used for [ActionButtonType::Main]");
    return;
  }

  if(isEqualPositionIndex(positionIndex, GameData::POSITIONINDEX_CHARACTER))
  {
    positionIndex = getGameLayer()->getCharacter()->getPositionIndex();
  }

  std::vector<ActionButtonType> choices{};

  //Animate && Harvest
  auto square = getGameLayer()->getScrollMap()->getSquare(positionIndex);
  if(square)
  {
    if(square->getSquareType() == "White")
    {
      choices.push_back(ActionButtonType::Animate);
    }

    if(square->isHighlighted())
    {
      choices.push_back(ActionButtonType::Harvest);
    }
  }

  _page = 1;
  _choices = choices;
  pageActionButton();
}

void ActionButton::fadeInAnimation()
{
  auto position = getVisiblePositionForPositionIndex(_positionIndex);
  auto delay = DelayTime::create(0.1 * (std::abs(_positionIndex.r) - 1));
  auto fadeIn = FadeIn::create(0.1);
  auto moveTo = MoveTo::create(0.1, position);
  auto setTouchable = CallFunc::create([this](){
    this->setTouchabled(true);
  });
  auto fadeInAnimation = Sequence::create(delay, Spawn::create(fadeIn, moveTo, nullptr), setTouchable, nullptr);
  runAction(fadeInAnimation);
}

Point ActionButton::getVisiblePositionForPositionIndex(PositionIndex positionIndex)
{
  if(positionIndex.r != 0)
  {
    auto sign = positionIndex.r / std::abs(positionIndex.r);

    return getPositionForPositionIndex(positionIndex) + Point(positionIndex.corner * ACTIONBUTTON_SIZE_WITHMARGIN * 0.5, (2 - std::abs(positionIndex.corner)) * ACTIONBUTTON_SIZE_WITHMARGIN * sign * 0.5);
  }

  return getPositionForPositionIndex(positionIndex) + Point(positionIndex.corner * ACTIONBUTTON_SIZE_WITHMARGIN, 0);
}

void ActionButton::fadeOutAnimation()
{
  auto position = getPositionForPositionIndex(_positionIndex);
  auto setUnTouchable = CallFunc::create([this](){
    this->setTouchabled(false);
  });
  auto delay = DelayTime::create(0.1 * (ACTIONBUTTON_HEIGHT - std::max(std::abs(_positionIndex.r), 1)));
  auto fadeOut = FadeOut::create(0.1);
  auto moveTo = MoveTo::create(0.1, position);
  auto fadeOutAnimation = Sequence::create(setUnTouchable, delay, Spawn::create(fadeOut, moveTo, nullptr), nullptr);
  runAction(fadeOutAnimation);
}

void ActionButton::resetPosition()
{
  setPosition(getPositionForPositionIndex(_positionIndex));
}

void ActionButton::setPositionIndex(PositionIndex positionIndex)
{
  _positionIndex = positionIndex;

  setTag(generateTag(_positionIndex));
}

ActionButton::PositionIndex ActionButton::getPositionIndex() const
{
  return _positionIndex;
}

Point ActionButton::getPositionForPositionIndex(PositionIndex positionIndex)
{
  if(positionIndex.r != 0)
  {
    auto sign = positionIndex.r / std::abs(positionIndex.r);
    return Point(ACTIONBUTTON_SIZE / 2, sign * (std::abs(positionIndex.r) - 1) * ACTIONBUTTON_SIZE_WITHMARGIN + ACTIONBUTTON_SIZE / 2);
  }

  return Point(ACTIONBUTTON_SIZE / 2, ACTIONBUTTON_SIZE / 2);
}

int ActionButton::generateTag(PositionIndex positionIndex)
{
  int tag = positionIndex.r * 10 + positionIndex.corner + 100;

  return tag;
}

bool ActionButton::onTouchBegan(Touch *touch, Event *unused_event)
{
  if (!isTouchInside(touch) || !isEnabled() || !isVisible() || !hasVisibleParents() || !_touchable)
  {
      return false;
  }

  for (Node *c = this->_parent; c != nullptr; c = c->getParent())
  {
      if (c->isVisible() == false)
      {
        CCLOG("ONTOUCH FALSE");
          return false;
      }
  }

  getGameLayer()->reorderChildren(this->getActionButtonMain());

  _touchMoved = true;
  _touchPoint = this->convertTouchToNodeSpace(touch);

  _isPushed = true;
  this->setHighlighted(true);
  sendActionsForControlEvents(Control::EventType::TOUCH_DOWN);
  return true;
}

void ActionButton::onTouchMoved(Touch *touch, Event *unused_event)
{
  if (!isEnabled() || !isPushed() || isSelected())
  {
    if (isHighlighted())
    {
      setHighlighted(false);
    }
    return;
  }

///*
  if(_actionButtonType == ActionButtonType::Move)
  { // scrolling
      Point moveDistance, newPoint;
      float newX, newY;

      newPoint     = this->convertTouchToNodeSpace(touch);
      moveDistance = newPoint - _touchPoint;

      float dis = sqrtf(moveDistance.x*moveDistance.x + moveDistance.y*moveDistance.y);

      if (!_touchMoved && fabs(convertDistanceFromPointToInch(dis)) < MOVE_INCH)
      {
          //CCLOG("Invalid movement, distance = [%f, %f], disInch = %f", moveDistance.x, moveDistance.y);
          return;
      }

      if (!_touchMoved)
      {
          moveDistance.setZero();
      }

      _touchMoved = true;
      _touchPoint = newPoint;

      setPosition(getPosition() + touch->getDelta());
    }
//*/
  bool isTouchMoveInside = isTouchInside(touch);

  if (isTouchMoveInside && !isHighlighted())
  {
      setHighlighted(true);
      sendActionsForControlEvents(Control::EventType::DRAG_ENTER);
  }
  else if (isTouchMoveInside && isHighlighted())
  {
      sendActionsForControlEvents(Control::EventType::DRAG_INSIDE);
  }
  else if (!isTouchMoveInside && isHighlighted())
  {
      setHighlighted(false);

      sendActionsForControlEvents(Control::EventType::DRAG_EXIT);
  }
  else if (!isTouchMoveInside && !isHighlighted())
  {
      sendActionsForControlEvents(Control::EventType::DRAG_OUTSIDE);
  }
}

void ActionButton::onTouchEnded(Touch *touch, Event *unused_event)
{
  _isPushed = false;
  setHighlighted(false);
  _touchMoved = false;

/*
  bool _isTouchStartInside = isTouchStartInside(touch);
  if(_actionButtonClass == ActionButtonClass::Sub)
  {
    auto parent = this->getParent();
    auto actionButtonParent = dynamic_cast<ActionButton*>(parent);
    if(actionButtonParent)
    {
      _isTouchStartInside = _isTouchStartInside || actionButtonParent->isTouchStartInside(touch);
    }
  }
*/
  auto touchMoveDistance = touch->getLocation().getDistance(touch->getStartLocation());
  if(_actionButtonType == ActionButtonType::Move && std::fabs(convertDistanceFromPointToInch(touchMoveDistance)) < MOVE_INCH && isTouchInside(touch))
  {
    sendActionsForControlEvents(Control::EventType::TOUCH_UP_INSIDE);
  }
  else if (_actionButtonType != ActionButtonType::Move && isTouchInside(touch))
  {
    sendActionsForControlEvents(Control::EventType::TOUCH_UP_INSIDE);
  }
  else
  {
    sendActionsForControlEvents(Control::EventType::TOUCH_UP_OUTSIDE);
  }
}

bool ActionButton::isTouchInside(Touch* touch)
{
    Vec2 touchLocation = touch->getLocation(); // Get the touch position
    touchLocation = this->getParent()->convertToNodeSpace(touchLocation);
    touchLocation = touchLocation - getPosition();

    float sizeX = getContentSize().width;
    float sizeY = getContentSize().height;

    bool isUpwardSloping = std::fabs(sizeX * touchLocation.y - sizeY * touchLocation.x) <= sizeX * sizeY / 2;
    bool isDownwardSloping = std::fabs(sizeX * touchLocation.y + sizeY * touchLocation.x) <= sizeX * sizeY / 2;

    return isUpwardSloping && isDownwardSloping;
} //ひし形について判定できる

float ActionButton::convertDistanceFromPointToInch(float pointDis)
{
    auto glview = Director::getInstance()->getOpenGLView();
    float factor = ( glview->getScaleX() + glview->getScaleY() ) / 2;
    return pointDis * factor / Device::getDPI();
}

GameLayer* ActionButton::getGameLayer()
{
  for(Node* node = this->getParent(); node != nullptr; node = node->getParent())
  {
    auto gameLayer = dynamic_cast<GameLayer*>(node);
    if(gameLayer)
    {
      return gameLayer;
    }
  }
  CCLOG("error : [ActionButton] couldn't find [GameLayer]");
  return nullptr;
}

ActionButton* ActionButton::getActionButtonMain()
{
  for(Node* node = this; node != nullptr; node = node->getParent())
  {
    if(auto actionButton = dynamic_cast<ActionButton*>(node))
    {
      if(actionButton->getActionButtonClass() == ActionButtonClass::Main || actionButton->getActionButtonClass() == ActionButtonClass::MainR)
      {
        return actionButton;
      }
    }
  }
  CCLOG("error : [ActionButton] couldn't find [ActionButtonClass::Main]");
  return nullptr;
}

ActionButton* ActionButton::getActionButtonSub(PositionIndex positionIndex)
{
  auto actionButtonSub = (ActionButton*)getChildByTag(generateTag(positionIndex));

  if(!actionButtonSub)
  {
    CCLOG("error : [ActionButton] couldn't find [ActionButtonClass::Sub]");
  }
  return actionButtonSub;
}
/*
bool ActionButton::isTouchStartInside(Touch* touch)
{
Vec2 touchLocation = touch->getStartLocation(); // Get the touch position
touchLocation = this->getParent()->convertToNodeSpace(touchLocation);
touchLocation = touchLocation - getPosition();

float sizeX = getContentSize().width;
float sizeY = getContentSize().height;

bool isUpwardSloping = std::fabs(sizeX * touchLocation.y - sizeY * touchLocation.x) <= sizeX * sizeY / 2;
bool isDownwardSloping = std::fabs(sizeX * touchLocation.y + sizeY * touchLocation.x) <= sizeX * sizeY / 2;

return isUpwardSloping && isDownwardSloping;
} //ひし形について判定できる

bool ActionButton::isTouch(Point touchPosAnchorZERO) //Ex.touchPosAnchorZERO =  _actionButton->convertTouchToNodeSpace(touch)
{
  float sizeX = getContentSize().width;
  float sizeY = getContentSize().height;
  Point touchPos = touchPosAnchorZERO - getContentSize() / 2

  bool isUpwardSloping = std::fabs(sizeX * touchPos.y - sizeY * touchPos.x) <= sizeX * sizeY / 2;
  bool isDownwardSloping = std::fabs(sizeX * touchPos.y + sizeY * touchPos.x) <= sizeX * sizeY / 2;

  return isUpwardSloping && isDownwardSloping;
} //ひし形について判定できる

*/
