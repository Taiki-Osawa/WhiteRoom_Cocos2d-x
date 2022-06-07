#ifndef _WhiteRoom_UserInterface_
#define _WhiteRoom_UserInterface_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Square.h"
//#include "GameData.h"

class UserInterface : public cocos2d::ui::Layout
{
protected:
  enum Tag
  {
    T_DetailBackground = 0,
    T_SquareDetail,
    T_Slider,
  };
  enum ZOrder
  {
    Z_DetailBackground = 0,
    Z_SquareDetail,
    Z_Slider,
  };
public:
  UserInterface();
  static UserInterface* create();
  virtual bool init();

  void detailBackground(float height);
  void squareDetail(Square *square);
  void harvestItemDetail(ItemContainer harvestItem);

//  float convertDistanceFromPointToInch(float pointDis);

//  virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
//  virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
//  virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
//  virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event);
};
#endif /* defined(_WhiteRoom_UserInterface_) */
