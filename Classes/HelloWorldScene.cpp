#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    auto director = Director::getInstance();
    director->getOpenGLView()->setDesignResolutionSize(480, 320, ResolutionPolicy::SHOW_ALL);
    Size visibleSize = director->getVisibleSize();
    Vec2 origin = director->getVisibleOrigin();
    _winSize = director->getWinSize();
    _shipMaxPointsPerSec = _winSize.height * 0.5f;

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    _batchNode = SpriteBatchNode::create("Sprites.png");
    this->addChild(_batchNode);
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Sprites.plist");
    
    _ship = Sprite::createWithSpriteFrameName("SpaceFlier_sm_1.png");
    _ship->setPosition(Vec2(_winSize.width * 0.1f, _winSize.height/2));
    _batchNode->addChild(_ship, 1);
    
    _backgroundNode = ParallaxNodeExtras::node();
    this->addChild(_backgroundNode, -1);
    
    _spacedust1 = Sprite::create("bg_front_spacedust.png");
    _spacedust2 = Sprite::create("bg_front_spacedust.png");
    _planetsunrise = Sprite::create("bg_planetsunrise.png");
    _galaxy = Sprite::create("bg_galaxy.png");
    _spacialanomaly1 = Sprite::create("bg_spacialanomaly.png");
    _spacialanomaly2 = Sprite::create("bg_spacialanomaly2.png");
    Vec2 dustSpeed = Vec2(0.1f, 0.1f);
    Vec2 bgSpeed = Vec2(0.05f, 0.05f);
    
    _backgroundNode->addChild(_spacedust1, 0, dustSpeed, Vec2(0, _winSize.height/2));
    _backgroundNode->addChild(_spacedust2, 0, dustSpeed, Vec2(_spacedust1->getContentSize().width, _winSize.height/2));
    _backgroundNode->addChild(_galaxy, -1, bgSpeed, Vec2(0, _winSize.height/2));
    _backgroundNode->addChild(_planetsunrise, -1, bgSpeed, Vec2(600, _winSize.height * 0));
    _backgroundNode->addChild(_spacialanomaly1, -1, bgSpeed, Vec2(900, _winSize.height * 0.3));
    _backgroundNode->addChild(_spacialanomaly2, -1, bgSpeed, Vec2(1500, _winSize.height * 0.9));
    
    this->addChild(ParticleSystemQuad::create("Stars1.plist"));
    this->addChild(ParticleSystemQuad::create("Stars2.plist"));
    this->addChild(ParticleSystemQuad::create("Stars3.plist"));
    this->scheduleUpdate();
    
    
    for (int i = 0; i < KNUMASTEROIDS; i++) {
        auto asteroid = Sprite::createWithSpriteFrameName("asteroid.png");
        asteroid->setVisible(false);
        _batchNode->addChild(asteroid);
        _asteroids.push_back(asteroid);
    }
    
    for (int i = 0; i < KNUMLASERS; i++) {
        auto shiplaser = Sprite::createWithSpriteFrameName("laserbeam_blue.png");
        shiplaser->setVisible(false);
        _batchNode->addChild(shiplaser);
        _shipLasers.push_back(shiplaser);
    }
    this->enableTouchEvent(true);
    this->enableAccelerationEvent(true);
    _lives = 3;
    double curTime = getTimeTick();
    _gameOverTime = curTime + 30000;
    
    return true;
}

#pragma mark -
#pragma mark touch events
void HelloWorld::enableTouchEvent(bool enabled)
{
    if (this->_touchListener != nullptr) {
        Director::getInstance()->getEventDispatcher()->removeEventListener(this->_touchListener);
        this->_touchListener = nullptr;
    }
    if (enabled) {
        this->_touchListener = EventListenerTouchOneByOne::create();
        _touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
        _touchListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
        _touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
        _touchListener->onTouchCancelled = CC_CALLBACK_2(HelloWorld::onTouchCancelled, this);
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_touchListener, this);
    }
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *event)
{
    auto shipLaser = _shipLasers.at(_nextShipLaser);
    if (_nextShipLaser >= _shipLasers.size()) {
        _nextShipLaser = 0;
    }
    shipLaser->setPosition(_ship->getPosition() + Vec2(shipLaser->getContentSize().width/2, 0));
    shipLaser->setVisible(true);
    shipLaser->stopAllActions();
    shipLaser->runAction(Sequence::create(MoveBy::create(0.5, Vec2(_winSize.width, 0)), CallFuncN::create(CC_CALLBACK_1(HelloWorld::setInvisible, this)), NULL));
    return true;
}

