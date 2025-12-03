#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

class Orc;
class Squirrel;
class Druid;


class NPCVisitor {
public:
    virtual bool visit(Orc &orc) = 0;
    virtual bool visit(Squirrel &squirrel) = 0;
    virtual bool visit(Druid &druid) = 0;
    virtual ~NPCVisitor() = default;
};


class NPC : public NPCVisitor {
protected:
    std::string name;
    int x, y;

public:
    NPC(const std::string &name, int x, int y) : name(name), x(x), y(y) {}
    virtual ~NPC() = default;

    virtual std::string get_type() const = 0;

    virtual void print() const {
        std::cout << get_type() << " \"" << name << "\" at (" << x << ", " << y << ")" << std::endl;
    }

    virtual void save(std::ostream& os) const {
        os << get_type() << " " << name << " " << x << " " << y << std::endl;
    }

    //принимаем визера
    virtual bool accept(NPCVisitor& visitor) = 0;

    std::string get_name() const { return name; }
    int get_x() const { return x; }
    int get_y() const { return y; }

    double distance_to(const NPC& other) const {
        int dx = x - other.x;
        int dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }
};

