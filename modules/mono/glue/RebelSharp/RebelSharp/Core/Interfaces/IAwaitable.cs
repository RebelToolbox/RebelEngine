namespace Rebel
{
    public interface IAwaitable
    {
        IAwaiter GetAwaiter();
    }

    public interface IAwaitable<out TResult>
    {
        IAwaiter<TResult> GetAwaiter();
    }
}
