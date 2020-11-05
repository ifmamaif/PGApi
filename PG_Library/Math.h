#pragma once

int Abs(int value);
float Abs(float value);
double Abs(double value);

//float Floor(float f);

int FloorToInt(float f);

int RoundToInt(float f);

// Function to linearly interpolate between a0 and a1
// weight should be in the range [0.0, 1.0]
float Lerp(float a0, float a1, float weight);

// Function to linearly interpolate between a0 and a1
// weight should be in the range [0.0, 1.0]
double Lerp(double a0, double a1, double weight);