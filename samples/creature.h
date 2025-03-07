#pragma once
// SPDX-FileCopyrightText: 2023 Erin Catto
// SPDX-License-Identifier: MIT

#pragma once

#include "box2d/types.h"

class Creature
{
public:
	Creature();

	void Spawn( b2WorldId worldId, b2Vec2 position, float scale, float dampingRatio, void* userData );
	void Despawn();

	void SetSpeed( float speed );
	void SetDampingRadio( float dampingRatio );

	b2BodyId m_bodyId;
	bool m_isSpawned;
};
