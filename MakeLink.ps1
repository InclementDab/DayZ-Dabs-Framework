

function Get-ModPrefix {
    # Define the path to the .prefix file in the current directory
    $prefix_file = Join-Path -Path $PSScriptRoot -ChildPath '.prefix'

    # Check if the .prefix file exists
    while (-Not (Test-Path $prefix_file)) {
        $prefix = Read-Host -Prompt 'Enter your mod prefix'
        $prefix_folder = Join-Path -Path $PSScriptRoot -ChildPath $prefix
        if (-Not (Test-Path $prefix_folder)) {
            Write-Error "The path specified is invalid ($prefix_folder)"
        } else {
            Set-Content -Path $prefix_file $prefix
        }
    }

    # Return the contents of the .prefix file
    return Get-Content $prefix_file
}

# Self elevating powershell script
if (!([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] "Administrator")) { Start-Process powershell.exe "-NoProfile -ExecutionPolicy Bypass -File `"$PSCommandPath`"" -Verb RunAs; exit }

$source = Join-Path -Path $PSScriptRoot -ChildPath (Get-ModPrefix)

# Check if our source folder exists
if (-Not (Test-Path -Path $source)) {
    Write-Error "The path '$source' does not exist"
    return
}

$workdrive = Read-Host -Prompt 'Destination Path (P:\)'
if ("" -eq $workdrive) {
    $workdrive = "P:\"
}

if (-Not (Test-Path -Path $workdrive)) {
    Write-Host "Destination Path is not valid $($workdrive)" -ForegroundColor Red
    Write-Host -NoNewLine 'Press any key to continue...'
    $null = $Host.UI.RawUI.ReadKey('NoEcho,IncludeKeyDown')
    return
}

$destination = Join-Path -Path $workdrive -ChildPath (Get-ModPrefix)
if (Test-Path -Path $destination) {
    # File already exists
    return
}

# Check if the destination directory already exists, if it does just do nothing
try {
    # Create symbolic link
    New-Item -ItemType SymbolicLink -Path $destination -Target $source
} catch {
    Write-Error "Failed to create SymLink: $_"
}