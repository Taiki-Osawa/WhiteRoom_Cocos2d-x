#include "GameLayer.h"

USING_NS_CC;

#define WINSIZE Director::getInstance()->getWinSize()
#define MOVE_INCH 7.0f/160.0f

Scene* GameLayer::createScene()
{
  auto scene = Scene::create();
  auto layer = GameLayer::create();

  scene->addChild(layer);

  return scene;
}

GameLayer* GameLayer::create()
{
  GameLayer *pRet = new GameLayer();
  pRet->init();
  pRet->autorelease();

  return pRet;
}

bool GameLayer::init()
{
  if(!Layer::init()) return false;

  auto touchListener = EventListenerTouchAllAtOnce::create();
  touchListener->onTouchesBegan = CC_CALLBACK_2(GameLayer::onTouchesBegan, this);
  touchListener->onTouchesMoved = CC_CALLBACK_2(GameLayer::onTouchesMoved, this);
  touchListener->onTouchesEnded = CC_CALLBACK_2(GameLayer::onTouchesEnded, this);
//  touchListener->onTouchesCancelled = CC_CALLBACK_2(GameLayer::onTouchesCancelled, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
 //eventlistener
  initScrollMap();
  CCLOG("CLEAR : GameLayer::initScrollMap (next -> GameLayer::initUserInterface)");
  initUserInterface();
  CCLOG("CLEAR : GameLayer::initUserInterface (next -> GameLayer::initActionButton)");
  initActionButton();
  getActionButtonMain()->reloadActionButtonSub();
  CCLOG("CLEAR : GameLayer::initActionButton (GameLayer::next -> //)");


  return true;
}

void GameLayer::initScrollMap()
{
  auto scrollMap = ScrollMap::create();
  addChild(scrollMap, Z_ScrollMap, T_ScrollMap);
}

void GameLayer::initUserInterface()
{
  auto userInterface = UserInterface::create();
  addChild(userInterface, Z_UserInterface, T_UserInterface);
}

void GameLayer::initActionButton()
{
  auto actionButton = ActionButton::create(ActionButton::ActionButtonClass::Main, GameData::ActionButtonType::Move);
  actionButton->setPosition(WINSIZE / 3);
  addChild(actionButton, Z_ActionButton, T_ActionButtonMain);

  auto actionButtonR = ActionButton::create(ActionButton::ActionButtonClass::MainR, GameData::ActionButtonType::Move);
  actionButtonR->setPosition(Point(WINSIZE.width * 2 / 3, WINSIZE.height / 3));
  addChild(actionButtonR, Z_ActionButton, T_ActionButtonMainR);
}

void GameLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
  for(auto touch : touches)
  {
    TouchingAnimation(touch->getLocation());
  }
}
//onTouchesBegan

void GameLayer::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
{
  for(auto touch : touches)
  {
    TouchingAnimation(touch->getLocation());
  }
}
//onTouchesMoved

void GameLayer::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
{
  for(auto touch : touches)
  {
    auto scrollMap = getScrollMap();
    auto userInterface = getUserInterface();
    auto touchSquare = scrollMap->getTouchSquare(scrollMap->convertTouchToScrollMapSpace(touch));
    auto touchMoveDistance = touch->getLocation().getDistance(touch->getStartLocation());
    if(touchSquare && std::fabs(scrollMap->convertDistanceFromPointToInch(touchMoveDistance)) < MOVE_INCH)
    {
      reorderChildren(userInterface);

      squareDetail(touchSquare);
    }
  }
}

void GameLayer::squareDetail(Square* square)
{
  auto scrollMap = getScrollMap();
  auto userInterface = getUserInterface();

  scrollMap->setSquareDetailed(square);
  userInterface->squareDetail(square);
}

void GameLayer::reorderChildren(Node* node)
{
  for(auto child : getChildren())
  {
    if(child->getLocalZOrder() == Z_Highlighted)
    {
      switch(child->getTag())
      {
        case T_ScrollMap:
        {
          reorderChild(child, Z_ScrollMap);
          break;
        }
        case T_ActionButtonMain:
        case T_ActionButtonMainR:
        {
          reorderChild(child, Z_ActionButton);
          break;
        }
        case T_UserInterface:
        {
          reorderChild(child, Z_UserInterface);
          break;
        }
        default:
        {
          break;
        }
      }
    }

    if(node == child)
    {
      reorderChild(child, Z_Highlighted);
    }
  }
}

ScrollMap* GameLayer::getScrollMap()
{
  if(auto scrollMap = (ScrollMap*)getChildByTag(T_ScrollMap))
  {
    return scrollMap;
  }
  CCLOG("error : [GameLayer] couldn't find [ScrollMap]");
  return nullptr;
}

UserInterface* GameLayer::getUserInterface()
{
  if(auto userInterface = (UserInterface*)getChildByTag(T_UserInterface))
  {
    return userInterface;
  }
  CCLOG("error : [GameLayer] couldn't find [UserInterface]");
  return nullptr;
}

Character* GameLayer::getCharacter()
{
  if(auto scrollMap = getScrollMap())
  {
    for(auto node : scrollMap->getContainer()->getChildren())
    {
      auto character = dynamic_cast<Character*>(node);
      if(character)
      {
        return character;
      }
    }
  }
  CCLOG("error : [GameLayer] couldn't find [Character]");
  return nullptr;
}

ActionButton* GameLayer::getActionButtonMain()
{
  if(auto actionButtonMain = (ActionButton*)getChildByTag(T_ActionButtonMain))
  {
    return actionButtonMain;
  }
  CCLOG("error : [GameLayer] couldn't find [ActionButtonMain]");
  return nullptr;
}

void GameLayer::TouchingAnimation(Point touchPos)
{
  auto touchingSprite = Sprite::createWithSpriteFrameName("TouchingAnimation000.png");
  touchingSprite->setPosition(touchPos);
  addChild(touchingSprite, Z_TouchingAnimation, T_TouchingAnimation);

  auto touchingAnimation = Animate::create(AnimationCache::getInstance()->getAnimation("TouchingAnimation"));
  auto removeSelf = RemoveSelf::create();

  touchingSprite->runAction(Sequence::create(touchingAnimation, removeSelf, nullptr));
  /*
  auto cache = SpriteFrameCache::getInstance();
  cache->addSpriteFramesWithFile("TouchingAnimation.plist");
  */ //setSpriteFrameCache
  /*
  auto _animation = Animation::create();
  for(int animationOrder = 0; animationOrder < 8; animationOrder++)
  {
    auto str = StringUtils::format("TouchingAnimation00%d.png",animationOrder);
    SpriteFrame* sprite = cache->getSpriteFrameByName(str);
    _animation->addSpriteFrame(sprite);
  }
  _animation->setDelayPerUnit(0.05);

  AnimationCache::getInstance()->addAnimation(_animation, "FlipingWings");
  */ //setAnimation
}
