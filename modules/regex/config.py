def can_build(env, platform):
    return not env["arch"].startswith("rv")


def configure(env):
    pass


def get_classes():
    return [
        "RegEx",
        "RegExMatch",
    ]
