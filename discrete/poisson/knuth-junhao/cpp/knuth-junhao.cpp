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

    int N = arr.size();
    for (int i = 0; i < N; i++)
        f << arr[i] << " ";
    f.close();
}

// Sampling methods

template <typename T, typename F>
T poisson_knuth(F lambda)
{
    uniform_real_distribution<F> unif(0, 1);
    F L = exp(-lambda);
    T k = 0;
    F p = 1.0;
    do
    {
        k = k + 1;
        F u = unif(rng);
        p = p * u;
    } while (p > L);

    return k - 1;
}

template <typename T, typename F>
int poisson_junhao(F lambda, F STEP = 10.0)
{
    uniform_real_distribution<F> unif(0, 1);
    F lambda_left = lambda;
    T k = 0;
    F p = 1.0;
    do
    {
        k = k + 1;
        F u = unif(rng);
        p = p * u;
        while (p < 1.0 && lambda_left > 0.0)
        {
            if (lambda_left > STEP)
            {
                p = p * exp(STEP);
                lambda_left -= STEP;
            }
            else
            {
                p = p * exp(lambda_left);
                lambda_left = 0.0;
            }
        }
    } while (p > 1.0);

    return k - 1;
}

int main(int argc, char *argv[])
{
    float lambda = atof(argv[1]);
    int N = atoi(argv[2]);

    seed(&rng);
    vector<int> knuth_a(N), junhao_a(N);
    for (int i = 0; i < N; i++)
    {
        knuth_a[i] = poisson_knuth<int, float>(lambda);
        junhao_a[i] = poisson_junhao<int, float>(lambda);
    }

    write_to_file<int>("knuth.txt", knuth_a);
    write_to_file<int>("junhao.txt", junhao_a);
}