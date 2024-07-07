using System.Runtime.InteropServices;

namespace OOHDLibraryWrapper
{
    public class QueryResult : IDisposable
    {
        private IntPtr _results;
        private int _count;

        internal QueryResult(IntPtr results, int count)
        {
            _results = results;
            _count = count;
        }

        public int Count => _count;

        public DatabaseObject this[int index]
        {
            get
            {
                if (index < 0 || index >= _count)
                    throw new IndexOutOfRangeException();
                IntPtr ptr = Marshal.ReadIntPtr(_results, index * IntPtr.Size);
                return new DatabaseObject(ptr);
            }
        }

        public void Dispose()
        {
            OOHDNativeMethods.OOHD_FreeQueryResult(_results, _count);
            _results = IntPtr.Zero;
        }
    }
}