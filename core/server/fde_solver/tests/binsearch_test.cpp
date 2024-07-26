#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <math_utils.hpp>
#include <functional>
#include <random>
#include <bitset>

struct BinarySearchBase {
    virtual std::vector<usize> Prepare(std::vector<f64>&) const = 0;
    virtual std::ptrdiff_t Search(const f64*, const f64*, f64, const std::vector<usize>&) const = 0;
};

struct BinarySearch1 : BinarySearchBase {
    std::vector<usize> Prepare(std::vector<f64>& data) const override { return {}; }

    std::ptrdiff_t Search(const f64* begin, const f64* end, f64 rnd, const std::vector<usize>&) const override {
        return std::lower_bound(begin, end, rnd) - begin;
    }
};

struct BinarySearch2 : BinarySearchBase {
    std::vector<usize> Prepare(std::vector<f64>& data) const override { return {}; }

    std::ptrdiff_t Search(const f64* begin, const f64* end, f64 rnd, const std::vector<usize>&) const override {
        std::ptrdiff_t l = 0, r = end - begin - 1;

        while (l < r) {
            std::ptrdiff_t m = l + (r - l) / 2;

            if (begin[m] >= rnd) {
                r = m;
            } else {
                l = m + 1;
            }
        }

        return begin[l] >= rnd ? l : l + 1;
    }
};

struct BinarySearch3 : BinarySearchBase {
    std::vector<usize> Prepare(std::vector<f64>& data) const override { return {}; }

    std::ptrdiff_t Search(const f64* begin, const f64* end, f64 rnd, const std::vector<usize>&) const override {
        f64* base = (f64*)(begin);
        std::ptrdiff_t len = end - begin;

        while (len > 1) {
            std::ptrdiff_t half = len / 2;

            if (base[half - 1] < rnd) {
                base += half;
                len = len - half;
            } else {
                len = half;
            }
        }

        return *base >= rnd ? base - begin : base - begin + 1;
    }
};

struct BinarySearch4 : BinarySearchBase {
    std::vector<usize> Prepare(std::vector<f64>& data) const override { return {}; }

    std::ptrdiff_t Search(const f64* begin, const f64* end, f64 rnd, const std::vector<usize>&) const override {
        f64* base = (f64*)(begin);
        std::ptrdiff_t len = end - begin;

        while (len > 1) {
            std::ptrdiff_t half = len / 2;

            if (base[half - 1] < rnd) {
                base += half;
            }
            len -= half;
        }

        return *base >= rnd ? base - begin : base - begin + 1;
    }
};

struct BinarySearch5 : BinarySearchBase {
    std::vector<usize> Prepare(std::vector<f64>& data) const override { return {}; }

    std::ptrdiff_t Search(const f64* begin, const f64* end, f64 rnd, const std::vector<usize>&) const override {
        f64* base = (f64*)(begin);
        std::ptrdiff_t len = end - begin;

        while (len > 1) {
            std::ptrdiff_t half = len / 2;
            base += (base[half - 1] < rnd) * half;
            len -= half;
        }

        return *base >= rnd ? base - begin : base - begin + 1;
    }
};

struct BinarySearch6 : BinarySearchBase {
    std::vector<usize> Prepare(std::vector<f64>& data) const override { return {}; }

    std::ptrdiff_t Search(const f64* begin, const f64* end, f64 rnd, const std::vector<usize>&) const override {
        f64* base = (f64*)(begin);
        std::ptrdiff_t len = end - begin;

        while (len > 1) {
            std::ptrdiff_t half = len / 2;
            len -= half;
            data_prefetch((const char*)(&base[len / 2 - 1]));
            data_prefetch((const char*)(&base[half + len / 2 - 1]));
            if (base[half - 1] < rnd) {
                base += half;
            }
        }

        return *base >= rnd ? base - begin : base - begin + 1;
    }
};

struct BinarySearch7 : BinarySearchBase {
    std::vector<usize> Prepare(std::vector<f64>& data) const override { return {}; }

    std::ptrdiff_t Search(const f64* begin, const f64* end, f64 rnd, const std::vector<usize>&) const override {
        f64* base = (f64*)(begin);
        std::ptrdiff_t len = end - begin;

        while (len > 1) {
            std::ptrdiff_t half = len / 2;
            len -= half;
            data_prefetch((const char*)(&base[len / 2 - 1]));
            data_prefetch((const char*)(&base[half + len / 2 - 1]));
            base += (base[half - 1] < rnd) * half;
        }

        return *base >= rnd ? base - begin : base - begin + 1;
    }
};

struct BinarySearch8 : BinarySearchBase {
    std::vector<usize> Prepare(std::vector<f64>& data) const override {
        std::vector<f64> dataNew(data.size() + 1);
        std::vector<usize> help(data.size() + 1, data.size());
        Eytzinger(data, dataNew, help);
        data = std::move(dataNew);

        return help;
    }

