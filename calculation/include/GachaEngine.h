#pragma once
#include "GachaState.h"
#include "RNG.h"

enum class PullResult
{
	NONE,
	COMMON_SSR,
	TARGET_SSR
};

// 30, 60은 복잡해서 Simulator에서 구현함
// 80, 120만 고려함
class GachaEngine
{
public:
	GachaEngine();

	PullResult rollOnce(GachaState &state);

private:
	double getSSRProbability(int pityCount);

	RNG rng;

	const int pityThreshold = 80;
	const int targetThreshold = 120;

	double baseSSRProbability = 0.008;
	const int probabilityIncreaseThreshold = 65;
	const long double probabilityIncreasePerPull = 0.05;

	double targetSSRProbability = 0.5;
};