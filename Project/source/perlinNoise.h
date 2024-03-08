#pragma once

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
#include <numeric>

/// <summary>
/// 2Dのパーリンノイズを生成するクラス
/// </summary>
class PerlinNoise {
public:
    PerlinNoise();
    PerlinNoise(unsigned int seed);

    double noise(double x, double y) const;

private:
    std::vector<int> p;

    static double fade(double t);
    static double lerp(double t, double a, double b);
    static double grad(int hash, double x, double y);
};

PerlinNoise::PerlinNoise() {
    p.resize(512);
    std::iota(p.begin(), p.end(), 0);
    std::default_random_engine engine;
    std::shuffle(p.begin(), p.end(), engine);
    p.insert(p.end(), p.begin(), p.end());
}

PerlinNoise::PerlinNoise(unsigned int seed) {
    p.resize(512);
    std::iota(p.begin(), p.end(), 0);
    std::default_random_engine engine(seed);
    std::shuffle(p.begin(), p.end(), engine);
    p.insert(p.end(), p.begin(), p.end());
}

double PerlinNoise::fade(double t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double PerlinNoise::lerp(double t, double a, double b) {
    return a + t * (b - a);
}

double PerlinNoise::grad(int hash, double x, double y) {
    int h = hash & 15;
    double grad = 1 + (h & 7); // Gradient value 1-8
    if (h & 8) grad = -grad;   // Randomly invert half of them
    return (h & 1 ? x : -x) + (h & 2 ? y : -y);
}

double PerlinNoise::noise(double x, double y) const {
    int X = static_cast<int>(std::floor(x)) & 255;
    int Y = static_cast<int>(std::floor(y)) & 255;

    x -= std::floor(x);
    y -= std::floor(y);

    double u = fade(x);
    double v = fade(y);

    int a = p[X] + Y;
    int b = p[X + 1] + Y;

    return lerp(v, lerp(u, grad(p[a], x, y), grad(p[b], x - 1, y)),
        lerp(u, grad(p[a + 1], x, y - 1), grad(p[b + 1], x - 1, y - 1)));
}