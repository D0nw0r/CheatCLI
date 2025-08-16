
set(CHEATCLI_SOURCES
    src/main.cpp
    src/api.cpp
)



add_executable(CheatCLI ${CHEATCLI_SOURCES})

# Compiler options for clang++ on Windows
target_compile_options(CheatCLI PRIVATE
    -fms-extensions
    -fms-compatibility
    -Wno-microsoft
)

# Windows definitions  
target_compile_definitions(CheatCLI PRIVATE
    WIN32_LEAN_AND_MEAN
    NOMINMAX
    _WIN32_WINNT=0x0601
    UNICODE
    _UNICODE
)

# Link Windows libraries
target_link_libraries(CheatCLI PRIVATE
    kernel32
    psapi
)
