#ifndef _WhiteRoom_ActionButton_
#define _WhiteRoom_ActionButton_

//#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include <string>
#include "GameLayer.h"

#define ACTIONBUTTON_SIZE_WITHMARGIN 200
#define ACTIONBUTTON_SIZE 198

class GameLayer;

class ActionButton : public cocos2d::extension::ControlButton, GameData
{
protected:
  enum Tag
  {
    T_Background = 0,
    T_Sub,
    T_Main,
  };
  enum ZOrder
  {
    Z_Background = -1,
    Z_Sub,
    Z_Main,
  };

  bool _touchMoved;
  bool _touchable;
  cocos2d::Point _touchPoint;
  int _page;
  std::vector<ActionButtonType> _choices;
public:
  enum class ActionButtonClass
  {
    Main,
    MainR,
    Sub,
    SubR
  };
  struct PositionIndex
  {
    PositionIndex()
    {
      r = 0;
      corner = 0;
    }

    PositionIndex(int _r, int _corner)
    {
      r = _r;
      corner = _corner;
    }

    int r; //-1 : back, 1 < : over
    int corner; //-1 : left, 0 : center, 1 : right
  };

  ActionButton();
  static ActionButton* create(ActionButtonClass actionButtonClass, ActionButtonType actionButtonType);
  virtual bool init(ActionButtonClass actionButtonClass, ActionButtonType actionButtonType);

  void initActionButtonClassAndType(ActionButtonClass actionButtonClass, ActionButtonType actionButtonType);

  CC_SYNTHESIZE(ActionButtonClass, _actionButtonClass, ActionButtonClass);
  CC_PROPERTY(ActionButtonType, _actionButtonType, ActionButtonType);
  CC_PROPERTY(PositionIndex, _positionIndex, PositionIndex);

  void resetPosition();
  void setPositionIndexAndChangePosition(PositionIndex positionIndex);
  static cocos2d::Point getPositionForPositionIndex(PositionIndex positionIndex);
  static cocos2d::Point getVisiblePositionForPositionIndex(PositionIndex positionIndex);
  static int generateTag(PositionIndex positionIndex);

  bool isTouchabled();
  void setTouchabled(bool touchable);
  void TappedAction(cocos2d::Ref* sender, cocos2d::extension::Control::EventType controlEvent);

  void pageActionButton(int pageDelta = 0);
  void reloadActionButtonSub(GameData::PositionIndex positionIndex = GameData::POSITIONINDEX_CHARACTER);

  void fadeInAnimation();
  void fadeOutAnimation();

  virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
  virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
  virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;

  virtual bool isTouchInside(cocos2d::Touch *touch) override;

  float convertDistanceFromPointToInch(float pointDis);

  GameLayer* getGameLayer();
  ActionButton* getActionButtonMain();
  ActionButton* getActionButtonSub(PositionIndex positionIndex);
/*
  bool isTouchStartInside(cocos2d::Touch *touch);
  bool _isMove();
  bool isTouch(cocos2d::Point touchPosAnchorZERO); //Ex.touchPosAnchorZERO =  _actionButton->convertTouchToNodeSpace(touch)
*/
  /*
    CC_PROPERTY(PositionIndex, _positionIndex, PositionIndex);
    CC_PROPERTY(SquareType, _squareType, SquareType);

    void resetPosition();
    void setPositionIndexAndChangePosition(PositionIndex positionIndex);
    static cocos2d::Point getPositionForPositionIndex(PositionIndex positionIndex);
    static int generateTag(PositionIndex positionIndex);
    std::string getSquareImageFilePath(SquareType type);

    std::map<std::string, std::vector<std::string>> getSquareDetail();

    void detailedAnimation();
    void stopDetailedAnimation();
  */
};

#endif /* defined(_WhiteRoom_ActionButton_) */
