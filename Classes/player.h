#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"

class Player : public cocos2d::Sprite
{
public:
    virtual bool init();
    
    CREATE_FUNC(Player);
};

#endif /* __PLAYER_H__ */
