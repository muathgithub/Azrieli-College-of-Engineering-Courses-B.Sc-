package Ex4_Question2;

import java.util.HashMap;
import java.util.Map.Entry;

//This class defines the market account that the question asked about
public class MarketAccount {

	// Defining the needed variables for the market account
	private Catalog itemsCatalog;
	private HashMap<String, Integer> cartHash;
	private double discount;

	// constructor for initializing the market account variables
	public MarketAccount(Catalog itemsCatalog) {

		this.itemsCatalog = itemsCatalog;
		this.cartHash = new HashMap<String, Integer>();
		this.discount = 0;
	}

	// add new item to the shopping cart
	public boolean addItem(String productName, int productQuantity) {
		
		if(productQuantity < 0) {
			return false;
		}

		if (itemsCatalog.isProductExists(productName)) {

			if (!isProductInCart(productName)) {

				cartHash.put(productName, productQuantity);

				return true;
			}
		}

		return false;
	}

	// changing the quantity of the item in the cart
	public boolean changeProductQuantity(String productName, int newProductQuantity) {

		if (isProductInCart(productName)) {

			cartHash.put(productName, newProductQuantity);
			return true;
		}

		return false;
	}

	// removing product from the shopping cart
	public boolean removeProductFromCart(String productName) {

		if (isProductInCart(productName)) {

			cartHash.remove(productName);
			return true;
		}

		return false;
	}
	
	// getting the total price of the items in the cart
	public double getTotalPrice() {
		
		double totalPrice = 0;

		for (Entry<String, Integer> currItem : cartHash.entrySet()) {
			
			totalPrice += (currItem.getValue() * itemsCatalog.getProductPriceByName(currItem.getKey())) * (1 - discount);
		}
		
		return totalPrice;
	}
	
	// getting the number of the different items in the cart
	public int getNumOfDifProducts() {
		
		return cartHash.size();
	}
	
	public int getProductQuantity(String productName) {
		
		if(isProductInCart(productName)) {
			
			return cartHash.get(productName);
		}
		
		return -1;
	}
	
	// setting a discount value
	public boolean setDiscountPersentage(double discountPersentage) {
		
		if (discountPersentage > 1) {
			
			return false;
		}
		
		this.discount = discountPersentage;
		
		return true;
	}

	// checking it the item is in the cart
	public boolean isProductInCart(String productName) {

		if (cartHash.containsKey(productName)) {
			return true;
		}

		return false;
	}

}
