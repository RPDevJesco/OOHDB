#include "OOHD.hpp"
#include <stdexcept>
#include <fstream>
#include <iostream>

OOHD::OOHD() : root(nullptr) {

}

void OOHD::addObject(const std::string& parentId, std::shared_ptr<DatabaseObject> object) {
    std::ofstream logFile("oohd_log.txt", std::ios::app);
    logFile << "Entering OOHD::addObject" << std::endl;
    logFile << "parentId: " << (parentId.empty() ? "empty (root)" : parentId) << std::endl;
    logFile << "object ID: " << object->getId() << std::endl;
    logFile << "object type: " << object->getType() << std::endl;
    logFile << "object address: " << object.get() << std::endl;

    try {
        if (parentId.empty() || parentId == "root") {
            if (root) {
                logFile << "Error: Root object already exists" << std::endl;
                throw std::runtime_error("Root object already exists");
            }
            root = object;
            logFile << "Root object set with ID: " << object->getId() << std::endl;
        } else {
            auto parentIt = objectMap.find(parentId);
            if (parentIt == objectMap.end()) {
                logFile << "Error: Parent object not found. Parent ID: " << parentId << std::endl;
                logFile << "Current objects in map:" << std::endl;
                for (const auto& pair : objectMap) {
                    logFile << "  " << pair.first << std::endl;
                }
                throw std::runtime_error("Parent object not found");
            }
            logFile << "Parent object found with ID: " << parentId << std::endl;
            parentIt->second->addChild(object);
            logFile << "Child added to parent" << std::endl;
        }

        objectMap[object->getId()] = object;
        logFile << "Object added to objectMap with ID: " << object->getId() << std::endl;

        typeIndex[object->getType()].push_back(object);
        logFile << "Object added to typeIndex for type: " << object->getType() << std::endl;

        for (const auto& attr : object->getAttributes()) {
            attributeIndex[attr.first][attr.second].push_back(object);
            logFile << "Object indexed for attribute: " << attr.first << " = " << attr.second << std::endl;
        }

        queryCache.clear();
        logFile << "Query cache cleared" << std::endl;

        logFile << "OOHD::addObject completed successfully" << std::endl;
    } catch (const std::exception& e) {
        logFile << "Exception in OOHD::addObject: " << e.what() << std::endl;
        throw;
    }
}

std::shared_ptr<DatabaseObject> OOHD::getObject(const std::string& id) const {
    auto it = objectMap.find(id);
    if (it != objectMap.end()) return it->second;
    throw std::runtime_error("Object not found");
}

void OOHD::displayHierarchy(const std::shared_ptr<DatabaseObject>& node, int depth) const {
    std::string indent(depth * 2, ' ');
    node->display();
    for (const auto& child : node->children) {
        displayHierarchy(child, depth + 1);
    }
}

void OOHD::displayFullHierarchy() const {
    displayHierarchy(root);
}

QueryResult OOHD::query(const Query& query) {
    if (query.getLimit() == 0) {
        auto cacheIt = queryCache.find(query.hash());
        if (cacheIt != queryCache.end()) {
            return cacheIt->second;
        }
    }

    std::vector<std::shared_ptr<DatabaseObject>> results;
    for (const auto& [id, obj] : objectMap) {
        bool matches = query.evaluate(*obj);

        if (matches) {
            results.push_back(obj);
        }
    }

    QueryResult queryResult(std::move(results));

    // Only cache queries without LIMIT
    if (query.getLimit() == 0) {
        queryCache[query.hash()] = queryResult;
    }

    return queryResult;
}

void OOHD::saveToFile(const std::string& filename) const {
    std::ofstream out(filename, std::ios::binary);
    if (!out) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }

    // Write the number of objects
    size_t objectCount = objectMap.size();
    out.write(reinterpret_cast<const char*>(&objectCount), sizeof(size_t));

    // Serialize all objects
    for (const auto& pair : objectMap) {
        pair.second->serialize(out);
    }

    out.close();
}

OOHD OOHD::loadFromFile(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        throw std::runtime_error("Cannot open file for reading: " + filename);
    }

    OOHD database;

    // Read the number of objects
    size_t objectCount;
    in.read(reinterpret_cast<char*>(&objectCount), sizeof(size_t));

    // Deserialize all objects
    for (size_t i = 0; i < objectCount; ++i) {
        auto obj = DatabaseObject::deserialize(in);
        database.objectMap[obj->getId()] = obj;
        database.typeIndex[obj->getType()].push_back(obj);
        for (const auto& attr : obj->attributes) {
            database.attributeIndex[attr.first][attr.second].push_back(obj);
        }
    }

    // Link children
    for (auto& pair : database.objectMap) {
        auto& obj = pair.second;
        std::vector<std::shared_ptr<DatabaseObject>> actualChildren;
        for (const auto& placeholder : obj->children) {
            actualChildren.push_back(database.objectMap[placeholder->getId()]);
        }
        obj->children = actualChildren;
    }

    // Set the root
    database.root = database.objectMap["root"];

    in.close();
    return database;
}

void OOHD::generateVisualization(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }

    out << R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>OOHD Visualization</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 20px; }
        .tree ul { list-style-type: none; padding-left: 20px; }
        .tree li { margin: 10px 0; }
        .node { cursor: pointer; display: inline-block; }
        .children { display: none; }
        .expanded > .children { display: block; }
        .object-details { margin-left: 20px; font-size: 0.9em; color: #666; }
        .toggle { display: inline-block; width: 10px; }
    </style>
</head>
<body>
    <h1>OOHD Visualization</h1>
    <div class="tree">
)";

    writeObjectToHTML(out, root, 0);

    out << R"(
    </div>
    <script>
        document.querySelectorAll('.node').forEach(node => {
            node.addEventListener('click', function(e) {
                e.stopPropagation();
                this.parentElement.classList.toggle('expanded');
                let toggle = this.querySelector('.toggle');
                toggle.textContent = this.parentElement.classList.contains('expanded') ? '▼' : '►';
            });
        });
    </script>
</body>
</html>
)";

    out.close();
}

void OOHD::writeObjectToHTML(std::ofstream& out, const std::shared_ptr<DatabaseObject>& obj, int depth) const {
    std::string indent(depth * 4, ' ');
    out << indent << "<ul>\n";
    out << indent << "    <li>\n";
    out << indent << "        <div class=\"node\">";
    if (!obj->getChildren().empty()) {
        out << "<span class=\"toggle\">►</span> ";
    }
    out << obj->getType() << ": " << obj->getId() << "</div>\n";
    out << indent << "        <div class=\"object-details\">\n";
    for (const auto& [key, value] : obj->getAttributes()) {
        out << indent << "            " << key << ": " << value << "<br>\n";
    }
    out << indent << "        </div>\n";
    if (!obj->getChildren().empty()) {
        out << indent << "        <div class=\"children\">\n";
        for (const auto& child : obj->getChildren()) {
            writeObjectToHTML(out, child, depth + 1);
        }
        out << indent << "        </div>\n";
    }
    out << indent << "    </li>\n";
    out << indent << "</ul>\n";
}