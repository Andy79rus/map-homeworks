#include <iostream>
#include <thread>
#include <Windows.h>

using namespace std::chrono_literals;

void client(int& current, int max_query)
{
	do
	{
		std::cout << "client - Число клиентов в очереди: " << ++current << std::endl;
		std::this_thread::sleep_for(1000ms);
	} while (current != max_query);
}

void manager(int& current)
{
	do
	{
		std::this_thread::sleep_for(2000ms);
		std::cout << "manager - Число клиентов в очереди: " << --current << std::endl;
	} while (0 < current);
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Russian");

	int max_query_count = 0;
	thread_local int query_to_use = 0;

	std::cout << "Введите максимальную длину очереди: ";
	std::cin >> max_query_count;

	std::thread T1(client, std::ref(query_to_use), max_query_count);
	std::thread T2(manager, std::ref(query_to_use));
	T1.join();
	T2.join();

	return 0;
}