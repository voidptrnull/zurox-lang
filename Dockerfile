# Use the appropriate platform for the build
FROM --platform=$BUILDPLATFORM alpine:latest

# Update and install necessary packages
RUN apk update \
    && apk add --no-cache \
        clang \
        llvm \
        musl-dev \
        libc-dev \
        linux-headers \
        cmake \
        git \
        build-base \
        file \
        python3 \
        binutils \
        g++ \
        bash \
        gdb 

# Set environment variables for Clang/LLVM
ENV CC=clang
ENV CXX=clang++
ENV LD=lld

# Set working directory
WORKDIR /app

# Copy project files to the working directory
COPY . /app/

# Default command to run
CMD [ "sh", "scripts/build.sh" ]