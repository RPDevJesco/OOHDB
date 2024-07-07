using System;
using System.Runtime.InteropServices;

namespace OOHDLibraryWrapper
{
    internal static class OOHDNativeMethods
    {
        private const string DllName = "OOOHDLib";

        // OOHD functions
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr OOHD_Create();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void OOHD_Destroy(IntPtr handle);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int OOHD_AddObject(IntPtr handle, [MarshalAs(UnmanagedType.LPStr)] string parentId, IntPtr objectHandle);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr OOHD_GetObject(IntPtr handle, string id);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void OOHD_DisplayFullHierarchy(IntPtr handle);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void OOHD_SaveToFile(IntPtr handle, string filename);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr OOHD_LoadFromFile(string filename);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void OOHD_GenerateVisualization(IntPtr handle, string filename);

        // DatabaseObject functions
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr DatabaseObject_Create(string id, string type);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void DatabaseObject_Destroy(IntPtr handle);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr DatabaseObject_GetId(IntPtr handle);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr DatabaseObject_GetType(IntPtr handle);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void DatabaseObject_SetAttribute(IntPtr handle, string key, string value);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr DatabaseObject_GetAttribute(IntPtr handle, string key);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void DatabaseObject_Display(IntPtr handle);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int DatabaseObject_GetChildCount(IntPtr handle);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr DatabaseObject_GetChild(IntPtr handle, int index);

        // Query functions
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr Query_Create();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void Query_Destroy(IntPtr handle);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void Query_ByType(IntPtr handle, string type);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void Query_ByAttribute(IntPtr handle, string key, string value);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void Query_SetOperator(IntPtr handle, int op);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int Query_Evaluate(IntPtr queryHandle, IntPtr objHandle);

        // QueryResult functions
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int OOHD_ExecuteQuery(IntPtr handle, string queryString, out IntPtr results);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void OOHD_FreeQueryResult(IntPtr results, int count);

        // Helper methods for string marshaling
        internal static string PtrToStringUtf8(IntPtr ptr)
        {
            if (ptr == IntPtr.Zero)
                return null;

            int len = 0;
            while (Marshal.ReadByte(ptr, len) != 0) len++;
            byte[] buffer = new byte[len];
            Marshal.Copy(ptr, buffer, 0, buffer.Length);
            return System.Text.Encoding.UTF8.GetString(buffer);
        }
    }
}