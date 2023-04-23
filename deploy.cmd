@echo off

:: Install system packages
IF EXIST "%DEPLOYMENT_TARGET%\apt_requirements.txt" (
    echo Installing apt packages...
    apt-get update -y
    xargs -a "%DEPLOYMENT_TARGET%\apt_requirements.txt" apt-get install -y --no-install-recommends
)

IF EXIST "%DEPLOYMENT_TARGET%\yum_requirements.txt" (
    echo Installing yum packages...
    yum install -y $(cat "%DEPLOYMENT_TARGET%\yum_requirements.txt")
)

:: Install Python packages
IF EXIST "%DEPLOYMENT_TARGET%\requirements.txt" (
    echo Installing Python packages...
    pushd "%DEPLOYMENT_TARGET%"
    python -m venv env
    source env/bin/activate
    pip install -r requirements.txt
    popd
)

:: Copy packages to a different directory
mkdir -p "%DEPLOYMENT_TARGET%\packages"
cp -r "%DEPLOYMENT_TARGET%\env\Lib\site-packages\*" "%DEPLOYMENT_TARGET%\packages\"

:: Restart the app
echo Restarting app...
IF DEFINED APP_POOL_ID (
    %windir%\System32\inetsrv\appcmd.exe stop apppool /apppool.name:%APP_POOL_ID%
    %windir%\System32\inetsrv\appcmd.exe start apppool /apppool.name:%APP_POOL_ID%
)

echo Deployment complete.
