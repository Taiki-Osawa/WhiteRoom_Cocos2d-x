#ifndef _WhiteRoom_Item_
#define _WhiteRoom_Item_

#include "cocos2d.h"

class Item
{
public:
  Item(std::string name = "None", int number = 1);

  CC_SYNTHESIZE_READONLY(int, _serialNumber, SerialNumber);
  CC_SYNTHESIZE(int, _number, Number);
  CC_SYNTHESIZE_READONLY(std::string, _name, Name);

  std::string getFileName();
  void addNumber(int number);
};

class ItemLottery
{
protected:
public:
  ItemLottery(std::string = "None", int weight = 1, float average = 1.0, float standardDeviation = 0.0);

  std::normal_distribution<float> _distNumber;
  CC_PROPERTY(float, _average, Average);
  CC_PROPERTY(float, _standardDeviation, StandardDeviation);
  CC_SYNTHESIZE_READONLY(std::string, _name, Name);
  CC_SYNTHESIZE(int, _weight, Weight);
};

class ItemContainer
{
protected:
public:
  ItemContainer(std::vector<Item> items = {});

  CC_SYNTHESIZE(std::vector<Item>, _container, Container);

  void addItem(Item item);
  void addItemContainer(ItemContainer itemContainer);
};

class ItemTable
{
protected:
  std::default_random_engine _engine;
public:
  ItemTable(int lotteryNumber = 0, std::vector<ItemLottery> itemLotterys = {});

  std::vector<ItemLottery> _itemLotterys;
  CC_SYNTHESIZE(int, _lotteryNumber, LotteryNumber);

  ItemContainer lottery(int lotteryNumber);

  void addItemLottery(ItemLottery itemLottery);
  void mixItemTable(ItemTable itemTable);
};

class ItemList
{
protected:
  ItemList();
  static ItemList* itemList;

  cocos2d::ValueMap _itemList;
public:
  static ItemList* getInstance();

  cocos2d::ValueVector split(const std::string &str, const std::string &delim);

  std::string getFileName(std::string name);
  int getSerialNumber(std::string name);
};

#endif /* defined(_WhiteRoom_Item_) */

//baryte
