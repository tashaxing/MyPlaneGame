#include "game_scene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

const int kBackgroundZorder = 0;
const int kBattleZorder = 1;
const int kWeaponZorder = 2;

const float kBulletGenerateInterval = 0.2;
const float kEnemyGenerateInterval = 1.0;
const float kWeaponGenerateInterval = 5.0;

const int kSmallPlaneScore = 100;
const int kMediumPlaneScore = 500;
const int kBigPlaneScore = 1000;

const int kAchievementScoreUnit = 5000;

const Point kInitPoint = Point(-1, -1);

Scene *GameScene::createScene()
{
    Scene *game_scene = Scene::create();
    Layer *game_layer = GameScene::create();
    game_scene->addChild(game_layer);
    return game_scene;
}

bool GameScene::init()
{
    if (!Layer::init())
        return false;
    
    Size visible_size = Director::getInstance()->getVisibleSize();
    Point visible_origin = Director::getInstance()->getVisibleOrigin();
    
    // 初始化游戏状态
    m_is_over = false;
	
	// 初始化游戏分数
	m_score = 0;
	//m_score_label

    // 初始化天空背景并循环移动
    m_sky_background = SkyBackground::create();
    addChild(m_sky_background, kBackgroundZorder);
    
    // 播放背景音乐(其实，音乐和音效最好用预先加载)
    SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/game_music.wav", true); // mp3 not supported, use wav
    
    // NOTICE: except sky, other sprites' anchor point are all on the center
    
    // 初始化玩家飞机
    m_player = Player::create();
    addChild(m_player, kBattleZorder);
    
    // 初始化敌机，调度敌机
    schedule(schedule_selector(GameScene::generateEnemy), kEnemyGenerateInterval);
    
    // 初始化道具，调度道具
    schedule(schedule_selector(GameScene::generateWeapon), kWeaponGenerateInterval);
    
    // 定时器触发调度飞机射击子弹
    schedule(schedule_selector(GameScene::generateBullet), kBulletGenerateInterval); // bullet interval could be adjusted
    
    // 添加触摸事件监听
    auto touch_listener = EventListenerTouchOneByOne::create();
    touch_listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    touch_listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    touch_listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touch_listener, this);
    
    m_pretouch_pos = kInitPoint; // 未触摸时的初始点
    
    // 默认渲染更新
    scheduleUpdate();
    
    return true;
}

void GameScene::onEnter()
{
    Layer::onEnter();
    CCLOG("GameScene onEnter");
}

void GameScene::onExit()
{
    Layer::onExit();
    CCLOG("GameScene onExit");
}

void GameScene::getScore(EnemyType enemy_type)
{
	// 根据不同敌机类别得不同分数
	switch (enemy_type)
	{
	case EnemyType::SMALL:
		m_score += kSmallPlaneScore;
		break;
	case EnemyType::MEDIUM:
		m_score += kMediumPlaneScore;
		break;
	case EnemyType::BIG:
		m_score += kBigPlaneScore;
		break;
	default:
		break;
	}

	CCLOG("score = %d", m_score);

	// 如果分数达到一定阶段，播放成就音效
	if (m_score > 0 && m_score % kAchievementScoreUnit == 0)
		SimpleAudioEngine::getInstance()->playEffect("img/achievement.wav");

	// TODO: 刷新UI
}

