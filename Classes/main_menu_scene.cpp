#include "main_menu_scene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "game_scene.h"

USING_NS_CC;
using namespace CocosDenshion;
using namespace ui; // button namespace

Scene* MainMenuScene::createScene()
{
    return MainMenuScene::create();
}

// on "init" you need to initialize your instance
bool MainMenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visible_size = Director::getInstance()->getVisibleSize();
    auto visible_origin = Director::getInstance()->getVisibleOrigin();

	// 添加背景图
    Sprite* background = Sprite::create("img/background.png");
    background->setContentSize(visible_size);
    background->setPosition(visible_origin.x + visible_size.width / 2,
                            visible_origin.y + visible_size.height / 2);
    addChild(background);
    
    Sprite* title = Sprite::create("img/title.png");
    title->setScale(1.5); // 尺寸适当调整
    title->setPosition(visible_origin.x + visible_size.width / 2,
                            visible_origin.y + visible_size.height / 2 + 40);
    addChild(title);

    // 添加按钮
    Button* start_btn = Button::create("img/game_resume_nor.png", "img/game_resume_pressed.png", "img/game_resume_nor.png"); // 指定好各种状态图的按钮
    start_btn->setScale(1.5); // 尺寸适当调整
    start_btn->setPosition(Vec2(visible_origin.x + visible_size.width / 2,
                                visible_origin.y + visible_size.height / 2));
    start_btn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        // 按钮点击事件
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                // 播放音效
                SimpleAudioEngine::getInstance()->playEffect("sound/button.wav");
                break;
            case ui::Widget::TouchEventType::ENDED:
            {
                // 切换到主游戏场景
                auto game_scene = GameScene::createScene();
                TransitionScene* transition_scene = TransitionFade::create(0.5, game_scene);
                Director::getInstance()->replaceScene(transition_scene);
            }
                break;
            default:
                break;
        }
    });
    addChild(start_btn);
    
    return true;
}
