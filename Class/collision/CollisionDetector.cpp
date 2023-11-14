#include "CollisionDetector.h"


bool CollisionDetector::isCollision(LWP::Math::Vector2 circleCenter, float circleRadius, LWP::Math::Vector2 rectCenter, LWP::Math::Vector2 rectSide)
{
    LWP::Math::Vector2 cubeMin
    {
        rectCenter.x - (rectSide.x / 2.0f),
        rectCenter.y,
    };

    LWP::Math::Vector2 cubeMax
    {
        rectCenter.x + (rectSide.x / 2.0f),
        rectCenter.y + (rectSide.y)
    };

    // 最近接点
    LWP::Math::Vector2 closestPoint
    {
        static_cast<float>( std::clamp(circleCenter.x, cubeMin.x, cubeMax.x)),
        static_cast<float>( std::clamp(circleCenter.y, cubeMin.y,cubeMax.y)),
    };

    // 最近接点と円の中心との距離を計算
    float distance = static_cast<float>(std::sqrt(static_cast<float>(std::pow(closestPoint.x - circleCenter.x, 2) +
        static_cast<float>(std::pow(closestPoint.y - circleCenter.y, 2)))));

    // 距離が円の半径以下ならば当たり
    return distance <= circleRadius;
}

