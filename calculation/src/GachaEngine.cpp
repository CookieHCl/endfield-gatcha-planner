#include "GachaEngine.h"

GachaEngine::GachaEngine() : rng() {}

PullResult GachaEngine::rollOnce(GachaState &state)
{
	// 편하게 일단 스택 증가시키고 시작함
	// 1번째 뽑기 -> 1/1
	++state.pityCount;
	++state.totalCount;

	// 뽑기 시뮬레이션
	PullResult result;
	// 120회 천장
	if (!state.gotTargetSSR && state.totalCount >= targetThreshold)
	{
		result = PullResult::TARGET_SSR;
	}
	// 반천
	else
	{
		double ssrProbability = getSSRProbability(state.pityCount);

		if (rng.hit(ssrProbability))
		{
			if (rng.hit(targetSSRProbability))
			{
				result = PullResult::TARGET_SSR;
			}
			else
			{
				result = PullResult::COMMON_SSR;
			}
		}
		else
		{
			result = PullResult::NONE;
		}
	}

	// SSR 당첨시 갱신
	switch (result)
	{
	case PullResult::NONE:
		break;
	case PullResult::COMMON_SSR:
		state.pityCount = 0;
		break;
	case PullResult::TARGET_SSR:
		state.pityCount = 0;
		state.gotTargetSSR = true;
		break;
	}
	return result;
}

UrgentPullResult GachaEngine::rollTen()
{
	UrgentPullResult result;

	result.SSRCount = rng.binom(10, baseSSRProbability);

	if (result.SSRCount > 0)
	{
		result.targetSSRCount = rng.binom(result.SSRCount, targetSSRProbability);
	}

	return result;
}

double GachaEngine::getSSRProbability(int pityCount)
{
	// 80회 천장
	if (pityCount >= pityThreshold)
	{
		return 1.0;
	}
	// 66회부터 확률업
	else if (pityCount >= probabilityIncreaseThreshold)
	{
		double increasedProbability = baseSSRProbability + (pityCount - probabilityIncreaseThreshold) * probabilityIncreasePerPull;
		return std::min(increasedProbability, 1.0);
	}
	else
	{
		return baseSSRProbability;
	}
}