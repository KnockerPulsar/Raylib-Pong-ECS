#include "RectCollision.h"
#include "BaseCollision.h"
#include "BallCollision.h"
#include "Component.h"
#include <vector>
#include "raylib.h"
#include "../../vendor/raylib-cpp/raylib-cpp.hpp"

namespace pong
{

    RectCollision::RectCollision(raylib::Vector2 *pos, float w, float h) : width(w), height(h)
    {
        position = pos;
        tag = tags::coll;
    }

    RectCollision::~RectCollision()
    {
        // if (position)
        // delete position;
    }

    void RectCollision::Update()
    {
        DrawDebug();
    }

    void RectCollision::DrawDebug(raylib::Color col)
    {
        DrawRectangle(position->x,position->y,width,height, col);
        DrawRectangleLines(position->x, position->y, width, height, col);
    }

    Rectangle RectCollision::GetRect()
    {
        return (Rectangle){position->x, position->y, width, height};
    }

    bool RectCollision::CheckCollision(Component *other) { return other->CheckCollision(this); }

    bool RectCollision::CheckCollision(BaseCollision *other) { return other->CheckCollision(this); }

    // Rect-ball collision
    bool RectCollision::CheckCollision(BallCollision *other)
    {
        Rectangle rect = this->GetRect();
        return CheckCollisionCircleRec(*other->position, other->radius, rect);
    }

    // Rect-Rect collision
    bool RectCollision::CheckCollision(RectCollision *other)
    {
        return CheckCollisionRecs(this->GetRect(), other->GetRect());
    }

    bool RectCollision::IsNotOnScreen()
    {
        return !CheckCollisionRecs(this->GetRect(), raylib::Rectangle(0, 0, GetScreenWidth(), GetScreenHeight()));
    }

} // namespace pong
