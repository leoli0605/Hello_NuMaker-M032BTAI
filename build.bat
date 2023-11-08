@echo off
set IMAGE_NAME=jafee201153/arm-none-eabi-gcc:13.2-ubuntu-20.04
set CONTAINER_NAME=arm-none-eabi-gcc-container

docker images -q %IMAGE_NAME% 2>nul | findstr /r /c:"^" >nul
if errorlevel 1 (
    docker pull %IMAGE_NAME%
)

docker run --name %CONTAINER_NAME% --rm -v "%cd%":/share -d -it %IMAGE_NAME% sh
docker exec %CONTAINER_NAME% sh -c "cd /share && make clean && make"
docker stop %CONTAINER_NAME%
