#pragma once

#include <string>
#include <vector>

// Defines shared state and the persistence/search contract for entity lists.
template <typename T>
class EntityList {
protected:
    bool hasLoadFromFile;
    bool hasChangeNotUpdated;
    std::vector<T> itemArr;

public:
    EntityList()
        : hasLoadFromFile(false), hasChangeNotUpdated(false), itemArr() {}

    bool isLoaded() const { return hasLoadFromFile; }
    bool hasUnsavedChanges() const { return hasChangeNotUpdated; }

    virtual bool loadFromFile(const std::string& filename) = 0;
    virtual bool saveToFile(const std::string& filename) = 0;
    virtual int searchID(const std::string& id) = 0;
    virtual ~EntityList() = default;
};
