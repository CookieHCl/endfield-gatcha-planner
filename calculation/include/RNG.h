#pragma once
#include <random>
#include <chrono>
#include <atomic>
#include <cstdint>
#include <stdfloat>
#include <bit>

// 64bit [0.0~1.0) RNG
class RNG
{
private:
	using float64_t = std::float64_t;

	class Seeder
	{
	private:
		static inline std::atomic<uint64_t> global_seed_state = []()
		{
			std::random_device rd;

			uint64_t rd_seed = ((uint64_t)(rd()) << 32) | (uint64_t)(rd());
			uint64_t time_seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();

			return rd_seed ^ time_seed;
		}();

	public:
		static inline uint64_t getSeed()
		{
			// https://softwareengineering.stackexchange.com/q/402542
			return global_seed_state.fetch_add(0x9e3779b97f4a7c15ull, std::memory_order_relaxed);
		}
	};

	class InternalRNG
	{
		// wyrand (26/02/27)
		// https://github.com/wangyi-fudan/wyhash
	private:
		uint64_t seed;

		inline uint64_t wyrand()
		{
			seed += 0x2d358dccaa6c78a5ull;
			__uint128_t r = (__uint128_t)seed * (__uint128_t)(seed ^ 0x8bb84b93962eacc9ull);
			return (uint64_t)r ^ (uint64_t)(r >> 64);
		}

	public:
		inline InternalRNG() : seed(Seeder::getSeed()) {}

		inline uint64_t operator()()
		{
			return wyrand();
		}
	};

	InternalRNG rand;

public:
	inline uint64_t operator()()
	{
		return rand();
	}

	inline float64_t ie01() // [0,1)
	{
		uint64_t rand52 = rand() >> 12;
		uint64_t bits = 0x3FF0000000000000ULL | rand52;
		return std::bit_cast<float64_t>(bits) - 1.0;
	}

	inline bool hit(float64_t probability)
	{
		return ie01() < probability;
	}

	inline int binom(int n, float64_t p)
	{
		int count = 0;
		for (int i = 0; i < n; ++i)
		{
			if (hit(p)) [[unlikely]]
			{
				++count;
			}
		}
		return count;
	}
};