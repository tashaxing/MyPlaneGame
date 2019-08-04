#include "enemy.h"

USING_NS_CC;

const float kSpeedForSmall = 5.0;
const float kSpeedForMedium = 3.0;
const float kSpeedForBig = 1.0;

bool Enemy::init()
{
    if (!Sprite::init())
        return false;
    
    
    return true;
}

void Enemy::initWithType(EnemyType enemy_type)
{
    // 设置纹理和动画
    if (enemy_type == EnemyType::SMALL)
    {
        setTexture("img/enemy1.png");
        m_speed = kSpeedForSmall;
    }
    else if (enemy_type == EnemyType::MEDIUM)
    {
        setTexture("img/enemy2.png");
        m_speed = kSpeedForMedium;
    }
    else if (enemy_type == EnemyType::BIG)
    {
        setTexture("img/enemy3_n1.png");
        m_speed = kSpeedForBig;
        
        
    }
}

