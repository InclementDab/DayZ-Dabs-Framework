# DayZ-Mod-Template
Mod Template for DayZ Standalone. Includes build tools, gproj, and script folders

### If you are cloning this and the mod already exists
```
1.) Clone into hard drive outside of your P drive
2.) Configure a user.cfg from the user_sample.cfg
3.) Run SetupWorkdrive.bat
4.) Build with ZBinarizeDeploy.bat
5.) Run with any of the provided Batch Files
```

### If you are creating your own mod
```
1.) Clone into hard drive outside of your P drive
2.) Use VSCode to rename every instance of ModTemplate to your ModPrefixName, and manually rename the two folders
3.) Configure a user.cfg from the user_sample.cfg
4.) Run SetupWorkdrive.bat
5.) Build with ZBinarizeDeploy.bat
6.) Open dayz.gproj by double clicking it and opening with workbench
7.) Run with any of the provided Batch Files
```

Any folders inside of the ModTemplate directory will be converted into PBO's (excluding the Workbench folder)

DayZ Workbench Wiki:
https://community.bistudio.com/wiki/DayZ:Workbench_Script_Debugging#Filepatching
