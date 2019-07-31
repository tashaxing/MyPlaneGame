#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"

class Enemy : public cocos2d::Sprite
{
public:
    virtual bool init();
    
    CREATE_FUNC(Enemy);
};

#endif /* __ENEMY_H__ */
