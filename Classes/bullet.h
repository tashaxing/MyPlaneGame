#ifndef __BULLET_H__
#define __BULLET_H__

#include "cocos2d.h"

class Bullet : public cocos2d::Sprite
{
public:
    virtual bool init();
    
    CREATE_FUNC(Bullet);
    
public:
    void move();
    int m_speed;
};

#endif /* __BULLET_H__ */
