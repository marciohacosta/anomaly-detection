# Build environment
FROM alpine:3.20.3 AS builder
RUN apk update && apk upgrade && apk add --no-cache build-base linux-headers git cmake ninja pkgconfig curl unzip tar zip perl openssl bash
ENV VCPKG_FORCE_SYSTEM_BINARIES=1

# VCPkg setup
WORKDIR /home
RUN git clone https://github.com/microsoft/vcpkg.git
WORKDIR /home/vcpkg
RUN echo $PWD
RUN ls -l
RUN ./bootstrap-vcpkg.sh
ENV VCPKG_ROOT=/home/vcpkg
ENV PATH=$VCPKG_ROOT:$PATH

# Folders copy
WORKDIR /home
COPY ./Solution /app/Solution
COPY ./app.cpp /app/app.cpp
COPY ./CMakeLists.txt /app/CMakeLists.txt
COPY ./CMakePresets.json /app/CMakePresets.json
COPY ./CMakeUserPresets.json /app/CMakeUserPresets.json
COPY ./vcpkg-configuration.json /app/vcpkg-configuration.json
COPY ./vcpkg.json /app/vcpkg.json

# Build
WORKDIR /app
RUN cmake --preset=release
RUN cmake --build build

# Runtime environment
FROM alpine:3.20.3
RUN apk update && apk upgrade && apk add --no-cache libstdc++

# Deploy
WORKDIR /app
COPY --from=builder /app/build/AnomalyDetection .
EXPOSE {{MSWAY_IAM_ANALYSER_APPLICATION_PORT}}

ENTRYPOINT ["/app/AnomalyDetection"]
