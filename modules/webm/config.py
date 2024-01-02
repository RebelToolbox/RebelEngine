def can_build(env, platform):
    if env["arch"].startswith("rv"):
        return False
    return platform not in ["iphone"]


def configure(env):
    pass


def get_classes():
    return [
        "VideoStreamWebm",
    ]
