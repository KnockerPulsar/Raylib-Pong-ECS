#include "RectCollision.h"
#include "BaseCollision.h"
#include "BallCollision.h"
#include "Component.h"
#include <vector>
#include "raylib.h"
#include "../../include/raylib-cpp/include/raylib-cpp.hpp"

namespace pong
{

    RectCollision::RectCollision(float w, float h) : width(w), height(h) {}

    RectCollision::RectCollision(float w, float h)
        : width(w), height(h)
    {
    }

    RectCollision::RectCollision(const RectCollision &rc)
    {
        this->width = rc.width;
        this->height = rc.height;
    }

    RectCollision::~RectCollision() {}

    void RectCollision::Update() { DrawDebug(); }

    void RectCollision::Start() {}

    void RectCollision::DrawDebug(raylib::Color col)
    {
        raylib::Vector2 position = GetPosition();
        DrawRectangle(position.x, position.y, width, height, col);
        DrawRectangleLines(position.x, position.y, width, height, col);
    }

    Rectangle RectCollision::GetRect()
    {
        return (Rectangle){
            GetPosition().x, GetPosition().y, width, height};
    }

    bool RectCollision::CheckCollision(Component *other) { return other->CheckCollision(this); }

    bool RectCollision::CheckCollision(BaseCollision *other) { return other->CheckCollision(this); }

    // Rect-ball collision
    bool RectCollision::CheckCollision(BallCollision *other)
    {
        Rectangle rect = this->GetRect();
        return CheckCollisionCircleRec(other->GetPosition(), other->radius, rect);
    }

    // Rect-Rect collision
    bool RectCollision::CheckCollision(RectCollision *other)
    {
        return CheckCollisionRecs(this->GetRect(), other->GetRect());
    }

    bool RectCollision::IsNotOnScreen()
    {
        return CheckCollisionRecs(this->GetRect(), raylib::Rectangle(0, 0, GetScreenWidth(), GetScreenHeight()));
    }

} // namespace pong
