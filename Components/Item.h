#ifndef ITEM_H
#define ITEM_H

#include "GameObject.h"

class Item : public GameObject
{
public:
    Item();
    ~Item() override;

    void DebugPrint() const;



};


#endif