add_rules("mode.debug", "mode.release")

add_repositories("liteldev-repo https://github.com/LiteLDev/xmake-repo.git")

if is_config("target_type", "server") then
    add_requires("levilamina 26.20.0", {configs = {target_type = "server"}})
else
    add_requires("levilamina 26.20.0", {configs = {target_type = "client"}})
end

add_requires("levibuildscript")

if not has_config("vs_runtime") then
    set_runtimes("MD")
end

option("target_type")
    set_default("server")
    set_showmenu(true)
    set_values("server", "client")
option_end()

target("PlaceholderApi") -- Change this to your mod name.
    add_rules("@levibuildscript/linkrule")
    add_rules("@levibuildscript/modpacker")

    if is_plat("windows") then
        add_defines("NOMINMAX", "UNICODE")
        set_exceptions("none") -- To avoid conflicts with /EHa.
        add_cxflags( "/EHa", "/utf-8", "/W4", "/w44265", "/w44289", "/w44296", "/w45263", "/w44738", "/w45204")
        add_cxflags(
            "/EHs",
            "-Wno-microsoft-cast",
            "-Wno-invalid-offsetof",
            "-Wno-c++2b-extensions",
            "-Wno-microsoft-include",
            "-Wno-overloaded-virtual",
            "-Wno-ignored-qualifiers",
            "-Wno-missing-field-initializers",
            "-Wno-potentially-evaluated-expression",
            "-Wno-pragma-system-header-outside-header",
            {tools = {"clang_cl"}}
        )
        set_toolchains("clang-cl")
    end

    add_packages("levilamina")

    set_languages("c++20")
    if is_windows then
        add_defines("_HAS_CXX23=1")
    else
        add_defines("_LIBCPP_STD_VER=23")
        add_cxxflags("clang::-stdlib=libc++")
    end

    add_defines("PLACEHOLDERAPI_EXPORT")
    set_symbols("debug")
    set_kind("shared")

    add_files("src/**.cpp")
    add_includedirs("src")

    if is_config("target_type", "server") then
        add_defines("LL_PLAT_S")
    else
        add_defines("LL_PLAT_C")
    end
    
    after_build(function (target)
        local binDirectory = path.join(os.projectdir(), "bin")
        local includeDirectory = path.join(path.join(binDirectory, "include"), "placeholder_api")
        local libDirectory = path.join(binDirectory, "lib")

        os.mkdir(includeDirectory)
        os.mkdir(libDirectory)

        os.cp(path.join(os.projectdir(), "src", "mod", "core", "Api.h"), includeDirectory)
        os.cp(path.join(target:targetdir(), "PlaceholderApi.lib"), libDirectory)
    end)
