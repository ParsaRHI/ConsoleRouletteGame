#include "item.h"
#include <utility>
#include <vector>
#include <unistd.h>
#include "GRand.h"
#include <algorithm>
enum ammoTypes{
    FAKE,REAL
};

std::string ammoTypeToString(ammoTypes type) {
    switch (type) {
        case FAKE: return "FAKE";
        case REAL: return "REAL";
        default: return "UNKNOWN";
    }
}

class gun;

void REFILL(gun &playerGun);

void useSpecial(gun &playerGun);

class Stats{
    std::string name;
    float health;
    std::vector<Item> items;
public:
    bool isEnemy;
    explicit Stats(std::string name_val,float health_val,bool isEnemy_val):name(std::move(name_val)),health(health_val),isEnemy(isEnemy_val){}
    [[nodiscard]] float getHealth() const{return health;}
    void setHealth(float health_val){health=health_val;}
    [[nodiscard]] std::string_view getName() const{return name;}

    void damage(float damage_val) {health -= damage_val;}
    void heal(float heal_val)
    {
        health += heal_val;
        std::cout << name << " healed by " << heal_val << " health points.\n";
    }

    void addItem(const Item &item) {
        items.push_back(item);
    }

    void useItem(gun &playerGun) {
        if (items.empty()) {
            std::cout << "No items to use!\n";
            return;
        }

        std::cout << "Choose an item to use:\n";
        for (size_t i = 0; i < items.size(); ++i) {
            std::cout << i + 1 << ". ";
            items[i].getItemColor();
            std::cout << "\n";
        }

        int choice;
        std::cout << dye::grey("Answer: ");
        std::cin >> choice;
        if (choice > 0 && choice <= items.size()) {
            Item &selectedItem = items[choice - 1];

            switch (selectedItem.getType()) {
                case Item::HEALING:
                    heal(selectedItem.getValue());
                    break;
                case Item::SEE:
                    useSpecial(playerGun);
                    break;
                case Item::REFILL:
                    REFILL(playerGun);
                    break;
                default:
                    std::cout << "Item had no effect.\n";
                    break;
            }

            // Remove the used item
            items.erase(items.begin() + (choice - 1));
        } else {
            std::cout << "Invalid choice.\n";
        }
    }


};
class gun{
    int fakeAmmo;
    int realAmmo;
public:
    std::vector<ammoTypes> totalAmmo;
    void generateAmmo() {

        totalAmmo.clear();

        for (int i = 0; i < fakeAmmo; ++i) {
            totalAmmo.push_back(FAKE);
        }

        for (int i = 0; i < realAmmo; ++i) {
            totalAmmo.push_back(REAL);
        }

        GRand r;
        std::shuffle(totalAmmo.begin(), totalAmmo.end(), r);
    }
    explicit gun(int fakeAmmo_val,int realAmmo_val):fakeAmmo(fakeAmmo_val),realAmmo(realAmmo_val){
        generateAmmo();
        //std::cout << "Started !\nTotal Ammo: " << totalAmmo.size()<<dye::bright_white("\nFake Ammos: ")<<fakeAmmo<<dye::yellow("\nReal Ammos:")<<realAmmo <<"\n";
    }
    [[nodiscard]] int getFakeAmmo() const{
        std::vector<ammoTypes> tempTotal;
        for(auto i:totalAmmo){

            if (i == 0){
                tempTotal.push_back(i);
            }
            else{

            }
        }
        return (tempTotal.size());}
    [[nodiscard]] int getRealAmmo() const{
        std::vector<ammoTypes> tempTotal;
        for(auto i:totalAmmo){

            if (i == 1){
                tempTotal.push_back(i);
            }
            else{

            }
        }
        return (tempTotal.size());
    }

