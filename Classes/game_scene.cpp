#include "game_scene.h"
#include "main_menu_scene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace CocosDenshion;
using namespace ui;

const int kBackgroundZorder = 0;
const int kBattleZorder = 1;
const int kWeaponZorder = 2;
const int kGameoverZorder = 3;

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
    m_is_pause = false;

    // 初始化天空背景并循环移动
    m_sky_background = SkyBackground::create();
    addChild(m_sky_background, kBackgroundZorder);
    
    // 初始化游戏分数
    m_score = 0;
    m_score_label = Label::createWithTTF("score: 0", "fonts/Marker Felt.ttf", 14);
    m_score_label->setColor(Color3B::BLACK);
    m_score_label->setPosition(visible_origin.x + 35, visible_origin.y + visible_size.height - 20);
    addChild(m_score_label, kGameoverZorder); // 分数浮层在最上方
    
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
    
    // 添加右上角暂停和开始切换
    Button* pause_btn = Button::create("img/game_pause_nor.png", "img/game_pause_pressed.png");
    pause_btn->setPosition(Vec2(visible_origin.x + visible_size.width - 20,
                                visible_origin.y + visible_size.height - 20));
    pause_btn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        // 按钮点击事件
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                // 播放音效
                SimpleAudioEngine::getInstance()->playEffect("sound/button.wav");
                break;
            case ui::Widget::TouchEventType::ENDED:
                // 暂停游戏
                gamePause();
                break;
            default:
                break;
        }
    });
//    pause_btn->setEnabled(true);
//    pause_btn->setVisible(true);
    addChild(pause_btn, kGameoverZorder, "pause_btn");
    
    Button* resume_btn = Button::create("img/game_resume_nor.png", "img/game_resume_pressed.png");
    resume_btn->setPosition(Vec2(visible_origin.x + visible_size.width - 20,
                                visible_origin.y + visible_size.height - 20)); // 与暂停按钮位置重叠
    resume_btn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        // 按钮点击事件
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                // 播放音效
                SimpleAudioEngine::getInstance()->playEffect("sound/button.wav");
                break;
            case ui::Widget::TouchEventType::ENDED:
                // 恢复游戏
                gameResume();
                break;
            default:
                break;
        }
    });
    resume_btn->setEnabled(false); // 初始不可用并隐藏
    resume_btn->setVisible(false);
    addChild(resume_btn, kGameoverZorder, "resume_btn");
    
    // 添加触摸事件监听
    m_touch_listener = EventListenerTouchOneByOne::create();
    m_touch_listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    m_touch_listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    m_touch_listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_touch_listener, this);
//    m_touch_listener->setEnabled(true);
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
    static int phase = 0;
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

	// 如果分数达到一定阶段，播放成就音效
    int new_phase = m_score / kAchievementScoreUnit;
	if (new_phase > phase)
    {
        SimpleAudioEngine::getInstance()->playEffect("sound/achievement.wav");
        phase = new_phase;
    }

	// 刷新UI
    m_score_label->setString(__String::createWithFormat("score: %d", m_score)->_string);
//    m_score_label->setString(StringUtils::format("score: %d", m_score)); // the same
}

