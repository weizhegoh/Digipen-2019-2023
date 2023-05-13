#include <iostream>

//==============================================================================

class Item
{
	int _id;
public:
	Item(int id) : _id{id}
	{
	}
	
	virtual ~Item() = default;
	
	int getId() const
	{
		return _id;
	}
};

//==============================================================================

class ItemToBuy :
	public virtual Item
{
	int _price;
public:
	ItemToBuy(int id, int price) :
		Item{id},	// Not called if ItemToBuy is not the most derived class.
		_price{price}
	{
	}
	
	// No explicit destructor needed:
	// - nothing particular to do
	// - no user-defined destructor; one will be synthesized
	// - virtual-ness is inherited
	
	int getPrice() const
	{
		return _price;
	}
};

//==============================================================================

class ItemToSell :
	public virtual Item
{
	int _price;
public:
	ItemToSell(int id, int price) :
		Item{id},	// Not called if ItemToSell is not the most derived class.
		_price{price}
	{
	}
	
	// No explicit destructor needed:
	// - Nothing particular to do.
	// - No user-defined destructor; one will be synthesized.
	// - Virtual-ness is inherited.
	
	int getPrice() const
	{
		return _price;
	}
};

//==============================================================================

class Sword :
	public ItemToBuy,
	public ItemToSell
{
	int _attack;
public:
	Sword(int id, int priceBuy, int priceSell, int attack) :
		Item{id},	// Not called if Sword is not the most derived class.
		ItemToBuy{id, priceBuy},
		ItemToSell{id, priceSell},
		_attack{attack}
	{
	}
	
	int getAttack() const
	{
		return _attack;
	}
};

//==============================================================================

void print()
{
	Sword sword{0xABCD, 16, 8, 0x1234};
	ItemToBuy& itb = sword;
	
	std::cout
		// Not ambiguous due to virtual inheritance.
		<< "ID:\t\t" << sword.getId() << "\n"
		// Ambiguous; use a non-ambiguous base class reference...
		<< "Price to buy:\t" << itb.getPrice() << "\n"
		// ...or score the member name fully.
		<< "Price to sell:\t" << sword.ItemToSell::getPrice() << "\n"
		<< "Attack:\t\t" << sword.getAttack() << std::endl;
}

int main()
{
	print();
	
	std::cout
		<< "Item:\t\t" << sizeof(Item) << "\n"
		<< "ItemToBuy:\t" << sizeof(ItemToBuy) << "\n"
		<< "ItemToSell:\t" << sizeof(ItemToSell) << "\n"
		<< "Sword:\t\t" << sizeof(Sword) << std::endl;
}
