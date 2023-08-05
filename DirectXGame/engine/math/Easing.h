#pragma once
#include "Vector3.h"

float PlayEaseIn(float time, float startPos, float endDistance, float totalTime);
float easeInCubic(float x);

float PlayEaseOutBouce(float time, float startPos, float endDistance, float totalTime);
float easeOutBouce(float x);