    usize Eytzinger(const std::vector<f64>& data, std::vector<f64>& dataNew, std::vector<usize>& help, usize i = 0, usize k = 1) const {
        if (k <= data.size()) {
            i = Eytzinger(data, dataNew, help, i, 2 * k);
            help[k] = i;
            dataNew[k] = data[i++];
            i = Eytzinger(data, dataNew, help, i, 2 * k + 1);
        }
        return i;
    }

    std::ptrdiff_t Search(const f64* begin, const f64* end, f64 rnd, const std::vector<usize>& help) const override {
        usize k = 1;
        usize len = end - begin - 1;

        while (k <= len) {
            if (begin[k] < rnd) {
                k = k * 2 + 1;
            } else {
                k = k * 2;
            }
        }
        k >>= std::countr_zero(~k) + 1;
        return help[k];
    }
};

struct BinarySearch9 : BinarySearchBase {
    std::vector<usize> Prepare(std::vector<f64>& data) const override {
        std::vector<f64> dataNew(data.size() + 1);
        std::vector<usize> help(data.size() + 1, data.size());
        Eytzinger(data, dataNew, help);
        data = std::move(dataNew);

        return help;
    }

    usize Eytzinger(const std::vector<f64>& data, std::vector<f64>& dataNew, std::vector<usize>& help, usize i = 0, usize k = 1) const {
        if (k <= data.size()) {
            i = Eytzinger(data, dataNew, help, i, 2 * k);
            help[k] = i;
            dataNew[k] = data[i++];
            i = Eytzinger(data, dataNew, help, i, 2 * k + 1);
        }
        return i;
    }

    std::ptrdiff_t Search(const f64* begin, const f64* end, f64 rnd, const std::vector<usize>& help) const override {
        usize k = 1;
        usize len = end - begin - 1;

        while (k <= len) {
            k = (k << 1) + (begin[k] < rnd);
        }
        k >>= std::countr_zero(~k) + 1;
        return help[k];
    }
};

struct BinarySearch10 : BinarySearchBase {
    std::vector<usize> Prepare(std::vector<f64>& data) const override {
        std::vector<f64> dataNew(data.size() + 1);
        std::vector<usize> help(data.size() + 1, data.size());
        Eytzinger(data, dataNew, help);
        data = std::move(dataNew);

        return help;
    }

    usize Eytzinger(const std::vector<f64>& data, std::vector<f64>& dataNew, std::vector<usize>& help, usize i = 0, usize k = 1) const {
        if (k <= data.size()) {
            i = Eytzinger(data, dataNew, help, i, 2 * k);
            help[k] = i;
            dataNew[k] = data[i++];
            i = Eytzinger(data, dataNew, help, i, 2 * k + 1);
        }
        return i;
    }

    std::ptrdiff_t Search(const f64* begin, const f64* end, f64 rnd, const std::vector<usize>& help) const override {
        usize k = 1;
        usize len = end - begin - 1;

        while (k <= len) {
            data_prefetch((const char*)(begin + k * 16));
            k = (k << 1) + (begin[k] < rnd);
        }
        k >>= std::countr_zero(~k) + 1;
        return help[k];
    }
};

BinarySearch1 bs1;
BinarySearch2 bs2;
BinarySearch3 bs3;
BinarySearch4 bs4;
BinarySearch5 bs5;
BinarySearch6 bs6;
BinarySearch7 bs7;
BinarySearch8 bs8;
BinarySearch9 bs9;
BinarySearch10 bs10;

namespace {
    const std::map<const char*, BinarySearchBase*> gNAME_TO_METHOD = {
        {"std::lower_bound", &bs1},
        {"BinSearch standart", &bs2},
        {"BinSearch stl like 1", &bs3},
        {"BinSearch stl like 2", &bs4},
        {"BinSearch stl like 2 cmov", &bs5},
        {"BinSearch stl like 2 + prefetch", &bs6},
        {"BinSearch stl like 2 cmov + prefetch", &bs7},
        {"BinSearch Eytzinger", &bs8},
        {"BinSearch Eytzinger cmov", &bs9},
        {"BinSearch Eytzinger cmov + prefetch", &bs10},
    };

    const usize gSIZES[] = {4, 8, 16, 32, 64, 128, 256, 512, 1024, 8192, 2097152};
}

struct TTestData {
    const std::vector<f64>& Data;
    const std::vector<usize>& Help;
    std::string MethodName;
    const BinarySearchBase& BinarySearch;
    usize VectorSize;
};

void RunTest(const TTestData& testData) {
    std::random_device device;
    std::minstd_rand0 engine(device());
    std::uniform_real_distribution<f64> generator(0.0, 1.0);

    BENCHMARK_ADVANCED(testData.MethodName.c_str())(Catch::Benchmark::Chronometer meter) {
        std::vector<f64> rnds(meter.runs());
        for (f64& i : rnds) {
            i = generator(engine);
        }
        meter.measure([&] (int i) {
            return testData.BinarySearch.Search(&(*testData.Data.begin()), &(*testData.Data.end()), rnds[i], testData.Help); 
        });
    };
}


