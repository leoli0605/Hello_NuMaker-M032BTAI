@echo off
SETLOCAL EnableDelayedExpansion

:: Set image and container names
SET "IMAGE_NAME=jafee201153/arm-none-eabi-gcc:13.2-ubuntu-20.04"
SET "CONTAINER_NAME=arm-none-eabi-gcc-container"

:: Define the SSH key paths
SET "SSH_PRIVATE_KEY_PATH=%CD%\.ssh\id_rsa"
SET "SSH_PUBLIC_KEY_PATH=%CD%\.ssh\id_rsa.pub"

:: Ensure the SSH private key is secure
ICACLS "%SSH_PRIVATE_KEY_PATH%" /inheritance:r /grant:r "%USERNAME%:R"
ICACLS "%SSH_PUBLIC_KEY_PATH%" /inheritance:r /grant:r "%USERNAME%:R"

:: Check the status of the ssh-agent service and start if not running
sc query ssh-agent | find "RUNNING"
IF ERRORLEVEL 1 (
    echo The ssh-agent service is not running. Starting it now...
    :: Require to run as Administrator
    net session >nul 2>&1
    IF ERRORLEVEL 1 (
        echo This script must be run as an Administrator
        :: Exit if not admin
        GOTO :EOF
    )
    sc config ssh-agent start= demand
    net start ssh-agent
)

:: Start the ssh-agent and add the private key
CALL ssh-agent
ssh-add "%SSH_PRIVATE_KEY_PATH%"

:: Pull the Docker image if it's not available locally
FOR /F "tokens=*" %%i IN ('docker images -q "%IMAGE_NAME%" 2^>nul') DO SET "IMAGE_EXISTS=%%i"
IF "%IMAGE_EXISTS%"=="" (
    docker pull "%IMAGE_NAME%"
)

:: Run the Docker container with the current directory mounted to /share
docker run --name "%CONTAINER_NAME%" --rm -v "%CD%":/share -v "%SSH_PRIVATE_KEY_PATH%":/tmp/id_rsa -v "%SSH_PUBLIC_KEY_PATH%":/tmp/id_rsa.pub -d -it "%IMAGE_NAME%" sh

:: Set up SSH within the container for subsequent Git operations
docker exec "%CONTAINER_NAME%" sh -c "mkdir -p ~/.ssh && cp /tmp/id_rsa ~/.ssh/id_rsa && cp /tmp/id_rsa.pub ~/.ssh/id_rsa.pub && chmod 600 ~/.ssh/id_rsa && chmod 600 ~/.ssh/id_rsa.pub && echo 'Host *' > ~/.ssh/config && echo '  StrictHostKeyChecking no' >> ~/.ssh/config"

:: Perform Git submodule update and make operations
docker exec "%CONTAINER_NAME%" sh -c "cd /share && git submodule update --init --recursive && make clean && make"

:: Stop the container
docker stop "%CONTAINER_NAME%"

:: Kill the running ssh-agent
CALL ssh-agent -k

ENDLOCAL
