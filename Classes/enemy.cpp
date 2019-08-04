#include "enemy.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

const int kHpForSmall = 10;
const int kHpForMedium = 30;
const int kHpForBig = 60;

const float kSpeedForSmall = 1.2;
const float kSpeedForMedium = 0.6;
const float kSpeedForBig = 0.3;

bool Enemy::init()
{
    if (!Sprite::init())
        return false;
    
    
    return true;
}

void Enemy::initWithType(EnemyType enemy_type)
{
    m_type = enemy_type;
    // 设置纹理
    if (enemy_type == EnemyType::SMALL)
    {
        setTexture("img/enemy1.png");
        m_hp = kHpForSmall;
        m_speed = kSpeedForSmall;
    }
    else if (enemy_type == EnemyType::MEDIUM)
    {
        setTexture("img/enemy2.png");
        m_hp = kHpForMedium;
        m_speed = kSpeedForMedium;
    }
    else if (enemy_type == EnemyType::BIG)
    {
        setTexture("img/enemy3_n1.png");
        m_hp = kHpForBig;
        m_speed = kSpeedForBig;
        
        // boss有火力动画和飞行音效
        SimpleAudioEngine::getInstance()->playEffect("sound/big_spaceship_flying.wav");
        
        Animation* animation = Animation::create();
        for (int i = 1; i <= 2; i++)
        {
            std::string enemy_frame_name = "img/enemy3_n" + std::to_string(i) + ".png";
            animation->addSpriteFrameWithFile(enemy_frame_name);
        }
        animation->setDelayPerUnit(0.1);
        
        Animate* animate = Animate::create(animation);
        runAction(RepeatForever::create(animate));
    }
    
    // 调度敌机的移动，固定间隔移动固定单位
    schedule(schedule_selector(Enemy::fly), 0.02);
}

void Enemy::fly(float tm)
{
    setPositionY(getPositionY() - m_speed);
}

void Enemy::hit(int reduce_hp)
{
    // 若当前还存活，medium和big敌机有击中特效
    if (m_hp > 0)
    {
        if (m_type == EnemyType::MEDIUM)
        {
            // 设置击中图片后再切换回来
            setTexture("img/enemy2_hit.png");
            runAction(Sequence::create(DelayTime::create(0.3), CallFunc::create([&](){
                setTexture("img/enemy2.png");
            }), NULL));
        }
        else if (m_type == EnemyType::BIG)
        {
            // 设置击中图片后再切换回来
            setTexture("img/enemy3_hit.png");
            runAction(Sequence::create(DelayTime::create(0.3), CallFunc::create([&](){
                setTexture("img/enemy3_n1.png");
            }), NULL));
        }
        
        // 被子弹打击，减血
        m_hp -= reduce_hp;
    }
}

void Enemy::die()
{
    // 播放爆炸动画
    Animation* animation = Animation::create();
    
    if (m_type == EnemyType::SMALL)
    {
        for (int i = 1; i <= 4; i++)
        {
            std::string enemy_frame_name = "img/enemy1_down" + std::to_string(i) + ".png";
            animation->addSpriteFrameWithFile(enemy_frame_name);
        }
    }
    else if (m_type == EnemyType::MEDIUM)
    {
        for (int i = 1; i <= 4; i++)
        {
            std::string enemy_frame_name = "img/enemy2_down" + std::to_string(i) + ".png";
            animation->addSpriteFrameWithFile(enemy_frame_name);
        }
    }
    else if (m_type == EnemyType::BIG)
    {
        for (int i = 1; i <= 6; i++)
        {
            std::string enemy_frame_name = "img/enemy3_down" + std::to_string(i) + ".png";
            animation->addSpriteFrameWithFile(enemy_frame_name);
        }
    }

    animation->setDelayPerUnit(0.1);
    
    Animate* animate = Animate::create(animation);
    runAction(animate);
    
    // TODO: clean up and callback
}

