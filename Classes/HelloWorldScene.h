#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ParallaxNodeExtras.h"

USING_NS_CC;

#define KNUMASTEROIDS 15
#define KNUMLASERS 5

typedef enum {
    KENDREASONWIN,
    KENDREASONLOSE
} EndReason;

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    virtual void update(float dt);
    
    /** Acceleration listeer **/
    void enableAccelerationEvent(bool enabled);
    CC_SYNTHESIZE(cocos2d::EventListenerAcceleration*, _accelerationListener, AccelerationListener);
    void OnAcceleration(cocos2d::Acceleration* acc, cocos2d::Event* event);
    /** Touch listener */
    void enableTouchEvent(bool enabled);
    CC_SYNTHESIZE(EventListenerTouchOneByOne*, _touchListener, TouchListener);
    virtual bool onTouchBegan(Touch *touch, Event *event);
    virtual void onTouchMoved(Touch *touch, Event *event);
    virtual void onTouchEnded(Touch *touch, Event *event);
    virtual void onTouchCancelled(Touch *touch, Event *event);
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
private:
    Size _winSize;
    float _shipPointPerSecY;
    float _shipMaxPointsPerSec;
    double _rollingX;
    
    double _gameOverTime;
    bool _gameOver;
    void endScene(EndReason endReason);
    void restartTapped(Ref* pSender);

    SpriteBatchNode* _batchNode;
    Sprite* _ship;
    ParallaxNodeExtras* _backgroundNode;
    Sprite* _spacedust1;
    Sprite* _spacedust2;
    Sprite* _planetsunrise;
    Sprite* _galaxy;
    Sprite* _spacialanomaly1;
    Sprite* _spacialanomaly2;

    std::vector<Sprite*> _asteroids;
    int _nextAsteroid;
    float _nextAsteroidSpawn;
    
    float randomValueBetween(float low, float high);
    void setInvisible(Node* node);
    float getTimeTick();
    
    std::vector<Sprite*> _shipLasers;
    int _nextShipLaser;
    
    int _lives;
};

#endif // __HELLOWORLD_SCENE_H__
