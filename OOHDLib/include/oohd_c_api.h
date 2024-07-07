#ifndef OOHD_C_API_H
#define OOHD_C_API_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#ifdef OOHD_EXPORTS
#define OOHD_API __declspec(dllexport)
#else
#define OOHD_API __declspec(dllimport)
#endif
#else
#define OOHD_API
#endif

typedef struct OOHD_Handle OOHD_Handle;
typedef struct DatabaseObject_Handle DatabaseObject_Handle;
typedef struct Query_Handle Query_Handle;

// OOHD functions
OOHD_API OOHD_Handle* OOHD_Create();
OOHD_API void OOHD_Destroy(OOHD_Handle* handle);
OOHD_API int OOHD_AddObject(OOHD_Handle* handle, const char* parentId, DatabaseObject_Handle* object);
OOHD_API DatabaseObject_Handle* OOHD_GetObject(OOHD_Handle* handle, const char* id);
OOHD_API void OOHD_DisplayFullHierarchy(OOHD_Handle* handle);
OOHD_API void OOHD_SaveToFile(OOHD_Handle* handle, const char* filename);
OOHD_API OOHD_Handle* OOHD_LoadFromFile(const char* filename);
OOHD_API void OOHD_GenerateVisualization(OOHD_Handle* handle, const char* filename);

// DatabaseObject functions
OOHD_API DatabaseObject_Handle* DatabaseObject_Create(const char* id, const char* type);
OOHD_API void DatabaseObject_Destroy(DatabaseObject_Handle* handle);
OOHD_API const char* DatabaseObject_GetId(DatabaseObject_Handle* handle);
OOHD_API const char* DatabaseObject_GetType(DatabaseObject_Handle* handle);
OOHD_API void DatabaseObject_SetAttribute(DatabaseObject_Handle* handle, const char* key, const char* value);
OOHD_API const char* DatabaseObject_GetAttribute(DatabaseObject_Handle* handle, const char* key);
OOHD_API void DatabaseObject_Display(DatabaseObject_Handle* handle);
OOHD_API int DatabaseObject_GetChildCount(DatabaseObject_Handle* handle);
OOHD_API DatabaseObject_Handle* DatabaseObject_GetChild(DatabaseObject_Handle* handle, int index);

// Query functions
OOHD_API Query_Handle* Query_Create();
OOHD_API void Query_Destroy(Query_Handle* handle);
OOHD_API void Query_ByType(Query_Handle* handle, const char* type);
OOHD_API void Query_ByAttribute(Query_Handle* handle, const char* key, const char* value);
OOHD_API void Query_SetOperator(Query_Handle* handle, int op);
OOHD_API int Query_Evaluate(Query_Handle* handle, DatabaseObject_Handle* obj);

// QueryResult functions
OOHD_API int OOHD_ExecuteQuery(OOHD_Handle* handle, const char* queryString, DatabaseObject_Handle*** results);
OOHD_API void OOHD_FreeQueryResult(DatabaseObject_Handle** results, int count);

#ifdef __cplusplus
}
#endif

#endif // OOHD_C_API_H