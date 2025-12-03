#include <gtest/gtest.h>
#include <memory>
#include <sstream>
#include <vector>

#include "../npc.h"
#include "../defineNpc.h"
#include "../factory.h"
#include "../dung.h"
#include "../obs.h"

//фабрика
TEST(FactoryTest, CreateOrc) {
    auto npc = NPCFactory::create("Orc", "HOHO", 10, 20);
    EXPECT_NE(npc, nullptr);
    EXPECT_EQ(npc->get_type(), "Orc");
    EXPECT_EQ(npc->get_name(), "HOHO");
    EXPECT_EQ(npc->get_x(), 10);
    EXPECT_EQ(npc->get_y(), 20);
}

TEST(FactoryTest, CreateSquirrel) {
    auto npc = NPCFactory::create("Squirrel", "Chip", 5, 5);
    EXPECT_EQ(npc->get_type(), "Squirrel");
}

TEST(FactoryTest, CreateDruid) {
    auto npc = NPCFactory::create("Druid", "Malfurion", 50, 50);
    EXPECT_EQ(npc->get_type(), "Druid");
}

TEST(FactoryTest, InvalidType) {
    EXPECT_THROW(NPCFactory::create("Dragon", "LOL", 0, 0), std::invalid_argument);
}

TEST(FactoryTest, LoadFromStream) {
    std::stringstream ss;
    ss << "Orc HOHO 100 200";
    auto npc = NPCFactory::load_from_stream(ss);
    
    ASSERT_NE(npc, nullptr);
    EXPECT_EQ(npc->get_type(), "Orc");
    EXPECT_EQ(npc->get_name(), "HOHO");
    EXPECT_EQ(npc->get_x(), 100);
}


//визер
TEST(VisitorTest, OrcKillsDruid) {
    Orc orc("Attack", 0, 0);
    Druid druid("Defend", 0, 0);
    
    // Druid (Defender) принимает Orc (Visitor/Attacker)
    // true
    EXPECT_TRUE(druid.accept(orc));
}

TEST(VisitorTest, DruidDoesNotKillOrc) {
    Druid druid("Attacker", 0, 0);
    Orc orc("Defender", 0, 0);
    
    // Orc принимает Druid
    // false
    EXPECT_FALSE(orc.accept(druid));
}

TEST(VisitorTest, DruidKillsSquirrel) {
    Druid druid("Attacker", 0, 0);
    Squirrel squirrel("Defender", 0, 0);
    
    EXPECT_TRUE(squirrel.accept(druid));
}

TEST(VisitorTest, SquirrelIsPeaceful) {
    Squirrel squirrel("Attacker", 0, 0);
    Orc orc("Defender", 0, 0);
    Druid druid("Defender2", 0, 0);
    
    EXPECT_FALSE(orc.accept(squirrel));
    EXPECT_FALSE(druid.accept(squirrel));
}

TEST(VisitorTest, FriendlyFireDisabled) {
    Orc orc1("Orc1", 0, 0);
    Orc orc2("Orc2", 0, 0);
    EXPECT_FALSE(orc1.accept(orc2));
}

//редактор и обсервер
//создаем наблюдателя
class TestObserver : public IFightObserver {
public:
    int kill_count = 0;
    std::string last_killer_type;
    std::string last_victim_type;

    void on_fight(const std::string&, const std::string& attacker_type,
                  const std::string&, const std::string& defender_type,
                  bool defender_killed) override {
        if (defender_killed) {
            kill_count++;
            last_killer_type = attacker_type;
            last_victim_type = defender_type;
        }
    }
};

TEST(DungeonEditorTest, Validation) {
    DungeonEditor editor;
    
    //проверка координат
    EXPECT_THROW(editor.add_npc("Orc", "BadPos", -1, 0), std::invalid_argument);
    EXPECT_THROW(editor.add_npc("Orc", "BadPos2", 0, 600), std::invalid_argument);
    
    //проверка уникальности имен
    editor.add_npc("Orc", "Unique", 10, 10);
    EXPECT_THROW(editor.add_npc("Druid", "Unique", 20, 20), std::invalid_argument);
}

TEST(DungeonEditorTest, BattleProcess) {
    DungeonEditor editor;
    auto spy = std::make_shared<TestObserver>();
    editor.add_observer(spy);

    // Сценарий: Орк и Друид стоят рядом (дистанция 0)
    editor.add_npc("Orc", "Killer", 100, 100);
    editor.add_npc("Druid", "Victim", 100, 100);
    
    // Белка стоит далеко
    editor.add_npc("Squirrel", "Survivor", 400, 400);

    // Запускаем бой с радиусом 10
    editor.battle_mode(10.0);

    // Проверяем результаты через наблюдателя
    EXPECT_EQ(spy->kill_count, 1);
    EXPECT_EQ(spy->last_killer_type, "Orc");
    EXPECT_EQ(spy->last_victim_type, "Druid");
}

TEST(DungeonEditorTest, BattleRange) {
    DungeonEditor editor;
    auto spy = std::make_shared<TestObserver>();
    editor.add_observer(spy);

    // Орк и Друид на расстоянии 50 друг от друга
    // (100,100) и (100,150) -> дистанция 50
    editor.add_npc("Orc", "Killer", 100, 100);
    editor.add_npc("Druid", "Victim", 100, 150);

    // Запускаем бой с радиусом 10 (слишком мало)
    editor.battle_mode(10.0);
    EXPECT_EQ(spy->kill_count, 0);

    // Запускаем бой с радиусом 60 (достаточно)
    editor.battle_mode(60.0);
    EXPECT_EQ(spy->kill_count, 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}