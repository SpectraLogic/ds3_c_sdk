# Build the applciation.
nmake -f Makefile.vc ds3\bin\ds3.dll ds3\include\ds3.h OUTPUT=ds3

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

