using System;
using OOHDLibraryWrapper;

namespace OOHDConsoleApp
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                File.WriteAllText("csharp_log.txt", "Starting application\n");

                using (var database = new OOHD())
                {
                    Console.WriteLine("OOHD instance created successfully.");

                    using (var rootObject = new DatabaseObject("1", "RootType"))
                    {
                        Console.WriteLine("Root object created.");
                        Console.WriteLine($"Root object handle: {rootObject.Handle}");
                        File.AppendAllText("csharp_log.txt", $"Root object created with handle: {rootObject.Handle}\n");

                        rootObject.SetAttribute("name", "Root Object");
                        Console.WriteLine("Attribute set on root object.");

                        try
                        {
                            database.AddObject(null, rootObject);
                            Console.WriteLine("Root object added to database.");
                        }
                        catch (Exception ex)
                        {
                            Console.WriteLine($"Error adding root object: {ex.Message}");
                            File.AppendAllText("csharp_log.txt", $"Error adding root object: {ex}\n");
                        }

                        using (var childObject1 = new DatabaseObject("2", "ChildType"))
                        {
                            childObject1.SetAttribute("name", "Child Object 1");
                            database.AddObject("1", childObject1);
                            Console.WriteLine("Child object 1 added to database.");
                        }

                        using (var childObject2 = new DatabaseObject("3", "ChildType"))
                        {
                            childObject2.SetAttribute("name", "Child Object 2");
                            database.AddObject("1", childObject2);
                            Console.WriteLine("Child object 2 added to database.");
                        }
                        
                        database.SaveToFile("testdb");
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error: {ex.Message}");
                Console.WriteLine($"Stack Trace: {ex.StackTrace}");
                File.AppendAllText("csharp_log.txt", $"Unhandled exception: {ex}\n");
            }
        }
    }
}