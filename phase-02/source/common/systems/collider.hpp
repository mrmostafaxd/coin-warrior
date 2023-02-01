#include "../ecs/world.hpp"
#include "../components/collider.hpp"
#include "../components/free-player-controller.hpp"

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <vector>
#include <iostream>

namespace our
{
    class ColliderSystem
    {
        Application *app; // The application in which the state runs

    public:
        // when a state enters, it should call this function and give it the pointer to the application
        void enter(Application *app)
        {
            this->app = app;
        }

        // this should be called every frame to update all entities containing a colliderComponent
        void update(World *world, float deltaTime)
        {
            // create a vector of all collider components
            std::vector<ColliderComponent *> colliders;
            for (auto entity : world->getEntities())
            {
                ColliderComponent *collider = entity->getComponent<ColliderComponent>();
                if (collider)
                {
                    colliders.push_back(collider);
                }
            }

            // iterate through all colliders
            for (int i = 0; i < colliders.size(); i++)
            {
                // get the first collider
                ColliderComponent *collider1 = colliders[i];

                // iterate through all colliders again
                for (int j = 0; j < colliders.size(); j++)
                {
                    // if the colliders are the same, skip
                    if (i == j)
                    {
                        continue;
                    }

                    // get the second collider
                    ColliderComponent *collider2 = colliders[j];

                    // get the entity that we found via getOwner of collider
                    Entity *entity1 = collider1->getOwner();
                    Entity *entity2 = collider2->getOwner();

                    // get the diffrence in x position  between the two entities
                    float x_diff = entity1->localTransform.position.x - entity2->localTransform.position.x;
                    // get the diffrence in y position  between the two entities
                    float y_diff = entity1->localTransform.position.y - entity2->localTransform.position.y;
                    // get the diffrence in z position  between the two entities
                    float z_diff = entity1->localTransform.position.z - entity2->localTransform.position.z;

                    // check if the difference is negative then make it positive
                    if (x_diff < 0)
                    {
                        x_diff = -1 * x_diff;
                    }
                    if (y_diff < 0)
                    {
                        y_diff = -1 * y_diff;
                    }
                    if (z_diff < 0)
                    {
                        z_diff = -1 * z_diff;
                    }
                    

                    // if the difference in x, y, and z is less than the sum of the two colliders' x_diff, y_diff, and z_diff
                    if (x_diff <= (collider1->x_diff + collider2->x_diff) && y_diff <= (collider1->y_diff + collider2->y_diff) && z_diff <= (collider1->z_diff + collider2->z_diff))
                    {
                        if (collider1->action == 0 && collider2->action == 1)
                        {
                            // if the first collider is of action 0 and the second is of action 1, then the first is a player and the second is a coin

                            // so we print a message to the console about coin collision
                            std::cout << "Coin collision 1" << std::endl;

                            // we add 1 to the coin count of the player
                            entity1->getComponent<FreePlayerControllerComponent>()->coin_count += 1;
                        }

                        if (collider1->action == 0 && collider2->action == 2)
                        {
                            std::cout << "Obstacle collision 1" << std::endl;

                            // set obstacle collision to true
                            entity1->getComponent<FreePlayerControllerComponent>()->obstacle_hit = true;
                        }

                        if (collider2->action == 1) {
                            world->markForRemoval(entity2);
                        }
                    }
                }
            }
        }
    };
}