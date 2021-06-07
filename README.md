# Procedural generation library

3D graphics procedural generation library based on [Urho3D](https://urho3d.github.io/).

## Build

[Download Urho3D](https://sourceforge.net/projects/urho3d/files/Urho3D/) library, install it and make a [Urho3D project](https://urho3d.github.io/documentation/1.7.1/_using_library.html).

### Run

`cd` to `bin` and run `./Procgen`.

### Configure, compile and run (in one go)

If you start in the `build` directory, to configure

```
cd ../ && script/cmake_generic.sh .
```

If you start in the `build` directory, to compile and run

```
cd ../ && make && cd bin/ && ./Procgen
```

## Procedural model

## Materials

Custom techniques are in `CoreData/Techniques/` and are prefixed with `Tec_`. 

Custom shaders are in `CoreData/Shaders/GLSL` and are prefixed with `Sha_`. 

Custom post processing effects are in `CoreData/Shaders/GLSL` and are prefixed with `PPE_`.

When you write shaders, you can `#include`
- `Transform2D.glsl` to transform 2D points in the shader,
- `Shapes2D.glsl` to draw basic shapes in the shader; see `PPE_Shapes.glsl` for the shape drawing examples,

### Textures

Use `TextureCreator::CreateEffectTexture(int w, int h, const String& shader, RenderSurfaceUpdateMode mode)` with `"PPE_<effect_name>"` as `shader` to create a texture based on a post process effect.

The easiest way to display a texture is to draw it in the UI as an Urho3D `Sprite`.

You can also display a texture by creating material with `MaterialCreator::Create(const String& technique, const HashMap<TextureUnit, Texture*>& textureData)` that uses a technique that displays a diffuse map. Use `ModelCreator::CreateStockModel(const String& modelName, Material* material)` to create a model using `.mdl` files from `Data/Models`.

## Voxels
