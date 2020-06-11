
rd /Q /S vs_sln 
md vs_sln
cd vs_sln
cmake ../ -DCMAKE_BUILD_TYPE=Release -A x64 -DCMAKE_GENERATOR_PLATFORM=x64 
MSBuild log4z.sln /property:Configuration=Release /property:Platform=x64
mv ../lib/Release/* ../lib/
rd /Q /S  "../lib/Release" 
cmake ../ -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 16 2019" -A x64 -DCMAKE_GENERATOR_PLATFORM=x64
MSBuild log4z.sln /property:Configuration=Debug /property:Platform=x64
mv ../lib/Debug/* ../lib/
rd /Q /S  "../lib/Debug"
cd ..