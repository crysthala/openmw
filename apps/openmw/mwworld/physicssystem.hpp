#ifndef GAME_MWWORLD_PHYSICSSYSTEM_H
#define GAME_MWWORLD_PHYSICSSYSTEM_H

#include <OgreVector3.h>

#include <btBulletCollisionCommon.h>

#include "ptr.hpp"


namespace OEngine
{
    namespace Render
    {
        class OgreRenderer;
    }
    namespace Physic
    {
        class PhysicEngine;
    }
}

namespace MWWorld
{
    class World;

    typedef std::vector<std::pair<Ptr,Ogre::Vector3> > PtrVelocityList;

    class PhysicsSystem
    {
        public:
            PhysicsSystem (OEngine::Render::OgreRenderer &_rend);
            ~PhysicsSystem ();

            void addObject (const MWWorld::Ptr& ptr, bool placeable=false);

            void addActor (const MWWorld::Ptr& ptr);

            void addHeightField (float* heights,
                int x, int y, float yoffset,
                float triSize, float sqrtVerts);

            void removeHeightField (int x, int y);

            // have to keep this as handle for now as unloadcell only knows scenenode names
            void removeObject (const std::string& handle);

            void moveObject (const MWWorld::Ptr& ptr);

            void rotateObject (const MWWorld::Ptr& ptr);

            void scaleObject (const MWWorld::Ptr& ptr);

            bool toggleCollisionMode();

            void stepSimulation(float dt);

            std::vector<std::string> getCollisions(const MWWorld::Ptr &ptr); ///< get handles this object collides with
            Ogre::Vector3 traceDown(const MWWorld::Ptr &ptr, float maxHeight);

            std::pair<float, std::string> getFacedHandle(float queryDistance);
            std::pair<std::string,Ogre::Vector3> getHitContact(const std::string &name,
                                                               const Ogre::Vector3 &origin,
                                                               const Ogre::Quaternion &orientation,
                                                               float queryDistance);
            std::vector < std::pair <float, std::string> > getFacedHandles (float queryDistance);
            std::vector < std::pair <float, std::string> > getFacedHandles (float mouseX, float mouseY, float queryDistance);

            // cast ray, return true if it hit something. if raycasringObjectOnlt is set to false, it ignores NPCs and objects with no collisions.
            bool castRay(const Ogre::Vector3& from, const Ogre::Vector3& to, bool raycastingObjectOnly = true,bool ignoreHeightMap = false);

            std::pair<bool, Ogre::Vector3>
            castRay(const Ogre::Vector3 &orig, const Ogre::Vector3 &dir, float len);

            std::pair<bool, Ogre::Vector3> castRay(float mouseX, float mouseY, Ogre::Vector3* normal = NULL, std::string* hit = NULL);
            ///< cast ray from the mouse, return true if it hit something and the first result
            /// @param normal if non-NULL, the hit normal will be written there (if there is a hit)
            /// @param hit if non-NULL, the string handle of the hit object will be written there (if there is a hit)

            OEngine::Physic::PhysicEngine* getEngine();

            bool getObjectAABB(const MWWorld::Ptr &ptr, Ogre::Vector3 &min, Ogre::Vector3 &max);

            /// Queues velocity movement for a Ptr. If a Ptr is already queued, its velocity will
            /// be overwritten. Valid until the next call to applyQueuedMovement.
            void queueObjectMovement(const Ptr &ptr, const Ogre::Vector3 &velocity);

            const PtrVelocityList& applyQueuedMovement(float dt);

        private:

            OEngine::Render::OgreRenderer &mRender;
            OEngine::Physic::PhysicEngine* mEngine;
            std::map<std::string, std::string> handleToMesh;

            PtrVelocityList mMovementQueue;
            PtrVelocityList mMovementResults;

            float mTimeAccum;

            PhysicsSystem (const PhysicsSystem&);
            PhysicsSystem& operator= (const PhysicsSystem&);
    };
}

#endif
