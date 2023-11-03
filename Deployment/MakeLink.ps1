

function Get-ModPrefix {
    # Define the path to the .prefix file in the current directory
    $prefix_file = Join-Path -Path (Get-RepositoryRoot) -ChildPath '.prefix'

    # Check if the .prefix file exists
    while (-Not (Test-Path $prefix_file)) {
        $prefix = Read-Host -Prompt 'Enter your mod prefix'
        if ("" -eq $prefix) {
            Write-Error "Invalid prefix"
            continue
        }

        $prefix_folder = Join-Path -Path (Get-RepositoryRoot) -ChildPath $prefix
        if (-Not (Test-Path $prefix_folder)) {
            Write-Error "The path specified is invalid ($prefix_folder)"
        } else {
            Set-Content -Path $prefix_file $prefix
        }
    }

    # Return the contents of the .prefix file
    return Get-Content $prefix_file
}

function Get-Workdrive {
    $workdrive = "P:\"

    while (-Not (Test-Path -Path $workdrive)) {
        # Get workdrive name
        $workdrive = Read-Host -Prompt 'Destination Path (P:\)'
        if ("" -eq $workdrive) {
            $workdrive = "P:\"
        }
    }

    return $workdrive
}

function Get-RepositoryRoot {
    return (Get-Item $PSScriptRoot).parent.FullName;
}

# Self elevating powershell script
if (!([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] "Administrator")) { Start-Process powershell.exe "-NoProfile -ExecutionPolicy Bypass -File `"$PSCommandPath`"" -Verb RunAs; exit }

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
    New-Item -ItemType SymbolicLink -Path $destination -Target $source
} catch {
    Write-Error "Failed to create SymLink: $_"
}