#include <iostream> // for input/output operations
#include <future>   // for std::async and std::shared_future
#include <chrono>   // for measuring time
#include <thread>   // for std::thread and std::thread::hardware_concurrency
#include <vector>   // for std::vector
#include <mutex>    // for std::mutex
#include <cmath>    // for pow and M_PI
#include <iomanip>  // for std::setprecision

double calculate_pi(int terms, int start, int skip) // calculates the value of pi using the Leibniz formula.
                                                    // It takes three arguments: the number of terms to calculate, the starting index, and the step size.
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
   
    const int threads_number = std::thread::hardware_concurrency(); // Get the number of hardware threads available on the current system.
    std::vector<std::shared_future<double>> futures; // Create a vector of shared futures to store the results of each thread.

    auto start = std::chrono::steady_clock::now(); // Get the current time.

    for (int i = 0; i < threads_number; i++) // Launch a separate thread for each chunk of the calculation.
    {
        std::shared_future<double> f = std::async(std::launch::async, calculate_pi, 1E8, i, threads_number);
        futures.push_back(f);
    }

    double sum = 0.0; // Sum up the results from each thread.

    for (auto f : futures)
    {
        sum += f.get();
    }

    auto end = std::chrono::steady_clock::now(); // Get the end time and calculate the duration of the calculation.
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    
    std::cout << "Duration: " << duration << " miliseconds" << std::endl; // Print the results.
    std::cout << std::setprecision(15) << "PI:  " << M_PI << std::endl;
    std::cout << std::setprecision(15) << "Sum: " << sum << std::endl;

    return 0;
}
