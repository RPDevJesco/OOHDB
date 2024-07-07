#define OOHD_EXPORTS
#include "oohd_c_api.h"
#include "OOHD.hpp"
#include "DatabaseObject.hpp"
#include "Query.hpp"
#include "OOHDQuery.hpp"
#include <cstring>
#include <vector>

extern "C" {

OOHD_Handle* OOHD_Create() {
    return reinterpret_cast<OOHD_Handle*>(new OOHD());
}

void OOHD_Destroy(OOHD_Handle* handle) {
    delete reinterpret_cast<OOHD*>(handle);
}

int OOHD_AddObject(OOHD_Handle* handle, const char* parentId, DatabaseObject_Handle* object) {
    std::ofstream logFile("oohd_log.txt", std::ios::app);
    logFile << "Entering OOHD_AddObject" << std::endl;
    logFile << "OOHD_Handle: " << handle << std::endl;
    logFile << "parentId: " << (parentId && *parentId ? parentId : "empty (root)") << std::endl;
    logFile << "DatabaseObject_Handle: " << object << std::endl;

    if (!handle) {
        logFile << "Error: OOHD handle is null" << std::endl;
        return -1;
    }
    if (!object) {
        logFile << "Error: DatabaseObject handle is null" << std::endl;
        return -2;
    }
    try {
        OOHD* oohd = reinterpret_cast<OOHD*>(handle);
        logFile << "OOHD object address: " << oohd << std::endl;

        std::shared_ptr<DatabaseObject>* dbObj = reinterpret_cast<std::shared_ptr<DatabaseObject>*>(object);
        logFile << "DatabaseObject shared_ptr address: " << dbObj << std::endl;

        if (!dbObj || !*dbObj) {
            logFile << "Error: Invalid DatabaseObject" << std::endl;
            return -3;
        }

        logFile << "Calling OOHD::addObject" << std::endl;
        oohd->addObject(parentId ? parentId : "", *dbObj);
        logFile << "OOHD::addObject called successfully" << std::endl;

        return 0;
    } catch (const std::exception& e) {
        logFile << "Error in OOHD_AddObject: " << e.what() << std::endl;
        return -4;
    } catch (...) {
        logFile << "Unknown error in OOHD_AddObject" << std::endl;
        return -5;
    }
}

DatabaseObject_Handle* OOHD_GetObject(OOHD_Handle* handle, const char* id) {
    try {
        auto obj = reinterpret_cast<OOHD*>(handle)->getObject(id);
        return reinterpret_cast<DatabaseObject_Handle*>(new std::shared_ptr<DatabaseObject>(obj));
    } catch (const std::exception&) {
        return nullptr;
    }
}

void OOHD_DisplayFullHierarchy(OOHD_Handle* handle) {
    reinterpret_cast<OOHD*>(handle)->displayFullHierarchy();
}

void OOHD_SaveToFile(OOHD_Handle* handle, const char* filename) {
    try {
        reinterpret_cast<OOHD*>(handle)->saveToFile(filename);
    } catch (const std::exception&) {
        // Handle or log error
    }
}

OOHD_Handle* OOHD_LoadFromFile(const char* filename) {
    try {
        OOHD* oohd = new OOHD(OOHD::loadFromFile(filename));
        return reinterpret_cast<OOHD_Handle*>(oohd);
    } catch (const std::exception&) {
        return nullptr;
    }
}

void OOHD_GenerateVisualization(OOHD_Handle* handle, const char* filename) {
    try {
        reinterpret_cast<OOHD*>(handle)->generateVisualization(filename);
    } catch (const std::exception&) {
        // Handle or log error
    }
}

DatabaseObject_Handle* DatabaseObject_Create(const char* id, const char* type) {
    return reinterpret_cast<DatabaseObject_Handle*>(
            new std::shared_ptr<DatabaseObject>(std::make_shared<DatabaseObject>(id, type))
    );
}

void DatabaseObject_Destroy(DatabaseObject_Handle* handle) {
    delete reinterpret_cast<std::shared_ptr<DatabaseObject>*>(handle);
}

const char* DatabaseObject_GetId(DatabaseObject_Handle* handle) {
    return (*reinterpret_cast<std::shared_ptr<DatabaseObject>*>(handle))->getId().c_str();
}

const char* DatabaseObject_GetType(DatabaseObject_Handle* handle) {
    return (*reinterpret_cast<std::shared_ptr<DatabaseObject>*>(handle))->getType().c_str();
}

void DatabaseObject_SetAttribute(DatabaseObject_Handle* handle, const char* key, const char* value) {
    (*reinterpret_cast<std::shared_ptr<DatabaseObject>*>(handle))->setAttribute(key, value);
}

const char* DatabaseObject_GetAttribute(DatabaseObject_Handle* handle, const char* key) {
    return (*reinterpret_cast<std::shared_ptr<DatabaseObject>*>(handle))->getAttribute(key).c_str();
}

void DatabaseObject_Display(DatabaseObject_Handle* handle) {
    (*reinterpret_cast<std::shared_ptr<DatabaseObject>*>(handle))->display();
}

int DatabaseObject_GetChildCount(DatabaseObject_Handle* handle) {
    return static_cast<int>((*reinterpret_cast<std::shared_ptr<DatabaseObject>*>(handle))->getChildren().size());
}

DatabaseObject_Handle* DatabaseObject_GetChild(DatabaseObject_Handle* handle, int index) {
    auto& children = (*reinterpret_cast<std::shared_ptr<DatabaseObject>*>(handle))->getChildren();
    if (index >= 0 && index < children.size()) {
        return reinterpret_cast<DatabaseObject_Handle*>(new std::shared_ptr<DatabaseObject>(children[index]));
    }
    return nullptr;
}

Query_Handle* Query_Create() {
    return reinterpret_cast<Query_Handle*>(new Query());
}

void Query_Destroy(Query_Handle* handle) {
    delete reinterpret_cast<Query*>(handle);
}

void Query_ByType(Query_Handle* handle, const char* type) {
    reinterpret_cast<Query*>(handle)->byType(type);
}

void Query_ByAttribute(Query_Handle* handle, const char* key, const char* value) {
    reinterpret_cast<Query*>(handle)->byAttribute(key, value);
}

void Query_SetOperator(Query_Handle* handle, int op) {
    reinterpret_cast<Query*>(handle)->setOperator(static_cast<Query::Operator>(op));
}

int Query_Evaluate(Query_Handle* handle, DatabaseObject_Handle* obj) {
    return reinterpret_cast<Query*>(handle)->evaluate(**reinterpret_cast<std::shared_ptr<DatabaseObject>*>(obj));
}

int OOHD_ExecuteQuery(OOHD_Handle* handle, const char* queryString, DatabaseObject_Handle*** results) {
    OOHD* oohd = reinterpret_cast<OOHD*>(handle);
    OOHDQuery query(*oohd);
    auto queryResult = query.execute(queryString);
    auto& resultVector = queryResult.getResults();

    *results = reinterpret_cast<DatabaseObject_Handle**>(
            malloc(resultVector.size() * sizeof(DatabaseObject_Handle*))
    );

    for (size_t i = 0; i < resultVector.size(); ++i) {
        (*results)[i] = reinterpret_cast<DatabaseObject_Handle*>(
                new std::shared_ptr<DatabaseObject>(resultVector[i])
        );
    }

    return static_cast<int>(resultVector.size());
}

void OOHD_FreeQueryResult(DatabaseObject_Handle** results, int count) {
    for (int i = 0; i < count; ++i) {
        DatabaseObject_Destroy(results[i]);
    }
    free(results);
}

} // extern "C"