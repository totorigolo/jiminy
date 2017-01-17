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

#include "DebugDraw.hpp"
#include "sf2b2.hpp"


// Ctor
DebugDraw::DebugDraw(b2World * b2World):
        mb2World(b2World)
{
    //AppendFlags(b2Draw::e_aabbBit);
    //AppendFlags(b2Draw::e_centerOfMassBit);
    AppendFlags(b2Draw::e_jointBit);
    //AppendFlags(b2Draw::e_pairBit);
    AppendFlags(b2Draw::e_shapeBit);
}

// Dessin
void DebugDraw::DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{
    if (!mTarget) return;

    sf::Color c = convert(color);
    sf::VertexArray va(sf::LinesStrip, vertexCount + 1U);

    for (int32 i = 0; i < vertexCount; ++i)
    {
        va[i].position = convert(vertices[i]);
        va[i].color = c;
    }
    va[vertexCount].position = convert(*vertices);
    va[vertexCount].color = c;

    mTarget->draw(va);
}

void DebugDraw::DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{
    if (!mTarget) return;

    DrawPolygon(vertices, vertexCount, color);
}

void DebugDraw::DrawCircle(const b2Vec2 &center, float32 radius, const b2Color &color)
{
    if (!mTarget) return;

    float r = radius * PPM - 1.f; // 1.f = bordure
    sf::CircleShape cs;
    cs.setRadius(r);
    cs.setOutlineThickness(1.f);
    cs.setOrigin(sf::Vector2f(r, r));
    cs.setOutlineColor(convert(color));
    cs.setFillColor(sf::Color::Transparent);
    cs.setPosition(convert(center));

    mTarget->draw(cs);
}

void DebugDraw::DrawSolidCircle(const b2Vec2 &center, float32 radius, const b2Vec2 &axis, const b2Color &color)
{
    if (!mTarget) return;

    DrawCircle(center, radius, color);
    DrawSegment(center, center + radius * axis, color);
}

void DebugDraw::DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color)
{
    if (!mTarget) return;

    sf::Color c = convert(color);
    sf::VertexArray va(sf::Lines, 2U);

    va[0].position = convert(p1);
    va[0].color = c;
    va[1].position = convert(p2);
    va[1].color = c;

    mTarget->draw(va);
}

void DebugDraw::DrawTransform(const b2Transform &xf)
{
    if (!mTarget) return;

    b2Vec2 p1 = xf.p, p2;
    const float32 k_axisScale = 0.4f;
    p2 = p1 + k_axisScale * xf.q.GetXAxis();
    DrawSegment(p1, p2, b2Color(1.f, 0.f, 0.f));

    p2 = p1 + k_axisScale * xf.q.GetYAxis();
    DrawSegment(p1, p2, b2Color(0.f, 1.f, 0.f));
}

void DebugDraw::DrawPoint(const b2Vec2 &p, const b2Color &color)
{
    if (!mTarget) return;

    // Affichage d'un disque
    sf::CircleShape cs;
    cs.setRadius(2.f);
    cs.setFillColor(convert(color));
    cs.setOrigin(sf::Vector2f(2.f, 2.f));
    cs.setPosition(convert(p));
    mTarget->draw(cs);
}

void DebugDraw::DrawContacts()
{
    // Récupère les contacts
    b2Contact *contact = mb2World->GetContactList();

    // Affiche les contacts
    for (; contact; contact = contact->GetNext())
    {
        // Affiche les Sensor
        if (contact->IsTouching())
        {
            if (contact->GetFixtureA()->IsSensor())
            {
                DrawPoint(contact->GetFixtureA()->GetBody()->GetWorldCenter() - b2Vec2(0.05f, 0.f),
                          b2Color(1.f, 0.4f, 0.f));
            }
            if (contact->GetFixtureB()->IsSensor())
            {
                DrawPoint(contact->GetFixtureB()->GetBody()->GetWorldCenter() + b2Vec2(0.05f, 0.f),
                          b2Color(1.f, 1.f, 0.f));
            }
        }

        // Affiche les points
        b2Manifold *m = contact->GetManifold();
        if (m->pointCount == 0)
            continue;

        b2WorldManifold wm;
        contact->GetWorldManifold(&wm);

        // Pour chaque point de contact
        for (int j = 0; j < m->pointCount && j < 2; ++j)
        {
            b2Vec2 p1 = wm.points[j];

            // Affiche les points
            DrawPoint(p1, b2Color(0.3f, 0.95f, 0.3f));

            // Affiche les normales
            b2Vec2 p2 = p1 + 0.3f * wm.normal;
            DrawSegment(p1, p2, b2Color(0.9f, 0.9f, 0.9f));
        }
    }
}
