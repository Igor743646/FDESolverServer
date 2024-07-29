#pragma once

#ifdef _WIN32
    #define _WIN32_WINNT 0x0601
#endif

#include <math_expression_calculator.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <config.pb.h> // PFDESolver::TClientConfig
#include <result.pb.h> // PFDESolver::TResults
#include <mfdes.hpp>
#include <sfdes.hpp>

namespace NFDESolverService {

    using NEquationSolver::IEquationSolver;
    using NEquationSolver::TSolverConfigBase;
    using NEquationSolver::TParsedSolverConfig;
    using NEquationSolver::TSolverConfig;
    using NEquationSolver::TMatrixFDES;
    using NEquationSolver::TMFDESRule;
    using NEquationSolver::TStochasticFDES;
    using NEquationSolver::TRLFDESRule;

    struct IMethodBuilder {
        constexpr IMethodBuilder() = default;
        virtual ~IMethodBuilder() = default;
        virtual usize Number() const = 0;
        virtual std::unique_ptr<IEquationSolver> GetMethod(const TSolverConfig&) const = 0;
        virtual std::unique_ptr<IEquationSolver> GetMethod(TSolverConfig&&) const = 0;
    };

    template<class TMethod>
    struct TMethodBuilder final : public IMethodBuilder {
        const usize SeqNumber;

        constexpr explicit TMethodBuilder(usize seqNumber) : IMethodBuilder(), SeqNumber(seqNumber) {}

        usize Number() const override {
            return SeqNumber;
        }

        std::unique_ptr<IEquationSolver> GetMethod(const TSolverConfig& config) const override {
            return std::make_unique<TMethod>(config);
        }

        std::unique_ptr<IEquationSolver> GetMethod(TSolverConfig&& config) const override {
            return std::make_unique<TMethod>(std::move(config));
        }
    };

    class TFDESolverService {

        using TClientConfig = ::PFDESolver::TClientConfig;
        using TResults = ::PFDESolver::TResults;
        using TSolvers = std::array<std::shared_ptr<IEquationSolver>, NEquationSolver::MethodsCount>;

    public:

        TFDESolverService() : ThreadPool(4) {
            Methods.insert({"MGL", std::make_unique<TMethodBuilder<TMatrixFDES<TMFDESRule>>>(0)});
            Methods.insert({"MRL", std::make_unique<TMethodBuilder<TMatrixFDES<TRLFDESRule>>>(1)});
            Methods.insert({"SGL", std::make_unique<TMethodBuilder<TStochasticFDES<TMFDESRule>>>(2)});
            Methods.insert({"SRL", std::make_unique<TMethodBuilder<TStochasticFDES<TRLFDESRule>>>(3)});
        }

        void DoRunTask(const TClientConfig*, TResults*);

    private:

        void SolveTask(IEquationSolver&, PFDESolver::TResult&, bool = true);
        void AddConfig(const TSolverConfig& config, TResults& response);
        void AddRealSolution(const TSolverConfig&, TResults&);

        template<std::convertible_to<std::string>... Args>
        auto GetFunction(std::string, const Args...);
        TParsedSolverConfig ParseClientConfig(const TClientConfig&);
        TSolvers ParseSolveMethods(const TClientConfig&, const TSolverConfig&);

    private:
        std::unordered_map<std::string, std::unique_ptr<IMethodBuilder>> Methods;
        boost::asio::thread_pool ThreadPool;
    };
}
