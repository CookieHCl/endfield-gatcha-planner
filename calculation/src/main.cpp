#include <bits/stdc++.h>
#include "GachaState.h"
using namespace std;

int main()
{
	iostream::sync_with_stdio(0);

	GachaState state(1);
	state.totalCount += 5;

	cout << state.pityCount << ' ' << state.totalCount << endl;
	return 0;
}