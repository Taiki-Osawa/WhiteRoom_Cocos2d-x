/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "AppDelegate.h"
#include "GameLayer.h"

// #define USE_AUDIO_ENGINE 1
// #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(3040, 1440);

static const  float CONTENT_SCALE_FACTOR = 1.0;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("WhiteRoom", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("WhiteRoom");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::SHOW_ALL);
    director->setContentScaleFactor(CONTENT_SCALE_FACTOR);

    register_all_packages();

    //setSpriteFrameCache----------------------------------------------------
    //@---------------------------------------------@
      auto spriteFrameCache = SpriteFrameCache::getInstance();
    //@---------------------------------------------@
    spriteFrameCache->addSpriteFramesWithFile("GameLayer/TouchingAnimation.plist");
    spriteFrameCache->addSpriteFramesWithFile("Square/SquareBasic.plist");
    spriteFrameCache->addSpriteFramesWithFile("Square/Grass.plist");
    spriteFrameCache->addSpriteFramesWithFile("Square/Forest.plist");
    spriteFrameCache->addSpriteFramesWithFile("Square/LightBlue.plist");
    spriteFrameCache->addSpriteFramesWithFile("Square/DeepBlue.plist");
    spriteFrameCache->addSpriteFramesWithFile("Square/Quarry.plist");
    spriteFrameCache->addSpriteFramesWithFile("Square/White.plist");
    spriteFrameCache->addSpriteFramesWithFile("Square/Noised.plist");
    spriteFrameCache->addSpriteFramesWithFile("Character/SantaGirl.plist");
    spriteFrameCache->addSpriteFramesWithFile("ActionButton/ActionButtonChangeParticle.plist");
    spriteFrameCache->addSpriteFramesWithFile("Item/Item.plist");
    spriteFrameCache->addSpriteFramesWithFile("Item/ItemFrame.plist");
    spriteFrameCache->addSpriteFramesWithFile("UserInterface/Slider.plist");
