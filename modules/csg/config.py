def can_build(env, platform):
    return True


def configure(env):
    pass


def get_classes():
    return [
        "CSGBox",
        "CSGCombiner",
        "CSGCylinder",
        "CSGMesh",
        "CSGPolygon",
        "CSGPrimitive",
        "CSGShape",
        "CSGSphere",
        "CSGTorus",
    ]
