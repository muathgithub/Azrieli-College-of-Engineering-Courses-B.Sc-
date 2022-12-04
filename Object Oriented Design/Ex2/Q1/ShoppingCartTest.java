package Ex3_Question1;

import java.util.ArrayList;

public class ShoppingCartTest {

	public static void main(String[] args) {
		ShoppingCart cart = new ShoppingCart();
		
		Item item1 = new Item("1234",10);
		Item item2 = new Item("5678",40);
				
		cart.addItem(item1);
		cart.addItem(item2);
		
		// Creating payment strategies packages
		
		PaymentStrategisFactory paymentStrategisFactory = new PaymentStrategisFactory();
		
		String [][] PayPalStrategyPackageDetails = {{"One@example.com", "mypwdOne"}, {"Two@example.com", "mypwdTwo"}};
		
		String [][] CreditCardStrategyPackageDetails = {{"UserOne", "1234 5678 9876 5364", "543", "06/23"}, {"UserTwo", "5643 9876 3256 0987", "321", "02/27"}};
		
		ArrayList<PaymentStrategy> PayPalStrategyPackage = paymentStrategisFactory.getPaymentStrategiesPackage("PayPalStrategy", PayPalStrategyPackageDetails);
		
		ArrayList<PaymentStrategy> CreditCardStrategyPackage = paymentStrategisFactory.getPaymentStrategiesPackage("CreditCardStrategy", CreditCardStrategyPackageDetails);
		
		
		System.out.println("\nPaying With PayPal Strategies:\n");
		for(int i = 0; i < PayPalStrategyPackage.size(); i++) {
			
			cart.pay(PayPalStrategyPackage.get(i));
		}
		
		System.out.println("\nPaying With CreditCard Strategies:\n");
		for(int i = 0; i < CreditCardStrategyPackage.size(); i++) {
			
			cart.pay(CreditCardStrategyPackage.get(i));
		}
		
		
	}

}
