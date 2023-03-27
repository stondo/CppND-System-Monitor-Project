########################################################################################################################
# MagicalForestFights build stage
########################################################################################################################

FROM ubuntu:23.04 AS build

RUN apt-get update && apt-get -y install build-essential cmake libncurses5-dev libncursesw5-dev

WORKDIR /monitor

COPY include/ ./include/
COPY src/ ./src/
COPY CMakeLists.txt .

WORKDIR /monitor

RUN cmake -S . -B build && cd build && make

########################################################################################################################
# MagicalForestFights image
########################################################################################################################

FROM ubuntu:23.04

COPY --from=build \
    ./monitor/build/monitor \
    ./app/

ENTRYPOINT [ "./app/monitor"]
