# sensei-docker
Dockerfile for synergylabs/sensei

### Usage (Linux Host)

`docker run -it -v /tmp/.X11-unix:/tmp/.X11-unix -e DISPLAY=unix$DISPLAY --device /dev/dri:/dev/dri --device=/dev/bus/usb aroman/sensei`
