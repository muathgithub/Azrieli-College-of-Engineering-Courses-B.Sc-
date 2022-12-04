import java.util.ArrayList;

public class Polynomial {

	// ---------------------------- Properties ---------------------------------//
	private ArrayList<Monom> monoms;

	// ---------------------------- Constructors ------------------------------//
	// This empty constructor initializes the ArrayList<Monom> monoms with new
	// ArrayList<>()
	public Polynomial() {

		this.monoms = new ArrayList<>();
	}

	// This constructor initializes the ArrayList<Monom> monoms with the given
	// ArrayList(ArrayList<Monom> monoms)
	// I use it when i have to make changes on the Polynomial and return a new one
	// after the changes
	public Polynomial(ArrayList<Monom> monoms) {

		this.monoms = monoms;
	}

	//------------------------------- Methods -------------------------------------//
	
	// This toString method returns a string of the Polynomial in a formated way
	// from the Monom with lowest exp(power) to the one with the uppermost
	// exp(power) [IncreasingOrder]
	public String toString() {

		// if the arrayList of monoms is empty it returns "0"
		if (this.monoms.isEmpty())
			return "0";

		String result = "";

		// Sorting the Polynomial by calling sortPolynom() [explanation below](where the
		// function is)
		sortPolynom();

		// This for goes through the arrayList of monoms one by one
		// and for each one it checks if its coeff is < 0
		// if true it adds " - " to the string then it adds the monom after cutting the
		// "-" that cums from the toString of the Monom class
		// else it adds " + " to the string then it adds the monom
		for (Monom monom : this.monoms) {
			if (monom.getCoeff() < 0)
				result = result.concat(" - " + monom.toString().substring(1));
			else
				result = result.concat(" + " + monom.toString());
		}

		// It returns the result(String) after cutting the "+" if the first coeff is
		// positive because there is no need for it
		// substring(3) because there is spaces before and after the + (" + ") and
		// before the - there is space so substring(1)
		return result.charAt(1) == '+' ? result.substring(3) : result.substring(1);
	}

	// This method evaluates the Polynomial with the given value (double x)
	// By going through the arrayList of monoms and for each one calls the method
	// evaluates with the value(x)
	// and it adds all the result to the variable sum then it returns it
	public double evaluate(double x) {
		double sum = 0;

		for (Monom monom : this.monoms)
			sum += monom.evaluate(x);

		return sum;
	}

	// This method adds the given monom to the arrayList of monoms according to the
	// written conditions
	public void addMonom(Monom m) {

		// boolean variable that means that there is a monom with the same exp(power) of
		// the given monom in the arrayList or not
		boolean found = false;
		Monom tempMonom;

		// This for goes through the arrayList and for each elements...
		for (Monom monom : this.monoms) {
			// if the current monom has the same exp(power) of the given one it modifies the
			// coeff for the exists one
			if (monom.getExp() == m.getExp()) {
				// This if checks if after the sum the coeff becomes 0 so it removes the exist monom and it breaks
				if(monom.getCoeff() + m.getCoeff() == 0) {
					this.monoms.remove(monom);
					
					// found = true that means that there is a monom with the same exp(power) in the
					// arrayList so we don't have to add a new one
					found = true;
					break;
				}
				
				// adds the coeffs of the to monoms and sets the value for the coeff property of
				// the exists(in the arralyList) monom
				tempMonom = new Monom(monom.getCoeff() + m.getCoeff(), monom.getExp());
				this.monoms.set(this.monoms.indexOf(monom), tempMonom);
				

				// found = true that means that there is a monom with the same exp(power) in the
				// arrayList so we don't have to add a new one
				found = true;
				break;
			}
		}

		// if the monom exp(power) desen't exists we add it to the arrayList
		if (!found) {
			this.monoms.add(m);
		}
	}

	// This method returns a new object of Polynomial
	// with the result of the derivative for 1 time of this Polynomial
	public Polynomial derivative() {

		Polynomial derivativePolynom = new Polynomial();

		// if the maxPower in the Polynomial is 0 it returns the new Polynomial() with
		// the empty arrayList() [0] (there is no mean for derivative)
		if (maxPow() == 0)
			return new Polynomial();

		// This for applies the mathematics of the derivative for each monom and it adds
		// it to the new Polynomial() [derivativePolynom]
		for (Monom monom : this.monoms) {
			if (monom.getExp() != 0)
				derivativePolynom.addMonom(new Monom(monom.getCoeff() * monom.getExp(), monom.getExp() - 1));
		}

		return derivativePolynom;
	}

	// This method returns a new object of Polynomial
	// with the result of the derivative for n time of this Polynomial
	public Polynomial derivative(int n) {

		// if the maxPower in the Polynomial is < 0 it returns the new Polynomial() with
		// the empty arrayList() [0] (there is no mean for derivative) at the end it
		// will become zero
		if (maxPow() < n)
			return new Polynomial();

		// This line creates a copy of this Polynomial for making changes on it
		Polynomial derivativePolynom = new Polynomial((ArrayList<Monom>) this.monoms.clone());

		// if n <= 0 it returns the copy [derivativePolynom] with no changes
		if (n <= 0)
			return derivativePolynom;

		// else this for applies the derivative() method on the copy [derivativePolynom]
		// n times
		for (int i = 0; i < n; i++) {
			derivativePolynom = derivativePolynom.derivative();
		}

		return derivativePolynom;
	}

