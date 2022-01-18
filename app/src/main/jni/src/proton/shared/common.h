#pragma once

typedef struct _CL_Vec2f {
    float x;
    float y;

    _CL_Vec2f() {
        x = 0.0f;
        y = 0.0f;
    }

    _CL_Vec2f(float f) {
        x = f;
        y = f;
    }

    _CL_Vec2f(float x, float y) {
        this->x = x;
        this->y = y;
    }

    bool operator==(const _CL_Vec2f& other) const {
        return x == other.x && y == other.y;
    }

    _CL_Vec2f operator-(const _CL_Vec2f& other) const {
        return _CL_Vec2f(x - other.x, y - other.y);
    }

    _CL_Vec2f operator*(const float& other) const {
        return _CL_Vec2f(x * other, y * other);
    }
} CL_Vec2f;

typedef struct _CL_Vec2i {
    int x;
    int y;

    _CL_Vec2i() {
        x = 0;
        y = 0;
    }

    _CL_Vec2i(int f) {
        x = f;
        y = f;
    }

    _CL_Vec2i(int x, int y) {
        this->x = x;
        this->y = y;
    }

    bool operator==(const _CL_Vec2i& other) const {
        return x == other.x && y == other.y;
    }

    _CL_Vec2i operator-(const _CL_Vec2i& other) const {
        return _CL_Vec2i(x - other.x, y - other.y);
    }

    _CL_Vec2i operator*(const int& other) const {
        return _CL_Vec2i(x * other, y * other);
    }
} CL_Vec2i;

typedef struct _CL_Vec3f {
    float x;
    float y;
    float z;

    _CL_Vec3f() {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }

    _CL_Vec3f(float f) {
        x = f;
        y = f;
        z = f;
    }

    _CL_Vec3f(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    bool operator==(const _CL_Vec3f& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    _CL_Vec3f operator-(const _CL_Vec3f& other) const {
        return _CL_Vec3f(x - other.x, y - other.y, z - other.z);
    }

    _CL_Vec3f operator*(const float& other) const {
        return _CL_Vec3f(x * other, y * other, z * other);
    }
} CL_Vec3f;

typedef struct _CL_Vec3i {
    int x;
    int y;
    int z;

    _CL_Vec3i() {
        x = 0;
        y = 0;
        z = 0;
    }

    _CL_Vec3i(int f) {
        x = f;
        y = f;
        z = f;
    }

    _CL_Vec3i(int x, int y, int z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    bool operator==(const _CL_Vec3i& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    _CL_Vec3i operator-(const _CL_Vec3i& other) const {
        return _CL_Vec3i(x - other.x, y - other.y, z - other.z);
    }

    _CL_Vec3i operator*(const int& other) const {
        return _CL_Vec3i(x * other, y * other, z * other);
    }
} CL_Vec3i;

typedef struct _CL_Rectf {
    float x;
    float y;
    float width;
    float height;

    _CL_Rectf() {
        x = 0.0f;
        y = 0.0f;
        width = 0.0f;
        height = 0.0f;
    }

    _CL_Rectf(float f) {
        x = f;
        y = f;
        width = f;
        height = f;
    }

    _CL_Rectf(float x, float y, float width, float height) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }

    bool operator==(const _CL_Rectf& other) const {
        return x == other.x && y == other.y && width == other.width && height == other.height;
    }

    _CL_Rectf operator-(const _CL_Rectf& other) const {
        return _CL_Rectf(x - other.x, y - other.y, width - other.width, height - other.height);
    }

    _CL_Rectf operator*(const float& other) const {
        return _CL_Rectf(x * other, y * other, width * other, height * other);
    }
} CL_Rectf;

typedef struct _CL_Recti {
    int x;
    int y;
    int width;
    int height;

    _CL_Recti() {
        x = 0;
        y = 0;
        width = 0;
        height = 0;
    }

    _CL_Recti(int f) {
        x = f;
        y = f;
        width = f;
        height = f;
    }

    _CL_Recti(int x, int y, int width, int height) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }

    bool operator==(const _CL_Recti& other) const {
        return x == other.x && y == other.y && width == other.width && height == other.height;
    }

    _CL_Recti operator-(const _CL_Recti& other) const {
        return _CL_Recti(x - other.x, y - other.y, width - other.width, height - other.height);
    }

    _CL_Recti operator*(const int& other) const {
        return _CL_Recti(x * other, y * other, width * other, height * other);
    }
} CL_Recti;