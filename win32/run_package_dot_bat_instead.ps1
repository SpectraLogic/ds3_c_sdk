# Build the application.
cmake .. -G "Visual Studio 14 2015"
msbuild libds3.sln /p:Configuration=Release

# Copy dependencies into output directory
Copy-Item deps\install -Destination output -Recurse -force

Copy-Item src\Release\* -Destination output\bin -force
Copy-Item src\Release\ds3.lib -Destination output\lib\ds3.lib

Copy-Item ..\src\ds3.h output\include
Copy-Item ..\src\ds3_string.h output\include
Copy-Item ..\src\ds3_string_multimap.h output\include

# Create a zip file with the application contents.
[Reflection.Assembly]::LoadWithPartialName("System.IO.Compression.FileSystem")
[System.IO.Compression.ZipFile]::CreateFromDirectory(
    "output",
    "ds3_win32.zip",
    [System.IO.Compression.CompressionLevel]::Optimal,
    $false
)

# Delete the build.
Remove-Item -Recurse -Force output

