#pragma once
#include "GachaState.h"
#include "RNG.h"

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
	static PullResult rollOnce(GachaState &state, RNG &rng);
	static UrgentPullResult rollTen(RNG &rng);

private:
	static double getSSRProbability(int pityCount);

	static const int pityThreshold = 80;
	static const int targetThreshold = 120;

	static constexpr double baseSSRProbability = 0.008;
	static const int probabilityIncreaseThreshold = 65;
	static constexpr double probabilityIncreasePerPull = 0.05;

	static constexpr double targetSSRProbability = 0.5;
};