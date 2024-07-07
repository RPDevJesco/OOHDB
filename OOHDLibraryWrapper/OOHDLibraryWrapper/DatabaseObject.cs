using System;
using System.Runtime.InteropServices;

namespace OOHDLibraryWrapper
{
    public class DatabaseObject : IDisposable
    {
        private IntPtr _handle;
        public IntPtr Handle => _handle;

        public DatabaseObject(string id, string type)
        {
            _handle = OOHDNativeMethods.DatabaseObject_Create(id, type);
            if (_handle == IntPtr.Zero)
                throw new InvalidOperationException("Failed to create DatabaseObject");
        }

        internal DatabaseObject(IntPtr handle)
        {
            _handle = handle;
        }

        public string Id => OOHDNativeMethods.PtrToStringUtf8(OOHDNativeMethods.DatabaseObject_GetId(Handle));
        public string Type => OOHDNativeMethods.PtrToStringUtf8(OOHDNativeMethods.DatabaseObject_GetType(Handle));

        public void SetAttribute(string key, string value)
        {
            OOHDNativeMethods.DatabaseObject_SetAttribute(Handle, key, value);
        }

        public string GetAttribute(string key)
        {
            return OOHDNativeMethods.PtrToStringUtf8(OOHDNativeMethods.DatabaseObject_GetAttribute(Handle, key));
        }

        public void Display()
        {
            OOHDNativeMethods.DatabaseObject_Display(Handle);
        }

        public int ChildCount => OOHDNativeMethods.DatabaseObject_GetChildCount(Handle);

        public DatabaseObject GetChild(int index)
        {
            IntPtr childHandle = OOHDNativeMethods.DatabaseObject_GetChild(Handle, index);
            if (childHandle == IntPtr.Zero)
                throw new IndexOutOfRangeException("Child index out of range.");
            return new DatabaseObject(childHandle);
        }

        public void Dispose()
        {
            if (_handle != IntPtr.Zero)
            {
                OOHDNativeMethods.DatabaseObject_Destroy(_handle);
                _handle = IntPtr.Zero;
            }
        }

        ~DatabaseObject()
        {
            Dispose();
        }
    }
}