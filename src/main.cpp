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

        std::string error;
        if(!engine.Init(isFullscreen, windowMultiplier, error)) {
            spdlog::error("Engine initialization error: {}", error);
            return false;
        }

        if(!editor.Init(error)) {
            spdlog::error("Editor initialization error: {}", error);
            return false;
        }

        engine.Run([&editor](){
            editor.Render();
        });

        editor.Destroy();
    } catch(const std::exception& e) {
        spdlog::error("Runtime exception: {}", e.what());
        return false;
    }

    return true;
}

int main() {
    return run(false, 0.8f, spdlog::level::debug, true) ? EXIT_SUCCESS : EXIT_FAILURE;
}
