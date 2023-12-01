. ".\common.ps1"

$source = Join-Path -Path (Get-RepositoryRoot) -ChildPath (Get-ModPrefix)

# Check if our source folder exists
if (-Not (Test-Path -Path $source)) {
    Write-Error "The path '$source' does not exist"
    return
}

$destination = Join-Path -Path (Get-Workdrive) -ChildPath (Get-ModPrefix)
if (Test-Path -Path $destination) {
    # File already exists
    return
}

# Check if the destination directory already exists, if it does just do nothing
try {
    # Create symbolic link
    New-Item -ItemType Junction -Path $destination -Target $source
} catch {
    Write-Error "Failed to create SymLink: $_"
}