#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <math_utils.hpp>
#include <functional>
#include <random>
#include <bitset>

struct IBinarySearchBase {
    IBinarySearchBase() = default;
    IBinarySearchBase(const IBinarySearchBase&) = default;
    IBinarySearchBase(IBinarySearchBase&&) = default;
    IBinarySearchBase& operator=(const IBinarySearchBase&) = default;
    IBinarySearchBase& operator=(IBinarySearchBase&&) = default;
    virtual ~IBinarySearchBase() = default;
    virtual std::vector<usize> Prepare(std::vector<f64>&) const = 0;
    virtual std::ptrdiff_t Search(const f64*, const f64*, f64, const std::vector<usize>&) const = 0;
};

struct TBinarySearch1 : IBinarySearchBase {
    std::vector<usize> Prepare(std::vector<f64>& data) const override { return {}; }

    std::ptrdiff_t Search(const f64* begin, const f64* end, f64 rnd, const std::vector<usize>&) const override {
        return std::lower_bound(begin, end, rnd) - begin;
    }
};

struct TBinarySearch2 : IBinarySearchBase {
    std::vector<usize> Prepare(std::vector<f64>& data) const override { return {}; }

    std::ptrdiff_t Search(const f64* begin, const f64* end, f64 rnd, const std::vector<usize>&) const override {
        std::ptrdiff_t left = 0;
        std::ptrdiff_t right = end - begin - 1;

        while (left < right) {
            std::ptrdiff_t mid = left + (right - left) / 2;

            if (begin[mid] >= rnd) {
                right = mid;
            } else {
                left = mid + 1;
            }
        }

        return begin[left] >= rnd ? left : left + 1;
    }
};

struct TBinarySearch3 : IBinarySearchBase {
    std::vector<usize> Prepare(std::vector<f64>& data) const override { return {}; }

    std::ptrdiff_t Search(const f64* begin, const f64* end, f64 rnd, const std::vector<usize>&) const override {
        std::span base{begin, end};
        std::ptrdiff_t len = std::distance(begin, end);

        while (len > 1) {
            std::ptrdiff_t half = len / 2;

            if (base[half - 1] < rnd) {
                base = base.subspan(half);
                len = len - half;
            } else {
                len = half;
            }
        }

        return std::distance(begin, base.data()) + (base.front() >= rnd ? 0 : 1);
    }
};

struct TBinarySearch4 : IBinarySearchBase {
    std::vector<usize> Prepare(std::vector<f64>& data) const override { return {}; }

    std::ptrdiff_t Search(const f64* begin, const f64* end, f64 rnd, const std::vector<usize>&) const override {
        std::span base{begin, end};
        std::ptrdiff_t len = std::distance(begin, end);

        while (len > 1) {
            std::ptrdiff_t half = len / 2;

            if (base[half - 1] < rnd) {
                base = base.subspan(half);
            }
            len -= half;
        }

        return std::distance(begin, base.data()) + (base.front() >= rnd ? 0 : 1);
    }
};

struct TBinarySearch5 : IBinarySearchBase {
    std::vector<usize> Prepare(std::vector<f64>& data) const override { return {}; }

    std::ptrdiff_t Search(const f64* begin, const f64* end, f64 rnd, const std::vector<usize>&) const override {
        std::span base{begin, end};
        std::ptrdiff_t len = std::distance(begin, end);

        while (len > 1) {
            std::ptrdiff_t half = len / 2;
            base = base.subspan(static_cast<std::ptrdiff_t>(base[half - 1] < rnd) * half);
            len -= half;
        }

        return std::distance(begin, base.data()) + (base.front() >= rnd ? 0 : 1);
    }
};

struct TBinarySearch6 : IBinarySearchBase {
    std::vector<usize> Prepare(std::vector<f64>& data) const override { return {}; }

    std::ptrdiff_t Search(const f64* begin, const f64* end, f64 rnd, const std::vector<usize>&) const override {
        std::span base{begin, end};
        std::ptrdiff_t len = std::distance(begin, end);

        while (len > 1) {
            std::ptrdiff_t half = len / 2;
            len -= half;
            data_prefetch(reinterpret_cast<const char*>(&base[len / 2 - 1]));
            data_prefetch(reinterpret_cast<const char*>(&base[half + len / 2 - 1]));
            if (base[half - 1] < rnd) {
                base = base.subspan(half);
            }
        }

        return std::distance(begin, base.data()) + (base.front() >= rnd ? 0 : 1);
    }
};

