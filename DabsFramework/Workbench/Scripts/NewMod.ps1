

return;

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

# Run MakeLink now
.\MakeLink.ps1
