#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "player.h"
#include "bullet.h"
#include "weapon.h"
#include "enemy.h"

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene *createScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual void update(float dt);
    
    // 触摸事件
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
    
private:
    void generateBullet();
    void generateWeapon();
    void generateEnemy();
    
    Vector<Bullet*> m_bullets; // 飞行中的子弹
    Vector<Enemy*> m_enemies; // 天空中的道具
    Vector<Weapon*> m_weapons; // 存活的敌机
};

#endif /* __GAME_SCENE_H__ */
