#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "editor/editor.h"


static bool run(bool isFullscreen, float windowMultiplier, spdlog::level::level_enum logLevel, bool logToFile) {
    spdlog::set_level(logLevel);
    if (logToFile) {
        auto file_logger = spdlog::basic_logger_mt("basic_logger", "rtge.log");
        spdlog::set_default_logger(file_logger);
    }

    try {
        Engine engine;
        Editor editor(engine);

        try {
            engine.Create(isFullscreen, windowMultiplier);
        } catch(const std::exception& e) {
            spdlog::error("Engine initialization error: {}", e.what());
            return false;
        }

        try {
            editor.Create();
        } catch(const std::exception& e) {
            spdlog::error("Editor initialization error: {}", e.what());
            return false;
        }

        engine.Run(
            [&editor](float deltaTime){
                editor.Update(deltaTime);
            },
            [&editor](){
                editor.Draw();
            }
        );

        editor.Destroy();
    } catch(const std::exception& e) {
        spdlog::error("Runtime exception: {}", e.what());
        return false;
    } catch(...) {
        spdlog::error("Unhandled exception");
        return false;
    }

    return true;
}

int main() {
    return run(false, 0.8f, spdlog::level::debug, false) ? EXIT_SUCCESS : EXIT_FAILURE;
}
