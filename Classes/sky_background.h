#ifndef __SKY_BACKGROUND_H__
#define __SKY_BACKGROUND_H__

#include "cocos2d.h"

class SkyBackground : public cocos2d::Node
{
public:
    virtual bool init();
    CREATE_FUNC(SkyBackground);
    void pauseRotate();
    void resumeRotate();
    
private:
    void backgroundRotate(float tm);
    cocos2d::Sprite* m_background1;
    cocos2d::Sprite* m_background2;
};

#endif /* __SKY_BACKGROUND_H__ */
