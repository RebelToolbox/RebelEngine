# FBX Module

[FBX](https://en.wikipedia.org/wiki/FBX) is a proprietary file format owned by Autodesk.

The Rebel Engine FBX Importer is based on the Open Asset Import Library (assimp) FBX importer:
- Folder: /modules/fbx/fbx_parser
- Project: https://assimp.org/
- Source: https://github.com/assimp/assimp
- Commit: 308db73d0b3c2d1870cd3e465eaa283692a4cf23 (November 2019)
- License: BSD-3-Clause

The FBX parser code has been extensively modified to provide memory safety and additional functionality.
These updates need to be sent upstream before we can update the assimp FBX parser again.

## FBX File Structure

Since FBX is a proprietary file format, its actual structure is not published.
Our knowledge of the FBX file format is based on Blender developer research:
https://code.blender.org/2013/08/fbx-binary-file-format-specification/

### FBX File Headers

FBX binary files start with the header "Kaydara FBX Binary".
FBX text files contain a larger header, which sometimes includes copyright information for the file.

### Nodes

In Autodesk's [Maya](https://en.wikipedia.org/wiki/Autodesk_Maya) and [3ds Max](https://en.wikipedia.org/wiki/Autodesk_3ds_Max) everything is a node.
Bones, joints, locators, skins, lights, cameras, etc. are all defined as nodes.
Nodes are connected to each other to create a node tree.

### Node Connections

FBX Nodes are connected using Object to Object Links (OO links).
Additional information is provided using Object to Property Links (OP links).
Property information includes extra attributes, defaults or even simple settings.
The link source and destination are based on the object and property IDs.

## Bones

Bones have links to sub-deformers.
The sub-deformer is part of the skin.
Bones also have an explicit skin link.
Skins link to the mesh geometry.

## Rotation Order

Rebel Engine and FBX use different rotation orders.
Rebel Engine uses the **YXZ** rotation order.
FBX uses a dynamic rotation order to prevent gimbal lock with complex animations.

```cpp
enum RotOrder {
	RotOrder_EulerXYZ = 0
	RotOrder_EulerXZY,
	RotOrder_EulerYZX,
	RotOrder_EulerYXZ,
	RotOrder_EulerZXY,
	RotOrder_EulerZYX,
	RotOrder_SphericXYZ // nobody uses this - as far as we can tell
};
```

## Transforms

Transforms are linked to a node and are used to calculate where a node is in space.
Maya transforms are cumulative i.e. parent transforms affect child nodes.
Whereas 3ds Max transforms use geometric space i.e. transforms only affect the current node, and child nodes have their own transform.

```cpp
Transform ComputePivotTransform(
		Transform chain[TransformationComp_MAXIMUM],
		Transform &geometric_transform) {

	// Maya pivots
	Transform T = chain[TransformationComp_Translation];
	Transform Roff = chain[TransformationComp_RotationOffset];
	Transform Rp = chain[TransformationComp_RotationPivot];
	Transform Rpre = chain[TransformationComp_PreRotation];
	Transform R = chain[TransformationComp_Rotation];
	Transform Rpost = chain[TransformationComp_PostRotation];
	Transform Soff = chain[TransformationComp_ScalingOffset];
	Transform Sp = chain[TransformationComp_ScalingPivot];
	Transform S = chain[TransformationComp_Scaling];

	// 3DS Max Pivots
	Transform OT = chain[TransformationComp_GeometricTranslation];
	Transform OR = chain[TransformationComp_GeometricRotation];
	Transform OS = chain[TransformationComp_GeometricScaling];

	// Calculate 3DS max pivot transform - use geometric space (e.g doesn't effect children nodes only the current node)
	geometric_transform = OT * OR * OS;
	// Calculate standard maya pivots
	return T * Roff * Rp * Rpre * R * Rpost.inverse() * Rp.inverse() * Soff * Sp * S * Sp.inverse();
}
```

## Transform Inheritance

Nodes have an option to scale based on the global 3D axes or their orientation.

```cpp
enum TransformInheritance {
	Transform_RrSs = 0,
	// Local scaling is offset by rotation of parent node
	Transform_RSrs = 1,
	// Parent * Local (normal mode)
	Transform_Rrs = 2,
	// Node transform scale is the only relevant component
	TransformInheritance_MAX // end-of-enum sentinel
};
```

## Axes

Rebel Engine ues the following axes:
- Right: +X
- Up: +Y
- Forward: -Z

FBX supports every axes format:
- Right: CoordAxis
- Up: UpAxis
- Forward: FrontAxis

```
GlobalSettings:  {
	Version: 1000
	Properties70:  {
		P: "UpAxis", "int", "Integer", "",1
		P: "UpAxisSign", "int", "Integer", "",1
		P: "FrontAxis", "int", "Integer", "",2
		P: "FrontAxisSign", "int", "Integer", "",1
		P: "CoordAxis", "int", "Integer", "",0
		P: "CoordAxisSign", "int", "Integer", "",1
		P: "OriginalUpAxis", "int", "Integer", "",1
		P: "OriginalUpAxisSign", "int", "Integer", "",1
		P: "UnitScaleFactor", "double", "Number", "",1
		P: "OriginalUnitScaleFactor", "double", "Number", "",1
		P: "AmbientColor", "ColorRGB", "Color", "",0,0,0
		P: "DefaultCamera", "KString", "", "", "Producer Perspective"
		P: "TimeMode", "enum", "", "",6
		P: "TimeProtocol", "enum", "", "",2
		P: "SnapOnFrameMode", "enum", "", "",0
		P: "TimeSpanStart", "KTime", "Time", "",0
		P: "TimeSpanStop", "KTime", "Time", "",92372316000
		P: "CustomFrameRate", "double", "Number", "",-1
		P: "TimeMarker", "Compound", "", ""
		P: "CurrentTimeMarker", "int", "Integer", "",-1
	}
}
```

## Mesh Geometry Indexing

Mesh geometry information depends on the mapping type.

### Vertex Mapping Types
- **None**: The mapping is undetermined.
- **ByVertex**: For each vertex there will be one mapping coordinate.
  - **Direct**: Vertex
  - **IndexToDirect**: UV
- **ByPolygonVertex**: For each vertex there will be a mapping coordinate for every polygon the vertex is a part of.
Mapping information is sorted by polygon and vertex.
- **ByPolygon**: For each polygon there will be one mapping coordinate.
- **ByEdge**: For each unique edge in the mesh there will be one mapping coordinate.
Mapping information references the edge id.
This is used with smoothing layer elements.
- **AllSame**: One mapping coordinate for the whole surface.
