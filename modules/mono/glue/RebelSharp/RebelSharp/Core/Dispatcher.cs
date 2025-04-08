using System.Runtime.CompilerServices;

namespace Rebel
{
    public static class Dispatcher
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern RebelTaskScheduler rebel_icall_DefaultRebelTaskScheduler();

        public static RebelSynchronizationContext SynchronizationContext =>
            rebel_icall_DefaultRebelTaskScheduler().Context;
    }
}
