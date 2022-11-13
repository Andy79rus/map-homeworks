#include <iostream>
#include <thread>
#include <windows.h>
#include <vector>
#include <random>
#include <algorithm>
#include <execution>

enum THREADS
{
	THREADS_1 = 1,
	THREADS_2 = 2,
	THREADS_4 = 4,
	THREADS_8 = 8,
	THREADS_16 = 16
};

enum ELEMS
{
	ELEM1000 = 1'000,
	ELEM10000 = 10'000,
	ELEM100000 = 100'000,
	ELEM1000000 = 1'000'000
};

void DoCalc(const std::vector<int>& a, const std::vector<int>& b, const int th, const int elem)
{
	std::vector<int> result;
	result.resize(elem);
	auto sum([&](int i)
		{
			for (int j = i; j < elem; j+=th)
			{
				result[j] = std::move(a[j] + b[j]);
			}
		});

	std::vector<std::thread> T;
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < th; ++i)
		{
			T.push_back(std::thread(sum, i));
		}
	for (auto& thd : T)
	{
		thd.join();
	}
	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double, std::milli> time = end - start;
	std::cout << time.count()/1000 << "s" "\t";
	if (elem == ELEM1000000) std::cout << std::endl;
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Russian");

	std::cout << "Количество аппаратных ядер - " << std::thread::hardware_concurrency() << std::endl << std::endl;
	std::cout << "\t\t" << ELEM1000 << "\t\t" << ELEM10000 << "\t\t" << ELEM100000 << "\t\t" << ELEM1000000 << std::endl;

	std::vector<int> vec1(ELEM1000000);
	std::vector<int> vec2(ELEM1000000);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, ELEM1000000);
	auto rand_num([=]() mutable { return dis(gen); });
	std::generate(std::execution::par, begin(vec1), end(vec1), rand_num);
	std::generate(std::execution::par, begin(vec2), end(vec2), rand_num);

	std::cout << THREADS_1 << " поток:\t";
	DoCalc(vec1, vec2, THREADS_1, ELEM1000);
	DoCalc(vec1, vec2, THREADS_1, ELEM10000);
	DoCalc(vec1, vec2, THREADS_1, ELEM100000);
	DoCalc(vec1, vec2, THREADS_1, ELEM1000000);

	std::cout << THREADS_2 << " потока:\t";
	DoCalc(vec1, vec2, THREADS_2, ELEM1000);
	DoCalc(vec1, vec2, THREADS_2, ELEM10000);
	DoCalc(vec1, vec2, THREADS_2, ELEM100000);
	DoCalc(vec1, vec2, THREADS_2, ELEM1000000);

	std::cout << THREADS_4 << " потока:\t";
	DoCalc(vec1, vec2, THREADS_4, ELEM1000);
	DoCalc(vec1, vec2, THREADS_4, ELEM10000);
	DoCalc(vec1, vec2, THREADS_4, ELEM100000);
	DoCalc(vec1, vec2, THREADS_4, ELEM1000000);

	std::cout << THREADS_8 << " потоков:\t";
	DoCalc(vec1, vec2, THREADS_8, ELEM1000);
	DoCalc(vec1, vec2, THREADS_8, ELEM10000);
	DoCalc(vec1, vec2, THREADS_8, ELEM100000);
	DoCalc(vec1, vec2, THREADS_8, ELEM1000000);

	std::cout << THREADS_16 << " потоков:\t";
	DoCalc(vec1, vec2, THREADS_16, ELEM1000);
	DoCalc(vec1, vec2, THREADS_16, ELEM10000);
	DoCalc(vec1, vec2, THREADS_16, ELEM100000);
	DoCalc(vec1, vec2, THREADS_16, ELEM1000000);

	return 0;
}