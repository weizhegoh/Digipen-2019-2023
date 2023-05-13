/*****************************************************************
*\file		 EventHandler.h
*\brief		 Header file declarations for EventHandler.

*\author(s)   Lee Jun Jie	23 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include "Event.h"
class IEventHandler
{
public:
    virtual void execute(Event* evnt) = 0;
    virtual ~IEventHandler() = default;
};

template<typename T, typename EventType>
class EventHandler : public IEventHandler
{

public:
    using EventFunction = void (T::*)(EventType*);
    EventHandler(T* instance, EventFunction eventFunction) :
        instance{ instance }, eventFunction{ eventFunction } {};

    void execute(Event* evnt) override
    {
        (instance->*eventFunction)(static_cast<EventType*>(evnt));
    }
private:
    T* instance;
    EventFunction eventFunction;
};