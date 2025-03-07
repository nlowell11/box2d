// SPDX-FileCopyrightText: 2022 Erin Catto
// SPDX-License-Identifier: MIT

#include "creature.h"
#include "draw.h"
#include "human.h"
#include "sample.h"
#include "settings.h"

#include "box2d/box2d.h"
#include "box2d/math_functions.h"

#include <GLFW/glfw3.h>
#include <imgui.h>

class BenchmarkSmashNate : public Sample
{
public:
	explicit BenchmarkSmashNate( Settings& settings )
		: Sample( settings )
	{
		if ( settings.restart == false )
		{
			g_camera.m_center = { 60.0f, 6.0f };
			g_camera.m_zoom = 25.0f * 1.6f;
		}

		b2World_SetGravity( m_worldId, b2Vec2_zero );

		{
			b2Polygon box = b2MakeBox( 4.0f, 4.0f );

			b2BodyDef bodyDef = b2DefaultBodyDef();
			bodyDef.type = b2_dynamicBody;
			bodyDef.position = { -20.0f, 0.0f };
			bodyDef.linearVelocity = { 10.0f, 0.0f };
			bodyDef.linearDamping = { 0.0f };
			b2BodyId bodyId = b2CreateBody( m_worldId, &bodyDef );

			b2ShapeDef shapeDef = b2DefaultShapeDef();
			shapeDef.density = 8.0f;
			b2CreatePolygonShape( bodyId, &shapeDef, &box );
		}

		m_created = false;
	}

	void CreateScene1()
	{
		ImGuiIO& io = ImGui::GetIO();
		if ( io.Fonts->Fonts.size() == 0 )
		{
			return;
		}

		const ImFont* font = io.Fonts->Fonts[0];
		const unsigned char* pixels = font->ContainerAtlas->TexPixelsAlpha8;
		int width = font->ContainerAtlas->TexWidth;
		int height = font->ContainerAtlas->TexHeight;

		float scale = 0.1f;

		b2BodyDef bodyDef = b2DefaultBodyDef();
		bodyDef.type = b2_dynamicBody;
		bodyDef.isAwake = false;

		b2ShapeDef shapeDef = b2DefaultShapeDef();

		for ( int i = 0; i < height; ++i )
		{
			for ( int j = 0; j < width; ++j )
			{
				unsigned char value = pixels[i * width + j];
				if ( value != 0 && value != 0xFF )
				{
					value += 0;
				}

				if ( value > 50 )
				{
					b2Polygon square = b2MakeSquare( 0.95f * scale * ( value / 255.0f ) );
					bodyDef.position = { 2.0f * j * scale, 2.0f * ( height - i ) * scale - 10.0f };
					b2BodyId bodyId = b2CreateBody( m_worldId, &bodyDef );
					b2CreatePolygonShape( bodyId, &shapeDef, &square );
				}
			}
		}

		m_created = true;
	}

	void CreateScene2()
	{
		ImGuiIO& io = ImGui::GetIO();
		if ( io.Fonts->Fonts.size() == 0 )
		{
			return;
		}

		const ImFont* font = io.Fonts->Fonts.back();
		const unsigned char* pixels = font->ContainerAtlas->TexPixelsAlpha8;
		int width = font->ContainerAtlas->TexWidth;
		int height = font->ContainerAtlas->TexHeight;
		int fontSize = font->Ascent;

		float scale = 0.1f;

		b2BodyDef bodyDef = b2DefaultBodyDef();
		bodyDef.type = b2_dynamicBody;
		bodyDef.isAwake = false;

		b2ShapeDef shapeDef = b2DefaultShapeDef();

		const char* text = "I love programming";
		int n = (int)strlen( text );
		float zoom = 1.0f;

		float x = 0.0f;
		for ( int k = 0; k < n; ++k )
		{
			const ImFontGlyph* glyph = font->FindGlyph( text[k] );
			float x1 = glyph->X0;
			float x2 = glyph->X1;
			float y1 = glyph->Y0;
			float y2 = glyph->Y1;
			float u1 = glyph->U0;
			float v1 = glyph->V0;
			float u2 = glyph->U1;
			float v2 = glyph->V1;

			float w = zoom * ( x2 - x1 );
			float h = zoom * ( y2 - y1 );

			int gridx = int( w );
			int gridy = int( h );
			for ( int i = 0; i < gridy; ++i )
			{
				float v = v1 + i / h * ( v2 - v1 );
				int iy = int( v * height );

				for ( int j = 0; j < gridx; ++j )
				{
					float u = u1 + j / w * ( u2 - u1 );
					int ix = int( u * width );

					unsigned char value = pixels[iy * width + ix];
					if ( value > 50 )
					{
						b2Polygon square = b2MakeSquare( 0.9f * scale * value / 255.0f );
						bodyDef.position = { x + 2.0f * ( zoom * x1 + j ) * scale, -2.0f * ( zoom * y1 + i ) * scale + 13.0f };
						b2BodyId bodyId = b2CreateBody( m_worldId, &bodyDef );
						b2CreatePolygonShape( bodyId, &shapeDef, &square );
					}
				}
			}

			x += 2.0f * zoom * scale * glyph->AdvanceX;
		}

		m_created = true;
	}

