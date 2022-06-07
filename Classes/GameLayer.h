#ifndef _WhiteRoom_GameLayer_
#define _WhiteRoom_GameLayer_

//#include "cocos2d.h"
//#include "Character.h"
//#include "Square.h"
#include "ScrollMap.h"
#include "UserInterface.h"
#include "ActionButton.h"

class ActionButton;

class GameLayer : public cocos2d::Layer
{
protected:
  enum Tag
  {
    T_ScrollMap = 0,
    T_ActionButtonMain,
    T_ActionButtonMainR,
    T_UserInterface,
    T_TouchingAnimation,
  };
  enum ZOrder
  {
    Z_ScrollMap = 0,
    Z_ActionButton,
    Z_UserInterface,
    Z_Highlighted,
    Z_TouchingAnimation,
  };

public:
  static cocos2d::Scene* createScene();
  static GameLayer* create();
  virtual bool init();

  void initScrollMap();
  void initUserInterface();
  void initActionButton();

  virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
  virtual void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
  virtual void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
//  virtual void onTouchesCancelled(std::vector<cocos2d::Touch*> touches, cocos2d::Event *unused_event);

  void reorderChildren(Node* node);

  ScrollMap* getScrollMap();
  Character* getCharacter();
  UserInterface* getUserInterface();
  ActionButton* getActionButtonMain();

  void squareDetail(Square* square);

  void TouchingAnimation(cocos2d::Point touchPos);
};

#endif /* defined(_WhiteRoom_GameLayer_) */
