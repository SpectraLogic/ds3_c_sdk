# Build the application.
cmake .. -G "Visual Studio 14 2015"
msbuild libds3.sln

# Copy dependencies into output directory
Copy-Item -Path deps\install\* -Destination output -Recurse -force

# Create a zip file with the application contents.
[Reflection.Assembly]::LoadWithPartialName("System.IO.Compression.FileSystem")
[System.IO.Compression.ZipFile]::CreateFromDirectory(
    "ds3",
    "ds3.zip",
    [System.IO.Compression.CompressionLevel]::Optimal,
    $true
)

# Delete the build.
Remove-Item -Recurse -Force output

