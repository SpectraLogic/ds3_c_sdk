# Build the application.
cmake .. -G "Visual Studio 14 2015 Win64"
msbuild libds3.sln

# Create a zip file with the application contents.
[Reflection.Assembly]::LoadWithPartialName("System.IO.Compression.FileSystem")
[System.IO.Compression.ZipFile]::CreateFromDirectory(
    "output\bin",
    "ds3.zip",
    [System.IO.Compression.CompressionLevel]::Optimal,
    $true
)

# Delete the build.
Remove-Item -Recurse -Force output

