#pragma once
#include "GachaState.h"
#include "RNG.h"
#include <array>

enum class PullResult
{
	NONE,
	COMMON_SSR,
	TARGET_SSR
};

struct UrgentPullResult
{
	int SSRCount = 0;
	int targetSSRCount = 0;
};

// 30, 60은 복잡해서 Simulator에서 구현함
// 80, 120만 고려함
class GachaEngine
{
public:
	static inline PullResult rollOnce(GachaState &state, RNG &rng)
	{
		// 편하게 일단 스택 증가시키고 시작함
		// 1번째 뽑기 -> 1/1
		++state.pityCount;
		++state.totalCount;

		// 120회 천장
		if (!state.gotTargetSSR && state.totalCount >= targetThreshold)
		{
			state.pityCount = 0;
			state.gotTargetSSR = true;
			return PullResult::TARGET_SSR;
		}
		// 반천
		else
		{
			double ssrProbability = ssrProbTable[state.pityCount];

			if (rng.hit(ssrProbability))
			{
				state.pityCount = 0;
				if (rng.hit(targetSSRProbability))
				{
					state.gotTargetSSR = true;
					return PullResult::TARGET_SSR;
				}
				else
				{
					return PullResult::COMMON_SSR;
				}
			}
			else
			{
				return PullResult::NONE;
			}
		}
	}

	static inline UrgentPullResult rollTen(RNG &rng)
	{
		UrgentPullResult result;

		result.SSRCount = rng.binom(10, baseSSRProbability);

		if (result.SSRCount > 0)
		{
			result.targetSSRCount = rng.binom(result.SSRCount, targetSSRProbability);
		}

		return result;
	}

private:
	static constexpr int pityThreshold = 80;
	static constexpr int targetThreshold = 120;

	static constexpr double baseSSRProbability = 0.008;
	static constexpr int probabilityIncreaseThreshold = 65;
	static constexpr double probabilityIncreasePerPull = 0.05;

	static constexpr double targetSSRProbability = 0.5;

	static constexpr auto ssrProbTable = []
	{
		std::array<double, 81> table{};
		for (int pityCount = 0; pityCount <= pityThreshold; ++pityCount)
		{
			if (pityCount >= pityThreshold)
			{
				table[pityCount] = 1.0;
			}
			else if (pityCount >= probabilityIncreaseThreshold)
			{
				double increasedProbability = baseSSRProbability + (pityCount - probabilityIncreaseThreshold) * probabilityIncreasePerPull;
				table[pityCount] = std::min(increasedProbability, 1.0);
			}
			else
			{
				table[pityCount] = baseSSRProbability;
			}
		}
		return table;
	}();
};