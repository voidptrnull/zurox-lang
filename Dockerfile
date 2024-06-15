FROM alpine:edge
RUN apk add llvm17-dev llvm17 llvm17-libs llvm17-linker-tools llvm17-static llvm17-test-utils llvm17-gtest llvm17-test-utils-pyc
RUN apk add cmake ninja-build samurai
RUN apk add libxml2-dev
RUN apk add build-base
RUN apk add clang17
RUN apk add lld17

RUN llvm-config --version && \
    cmake --version

WORKDIR /app
COPY . /app

CMD [ "sh", "scripts/build-wrapper.sh"]