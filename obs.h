#pragma once
#include <string>
#include <iostream>
#include <fstream>

//интерфейс
class IFightObserver {
public:
    virtual void on_fight(const std::string &attacker_name, const std::string& attacker_type,
                          const std::string &defender_name, const std::string& defender_type,
                          bool defender_killed) = 0;
    virtual ~IFightObserver() = default;
};

//вывод в консоль
class ConsoleObserver : public IFightObserver {
public:
    void on_fight(const std::string& attacker_name, const std::string& attacker_type,
                  const std::string& defender_name, const std::string& defender_type,
                  bool defender_killed) override {
        if (defender_killed) {
            std::cout << "(БОЙ) " << attacker_type << " \"" << attacker_name
                      << "\" убит " << defender_type << " \"" << defender_name << "\"" << std::endl;
        }
    }
};

//запись в файл
class FileObserver : public IFightObserver {
private:
    std::string filename;
public:
    FileObserver(const std::string& filename) : filename(filename) {}

    void on_fight(const std::string& attacker_name, const std::string& attacker_type,
                  const std::string& defender_name, const std::string& defender_type,
                  bool defender_killed) override {
        if (defender_killed) {
            std::ofstream log(filename, std::ios::app);
            if (log.is_open()) {
                log << "(БОЙ) " << attacker_type << " \"" << attacker_name
                    << "\" убит " << defender_type << " \"" << defender_name << "\"" << std::endl;
                log.close();
            }
        }
    }
};