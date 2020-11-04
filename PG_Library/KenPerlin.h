#pragma once

double Noise(double x, double y, double z);

double Noise(double x, double y);

double Noise(double x);

double** Generate2DMap(int width, int height, float scale, float offsetX, float offsetY);