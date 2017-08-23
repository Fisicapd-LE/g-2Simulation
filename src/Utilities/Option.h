

#ifndef OPTION_H_
#define OPTION_H_

// l'optional dei poveri

#include <stdexcept>

template<typename T>
class Option
{
	public:
		Option(): value(), valid(false) {};
		Option(const Option<T>& rhs)
		{
			valid = bool(rhs);
			if (*this)
			{
				value = std::move(rhs.value);
			}
		};
		Option& operator=(const Option& rhs)
		{
			valid = bool(rhs);
			if (*this)
			{
				value = std::move(rhs.value);
			}
			return *this;
		}
		Option(T value): value(value), valid(true) {};
		Option& operator=(T value)
		{
			valid = true;
			this->value = value;
		}
		
		Option(Option&& rhs)
		{
			valid = bool(rhs);
			if (*this)
			{
				value = std::move(rhs.value);
			}
			rhs.valid = false;
		};
		
		Option& operator=(Option&& rhs)
		{
			valid = bool(rhs);
			if (*this)
			{
				value = std::move(rhs.value);
			}
			rhs.valid = false;
			return *this;
		}
	
		void clear()
		{
			valid = false;
		};
		
		T valueOr(T fallback)
		{
			if (*this)
				return *this;
				
			return fallback;
		}
		
		explicit operator bool() const
		{
			return valid;
		};
		
		T& operator*()
		{
			if (*this)
				return value;
			
			throw std::runtime_error("dereferenced empty optional");
		};
		
		const T& operator*() const
		{
			if (*this)
				return value;
			
			throw std::runtime_error("dereferenced empty optional");
		};
		
		T* operator->()
		{
			if (*this)
				return &value;
			
			throw std::runtime_error("indexed empty optional");
		};
		
		const T* operator->() const
		{
			if (*this)
				return &value;
			
			throw std::runtime_error("indexed empty optional");
		};
		
	private:
		T value;
		bool valid;
		
};

template<typename T>
Option<T> No()
{
	static const Option<T> none;
	return none;
}

#endif
