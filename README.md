# Dynamic Mesh Generator And Procedural Fence Generator And Interactive Mesh Generation System using HISM - combined Assignments
 
## Dynamic Mesh Generator Video Demo

https://drive.google.com/file/d/1j7Ywt26jE8DQZoC-4rkqzqxZSgXrpOSJ/view?usp=sharing

## Fence Generator Video Demo

https://drive.google.com/file/d/1xTeGjZU-noce-MKrK4C4OFky19ZZUjTo/view?usp=sharing

## Interactive Mesh Generation System using HISM Video Demo
https://drive.google.com/file/d/1KmFEjXrtA0QlOdk0IYgKVpj9dAh-2yAb/view?usp=sharing



# Dynamic Mesh Generator

## Objective

This project in Unreal Engine allows players to interact with a user interface to spawn static meshes in the game environment. Players can select different meshes and materials from a UI menu, place these objects in the game world, replace them, and customize their appearances by changing materials and textures.

## Features

- **Data Management**:
  - **Mesh, Material, and Texture Data**: Store references and properties using C++ structs (`FMeshData`, `FMaterialData`, `FTextureData`).
  - **Asset Management**: Use a `UDataAsset` class (`UMeshAssetManager`) to organize and access these resources.

- **User Interface**:
  - **Slate Widget**: `SMeshSelectionScrollBox` class for displaying options in a horizontal scroll box.
  - **Widget Integration**: `MeshSelectionScrollBox` UWidget to incorporate the Slate widget.
  - **Selection Interface**: `USelectionWidget` class to integrate the selection widgets into a user-friendly UI.

- **Custom Player Controller**:
  - **Mesh Management**: `AInteractiveArchController` class to handle spawning, replacing, and managing meshes.
  - **Input Handling**: Bind keys for selecting floors, displaying the UI, and hiding it.
  - **Actor Management**: `AArchMeshActor` class for placing and managing static meshes.

- **Material and Texture Customization**:
  - **UI for Customization**: Extend `USelectionWidget` to allow selecting materials and textures for the spawned meshes.
  - **Dynamic Updates**: Change materials and textures of `AArchMeshActor` based on UI selections.

- **Multiple Camera Views**:
  - **Perspective View**: Default 3D view for general navigation and interaction.
  - **Isometric View**: Fixed-angle 3D view for a simulated 2D perspective.
  - **Orthographic View**: Top-down view for detailed adjustments and precise placements.
  - **Dynamic Camera Control**: Switch between views using key bindings, with tailored controls for each perspective.

## Demo
https://drive.google.com/file/d/1j7Ywt26jE8DQZoC-4rkqzqxZSgXrpOSJ/view?usp=sharing

# Procedural Fence System

## Objective

This project involves developing a procedural fence system in Unreal Engine using C++. The system uses splines to place fences along a path with customizable parameters such as shape, length, height, and spacing. The vertical rail meshes for the fence are procedural, featuring custom shape designs.

## Features

- **Data Management**:
  - **Fence Properties**: Structure `FFenceProperties` to manage fence characteristics like length, width, height, and spacing.

- **Procedural Fence Generation**:
  - **Spline Component**: Use a spline component in `AFenceMeshActor` to define the path along which the fence will be placed.
  - **Vertical Rail Actor**: `AVerticalRailActor` to define and customize the shape and size of vertical rails for the fence.
  - **Procedural Mesh Component**: Create procedural vertical rail meshes with customizable parts, including fixed bottom parts and customizable top parts.

- **Blueprint Integration**:
  - **Predefined Blueprints**: Derived from `AVerticalRailActor` to provide unique shapes for vertical meshes.

- **Dynamic Fence Construction**:
  - **Construction Script**: Spawn vertical and horizontal fence meshes along the spline path.
  - **Runtime Replacement**: Replace static vertical meshes with actor instances upon game start.

- **Custom Materials**:
  - **Wooden Material**: Create a custom wooden material with parameters for color and textures (diffuse, roughness, normal).
  - **Tileable UV Coordinates**: Use TileX and TileY parameters to adjust UV tiling based on mesh scale.
  - **Dynamic Material Instances**: Apply dynamic materials to both vertical and horizontal mesh components with appropriate UV tiling adjustments.

## Demo
https://drive.google.com/file/d/1xTeGjZU-noce-MKrK4C4OFky19ZZUjTo/view?usp=sharing


# Interactive Mesh Generation System using HISM

## Objective

Develop an interactive runtime system in Unreal Engine where users can select an area on the ground, generate meshes using Hierarchical Instanced Static Meshes (HISM) in a background thread, and apply a stylized toon shader material to them. The system includes a UI for interaction and a progress bar to display the progress of mesh generation.

## Features

- **Toon Shader Material**:
  - **M_ToonShader**: Stylized material using HLSL for toon shading.
  - **Toon Shading Technique**: Creates hard transitions between light and dark areas for a cartoon-like effect.

- **Mesh Properties Data Asset**:
  - **MeshDataAsset**: Stores properties like mesh type, scale, rotation, and footprint distance.

- **User Interface**:
  - **W_MeshGenerationUI**: Widget Blueprint with:
    - Combo button for area shape selection (spherical or box).
    - Numeric box for radius/dimensions.
    - Button to generate meshes.
    - Progress bar for mesh generation progress.
    - Numeric box for the number of instances.

- **Area Selection**:
  - **ASelectionArea**: Actor class to select a spherical or box area on the ground.
  - **Procedural Meshes**: Functions to generate and toggle between spherical and box shapes.
  - **Visual Indicator**: Translucent material applied to procedural meshes for visualization.

- **Mesh Generation Using HISM**:
  - **AMeshGenerator**: Actor class to generate meshes using HISM.
  - **Background Task**: Offload mesh generation to a background thread using AsyncTask.
  - **Progress Update**: Update the progress bar in the UI to reflect mesh generation progress.
  - **Toon Shader Application**: Apply toon shader material to generated meshes.

## Demo
https://drive.google.com/file/d/1KmFEjXrtA0QlOdk0IYgKVpj9dAh-2yAb/view?usp=sharing