struct TBinarySearch7 : IBinarySearchBase {
    std::vector<usize> Prepare(std::vector<f64>& data) const override { return {}; }

    std::ptrdiff_t Search(const f64* begin, const f64* end, f64 rnd, const std::vector<usize>&) const override {
        std::span base{begin, end};
        std::ptrdiff_t len = std::distance(begin, end);

        while (len > 1) {
            std::ptrdiff_t half = len / 2;
            len -= half;
            data_prefetch(reinterpret_cast<const char*>(&base[len / 2 - 1]));
            data_prefetch(reinterpret_cast<const char*>(&base[half + len / 2 - 1]));
            base = base.subspan(static_cast<std::ptrdiff_t>(base[half - 1] < rnd) * half);
        }

        return std::distance(begin, base.data()) + (base.front() >= rnd ? 0 : 1);
    }
};

struct TBinarySearch8 : IBinarySearchBase {
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
        return static_cast<std::ptrdiff_t>(help[k]);
    }
};

struct TBinarySearch9 : IBinarySearchBase {
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
            k = (k << 1) + static_cast<usize>(begin[k] < rnd);
        }
        k >>= std::countr_zero(~k) + 1;
        return static_cast<std::ptrdiff_t>(help[k]);
    }
};

struct TBinarySearch10 : IBinarySearchBase {
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
            data_prefetch(reinterpret_cast<const char*>(begin + k * 16));
            k = (k << 1) + static_cast<usize>(begin[k] < rnd);
        }
        k >>= std::countr_zero(~k) + 1;
        return static_cast<std::ptrdiff_t>(help[k]);
    }
};

const TBinarySearch1 gBS1;
const TBinarySearch2 gBS2;
const TBinarySearch3 gBS3;
const TBinarySearch4 gBS4;
const TBinarySearch5 gBS5;
const TBinarySearch6 gBS6;
const TBinarySearch7 gBS7;
const TBinarySearch8 gBS8;
const TBinarySearch9 gBS9;
const TBinarySearch10 gBS10;

namespace {
    const std::map<const char*, const IBinarySearchBase*> gNAME_TO_METHOD = {
        {"std::lower_bound", &gBS1},
        {"BinSearch standart", &gBS2},
        {"BinSearch stl like 1", &gBS3},
        {"BinSearch stl like 2", &gBS4},
        {"BinSearch stl like 2 cmov", &gBS5},
        {"BinSearch stl like 2 + prefetch", &gBS6},
        {"BinSearch stl like 2 cmov + prefetch", &gBS7},
        {"BinSearch Eytzinger", &gBS8},
        {"BinSearch Eytzinger cmov", &gBS9},
        {"BinSearch Eytzinger cmov + prefetch", &gBS10},
    };

    const std::array gSIZES = {4ULL, 8ULL, 16ULL, 32ULL, 64ULL, 128ULL, 256ULL, 512ULL, 1024ULL, 8192ULL, 2097152ULL};
}  // namespace

struct TTestData {
    const std::vector<f64>* Data;
    const std::vector<usize>* Help;
    std::string MethodName;
    const IBinarySearchBase* TBinarySearch;
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
            return testData.TBinarySearch->Search(&(*testData.Data->begin()), &(*testData.Data->end()), rnds[i], *testData.Help); 
        });
    };
}


