#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "cocos2d.h"

class Weapon : public cocos2d::Sprite
{
public:
    virtual bool init();
    
    CREATE_FUNC(Weapon);
};

#endif /* __WEAPON_H__ */
