#include "RNG.h"

RNG::RNG() : dist(0.0, 1.0)
{
	// initialize mt19937 with seed
	// seed_seq를 써야 64bit 대신 19937bit의 랜덤성을 가짐
	typedef std::seed_seq::result_type seed_type;

	std::seed_seq ss{
		static_cast<seed_type>(std::random_device{}()),
		static_cast<seed_type>(std::chrono::high_resolution_clock::now().time_since_epoch().count()),
		static_cast<seed_type>(std::hash<std::thread::id>{}(std::this_thread::get_id()))};

	gen.seed(ss);
}

double RNG::operator()()
{
	return dist(gen);
}

bool RNG::hit(double probability)
{
	return (*this)() < probability;
}

int RNG::binom(int n, double p)
{
	binomDist.param(std::binomial_distribution<int>::param_type(n, p));
	return binomDist(gen);
}