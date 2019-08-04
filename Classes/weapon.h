#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "cocos2d.h"

enum WeaponType
{
    AMMO,
    BOMB
};

class Weapon : public cocos2d::Sprite
{
public:
    virtual bool init();
    
    CREATE_FUNC(Weapon);
    
    void initWithType(WeaponType weapon_type);
    
    WeaponType m_type;
    
private:
    void move(float tm);
};

#endif /* __WEAPON_H__ */
