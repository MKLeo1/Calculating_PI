#include <iostream>
#include <future>
#include <chrono>
#include <thread>
#include <vector>
#include <mutex>
#include <cmath>
#include <iomanip>
#include <chrono>

double calculate_pi(int terms, int start, int skip)
{
    double sum = 0.0;

    for (int i = start; i < terms; i += skip)
    {
        int sign = pow(-1, i);
        double term = 1.0 / (i * 2 + 1);
        sum += sign * term;
    }
    return sum * 4;
}

int main()
{
    const int threads_number = std::thread::hardware_concurrency();
    std::vector<std::shared_future<double>> futures;

    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < threads_number; i++)
    {
        std::shared_future<double> f = std::async(std::launch::async, calculate_pi, 1E8, i, threads_number);
        futures.push_back(f);
    }

    double sum = 0.0;

    for (auto f : futures)
    {
        sum += f.get();
    }

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Duration: " << duration << " miliseconds" << std::endl;
    std::cout << std::setprecision(15) << "PI:  " << M_PI << std::endl;
    std::cout << std::setprecision(15) << "Sum: " << sum << std::endl;

    return 0;
}
