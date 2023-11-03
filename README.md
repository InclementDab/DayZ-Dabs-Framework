# DayZ-Dabs-Framework

## Introduction

Dabs Framework is a complete, open-source modding framework for DayZ Standalone. For documentation please view the [wiki](https://github.com/InclementDab/DayZ-Dabs-Framework/wiki)

## Installation

Navigate to your project folder in any terminal of your choice, I use command prompt. 

**Important:** Do not clone this repository into your your Workdrive. This is a built clone for all source files, your workdrive will be handled below

### Setting up Workdrive

```
# Clone the repository onto your PC
git clone https://github.com/InclementDab/DayZ-Dabs-Framework.git

# Go to your newly cloned folder
cd DayZ-Dabs-Framework

# Run Make file
MakeLink.bat
```

### Launching Workbench

```
# Navigate to the project folder
../DabsFramework/Workbench

# Open the project
Double click 'dayz.gproj'. If prompted, open with Workbench (located in 'steamapps/common/DayZ Tools/Bin/Workbench')

# Set Source Data Directory (Workbench -> Options)
Set "Source Data Directory" to DayZ install path (e.g., C:/.../DayZ Exp) 
Restart Workbench by reopening dayz.gproj

# Configure Project (Plugins -> Settings -> Configure Project)
Set Repository path to mod location (e.g., C:/Dev/proj/DayZ-Dabs-Framework)
```

## Workbench Shortcuts

All plugin shortcuts are listed below

| Key          | Command                     |
|--------------|-----------------------------|
| <kbd>F6</kbd> | Launch Game |
| <kbd>Ctrl</kbd> + <kbd>F6</kbd> | Configure Project |
| <kbd>F10</kbd> | Close Game |
| <kbd>Ctrl</kbd> + <kbd>N</kbd> | Create New File |
| <kbd>Ctrl</kbd> + <kbd>-</kbd> | Open Current Directory |
| <kbd>Ctrl</kbd> + <kbd>Shift</kbd> + <kbd>F2</kbd> | Rename Current File |
| <kbd>Ctrl</kbd> + <kbd>Shift</kbd> + <kbd>Delete</kbd> | Delete Current File |

You can find more information on the [DayZ Workbench Wiki](https://community.bistudio.com/wiki/DayZ:Workbench_Script_Debugging)


# Mod Template

## Creating a New Mod
```
# Launch Script
Run script ./CreateMod.ps1
```

## License

This mod is licensed under [CC Attribution NonCommercial NoDerivatives 4.0](LICENSE.md)

For any additional questions or feedback, please raise an issue in this repository or contact [Tyler Paul](mailto:tylerpaul2329@gmail.com).