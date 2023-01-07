 
#ifndef FREEIMAGE_FIRATIONAL_H
#define FREEIMAGE_FIRATIONAL_H
 
class FIRational {
private:
	/// numerator
	LONG _numerator;
	/// denominator
	LONG _denominator;

public:
	/// Default constructor
	FIRational();

	/// Constructor with longs
	FIRational(LONG n, LONG d = 1);

	/// Constructor with FITAG
	FIRational(const FITAG *tag);

	/// Constructor with a float
	FIRational(float value);

	/// Copy constructor
	FIRational (const FIRational& r);

	/// Destructor
	~FIRational();

	/// Assignement operator
	FIRational& operator=(FIRational& r);

	/// Get the numerator
	LONG getNumerator();

	/// Get the denominator
	LONG getDenominator();

	/// Converts rational value by truncating towards zero
	LONG truncate() {
		// Return truncated rational
		return _denominator ? (LONG) (_numerator / _denominator) : 0;
	}

	/**@name Implicit conversions */
	//@{	
	short shortValue() {
		return (short)truncate();
	}
	int intValue() {
		return (int)truncate();
	}
	LONG longValue() {
		return (LONG)truncate();
	}
	float floatValue() {
		return _denominator ? ((float)_numerator)/((float)_denominator) : 0;
	}
	double doubleValue() {
		return _denominator ? ((double)_numerator)/((double)_denominator) : 0;
	}
	//@}

	/// Checks if this rational number is an integer, either positive or negative
	BOOL isInteger();

	/// Convert as "numerator/denominator"
	std::string toString();

private:
	/// Initialize and normalize a rational number
	void initialize(LONG n, LONG d);

	/// Calculate GCD
	LONG gcd(LONG a, LONG b);
	
	/// Normalize numerator / denominator 
	void normalize();

};

#endif // FREEIMAGE_FIRATIONAL_H

