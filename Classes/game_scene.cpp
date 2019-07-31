#include "game_scene.h"

USING_NS_CC;

Scene *GameScene::createScene()
{
    Scene *game_scene = Scene::create();
    Layer *game_layer = GameScene::create();
    game_scene->addChild(game_layer);
    return game_scene;
}

bool GameScene::init()
{
    if (!Layer::init())
        return false;
    
    CCLOG("game init");
    
    // init sky background and rotate moving
    
    // 添加触摸事件监听
    auto touch_listener = EventListenerTouchOneByOne::create();
    touch_listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    touch_listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    touch_listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touch_listener, this);
    
    scheduleUpdate();
    
    return true;
}

void GameScene::onEnter()
{
    Layer::onEnter();
    CCLOG("GameScene onEnter");
}

void GameScene::onExit()
{
    Layer::onExit();
    CCLOG("GameScene onExit");
}

void GameScene::update(float dt)
{
//    CCLOG("update");
}

bool GameScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    CCLOG("onTouchBegan");
    
    return true;
}

void GameScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    CCLOG("onTouchMoved");
}

void GameScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    CCLOG("onTouchEnded");
}
