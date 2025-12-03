#include "dung.h"
#include <iostream>
#include <limits>

int main() {
    try {
        DungeonEditor editor;
        editor.add_observer(std::make_shared<ConsoleObserver>());
        editor.add_observer(std::make_shared<FileObserver>("log.txt"));

        // Очистка лога при запуске
        std::ofstream("log.txt", std::ios::trunc).close();

        int choice;
        while (true) {
            std::cout << "\n=== DUNGEON EDITOR ===" << std::endl;
            std::cout << "1. Add NPC\n2. Save\n3. Load\n4. Print\n5. Battle\n0. Exit\nChoice: ";
            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            if (choice == 0) break;

            try {
                if (choice == 1) {
                    std::string type, name;
                    int x, y;
                    std::cout << "Type (Orc/Squirrel/Druid): "; std::cin >> type;
                    std::cout << "Name: "; std::cin >> name;
                    std::cout << "X: "; std::cin >> x;
                    std::cout << "Y: "; std::cin >> y;
                    editor.add_npc(type, name, x, y);
                }
                else if (choice == 2) {
                    std::string fn; std::cout << "Filename: "; std::cin >> fn;
                    editor.save_to_file(fn);
                }
                else if (choice == 3) {
                    std::string fn; std::cout << "Filename: "; std::cin >> fn;
                    editor.load_from_file(fn);
                }
                else if (choice == 4) editor.print_all();
                else if (choice == 5) {
                    double r; std::cout << "Range: "; std::cin >> r;
                    editor.battle_mode(r);
                }
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}