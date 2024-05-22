FROM python:3

LABEL author="Igor Glushatov <igor_743646@mail.ru>"
LABEL version="0.1.0"

WORKDIR /usr/src/WebServer

ENV PYTHONPATH "${PYTHONPATH}:/usr/src/WebServer/core/client/libs/generated/"

EXPOSE 8002

COPY ./requirements.txt ./requirements.txt
RUN pip install -r ./requirements.txt

COPY . .
CMD [ "python", "./client.py", "--host", "0.0.0.0" ]
