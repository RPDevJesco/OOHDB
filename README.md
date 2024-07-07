# Object-Oriented Hierarchical Database (OOHD)

## Project Overview

The Object-Oriented Hierarchical Database (OOHD) is a C++ implementation that combines the hierarchical structure of tree-based databases with the flexibility and richness of object-oriented databases. This design allows each record to have a single parent, maintaining a clear hierarchical structure, while storing data as objects to support complex data types and relationships.

## Key Features

1. **Hierarchical Structure**: Data is organized in a tree-like hierarchy, where each record has a single parent and potentially multiple children.

2. **Object-Oriented Data Storage**: Each node in the hierarchy represents an object, storing complex data types and supporting object-oriented principles such as encapsulation and inheritance.

3. **Flexible Schema**: Supports a flexible schema, enabling different types of objects to coexist within the hierarchy.

4. **Advanced Query System**: Implements a flexible query system that allows for complex searches based on object types, attributes, and hierarchical relationships. Supports SQL-like queries with SELECT, FROM, WHERE, and LIMIT clauses.

5. **Caching**: Includes a query cache to improve performance for repeated queries, while ensuring up-to-date results for queries with LIMIT clauses.

6. **Serialization**: Supports saving the database to a file and loading it back, with basic encryption to protect the data.

7. **Dynamic Objects**: Utilizes a DynamicObject class to represent any type of object in the database, allowing for runtime-defined schemas.

## Project Structure

The project is organized into the following main components:

- `DatabaseObject`: Represents a node in the hierarchical structure.
- `DynamicObject`: A flexible object type that can represent any kind of entity in the database.
- `Query`: Implements the query system for searching the database.
- `QueryResult`: Handles the results of queries, including sorting and limiting.
- `OOHD`: The main database class that manages the overall structure and operations.
- `OOHDQuery`: Parses and executes SQL-like queries.

## Building and Running

This project uses CMake for building. To build and run the project:

1. Create a build directory: `mkdir build && cd build`
2. Run CMake: `cmake ..`
3. Build the project: `make`
4. Run the executable: `./oohd`

## Appropriate Datasets

The OOHD system is particularly well-suited for datasets with the following characteristics:

1. **Hierarchical Structure**: Data that naturally fits into a parent-child relationship structure.
2. **Flexible Attributes**: Datasets where different objects may have varying attributes.
3. **Moderate Size**: Currently optimized for datasets with thousands to tens of thousands of objects.
4. **Complex Querying Needs**: Datasets that require a mix of attribute-based and hierarchical queries.

Examples of appropriate datasets include:

- Organizational structures (companies, departments, employees)
- File systems (directories and files)
- Product catalogs with categories and subcategories
- Geographic data (continents, countries, cities)
- Family trees or genealogical data
- Decision trees and other hierarchical AI models
- Game development scenarios, such as:
    - Game world hierarchies (regions, areas, locations)
    - Character skill trees
    - Quest and dialogue trees
    - Item crafting recipes and upgrade paths
    - Tech trees in strategy games
- XML-like document structures
- Menu systems and UI hierarchies
- Small to Medium sized Datasets

The OOHD system is particularly valuable for decision trees and game development for the following reasons:

1. **Decision Trees**:
    - Naturally represents the hierarchical structure of decision nodes
    - Supports different attributes for different types of nodes (e.g., decision nodes vs. leaf nodes)
    - Allows for efficient traversal and querying of the tree structure
    - Facilitates easy updates and modifications to the decision tree

2. **Game Development**:
    - Flexible schema allows for diverse game entities with varying attributes
    - Hierarchical structure suits many game design patterns (e.g., entity-component systems)
    - Efficient querying supports common game operations like collision detection or AI decision-making
    - Supports complex relationships between game objects (e.g., inventory systems, character relationships)
    - Allows for dynamic addition and modification of game elements during runtime

These use cases benefit from OOHD's combination of hierarchical structure and object-oriented design, allowing for intuitive modeling of complex, nested structures while maintaining flexibility in object attributes and behaviors.

## Recent Improvements

1. Enhanced query parsing and execution for SQL-like queries.
2. Improved caching mechanism that ensures up-to-date results for queries with LIMIT clauses.
3. Added detailed debugging output for query execution.
4. Implemented a more robust hash function for query caching.
5. Enhanced the DynamicObject class to handle various attribute types.

## Future Enhancements

While the current implementation provides a solid foundation for an object-oriented hierarchical database, there are several areas where it could be expanded and improved:

1. **Advanced Querying**: Implement more complex query operations, such as joins between different object types and support for OR conditions.
2. **Indexing**: Add support for indexing to improve query performance, especially for large datasets.
3. **Concurrency**: Implement thread-safety and support for concurrent operations on the database.
4. **Transactions**: Add support for ACID transactions to ensure data integrity.
5. **Persistence**: Improve the current serialization method, possibly using a more standardized format like Protocol Buffers.
6. **Query Optimization**: Implement a query plan optimizer for more efficient execution of complex queries.
7. **Data Modification Operations**: Add support for INSERT, UPDATE, and DELETE operations.
8. **Flexible Query Language**: Develop a more comprehensive query language parser for even more flexible querying capabilities.

This repository contains the C++ library version of the OOHDB (see https://github.com/RPDevJesco/OOHD for the pure C++ console version), It contains the C# wrapper library project and a small test project to showcase it working. There is detailed logging included.
