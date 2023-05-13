#include <iostream>

/*******************************************************************************
 * Sword and Bow have the same implicit interface, but they do not inherit from
 * any common base class. These can easily fit into the same type-erased model.
 ******************************************************************************/

class Sword
{
public:
	Sword()
	{
		std::cout << "Sword()\n";
	}
	
	Sword(const Sword&)
	{
		std::cout << "Sword(copy)\n";
	}
	
	Sword(Sword&&)
	{
		std::cout << "Sword(move)\n";
	}
	
	~Sword()
	{
		std::cout << "~Sword()\n";
	}
	
	void attack()
	{
		std::cout << "\tAttacked with a sword!\n";
	}
};

class Bow
{
	int _arrows;
public:
	Bow(int arrows) : _arrows{arrows}
	{
		std::cout << "Bow()\n";
	}
	
	Bow(const Bow& rhs) : _arrows{rhs._arrows}
	{
		std::cout << "Bow(copy)\n";
	}
	
	Bow(Bow&& rhs) : _arrows{rhs._arrows}
	{
		rhs._arrows = 0;
		std::cout << "Bow(move)\n";
	}
	
	~Bow()
	{
		std::cout << "~Bow()\n";
	}
	
	void attack()
	{
		std::cout << "\tAttacked with a bow; " << --_arrows << " arrows left.\n";
	}
};

/*******************************************************************************
 * Staff does not share the implicit interface, as its performAttack() function
 * name differs; it needs to be adapted with an Adaptor Design Pattern wrapper.
 ******************************************************************************/

class Staff
{
public:
	Staff()
	{
		std::cout << "Staff()\n";
	}
	
	Staff(const Staff&)
	{
		std::cout << "Staff(copy)\n";
	}
	
	Staff(Staff&&)
	{
		std::cout << "Staff(move)\n";
	}
	
	~Staff()
	{
		std::cout << "~Staff()\n";
	}
	
	// The name of this function does not match the implicit interface
	// (duck-typing) required by a Weapon::Model<T>::attack().
	void performAttack()
	{
		std::cout << "\tAttacked with a staff!\n";
	}
};

class StaffAdaptor
{
	Staff _instance;
public:
	void attack()
	{
		_instance.performAttack();
	}
};

/*******************************************************************************
 * Weapon is a type-erased class (it is not a template!). It hides an explicit
 * interface class IConcept, and a class template Model<T> for any type T that
 * we intend to erase if it matches the implicit interface.
 ******************************************************************************/

class Weapon
{
	struct IConcept
	{
		virtual void attack() = 0;
		virtual ~IConcept() = default;
	};
	
	IConcept* _concept;
	
	template <typename T>
	class Model : public IConcept
	{
		T _instance;
	public:
		template <typename Arg>
		Model(Arg&& arg) :
			_instance{
				// Perfect forwarding
				std::forward<Arg>(arg)
			}
		{
		}
		
		void attack() override
		{
			_instance.attack();
		}
	};
	
	Weapon(IConcept* concept) :
		_concept{concept}
	{
	}

public:
	template <typename T, typename Arg>
	static Weapon create(Arg&& arg)
	{
		return Weapon
		{
			new Model<T>
			{
				// Perfect forwarding
				std::forward<Arg>(arg)
			}
		};
	}
	
	void attack()
	{
		_concept->attack();
	}
	
	virtual ~Weapon()
	{
		delete _concept;
	}
};

// Not a template, even though the actual type of a weapon is not known!
void attack(Weapon w)
{
	w.attack();
}

int main()
{
	Sword s;
	Bow b{10};
	StaffAdaptor t;
	
	std::cout << "Attack with the copies!\n";
	attack(Weapon::create<Sword>(s));
	attack(Weapon::create<Bow>(b));
	attack(Weapon::create<StaffAdaptor>(t));
	
	std::cout << "Attack with the originals!\n";
	attack(Weapon::create<Sword>(std::move(s)));
	attack(Weapon::create<Bow>(std::move(b)));
	attack(Weapon::create<StaffAdaptor>(std::move(t)));
	
	std::cout << "Attack with a conversion-constructed bow!\n";
	attack(Weapon::create<Bow>(100));
}
