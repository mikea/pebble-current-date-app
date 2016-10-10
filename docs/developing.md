# Building Application

## LEGAL NOTE
You must accept the [Pebble Terms of Use][TOS] and the [SDK License Agreement][LA] to use the Pebble SDK *and* to build this image.

[TOS]: https://developer.getpebble.com/legal/terms-of-use/
[LA]: https://developer.getpebble.com/legal/sdk-license/

```bash
# from project root
docker build . -t pebble-today-is && docker run -ti pebble-today-is
```

## Saving `.PBW` file

```bash
# .pbw file will be in out/
mkdir out
docker build . -t pebble-today-is && docker run -ti -v $PWD/out:/out pebble-today-is
```

## Installing Application
Use your local SDK to install app into emulator:

```bash
# substitute emulator type and current app version
pebble install --emulator basalt ./out/pebble-today-is.0.6.0.pbw
```
