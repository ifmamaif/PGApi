#pragma once

void Test_PerlinNoise();

void GetMinMaxNoise(float (*f)(float, float, float));
void GetMinMaxNoise2D(float (*f)(float, float));

void TestVersions(float (*f1)(float, float), float (*f2)(float, float));