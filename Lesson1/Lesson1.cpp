#include <omp.h>
#include <thread>
#define n 10000000
const auto max_threads = std::thread::hardware_concurrency();

double f(double x) {
    return x * x;
}

double integrate_par(double a, double b, int thread_num) {
    double sum = 0;
    double dx = (b - a) / n;
    #pragma omp parallel
    {
        double my_sum = 0;
        size_t t = omp_get_thread_num();
        //size_t T = omp_get_num_threads();
        size_t T = thread_num;
        for (size_t i = t; i < n; i += T)
            my_sum += f(a + i * dx);
        #pragma omp critical
        {
            sum += my_sum;
        }
    }
    return dx * sum;
}

double integrate(double a, double b) {
    double sum = 0;
    double dx = (b - a) / n;
    for (int i = 0; i < n; i++)
        sum += f(a + i * dx);
    return dx * sum;
}

#include <iostream>
int main(int argc, char** argv)
{
    //std::cout << max_threads << "\n";

    //for (size_t i = 0; i < max_threads + 1; i++) {
    //    std::cout << "Thread num: " << i << "\n";
    //    for (size_t j = 0; j < 20; j++) {
    //        if (i > 0) {
    //            double t1 = omp_get_wtime();
    //            integrate_par(-1, 1, i);
    //            double t2 = omp_get_wtime();
    //            std::cout << t2 - t1 << "\n";
    //        }
    //        else {
    //            double t1 = omp_get_wtime();
    //            integrate(-1, 1);
    //            double t2 = omp_get_wtime();
    //            std::cout << t2 - t1 << "\n";
    //        }
    //        
    //    }
    //}
    double result;
    auto duration1 = std::chrono::steady_clock::now();

    double time_sum_1;
    for (std::size_t i = 1; i <= std::thread::hardware_concurrency(); i++)
    {
        double time_sum = 0;
        for (size_t j = 0; j < 20; j++) {
            auto t1 = std::chrono::steady_clock::now();
            result = integrate_par(-1, 1, i);
            auto t2 = std::chrono::steady_clock::now();
            time_sum += std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();

            
        }

        if (i == 1)
        {
            time_sum_1 = time_sum;
        }

        std::cout << "T = " << i << ", duration = "
            << time_sum / 20 << ", acceleration = " << (time_sum_1 / 20) / (time_sum / 20) << "\n";
    }
    return 0;
}


