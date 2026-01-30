#pragma once
#include "GachaEngine.h"
#include <array>
#include <utility>

struct SimulationResult
{
	static constexpr int MAX_SSR_COUNT = 20; // Gemini 피셜 10^-20? 검증은 안 해봄

	std::array<long long, 121> rollCount = {};
	std::array<long long, MAX_SSR_COUNT + 1> SSRCount = {};
};

class Simulator
{
public:
	Simulator(int initialPity = 0);

	SimulationResult runSimulation(long long simulationCount = 1'000'000);

private:
	std::pair<int, int> runSingleSession(RNG &rng);

	int initialPity;
};