#include "Square.h"

USING_NS_CC;

#define WINSIZE Director::getInstance()->getWinSize()

Square::Square()
: _positionIndex({0, 0})
, _squareType("White")
, _highlighted(false)
{
  std::random_device device;
  _engine = std::default_random_engine(device());
  _distHighlighted = std::normal_distribution<float>(10.0, 3.0);
}

Square* Square::create(SquareType type)
{
  Square* pRet = new Square();
  if(pRet && pRet->init(type))
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

bool Square::init(SquareType type)
{
  if(!Sprite::init()) return false;

  setSquareType(type);

  return true;
}

std::string Square::getStringPositionIndex()
{
  auto positionIndexString = "(" + std::to_string(_positionIndex.x) + ", " + std::to_string(_positionIndex.y) + ")";
  return positionIndexString;
}

std::string Square::getStringSquareType()
{
  auto filename = getSquareImageFilePath();
  auto squareTypeString = filename.substr(0, filename.length() - 4); // except ".png"
  return squareTypeString;
}

void Square::resetPosition()
{
  setPosition(getPositionForPositionIndex(_positionIndex));
}

Square::PositionIndex Square::getPositionIndex() const
{
  return _positionIndex;
}

void Square::setPositionIndex(PositionIndex positionIndex)
{
  _positionIndex = positionIndex;

  setTag(generateTag(_positionIndex));
}

SquareType Square::getSquareType() const
{
  return _squareType;
}

void Square::setSquareType(SquareType type)
{
  _squareType = type;
  auto squareImageFilePath = getSquareImageFilePath();

  setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(squareImageFilePath));

  if(_highlighted)
  {
    auto squareAnimationPath = SquareList::getInstance()->getAnimationHighlighted(_squareType);
    if(!squareAnimationPath.empty())
    {
      auto squareAnimation = Animate::create(AnimationCache::getInstance()->getAnimation(squareAnimationPath));
      squareAnimation->setTag(ActionTag::AT_Highlighted);

      runAction(squareAnimation);
    }
  }
  else
  {
    auto squareAnimationPath = SquareList::getInstance()->getAnimationNormal(_squareType);
    if(!squareAnimationPath.empty())
    {
      auto squareAnimation = Animate::create(AnimationCache::getInstance()->getAnimation(squareAnimationPath));
      squareAnimation->setTag(ActionTag::AT_Normal);

      runAction(squareAnimation);
    }
  }
}

void Square::setPositionIndexAndChangePosition(PositionIndex positionIndex)
{
  setPositionIndex(positionIndex);

  resetPosition();
}

Point Square::getPositionForPositionIndex(PositionIndex positionIndex)
{
  return Point(SQUARE_SIZE * (positionIndex.x - 0.5), SQUARE_SIZE * (positionIndex.y - 0.5));
}

int Square::generateTag(PositionIndex positionIndex)
{
  int tag = positionIndex.x + (positionIndex.y - 1) * 100;

  return tag;
}

std::string Square::getSquareImageFilePath()
{
  auto i = SquareList::getInstance();
  if(_highlighted)
  {
    return i->getFileNameHighlighted(_squareType);
    //return SquareList::getInstance()->getFileNameHighlighted(_squareType);
  }
  else
  {
    return i->getFileNameNormal(_squareType);
    //return SquareList::getInstance()->getFileNameNormal(_squareType);
  }
}

std::map<std::string, std::vector<std::string>> Square::getSquareDetail() // 不完全
{
  std::map<std::string, std::vector<std::string>> squareDetail;

  std::vector<std::string> positionIndex;
  positionIndex.push_back(getStringPositionIndex());
  squareDetail.insert(std::make_pair("Position", positionIndex));

  std::vector<std::string> squareType;
  squareType.push_back(getStringSquareType());
  squareDetail.insert(std::make_pair("SquareType", squareType));

  //TEST
  for(int i = 0; i++; i < 5)
  {
    std::vector<std::string> test;
    test.push_back("TEST_R");
    squareDetail.insert(std::make_pair("TEST_L", test));
  }
  //TEST

  return squareDetail;
}

bool Square::isHighlighted()
{
  return _highlighted;
}

void Square::setHighlighted(bool highlighted)
{
  if(_highlighted != highlighted)
  {
    _highlighted = highlighted;

    setSquareType(_squareType);

    if(!_highlighted)
    {
      stopAllActionsByTag(ActionTag::AT_Highlighted);
      auto delay = DelayTime::create(_distHighlighted(_engine));
      auto func = CallFunc::create([this]{
        this->setHighlighted(true);
      });
      runAction(Sequence::create(delay, func, nullptr));
    }
  }
}

