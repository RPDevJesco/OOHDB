#include "Query.hpp"
#include "DatabaseObject.hpp"
#include <functional>
#include <iostream>

Query& Query::byType(const std::string& type) {
    queryType = type;
    conditions.push_back([type](const DatabaseObject& obj) {
        return obj.getType() == type;
    });
    invalidateHash();
    return *this;
}

Query& Query::byAttribute(const std::string& key, const std::string& value) {
    conditions.push_back([key, value](const DatabaseObject& obj) {
        std::string attrValue = obj.getAttribute(key);
        return attrValue == value;
    });
    invalidateHash();
    return *this;
}

Query& Query::byCustomCondition(std::function<bool(const DatabaseObject&)> customCondition) {
    conditions.push_back([customCondition](const DatabaseObject& obj) {
        bool result = customCondition(obj);
        return result;
    });
    invalidateHash();
    return *this;
}

Query& Query::setOperator(Operator op) {
    operators.push_back(op);
    invalidateHash();
    return *this;
}

Query& Query::orQuery() { return setOperator(Operator::OR); }
Query& Query::andQuery() { return setOperator(Operator::AND); }
Query& Query::notQuery() { return setOperator(Operator::NOT); }

bool Query::evaluate(const DatabaseObject& obj) const {
    if (conditions.empty()) return true;

    std::vector<bool> results;
    for (size_t i = 0; i < conditions.size(); ++i) {
        bool result = conditions[i](obj);
        if (i > 0 && operators[i-1] == Operator::NOT) result = !result;
        results.push_back(result);
    }

    bool finalResult = results[0];
    for (size_t i = 1; i < results.size(); ++i) {
        if (operators[i-1] == Operator::OR) finalResult |= results[i];
        else if (operators[i-1] == Operator::AND) finalResult &= results[i];
    }
    return finalResult;
}

size_t Query::hash() const {
    if (!hashComputed) {
        cachedHash = std::hash<std::string>{}(queryType);
        for (size_t i = 0; i < conditions.size(); ++i) {
            cachedHash ^= i ^ std::hash<size_t>{}(conditions[i].target_type().hash_code());
            if (i < operators.size()) {
                cachedHash ^= std::hash<int>{}(static_cast<int>(operators[i]));
            }
        }
        hashComputed = true;
    }
    return cachedHash;
}

bool Query::operator==(const Query& other) const {
    return hash() == other.hash();
}

void Query::invalidateHash() {
    hashComputed = false;
}