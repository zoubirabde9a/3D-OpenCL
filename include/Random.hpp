#pragma once
#include <random>
#include <ctime>

namespace ZEngine {
	template < typename T >
	T randomReal(T min, T max) {
		static std::mt19937 randomEngine(time(nullptr));
		std::uniform_real_distribution < T > randomNumber(min, max);
		return randomNumber(randomEngine);
	}
	template < typename T >
	T randomInt(T min, T max) {
		static std::mt19937 randomEngine(time(nullptr));
		std::uniform_int_distribution < T > randomNumber(min, max);
		return randomNumber(randomEngine);
	}
	template < typename T>
	std::vector < T > randomVec(T min, T max, size_t size) {
		std::vector < T > vec(size);
		for (size_t i = 0; i < size; i++) {
			vec[i] = randomReal(min, max);
		}
	}
	template < typename T>
	std::vector < T > randomIVec(T min, T max, size_t size) {
		std::vector < T > vec(size);
		for (size_t i = 0; i < size; i++) {
			vec[i] = randomInt(min, max);
		}
	}
}

