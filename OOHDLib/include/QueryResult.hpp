#pragma once

#include <vector>
#include <memory>
#include <functional>

class DatabaseObject;

class QueryResult {
private:
    std::vector<std::shared_ptr<DatabaseObject>> results;

public:
    QueryResult() = default; // Default constructor
    QueryResult(std::vector<std::shared_ptr<DatabaseObject>> results);

    // Copy operations
    QueryResult(const QueryResult&) = default;
    QueryResult& operator=(const QueryResult&) = default;

    // Move operations
    QueryResult(QueryResult&&) = default;
    QueryResult& operator=(QueryResult&&) = default;

    QueryResult& sort(std::function<bool(const DatabaseObject&, const DatabaseObject&)> comparator);
    QueryResult& limit(size_t n);
    const std::vector<std::shared_ptr<DatabaseObject>>& getResults() const;
};