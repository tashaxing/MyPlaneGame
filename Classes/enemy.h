#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"

enum EnemyType
{
    SMALL,  // 小飞机
    MEDIUM, // 中等飞机
    BIG     // 大飞船
};

class Enemy : public cocos2d::Sprite
{
public:
    virtual bool init();
    
    CREATE_FUNC(Enemy);
    void pauseMove();
    void resumeMove();
    
    void initWithType(EnemyType enemy_type);
    void move(float tm);
    void hit(int reduce_hp); // 敌机被子弹打中
    void die(); // 敌机死亡
    
    int m_hp; // 敌机总血量
	EnemyType m_type;
    
private:
    float m_speed;
};

#endif /* __ENEMY_H__ */