/*
    if you call these ...
    auto xxx = Sprite::createWithSpriteFrameName("xxx.png");
*/

    //setAnimation------------------------------------------------------
    #define SquareHighlightedDelayPerUnit 1.0
    //@---------------------------------------------@
      auto animationCache = AnimationCache::getInstance();
    //@---------------------------------------------@
    auto touchingAnimation = Animation::createWithSpriteFrames({}, 0.05, 1);
    for(int animationOrder = 0; animationOrder < 8; animationOrder++)
    {
      auto str = StringUtils::format("TouchingAnimation00%d.png",animationOrder);
      SpriteFrame* sprite = spriteFrameCache->getSpriteFrameByName(str);
      touchingAnimation->addSpriteFrame(sprite);
    }
    animationCache->addAnimation(touchingAnimation, "TouchingAnimation");

    auto actionButtonChangeParticleAnimation = Animation::createWithSpriteFrames({}, 0.0167, 1);
    for(int animationOrder = 1; animationOrder < 5; animationOrder++)
    {
      auto str = StringUtils::format("ChangeParticle00%d.png",animationOrder);
      SpriteFrame* sprite = spriteFrameCache->getSpriteFrameByName(str);
      actionButtonChangeParticleAnimation->addSpriteFrame(sprite);
    }
    animationCache->addAnimation(actionButtonChangeParticleAnimation, "ActionButtonChangeParticle");

    auto noisedAnimation = Animation::createWithSpriteFrames({spriteFrameCache->getSpriteFrameByName("NoisedNormal.png")}, 0.05, -1);
    for(int animationOrder = 1; animationOrder < 8; animationOrder++)
    {
      auto str = StringUtils::format("Noised00%d.png",animationOrder);
      SpriteFrame* sprite = spriteFrameCache->getSpriteFrameByName(str);
      noisedAnimation->addSpriteFrame(sprite);
    }
    animationCache->addAnimation(noisedAnimation, "Noised");

    auto grassAnimation = Animation::createWithSpriteFrames({spriteFrameCache->getSpriteFrameByName("GrassHighlighted.png")}, SquareHighlightedDelayPerUnit, -1);
    for(int animationOrder = 1; animationOrder < 4; animationOrder++)
    {
      auto str = StringUtils::format("GrassHighlighted00%d.png",animationOrder);
      SpriteFrame* sprite = spriteFrameCache->getSpriteFrameByName(str);
      grassAnimation->addSpriteFrame(sprite);
    }
    animationCache->addAnimation(grassAnimation, "Grass");

    auto forestAnimation = Animation::createWithSpriteFrames({spriteFrameCache->getSpriteFrameByName("ForestHighlighted.png")}, SquareHighlightedDelayPerUnit, -1);
    for(int animationOrder = 1; animationOrder < 4; animationOrder++)
    {
      auto str = StringUtils::format("ForestHighlighted00%d.png",animationOrder);
      SpriteFrame* sprite = spriteFrameCache->getSpriteFrameByName(str);
      forestAnimation->addSpriteFrame(sprite);
    }
    animationCache->addAnimation(forestAnimation, "Forest");

    auto lightBlueAnimation = Animation::createWithSpriteFrames({spriteFrameCache->getSpriteFrameByName("LightBlueHighlighted.png")}, SquareHighlightedDelayPerUnit, -1);
    for(int animationOrder = 1; animationOrder < 4; animationOrder++)
    {
      auto str = StringUtils::format("LightBlueHighlighted00%d.png",animationOrder);
      SpriteFrame* sprite = spriteFrameCache->getSpriteFrameByName(str);
      lightBlueAnimation->addSpriteFrame(sprite);
    }
    animationCache->addAnimation(lightBlueAnimation, "LightBlue");

    auto deepBlueAnimation = Animation::createWithSpriteFrames({spriteFrameCache->getSpriteFrameByName("DeepBlueHighlighted.png")}, SquareHighlightedDelayPerUnit, -1);
    for(int animationOrder = 1; animationOrder < 4; animationOrder++)
    {
      auto str = StringUtils::format("DeepBlueHighlighted00%d.png",animationOrder);
      SpriteFrame* sprite = spriteFrameCache->getSpriteFrameByName(str);
      deepBlueAnimation->addSpriteFrame(sprite);
    }
    animationCache->addAnimation(deepBlueAnimation, "DeepBlue");

    auto quarryAnimation = Animation::createWithSpriteFrames({spriteFrameCache->getSpriteFrameByName("QuarryHighlighted.png")}, SquareHighlightedDelayPerUnit, -1);
    for(int animationOrder = 1; animationOrder < 4; animationOrder++)
    {
      auto str = StringUtils::format("QuarryHighlighted00%d.png",animationOrder);
      SpriteFrame* sprite = spriteFrameCache->getSpriteFrameByName(str);
      quarryAnimation->addSpriteFrame(sprite);
    }
    animationCache->addAnimation(quarryAnimation, "Quarry");

    auto santaGirlFrontAnimation = Animation::createWithSpriteFrames({}, 0.4, -1);
    for(auto animationOrder : std::vector<int>{1, 2, 3, 2})
    {
      auto str = StringUtils::format("SantaGirlFront00%d.png", animationOrder);
      SpriteFrame* sprite = spriteFrameCache->getSpriteFrameByName(str);
      santaGirlFrontAnimation->addSpriteFrame(sprite);
    }
    animationCache->addAnimation(santaGirlFrontAnimation, "SantaGirlFrontAnimation");

    auto santaGirlLeftAnimation = Animation::createWithSpriteFrames({}, 0.4, -1);
    for(auto animationOrder : std::vector<int>{1, 2, 3, 2})
    {
      auto str = StringUtils::format("SantaGirlLeft00%d.png", animationOrder);
      SpriteFrame* sprite = spriteFrameCache->getSpriteFrameByName(str);
      santaGirlLeftAnimation->addSpriteFrame(sprite);
    }
    animationCache->addAnimation(santaGirlLeftAnimation, "SantaGirlLeftAnimation");

    auto santaGirlRightAnimation = Animation::createWithSpriteFrames({}, 0.4, -1);
    for(auto animationOrder : std::vector<int>{1, 2, 3, 2})
    {
      auto str = StringUtils::format("SantaGirlRight00%d.png", animationOrder);
      SpriteFrame* sprite = spriteFrameCache->getSpriteFrameByName(str);
      santaGirlRightAnimation->addSpriteFrame(sprite);
    }
    animationCache->addAnimation(santaGirlRightAnimation, "SantaGirlRightAnimation");

    auto santaGirlBackAnimation = Animation::createWithSpriteFrames({}, 0.4, -1);
    for(auto animationOrder : std::vector<int>{1, 2, 3, 2})
    {
      auto str = StringUtils::format("SantaGirlBack00%d.png", animationOrder);
      SpriteFrame* sprite = spriteFrameCache->getSpriteFrameByName(str);
      santaGirlBackAnimation->addSpriteFrame(sprite);
    }
    animationCache->addAnimation(santaGirlBackAnimation, "SantaGirlBackAnimation");

    auto itemFrameAnimation = Animation::createWithSpriteFrames({}, 0.1, 1);
    for(auto animationOrder = 0; animationOrder < 7; animationOrder++)
    {
      auto str = StringUtils::format("ItemFrameBronze00%d.png", animationOrder);
      SpriteFrame* sprite = spriteFrameCache->getSpriteFrameByName(str);
      itemFrameAnimation->addSpriteFrame(sprite);
    }
    animationCache->addAnimation(itemFrameAnimation, "ItemFrameAnimation");

    // create a scene. it's an autorelease object
    auto scene = GameLayer::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
