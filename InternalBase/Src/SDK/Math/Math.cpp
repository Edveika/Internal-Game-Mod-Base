#include "Math.h"

// Converts radians of a float to degrees
float Math::radian_to_degree(float radian)
{
	return radian * (180 / PI);
}

// Converts degrees of a float to radians
float Math::degree_to_radian(float degree)
{
	return degree * (PI / 180);
}

// Converts radians of a Vector3 to degrees
Vector3 Math::radian_to_degree(Vector3 radians)
{
	Vector3 degrees;

	degrees.x = radians.x * (180 / PI);
	degrees.y = radians.y * (180 / PI);
	degrees.z = radians.z * (180 / PI);

	return degrees;
}

// Converts degrees of a Vector3 to radians
Vector3 Math::degree_to_radian(Vector3 degrees)
{
	Vector3 radians;

	radians.x = degrees.x * (PI / 180);
	radians.y = degrees.y * (PI / 180);
	radians.z = degrees.z * (PI / 180);

	return radians;
}

// Converts object's 3D coordinates to 2D so we can draw an esp box around it. Also checks if the object is on our screen or behind us.
bool Math::world_to_screen(Vector3 position, Vector2& screenPosition, float viewMatrix[16], int screenWidth, int screenHeight)
{
	// Okay so whats happening here:
	// All you have to do is multiply position vector's rows with viewMatrix's columns
	// NOTE: on some games viewMatrix[1] can be the second x value and viewMatrix[4] can be the first y value

	// x = position.x * viewMatrix.x + position.y * viewMatrix.x1 + position.z * viewMatrix.x2 + x3;
	float xValue = position.x * viewMatrix[0] + position.y * viewMatrix[4] + position.z * viewMatrix[8] + viewMatrix[12];
	//float xValue = position.x * viewMatrix[0] + position.y * viewMatrix[1] + position.z * viewMatrix[2] + viewMatrix[3];

	// y = position.x * viewMatrix.y + position.y * viewMatrix.y1 + position.z * viewMatrix.y2 + y3;
	float yValue = position.x * viewMatrix[1] + position.y * viewMatrix[5] + position.z * viewMatrix[9] + viewMatrix[13];
	//float yValue = position.x * viewMatrix[4] + position.y * viewMatrix[5] + position.z * viewMatrix[6] + viewMatrix[7];

	// And now we have x & y values. BUT we still need to check if object is on our screen or behind us.
	// Acording to this guide: https://guidedhacking.com/threads/so-what-is-a-viewmatrix-anyway-and-how-does-a-w2s-work.10964/
	// Z value doesnt matter for us, so all i need to do is multiply position vector and viewMatrix.w

	// w = position.x * viewMatrix.w + position.y * viewMatrix.w1 + position.z * viewMatrix.w2 + w3;
	float wValue = position.x * viewMatrix[3] + position.y * viewMatrix[7] + position.z * viewMatrix[11] + viewMatrix[15];
	//float wValue = position.x * viewMatrix[12] + position.y * viewMatrix[13] + position.z * viewMatrix[14] + viewMatrix[15];

	// If w value is less then 0.1; NOTE: acording to GH this value may not work on some games, but it should work on most of em
	if (wValue < 0.1f)
		// Object is not on our screen(behind us)
		return false;

	// NDC (normalized device coordinates)
	// This thing scales the screen position to your screen's dimensions
	Vector3 NDC;
	NDC.x = xValue / wValue;
	NDC.y = yValue / wValue;

	// Multiply by your screen dimensions so we can scale it
	// Add half the width and the height to our FINAL values
	// Write the final values to the screenPosition vector
	screenPosition.x = (screenWidth / 2 * NDC.x) + (NDC.x + screenWidth / 2);
	screenPosition.y = -(screenHeight / 2 * NDC.y) + (NDC.y + screenHeight / 2);

	// Object is on our screen
	return true;
}

// Aimbot step 1: subtract enemy coordinates by local player coordinates. It saves a lot of math & makes local player coords. at 0,0,0
Vector3 Math::subtract(Vector3 src, Vector3 dst)
{
	Vector3 difference;

	difference.x = dst.x - src.x;
	difference.y = dst.y - src.y;
	difference.z = dst.z - src.z;

	return difference;
}

// Aimbot step 2: get the hypotenuse using pythagorean theorem.
float Math::pythagorean_theorem(Vector3 vec)
{
	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z); // c^2 = a^2 + b^2
}

// Get the distance from src to dst
float Math::distance(Vector3 src, Vector3 dst)
{
	// Calculate the diffrence so local player is at 0,0,0
	Vector3 difference = subtract(src, dst);

	return pythagorean_theorem(difference);
}

// Aimbot step 3: calclulate angles from src(local player) to dst(enemy)
Vector3 Math::calc_angle(Vector3 src, Vector3 dst)
{
	Vector3 result;

	result.x = -atan2f(dst.x - src.x, dst.y - src.y) / PI * 180.0f + 180.0f;
	result.y = asinf((dst.z - src.z) / distance(src, dst)) * 180.0f / PI;
	// z is used in flight simulators
	result.z = 0.0f;

	return result;
}