void GameScene::update(float dt)
{
    // NOTICE: dt is normally 0.0167, so other timer may not be faster than this
    //    CCLOG("update delta: %f", dt);
    
    // 游戏结束后避免出现空指针内存错误
    if (m_is_over)
        return;
    
    Size visible_size = Director::getInstance()->getVisibleSize();
    Point visible_origin = Director::getInstance()->getVisibleOrigin();
    
    // --- 碰撞监测 ---
    // 判断玩家
    for (Enemy* enemy : m_enemies)
    {
        if (enemy->m_hp > 0 && enemy->getBoundingBox().intersectsRect(m_player->getBoundingBox()))
        {
            // 玩家飞机撞了
            m_is_over = true;
            m_player->destroy();

			// 游戏结束，处理后续
            gameOver();
            return;
        }
    }
    
    // 判断敌机被子弹击中
    Vector<Bullet*> hit_bullets;
//    Vector<Enemy*> dead_enemies;
    for (Bullet* bullet : m_bullets)
    {
        for (Enemy* enemy : m_enemies)
        {
            // 注意，某个子弹撞击了后要消失，并且不可再参与碰撞检测
            if (!bullet->m_hit_flag
                && enemy->m_hp > 0
                && bullet->getBoundingBox().intersectsRect(enemy->getBoundingBox()))
            {
                enemy->hit(bullet->m_kill_hp);
                if (enemy->m_hp <= 0)
                {
					// 先更新分数，再移除飞机
					getScore(enemy->m_type);

                    enemy->die(); // 这里面做了对象销毁
                    m_enemies.eraseObject(enemy); // 这里可以先于动画放完就移出管理器
                }
                
                // 移除子弹, 先标记起来放到移除列表
                bullet->m_hit_flag = true;
                hit_bullets.pushBack(bullet);
            }
        }
    }
    // 这里统一移除子弹，避免边判断边移除导致的内存问题
    for (Bullet* bullet : hit_bullets)
    {
        m_bullets.eraseObject(bullet);
        bullet->removeFromParent();
    }
    hit_bullets.clear();
    
    // 判断拾取道具
    for (Weapon* weapon : m_weapons)
    {
        if (weapon->getBoundingBox().intersectsRect(m_player->getBoundingBox()))
        {
            if (weapon->m_type == WeaponType::AMMO)
                m_player->fetchWeapon(WeaponType::AMMO);
            else if (weapon->m_type == WeaponType::BOMB)
            {
                // 炸掉所有敌机
                SimpleAudioEngine::getInstance()->playEffect("sound/use_bomb.wav");
                for (Enemy* enemy : m_enemies)
                {
					// 先更新分数，再移除飞机
					getScore(enemy->m_type);

                    enemy->m_hp = 0; // 先把血量置空
                    enemy->die(); // 这里面做了对象销毁
					m_enemies.eraseObject(enemy); // 这里可以先于动画放完就移出管理器
                }
            }
            
            // 道具本身也要消失
            m_weapons.eraseObject(weapon);
            weapon->removeFromParent();
        }
    }
    
    // --- 元素管理 ---
    for (Bullet* bullet : m_bullets)
    {
        // 若飞行的子弹出了屏幕，则移除
        if (bullet->getPositionY() > visible_origin.y +
                                    visible_size.height + bullet->getContentSize().height / 2)
        {
            m_bullets.eraseObject(bullet); // erase from vector should before itself cleanup
            bullet->removeFromParent();
        }
    }
    CCLOG("current bullets count: %d", m_bullets.size());

    for (Enemy* enemy : m_enemies)
    {
        // 若存活敌机出了平面，则移除
        if (enemy->getPositionY() < visible_origin.y - enemy->getContentSize().height / 2)
        {
            m_enemies.eraseObject(enemy);
            enemy->removeFromParent();
        }
    }
    CCLOG("alive enemy count: %d", m_enemies.size());

    for (Weapon* weapon : m_weapons)
    {
        // 若未拾取的道具出了屏幕，则移除
        if (weapon->getPositionY() < visible_origin.y - weapon->getContentSize().height / 2)
        {
            m_weapons.eraseObject(weapon);
            weapon->removeFromParent();
        }
    }
    CCLOG("unfetched weapon count: %d", m_weapons.size());
}

bool GameScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (m_is_over)
        return true;
    
    CCLOG("onTouchBegan");
    m_pretouch_pos = touch->getLocation();
    m_preplayer_pos = m_player->getPosition();
    
    return true;
}

void GameScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (m_is_over)
        return;
    CCLOG("onTouchMoved");
    Point current_touch_pos = touch->getLocation();
    Vec2 move_delta = current_touch_pos - m_pretouch_pos;
    m_player->setPosition(m_preplayer_pos + move_delta);
}

void GameScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (m_is_over)
        return;
    
    CCLOG("onTouchEnded");
    m_pretouch_pos = kInitPoint;
}

void GameScene::generateBullet(float interval)
{
    if (m_is_over)
        return;
    
    // 由定时器触发产生子弹，也可以由某个按键触发
    // 根据玩家子弹状态产生不同的子弹，加到场景和管理器
    if (m_player->m_bullet_type == BulletType::BASE)
    {
        Bullet* bullet = m_player->shootSingle();
        addChild(bullet, kBattleZorder);
        m_bullets.pushBack(bullet);
    }
    else if (m_player->m_bullet_type == BulletType::POWER)
    {
        Vector<Bullet*> double_bullets = m_player->shootDouble();
        for (Bullet* bullet : double_bullets)
        {
            addChild(bullet, kBattleZorder);
            m_bullets.pushBack(bullet);
        }
    }
}

void GameScene::generateEnemy(float interval)
{
    if (m_is_over)
        return;
    
    Size visible_size = Director::getInstance()->getVisibleSize();
    Point visible_origin = Director::getInstance()->getVisibleOrigin();
    
    // 定时器触发产生道具，根据概率生成不同类型
    // 加载到场景和管理器
    EnemyType enemy_type;
    float random_factor = CCRANDOM_0_1();
    if (random_factor >= 0.9)
        enemy_type = EnemyType::BIG;
    else if (random_factor >= 0.6 && random_factor < 0.9)
        enemy_type = EnemyType::MEDIUM;
    else
        enemy_type = EnemyType::SMALL;

    Enemy* enemy = Enemy::create();
    enemy->initWithType(enemy_type);
    
    // 生成随机位置，但要保证敌机显示完整，所以左右各留出半个敌机身位
    float random_x = enemy->getContentSize().width / 2 +
                    (visible_size.width - enemy->getContentSize().width) * CCRANDOM_0_1();
    
    enemy->setPosition(visible_origin.x + random_x,
                       visible_origin.y + visible_size.height + enemy->getContentSize().height / 2);
    addChild(enemy, kBattleZorder);
    m_enemies.pushBack(enemy);
}

void GameScene::generateWeapon(float interval)
{
    if (m_is_over)
        return;
    
    Size visible_size = Director::getInstance()->getVisibleSize();
    Point visible_origin = Director::getInstance()->getVisibleOrigin();
    
    // 定时器触发产生敌机，跟据概率生成不同类型
    WeaponType weapon_type;
    float random_factor = CCRANDOM_0_1();
    if (random_factor >= 0.7)
        weapon_type = WeaponType::BOMB;
    else
        weapon_type = WeaponType::AMMO;
    
    Weapon* weapon = Weapon::create();
    weapon->initWithType(weapon_type);
    
    // 随机位置
    float random_x = weapon->getContentSize().width / 2 +
                    (visible_size.width - weapon->getContentSize().width) * CCRANDOM_0_1();
    
    weapon->setPosition(visible_origin.x + random_x,
                       visible_origin.y + visible_size.height + weapon->getContentSize().height / 2);
    
    addChild(weapon, kWeaponZorder);
    m_weapons.pushBack(weapon);
}

void GameScene::gameOver()
{
    CCLOG("player hit, game over");
    
    // 暂停所有的调度器
    unschedule(schedule_selector(GameScene::generateEnemy));
    unschedule(schedule_selector(GameScene::generateWeapon));
    unschedule(schedule_selector(GameScene::generateBullet));
    
    // TODO: 切换到菜单场景
    
}
