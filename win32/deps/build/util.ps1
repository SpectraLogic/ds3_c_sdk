Add-Type -AssemblyName System.IO.Compression.FileSystem
function Unzip
{
    param(
        [Parameter(Mandatory=$true)][string]$zipfile,
        [Parameter(Mandatory=$true)][string]$outpath
    )

    [System.IO.Compression.ZipFile]::ExtractToDirectory($zipfile, $outpath)
}

function Untar() {
    param(
        [Parameter(Mandatory=$true)][string]$archive,
        [Parameter(Mandatory=$true)][string]$outpath
    )

    $7z_path = "$env:TEMP\7z"
    if (!(test-path $7z_path) -or !(test-path "$7z_path\7za.exe")) { 
        if (!(test-path $7z_path)) { md $7z_path | out-null }
        push-location $7z_path
        try {
            write-host "Downloading 7zip" -foregroundcolor cyan

            $7zip_dl = "http://www.7-zip.org/a/7za920.zip"
            (new-object System.Net.WebClient).DownloadFile("$7zip_dl", "$7z_path\7za.zip")

            write-host "done." -foregroundcolor green

            Unzip "$7z_path\7za.zip" "$7z_path"
            del "$7z_path\7za.zip"
        }
        finally { pop-location }
    }
    write-host $7za_path
    $7za_exe = "$7z_path" + '\7za.exe'
    write-host $7za_exe

    if ($archive.endswith('.tar.gz') -or $archive.endswith('.tgz')) {
        $x = "cmd.exe"
        # Properly handle paths and archives with spaces in names
        $y = "/C `"^`"$7za_exe^`" e ^`"$archive^`" -so | ^`"$7za_exe^`" x -aoa -si -ttar -o^`"$outpath^`"" 
        & $x $y | findstr /v /b /c:"Extracting " # suppress noisy per file msg
    } else {
        & "$7za_exe" x $archive -y -o"$outpath" | findstr /v /b /c:"Extracting " # suppress noisy per file msg
    }
}
