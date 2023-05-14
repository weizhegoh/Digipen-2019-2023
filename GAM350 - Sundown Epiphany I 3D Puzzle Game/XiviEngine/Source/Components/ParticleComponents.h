/*****************************************************************
*\file         ParticleComponents.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#include "Particles/ParticleSettings.h"
#include "Particles/ParticleContainer.h"
namespace Xivi
{
using Particles_CList = CList<ParticleSettings, ParticleEmitter>;
}