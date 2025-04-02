namespace Rebel
{
    public interface ISerializationListener
    {
        void OnBeforeSerialize();
        void OnAfterDeserialize();
    }
}
