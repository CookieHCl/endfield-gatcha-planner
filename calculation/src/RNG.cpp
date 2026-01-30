#include "RNG.h"

RNG::RNG() : dist(0.0, 1.0)
{
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