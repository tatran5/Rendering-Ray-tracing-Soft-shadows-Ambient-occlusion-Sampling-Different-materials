#include "globalincludes.h"

Vector3f middleVector2(Vector3f v1, Vector3f v2)
{
    return v1 + v2;
}

Vector3f middleVector2(Vector3f v1, Vector3f v2, Vector3f add)
{
    return middleVector2(v1, v2) + add;
}

