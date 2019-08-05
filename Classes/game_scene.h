#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "sky_background.h"
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
	void getScore(EnemyType enemy_type);
	int m_score; // 游戏分数
    void gameOver(); // 游戏结束，处理后续
    bool m_is_over; // 游戏结束标志
    
    void generateBullet(float interval); // 固定时间间隔发射子弹
    void generateWeapon(float interval); // 固定时间间隔产生道具
    void generateEnemy(float interval); // 固定时间间隔出现敌机
    
    SkyBackground* m_sky_background; // 天空背景
    Player* m_player; // 玩家飞机

    cocos2d::Vector<Bullet*> m_bullets; // 飞行中的子弹管理器
    cocos2d::Vector<Enemy*> m_enemies; // 存活的敌机管理器
    cocos2d::Vector<Weapon*> m_weapons;// 天空中的道具管理器
    
    cocos2d::Point m_pretouch_pos; // 保存触摸的初始点
    cocos2d::Point m_preplayer_pos; // 保存触摸时候的玩家飞机初始点
};

#endif /* __GAME_SCENE_H__ */
