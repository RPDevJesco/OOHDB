#include "QueryResult.hpp"
#include "DatabaseObject.hpp"
#include <algorithm>

QueryResult::QueryResult(std::vector<std::shared_ptr<DatabaseObject>> results)
        : results(std::move(results)) {}

QueryResult& QueryResult::sort(std::function<bool(const DatabaseObject&, const DatabaseObject&)> comparator) {
    std::sort(results.begin(), results.end(),
              [&comparator](const auto& a, const auto& b) { return comparator(*a, *b); });
    return *this;
}

QueryResult& QueryResult::limit(size_t n) {
    if (n < results.size()) results.resize(n);
    return *this;
}

const std::vector<std::shared_ptr<DatabaseObject>>& QueryResult::getResults() const {
    return results;
}