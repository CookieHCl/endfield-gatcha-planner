#include "Simulator.h"
#include <bits/stdc++.h>
using namespace std;

int main()
{
	iostream::sync_with_stdio(0);

	constexpr int SIMULATION_COUNT = 10'000'000;
	const string outputDir = "results";

	if (!filesystem::exists(outputDir))
	{
		filesystem::create_directory(outputDir);
	}

	for (int pityCount = 0; pityCount < 80; ++pityCount)
	{
		Simulator simulator(pityCount);
		SimulationResult result = simulator.runSimulation(SIMULATION_COUNT);

		// calculate EV
		long long rollEv = 0, ssrEv = 0;
		for (int i = 1; i <= 120; ++i)
		{
			rollEv += (long long)i * result.rollCount[i];
			ssrEv += (long long)i * result.SSRCount[i];
		}

		cout << "Pity " << pityCount << " => rollEv: "
			 << (double)rollEv / SIMULATION_COUNT
			 << ", ssrEv: "
			 << (double)ssrEv / SIMULATION_COUNT
			 << "\n";

		// output to file
		string fileName = outputDir + "/pity_" + to_string(pityCount) + ".json";
		ofstream outFile(fileName);

		if (!outFile.is_open())
		{
			cerr << "Error: Could not open file " << fileName << " for writing.\n";
			continue;
		}

		outFile << "{\n";
		outFile << "  \"rollCount\": [\n";
		for (int i = 0; i <= 120; ++i)
		{
			outFile << "    " << (double)result.rollCount[i] / SIMULATION_COUNT;
			if (i < 120)
				outFile << ",";
			outFile << "\n";
		}
		outFile << "  ],\n";
		outFile << "  \"SSRCount\": [\n";
		for (int i = 0; i <= 120; ++i)
		{
			outFile << "    " << (double)result.SSRCount[i] / SIMULATION_COUNT;
			if (i < 120)
				outFile << ",";
			outFile << "\n";
		}
		outFile << "  ],\n";
		outFile << "  \"rollEv\": " << (double)rollEv / SIMULATION_COUNT << ",\n";
		outFile << "  \"ssrEv\": " << (double)ssrEv / SIMULATION_COUNT << "\n";
		outFile << "}\n";
		outFile.close();
	}

	return 0;
}