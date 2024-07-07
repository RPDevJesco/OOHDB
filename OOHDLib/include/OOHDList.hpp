#pragma once

#include "OOHD.hpp"
#include "OOHDQuery.hpp"
#include "DynamicObject.hpp"
#include <vector>
#include <memory>
#include <stdexcept>

class OOHDList : public std::vector<std::shared_ptr<DynamicObject>> {
public:
    OOHDList() = default;
    OOHDList(const std::vector<std::shared_ptr<DatabaseObject>>& objects);

    // Operator overload for query embedding
    OOHDList& operator=(const std::string& query);

private:
    static OOHD* database;
    static OOHDQuery* queryExecutor;

public:
    static void setDatabase(OOHD& db) {
        database = &db;
        queryExecutor = new OOHDQuery(db);
    }
};

OOHDList::OOHDList(const std::vector<std::shared_ptr<DatabaseObject>>& objects) {
    for (const auto& obj : objects) {
        this->push_back(std::make_shared<DynamicObject>(*obj));
    }
}

OOHDList& OOHDList::operator=(const std::string& query) {
    if (!database || !queryExecutor) {
        throw std::runtime_error("Database not set for OOHDList");
    }

    QueryResult result = queryExecutor->execute(query);
    auto params = queryExecutor->parseQuery(query);
    auto formattedResults = queryExecutor->formatResults(result, params);

    this->clear();
    for (const auto& obj : result.getResults()) {
        this->push_back(std::make_shared<DynamicObject>(*obj));
    }

    return *this;
}

OOHD* OOHDList::database = nullptr;
OOHDQuery* OOHDList::queryExecutor = nullptr;