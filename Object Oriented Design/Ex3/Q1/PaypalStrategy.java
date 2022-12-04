package Ex3_Question1;

import java.util.ArrayList;

public class PaypalStrategy implements PaymentStrategy {

	private String emailId;
	private String password;
	
	public PaypalStrategy() {
		
		
	}
	
	public PaypalStrategy(String email, String pwd){
		this.emailId=email;
		this.password=pwd;
	}
	
	@Override
	public void pay(int amount) {
		System.out.println(String.format("%d paid using PayPal, PayPal Details: [ %s, %s ]", amount, this.emailId, this.password));
	}
	
	public ArrayList<PaymentStrategy> makePaymentStrategyPackage(String [][] PayPalStrategiesDetails){
		
		ArrayList<PaymentStrategy> PayPalStrategyPackage = new ArrayList<>();
        for (int i = 0; i < PayPalStrategiesDetails.length; i++) {
            PaypalStrategy PayPalStrategy = new PaypalStrategy(PayPalStrategiesDetails[i][0], PayPalStrategiesDetails[i][1]);
            PayPalStrategyPackage.add(PayPalStrategy);
        }

        System.out.println("One package of " + PayPalStrategiesDetails.length + " PayPal Strategy has been made!");
        return PayPalStrategyPackage;
	}

}

