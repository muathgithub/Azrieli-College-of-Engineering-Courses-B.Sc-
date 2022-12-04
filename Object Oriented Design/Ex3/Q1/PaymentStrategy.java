package Ex3_Question1;

import java.util.ArrayList;

public interface PaymentStrategy {

	public void pay(int amount);
	
	public ArrayList<PaymentStrategy> makePaymentStrategyPackage(String [][] strategiesDetails);
}


