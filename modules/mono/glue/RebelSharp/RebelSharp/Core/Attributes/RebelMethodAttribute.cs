using System;

namespace Rebel
{
    [AttributeUsage(AttributeTargets.Method)]
    internal class RebelMethodAttribute : Attribute
    {
        private string methodName;

        public string MethodName { get { return methodName; } }

        public RebelMethodAttribute(string methodName)
        {
            this.methodName = methodName;
        }
    }
}
