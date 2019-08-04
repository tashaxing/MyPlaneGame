#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"

enum EnemyType
{
    SMALL,
    MEDIUM,
    BIG
};

class Enemy : public cocos2d::Sprite
{
public:
    virtual bool init();
    
    CREATE_FUNC(Enemy);
    
    void initWithType(EnemyType enemy_type);
    void die();
    
    int m_speed;
};

#endif /* __ENEMY_H__ */
