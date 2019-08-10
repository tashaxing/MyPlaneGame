#include "sky_background.h"

USING_NS_CC;

const float kFrameUpdateInterval = 0.02;

bool SkyBackground::init()
{
    if (!Node::init())
        return false;
    
    Size visible_size = Director::getInstance()->getVisibleSize();
    Point visible_origin = Director::getInstance()->getVisibleOrigin();
    
    // 增加两张背景图，循环交替，构造连续滚动效果
    m_background1 = Sprite::create("img/background.png");
    m_background1->setContentSize(visible_size); // 与屏幕适应
    m_background1->setAnchorPoint(Point::ZERO); // 锚点设为左下坐标点，方便计算
    m_background1->setPosition(visible_origin); // 一定要设置初始位置
    addChild(m_background1);
    m_background2 = Sprite::create("img/background.png");
    m_background2->setContentSize(visible_size);
    m_background2->setAnchorPoint(Point::ZERO);
    m_background2->setPosition(visible_origin.x, visible_origin.y + visible_size.height);
    addChild(m_background2);
    
    // 调度两张图的移动
    schedule(schedule_selector(SkyBackground::backgroundRotate), kFrameUpdateInterval); // delta time smaller, move more smooth
    
    return true;
}

void SkyBackground::pauseRotate()
{
    unschedule(schedule_selector(SkyBackground::backgroundRotate));
}

void SkyBackground::resumeRotate()
{
    schedule(schedule_selector(SkyBackground::backgroundRotate), kFrameUpdateInterval);
}

void SkyBackground::backgroundRotate(float tm)
{
    Size visible_size = Director::getInstance()->getVisibleSize();
    Point visible_origin = Director::getInstance()->getVisibleOrigin();
    
    // 背景2在背景1上方无缝衔接，当背景2到达底部时候，将背景1重新归位
    m_background1->setPositionY(m_background1->getPositionY() - 0.3); // rotate speed
    m_background2->setPositionY(m_background1->getPositionY() + visible_size.height);
    if (m_background2->getPositionY() <= visible_origin.y)
        m_background1->setPositionY(visible_origin.y);
}
