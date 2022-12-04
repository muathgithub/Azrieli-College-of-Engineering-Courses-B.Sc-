
public class Monom {
	
	
	//----------------------- Properties ----------------------------//
	private double coeff;
	private int exp;
	
	
	//----------------------- Constructors --------------------------//
	
	//This constructor starts the Monom with the given arguments(double coeff, int exp)
	//if coeff == 0 it replace it with 1 else it keeps the given value then it initialize the properties with it
	//if exp(power) < 0 it replace it with 1 else it keeps the given value then it initialize the properties with it
	public Monom(double coeff, int exp) {
		
		this.coeff = (coeff == 0)? 1 : coeff;
		this.exp = (exp < 0)? 1 : exp;
	}
	
	
	//----------------------- Methods ----------------------------//
	
	//This toString method prints the Monom in a specific format
	public String toString() {
		
		String result = "";
		
		// if the | this.coeff | equlas 1 it checks if its < 0 then it adds (-) to the result (String) 
		if(Math.abs(this.coeff) == 1) {
			if(this.coeff < 0)
				result = result.concat("-");
		} else { // else it adds the value of the this.coeff to the result (String) after it converts it to string by using the method vlaueOf() 
			result = result.concat(String.valueOf(this.coeff));
		}
		
		//This if checks if the exp(power) is not equal 0
		//if true it adds "X" to the result (String)
		if(this.exp != 0) {
			
			result = result.concat("X");
			
			//This if checks if the exp(power) != 1 so there is aneed for the symbol "^" so it adds it to the string and it adds the this.exp(power) after it to the result (string)
			if(this.exp != 1)
				result = result.concat("^" + this.exp);
			
		} else if(Math.abs(this.coeff) == 1) {//This else if ckecks if the the coeff == 1 it adds "1.0" to the result(String) because the exp(power) is 0
			
			result = result.concat("1.0");
		}
		
		return result;
	}
	
	
	//This method evaluate the result of the monom according tho the given value (double x)
	public double evaluate(double x) {
		
		return this.coeff * Math.pow(x, this.exp);
	}
	
	
	//----------------------- Setters && Getters ----------------------------//

	public double getCoeff() {
		return coeff;
	}
	public int getExp() {
		return exp;
	}
	
}
