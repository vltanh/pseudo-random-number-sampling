#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <fstream>
using namespace std;

int poisson_knuth(long double lambda)
{
    std::mt19937_64 rng;
    uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32)};
    rng.seed(ss);
    std::uniform_real_distribution<long double> unif(0, 1);

    long double L = exp(-lambda);
    int k = 0;
    long double p = 1;
    do
    {
        k = k + 1;
        long double u = unif(rng);
        p = p * u;
    } while (p > L);

    return k - 1;
}

int main()
{
    long double lambda = 8.0;
    int N = 10000;
    ofstream myfile;
    myfile.open("example.txt");
    for (int i = 0; i < N; i++)
        myfile << poisson_knuth(lambda) << endl;
    myfile.close();
}