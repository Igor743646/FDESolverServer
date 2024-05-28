FROM ubuntu:24.04

LABEL author="Igor Glushatov <igor_743646@mail.ru>"
LABEL version="0.1.0"

WORKDIR /usr/src/FDEServer

RUN apt-get update
RUN apt-get install -y python3
RUN apt-get install -y pip
RUN apt-get install -y cmake
RUN apt-get install -y g++-13
RUN apt-get install -y default-jdk
RUN apt-get install -y curl inetutils-ping
RUN pip install conan --break-system-packages

COPY ./conanfile.py ./conanfile.py

RUN conan profile detect
RUN conan install conanfile.py --build=missing
RUN conan build . -s build_type=Release --build missing

COPY . .

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
RUN cmake --build ./build/ --config Release
RUN cd build && make

EXPOSE 50051

CMD [ "./build/server" ]
