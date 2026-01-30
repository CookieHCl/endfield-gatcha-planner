#pragma once
#include <random>
#include <chrono>
#include <thread>

// 64bit [0.0~1.0) RNG
class RNG
{
public:
	RNG();
	double operator()();
	bool hit(double probaility);
	int binom(int n, double p);

private:
	std::mt19937_64 gen;
	std::uniform_real_distribution<double> dist;
	std::binomial_distribution<int> binomDist;
};