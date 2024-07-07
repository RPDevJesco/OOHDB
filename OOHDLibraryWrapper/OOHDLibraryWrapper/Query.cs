namespace OOHDLibraryWrapper
{
    public class Query : IDisposable
    {
        private IntPtr _handle;

        public Query()
        {
            _handle = OOHDNativeMethods.Query_Create();
        }

        public void ByType(string type)
        {
            OOHDNativeMethods.Query_ByType(_handle, type);
        }

        public void ByAttribute(string key, string value)
        {
            OOHDNativeMethods.Query_ByAttribute(_handle, key, value);
        }

        public void SetOperator(int op)
        {
            OOHDNativeMethods.Query_SetOperator(_handle, op);
        }

        public bool Evaluate(DatabaseObject obj)
        {
            return OOHDNativeMethods.Query_Evaluate(_handle, obj.Handle) != 0;
        }

        public void Dispose()
        {
            OOHDNativeMethods.Query_Destroy(_handle);
        }
    }
}