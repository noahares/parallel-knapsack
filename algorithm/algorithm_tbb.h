#pragma once

#include "algorithm.h"

#include <tbb/global_control.h>

class AlgorithmTBB : public Algorithm {

private:
    tbb::global_control gc_;

protected:
    explicit AlgorithmTBB(const size_t n, const unsigned int threads)
        : Algorithm{n, threads}
        , gc_(tbb::global_control::parameter::max_allowed_parallelism, threads) {}
};
