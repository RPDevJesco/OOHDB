#include "DatabaseObject.hpp"
#include "Query.hpp"
#include <iostream>
#include <fstream>

DatabaseObject::DatabaseObject(const std::string& id, const std::string& type)
        : id(id), type(type) {}

std::string DatabaseObject::getId() const { return id; }
std::string DatabaseObject::getType() const { return type; }

void DatabaseObject::setParent(std::shared_ptr<DatabaseObject> newParent) {
    parent = newParent;
}

void DatabaseObject::addChild(std::shared_ptr<DatabaseObject> child) {
    children.push_back(child);
    child->setParent(shared_from_this());
}

void DatabaseObject::setAttribute(const std::string& key, const std::string& value) {
    attributes[key] = value;
}

std::string DatabaseObject::getAttribute(const std::string& key) const {
    auto it = attributes.find(key);
    return it != attributes.end() ? it->second : "";
}

void DatabaseObject::display() const {
    //std::cout << "Object ID: " << id << ", Type: " << type << std::endl;
    for (const auto& attr : attributes) {
       // std::cout << "  " << attr.first << ": " << attr.second << std::endl;
    }
}

bool DatabaseObject::matchesQuery(const Query& query) const {
    return query.evaluate(*this);
}

void DatabaseObject::encryptString(std::string& data, const std::string& key) {
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] ^= key[i % key.size()];
    }
}

void DatabaseObject::decryptString(std::string& data, const std::string& key) {
    encryptString(data, key); // XOR encryption is symmetric
}

void DatabaseObject::serialize(std::ofstream& out) const {
    const std::string encryptionKey = "OOHD_SECRET_KEY"; // You can change this key

    // Encrypt and write id and type
    std::string encryptedId = id;
    std::string encryptedType = type;
    encryptString(encryptedId, encryptionKey);
    encryptString(encryptedType, encryptionKey);

    size_t idSize = encryptedId.size();
    out.write(reinterpret_cast<const char*>(&idSize), sizeof(size_t));
    out.write(encryptedId.c_str(), idSize);

    size_t typeSize = encryptedType.size();
    out.write(reinterpret_cast<const char*>(&typeSize), sizeof(size_t));
    out.write(encryptedType.c_str(), typeSize);

    // Encrypt and write attributes
    size_t attrCount = attributes.size();
    out.write(reinterpret_cast<const char*>(&attrCount), sizeof(size_t));
    for (const auto& attr : attributes) {
        std::string encryptedKey = attr.first;
        std::string encryptedValue = attr.second;
        encryptString(encryptedKey, encryptionKey);
        encryptString(encryptedValue, encryptionKey);

        size_t keySize = encryptedKey.size();
        size_t valueSize = encryptedValue.size();
        out.write(reinterpret_cast<const char*>(&keySize), sizeof(size_t));
        out.write(encryptedKey.c_str(), keySize);
        out.write(reinterpret_cast<const char*>(&valueSize), sizeof(size_t));
        out.write(encryptedValue.c_str(), valueSize);
    }

    // Write children (only ids, actual objects will be serialized separately)
    size_t childCount = children.size();
    out.write(reinterpret_cast<const char*>(&childCount), sizeof(size_t));
    for (const auto& child : children) {
        std::string encryptedChildId = child->id;
        encryptString(encryptedChildId, encryptionKey);
        size_t childIdSize = encryptedChildId.size();
        out.write(reinterpret_cast<const char*>(&childIdSize), sizeof(size_t));
        out.write(encryptedChildId.c_str(), childIdSize);
    }
}

std::shared_ptr<DatabaseObject> DatabaseObject::deserialize(std::ifstream& in) {
    const std::string encryptionKey = "OOHD_SECRET_KEY"; // Must match the key used for encryption

    // Read and decrypt id and type
    size_t idSize, typeSize;
    in.read(reinterpret_cast<char*>(&idSize), sizeof(size_t));
    std::string id(idSize, '\0');
    in.read(&id[0], idSize);
    decryptString(id, encryptionKey);

    in.read(reinterpret_cast<char*>(&typeSize), sizeof(size_t));
    std::string type(typeSize, '\0');
    in.read(&type[0], typeSize);
    decryptString(type, encryptionKey);

    auto obj = std::make_shared<DatabaseObject>(id, type);

    // Read and decrypt attributes
    size_t attrCount;
    in.read(reinterpret_cast<char*>(&attrCount), sizeof(size_t));
    for (size_t i = 0; i < attrCount; ++i) {
        size_t keySize, valueSize;
        in.read(reinterpret_cast<char*>(&keySize), sizeof(size_t));
        std::string key(keySize, '\0');
        in.read(&key[0], keySize);
        decryptString(key, encryptionKey);

        in.read(reinterpret_cast<char*>(&valueSize), sizeof(size_t));
        std::string value(valueSize, '\0');
        in.read(&value[0], valueSize);
        decryptString(value, encryptionKey);

        obj->setAttribute(key, value);
    }

    // Read and decrypt child ids (actual objects will be linked later)
    size_t childCount;
    in.read(reinterpret_cast<char*>(&childCount), sizeof(size_t));
    for (size_t i = 0; i < childCount; ++i) {
        size_t childIdSize;
        in.read(reinterpret_cast<char*>(&childIdSize), sizeof(size_t));
        std::string childId(childIdSize, '\0');
        in.read(&childId[0], childIdSize);
        decryptString(childId, encryptionKey);
        // Store child ids for later linking
        obj->children.push_back(std::make_shared<DatabaseObject>(childId, "placeholder"));
    }

    return obj;
}