void Square::detailedAnimation()
{
  auto tintTo1 = TintTo::create(0.6, 180, 180, 180);
  auto tintTo2 = TintTo::create(0.3, 160, 160, 160);
  auto tintTo3 = TintTo::create(0.3, 180, 180, 180);
  auto tintTo4 = TintTo::create(0.6, 255, 255, 255);
  auto delay = DelayTime::create(0.4);
  auto tintToOnce = Sequence::create(tintTo1, tintTo2, tintTo3, tintTo4, delay, nullptr);
  auto tintTo = Repeat::create(tintToOnce, -1);
  tintTo->setTag(ActionTag::AT_Detailed);

  runAction(tintTo);
}

void Square::stopDetailedAnimation()
{
  stopAllActionsByTag(ActionTag::AT_Detailed);

  runAction(TintTo::create(0, 255, 255, 255));
}

SquareList* SquareList::squareList = NULL;

SquareList::SquareList()
{
  std::string csvStrSquareList = FileUtils::getInstance()->getStringFromFile("Square/SquareList.csv");
  ValueVector rowsSquareList = this->split(csvStrSquareList, "\n");
  ValueVector keysSquareList = this->split(rowsSquareList.at(0).asString(), ",");
  for(int i = 1; i < (int)rowsSquareList.size(); i++)
  {
    ValueVector paramsSquare = this->split(rowsSquareList.at(i).asString(), ",");
    if(paramsSquare.at(0).asString().empty())
    {
      break;
    }
    ValueMap squareDetail;
    CCLOG("paramsSquare[SquareName] = %s", paramsSquare.at(0).asString().c_str());
    for(int j = 1; j < (int)paramsSquare.size(); j++)
    {
      CCLOG("%s", keysSquareList.at(j).asString().c_str());
      CCLOG("           = %s", paramsSquare.at(j).asString().c_str());
      squareDetail[keysSquareList.at(j).asString()] = paramsSquare.at(j).asString();
    }

    auto csvFilePath = squareDetail.at("CSVFile").asString();
    if(!csvFilePath.empty())
    {
      ItemTable itemTable(squareDetail.at("LotteryNumber").asInt());
      CCLOG("Loading CSV... /$Square/%s", csvFilePath.c_str());
      std::string csvStrItemTable = FileUtils::getInstance()->getStringFromFile("Square/" + csvFilePath);
      ValueVector rowsItemTable = this->split(csvStrItemTable, "\n");
      ValueVector keysItemTable = this->split(rowsItemTable.at(0).asString(), ",");
      for(int j = 1; j < (int)rowsItemTable.size(); j++)
      {
        ValueVector paramsItemTable = this->split(rowsItemTable.at(j).asString(), ",");
        if(paramsItemTable.at(0).asString().empty())
        {
          break;
        }
        ValueMap itemLottery;
        CCLOG("  //  Item  //");
        for(int k = 0; k < (int)paramsItemTable.size(); k++)
        {
          CCLOG("    %s", keysItemTable.at(k).asString().c_str());
          CCLOG("               = %s", paramsItemTable.at(k).asString().c_str());
          itemLottery[keysItemTable.at(k).asString()] = paramsItemTable.at(k).asString();
          CCLOG("itemName ... %s", itemLottery.at(keysItemTable.at(k).asString()).asString().c_str());
        }
        CCLOG("LOOP=END");
        CCLOG("itemName ... %s", itemLottery.at("Weight").asString().c_str());

/*
        auto itemName = itemLottery.at("ItemName").asString();
        CCLOG("itemName ...");
        auto weight = itemLottery.at("Weight").asInt();
        CCLOG("weight ...");
        auto average = itemLottery.at("Average").asFloat();
        CCLOG("average ...");
        auto standardDeviation = itemLottery.at("StandardDeviation").asFloat();
        CCLOG("standardDeviation ...");
*/
        itemTable.addItemLottery
        (
          ItemLottery
          (
//            itemName,
            itemLottery.at("ItemName").asString(),
//            weight,
            itemLottery.at("Weight").asInt(),
//            average,
            itemLottery.at("Average").asFloat(),
//            standardDeviation
            itemLottery.at("StandardDeviation").asFloat()
          )
        );
      }
      _itemTableList[paramsSquare.at(0).asString()] = itemTable;
    }
    _squareList[paramsSquare.at(0).asString()] = squareDetail;
  }
  CCLOG("SquareList::SquareList() Constructed!");
}

SquareList* SquareList::getInstance()
{
  if(squareList == NULL)
  {
    squareList = new SquareList();
  }
  return squareList;
}

ValueVector SquareList::split(const std::string &str, const std::string &delim)
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