	void CreateScene3()
	{
		float d = 0.4f;
		b2Polygon box = b2MakeSquare( 0.5f * d );

		b2BodyDef bodyDef = b2DefaultBodyDef();
		bodyDef.type = b2_dynamicBody;
		bodyDef.isAwake = false;

		b2ShapeDef shapeDef = b2DefaultShapeDef();

		int columns = g_sampleDebug ? 20 : 120;
		int rows = g_sampleDebug ? 10 : 80;

		for ( int i = 0; i < columns; ++i )
		{
			for ( int j = 0; j < rows; ++j )
			{
				bodyDef.position.x = i * d + 30.0f;
				bodyDef.position.y = ( j - rows / 2.0f ) * d;
				b2BodyId bodyId = b2CreateBody( m_worldId, &bodyDef );
				b2CreatePolygonShape( bodyId, &shapeDef, &box );
			}
		}

		m_created = true;
	}

	void CreateScene4()
	{
		b2Vec2 creature_spawn_pos = { 0, 0 };

		m_creature.Spawn( m_worldId, creature_spawn_pos, 1.0f, 0.0f, {} );

		b2Vec2 creature_array_position = {5, 0};

		
		 for ( int i = 0; i < 8; ++i )
		{
			Creature creature;
			creature_array_position.x += 3.0f;

			//fprintf( stdout, "Created creature at X loc: %f \n", creature_array_position.x);
			
			creature.Spawn( m_worldId, creature_array_position, 1.0f, 0.0f, {} );
			//creature_array_position.x += 10.0f;
		}

		 
		float m_force = 2285.0f;
		//b2Body_ApplyForceToCenter( m_characterId, { m_force, 0.0f }, true );
		b2Body_ApplyForceToCenter( m_creature.m_bodyId, { m_force, 0.0f }, true );


		float d = 0.4f;
		b2Polygon box = b2MakeBox( 0.5f * d, d);

		b2BodyDef bodyDef = b2DefaultBodyDef();
		bodyDef.type = b2_dynamicBody;
		bodyDef.isAwake = false;

		b2ShapeDef shapeDef = b2DefaultShapeDef();

		int columns = g_sampleDebug ? 20 : 120;
		int rows = g_sampleDebug ? 10 : 80;

		for ( int i = 0; i < columns; ++i )
		{
			for ( int j = 0; j < rows; ++j )
			{
				bodyDef.position.x = i * d + 30.0f;
				bodyDef.position.y = ( j - rows / 2.0f ) * d;
				b2BodyId bodyId = b2CreateBody( m_worldId, &bodyDef );
				b2CreatePolygonShape( bodyId, &shapeDef, &box );
			}
		}

		m_created = true;
	}

	void Step( Settings& settings ) override
	{
		Sample::Step( settings );

		if ( m_created == false )
		{
			CreateScene4();
		}
	}

	static Sample* Create( Settings& settings )
	{
		return new BenchmarkSmashNate( settings );
	}

	bool m_created;
	Creature m_creature;
};

static int sampleSmash = RegisterSample( "Nate", "Smash_nate", BenchmarkSmashNate::Create );
