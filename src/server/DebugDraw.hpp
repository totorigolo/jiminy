// This file is part of Jiminy.
// 
// Jiminy is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Jiminy is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Jiminy.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>


class DebugDraw : public b2Draw
{
public:
    // Ctor
    DebugDraw(b2World *b2World);
    DebugDraw(DebugDraw const &) = delete;
    DebugDraw &operator=(DebugDraw const &) = delete;

    // Drawing
    virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

    virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

    virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);

    virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);

    virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

    virtual void DrawTransform(const b2Transform& xf);

    void DrawPoint(const b2Vec2& p, const b2Color& color);

    void DrawContacts();

    // Accessors
    void SetRenderTarget(sf::RenderTarget *target) { mTarget = target; }
    sf::RenderTarget* GetRenderTarget() { return mTarget; }
    const sf::RenderTarget* GetRenderTarget() const { return mTarget; }

private:
    b2World *mb2World;
    sf::RenderTarget *mTarget;
};
