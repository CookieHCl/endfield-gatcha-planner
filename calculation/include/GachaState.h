#pragma once

struct GachaState
{
	int pityCount;
	int totalCount;
	bool gotTargetSSR;

	inline GachaState(int pityCount) : pityCount(pityCount), totalCount(0), gotTargetSSR(false) {}
};