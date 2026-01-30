#include "Simulator.h"
#include <bits/stdc++.h>
using namespace std;

int main()
{
	iostream::sync_with_stdio(0);

	const int SIMULATION_COUNT = 10000000;

	Simulator simulator(0);

	chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
	SimulationResult result = simulator.runSimulation(SIMULATION_COUNT);
	chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();

	long long rollEv = 0, ssrEv = 0;

	for (int i = 1; i <= 120; ++i)
	{
		cout << i << " rolls: " << result.rollCount[i] << "\n";
		rollEv += i * result.rollCount[i];
		ssrEv += i * result.SSRCount[i];
	}
	cout << "Expected rolls to get target SSR: " << (double)rollEv / SIMULATION_COUNT << "\n";
	cout << "Expected SSR count: " << (double)ssrEv / SIMULATION_COUNT << "\n";

	// Thread 이전) 7909ms
	cout << "Simulation Time: "
		 << chrono::duration_cast<chrono::milliseconds>(end - start).count()
		 << " ms\n";

	return 0;
}