void HelloWorld::onTouchMoved(Touch *touch, Event *event)
{
}

void HelloWorld::onTouchEnded(Touch *touch, Event *event)
{
}

void HelloWorld::onTouchCancelled(Touch *touch, Event *event)
{
}

#pragma mark -
#pragma mark Acceleration
void HelloWorld::enableAccelerationEvent(bool enabled)
{
    if (this->_accelerationListener != nullptr) {
        Director::getInstance()->getEventDispatcher()->removeEventListener(this->_accelerationListener);
        this->_accelerationListener = nullptr;
        Device::setAccelerometerEnabled(false);
    }
    Device::setAccelerometerEnabled(enabled);
    if (enabled) {
        Device::setAccelerometerInterval(0.1);
        this->_accelerationListener = EventListenerAcceleration::create(CC_CALLBACK_2(HelloWorld::OnAcceleration, this));
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(this->_accelerationListener, this);
    }
}

void HelloWorld::OnAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event)
{
#define KFILLTERINGFACTOR 0.1
#define KRESTACCELX -0.6
#define KMAXDIFFX 0.2

    acc->x = acc->y;
    log("OnAcceleration: %f", acc->x);
    _rollingX = (acc->x * KFILLTERINGFACTOR) + (0.0 * (1.0 - KFILLTERINGFACTOR));
    log("rollingX: %f", _rollingX);
    float accelX = acc->x - _rollingX;
    float accelDiff = accelX - KRESTACCELX;
    float accelFraction = accelDiff / KMAXDIFFX;
    _shipPointPerSecY = _shipMaxPointsPerSec * accelFraction;
    log("PointPerSecY: %f", _shipPointPerSecY);
}

