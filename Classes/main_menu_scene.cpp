#include "main_menu_scene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace CocosDenshion;

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
                            visible_origin.y + visible_size.height * 2 / 3);
    addChild(title);

    // 添加按钮
    Button* start_btn = Button::create("img/game_resume_nor.png", "img/game_resume_pressed.png", "img/game_resume_nor.png");
    start_btn->setPosition(visible_origin.x + visible_size.width / 2,
                           visible_origin.y + visible_size.height / 2);
    addChild(start_btn);
    
    return true;
}