TEST_CASE("Binary Search Impl Tests", "[binsearch]") {
    for (const auto [name, method] : gNAME_TO_METHOD) {
        SECTION (std::format("{0} Test 1", name)) {
            std::vector<f64> v = {0.2};
            std::vector<usize> help = method->Prepare(v);

            REQUIRE(method->Search(&(*v.begin()), &(*v.end()), 0.0, help) == 0);
            REQUIRE(method->Search(&(*v.begin()), &(*v.end()), 0.2, help) == 0);
            REQUIRE(method->Search(&(*v.begin()), &(*v.end()), 0.3, help) == 1);
        }

        SECTION (std::format("{0} Test 2", name)) {
            std::vector<f64> v = {0.2, 0.5};
            std::vector<usize> help = method->Prepare(v);

            REQUIRE(method->Search(&(*v.begin()), &(*v.end()), 0.0, help) == 0);
            REQUIRE(method->Search(&(*v.begin()), &(*v.end()), 0.2, help) == 0);
            REQUIRE(method->Search(&(*v.begin()), &(*v.end()), 0.3, help) == 1);
            REQUIRE(method->Search(&(*v.begin()), &(*v.end()), 0.5, help) == 1);
            REQUIRE(method->Search(&(*v.begin()), &(*v.end()), 0.6, help) == 2);
        }

        SECTION (std::format("{0} Test 3", name)) {
            std::vector<f64> v = {0.2, 0.5, 0.7, 0.9, 1.0};
            std::vector<usize> help = method->Prepare(v);

            REQUIRE(method->Search(&(*v.begin()), &(*v.end()), 0.0, help) == 0);
            REQUIRE(method->Search(&(*v.begin()), &(*v.end()), 0.2, help) == 0);
            REQUIRE(method->Search(&(*v.begin()), &(*v.end()), 0.6, help) == 2);
            REQUIRE(method->Search(&(*v.begin()), &(*v.end()), 0.7, help) == 2);
            REQUIRE(method->Search(&(*v.begin()), &(*v.end()), 1.0, help) == 4);
        }

        SECTION (std::format("{0} Test 4", name)) {
            std::vector<f64> v = {0.2, 0.2, 0.5, 0.7, 0.9, 1.0, 1.0, 1.0};
            std::vector<usize> help = method->Prepare(v);

            REQUIRE(method->Search(&(*v.begin()), &(*v.end()), 0.0, help) == 0);
            REQUIRE(method->Search(&(*v.begin()), &(*v.end()), 0.2, help) == 0);
            REQUIRE(method->Search(&(*v.begin()), &(*v.end()), 0.6, help) == 3);
            REQUIRE(method->Search(&(*v.begin()), &(*v.end()), 0.7, help) == 3);
            REQUIRE(method->Search(&(*v.begin()), &(*v.end()), 1.0, help) == 5);
            REQUIRE(method->Search(&(*v.begin()), &(*v.end()), 1.1, help) == 8);
        }

        SECTION (std::format("{0} Random 1000", name)) {
            std::random_device device;
            std::minstd_rand0 engine(device());
            std::uniform_real_distribution<f64> generator(0.0, 1.0);
            std::vector<f64> v(10);
            for (usize i = 0; i < v.size(); i++) {
                v[i] = generator(engine);
            }
            std::sort(v.begin(), v.end());
            std::vector<f64> vOld = v;
            std::vector<usize> help = method->Prepare(v);

            for (usize i = 0; i < 1; i++) {
                f64 rnd = generator(engine);
                auto t1 = method->Search(&(*v.begin()), &(*v.end()), rnd, help);
                auto t2 = std::lower_bound(vOld.begin(), vOld.end(), rnd) - vOld.begin();
                if (t1 != t2) {
                    std::cout << "vOld: ";
                    for (f64 i : vOld) {
                        std::cout << i << " ";
                    }
                    std::cout << std::endl;
                    std::cout << "v: ";
                    for (f64 i : v) {
                        std::cout << i << " ";
                    }
                    std::cout << std::endl;
                    std::cout << std::format("{} != {}", t1, t2) << std::endl;
                    std::cout << rnd << std::endl;

                    FAIL("Wrong answer");
                } else {
                    SUCCEED("Right answer");
                }
            }
        }
    }
}


TEST_CASE("Binary Search Impl Benchmark", "[binsearch, benchmark]") {
    std::random_device device;
    std::minstd_rand0 engine(device());
    std::uniform_real_distribution<f64> generator(0.0, 1.0);

    auto createData = [&] (usize size) -> std::vector<f64> {
        std::vector<f64> v(size);
        for (usize i = 0; i < size; i++) {
            v[i] = generator(engine);
        }
        std::sort(v.begin(), v.end());
        return v;
    };

    for (auto size : gSIZES) {
        const std::vector<f64> v = createData(size);

        SECTION (std::format("Size: {}", size)) {
            for (const auto [name, method] : gNAME_TO_METHOD) {
                auto vNew = v;
                auto help = method->Prepare(vNew);
                RunTest(TTestData{
                    .Data = vNew,
                    .Help = help,
                    .MethodName = std::format("{0}", name),
                    .BinarySearch = *method,
                    .VectorSize = size,
                });
            }
        }
    }
}
