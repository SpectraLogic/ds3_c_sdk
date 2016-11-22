# Unzip the 32 bit libs for linking

# Build the application.
cmake .. -G "Visual Studio 14 2015"
msbuild libds3.sln

# Create a zip file with the application contents.
[Reflection.Assembly]::LoadWithPartialName("System.IO.Compression.FileSystem")
[System.IO.Compression.ZipFile]::CreateFromDirectory(
    "ds3",
    "ds3.zip",
    [System.IO.Compression.CompressionLevel]::Optimal,
    $true
)

# Delete the build.
Remove-Item -Recurse -Force obj
Remove-Item -Recurse -Force ds3

