#pragma once
#include "GachaState.h"
#include "RNG.h"
#include <algorithm>
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
	static PullResult rollOnce(GachaState &state, RNG &rng)
	{
		// 편하게 일단 스택 증가시키고 시작함
		// 1번째 뽑기 -> 1/1
		++state.pityCount;
		++state.totalCount;

		// 120회 천장
		if (!state.gotTargetSSR && state.totalCount >= targetThreshold) [[unlikely]]
		{
			state.pityCount = 0;
			state.gotTargetSSR = true;
			return PullResult::TARGET_SSR;
		}
		// 반천
		else
		{
			const int ssrProbability = ssrProbTable[state.pityCount];

			if (rng.lessThan<1000>() < static_cast<uint64_t>(ssrProbability)) [[unlikely]]
			{
				state.pityCount = 0;
				if (rng.lessThan<1000>() < static_cast<uint64_t>(targetSSRProbability))
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

	static UrgentPullResult rollTen(RNG &rng)
	{
		UrgentPullResult result;

		for (int i = 0; i < 10; ++i)
		{
			if (rng.lessThan<1000>() < static_cast<uint64_t>(baseSSRProbability)) [[unlikely]]
			{
				++result.SSRCount;

				if (rng.lessThan<1000>() < static_cast<uint64_t>(targetSSRProbability))
				{
					++result.targetSSRCount;
				}
			}
		}

		return result;
	}

private:
	static constexpr int pityThreshold = 80;
	static constexpr int targetThreshold = 120;

	// 확률은 1/1000 단위 정수로 관리
	static constexpr int baseSSRProbability = 8;  // 0.8%
	static constexpr int probabilityIncreaseThreshold = 65;
	static constexpr int probabilityIncreasePerPull = 50; // +5.0%p

	static constexpr int targetSSRProbability = 500; // 50%

	static constexpr auto ssrProbTable = []
	{
		std::array<int, 81> table{};
		for (int pityCount = 0; pityCount <= pityThreshold; ++pityCount)
		{
			if (pityCount >= pityThreshold)
			{
				table[pityCount] = 1000;
			}
			else if (pityCount >= probabilityIncreaseThreshold)
			{
				const int increasedProbability =
					baseSSRProbability +
					(pityCount - probabilityIncreaseThreshold) * probabilityIncreasePerPull;
				table[pityCount] = std::min(increasedProbability, 1000);
			}
			else
			{
				table[pityCount] = baseSSRProbability;
			}
		}
		return table;
	}();
};