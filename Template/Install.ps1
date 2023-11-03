# Function to check if script is running as administrator
function Test-IsAdmin {
    return ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] "Administrator")
}

# If not running as admin, re-run the script with elevation
if (-not (Test-IsAdmin)) {
    Start-Process -Verb RunAs -FilePath $PSCommandPath
    return
}

$prefix = Read-Host -Prompt 'Enter your mod prefix'
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

$folders = Get-ChildItem -Directory -Recurse

Write-Host "Creating empty folders..."

New-Item -Path "Missions\" -Name "Global" -ItemType Directory
New-Item -Path "Missions\" -Name "Dev" -ItemType Directory

# Individual script modules get their own sub-folders for wb organization
New-Item -Path "ModTemplate\Scripts\1_Core" -Name $prefix -ItemType Directory
New-Item -Path "ModTemplate\Scripts\3_Game" -Name $prefix -ItemType Directory
New-Item -Path "ModTemplate\Scripts\4_World" -Name $prefix -ItemType Directory
New-Item -Path "ModTemplate\Scripts\5_Mission" -Name $prefix -ItemType Directory

Write-Host "Renaming folders..."

try {
    foreach ($folder in $folders) {
        if ($folder.Name -like "*ModTemplate*") {
            # Construct the new folder name
            $new_name = $folder.Name -replace "ModTemplate", $prefix

            # Rename the folder
            #Rename-Item -Path $folder.FullName -NewName $new_name
        
            Write-Host "Renamed '$($folder.Name)' to '$new_name'"
        }
    }
}
catch {
    Write-Host "Failed to rename folders: $_" -ForegroundColor Red
}

# Create the symbolic link to P drive

try {
    $symlink_path = Join-Path -Path $workdrive -ChildPath $prefix
    New-Item -Path $symlink_path -ItemType SymbolicLink -Value $prefix
    Write-Host "Symbolic link created successfully." -ForegroundColor Green
}
catch {
    Write-Host "Failed to create symbolic link: $_" -ForegroundColor Red
}
