#pragma once

#include "DatabaseObject.hpp"
#include "Query.hpp"
#include "QueryResult.hpp"
#include <unordered_map>
#include <string>

class OOHD {
private:
    std::shared_ptr<DatabaseObject> root;
    std::unordered_map<std::string, std::shared_ptr<DatabaseObject>> objectMap;
    std::unordered_map<std::string, std::vector<std::shared_ptr<DatabaseObject>>> typeIndex;
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::shared_ptr<DatabaseObject>>>> attributeIndex;
    std::unordered_map<size_t, QueryResult> queryCache;

public:
    OOHD();

    void addObject(const std::string& parentId, std::shared_ptr<DatabaseObject> object);
    std::shared_ptr<DatabaseObject> getObject(const std::string& id) const;
    void displayHierarchy(const std::shared_ptr<DatabaseObject>& node, int depth = 0) const;
    void displayFullHierarchy() const;
    QueryResult query(const Query& query);

    // New methods for saving and loading
    void saveToFile(const std::string& filename) const;
    static OOHD loadFromFile(const std::string& filename);
    void generateVisualization(const std::string& filename) const;

private:
    void writeObjectToHTML(std::ofstream& out, const std::shared_ptr<DatabaseObject>& obj, int depth) const;
};