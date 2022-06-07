#include "UserInterface.h"

USING_NS_CC;
//USING_NS_CC_EXT;

#define WINSIZE Director::getInstance()->getWinSize()
#define FONT_SIZE 150
#define MOVE_INCH 7.0f/160.0f

UserInterface::UserInterface()
{
}

UserInterface* UserInterface::create()
{
  UserInterface *pRet = new UserInterface();
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
}

bool UserInterface::init()
{
  if(!Layout::init())
  {
    return false;
  }
/*
  auto touchListener = EventListenerTouchOneByOne::create();
  touchListener->setSwallowTouches(true);
  touchListener->onTouchBegan = CC_CALLBACK_2(UserInterface::onTouchBegan, this);
  touchListener->onTouchMoved = CC_CALLBACK_2(UserInterface::onTouchMoved, this);
  touchListener->onTouchEnded = CC_CALLBACK_2(UserInterface::onTouchEnded, this);
//  touchListener->onTouchCancelled = CC_CALLBACK_2(UserInterface::onTouchCancelled, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
*/
  return true;
}

void UserInterface::detailBackground(float height)
{
  auto detailBackground = (Sprite*)getChildByTag(Tag::T_DetailBackground);

  if(detailBackground)
  {
    detailBackground->removeFromParent();
  }

  auto sliderPrev = (Node*)getChildByTag(Tag::T_Slider);
  if(sliderPrev)
  {
    sliderPrev->removeFromParent();
  }

  detailBackground = Sprite::create("UserInterface/SquareDetailBackground.png");
  detailBackground->setAnchorPoint(Point(0, 0));
  detailBackground->setPosition(Point(WINSIZE.width - detailBackground->getContentSize().width, 0));
  addChild(detailBackground, ZOrder::Z_DetailBackground, Tag::T_DetailBackground);

  if(height + 300 > WINSIZE.height)
  {
    for(auto heightCounter = WINSIZE.height; heightCounter < height + 300; heightCounter += WINSIZE.height)
    {
      auto detailBackgroundSub = Sprite::create("UserInterface/SquareDetailBackground.png");
      detailBackgroundSub->setAnchorPoint(Point(0, 0));
      detailBackgroundSub->setPosition(Point(0, -1 * heightCounter));
      detailBackground->addChild(detailBackgroundSub);
    }
    /*
    auto slider = ui::Slider::create();
    slider->loadBarTexture("Slider_Background.png", TextureResType::PLIST);
    slider->loadSlidBallTextures("Slider_NodeNormal.png", "Slider_NodePress.png", "Slider_NodeDisable.png", TextureResType::PLIST);
    slider->loadProgressBarTexture("Slider_PressBar.png", TextureResType::PLIST);

    slider->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
            switch (type)
            {
                    case ui::Widget::TouchEventType::BEGAN:
                            break;
                    case ui::Widget::TouchEventType::MOVED:
                            detailBackground->setPositionY((height - WINSIZE.height + 300) * slider->getPercent() / 100);
                            break;
                    case ui::Widget::TouchEventType::ENDED:
                            CCLOG("slider moved");
                            break;
                    default:
                            break;
            }
    });
    slider->setPosition(Point(WINSIZE.width - detailBackground->getContentSize().width / 2, 50));
    addChild(slider, Z_Slider, T_Slider);
    */
  }
}

