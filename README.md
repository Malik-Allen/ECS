# ECS
Entity-Component-System-v1
User's Manual:
1. Through a world reference you will be able access the functionality of this Entity Component System
2. So the only way to access the ecs is via the world, which will make sure everything is allocated, managed, and deleted correctly
3. You can store your entityIds, component references however you please, no deletes of components or of systems allowed
4. Every component on an entity must be unique, meaning: Multiple components of the same type on a single entity is currently not supported
5. Every System must be unique, meaning: Multiple Systems of the same type/class being registered is currently not supported
6. Every Component class must have a public static constexpr uint32_t ID = GENERATE_ID( "EXAMPLE_Component" );
7. Every System class must have public static constexpr uint32_t ID = GENERATE_ID( "EXAMPLE_System" );
8. The Parser can be used to obtain the Entities with Components auto entitiesWithComponents = Parser<ExampleComponent1, ExampleComponent2>(World*);
