#include "game_scene.h"

USING_NS_CC;

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
    
    // TODO:
    
    return true;
}

void GameScene::onEnter()
{
    Layer::onEnter();
}

void GameScene::onExit()
{
    Layer::onExit();
}

void GameScene::update(float dt)
{
    
}

bool GameScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    return true;
}

void GameScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    
}

void GameScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    
}