void UserInterface::squareDetail(Square* square)
{
  auto squareDetailBackground = (Sprite*)getChildByTag(Tag::T_DetailBackground);
  if(!squareDetailBackground && !square) return;
  if(squareDetailBackground && !square)
  {
    squareDetailBackground->removeFromParent();

    auto slider = (Sprite*)getChildByTag(Tag::T_Slider);
  } //squareがなければSquareDetailBackground & sliderを削除
/*
  if(squareDetailBackground && square)
  {
    while(squareDetailBackground->getChildByTag(Tag::T_SquareDetail))
    {
      squareDetailBackground->removeChildByTag(Tag::T_SquareDetail);
    }
  } //squareDetailBackground上のすべてのnodeを削除
  if(!squareDetailBackground && square)
  {
    squareDetailBackground = Sprite::create("UserInterface/SquareDetailBackground.png");
    squareDetailBackground->setAnchorPoint(Point(0, 0));
    squareDetailBackground->setPosition(Point(WINSIZE.width - squareDetailBackground->getContentSize().width, 0));
    addChild(squareDetailBackground, ZOrder::Z_SquareDetailBackground, Tag::T_DetailBackground);
  } //BackgroundSquareDetailがなければBackgroundSquareDetailを表示
*/
  if(square)
  {
    TTFConfig labelConfig = {"Font/koku.ttf", FONT_SIZE};
    auto squareDetailMap = square->getSquareDetail();
    auto counter = 0;

    detailBackground(squareDetailMap.size() * 2 * FONT_SIZE);
    squareDetailBackground = (Sprite*)getChildByTag(Tag::T_DetailBackground);

    for(auto dataset : squareDetailMap)
    {
      auto category = Label::createWithTTF(labelConfig, dataset.first);
      category->setAnchorPoint(Point(0, 0));
      category->setPosition(Point(0, squareDetailBackground->getContentSize().height - FONT_SIZE * ++counter));
      squareDetailBackground->addChild(category, ZOrder::Z_SquareDetail, Tag::T_SquareDetail);
      for(auto data : dataset.second)
      {
        auto eachData = Label::createWithTTF(labelConfig, data);
        eachData->setAnchorPoint(Point(1, 0));
        eachData->setPosition(Point(squareDetailBackground->getContentSize().width, squareDetailBackground->getContentSize().height - FONT_SIZE * ++counter));
        squareDetailBackground->addChild(eachData, ZOrder::Z_SquareDetail, Tag::T_SquareDetail);
      }
    }
  } //Describe SquareDetail
}

/*
bool UserInterface::onTouchBegan(Touch *touch, Event *unused_event)
{
  auto isActionButtonTouch = _actionButton->isTouch(_actionButton->convertTouchToNodeSpace(touch)));
  return isActionButtonTouch;
}

void UserInterface::onTouchMoved(Touch *touch, Event *unused_event)
{
  if(_actionButton->isMove())
  {
    _actionButton->setPosition(_actionButton->getPosition() + touch->getDelta());
  }
}

void UserInterface::onTouchEnded(Touch *touch, Event *unused_event)
{
  auto scrollMap = (ScrollMap*)getChildByTag(T_ScrollMap);
  auto userInterface = (UserInterface*)getChildByTag(T_UserInterface);
  auto touchSquare = scrollMap->getTouchSquare(scrollMap->convertTouchToScrollMapSpace(touch));
  auto touchMoveDistance = touch->getLocation().getDistance(touch->getStartLocation());
  auto isActionButtonTouch = _actionButton->isTouch(_actionButton->convertTouchToNodeSpace(touch)));
  if(isActionButtonTouch && std::fabs(convertDistanceFromPointToInch(touchMoveDistance)) < MOVE_INCH)
  {
    switch(_actionButton->getActionButtonType())
    {
      case ActionButton::ActionButtonType::Move:
      {
        _actionButton->setActionButton(ActionButton::ActionButtonType::Control);
        break;
      }
      case ActionButton::ActionButtonType::Control:
      {
        _actionButton->setActionButton(ActionButton::ActionButtonType::Move);
        break;
      }
      default:
      {
        break;
      }
    }
  }
}

float UserInterface::convertDistanceFromPointToInch(float pointDis)
{
    auto glview = Director::getInstance()->getOpenGLView();
    float factor = ( glview->getScaleX() + glview->getScaleY() ) / 2;
    return pointDis * factor / Device::getDPI();
}
*/
