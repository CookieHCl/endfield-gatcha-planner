#pragma once
#include <random>
#include <chrono>
#include <thread>

// 64bit [0.0~1.0) RNG
class RNG
{
public:
	inline RNG()
	{
		// 느리지만 seed_seq를 사용해 무조건 랜덤 보장
		typedef std::seed_seq::result_type seed_type;

		std::seed_seq ss{
			static_cast<seed_type>(std::random_device{}()),
			static_cast<seed_type>(std::chrono::high_resolution_clock::now().time_since_epoch().count()),
			static_cast<seed_type>(std::hash<std::thread::id>{}(std::this_thread::get_id()))};

		uint32_t seeds[8];
		ss.generate(seeds, seeds + 8);

		for (int i = 0; i < 4; ++i)
		{
			state[i] = (static_cast<uint64_t>(seeds[i * 2]) << 32) | seeds[i * 2 + 1];
		}
	}

	inline double operator()()
	{
		static constexpr double inverse_2_53 = 1.0 / 9007199254740992.0; // 1 / 2^53
		return (xoshiro256p() >> 11) * inverse_2_53;
	}

	inline bool hit(double probability)
	{
		return (*this)() < probability;
	}

	inline int binom(int n, double p)
	{
		int count = 0;
		for (int i = 0; i < n; ++i)
		{
			if (hit(p))
			{
				++count;
			}
		}
		return count;
	}

private:
	// xoshiro256+ (only for floating number)
	// https://en.wikipedia.org/wiki/Xorshift#xoshiro256+
	static inline uint64_t rol64(uint64_t x, int k)
	{
		return (x << k) | (x >> (64 - k));
	}

	uint64_t state[4];

	inline uint64_t xoshiro256p()
	{
		uint64_t const result = state[0] + state[3];
		uint64_t const t = state[1] << 17;

		state[2] ^= state[0];
		state[3] ^= state[1];
		state[1] ^= state[2];
		state[0] ^= state[3];

		state[2] ^= t;
		state[3] = rol64(state[3], 45);

		return result;
	}
};