#ifndef ENTITY_H
#define ENTITY_H

#include "IEntity.h"

namespace ECS {

    class Entity : public IEntity {


    public:

        Entity() {}
        virtual ~Entity() {}

        // Called when this entity is enabled
        virtual void OnEnabled() override {}

        // Called when this entity is deisabled
        virtual void OnDisabled() override {}

    };


}


#endif // ENTITY_H