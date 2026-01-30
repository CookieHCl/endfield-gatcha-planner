#include "Simulator.h"

Simulator::Simulator(int initialPity) : initialPity(initialPity) {}

SimulationResult Simulator::runSimulation(int simulationCount)
{
	SimulationResult result;
	RNG rng;

	for (int i = 0; i < simulationCount; ++i)
	{
		auto [rolls, ssrs] = runSingleSession(rng);
		result.rollCount[rolls]++;
		result.SSRCount[ssrs]++;
	}

	return result;
}

std::pair<int, int> Simulator::runSingleSession(RNG &rng)
{
	GachaState state(initialPity);

	int SSRCount = 0;

	while (!state.gotTargetSSR)
	{
		PullResult pullResult = GachaEngine::rollOnce(state, rng);

		if (pullResult == PullResult::COMMON_SSR || pullResult == PullResult::TARGET_SSR)
		{
			++SSRCount;
		}

		if (state.totalCount == 30)
		{
			UrgentPullResult urgentResult = GachaEngine::rollTen(rng);
			SSRCount += urgentResult.SSRCount;
			if (urgentResult.targetSSRCount > 0)
			{
				break;
			}
		}
	}

	return {state.totalCount, SSRCount};
}
