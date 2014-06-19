//
//  ParallaxNodeExtras.cpp
//  t02
//
//  Created by 野口一也 on 2014/06/09.
//
//

#include "ParallaxNodeExtras.h"

USING_NS_CC;

class PointObject : public Ref
{
public:
    static PointObject * create(Vec2 ratio, Vec2 offset)
    {
        PointObject *ret = new PointObject();
        ret->initWithPoint(ratio, offset);
        ret->autorelease();
        return ret;
    }
    
    bool initWithPoint(Vec2 ratio, Vec2 offset)
    {
        _ratio = ratio;
        _offset = offset;
        _child = nullptr;
        return true;
    }
    
    inline const Vec2& getRatio() const { return _ratio; };
    inline void setRatio(const Vec2& ratio) { _ratio = ratio; };
    
    inline const Vec2& getOffset() const { return _offset; };
    inline void setOffset(const Vec2& offset) { _offset = offset; };
    
    inline Node* getChild() const { return _child; };
    inline void setChild(Node* child) { _child = child; };
    
private:
    Vec2 _ratio;
    Vec2 _offset;
    Node *_child; // weak ref
};

ParallaxNodeExtras::ParallaxNodeExtras()
: ParallaxNode::ParallaxNode()
{
}

ParallaxNodeExtras* ParallaxNodeExtras::node()
{
    return new ParallaxNodeExtras();
}

void ParallaxNodeExtras::incrementOffset(cocos2d::Vec2 offset, cocos2d::Node *node)
{
    for (unsigned int i = 0; i < _parallaxArray->num; i++) {
        PointObject* point = (PointObject*)_parallaxArray->arr[i];
        Node* curNode = point->getChild();
        if (curNode == node) {
            point->setOffset(point->getOffset() + offset);
            break;
        }
    }
}