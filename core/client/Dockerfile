FROM ubuntu:24.04

LABEL author="Igor Glushatov <igor_743646@mail.ru>"
LABEL version="0.1.0"

WORKDIR /usr/src/WebServer

RUN apt-get update 
RUN apt-get install -y python3 pip
RUN apt-get install -y curl inetutils-ping

COPY ./requirements.txt ./requirements.txt
RUN pip install -r ./requirements.txt --break-system-packages

COPY . .

ENV PYTHONPATH "${PYTHONPATH}:/usr/src/WebServer/libs/generated/"

EXPOSE 8002

CMD [ "python3", "./client.py", "--host", "0.0.0.0" ]
