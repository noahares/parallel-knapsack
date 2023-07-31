#include "algorithm_tbb.h"

#include <cstdlib>
#include <tbb/parallel_for.h>
#include <numeric>

// Derive from Algorithm{TBB,OMP}, if you are using either
class AlgorithmImpl : public AlgorithmTBB {
private:
    struct alignas(4) aligned_uint_32 {
        uint32_t content;
    };
    struct alignas(8) aligned_uint_64 {
        uint64_t content;
    };
    // This is the entrypoint of your implementation.
    // Feel free to define new memberfunctions and data members
    // The definition of ProblemInstance can be found in algorithm/algorithm.h
    tSolution do_run(const ProblemInstance &instance) override {

        uint64_t value_sum = std::accumulate(instance.items.begin(), instance.items.end(), 0UL, [](uint64_t s, const auto& item) {
          return s + item.value;
        });
        bool _use_64_bit = value_sum > static_cast<uint64_t>(std::numeric_limits<uint32_t>::max());
        if(_use_64_bit) {
            solve_parallel_64(instance);
            return _read_row_64[_c].content;
        } else {
            solve_parallel(instance);
            return _read_row[_c].content;
        }
    }

    void solve_item(const std::size_t col, const tItem &item) {
        _write_row[col].content = std::max(item.value + _read_row[col - item.weight].content, _read_row[col].content);
    };
    void solve_item_64(const std::size_t col, const tItem &item) {
        _write_row_64[col].content =
            std::max(item.value + _read_row_64[col - item.weight].content, _read_row_64[col].content);
    };

    void solve_parallel(const ProblemInstance &instance) {
        static tbb::affinity_partitioner ap;
        for(size_t row = 1; row <= n; ++row) {
            const tItem &item = instance.items[row - 1];
            tbb::parallel_for(
                tbb::blocked_range<Algorithm::tCapacity>(0UL, _c + 1),
                [&](const tbb::blocked_range<Algorithm::tCapacity> &r) {
                  Algorithm::tCapacity col = r.begin();
                    for(; col < item.weight; ++col) {
                        _write_row[col].content = _read_row[col].content;
                    }
                    for(; col < r.end(); ++col) {
                        solve_item(col, item);
                    }
                },
                ap);
            std::swap(_read_row, _write_row);
        }
    }

    void solve_parallel_64(const ProblemInstance &instance) {
        static tbb::affinity_partitioner ap;
        for(size_t row = 1; row <= n; ++row) {
            const tItem &item = instance.items[row - 1];
            tbb::parallel_for(
                tbb::blocked_range<Algorithm::tCapacity>(0UL, _c + 1),
                [&](const tbb::blocked_range<Algorithm::tCapacity> &r) {
                  Algorithm::tCapacity col = r.begin();
                    for(; col < item.weight; ++col) {
                        _write_row_64[col].content = _read_row_64[col].content;
                    }
                    for(; col < r.end(); ++col) {
                        solve_item_64(col, item);
                    }
                },
                ap);
            std::swap(_read_row_64, _write_row_64);
        }
    }

public:
    // Customize the name of the algorithm without whitespaces
    static constexpr auto name = "PrettyNeatKnapsackSolver";
    // Change this variable to the AlgorithmType. Possible values are
    // BranchAndBound, DynamicProgramming and TwoList
    static constexpr auto type = Algorithm::Type::DynamicProgramming;

    // Startup work like allocating memory can be done in the constructor.
    // The constructor call is not included in the experiments.
    explicit AlgorithmImpl([[maybe_unused]] Algorithm::tCapacity const c, const size_t n, const unsigned int threads)
        : AlgorithmTBB{n, threads}
        , _read_row(c + 1, {0})
        , _write_row(c + 1, {0})
        , _read_row_64(c + 1, {0})
        , _write_row_64(c + 1, {0})
        , _c(c) {
    }

private:
    std::vector<aligned_uint_32> _read_row, _write_row;
    std::vector<aligned_uint_64> _read_row_64, _write_row_64;
    Algorithm::tCapacity _c;
};
