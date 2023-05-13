/*****************************************************************
*\file		 ParticleStates.h
*\brief		 ParticleStates Component 

*\author(s)   Lee Liang Ping	28 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

struct ParticleStates
{
    ParticleState pState;
};

struct ParticleActions
{
    ParticleAction current_action;
    std::queue<ParticleAction> pActions;

    void Add(std::vector<ParticleAction>& list )
    {
        if (list.empty())
            return;

        clear(pActions);
        current_action = ParticleAction::START;
        for (auto x : list)
        {
            pActions.push(x);
        }
    }

    void UpdateAction()
    {
        if (pActions.empty())
            return;
        current_action = pActions.front();
        pActions.pop();
    }
};