#include "ScrollMap.h"

USING_NS_CC;

#define SQUARE_X 30
#define SQUARE_SPACE_X 25
#define SQUARE_Y 20
#define SQUARE_SPACE_Y 15
#define SQUARE_SIZE 200

#define WINSIZE Director::getInstance()->getWinSize()
#define MOVE_INCH 7.0f/160.0f

ScrollMap::ScrollMap()
: _squareDetailed(nullptr)
{
  std::random_device device;
  _engine = std::default_random_engine(device());
}

ScrollMap* ScrollMap::create()
{
  ScrollMap *pRet = new ScrollMap();
  if(pRet && pRet->init())
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
  return pRet;
}

bool ScrollMap::init()
{
  if(!extension::ScrollView::init()) return false;
/*
//  auto touchListener = EventListenerTouchAllAtOnce::create();
//  touchListener->onTouchesBegan = CC_CALLBACK_2(ScrollMap::onTouchesBegan, this);
//  touchListener->onTouchesMoved = CC_CALLBACK_2(ScrollMap::onTouchesMoved, this);
//  touchListener->onTouchesEnded = CC_CALLBACK_2(ScrollMap::onTouchesEnded, this);
//  touchListener->onTouchesCancelled = CC_CALLBACK_2(ScrollMap::onTouchesCancelled, this);
//  _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
*/ //touchlistener
  auto layer = LayerColor::create(Color4B::YELLOW, SQUARE_SIZE * SQUARE_X, SQUARE_SIZE * SQUARE_Y);

  setSwallowTouches(false);
  setViewSize(WINSIZE);
  setDirection(Direction::BOTH);
  setDelegate(this);
  setBounceable(true);
  setMinScale(std::max(WINSIZE.width / layer->getContentSize().width, WINSIZE.height / layer->getContentSize().height));
  setMaxScale(7.0f);

  setContainer(layer);
  setContentSize(layer->getContentSize());

  initSquare();
  CCLOG("CLEAR : ScrollMap::initSquare (next : ScrollMap::initCharacter)");
  initCharacter();
  CCLOG("CLEAR : ScrollMap::initCharacter (next : ScrollMap:://)");

  return true;
}

void ScrollMap::initSquare()
{
  for(int x = 1 - SQUARE_SPACE_X; x <= SQUARE_X + SQUARE_SPACE_X; x++)
  {
    for(int y = 1 - SQUARE_SPACE_Y; y <= SQUARE_Y + SQUARE_SPACE_Y; y++)
    {
      newSquare(PositionIndex(x, y));
    }
  }
}

Square* ScrollMap::newSquare(PositionIndex positionIndex)
{
  Square* square;
  if(1 <= positionIndex.x && positionIndex.x <= SQUARE_X && 1 <= positionIndex.y && positionIndex.y <= SQUARE_Y)
  {
    square = Square::create();
  }
  else
  {
    square = Square::create("Noised");
  }

  square->setPositionIndexAndChangePosition(positionIndex);
  addChild(square, Z_Square);

  return square;
}

void ScrollMap::initCharacter()
{
  auto santaGirl = Character::create(Character::CharacterType::SantaGirl);
  addChild(santaGirl, Z_Character);
}

void ScrollMap::setSquareDetailed(Square* squareDetailed)
{
  if(_squareDetailed)
  {
    _squareDetailed->stopDetailedAnimation();
  }

  _squareDetailed = squareDetailed;

  if(_squareDetailed)
  {
    _squareDetailed->detailedAnimation();
  }
}

Square* ScrollMap::getSquareDetailed() const
{
  return _squareDetailed;
}

void ScrollMap::animate(PositionIndex positionIndex)
{
  if(isEqualPositionIndex(positionIndex, POSITIONINDEX_CHARACTER))
  {
    positionIndex = getCharacter()->getPositionIndex();
  }

  auto square = getSquare(positionIndex);
  std::vector<SquareType> animateTable
  {
    "Grass",
    "Forest",
    "LightBlue",
    "DeepBlue",
    "Quarry"
  };
  auto distAnimate = std::discrete_distribution<int>{1, 1, 1, 1, 1};
  square->setSquareType(animateTable[distAnimate(_engine)]);
  square->setHighlighted(true);
}

