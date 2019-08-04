#ifndef __BULLET_H__
#define __BULLET_H__

#include "cocos2d.h"

enum BulletType
{
    BASE,
    POWER
};

class Bullet : public cocos2d::Sprite
{
public:
    virtual bool init();
    void initWithType(BulletType bullet_type); // 根据子弹类型设置纹理
    CREATE_FUNC(Bullet);
    
public:
    void move(float tm);
    int m_speed;
};

#endif /* __BULLET_H__ */
