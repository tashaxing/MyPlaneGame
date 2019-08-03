#include "game_scene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

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
    
    Size visible_size = Director::getInstance()->getVisibleSize();
    Point visible_origin = Director::getInstance()->getVisibleOrigin();
    
    // 初始化天空背景并循环移动
    m_sky_background = SkyBackground::create();
    addChild(m_sky_background, 0);
    
    // 播放背景音乐
    SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/game_music.wav", true); // mp3 not supported, use wav
    
    // 初始化玩家飞机
    m_player = Player::create();
    addChild(m_player, 1);
    
    // 初始化敌机，调度敌机
    
    // 初始化道具，调度道具
    
    // 定时器触发调度飞机射击子弹
    
    // 添加触摸事件监听
    auto touch_listener = EventListenerTouchOneByOne::create();
    touch_listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    touch_listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    touch_listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touch_listener, this);
    
    // 默认渲染更新
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