void ScrollMap::harvest(PositionIndex positionIndex)
{
  if(isEqualPositionIndex(positionIndex, POSITIONINDEX_CHARACTER))
  {
    positionIndex = getCharacter()->getPositionIndex();
  }

  auto square = getSquare(positionIndex);
  square->setHighlighted(false);


  auto harvestItem = SquareList::getInstance()->getItemTable(square->getSquareType()).lottery(1);
  getCharacter()->getItemContainer().addItemContainer(harvestItem);
  /*
  for(auto item : getItemContainer())
  {
    auto item = Sprite::createWithSpriteFrameName(item.getFileName());
    item->setPosition();
  }
  入手したアイテムを可視化するアニメーションする部分
  */
}

Character* ScrollMap::getCharacter()
{
  for(auto node : getContainer()->getChildren())
  {
    auto character = dynamic_cast<Character*>(node);
    if(character)
    {
      return character;
    }
  }
  CCLOG("error : [ScrollMap] couldn't find [Character]");
  return nullptr;
}


Square* ScrollMap::getSquare(PositionIndex positionIndex)
{
  int tag = Square::generateTag(positionIndex);
  auto square = (Square*)getContainer()->getChildByTag(tag);
  return square;
}

Square* ScrollMap::getTouchSquare(Point touchPos)
{
  int x = 1;
  while(x * SQUARE_SIZE <= touchPos.x)
  {
    x++;
  }
  while((x - 1) * SQUARE_SIZE > touchPos.x)
  {
    x--;
  }

  int y = 0;
  while(y * SQUARE_SIZE <= touchPos.y)
  {
    y++;
  }
  while((y - 1) * SQUARE_SIZE > touchPos.y)
  {
    y--;
  }

  int tag = Square::generateTag(PositionIndex(x, y));
  auto square = (Square*)getContainer()->getChildByTag(tag);
  return square;
  /*
  for(x = 1; x <= SQUARE_X; x++)
  {
    for(y = 1; y <= SQUARE_Y; y++)
    {
      int tag = Square::generateTag(PositionIndex(x, y));
      auto square = (Square*)getChildByTag(tag)
      if(square)
      {

      }
    }
  }
*/
}

Point ScrollMap::convertTouchToScrollMapSpace(Touch* touch)
{
  auto touchPos = (convertTouchToNodeSpace(touch) - getContainer()->getPosition()) / getZoomScale();

  return touchPos;
}

float ScrollMap::convertDistanceFromPointToInch(float pointDis)
{
    auto glview = Director::getInstance()->getOpenGLView();
    float factor = ( glview->getScaleX() + glview->getScaleY() ) / 2;
    return pointDis * factor / Device::getDPI();
}

/*
void ScrollMap::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
  for(auto touch : touches)
  {
    //    auto touchSquare = getTouchSquare(convertTouchToScrollMapSpace(touch));
  }
}
*/ //onTouchesBegan
/*
void ScrollMap::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
{
}
*/ //onTouchesMoved
/*
void ScrollMap::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
{
  for(auto touch : touches)
  {
    auto touchSquare = getTouchSquare(convertTouchToScrollMapSpace(touch));
    auto touchMoveDistance = touch->getLocation().getDistance(touch->getStartLocation());
    CCLOG("distance = %f", touchMoveDistance);
    if(touchSquare && std::fabs(convertDistanceFromPointToInch(touchMoveDistance)) < MOVE_INCH)
    {
      _squareDetailed = touchSquare;
      _squareDetailed
      switch(touchSquare->getSquareType())
      {
        case Square::SquareType::White:
        {
          touchSquare->setSquareType(Square::SquareType::Test);
          break;
        }
        case Square::SquareType::Test:
        {
          touchSquare->setSquareType(Square::SquareType::White);
          break;
        }
        default:
        {
          CCLOG("irregular!SquareType");
        }
      }
    }
  }
}
*/ //onTouchesEnded
/*
void ScrollMap::onTouchesCancelled(const std::vector<Touch*>& touches, Event *unused_event)
{

}
*/ //Cancelled
