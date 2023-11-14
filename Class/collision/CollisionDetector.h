#pragma once
#include"Adapter.h"
#include <iostream>
#include<cmath>



class CollisionDetector
{
public:


	/// <summary>
	/// 円と矩形の当たり判定
	/// </summary>
	/// <param name="circleCenter">円の中心点</param>
	/// <param name="circleRadius">円の半径</param>
	/// <param name="rectCenter">矩形の中心点</param>
	/// <param name="rectSide">矩形の一辺の長さ</param>
	/// <returns>当たっているか当たっていないか</returns>
	bool isCollision(LWP::Math::Vector2 circleCenter, float circleRadius, LWP::Math::Vector2 rectCenter, LWP::Math::Vector2 rectSide);

private:

};

