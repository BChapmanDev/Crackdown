// crackdown - ReXGlue Recompiled Project

#include "generated/default/crackdown_init.h"

#include <rex/rex_app.h>
#include <rex/filesystem.h>

#include "cache.h"

REXCVAR_DEFINE_BOOL(fix_lighting, false, "Game Enhancements", "Fix lighting calculation, currently halves FPS but improves visibility and color/brightness accuracy.");
REXCVAR_DEFINE_BOOL(fix_light_occlusion, true, "Game Enhancements", "Fix light source coronas being visible through walls.");
REXCVAR_DEFINE_BOOL(misc_performance_improvements, false, "Game Enhancements", "Disables bloom and shadows to potentially improve performance.");
REXCVAR_DEFINE_BOOL(show_perfgraph, false, "Game Enhancements", "Show FPS counter in the top right and performance graph in the left half of the screen.");

class CrackdownApp : public rex::ReXApp {
public:
    using rex::ReXApp::ReXApp;

    static std::unique_ptr<rex::ui::WindowedApp> Create(
        rex::ui::WindowedAppContext& ctx) {
        return std::unique_ptr<CrackdownApp>(new CrackdownApp(ctx, "crackdown",
            PPCImageConfig));
    }

    void OnConfigurePaths(rex::PathConfig& paths) override
    {
        // Support launching directly from the CMake build directory.
        // An explicit --game_data_root always takes precedence.
        const auto exe_dir = rex::filesystem::GetExecutableFolder();
        const auto source_assets = (exe_dir / "../../../assets").lexically_normal();
        if (paths.game_data_root.empty()) {
            const auto packaged_assets = exe_dir / "assets";
            if (std::filesystem::exists(packaged_assets / "default.xex")) {
                paths.game_data_root = packaged_assets;
            } else if (std::filesystem::exists(source_assets / "default.xex")) {
                paths.game_data_root = source_assets;
            }
        }
    }

    void OnPreSetup(rex::RuntimeConfig& config) override
    {
        // 0.10 separates guest GPU emulation from the shared runtime.
        if (config.gpu_plugin.empty()) config.gpu_plugin = "xenos";
    }

    void OnPostSetup() override
    {
        if (REXCVAR_GET(fix_lighting))
        {
            // Fix lighting (https://github.com/xenia-canary/game-compatibility/issues/102#issuecomment-2385029628)
            rex::cvar::SetFlagByName("d3d12_readback_resolve", "true");
        }

        // Fix invalid fetch constant type error
        rex::cvar::SetFlagByName("gpu_allow_invalid_fetch_constants", "true");

        // Fix infinite loading screen (https://github.com/xenia-canary/game-compatibility/issues/102#issuecomment-1222126375)
        auto file_system = runtime()->file_system();

        auto cache_device = std::make_unique<CacheDevice>("\\CACHE");
        if (!cache_device->Initialize()) {
            REXLOG_ERROR("Unable to initialize cache device.");
        }
        else {
            if (!file_system->RegisterDevice(std::move(cache_device))) {
                REXLOG_ERROR("Unable to register cache device.");
            }
            else {
                file_system->RegisterSymbolicLink("cache:", "\\CACHE");
            }
        }

        // Below memory writes are originally made by Adrian (https://github.com/xenia-canary/game-patches/blob/main/patches/4D5307DC%20-%20Crackdown%20%28TU0%29.patch.toml)
        auto membase = rex::system::kernel_state()->memory()->virtual_membase();

        if (REXCVAR_GET(fix_light_occlusion))
        {
            // Fix seeing lights through walls by disabling them.
            *reinterpret_cast<bool*>(membase + 0x82BAA3AD) = false; // "togglegloballights" command.
        }

        if (REXCVAR_GET(show_perfgraph))
        {
            *reinterpret_cast<bool*>(membase + 0x82DE4F14) = true; // "fps" command.
            *reinterpret_cast<bool*>(membase + 0x82DE4D1A) = true; // "perfgraph" command.
            *reinterpret_cast<bool*>(membase + 0x82DE25A9) = true; // Sets fps/perfgraph to have red text.
        }

        if (REXCVAR_GET(misc_performance_improvements))
        {
            // Performance-improving commands
            *reinterpret_cast<bool*>(membase + 0x82BAA3AA) = true; // "togglebloom" command.
            *reinterpret_cast<bool*>(membase + 0x82DE25B3) = false; // Disable shadows.
        }
    }
};

REX_DEFINE_APP(crackdown, CrackdownApp::Create)
