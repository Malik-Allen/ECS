#ifndef TEMPLATEHELPER_H
#define TEMPLATEHELPER_H

// Template Parameter Compile Constraint, Thanks Bjarne Stroustrup: https://www.stroustrup.com/bs_faq2.html#constraints
template<class T, class B> struct CanConvert_From
{
	static void constraints( T* p ) { B* pb = p; }
	// Complile-time check to see if class T can be converted to class B, valid for derivation check of class T from class B
	CanConvert_From() { void( *p )( T* ) = constraints; }
};

#endif // !TEMPLATEHELPER_H
