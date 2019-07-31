#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "bullet.h"

class Player : public cocos2d::Sprite
{
public:
    virtual bool init();
    
    CREATE_FUNC(Player);
    
public:
    Bullet* shoot(); // 射击一次，产生一颗子弹，根据武器类型不同射出的子弹不同，子弹射出后有自己的行为
    int m_weapon_type; // 根据获得的道具变换武器类型
    
public:
    void die();
};

#endif /* __PLAYER_H__ */
