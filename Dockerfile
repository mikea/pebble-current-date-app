FROM skippa/docker-pebble-sdk
RUN apt-get install -y jq 

ENV OUTPUT_DIR "/out"

COPY app/ /pebble-today-is/

WORKDIR /pebble-today-is/
CMD export APP_VERSION=$(jq -r '.version' package.json) && \
    pebble build && \
    mkdir -p $OUTPUT_DIR && \
    cp -v build/pebble-today-is.pbw $OUTPUT_DIR/pebble-today-is.$APP_VERSION.pbw
