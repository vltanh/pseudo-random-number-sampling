#include <iostream>
#include <cstring>
#include <cmath>
#include <random>
#include <chrono>
#include <fstream>
#include <functional>
using namespace std;

// Prototype

template <typename T, typename F>
class Distribution;
template <typename T, typename F>
class ExponentialDistribution;
template <typename T, typename F>
T inverse_sampling(const Distribution<T, F>);

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
T inverse_sampling(const Distribution<T, F> &d)
{
    uniform_real_distribution<F> unif(0, 1);
    F u = unif(rng);
    return d.quantile(u);
}

// Distributions

template <typename T, typename F>
class Distribution
{
public:
    virtual T quantile(F) const = 0;
    virtual T sample() const = 0;
    virtual vector<T> samples(int) const = 0;
};

template <typename T, typename F>
class ExponentialDistribution : public Distribution<T, F>
{
private:
    F lambda;

public:
    ExponentialDistribution(F lambda)
    {
        this->lambda = lambda;
    }

    T quantile(F y) const
    {
        return -log(1 - y) / this->lambda;
    }

    T sample() const
    {
        return inverse_sampling<T, F>(*this);
    }

    vector<T> samples(int N) const
    {
        vector<T> arr(N);
        for (int i = 0; i < N; i++)
        {
            arr[i] = this->sample();
        }
        return arr;
    }
};

int main(int argc, char *argv[])
{
    float lambda = atof(argv[1]);
    int N = atoi(argv[2]);

    seed(&rng);
    ExponentialDistribution<float, float> dist(lambda);
    vector<float> arr = dist.samples(N);

    write_to_file<float>("exp_inverse.txt", arr);
}