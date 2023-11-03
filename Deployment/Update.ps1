function Get-ToolsVersion {
    $version_file = Join-Path -Path $PSScriptRoot -ChildPath "VERSION"

    if (-Not (Test-Path -Path $version_file)) {
        return "0.0.0"
    }

    return Get-Content $version_file
}

# Downloads tools from website, returns where it downloaded to your PC
function Download-Tools {
    param (
        [Version]$Version,
        [string]$DestinationFolder
    )

    $url = "https://dab.dev/template/packages/ModTemplate-$Version.zip"
    $destination = Join-Path -Path $DestinationFolder -ChildPath "ModTemplate-$Version.zip"
    if (Test-Path -Path $destination) {
        Write-Host "File $destination already exists, skipping..."
        return $destination
    }

    Write-Host "Downloading file '$url'"
    Invoke-WebRequest -Uri $url -OutFile $destination
    return $destination
}

# Get current template version
Write-Host "Fetching latest version of tools..."
try {
    $response = Invoke-WebRequest -Uri "https://dab.dev/template/CURRENT"
} catch {
    Write-Error "Failed to reach host: $_"
}

[Version]$latest_version = $response.Content
Write-Host "Latest Version: '$latest_version'"

[Version]$current_version = Get-ToolsVersion

# Tools are up to date, no need to do anything
if ($current_version -eq $latest_version) {
    Write-Host "Your tools are up to date ('$current_version')" -ForegroundColor Green
    return;
}

Write-Host "Update Available ('$latest_version') > ('$current_version')" -ForegroundColor Yellow

# Prompt user for input
$update_choice = $null
while ($update_choice -ne 'y' -and $update_choice -ne 'n') {
    $update_choice = Read-Host -Prompt "Do you want to update to the latest version? (y/n)"
}

if ($update_choice -eq 'n') {
    return
}

Write-Host "Updating to '$latest_version'"

# Download the latest version
$download_directory = Join-Path -Path $PSScriptRoot -ChildPath temp

if (-Not (Test-Path $download_directory)) {
    New-Item -ItemType Directory -Path $download_directory
}

$download_zip = Download-Tools -Version $latest_version -Destination $download_directory
$download_extract = $download_zip -replace '\.zip', ''

# Uncompress file
Add-Type -AssemblyName System.IO.Compression.FileSystem
[System.IO.Compression.ZipFile]::ExtractToDirectory($download_zip, $download_extract)

