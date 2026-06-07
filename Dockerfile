FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    libgtk-3-0 \
    libwebkit2gtk-4.0-37 \
    libasio-dev \
    sudo \
    xdg-utils \
    dbus-x11 \
    zenity \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

EXPOSE 18080

CMD ["sh", "-c", "cd /app/bin && ./launcher_server"]