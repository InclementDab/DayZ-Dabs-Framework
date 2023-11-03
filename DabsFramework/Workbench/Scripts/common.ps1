function Get-ToolsVersion {
    $version_file = Join-Path -Path "$PSScriptRoot\..\ToolAddons" -ChildPath "VERSION"

    if (-Not (Test-Path -Path $version_file)) {
        return "0.0.0"
    }

    return Get-Content $version_file
}

# Downloads tools from website, returns where it downloaded to your PC
function Download-Tools {
    param (
        [Version]$Version
    )

    $url = "https://dab.dev/template/packages/ModTemplate-$Version.zip"
    $destination = Join-Path -Path $env:TEMP -ChildPath "ModTemplate-$Version.zip"
    if (Test-Path -Path $destination) {
        Write-Host "File $destination already exists, skipping..."
        return $destination
    }

    Write-Host "Downloading file '$url'"
    Invoke-WebRequest -Uri $url -OutFile $destination
    return $destination
}

function Get-RepositoryRoot {
    return (Get-Item $PSScriptRoot).parent.parent.FullName;
}

function Get-ModPrefix {
    return (Get-Item $PSScriptRoot).parent.Name;
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