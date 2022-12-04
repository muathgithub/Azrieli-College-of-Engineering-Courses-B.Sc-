package Ex3_Question1;

import java.util.ArrayList;

public class CreditCardStrategy implements PaymentStrategy {

	private String name;
	private String cardNumber;
	private String cvv;
	private String dateOfExpiry;
	
	public CreditCardStrategy() {
		
	}
	
	public CreditCardStrategy(String nm, String ccNum, String cvv, String expiryDate){
		this.name=nm;
		this.cardNumber=ccNum;
		this.cvv=cvv;
		this.dateOfExpiry=expiryDate;
	}
	
	@Override
	public void pay(int amount) {
		System.out.println(String.format("%d paid using CreditCard, CreditCard Details: [ %s, %s, %s, %s ]", amount, this.name, this.cardNumber, this.cvv, this.dateOfExpiry));
	}

	public ArrayList<PaymentStrategy> makePaymentStrategyPackage(String [][] CreditCardStrategiesDetails){
		
		ArrayList<PaymentStrategy> CreditCardStrategyPackage = new ArrayList<>();
        for (int i = 0; i < CreditCardStrategiesDetails.length; i++) {
            CreditCardStrategy CreditCardStrategy = new CreditCardStrategy(CreditCardStrategiesDetails[i][0], CreditCardStrategiesDetails[i][1], CreditCardStrategiesDetails[i][2], CreditCardStrategiesDetails[i][3]);
            CreditCardStrategyPackage.add(CreditCardStrategy);
        }

        System.out.println("One package of " + CreditCardStrategiesDetails.length + " CreditCard Strategy has been made!");
        return CreditCardStrategyPackage;
	}
}
