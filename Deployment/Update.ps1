function Get-ToolsVersion {
    $version_file = Join-Path -Path $PSScriptRoot -ChildPath "VERSION"

    if (-Not (Test-Path -Path $version_file)) {
        return "0.0.0"
    }

    return Get-Content $version_file
}

# Get current template version
Write-Host "Fetching latest version of tools..."
try {
    $response = Invoke-WebRequest -Uri "https://dab.dev/template/CURRENT"
} catch {
    Write-Error "Failed to reach host: $_"
}

[Version]$host_version = $response.Content
Write-Host "Latest Version: '$host_version'"

[Version]$current_version = Get-ToolsVersion

# Tools are up to date, no need to do anything
if ($current_version -eq $host_version) {
    Write-Host "Your tools are up to date ('$current_version')" -ForegroundColor Green
    return;
}


Write-Host "Update Available ('$host_version') > ('$current_version')" -ForegroundColor Yellow

# Prompt user for input
$update_choice = $null
while ($update_choice -ne 'y' -and $update_choice -ne 'n') {
    $update_choice = Read-Host -Prompt "Do you want to update to the latest version? (y/n)"
}

if ($update_choice -eq 'n') {
    return
}

Write-Host "Updating to '$host_version'"