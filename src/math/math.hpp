#pragma once

#include <Windows.h>
#include <cmath>
#include "../config/config.hpp"

struct view_matrix_t {
    float viewMatrix[4][4];

    float* operator[](int i) {
        return viewMatrix[i];
    }
};

struct Vector3 {
    float x, y, z;

    Vector3 operator+(const Vector3& other) {
        return { x + other.x, y + other.y, z + other.z };
    }
    Vector3 operator-(const Vector3& other) {
        return { x - other.x, y - other.y, z - other.z };
    }
    Vector3 operator*(float scalar) {
        return { x * scalar, y * scalar, z * scalar };
    }
    Vector3 operator/(float scalar) {
        return { x / scalar, y / scalar, z / scalar };
    }

	float distance(const Vector3& other) const {
		return sqrtf(powf(other.x - x, 2) + powf(other.y - y, 2) + powf(other.z - z, 2));
	}



    Vector3 WorldToScreen(view_matrix_t viewMatrix) const {

        int screenWidth = config::screenSize.GetWidth();
		int screenHeight = config::screenSize.GetHeight();

        float w = viewMatrix[3][0] * x + viewMatrix[3][1] * y + viewMatrix[3][2] * z + viewMatrix[3][3];

        if (w < 0.01f)
            return { -1, -1, 0 };

        float inv_w = 1.0f / w;

        float _x = (viewMatrix[0][0] * x + viewMatrix[0][1] * y + viewMatrix[0][2] * z + viewMatrix[0][3]) * inv_w;
        float _y = (viewMatrix[1][0] * x + viewMatrix[1][1] * y + viewMatrix[1][2] * z + viewMatrix[1][3]) * inv_w;

        _x = (screenWidth / 2.0f) + (_x * screenWidth / 2.0f);
        _y = (screenHeight / 2.0f) - (_y * screenHeight / 2.0f);
        return { _x, _y, w };
    }
};