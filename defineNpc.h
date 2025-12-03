#pragma once
#include "npc.h"

class Orc : public NPC {
public:
    Orc(const std::string &name, int x, int y) : NPC(name, x, y) {}

    std::string get_type() const override { return "Orc"; }

    bool accept(NPCVisitor &visitor) override {
        return visitor.visit(*this);
    }

    //атаки Орка:
    bool visit(Orc &orc) override { return false; }
    bool visit(Squirrel &squirrel) override { return false; }
    bool visit(Druid &druid) override { return true; }
};

class Squirrel : public NPC {
public:
    Squirrel(const std::string &name, int x, int y) : NPC(name, x, y) {}

    std::string get_type() const override { return "Squirrel"; }

    bool accept(NPCVisitor &visitor) override {
        return visitor.visit(*this);
    }

    //белка не атакует :(
    bool visit(Orc &orc) override { return false; }
    bool visit(Squirrel &squirrel) override { return false; }
    bool visit(Druid &druid) override { return false; }
};

class Druid : public NPC {
public:
    Druid(const std::string &name, int x, int y) : NPC(name, x, y) {}

    std::string get_type() const override { return "Druid"; }

    bool accept(NPCVisitor &visitor) override {
        return visitor.visit(*this);
    }

    // Логика атаки Друида:
    bool visit(Orc &orc) override { return false; }
    bool visit(Squirrel &squirrel) override { return true; }
    bool visit(Druid &druid) override { return false; }
};
