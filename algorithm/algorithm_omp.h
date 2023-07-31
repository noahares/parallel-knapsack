#pragma once

#ifdef HAS_OMP
#include "algorithm.h"

#include <omp.h>

class AlgorithmOmp : public Algorithm {
protected:
    explicit AlgorithmOmp(const size_t n, const unsigned int threads) : Algorithm{n, threads} {
        omp_set_num_threads(threads);
    }
};

#endif
