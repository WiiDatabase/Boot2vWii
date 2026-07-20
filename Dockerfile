FROM ghcr.io/wiiu-env/devkitppc:20260504

WORKDIR /Boot2vWii

ENTRYPOINT ["make"]
