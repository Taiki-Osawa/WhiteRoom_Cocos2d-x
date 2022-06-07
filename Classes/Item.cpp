#include "Item.h"

USING_NS_CC;

Item::Item(std::string name, int number)
: _name(name)
, _number(number)
{
  _serialNumber = ItemList::getInstance()->getSerialNumber(name);
}

void Item::addNumber(int number)
{
  setNumber(number + _number);
}

std::string Item::getFileName()
{
  return ItemList::getInstance()->getFileName(_name);
}

ItemLottery::ItemLottery(std::string name, int weight, float average, float standardDeviation)
: _name(name)
, _weight(weight)
, _distNumber(std::normal_distribution<float>(average, standardDeviation))
, _average(average)
, _standardDeviation(standardDeviation)
{
}

void ItemLottery::setAverage(float average)
{
  _average = average;

  _distNumber = std::normal_distribution<float>(_average, _standardDeviation);
}

float ItemLottery::getAverage() const
{
  return _average;
}

void ItemLottery::setStandardDeviation(float standardDeviation)
{
  _standardDeviation = standardDeviation;

  _distNumber = std::normal_distribution<float>(_average, _standardDeviation);
}

float ItemLottery::getStandardDeviation() const
{
  return _standardDeviation;
}

ItemTable::ItemTable(int lotteryNumber, std::vector<ItemLottery> itemLotterys)
{
  std::random_device device;
  _engine = std::default_random_engine(device());
}

void ItemTable::addItemLottery(ItemLottery itemLottery)
{
  auto itr = _itemLotterys.begin();
  while(itr != _itemLotterys.end())
  {
    if(itr->getName() == itemLottery.getName())
    {
      itr->setWeight(itr->getWeight() + itemLottery.getWeight());
      itr->setAverage(itr->getAverage() + itemLottery.getAverage());
      break;
    }
    itr++;
  }
  if(itr == _itemLotterys.end())
  {
    _itemLotterys.push_back(itemLottery);
  }
}

void ItemTable::mixItemTable(ItemTable itemTable)
{
  for(auto itemLottery : itemTable._itemLotterys)
  {
    addItemLottery(itemLottery);
  }
}

ItemContainer ItemTable::lottery(int lotteryNumber)
{
  auto itemContainer = ItemContainer();

  std::vector<int> totalWeight{};
  for(int number = 0; number < _itemLotterys.size(); number++)
  {
    std::vector<int> weight(number, _itemLotterys[number].getWeight());
    totalWeight.insert(totalWeight.end(), weight.begin(), weight.end());
  }

  std::uniform_int_distribution<> distWeight(0, totalWeight.size() - 1);

  for(int lotteryCounter = 0; lotteryCounter < lotteryNumber; lotteryCounter++)
  {
    auto itemLottery = _itemLotterys[totalWeight[distWeight(_engine)]];

    itemContainer.addItem(Item(itemLottery.getName(), std::max(0, (int)itemLottery._distNumber(_engine))));
  }

  return itemContainer;
}

ItemContainer::ItemContainer(std::vector<Item> items)
: _container(items)
{
}

void ItemContainer::addItem(Item item)
{
  auto itr = _container.begin();
  while(itr != _container.end())
  {
    if(itr->getSerialNumber() == item.getSerialNumber())
    {
      itr->addNumber(item.getNumber());
      break;
    }
    else if(itr->getSerialNumber() > item.getSerialNumber())
    {
      _container.insert(itr, item);
      break;
    }
    itr++;
  }
  if(itr == _container.end())
    _container.push_back(item);
}

void ItemContainer::addItemContainer(ItemContainer itemContainer)
{
  for(auto item : itemContainer.getContainer())
  {
    addItem(item);
  }
}

ItemList* ItemList::itemList = NULL;

ItemList::ItemList()
{
  std::string csvStr = FileUtils::getInstance()->getStringFromFile("Item/ItemList.csv");
  ValueVector balls = this->split(csvStr, "\n");
  ValueVector keys = this->split(balls.at(0).asString(), ",");
  for(int i = 1; i < (int)balls.size(); i++)
  {
    ValueMap itemDetail;
    ValueVector params = this->split(balls.at(i).asString(), ",");
    if(params.at(0).asString().empty())
    {
      break;
    }
    itemDetail["SerialNumber"] = i - 1;
    for(int j = 1; j < (int)params.size(); j++)
    {
      itemDetail[keys.at(j).asString()] = params.at(j).asString();
    }

    _itemList[params.at(0).asString()] = itemDetail;
  }
}

ItemList* ItemList::getInstance()
{
  if(itemList == NULL)
  {
    itemList = new ItemList();
  }
  return itemList;
}

ValueVector ItemList::split(const std::string &str, const std::string &delim)
{
    ValueVector res;
    size_t current = 0, found;
    while((found = str.find_first_of(delim, current)) != std::string::npos)
    {
        res.push_back(Value(std::string(str, current, found - current)));
        current = found + 1;
    }
    res.push_back(Value(std::string(str, current, str.size() - current)));
    return res;
}

std::string ItemList::getFileName(std::string name)//ASSERT
{
  if(_itemList.find(name) == _itemList.end())
  {
    CCLOG("Error : ItemList::getFileName(%s)", name.c_str());
    CCLOG("        ItemList::_itemList has NO-KEY(%s)", name.c_str());
    assert(false);
  }

  if(_itemList.at(name).asValueMap().find("FileName") == _itemList.at(name).asValueMap().end())
  {
    CCLOG("Error : ItemList::getFileName(%s)", name.c_str());
    CCLOG("        ItemList::_itemList(%s) has NO-KEY(FileName)", name.c_str());
    assert(false);
  }

  auto fileName = _itemList.at(name).asValueMap().at("FileName").asString();

  if(fileName.empty())
  {
    CCLOG("Error : ItemList::getFileName(%s)", name.c_str());
    CCLOG("        ItemList::_itemList(%s)(FileName) has EMPTY", name.c_str());
    assert(false);
  }

  return fileName;
}

int ItemList::getSerialNumber(std::string name)
{
  if(_itemList.find(name) == _itemList.end())
  {
    CCLOG("Error : ItemList::getSerialNumber(%s)", name.c_str());
    CCLOG("        ItemList::_itemList has NO-KEY(%s)", name.c_str());
    assert(false);
  }

  if(_itemList.at(name).asValueMap().find("SerialNumber") == _itemList.at(name).asValueMap().end())
  {
    CCLOG("Error : ItemList::getSerialNumber(%s)", name.c_str());
    CCLOG("        ItemList::_itemList(%s) has NO-KEY(SerialNumber)", name.c_str());
    assert(false);
  }

  auto serialNumber = _itemList.at(name).asValueMap().at("SerialNumber").asInt();

  if(serialNumber == 0)
  {
    CCLOG("Warning : ItemList::getSerialNumber(%s)", name.c_str());
    CCLOG("          ItemList::_itemList(%s)(SerialNumber) equals to [0]", name.c_str());
    assert(false);
  }

  return serialNumber;
}
