#pragma once
class Random
{
public:
    static unsigned int Rand();
    static unsigned int Rand(unsigned int newseed);
    static unsigned int Rand(unsigned int a, unsigned int b);

    static float FooPluginFunction() { return 5.0F; }
};