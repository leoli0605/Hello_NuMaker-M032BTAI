#!/bin/bash

# Set image and container names
IMAGE_NAME="jafee201153/arm-none-eabi-gcc:13.2-ubuntu-20.04"
CONTAINER_NAME="arm-none-eabi-gcc-container"

# Define the SSH key paths
SSH_PRIVATE_KEY_PATH="$(pwd)/.ssh/id_rsa"
SSH_PUBLIC_KEY_PATH="$(pwd)/.ssh/id_rsa.pub"

# Ensure the SSH private key is secure
chmod 600 "$SSH_PRIVATE_KEY_PATH"
# Public keys can be less restrictive, but typically should not be world-writable
chmod 644 "$SSH_PUBLIC_KEY_PATH"

# Start the ssh-agent and add the private key
eval "$(ssh-agent -s)"
ssh-add "$SSH_PRIVATE_KEY_PATH"

# Pull the Docker image if it's not available locally
if [[ "$(docker images -q "$IMAGE_NAME" 2>/dev/null)" == "" ]]; then
    docker pull "$IMAGE_NAME"
fi

# Run the Docker container with the current directory mounted to /share
docker run --name "$CONTAINER_NAME" --rm -v "$(pwd)":/share -v "$SSH_PRIVATE_KEY_PATH:/tmp/id_rsa" -v "$SSH_PUBLIC_KEY_PATH:/tmp/id_rsa.pub" -d -it "$IMAGE_NAME" sh

# Set up SSH within the container for subsequent Git operations
docker exec "$CONTAINER_NAME" sh -c "\
    mkdir -p ~/.ssh && \
    cp /tmp/id_rsa ~/.ssh/id_rsa && \
    cp /tmp/id_rsa.pub ~/.ssh/id_rsa.pub && \
    chmod 600 ~/.ssh/id_rsa && \
    chmod 600 ~/.ssh/id_rsa.pub && \
    echo 'Host *' > ~/.ssh/config && \
    echo '  StrictHostKeyChecking no' >> ~/.ssh/config"

# Perform Git submodule update and make operations
docker exec "$CONTAINER_NAME" sh -c "\
    cd /share && \
    git submodule update --init --recursive && \
    make clean && \
    make"

# Stop the container
docker stop "$CONTAINER_NAME"

# Kill the running ssh-agent
eval "$(ssh-agent -k)"
