package Ex4_Question2;

import java.util.HashMap;

// This class defines the catalog that the question asked about
public class Catalog {

	// hash map for holding the items and there prices
	private HashMap<String, Double> productsCatalog;

	// constructor for initializing the catalog variables
	public Catalog() {

		productsCatalog = new HashMap<String, Double>();
	}
	
	// This function adds product to the catalog after making the needed checks
	public boolean addProduct(String productName, double productPrice) {

		if (!isProductExists(productName)) {

			productsCatalog.put(productName, productPrice);
			return true;
		}

		return false;
	}

	
	// This function gets the product price after making the needed checks
	public double getProductPriceByName(String productName) {

		if (isProductExists(productName)) {

			return productsCatalog.get(productName);
		}

		return -1;
	}

	// checking if the item is in the catalog
	public boolean isProductExists(String productName) {

		if (productsCatalog.containsKey(productName)) {

			return true;
		}
		
		return false;
	}

}
