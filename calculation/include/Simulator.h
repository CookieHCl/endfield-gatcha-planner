#pragma once
#include "GachaEngine.h"
#include <array>
#include <utility>

struct SimulationResult
{
	std::array<int, 121> rollCount = {};
	std::array<int, 121> SSRCount = {};
};

class Simulator
{
public:
	Simulator(int initialPity = 0);

	SimulationResult runSimulation(int simulationCount = 1000000);

private:
	std::pair<int, int> runSingleSession();

	int initialPity;
	GachaEngine engine;
};