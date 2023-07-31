#pragma once

#include <chrono>
#include <memory>
#include <string>
#include <vector>

class Algorithm {
public:
    // Each item is a weight, value pair
    // Returns vector of the indices that correspond to the items in the solution
    using tWeight = uint32_t;
    using tValue = uint32_t;
    using tCapacity = uint64_t;
    struct tItem {
        tWeight weight;
        tValue value;
    };
    using tItemList = std::vector<tItem>;
    using tSolution = tCapacity;
    struct ProblemInstance {
        tCapacity capacity;
        tItemList items;
    };
    enum class Type { DynamicProgramming, BranchAndBound };

    std::pair<tSolution, std::chrono::milliseconds> run_timed(const ProblemInstance &instance) {
        auto start = std::chrono::high_resolution_clock::now();
        auto solution = do_run(instance);
        auto d = std::chrono::high_resolution_clock::now();
        return {std::move(solution), std::chrono::duration_cast<std::chrono::milliseconds>(d - start)};
    };

protected:
    explicit Algorithm(const size_t n, const unsigned int threads) : n{n}, threads{threads} {};

    const size_t n;
    const unsigned int threads;

private:
    virtual tSolution do_run(const ProblemInstance &instance) = 0;
};
