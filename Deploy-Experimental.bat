@echo off

echo Deploying to Experimental. Are you sure?

start /W "Build PBO" "%CD%/DabsFramework/Workbench/Batchfiles/ZBinarizeDeploy.bat"
echo Uploading to Workshop
cd "%CD%/DabsFramework/Workbench/Deployment"
start /W "Upload to Workshop" Deployment.exe experimental
