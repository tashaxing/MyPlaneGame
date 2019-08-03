#include "player.h"

USING_NS_CC;

bool Player::init()
{
    if (!Sprite::init())
        return false;
    
    Size visible_size = Director::getInstance()->getVisibleSize();
    Point visible_origin = Director::getInstance()->getVisibleOrigin();
    
    // 初始化玩家飞机
    Sprite* hero_plane = Sprite::create("img/hero1.png");
    hero_plane->setPosition(visible_origin.x + visible_size.width / 2,
                            visible_origin.y + hero_plane->getContentSize().height);
    addChild(hero_plane);
    
    // 玩家飞机喷气动画
    // FIXME: actually should use preload animation cache
    Vector<SpriteFrame*> sprite_frames;
    for (int i = 1; i <= 2; i++)
    {
        
    }
    
    
    return true;
}

Bullet* shoot()
{
    // 子弹从飞机头部打出来
}

void Player::die()
{
    
}

