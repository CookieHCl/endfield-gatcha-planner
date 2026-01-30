#include <bits/stdc++.h>
#include "GachaState.h"
#include "GachaEngine.h"
using namespace std;

int main()
{
	iostream::sync_with_stdio(0);

	GachaEngine engine;
	GachaState state(0);

	for (int i = 0; i < 120; ++i)
	{
		PullResult result = engine.rollOnce(state);

		cout << "Pull " << i + 1 << ": PityCount=" << state.pityCount << ", TotalCount=" << state.totalCount << ", Result=";
		switch (result)
		{
		case PullResult::NONE:
			cout << "none\n";
			break;
		case PullResult::COMMON_SSR:
			cout << "COMMON\n";
			break;
		case PullResult::TARGET_SSR:
			cout << "TARGET!!!!\n";
			break;
		}
	}
	return 0;
}