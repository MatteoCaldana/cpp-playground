#include <iostream>
#include <vector>
#include <chrono>
#include <random>

template<size_t N>
struct Body {
    double x[N], v[N];
};

constexpr size_t DIM = 3;

static_assert(DIM < 4);

void do_math_v0(std::vector<Body<DIM>> &bodies, double dt) {
    for(size_t i = 0; i < bodies.size(); ++i) {
        auto &body = bodies[i];
        body.x[0] += dt * body.v[0]; 
        body.x[1] += dt * body.v[1];
        if constexpr (DIM > 2)
            body.x[2] += dt * body.v[2];
    }
}

void do_math_v1(std::vector<Body<DIM>> &bodies, double dt) {
    for(size_t i = 0; i < bodies.size(); ++i) {
        auto &body = bodies[i];
        for(size_t k = 0; k < DIM; ++k)
            body.x[k] += dt * body.v[k]; 
    }
}

template<size_t UNROLL>
void do_math_v2(std::vector<Body<DIM>> &bodies, double dt) {
    for(size_t i = 0; i < bodies.size(); i += UNROLL) {
        for(size_t j = 0; j < UNROLL; ++j) {
            auto &body = bodies[i + j];
            for(size_t k = 0; k < DIM; ++k)
                body.x[k] += dt * body.v[k];
        }
    }
}

int main() {
    using namespace std::chrono;
    std::cout << "Allocating memory..." << std::endl;
    std::vector<Body<DIM>> bodies(100000000);
    std::cout << "Randomizing..." << std::endl;
    std::default_random_engine engine(0);
    std::uniform_real_distribution<double> uniform_dist(0., 1.);

    for(auto &b : bodies) {
        for(size_t i = 0; i < DIM; ++i) {
            b.x[i] = uniform_dist(engine);
            b.v[i] = uniform_dist(engine);
        }
    }

    for(int i = 0; i < 3; ++ i) {
        {
            const auto t0 = high_resolution_clock::now();
            do_math_v0(bodies, 0.1);
            const auto t1 = high_resolution_clock::now();
            const auto dt = duration_cast<nanoseconds>(t1 - t0).count();
            std::cout << "Elapsed: " << dt / 1.0e6 << " [ms]" << std::endl;
        }

        {
            const auto t0 = high_resolution_clock::now();
            do_math_v1(bodies, 0.1);
            const auto t1 = high_resolution_clock::now();
            const auto dt = duration_cast<nanoseconds>(t1 - t0).count();
            std::cout << "Elapsed: " << dt / 1.0e6 << " [ms]" << std::endl;
        }

        {
            const auto t0 = high_resolution_clock::now();
            do_math_v2<1>(bodies, 0.1);
            const auto t1 = high_resolution_clock::now();
            const auto dt = duration_cast<nanoseconds>(t1 - t0).count();
            std::cout << "Elapsed: " << dt / 1.0e6 << " [ms]" << std::endl;
        }

        {
            const auto t0 = high_resolution_clock::now();
            do_math_v2<2>(bodies, 0.1);
            const auto t1 = high_resolution_clock::now();
            const auto dt = duration_cast<nanoseconds>(t1 - t0).count();
            std::cout << "Elapsed: " << dt / 1.0e6 << " [ms]" << std::endl;
        }

        {
            const auto t0 = high_resolution_clock::now();
            do_math_v2<3>(bodies, 0.1);
            const auto t1 = high_resolution_clock::now();
            const auto dt = duration_cast<nanoseconds>(t1 - t0).count();
            std::cout << "Elapsed: " << dt / 1.0e6 << " [ms]" << std::endl;
        }

        {
            const auto t0 = high_resolution_clock::now();
            do_math_v2<4>(bodies, 0.1);
            const auto t1 = high_resolution_clock::now();
            const auto dt = duration_cast<nanoseconds>(t1 - t0).count();
            std::cout << "Elapsed: " << dt / 1.0e6 << " [ms]" << std::endl;
        }

        {
            const auto t0 = high_resolution_clock::now();
            do_math_v2<5>(bodies, 0.1);
            const auto t1 = high_resolution_clock::now();
            const auto dt = duration_cast<nanoseconds>(t1 - t0).count();
            std::cout << "Elapsed: " << dt / 1.0e6 << " [ms]" << std::endl;
        }

        {
            const auto t0 = high_resolution_clock::now();
            do_math_v2<6>(bodies, 0.1);
            const auto t1 = high_resolution_clock::now();
            const auto dt = duration_cast<nanoseconds>(t1 - t0).count();
            std::cout << "Elapsed: " << dt / 1.0e6 << " [ms]" << std::endl;
        }
        std::cout << "--------------------------------------" << std::endl;
    }


    return 0;
}