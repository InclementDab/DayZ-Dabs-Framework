. ".\common.ps1"

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

$current_directory = Join-Path -Path (Get-RepositoryRoot) -ChildPath (Get-ModPrefix)
Write-Host "Current Directory: '$current_directory'"

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
$download_archive = Download-Tools -Version $latest_version
$download_folder = ($download_archive -replace '\.zip', '');

if (Test-Path -Path $download_folder) {
    Remove-Item $download_folder -Force -Recurse
}

# Uncompress file
Add-Type -AssemblyName System.IO.Compression.FileSystem
[System.IO.Compression.ZipFile]::ExtractToDirectory($download_archive, $download_folder)

# Only update files in the ModTemplate that are .c and .cfg files (for now, todo?)
$filtered_files = Get-ChildItem -Path "$download_folder\ModTemplate" -Recurse -File -Include *.c

# Actually update the files
foreach ($filtered_file in $filtered_files) {
    # Replace 'ModTemplate' with our prefix in the file's directory path
    $relative_path = $filtered_file.FullName.Substring("$download_folder\ModTemplate".Length)

    $new_file = Join-Path -Path $current_directory -ChildPath $relative_path
    $new_file_directory = Split-Path -Path $new_file

    # Create the directory if it doesn't exist
    if (-not (Test-Path $new_file_directory)) {
        New-Item -Path $new_file_directory -ItemType Directory
    }

    # Copy the file to the new directory, overwriting if it already exists
    Copy-Item -Path $filtered_file.FullName -Destination $new_file -Force
}


# Finally update the version number
Set-Content -Path (Join-Path -Path "$download_folder\Workbench\ToolAddons" -ChildPath "VERSION") $latest_version