#pragma once

#include <vector>
#include <functional>
#include <string>
#include <iostream>

class DatabaseObject; // Forward declaration

class Query {
public:
    enum class Operator { AND, OR, NOT };

private:
    std::vector<std::function<bool(const DatabaseObject&)>> conditions;
    std::vector<Operator> operators;
    mutable size_t cachedHash = 0;
    mutable bool hashComputed = false;
    std::string queryType;
    size_t limit = 0;

public:
    Query& byType(const std::string& type);
    Query& byAttribute(const std::string& key, const std::string& value);
    Query& byCustomCondition(std::function<bool(const DatabaseObject&)> customCondition);
    Query& setOperator(Operator op);
    Query& orQuery();
    Query& andQuery();
    Query& notQuery();

    bool evaluate(const DatabaseObject& obj) const;
    size_t hash() const;
    bool operator==(const Query& other) const;

    // Add these new methods
    std::string getType() const { return queryType; }
    const std::vector<std::function<bool(const DatabaseObject&)>>& getConditions() const { return conditions; }
    const std::vector<Operator>& getOperators() const { return operators; }
    size_t getLimit() const { return limit; }
    void setLimit(size_t n) { limit = n; }
private:
    void invalidateHash();
};