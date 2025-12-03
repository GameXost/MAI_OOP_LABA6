#pragma once
#include "npc.h"
#include "factory.h"
#include "obs.h"
#include <vector>
#include <algorithm>

class DungeonEditor {
private:
    std::vector<std::shared_ptr<NPC>> npcs;
    std::vector<std::shared_ptr<IFightObserver>> observers;

    void notify_observers(const std::string& attacker_name, const std::string& attacker_type,
                          const std::string& defender_name, const std::string& defender_type,
                          bool defender_killed) {
        for (auto& observer : observers) {
            observer->on_fight(attacker_name, attacker_type, defender_name, defender_type, defender_killed);
        }
    }

public:
    void add_npc(const std::string& type, const std::string& name, int x, int y) {
        if (x < 0 || x > 500 || y < 0 || y > 500) {
            throw std::invalid_argument("Coordinates must be within [0, 500]");
        }
        for (const auto& npc : npcs) {
            if (npc->get_name() == name) {
                throw std::invalid_argument("NPC with name \"" + name + "\" already exists");
            }
        }
        npcs.push_back(NPCFactory::create(type, name, x, y));
        std::cout << "Added " << type << " \"" << name << "\" at (" << x << ", " << y << ")" << std::endl;
    }

    void save_to_file(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) throw std::runtime_error("Cannot open file for writing: " + filename);
        
        for (const auto& npc : npcs) {
            npc->save(file);
        }
        file.close();
        std::cout << "Saved " << npcs.size() << " NPCs to " << filename << std::endl;
    }

    void load_from_file(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) throw std::runtime_error("Cannot open file for reading: " + filename);
        
        npcs.clear();
        while (file) {
            auto npc = NPCFactory::load_from_stream(file);
            if (npc) npcs.push_back(npc);
        }
        file.close();
        std::cout << "Loaded " << npcs.size() << " NPCs from " << filename << std::endl;
    }

    void print_all() const {
        std::cout << "\n=== NPC List (" << npcs.size() << ") ===" << std::endl;
        for (const auto& npc : npcs) {
            npc->print();
        }
        std::cout << std::endl;
    }

    void add_observer(std::shared_ptr<IFightObserver> observer) {
        observers.push_back(observer);
    }

    void battle_mode(double range) {
        std::cout << "\n=== BATTLE MODE (Range: " << range << "m) ===" << std::endl;
        std::vector<size_t> killed_indices;

        for (size_t i = 0; i < npcs.size(); ++i) {
            if (std::find(killed_indices.begin(), killed_indices.end(), i) != killed_indices.end()) continue;

            for (size_t j = 0; j < npcs.size(); ++j) {
                if (i == j) continue;
                if (std::find(killed_indices.begin(), killed_indices.end(), j) != killed_indices.end()) continue;

                if (npcs[i]->distance_to(*npcs[j]) <= range) {
                    // !!! Visitor Pattern Magic !!!
                    bool defender_killed = npcs[j]->accept(*npcs[i]);

                    if (defender_killed) {
                        notify_observers(npcs[i]->get_name(), npcs[i]->get_type(),
                                         npcs[j]->get_name(), npcs[j]->get_type(), true);
                        killed_indices.push_back(j);
                    }
                }
            }
        }

        std::sort(killed_indices.rbegin(), killed_indices.rend());
        for (size_t idx : killed_indices) {
            npcs.erase(npcs.begin() + idx);
        }

        std::cout << "Killed: " << killed_indices.size() << ", Survivors: " << npcs.size() << std::endl;
    }
};