#include "Simulator.h"

Simulator::Simulator(int initialPity) : initialPity(initialPity), engine() {}

SimulationResult Simulator::runSimulation(int simulationCount)
{
	SimulationResult result;

	for (int i = 0; i < simulationCount; ++i)
	{
		auto [rolls, ssrs] = runSingleSession();
		result.rollCount[rolls]++;
		result.SSRCount[ssrs]++;
	}

	return result;
}

std::pair<int, int> Simulator::runSingleSession()
{
	GachaState state(initialPity);

	int SSRCount = 0;

	while (!state.gotTargetSSR)
	{
		PullResult pullResult = engine.rollOnce(state);

		if (pullResult == PullResult::COMMON_SSR || pullResult == PullResult::TARGET_SSR)
		{
			++SSRCount;
		}

		if (state.totalCount == 30)
		{
			UrgentPullResult urgentResult = engine.rollTen();
			SSRCount += urgentResult.SSRCount;
			if (urgentResult.targetSSRCount > 0)
			{
				break;
			}
		}
	}

	return {state.totalCount, SSRCount};
}
