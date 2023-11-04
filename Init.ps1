. ".\DabsFramework\Workbench\Scripts\common.ps1"

# Get user input
$name = Read-Host -Prompt 'Enter your mod name (DayZ-Mod-Name)'
$name = $name.Replace(" ", "")
$name = $name.Replace("\t", "")
$name = $name.Replace("\n", "")
if ("" -eq $name) {
    $name = "DayZ-Mod-Name"
}

$prefix = Read-Host -Prompt 'Enter your mod prefix (ModName)'
$prefix = $prefix.Replace(" ", "")
$prefix = $prefix.Replace("\t", "")
$prefix = $prefix.Replace("\n", "")
if ("" -eq $prefix) {
    $prefix = "ModName"
}

$destination = Read-Host -Prompt 'Enter destination directory (C:\Dev\proj)'
if ("" -eq $destination) {
    $destination = "C:\Dev\proj"
}

$target_directory = Join-Path -Path $destination -ChildPath $name

# Check if this folder already exists
if (Test-Path -Path $target_directory) {

    # DEBUG!!! remove me please on releases
    Remove-Item $target_directory -Force -Recurse
    continue

    Write-Error "The folder already exists ($target_directory)"
    Read-Host "Press Enter to exit..."
    return
}

# Get current template version
Write-Host "Fetching latest version of tools..."
try {
    $response = Invoke-WebRequest -Uri "https://dab.dev/template/CURRENT"
} catch {
    Write-Error "Failed to reach host: $_"
    return
}

[Version]$latest_version = $response.Content
Write-Host "Latest Version: '$latest_version'"

# Download the latest version
$download_archive = Download-Tools -Version $latest_version
$download_folder = ($download_archive -replace '\.zip', '');

if (Test-Path -Path $download_folder) {
    Remove-Item $download_folder -Force -Recurse
}

# Uncompress file
Add-Type -AssemblyName System.IO.Compression.FileSystem
[System.IO.Compression.ZipFile]::ExtractToDirectory($download_archive, $download_folder)

# Copy it over to our new location
Copy-Item -Path $download_folder -Destination $target_directory -Recurse

# Rename all ModTemplate folders
foreach ($folder in Get-ChildItem -Directory $target_directory -Recurse) {
    if ($folder.Name.Contains("ModTemplate")) {
        $new_name = $folder.FullName.Replace("ModTemplate", $prefix)
        Rename-Item -Path $folder.FullName -NewName $new_name
    }
}