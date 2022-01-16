# Procedural generation library

3D graphics procedural generation library based on [Urho3D](https://urho3d.github.io/).

## Build

[Download Urho3D](https://sourceforge.net/projects/urho3d/files/Urho3D/) library, install it and make a [Urho3D project](https://urho3d.github.io/documentation/1.7.1/_using_library.html).

### Configure

```
cd build
cmake ..
```

### Compile

```
cd build
make
```

### Run

```
cd bin
./ProcGen
```

## Modules

### ProcGen

ProcGen is the basic module used by Voxels, Simulation and Maze modules. ProcGen module depends on [libtcod](https://github.com/libtcod/libtcod) for noise, heightmap and BSP generation.

#### Models

Basic component for creation and manipulation of procedural geometry.

##### ProcModel

##### GeometryCreator

##### ModelCreator

#### Materials

##### Techniques

Urho3D uses `Technique` to specify render passes and shaders used when rendering materials. Custom techniques are in `CoreData/Techniques/` and are prefixed with `Tec_`. 

##### Shaders 

Custom shaders are in `CoreData/Shaders/GLSL` and are prefixed with `Sha_`. 

When you write shaders, you can `#include`

- `Transform2D.glsl` to transform 2D points in the shader,
- `Shapes2D.glsl` to draw basic shapes in the shader; see `PPE_Shapes.glsl` for the shape drawing examples,

##### Postprocessing effects

Custom post processing effects are in `CoreData/Shaders/GLSL` and are prefixed with `PPE_`.

##### Textures

Use `TextureCreator::CreateEffectTexture(int w, int h, const String& shader, RenderSurfaceUpdateMode mode)` with `"PPE_<effect_name>"` as `shader` to create a texture based on a post process effect.

The easiest way to display a texture is to draw it in the UI as an Urho3D `Sprite`.

You can also display a texture by creating material with `MaterialCreator::Create(const String& technique, const HashMap<TextureUnit, Texture*>& textureData)` that uses a technique that displays a diffuse map. Use `ModelCreator::CreateStockModel(const String& modelName, Material* material)` to create a model using `.mdl` files from `Data/Models`.

### Voxels

### Simulation

### Maze