    void shot(Stats &stats,Stats &enemyStats,bool self) {
        if (totalAmmo.empty()) {
            std::cout << dye::red("No Ammo Left!\n");
            return;
        }

        ammoTypes currentBullet = totalAmmo.back();  // Get the last bullet
        totalAmmo.pop_back();  // Remove the last bullet after firing

        if (currentBullet == REAL) {
            std::cout << dye::red("BANG\n");
            stats.damage(50);
            std::cout << dye::yellow(stats.getName()) << dye::red(" HAS TAKEN 50 DAMAGE !!!!\n");
        } else {
            std::cout << dye::grey("PA\n");
            std::cout << dye::bright_white_on_grey("Bullet was fake!\n");
            sleep(2);

            if (stats.isEnemy and !self){
                std::cout << dye::red("Enemy has taken the gun because of you mistake\n");
                std::cout << dye::bright_white("He Is Aiming At You !\n");
                sleep(1);
                shot(enemyStats,stats,false);


            }
        }
    }
};
void useSpecial(gun &playerGun){
    ammoTypes currentBullet = playerGun.totalAmmo.back();
    std::cout <<"You Used A magnifying glass and Current Ammo Is "<< dye::yellow(ammoTypeToString(currentBullet)) << "\n";
}
void REFILL(gun &playerGun){
    playerGun.generateAmmo();
    std::cout << "\nTotal Ammo: " << playerGun.totalAmmo.size()<<dye::bright_white("\nFake Ammos: ")
    <<playerGun.getFakeAmmo()<<dye::yellow("\nReal Ammos:")<<playerGun.getRealAmmo() <<"\n";
}
void rewardRandomItems(Stats& player) {
    std::vector<Item> possibleItems = {
            Item(Item::HEALING, 30, "Medkit", COMMON),
            Item(Item::HEALING, 50, "Potion", EPIC),
            Item(Item::SEE, 50, "Magnifying Glass", RARE),
            Item(Item::SEE, 50, "Magnifying Glass", RARE),
            Item(Item::REFILL, 50, "Ammo Refiller", LEGENDARY)
    };

    GRand r;

    // Number of items to choose (adjust as needed)
    const int numberOfItemsToChoose = 3;

    std::cout << "Items awarded after level:\n";
    for (int i = 0; i < numberOfItemsToChoose; ++i) {
        // Randomly choose an item from the list
        int randomIndex = r.i(possibleItems.size()-1);
        Item chosenItem = possibleItems[randomIndex];
        player.addItem(chosenItem);

        // Print chosen item details
        std::cout << "Item: " ;
        chosenItem.getItemColor();
        std::cout<< "\n";
    }
}

int fakeAmmos;
int realAmmos;
gun shotGun(fakeAmmos,realAmmos);
void play(Stats player,Stats enemy,bool regen,bool refill){
    if(refill){
        GRand r;
        GRand f;
        fakeAmmos={1+r.i(3)};
        realAmmos={1+f.i(3)};
        shotGun=gun (fakeAmmos,realAmmos);
    }
    else{
        fakeAmmos={shotGun.getFakeAmmo()};
        realAmmos={shotGun.getRealAmmo()};
        shotGun=gun (fakeAmmos,realAmmos);
    }

    if(regen){
        std::cout << dye::aqua_on_red("HELLO,IT'S A SIMPLE GAME\nIF YOU LOSE I WILL GET YOUR LIFE\nIF I LOSE YOU WILL GET ITEMS!\n");
        player.setHealth(100);
        enemy.setHealth(150);
    }

    while(player.getHealth() > 0){
        std::cout << dye::green("Player health: ") << dye::green(player.getHealth()) << "\n";
        sleep(1);
        std::cout << dye::red("Enemy health: ") << dye::red(enemy.getHealth()) << "\n";
        sleep(1);
            std::cout << "Total Ammo:" << shotGun.totalAmmo.size()<<dye::bright_white("\nFake Ammos: ")
                      <<shotGun.getFakeAmmo()<<dye::yellow("\nReal Ammos: ")<<shotGun.getRealAmmo() <<"\n";

        sleep(1);
        std::cout << "1-Shoot Enemy\n2-Shoot Self\n3-Use Item";
        std::cout << dye::grey("\nAnswer: ");
        int answer;
        std::cin >> answer;
        switch (answer) {
            case 1:
                shotGun.shot(enemy,player,false);
                break;
            case 2:
                shotGun.shot(player,enemy,true);
                break;
            case 3:
                player.useItem(shotGun);
                break;
            default:
                play(player,enemy,false,false);
                break;

        }


        if (player.getHealth() <= 0) {
            std::cout << dye::black_on_red("YOU LOST!\n");
            play(player,enemy,true,true);
            break;
        } else if (enemy.getHealth() <= 0) {
            std::cout << dye::black_on_green("VICTORY!\n");
            rewardRandomItems(player);
            play(player,enemy,true,true);
            break;
        }

        if (shotGun.totalAmmo.empty()) {
            sleep(1);
            std::cout << dye::yellow("Out of Ammo! Next Round !.\n");
            play(player,enemy,false,true);
            break;
        }
    }
}

void intro(){
    Stats player("You",100,false);
    Stats enemy("Enemy",150,true);
    player.addItem(Item (Item::HEALING, 50, "Potion", EPIC));
    player.addItem(Item (Item::SEE, 50, "Magnifying Glass", RARE));
    play(player,enemy,true,true);
}

int main(){
    intro();
}