#pragma once
#include <random>

#include "include/randutils.hpp"

namespace utilities {
    namespace random {
        inline randutils::mt19937_rng generator_static() {
            static randutils::mt19937_rng mt19937_rng;
            return mt19937_rng;
        }

        inline randutils::mt19937_rng generator_thread_local() {
            thread_local randutils::mt19937_rng mt19937_rng;
            return mt19937_rng;
        }

        inline randutils::mt19937_rng generator_local() {
            randutils::mt19937_rng mt19937_rng;
            return mt19937_rng;
        }
    } // namespace random
} // namespace utilities