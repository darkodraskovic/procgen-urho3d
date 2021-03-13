# Procedural generation library

3D graphics procedural generation library based on [Urho3D](https://urho3d.github.io/).

## Build

[Download Urho3D](https://sourceforge.net/projects/urho3d/files/Urho3D/) library, install it and make a [Urho3D project](https://urho3d.github.io/documentation/1.7.1/_using_library.html).

## Run

`cd` to `bin` and run `./Procgen`.

## Configure, compile and run (in one go)

If you start in the `build` directory, to configure

```
cd ../ && script/cmake_generic.sh .
```

If you start in the `build` directory, to compile and run

```
cd ../ && make && cd bin/ && ./Procgen
```