TEST_CASE("Binary Search Impl Tests", "[binsearch]") {
    for (const auto [name, method] : gNAME_TO_METHOD) {
        SECTION (std::format("{0} Test 1", name)) {
            std::vector<f64> data = {0.2};
            std::vector<usize> help = method->Prepare(data);

            REQUIRE(method->Search(&(*data.begin()), &(*data.end()), 0.0, help) == 0);
            REQUIRE(method->Search(&(*data.begin()), &(*data.end()), 0.2, help) == 0);
            REQUIRE(method->Search(&(*data.begin()), &(*data.end()), 0.3, help) == 1);
        }

        SECTION (std::format("{0} Test 2", name)) {
            std::vector<f64> data = {0.2, 0.5};
            std::vector<usize> help = method->Prepare(data);

            REQUIRE(method->Search(&(*data.begin()), &(*data.end()), 0.0, help) == 0);
            REQUIRE(method->Search(&(*data.begin()), &(*data.end()), 0.2, help) == 0);
            REQUIRE(method->Search(&(*data.begin()), &(*data.end()), 0.3, help) == 1);
            REQUIRE(method->Search(&(*data.begin()), &(*data.end()), 0.5, help) == 1);
            REQUIRE(method->Search(&(*data.begin()), &(*data.end()), 0.6, help) == 2);
        }

        SECTION (std::format("{0} Test 3", name)) {
            std::vector<f64> data = {0.2, 0.5, 0.7, 0.9, 1.0};
            std::vector<usize> help = method->Prepare(data);

            REQUIRE(method->Search(&(*data.begin()), &(*data.end()), 0.0, help) == 0);
            REQUIRE(method->Search(&(*data.begin()), &(*data.end()), 0.2, help) == 0);
            REQUIRE(method->Search(&(*data.begin()), &(*data.end()), 0.6, help) == 2);
            REQUIRE(method->Search(&(*data.begin()), &(*data.end()), 0.7, help) == 2);
            REQUIRE(method->Search(&(*data.begin()), &(*data.end()), 1.0, help) == 4);
        }

        SECTION (std::format("{0} Test 4", name)) {
            std::vector<f64> data = {0.2, 0.2, 0.5, 0.7, 0.9, 1.0, 1.0, 1.0};
            std::vector<usize> help = method->Prepare(data);

            REQUIRE(method->Search(&(*data.begin()), &(*data.end()), 0.0, help) == 0);
            REQUIRE(method->Search(&(*data.begin()), &(*data.end()), 0.2, help) == 0);
            REQUIRE(method->Search(&(*data.begin()), &(*data.end()), 0.6, help) == 3);
            REQUIRE(method->Search(&(*data.begin()), &(*data.end()), 0.7, help) == 3);
            REQUIRE(method->Search(&(*data.begin()), &(*data.end()), 1.0, help) == 5);
            REQUIRE(method->Search(&(*data.begin()), &(*data.end()), 1.1, help) == 8);
        }

        SECTION (std::format("{0} Random 1000", name)) {
            std::random_device device;
            std::minstd_rand0 engine(device());
            std::uniform_real_distribution<f64> generator(0.0, 1.0);
            const usize vSize = 1000;
            std::vector<f64> data(vSize);
            for (double & i : data) {
                i = generator(engine);
            }
            std::sort(data.begin(), data.end());
            std::vector<f64> vOld = data;
            std::vector<usize> help = method->Prepare(data);

            for (usize i = 0; i < 1; i++) {
                f64 rnd = generator(engine);
                auto res1 = method->Search(&(*data.begin()), &(*data.end()), rnd, help);
                auto res2 = std::lower_bound(vOld.begin(), vOld.end(), rnd) - vOld.begin();
                if (res1 != res2) {
                    std::cout << "vOld: ";
                    for (f64 i : vOld) {
                        std::cout << i << " ";
                    }
                    std::cout << '\n';
                    std::cout << "v: ";
                    for (f64 i : data) {
                        std::cout << i << " ";
                    }
                    std::cout << '\n';
                    std::cout << std::format("{} != {}", res1, res2) << '\n';
                    std::cout << rnd << '\n';

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
        std::vector<f64> data(size);
        for (usize i = 0; i < size; i++) {
            data[i] = generator(engine);
        }
        std::sort(data.begin(), data.end());
        return data;
    };

    for (auto size : gSIZES) {
        const std::vector<f64> data = createData(size);

        SECTION (std::format("Size: {}", size)) {
            for (const auto [name, method] : gNAME_TO_METHOD) {
                auto vNew = data;
                auto help = method->Prepare(vNew);
                RunTest(TTestData{
                    .Data = &vNew,
                    .Help = &help,
                    .MethodName = std::format("{0}", name),
                    .TBinarySearch = method,
                    .VectorSize = size,
                });
            }
        }
    }
}