void GameScene::update(float dt)
{
    // NOTICE: dt is normally 0.0167, so other timer may not be faster than this
    //    CCLOG("update delta: %f", dt);
    
    // 游戏结束后避免出现空指针内存错误
    if (m_is_over)
        return;
    
    // 游戏暂停和恢复
    // FIXME: every update will do this check, need to optimize
    if (m_is_pause)
    {
        Button* pause_btn = (Button*)getChildByName("pause_btn");
        pause_btn->setEnabled(false);
        pause_btn->setVisible(false);
        
        Button* resume_btn = (Button*)getChildByName("resume_btn");
        resume_btn->setEnabled(true);
        resume_btn->setVisible(true);
        
        m_touch_listener->setEnabled(false); // 暂停时不可操作
    }
    else
    {
        Button* pause_btn = (Button*)getChildByName("pause_btn");
        pause_btn->setEnabled(true);
        pause_btn->setVisible(true);
        
        Button* resume_btn = (Button*)getChildByName("resume_btn");
        resume_btn->setEnabled(false);
        resume_btn->setVisible(false);
        
        m_touch_listener->setEnabled(true);
    }
    
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

			// 游戏结束，处理后续（延迟等到玩家飞机t摧毁动画结束）
            scheduleOnce([&](float delay){
                gameOver();
            }, 2.0, "game over");

            return;
        }
    }
    
    // 判断敌机被子弹击中
    Vector<Bullet*> hit_bullets;
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
                CCLOG("before bomb enemy number = %d", m_enemies.size());
                for (Enemy* enemy : m_enemies)
                {
					// 先更新分数，再移除飞机
					getScore(enemy->m_type);

                    enemy->m_hp = 0; // 先把血量置空
                    enemy->die(); // 这里面做了对象销毁
                }
                m_enemies.clear(); // 一定要放在这里统一清空
                CCLOG("after bomb enemy number = %d", m_enemies.size());
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
    
    Size visible_size = Director::getInstance()->getVisibleSize();
    Point visible_origin = Director::getInstance()->getVisibleOrigin();
    
    // 停止背景音乐
    // FIXME: stop background music may cause crash
    if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
        SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    
    // 停止所有的调度器
    unschedule(schedule_selector(GameScene::generateEnemy));
    unschedule(schedule_selector(GameScene::generateWeapon));
    unschedule(schedule_selector(GameScene::generateBullet));
    
    // 添加结束画面
    Sprite* game_over_background = Sprite::create("img/gameover.png");
    game_over_background->setContentSize(visible_size);
    game_over_background->setAnchorPoint(Point::ZERO);
    game_over_background->setPosition(visible_origin);
    addChild(game_over_background, kGameoverZorder);
    
    Label* final_score_label = Label::createWithTTF(std::to_string(m_score), "fonts/Marker Felt.ttf", 14);
    final_score_label->setColor(Color3B::BLACK);
    final_score_label->setPosition(visible_origin.x + visible_size.width / 2,
                               visible_origin.y + visible_size.height / 2);
    addChild(final_score_label, kGameoverZorder);
    
    // 重新开始按钮
    Button* restart_btn = Button::create("img/restart.png"); // 只添加一张背景图按钮
    restart_btn->setScale(1.5);
    restart_btn->setPosition(Vec2(visible_origin.x + visible_size.width / 2,
                                  visible_origin.y + visible_size.height / 2 - 30));
    restart_btn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        // 按钮点击事件
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                // 播放音效
                SimpleAudioEngine::getInstance()->playEffect("sound/button.wav");
                break;
            case ui::Widget::TouchEventType::ENDED:
            {
                // 重载主游戏场景
                auto game_scene = GameScene::createScene();
                TransitionScene* transition_scene = TransitionFade::create(0.5, game_scene);
                Director::getInstance()->replaceScene(transition_scene);
            }
                break;
            default:
                break;
        }
    });
    addChild(restart_btn, kGameoverZorder);
    
    // 结束游戏按钮
    Button* quit_btn = Button::create("img/quit.png"); // 只添加一张背景图按钮
    quit_btn->setScale(1.5);
    quit_btn->setPosition(Vec2(visible_origin.x + visible_size.width / 2,
                                  visible_origin.y + visible_size.height / 2 - 60));
    quit_btn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        // 按钮点击事件
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                // 播放音效
                SimpleAudioEngine::getInstance()->playEffect("sound/button.wav");
                break;
            case ui::Widget::TouchEventType::ENDED:
            {
                // 返回到菜单场景
                auto main_menu_scene = MainMenuScene::createScene();
                TransitionScene* transition_scene = TransitionFade::create(0.5, main_menu_scene);
                Director::getInstance()->replaceScene(main_menu_scene);
            }
                break;
            default:
                break;
        }
    });
    addChild(quit_btn, kGameoverZorder);
}

void GameScene::gamePause()
{
    // 暂停背景音乐
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    
    // 暂停游戏标志
    m_is_pause = true;
    
    // 暂停所有调度器
    unschedule(schedule_selector(GameScene::generateEnemy));
    unschedule(schedule_selector(GameScene::generateWeapon));
    unschedule(schedule_selector(GameScene::generateBullet));
    
    // 暂停所有的元素移动
    m_sky_background->pauseRotate();
    for (Enemy* enemy : m_enemies)
        enemy->pauseMove();
    for (Bullet* bullet : m_bullets)
        bullet->pauseMove();
    for (Weapon* weapon : m_weapons)
        weapon->pauseMove();
}

void GameScene::gameResume()
{
    // 恢复背景音乐
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    
    // 恢复游戏标志
    m_is_pause = false;
    
    // 恢复所有调度器
    schedule(schedule_selector(GameScene::generateEnemy), kEnemyGenerateInterval);
    schedule(schedule_selector(GameScene::generateWeapon), kWeaponGenerateInterval);
    schedule(schedule_selector(GameScene::generateBullet), kBulletGenerateInterval);
    
    // 恢复所有的元素移动
    m_sky_background->resumeRotate();
    for (Enemy* enemy : m_enemies)
        enemy->resumeMove();
    for (Bullet* bullet : m_bullets)
        bullet->resumeMove();
    for (Weapon* weapon : m_weapons)
        weapon->resumeMove();
}
