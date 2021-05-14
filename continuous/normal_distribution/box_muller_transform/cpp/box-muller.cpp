#include <iostream>
#include <cstring>
#include <cmath>
#include <random>
#include <chrono>
#include <fstream>
using namespace std;

// Utility functions and variables

mt19937_64 rng;

void seed(mt19937_64 *rng)
{
    uint64_t timeSeed = chrono::high_resolution_clock::now().time_since_epoch().count();
    seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32)};
    rng->seed(ss);
}

template <typename T>
void write_to_file(string fn, vector<T> arr)
{
    ofstream f;
    f.open(fn);
    f.precision(9);

    int N = arr.size();
    for (int i = 0; i < N; i++)
        f << arr[i] << " ";
    f.close();
}

// Sampling methods

template <typename F>
vector<F> box_muller(F mu, F sigma, int N)
{
    uniform_real_distribution<F> unif(0, 1);

    int M = N / 2 + (N % 2);
    vector<F> arr(2 * M);

    int j = 0;
    while (j < N)
    {
        F u1 = unif(rng);
        F u2 = unif(rng);
        F R = sqrt(-2 * log(u1));
        F O = 2 * M_PI * u2;
        arr[j++] = sigma * R * cos(O) + mu;
        arr[j++] = sigma * R * sin(O) + mu;
    }

    if (N < 2 * M)
        arr.pop_back();
    return arr;
}

int main(int argc, char *argv[])
{
    float mu = atof(argv[1]);
    float sigma = atof(argv[2]);
    int N = atoi(argv[3]);

    seed(&rng);
    vector<float> arr = box_muller<float>(mu, sigma, N);

    write_to_file<float>("box-muller.txt", arr);
}