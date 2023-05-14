/*****************************************************************
*\file		 EventBus.h
*\brief		 Header file declarations for EventBus.

*\author(s)   Lee Liang Ping	18 lines x 33% Code contribution
*\author(s)   Kenric Tan Wei Liang	18 lines x 33% Code contribution
*\author(s)   Lee Jun Jie	17 lines x 33% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
#include "EventHandler.h"


class EventBus
{
public:
    using HandlerList = std::vector<std::unique_ptr<IEventHandler>>;

    ~EventBus()
    {
        for (auto evnt = eventQueue.begin(); evnt != eventQueue.end(); evnt++)
            delete* evnt;
    }
    template<typename EventType>
    void publish(EventType& evnt)
    {
        eventQueue.push_back(evnt);
    }

    template<typename EventType>
    void execute(EventType& evnt)
    {
        for (auto& handler : *subscribers[evnt->id])
            handler->execute(evnt);
        delete evnt;
    }

    void clearQueue()
    {
        eventQueue.clear();
    }

    void executeQueue()
    {
        if (!eventQueue.empty())
        {
            for (auto evnt = eventQueue.begin(); evnt != eventQueue.end(); evnt++)
            {
                if (subscribers[(*evnt)->id])
                    for (auto& handler : *subscribers[(*evnt)->id])
                        handler->execute(*evnt);
                delete* evnt;
            }
            clearQueue();
        }

    }

    template<typename T, typename EventType>
    void subscribe(T* instance, void (T::* eventFunction)(EventType*), EventID id)
    {
        //First time initialization
        if (subscribers[id] == nullptr)
            subscribers[id] = std::make_unique<HandlerList>();

        subscribers[id]->push_back(std::make_unique<EventHandler<T, EventType>>(instance, eventFunction));
    }
private:
    std::map<EventID, std::unique_ptr<HandlerList>> subscribers;
    std::vector<Event*> eventQueue;
};  