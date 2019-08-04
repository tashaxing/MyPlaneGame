#ifndef __BULLET_H__
#define __BULLET_H__

#include "cocos2d.h"

enum BulletType
{
    BASE, // 单子弹
    POWER // 双子单
};

class Bullet : public cocos2d::Sprite
{
public:
    virtual bool init();
    void initWithType(BulletType bullet_type); // 根据子弹类型设置纹理
    CREATE_FUNC(Bullet);
    
public:
    int m_kill_hp; // 子弹的杀伤力
    
private:
    void move(float tm);
    float m_speed;
};

#endif /* __BULLET_H__ */
