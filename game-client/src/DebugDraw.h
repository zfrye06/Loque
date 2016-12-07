/**
 * @file DebugDraw.h
 * @brief Practically this entire file was rewritten by me, so I pretty much own it now. It had to be re-written due to the 2011 SFML being completely different... It is just the hooks for box2D to call in order to draw stuff.
 * @author Dalton Nell
 * @version 0.0.0
 * @date 2016-12-06
 */

/*
    Copyright (C) 2011 Allen Jordan ()
    Copyright (C) 2011 Xabier Larrakoetxea (slok)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DEBUGDRAW_H
#define DEBUGDRAW_H

#include <Box2D/Box2D.h>
#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

// meter to pixel ratio..
#define RATIO 64.f
// Default alpha, lines have the same alpha + 50
#define ALPHA 75

class DebugDraw : public b2Draw {
    public:
        DebugDraw(sf::RenderTarget &window);
        virtual ~DebugDraw();

        void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
        void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
        void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
        void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
        void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
        void DrawTransform(const b2Transform& xf);
        void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);
        void DrawString(int x, int y, const char* string);
        void DrawAABB(b2AABB* aabb, const b2Color& color);
        sf::Color B2SFColor(const b2Color &color, int alpha);
        void DrawMouseJoint(b2Vec2& p1, b2Vec2& p2, const b2Color &boxColor, const b2Color &lineColor);

    private:
        sf::RenderTarget *window;
};

#endif // DEBUGDRAW_H
