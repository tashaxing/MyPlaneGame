#include "player.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

const float kPowerTime = 8.0;

bool Player::init()
{
    if (!Sprite::init())
        return false;
    
    Size visible_size = Director::getInstance()->getVisibleSize();
    Point visible_origin = Director::getInstance()->getVisibleOrigin();
    
    // 初始化玩家飞机
    setTexture("img/hero1.png");
    setPosition(visible_origin.x + visible_size.width / 2,
                visible_origin.y + getContentSize().height);
    
    // 玩家飞机喷气动画
    // FIXME: actually should use preload animation cache
    Animation* animation = Animation::create();
    for (int i = 1; i <= 2; i++)
    {
        std::string hero_frame_name = "img/hero" + std::to_string(i) + ".png";
        animation->addSpriteFrameWithFile(hero_frame_name);
    }
    animation->setDelayPerUnit(0.1);
    
    Animate* animate = Animate::create(animation);
    runAction(RepeatForever::create(animate));
    
    // 初始化子弹类型
    m_bullet_type = BulletType::BASE;
    
    return true;
}

Bullet* Player::shootSingle()
{
    // 发射子弹音效
    SimpleAudioEngine::getInstance()->playEffect("sound/bullet.wav");
    
    // 子弹从飞机头部打出来
    Bullet* bullet = Bullet::create();
    bullet->initWithType(BulletType::BASE);
    bullet->setPosition(getPositionX(), getPositionY() + getContentSize().height / 2);
    return bullet;
}

Vector<Bullet*> Player::shootDouble()
{
    // 发射子弹音效
    SimpleAudioEngine::getInstance()->playEffect("sound/bullet.wav");
    
    Vector<Bullet*> double_bullets;
    // 子弹都从飞机头部打出来,左右两个子弹分布在机头
    Bullet* bullet_left = Bullet::create();
    bullet_left->initWithType(BulletType::POWER);
    bullet_left->setPosition(getPositionX() - getContentSize().width / 4, getPositionY() + getContentSize().height / 2);
    double_bullets.pushBack(bullet_left);
    
    Bullet* bullet_right = Bullet::create();
    bullet_right->initWithType(BulletType::POWER);
    bullet_right->setPosition(getPositionX() + getContentSize().width / 4, getPositionY() + getContentSize().height / 2);
    double_bullets.pushBack(bullet_right);
    
    return double_bullets;
}

void Player::fetchWeapon(WeaponType weapon_type)
{
    // 如果道具是子弹,且之前是单子弹，切换子弹,并持续一段时间
    if (m_bullet_type == BulletType::BASE && weapon_type == WeaponType::AMMO)
    {
        m_bullet_type = BulletType::POWER;
        // 播放音效
        SimpleAudioEngine::getInstance()->playEffect("sound/get_double_laser.wav");
        
        // 子弹有效时间，用lambda做单次定时器回调
        scheduleOnce([&](float delay){
            m_bullet_type = BulletType::BASE;
            SimpleAudioEngine::getInstance()->playEffect("sound/out_porp.wav");
        }, kPowerTime, "power_status");
    }
    else if (weapon_type == WeaponType::BOMB)
    {
        // 播放音效
        SimpleAudioEngine::getInstance()->playEffect("sound/get_bomb");
        
        // TODO: 储存炸弹
    }
}

void Player::destroy()
{
    // 被摧毁音效
    SimpleAudioEngine::getInstance()->playEffect("sound/game_over.wav");
    
    // 被摧毁动画
    Animation* animation = Animation::create();
    
    for (int i = 1; i <= 4; i++)
    {
        std::string player_frame_name = "img/hero_blowup_n" + std::to_string(i) + ".png";
        animation->addSpriteFrameWithFile(player_frame_name);
    }
//    animation->setRestoreOriginalFrame(false); // 默认播放动画结束后不恢复到第一帧
    animation->setDelayPerUnit(0.2);
    
    Animate* animate = Animate::create(animation);
	runAction(Sequence::create(animate, CallFunc::create([&]() {
		// 动画放完回调里面清除该对象
		removeFromParent();
		}), NULL));
}

