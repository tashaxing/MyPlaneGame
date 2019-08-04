#include "game_scene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

static const int kBackgroundZorder = 0;
static const int kBattleZorder = 1;
static const int kWeaponZorder = 2;

static const Point kInitPoint = Point(-1, -1);

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
    addChild(m_sky_background, kBackgroundZorder);
    
    // 播放背景音乐(其实，音乐和音效最好用预先加载)
//    SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/game_music.wav", true); // mp3 not supported, use wav
    
    // 初始化玩家飞机
    m_player = Player::create();
    addChild(m_player, kBattleZorder);
    
    // 初始化敌机，调度敌机
    
    // 初始化道具，调度道具
    
    // 定时器触发调度飞机射击子弹
    schedule(schedule_selector(GameScene::generateBullet), 0.2); // bullet interval could be adjusted
    
    // 添加触摸事件监听
    auto touch_listener = EventListenerTouchOneByOne::create();
    touch_listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    touch_listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    touch_listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touch_listener, this);
    
    m_pretouch_pos = kInitPoint; // 未触摸时的初始点
    
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
    // NOTICE: dt is normally 0.0167, so other timer may not be faster than this
    //    CCLOG("update delta: %f", dt);
    
    for (Bullet* bullet : m_bullets)
    {
        // 若子弹出了屏幕，则销毁
        if (bullet->getPositionY() > Director::getInstance()->getVisibleSize().height)
        {
            m_bullets.eraseObject(bullet); // erase from vector should before itself cleanup
            bullet->removeFromParent();
        }
    }
    CCLOG("current bullets count: %d", m_bullets.size());
}

bool GameScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    CCLOG("onTouchBegan");
    m_pretouch_pos = touch->getLocation();
    m_preplayer_pos = m_player->getPosition();
    
    return true;
}

void GameScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    CCLOG("onTouchMoved");
    Point current_touch_pos = touch->getLocation();
    Vec2 move_delta = current_touch_pos - m_pretouch_pos;
    m_player->setPosition(m_preplayer_pos + move_delta);
}

void GameScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    CCLOG("onTouchEnded");
    m_pretouch_pos = kInitPoint;
}

void GameScene::generateBullet(float interval)
{
    // 由定时器触发产生子弹，也可以由某个按键触发
    // 根据玩家子弹状态产生不同的子弹，加到场景和管理器
    if (m_player->m_bullet_type == BulletType::BASE)
    {
        Bullet* bullet = m_player->shootSingle();
        addChild(bullet, kBattleZorder);
        m_bullets.pushBack(bullet);
    }
    else if (m_player->m_bullet_type == BulletType::POWER)
    {
        Vector<Bullet*> double_bullets = m_player->shootDouble();
        for (Bullet* bullet : double_bullets)
        {
            addChild(bullet, kBattleZorder);
            m_bullets.pushBack(bullet);
        }
    }
}

void GameScene::generateWeapon(float interval)
{
    
}

void GameScene::generateEnemy(float interval)
{
    
}
