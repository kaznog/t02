//
//  ParallaxNodeExtras.h
//  t02
//
//  Created by 野口一也 on 2014/06/09.
//
//

#ifndef __t02__ParallaxNodeExtras__
#define __t02__ParallaxNodeExtras__

#include "cocos2d.h"

USING_NS_CC;

class ParallaxNodeExtras : public cocos2d::ParallaxNode
{
public:
    ParallaxNodeExtras();
    
    static ParallaxNodeExtras* node();
    
    void incrementOffset(Vec2 offset, Node* node);
};

#endif /* defined(__t02__ParallaxNodeExtras__) */
