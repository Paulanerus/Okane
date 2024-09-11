workspace "Okane"
    configurations {"Debug", "Release"}

project "Okane"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    architecture "x86_64"
    targetdir "bin/%{cfg.buildcfg}"
    location "build/"

    warnings "Extra"

    includedirs {"libs/rang/include", "include/"}

    files {"src/**.hpp", "src/**.cpp"}

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"