std::string SquareList::getFileNameNormal(SquareType squareType)//ASSERT
{
  if(_squareList.find(squareType) == _squareList.end())
  {
    CCLOG("Error : SquareList::getFileNameNormal(%s)", squareType.c_str());
    CCLOG("        SquareList::_squareList has NO-KEY(%s)", squareType.c_str());
    assert(false);
  }

  if(_squareList.at(squareType).asValueMap().find("FileNameNormal") == _squareList.at(squareType).asValueMap().end())
  {
    CCLOG("Error : SquareList::getFileNameNormal(%s)", squareType.c_str());
    CCLOG("        SquareList::_squareList(%s) has NO-KEY(FileNameNormal)", squareType.c_str());
    assert(false);
  }

  auto fileNameNormal = _squareList.at(squareType).asValueMap().at("FileNameNormal").asString();

  if(fileNameNormal.empty())
  {
    CCLOG("Error : SquareList::getFileNameNormal(%s)", squareType.c_str());
    CCLOG("        SquareList::_squareList(%s)(FileNameNormal) has EMPTY", squareType.c_str());
    assert(false);
  }

  return fileNameNormal;
}

std::string SquareList::getFileNameHighlighted(SquareType squareType)//ASSERT
{
  if(_squareList.find(squareType) == _squareList.end())
  {
    CCLOG("Error : SquareList::getFileNameHighlighted(%s)", squareType.c_str());
    CCLOG("        SquareList::_squareList has NO-KEY(%s)", squareType.c_str());
    assert(false);
  }

  if(_squareList.at(squareType).asValueMap().find("FileNameHighlighted") == _squareList.at(squareType).asValueMap().end())
  {
    CCLOG("Error : SquareList::getFileNameHighlighted(%s)", squareType.c_str());
    CCLOG("        SquareList::_squareList(%s) has NO-KEY(FileNameHighlighted)", squareType.c_str());
    assert(false);
  }

  auto fileNameHighlighted = _squareList.at(squareType).asValueMap().at("FileNameHighlighted").asString();

  if(fileNameHighlighted.empty())
  {
    CCLOG("Error : SquareList::getFileNameHighlighted(%s)", squareType.c_str());
    CCLOG("        SquareList::_squareList(%s)(FileNameHighlighted) has EMPTY", squareType.c_str());
    assert(false);
  }

  return fileNameHighlighted;
}


std::string SquareList::getAnimationNormal(SquareType squareType)//OREMPTY
{
  if(_squareList.find(squareType) == _squareList.end())
  {
    CCLOG("Error : SquareList::getAnimationNormal(%s)", squareType.c_str());
    CCLOG("        SquareList::_squareList has NO-KEY(%s)", squareType.c_str());
    assert(false);
  }

  if(_squareList.at(squareType).asValueMap().find("AnimationNormal") == _squareList.at(squareType).asValueMap().end())
  {
    CCLOG("Error : SquareList::getAnimationNormal(%s)", squareType.c_str());
    CCLOG("        SquareList::_squareList(%s) has NO-KEY(AnimationNormal)", squareType.c_str());
    assert(false);
  }

  auto animationNormal = _squareList.at(squareType).asValueMap().at("AnimationNormal").asString();

  if(animationNormal.empty())
  {
    CCLOG("Warning : SquareList::getAnimationNormal(%s)", squareType.c_str());
    CCLOG("          SquareList::_squareList(%s)(AnimateNormal) has EMPTY", squareType.c_str());
  }

  return animationNormal;
}

std::string SquareList::getAnimationHighlighted(SquareType squareType)//OREMPTY
{
  if(_squareList.find(squareType) == _squareList.end())
  {
    CCLOG("Error : SquareList::getAnimationHighlighted(%s)", squareType.c_str());
    CCLOG("        SquareList::_squareList has NO-KEY(%s)", squareType.c_str());
    assert(false);
  }

  if(_squareList.at(squareType).asValueMap().find("AnimationHighlighted") == _squareList.at(squareType).asValueMap().end())
  {
    CCLOG("Error : SquareList::getAnimationHighlighted(%s)", squareType.c_str());
    CCLOG("        SquareList::_squareList(%s) has NO-KEY(AnimationHighlighted)", squareType.c_str());
    assert(false);
  }

  auto animationHighlighted = _squareList.at(squareType).asValueMap().at("AnimationHighlighted").asString();

  if(animationHighlighted.empty())
  {
    CCLOG("Warning : SquareList::getAnimationHighlighted(%s)", squareType.c_str());
    CCLOG("          SquareList::_squareList(%s)(AnimateHighlighted) has EMPTY", squareType.c_str());
  }

  return animationHighlighted;
}

ItemTable SquareList::getItemTable(SquareType squareType)
{
  if(_itemTableList.find(squareType) == _itemTableList.end())
  {
    CCLOG("Error : SquareList::getItemTable(%s)", squareType.c_str());
    CCLOG("        SquareList::_itemTableList has NO-KEY(%s)", squareType.c_str());
    assert(false);
  }

  return _itemTableList.at(squareType);
}
