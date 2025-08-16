Write-Host "Recreating the build dir"
Remove-Item -Path ./build -Force -Recurse

Write-Host "Rebuilding cmake"

cmake -B build -G Ninja -DCMAKE_CXX_COMPILER=clang++
ninja -C build
