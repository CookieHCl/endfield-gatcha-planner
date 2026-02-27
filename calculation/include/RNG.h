#pragma once
#include <bit>
#include <cmath>
// Types
#include <cstdint>
#include <stdfloat>
// Only for Seeder
#include <atomic>
#include <chrono>
#include <random>

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
		static uint64_t getSeed()
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

		uint64_t wyrand()
		{
			seed += 0x2d358dccaa6c78a5ull;
			__uint128_t r = (__uint128_t)seed * (__uint128_t)(seed ^ 0x8bb84b93962eacc9ull);
			return (uint64_t)r ^ (uint64_t)(r >> 64);
		}

	public:
		InternalRNG() : seed(Seeder::getSeed()) {}

		uint64_t operator()()
		{
			return wyrand();
		}
	};

	InternalRNG rand;

public:
	uint64_t operator()()
	{
		return rand();
	}

	// unsigned integers
	// N shoulde be N > 0
	uint64_t lessThan(uint64_t N) // [0, N)
	{
		__uint128_t m = (__uint128_t)(rand()) * (__uint128_t)N;
		uint64_t l = (uint64_t)m;

		if (l < N) [[unlikely]]
		{
			uint64_t t = -N % N;

			while (l < t)
			{
				m = (__uint128_t)(rand()) * (__uint128_t)N;
				l = (uint64_t)m;
			}
		}

		return m >> 64;
	}
	// N should be N > 0
	template <uint64_t N>
	uint64_t lessThan() // [0, N), slightly faster with constexpr
	{
		constexpr uint64_t t = -N % N;

		__uint128_t m;

		do
		{
			m = (__uint128_t)(rand()) * (__uint128_t)N;
			uint64_t l = (uint64_t)m;
		} while ((uint64_t)m < t);

		return m >> 64;
	}

	// integers
	// should be A <= B
	int64_t between(int64_t A, int64_t B) // [A, B]
	{
		uint64_t distance = (uint64_t)B - (uint64_t)A;
		if (distance == 0) [[unlikely]]
		{
			return A;
		}
		else if (distance == UINT64_MAX) [[unlikely]]
		{
			return (int64_t)(rand());
		}

		return A + (int64_t)(lessThan(distance + 1));
	}
	// should be A <= B
	template <int64_t A, int64_t B>
	int64_t between() // [A, B], slightly faster with constexpr
	{
		constexpr uint64_t distance = (uint64_t)B - (uint64_t)A;
		if constexpr (distance == 0)
		{
			return A;
		}
		else if constexpr (distance == UINT64_MAX)
		{
			return (int64_t)(rand());
		}

		return A + (int64_t)(lessThan(distance + 1));
	}

	// floating-point numbers
	float64_t ie01() // [0,1)
	{
		uint64_t rand52 = rand() >> 12;
		uint64_t bits = 0x3FF0000000000000ULL | rand52;
		return std::bit_cast<float64_t>(bits) - 1.0;
	}
	float64_t ei01() // (0,1]
	{
		uint64_t rand52 = rand() >> 12;
		uint64_t bits = 0x3FF0000000000000ULL | rand52;
		return 2.0 - std::bit_cast<float64_t>(bits);
	}
	float64_t ee01() // (0,1)
	{
		uint64_t rand52 = rand() >> 12;						// 0 ~ 2^52-1
		return ((float64_t)rand52 + 0.5f64) * 0x1.0p-52f64; // 0.5 / 2^52 ~ (2^52-0.5) / 2^52
	}
	float64_t ii01() // [0,1]
	{
		uint64_t rand53 = rand() >> 11;								   // 0 ~ 2^53-1
		return ((float64_t)rand53) * (1.0f64 / 9007199254740991.0f64); // 0 ~ (2^53-1) / (2^53-1)
	}

	// should be A < B
	float64_t ieRange(float64_t A, float64_t B) // [A, B)
	{
		float64_t res = std::fma(ie01(), B - A, A);
		if (res >= B) [[unlikely]]
		{
			res = std::nextafter(B, A);
		}
		return res;
	}
	// should be A < B
	float64_t eiRange(float64_t A, float64_t B) // (A, B]
	{
		float64_t res = std::fma(ei01(), B - A, A);
		if (res <= A) [[unlikely]]
		{
			res = std::nextafter(A, B);
		}
		return res;
	}
	// should be A < B
	float64_t eeRange(float64_t A, float64_t B) // (A, B)
	{
		float64_t res = std::fma(ee01(), B - A, A);
		if (res <= A) [[unlikely]]
		{
			res = std::nextafter(A, B);
		}
		else if (res >= B) [[unlikely]]
		{
			res = std::nextafter(B, A);
		}
		return res;
	}
	// should be A <= B
	float64_t iiRange(float64_t A, float64_t B) // [A, B]
	{
		return std::fma(ii01(), B - A, A);
	}

	bool hit(float64_t probability)
	{
		return ie01() < probability;
	}

	int binom(int n, float64_t p)
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