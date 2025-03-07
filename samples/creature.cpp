#pragma once
// SPDX-FileCopyrightText: 2023 Erin Catto
// SPDX-License-Identifier: MIT

#include "creature.h"
#include "box2d/types.h"
#include "box2d/math_functions.h"
#include <box2d/box2d.h>

#include <assert.h>

Creature::Creature()
{
	m_bodyId = b2_nullBodyId;
	m_isSpawned = false;
}

void Creature::Spawn( b2WorldId worldId, b2Vec2 position, float scale, float dampingRatio, void* userData )
{
    assert( m_isSpawned == false );

    float radius = 1.0f * scale;
    float length = 1.0f * scale;

    b2Capsule capsule = { { 0.0f, -0.5f * length }, { 0.0f, 0.5f * length }, 0.25f * scale };


    b2Vec2 center = position;

    b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.position = { center.x, center.y };
    bodyDef.type = b2_dynamicBody;
    bodyDef.userData = userData;

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    shapeDef.friction = 0.3f;
    shapeDef.customColor = b2_colorSteelBlue; // Change the color to red


    // Create bodies
    float angle = 0.0f;
    /*bodyDef.position = { radius * cosf( angle ) + center.x, radius * sinf( angle ) + center.y };
    bodyDef.rotation = b2MakeRot( angle );*/

    m_bodyId = b2CreateBody( worldId, &bodyDef );
    b2CreateCapsuleShape( m_bodyId, &shapeDef, &capsule );

    m_isSpawned = true;
}

void Creature::Despawn()
{
	assert( m_isSpawned == true );

	b2DestroyBody( m_bodyId );

	m_isSpawned = false;
}

void Creature::SetSpeed( float speed ){}
void Creature::SetDampingRadio( float dampingRatio ){}