#pragma mark -

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::update(float dt)
{
    Vec2 backgroundScrollVert = Vec2(-1000, 0);
    _backgroundNode->setPosition(_backgroundNode->getPosition() + (backgroundScrollVert * dt));
    
    std::vector<Sprite*> spaceDusts;
    spaceDusts.push_back(_spacedust1);
    spaceDusts.push_back(_spacedust2);
    for (std::vector<Sprite*>::iterator it = spaceDusts.begin(); it != spaceDusts.end(); it++) {
        auto spaceDust = dynamic_cast<Sprite*>(*it);
        float posX = _backgroundNode->convertToWorldSpace(spaceDust->getPosition()).x;
        float sizeX = spaceDust->getContentSize().width;
        if (posX < (-sizeX / 2)) {
            _backgroundNode->incrementOffset(Vec2(spaceDust->getContentSize().width * 2, 0), spaceDust);
        }
    }
    spaceDusts.clear();
    
    std::vector<Sprite*> backGrounds;
    backGrounds.push_back(_galaxy);
    backGrounds.push_back(_planetsunrise);
    backGrounds.push_back(_spacialanomaly1);
    backGrounds.push_back(_spacialanomaly2);
    for (std::vector<Sprite*>::iterator it = backGrounds.begin(); it != backGrounds.end(); it++) {
        auto backGround = dynamic_cast<Sprite*>(*it);
        float posX = _backgroundNode->convertToWorldSpace(backGround->getPosition()).x;
        float sizeX = backGround->getContentSize().width;
        if (posX < -sizeX) {
            _backgroundNode->incrementOffset(Vec2(2000, 0), backGround);
        }
    }
    
    float maxY = _winSize.height - _ship->getContentSize().height / 2;
    float minY = _ship->getContentSize().height/2;
    
    float diff = (_shipPointPerSecY * dt);
    log("update y Diff: %f", diff);
    float newY = _ship->getPositionY() + diff;
    newY = MIN(MAX(newY, minY), maxY);
    _ship->setPosition(Vec2(_ship->getPositionX(), newY));
    
//    return;
    
    float curTimeMillis = getTimeTick();
    if (curTimeMillis > _nextAsteroidSpawn) {
        float randMillisecs = randomValueBetween(0.20, 1.0) * 1000;
        _nextAsteroidSpawn = randMillisecs + curTimeMillis;
        
        float randY = randomValueBetween(0.0, _winSize.height);
        float randDuration = randomValueBetween(2.0, 10.0);
        
        auto asteroid = _asteroids.at(_nextAsteroid);
        _nextAsteroid++;
        if (_nextAsteroid >= _asteroids.size()) {
            _nextAsteroid = 0;
        }
        asteroid->stopAllActions();
        asteroid->setPosition(Vec2(_winSize.width + asteroid->getContentSize().width/2, randY));
        asteroid->setVisible(true);
        asteroid->runAction(Sequence::create(MoveBy::create(randDuration, Vec2(-_winSize.width - asteroid->getContentSize().width, 0)), CallFuncN::create(CC_CALLBACK_1(HelloWorld::setInvisible, this)),NULL));
    }
    
    for (std::vector<Sprite*>::iterator ait = _asteroids.begin(); ait != _asteroids.end(); ait++) {
        auto asteroid = dynamic_cast<Sprite*>(*ait);
        if (! asteroid->isVisible()) {
            continue;
        }
        for (std::vector<Sprite*>::iterator lit = _shipLasers.begin(); lit != _shipLasers.end(); lit++) {
            auto shipLaser = dynamic_cast<Sprite*>(*lit);
            if (! shipLaser->isVisible()) {
                continue;
            }
            if (shipLaser->getBoundingBox().intersectsRect(asteroid->getBoundingBox())) {
                shipLaser->setVisible(false);
                asteroid->setVisible(false);
                continue;
            }
        }
        if (_ship->getBoundingBox().intersectsRect(asteroid->getBoundingBox())) {
            asteroid->setVisible(false);
            _ship->runAction(Sequence::create(Blink::create(1.0, 9), CallFunc::create([&](){_ship->setVisible(true);}), NULL));
            _lives--;
        }
    }
    if (_lives <= 0) {
        _ship->stopAllActions();
        _ship->setVisible(false);
        this->endScene(KENDREASONLOSE);
    } else if (curTimeMillis >= _gameOverTime) {
        this->endScene(KENDREASONWIN);
    }
}

#pragma mark -
#pragma mark Utils
void HelloWorld::restartTapped(Ref* pSender)
{
    Director::getInstance()->replaceScene(TransitionZoomFlipX::create(0.5, HelloWorld::createScene()));
}

void HelloWorld::endScene(EndReason endReason)
{
    if (_gameOver) {
        return;
    }
    _gameOver = true;
    std::string message = "You Win";
    if (endReason == KENDREASONLOSE) {
        message = "You Lose";
    }
    auto label = LabelBMFont::create(message, "Arial.fnt");
    label->setScale(0.1);
    label->setPosition(Vec2(_winSize.width/2, _winSize.height * 0.6));
    this->addChild(label);

    auto restartLabel = LabelBMFont::create("Restart", "Arial.fnt");
    auto restartItem = MenuItemLabel::create(restartLabel, CC_CALLBACK_1(HelloWorld::restartTapped, this));
    restartItem->setScale(0.1);
    restartItem->setPosition(Vec2(_winSize.width/2, _winSize.height * 0.4));
    
    auto menu = Menu::create(restartItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
    
    restartItem->runAction(ScaleTo::create(0.5, 1.0));
    label->runAction(ScaleTo::create(0.5, 1.0));
    this->unscheduleUpdate();
}

float HelloWorld::randomValueBetween(float low, float high)
{
    return (((float)arc4random() / 0xFFFFFFFFu) * (high - low)) + low;
}

float HelloWorld::getTimeTick()
{
    timeval time;
    gettimeofday(&time, nullptr);
    unsigned long millisec = (time.tv_sec * 1000) + (time.tv_usec/1000);
    return (float)millisec;
}

void HelloWorld::setInvisible(Node* node)
{
    node->setVisible(false);
}
#pragma mark -