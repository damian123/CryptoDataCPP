#include <gtest/gtest.h>
#include <chrono>
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <StreamingMarketData.h>

class Environment : public ::testing::Environment {
public:
    ~Environment() override {}

    // Override this to define how to set up the environment.
    void SetUp() override 
    {
        spdlog::init_thread_pool(8192, 2);
        {            
            auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt >();
            std::vector<spdlog::sink_ptr> sinks{ stdout_sink };
            auto logger = std::make_shared<spdlog::async_logger>("CryptoData", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
            logger->flush_on(spdlog::level::err);   // trigger flush whenever errors or more severe messages are logged
            logger->set_level(spdlog::level::debug);
            spdlog::register_logger(logger);
        }

        {            
            auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("ftx_test_log.txt", 1024 * 1024 * 10, 3);
            std::vector<spdlog::sink_ptr> sinks{ rotating_sink };
            auto logger = std::make_shared<spdlog::async_logger>("ftx", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
            logger->flush_on(spdlog::level::err);   // trigger flush whenever errors or more severe messages are logged
            logger->set_level(spdlog::level::debug);
            spdlog::register_logger(logger);                       
        }

        try {            
			StreamingMarketData::getInstance().SecID("BTC/USD");
			StreamingMarketData::getInstance().start();
        }
        catch (const std::exception& ex)
        {
            spdlog::get("CryptoData")->error(fmt::format("Failed to setup the MarketDataFeedTest: {}", ex.what()));
            FAIL();
        }
    }

    // Override this to define how to tear down the environment.
    void TearDown() override 
    {
        using namespace std::chrono_literals;

        StreamingMarketData::getInstance().stop();
        std::this_thread::sleep_for(0.5s);  // wait bit for the feed to shut down.
    }
};

int main(int argc, char** argv) 
{
    try {
        testing::Environment* const foo_env = testing::AddGlobalTestEnvironment(new Environment);
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();    
    }
    catch (std::exception const& ex) {
        std::cout << ex.what() << std::endl;
        // perhaps log the message, ex.what()
    }
}