	// This method receives a string of a Polynomial and it pares it to a real one
	// (Polynomial object with arrayList of monoms)
	public static Polynomial parsePolynomial(String s) {

		// creates a new Polynomial() object and a new tempMonom
		Polynomial newPolynomial = new Polynomial();
		Monom tempMonom = new Monom(1, 1);

		// This line splits the array where there is a one or more a spaces and it after
		// that
		// it joins all the splited parts with each other without any divider "" so we
		// get the string with no spaces
		// "\\s+" (regex) means one space or more
		String str = s.join("", s.split("\\s+"));

		// This lien returns an array of Strings as a result of splitting the string str
		// before each + or -
		// "(?=[+-])" (regex) means before a/ followed by + or -
		String[] strArr = str.split("(?=[+-])");

		// This for foes through each string in strArr...
		for (String tempStr : strArr) {
			// It splits the string where there is x Or X followed by ^ one time or not at
			// all
			// 2 means to return an array of 2 elements if there was a need to split so i
			// can know if the exp(power) is one or zero
			// "[xX][\\^]?" (regex) means x or X followed by "^" ("?" means one time or not
			// at all)
			// if there was no x or X i get an array with length of 1 because there was no
			// split so i know that the exp(power) is 0
			// if i get array with 2 elements and the second tempStrArr[1] is with the
			// length of 0 that means that there was a x or X
			// so we made a split which mean that the power is 1 not 0
			String[] tempStrArr = tempStr.split("[xX][\\^]?", 2);

			// if the monom with exp(power) 0
			// we create a new Polynomial with the value of
			// parseNum(tempStrArr[0])[Explanation below] for the coeff and 0 for the
			// exp(power)
			if (tempStrArr.length == 1) {

				tempMonom = new Monom(parseNum(tempStrArr[0]), 0);
			}

			// else if the monom with exp(power) != 0
			// we create a new Polynomial with the value of
			// parseNum(tempStrArr[0])[Explanation below] for the coeff and (int)
			// parseNum(tempStrArr[1]) [Explanation below] for the exp(power)
			else if (tempStrArr.length == 2) {
				tempMonom = new Monom(parseNum(tempStrArr[0]), (int) parseNum(tempStrArr[1]));
			}

			// Then we add the monom to the newPolynomial by calling the method
			// addMonom(tempMonom)
			newPolynomial.addMonom(tempMonom);
		}

		return newPolynomial;

	}

	// This method parse the givven string to a number according to the conditions
	// for the returned strings from the splits in parse parsePolynomial() method
	private static double parseNum(String tempStrOfArr) {

		double result;

		// This if returns 1 it the tempStrOfArr.length() == 0
		// which means that the power is 1 or the coeff is one but not written if the
		// string is empty as i explained in parse parsePolynomial() method
		if (tempStrOfArr.length() == 0) {
			result = 1;
		}

		// else if the tempStrOfArr.length() == 1 it enters to a switch
		// because that means that the string is + or - or a number between 0-9
		// So i initialize the result(double) according to that
		else if (tempStrOfArr.length() == 1) {

			switch (tempStrOfArr) {

			case "+":
				result = 1;
				break;

			case "-":
				result = -1;
				break;

			default:
				result = Double.parseDouble(tempStrOfArr);
				break;
			}
		} else { // else it means that the string is a normal number with two digits or more so i
					// conver it to double using parseDouble
			result = Double.parseDouble(tempStrOfArr);
		}

		return result;

	}

	// This method sorts the Polynomial arrayList in increasing order by using
	// bubbleSort Algorithm
	private void sortPolynom() {

		for (int i = 0; i < this.monoms.size() - 1; i++) {

			for (int k = 0; k < this.monoms.size() - (i + 1); k++) {

				if (this.monoms.get(k).getExp() > this.monoms.get(k + 1).getExp())
					swap(k, k + 1);
			}
		}
	}

	// This method swaps two elements in the arrayList
	// according to the given indexes for the elements (int firstInd, int secondInd)
	private void swap(int firstInd, int secondInd) {

		Monom tempMonom = this.monoms.get(firstInd);

		this.monoms.set(firstInd, this.monoms.get(secondInd));
		this.monoms.set(secondInd, tempMonom);
	}

	// This method returns the maxPower of the monoms in the ArrayList of the monoms
	// in another words it returns the power of the Polynomial
	private int maxPow() {
		if (this.monoms.isEmpty())
			return 0;

		int maxPower = this.monoms.get(0).getExp();

		for (Monom monom : this.monoms) {
			if (monom.getExp() > maxPower)
				maxPower = monom.getExp();
		}

		return maxPower;
	}
}
