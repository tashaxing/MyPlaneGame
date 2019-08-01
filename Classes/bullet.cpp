#include "bullet.h"

USING_NS_CC;

bool Bullet::init()
{
    if (!Sprite::init())
        return false;
    
    // 加载纹理
    
    // 设置初始速度
    m_speed = 10;
    
    return true;
}

void Bullet::move()
{
    // 每帧更新，往上移动，出了边界则销毁
    setPositionY(getPositionY() + m_speed);
    if (getPositionY() > Director::getInstance()->getVisibleSize().height)
        removeFromParent();
}
