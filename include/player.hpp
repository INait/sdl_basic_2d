#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <random>
#include <ctime>

std::default_random_engine generator( (unsigned int) time (0) );
std::uniform_int_distribution<int> distribution(1,100);

enum Skills
{
	STRENGTH = 0,
	DEXTERITY = 1
};

class Player;
typedef std::shared_ptr< Player > PlayerPtr;

class Player
{
public:
	Player() { }
	virtual ~Player() { }
	virtual void Attack( PlayerPtr opponent )
	{
		int dice_roll = distribution(generator);
		// check opponent evasion
		if (dice_roll < opponent->skills[ Skills::DEXTERITY ])
		{
			std::cout << this->name << " attack evaded by " << opponent->name << std::endl;
		}
		else
		{
			opponent->hitpoints -= this->skills[ Skills::STRENGTH ];
			std::cout << this->name << " inflict " << this->skills[ Skills::STRENGTH ] << " damage on " << opponent->name << std::endl;;
		}
	}

	std::vector< int > skills;
	int hitpoints;

	std::string name;
};

class Monster : public Player
{
public:
	Monster()
	{
		skills.push_back( 6 ); // strength
		skills.push_back( 10 ); // dexterity

		hitpoints = 100;

		name = "monster";
	}

	virtual ~Monster() { }
private:

};

class Human : public Player
{
public:
	Human()
	{
		skills.push_back( 10 ); // strength
		skills.push_back( 5 ); // dexterity

		hitpoints = 200;

		name = "player";
	}
	virtual ~Human() { }
private:

};

