#pragma once
#include "defineNpc.h"
#include <memory>
#include <stdexcept>

class NPCFactory {
public:
    static std::shared_ptr<NPC> create(const std::string& type, const std::string& name, int x, int y) {
        if (type == "Orc") {
            return std::make_shared<Orc>(name, x, y);
        } else if (type == "Squirrel") {
            return std::make_shared<Squirrel>(name, x, y);
        } else if (type == "Druid") {
            return std::make_shared<Druid>(name, x, y);
        }
        throw std::invalid_argument("Unknown NPC type: " + type);
    }

    static std::shared_ptr<NPC> load_from_stream(std::istream& is) {
        std::string type, name;
        int x, y;
        if (is >> type >> name >> x >> y) {
            return create(type, name, x, y);
        }
        return nullptr;
    }
};