using System.Runtime.InteropServices;

namespace OOHDLibraryWrapper
{
    public class OOHD : IDisposable
    {
        private IntPtr _handle;

        public OOHD()
        {
            _handle = OOHDNativeMethods.OOHD_Create();
            if (_handle == IntPtr.Zero)
                throw new InvalidOperationException("Failed to create OOHD instance");
            File.AppendAllText("csharp_log.txt", $"OOHD created with handle: {_handle}\n");
        }

        public void AddObject(string parentId, DatabaseObject obj)
        {
            parentId = parentId ?? string.Empty; // Treat null as empty string for root object
            File.AppendAllText("csharp_log.txt", $"Attempting to add object. OOHD handle: {_handle}, Parent ID: {parentId}, Object ID: {obj.Id}, Object Type: {obj.Type}, Object handle: {obj.Handle}\n");

            if (obj == null)
                throw new ArgumentNullException(nameof(obj));

            if (obj.Handle == IntPtr.Zero)
                throw new InvalidOperationException("DatabaseObject handle is invalid");

            int result = OOHDNativeMethods.OOHD_AddObject(_handle, parentId, obj.Handle);
            File.AppendAllText("csharp_log.txt", $"OOHD_AddObject result: {result}\n");

            if (result != 0)
            {
                string errorMessage = $"Failed to add object. Error code: {result}";
                File.AppendAllText("csharp_log.txt", $"Error: {errorMessage}\n");
                throw new InvalidOperationException(errorMessage);
            }

            File.AppendAllText("csharp_log.txt", $"Object successfully added. ID: {obj.Id}, Type: {obj.Type}\n");
        }

        public DatabaseObject GetObject(string id)
        {
            IntPtr objHandle = OOHDNativeMethods.OOHD_GetObject(_handle, id);
            if (objHandle == IntPtr.Zero)
                throw new InvalidOperationException("Object not found.");
            return new DatabaseObject(objHandle);
        }

        public void DisplayFullHierarchy()
        {
            OOHDNativeMethods.OOHD_DisplayFullHierarchy(_handle);
        }

        public void SaveToFile(string filename)
        {
            OOHDNativeMethods.OOHD_SaveToFile(_handle, filename);
        }

        public static OOHD LoadFromFile(string filename)
        {
            IntPtr handle = OOHDNativeMethods.OOHD_LoadFromFile(filename);
            if (handle == IntPtr.Zero)
                throw new InvalidOperationException("Failed to load from file.");
            return new OOHD { _handle = handle };
        }

        public void GenerateVisualization(string filename)
        {
            OOHDNativeMethods.OOHD_GenerateVisualization(_handle, filename);
        }

        public QueryResult ExecuteQuery(string queryString)
        {
            IntPtr resultPtr;
            int count = OOHDNativeMethods.OOHD_ExecuteQuery(_handle, queryString, out resultPtr);
            return new QueryResult(resultPtr, count);
        }

        public void Dispose()
        {
            if (_handle != IntPtr.Zero)
            {
                OOHDNativeMethods.OOHD_Destroy(_handle);
                _handle = IntPtr.Zero;
            }
        }
    }
}