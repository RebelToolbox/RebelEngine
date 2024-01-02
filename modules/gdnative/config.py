def can_build(env, platform):
    return True


def configure(env):
    env.use_ptrcall = True


def get_classes():
    return [
        "ARVRInterfaceGDNative",
        "GDNative",
        "GDNativeLibrary",
        "MultiplayerPeerGDNative",
        "NativeScript",
        "PacketPeerGDNative",
        "PluginScript",
        "StreamPeerGDNative",
        "VideoStreamGDNative",
        "WebRTCPeerConnectionGDNative",
        "WebRTCDataChannelGDNative",
    ]
