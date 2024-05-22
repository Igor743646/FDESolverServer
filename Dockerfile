FROM python:3

WORKDIR /usr/src

COPY ./core/client/requirements.txt ./WebServer/core/client/requirements.txt
RUN pip install -r ./WebServer/core/client/requirements.txt
COPY ./core/client/ ./WebServer/core/client/
COPY ./core/protos/py/ ./WebServer/core/protos/py/
ENV PYTHONPATH "${PYTHONPATH}:./WebServer/core/protos/py/"
EXPOSE 8002
# COPY . .

CMD [ "python", "./WebServer/core/client/client.py" ]
