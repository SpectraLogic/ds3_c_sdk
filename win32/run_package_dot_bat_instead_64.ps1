# Build the application.
cmake .. -G "Visual Studio 14 2015 Win64"
msbuild libds3.sln

# Copy dependencies into output directory
Copy-Item -Path deps\install\* -Destination output -Recurse -force
Copy-Item -Path ..\src\*.h -Exclude ds3_request.h,ds3_net.h,ds3_utils.h,ds3_string_multimap_impl.h,ds3_connection.h -Destination output\include -force

# Create a zip file with the application contents.
[Reflection.Assembly]::LoadWithPartialName("System.IO.Compression.FileSystem")
[System.IO.Compression.ZipFile]::CreateFromDirectory(
    "output",
    "ds3_win64.zip",
    [System.IO.Compression.CompressionLevel]::Optimal,
    $false
)

# Delete the build.
Remove-Item -Recurse -Force output

