using System;

namespace Rebel
{
    [AttributeUsage(AttributeTargets.Field | AttributeTargets.Property)]
    public class ExportAttribute : Attribute
    {
        private PropertyHint hint;
        private string hintString;

        public ExportAttribute(PropertyHint hint = PropertyHint.None, string hintString = "")
        {
            this.hint = hint;
            this.hintString = hintString;
        }
    }
}
