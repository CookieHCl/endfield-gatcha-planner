#include "Simulator.h"

Simulator::Simulator(int initialPity) : initialPity(initialPity) {}

SimulationResult Simulator::runSimulation(int simulationCount)
{
	SimulationResult globalResult;

#pragma omp parallel
	{
		SimulationResult localResult;
		RNG rng;

#pragma omp for
		for (int i = 0; i < simulationCount; ++i)
		{
			auto [rolls, ssrs] = runSingleSession(rng);
			localResult.rollCount[rolls]++;
			localResult.SSRCount[ssrs]++;
		}

#pragma omp critical
		{
			for (int i = 1; i <= 120; ++i)
			{
				globalResult.rollCount[i] += localResult.rollCount[i];
				globalResult.SSRCount[i] += localResult.SSRCount[i];
			}
		}
	}

	return globalResult;
}

std::pair<int, int> Simulator::runSingleSession(RNG &rng)
{
	GachaState state(initialPity);

	int SSRCount = 0;

	while (!state.gotTargetSSR)
	{
		PullResult pullResult = GachaEngine::rollOnce(state, rng);

		if (pullResult == PullResult::COMMON_SSR || pullResult == PullResult::TARGET_SSR) [[unlikely]]
		{
			++SSRCount;
		}

		if (state.totalCount == 30) [[unlikely]]
		{
			UrgentPullResult urgentResult = GachaEngine::rollTen(rng);
			SSRCount += urgentResult.SSRCount;
			if (urgentResult.targetSSRCount > 0) [[unlikely]]
			{
				break;
			}
		}
	}

	return {state.totalCount, SSRCount};
}
