#pragma once
#include <vector>
#include <functional>
#include <unordered_map>
#include "Event.h"
#include "Game.h"
#include "Tags.h"
#include "System.h"

namespace pong
{
    class Entity;

    class IScene
    {
    public:
        std::unordered_map<int, Entity *> entites; // Entities keyed by their entity ID
        std::vector<Event> events;
        std::unordered_map<pong::tags, pong::System *, pong::TagsHashClass> systems;

        // Only one transition should be active at any time
        // How to enforce this?
        std::vector<std::pair<std::function<bool()>, IScene *>> sceneWeb;

        virtual void Update(){};                   // Called every frame
        virtual void Start(IScene *prevScene){};   // Called when transitioning into this scene
        virtual void CleanUp(IScene *nextScene){}; // Called when transitioning from this scene

        // Loops over all transitions and checks if they return true
        // Returns the first scene with a true transition
        IScene *CheckTransitions()
        {
            for (auto &&edge : sceneWeb)
            {

                IScene *nextScene = edge.second;
                bool shouldTransition = edge.first();

                if (shouldTransition)
                {
                    this->CleanUp(nextScene);
                    Game::currScene = nextScene;
                    if (nextScene)
                        nextScene->Start(this);
                    return nextScene;
                }
            }

            // No transitions found, stay on the same scene
            return this;
        }

        // Given a fucntion (preferably a lambda) and a scene
        // Calls back this function every frame to check for scene transitions
        void AddTransition(std::function<bool()> transitionCheck, IScene *scene)
        {
            sceneWeb.emplace_back(transitionCheck, scene);
        }

        // Loops over the event queue of the current scene
        void CheckSceneEvents(float dT)
        {
            std::vector<int> toBeRemovedInd;

            // Run events
            // If an event completed, store its index
            // so we can remove it from the vector
            for (int i = 0; i < events.size(); i++)
                if (events[i].Tick(dT))
                    toBeRemovedInd.push_back(i);

            /* 
            https://stackoverflow.com/questions/4442477/remove-ith-item-from-a-c-stdvector
            Remove elements from vector in O(1) by replacing the element
            to be removed with the last element, then removing the last element
            AKA the element we want to remove
            indx: 0 1 2 3 4
            data: 1 4 5 7 8
            To remove index 2
            data: 1 4 8 7 5  (swap)
            data: 1 4 8 7    (removal) 

            This is done to avoid iterator invalidation 
            (deleting something from an array you're iterating over)
            (AKA pulling the rug from under yourself)
            */

            for (auto &&ind : toBeRemovedInd)
            {
                events[ind] = std::move(events.back());
                events.pop_back();
            }
        }

        // Loops over each system contained in the scene and updates it,
        // which internally loops over the components contained in that system
        void UpdateSystems()
        {
            // C++ 17 destructurings
            // Similar to how python uses tuples
            for (auto &&[tag, system] : systems)
                system->Update();
        }

        // Add an event to the event queue with parameters
        template <typename func, typename... args>
        void AddEvent(float delay, func &&fun, int repititions = 1, float repititionDelay = 1, args... arguments)
        {
            AddEvent(delay, std::bind(fun, arguments...), repititions, repititionDelay);
        }

        // Add an even to the event queue without parameters
        // fn(void)
        template <typename func>
        void AddEvent(float delay, func &&fun, int repititions = 1, float repititionDelay = 1)
        {
            Event ev = Event(delay, fun, repititions, repititionDelay);
            events.push_back(ev);
        }
    };

}