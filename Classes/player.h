#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "bullet.h"
#include "weapon.h"

class Player : public cocos2d::Sprite
{
public:
    virtual bool init();
    
    CREATE_FUNC(Player);
    
public:
    Bullet* shootSingle(); // 射击一次，产生一颗子弹
    cocos2d::Vector<Bullet*> shootDouble(); // 射击一次，产生双子弹
    void getWeapon(int weapon_type);
    void destroy();
    
    BulletType m_bullet_type; // 根据子弹类型改变子弹
};

#endif /* __PLAYER_H__ */
