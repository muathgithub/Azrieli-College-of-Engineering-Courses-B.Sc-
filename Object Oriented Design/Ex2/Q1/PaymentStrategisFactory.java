package Ex3_Question1;

import java.util.ArrayList;

public class PaymentStrategisFactory {
	
	public static enum PaymentStrategiesEnum {
		PayPalStrategy,
		CreditCardStrategy
	}


	public static PaymentStrategy getPaymentStrategy(String type) {

		switch (type) {
		case "PayPalStrategy":
			return new PaypalStrategy();
		case "CreditCardStrategy":
			return new CreditCardStrategy();
		default:
			return null;
		}
	}
	
    public static ArrayList<PaymentStrategy> getPaymentStrategiesPackage(String type, String [][] strategiesDetails) {
        PaymentStrategy paymentStrategy = getPaymentStrategy(type);
        ArrayList<PaymentStrategy> paymentStrategiesPackage = paymentStrategy.makePaymentStrategyPackage(strategiesDetails);
        return paymentStrategiesPackage;
    }

}
