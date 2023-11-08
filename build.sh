#!/bin/bash

IMAGE_NAME="jafee201153/arm-none-eabi-gcc:13.2-ubuntu-20.04"
CONTAINER_NAME="arm-none-eabi-gcc-container"

if [[ "$(docker images -q $IMAGE_NAME 2>/dev/null)" == "" ]]; then
    docker pull $IMAGE_NAME
fi

docker run --name $CONTAINER_NAME --rm -v "$(pwd)":/share -d -it $IMAGE_NAME sh
docker exec $CONTAINER_NAME sh -c "cd /share && make clean && make"
docker stop $CONTAINER_NAME
