#include "weapon.h"

USING_NS_CC;

const float kSpeed = 0.7;

const float kFrameUpdateInterval = 0.02;

bool Weapon::init()
{
    if (!Sprite::init())
        return false;
    
    
    return true;
}

void Weapon::initWithType(WeaponType weapon_type)
{
    m_type = weapon_type;
    // 设置纹理，存储类型用于道具生效判断
    if (weapon_type == WeaponType::AMMO)
        setTexture("img/weapon1.png");
    else if (weapon_type == WeaponType::BOMB)
        setTexture("img/weapon2.png");
    
    // 调度道具的移动，固定间隔移动固定单位
    schedule(schedule_selector(Weapon::move), kFrameUpdateInterval);
}

void Weapon::pauseMove()
{
    unschedule(schedule_selector(Weapon::move));
}

void Weapon::resumeMove()
{
    schedule(schedule_selector(Weapon::move), kFrameUpdateInterval);
}

void Weapon::move(float tm)
{
    setPositionY(getPositionY() - kSpeed);
}

