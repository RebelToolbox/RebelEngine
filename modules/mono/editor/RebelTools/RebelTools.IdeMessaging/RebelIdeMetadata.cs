namespace RebelTools.IdeMessaging
{
    public readonly struct RebelIdeMetadata
    {
        public int Port { get; }
        public string EditorExecutablePath { get; }

        public const string DefaultFileName = "ide_messaging_meta.txt";

        public RebelIdeMetadata(int port, string editorExecutablePath)
        {
            Port = port;
            EditorExecutablePath = editorExecutablePath;
        }

        public static bool operator ==(RebelIdeMetadata a, RebelIdeMetadata b)
        {
            return a.Port == b.Port && a.EditorExecutablePath == b.EditorExecutablePath;
        }

        public static bool operator !=(RebelIdeMetadata a, RebelIdeMetadata b)
        {
            return !(a == b);
        }

        public override bool Equals(object obj)
        {
            if (obj is RebelIdeMetadata metadata)
                return metadata == this;

            return false;
        }

        public bool Equals(RebelIdeMetadata other)
        {
            return Port == other.Port && EditorExecutablePath == other.EditorExecutablePath;
        }

        public override int GetHashCode()
        {
            unchecked
            {
                return (Port * 397) ^ (EditorExecutablePath != null ? EditorExecutablePath.GetHashCode() : 0);
            }
        }
    }
}
