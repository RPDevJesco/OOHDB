#pragma once

#include "DatabaseObject.hpp"
#include <any>
#include <stdexcept>
#include <type_traits>
#include <sstream>

class DynamicObject : public DatabaseObject {
public:
    DynamicObject(const std::string& id, const std::string& type)
            : DatabaseObject(id, type) {}

    DynamicObject(const DatabaseObject& obj) : DatabaseObject(obj) {}

    template<typename T>
    T getAttribute(const std::string& key) const {
        std::string value = DatabaseObject::getAttribute(key);
        try {
            if constexpr (std::is_same_v<T, int>) {
                return std::stoi(value);
            } else if constexpr (std::is_same_v<T, double>) {
                return std::stod(value);
            } else if constexpr (std::is_same_v<T, bool>) {
                return value == "true" || value == "1";
            } else {
                return value;
            }
        } catch (const std::exception& e) {
            throw std::runtime_error("Error converting attribute '" + key + "': " + e.what());
        }
    }

    template<typename T>
    void setAttribute(const std::string& key, const T& value) {
        if constexpr (std::is_same_v<T, std::string>) {
            DatabaseObject::setAttribute(key, value);
        } else if constexpr (std::is_arithmetic_v<T>) {
            DatabaseObject::setAttribute(key, std::to_string(value));
        } else if constexpr (std::is_same_v<T, bool>) {
            DatabaseObject::setAttribute(key, value ? "true" : "false");
        } else {
            std::ostringstream oss;
            oss << value;
            DatabaseObject::setAttribute(key, oss.str());
        }
    }

    std::shared_ptr<DatabaseObject> clone() const override {
        return std::make_shared<DynamicObject>(*this);
    }
};