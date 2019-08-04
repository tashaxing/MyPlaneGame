#include "bullet.h"

USING_NS_CC;

const float kLowSpeed = 3.0;
const float kHighSpeed = 5.0;

bool Bullet::init()
{
    if (!Sprite::init())
        return false;
    
    return true;
}

void Bullet::initWithType(BulletType bullet_type)
{
    // 设置纹理和子弹速度
    if (bullet_type == BulletType::BASE)
    {
        setTexture("img/bullet1.png");
        m_speed = kLowSpeed;
    }
    else if (bullet_type == BulletType::POWER)
    {
        setTexture("img/bullet2.png");
        m_speed = kHighSpeed;
    }
    
    // 调度子弹的移动，固定间隔移动固定单位
    schedule(schedule_selector(Bullet::move), 0.02);
}

void Bullet::move(float tm)
{
    // 每帧更新，往上移动，出了边界则销毁
    setPositionY(getPositionY() + m_speed);
}
