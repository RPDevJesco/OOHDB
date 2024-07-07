#include "OOHDQuery.hpp"
#include "QueryResult.hpp"
#include "Query.hpp"
#include <sstream>
#include <iostream>
#include <functional>
#include <stdexcept>

QueryResult OOHDQuery::execute(const std::string& queryString) {
    QueryParams params = parseQuery(queryString);
    Query query = buildQuery(params);
    QueryResult result = database.query(query);
    if (params.limit > 0) {
        result = result.limit(params.limit);
    }
    return result;
}

OOHDQuery::QueryParams OOHDQuery::parseQuery(const std::string& queryString) {
    QueryParams params;
    std::istringstream iss(queryString);
    std::string token;

    // Parse SELECT
    iss >> token;
    if (token != "SELECT") throw std::runtime_error("Query must start with SELECT");

    // Parse attributes
    while (iss >> token && token != "FROM") {
        if (token != ",") params.selectAttributes.push_back(token);
    }

    // Parse FROM
    if (token != "FROM") throw std::runtime_error("FROM keyword not found");
    iss >> params.objectType;

    // Parse WHERE (if present)
    if (iss >> token) {
        if (token == "WHERE") {
            std::string key, op, value;
            while (iss >> key >> op) {
                if (op != "=" && op != ">" && op != "<")
                    throw std::runtime_error("Only '=', '>' and '<' operators are supported in WHERE clause");

                // Read the value, which might be quoted
                iss >> std::ws;
                if (iss.peek() == '\'') {
                    iss.get(); // Remove the leading quote
                    std::getline(iss, value, '\''); // Read until the closing quote
                } else {
                    iss >> value;
                }

                params.conditions[key] = std::make_pair(op, value);
                if (!(iss >> token)) break;
                if (token != "AND") break; // Allow the query to end without AND
            }
        }

        // Parse LIMIT (if present)
        if (token == "LIMIT" || iss >> token) {
            if (token == "LIMIT") {
                iss >> params.limit;
            }
        }
    }

    return params;
}

Query OOHDQuery::buildQuery(const QueryParams& params) {
    Query query;
    query.byType(params.objectType);
    query.setLimit(params.limit);

    for (const auto& [key, condition] : params.conditions) {
        const std::string& op = condition.first;
        const std::string& value = condition.second;

        if (op == "=") {
            query.andQuery().byAttribute(key, value);
        } else if (op == ">" || op == "<") {
            query.andQuery().byCustomCondition(
                    [key, op, value](const DatabaseObject& obj) -> bool {
                        std::string attrValue = obj.getAttribute(key);
                        if (attrValue.empty()) return false;
                        try {
                            double objValue = std::stod(attrValue);
                            double condValue = std::stod(value);
                            bool result = op == ">" ? objValue > condValue : objValue < condValue;
                            return result;
                        } catch (const std::exception& e) {
                            std::cout << "Exception during comparison: " << e.what() << std::endl;
                            return false;
                        }
                    }
            );
        }
    }

    return query;
}

std::vector<std::unordered_map<std::string, std::string>> OOHDQuery::formatResults(const QueryResult& results, const QueryParams& params) {
    std::vector<std::unordered_map<std::string, std::string>> formattedResults;

    for (const auto& obj : results.getResults()) {
        std::unordered_map<std::string, std::string> row;
        for (const auto& attr : params.selectAttributes) {
            row[attr] = obj->getAttribute(attr);
        }
        formattedResults.push_back(row);
    }

    return formattedResults;
}