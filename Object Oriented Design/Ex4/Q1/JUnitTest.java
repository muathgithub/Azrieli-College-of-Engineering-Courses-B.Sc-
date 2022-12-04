package Ex4_Question2;

import static org.junit.Assert.assertFalse;
import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

class JUnitTest {
	
	// defining the needed variables
	public static Catalog itemsCatalog;
	public static MarketAccount marketAccount;
	
	// initializing the catalog and filling it with some items
	@BeforeAll
	static void setUpBeforeClass() throws Exception {
		
		itemsCatalog = new Catalog();
		itemsCatalog.addProduct("Apple", 10);
		itemsCatalog.addProduct("Banana", 15);
		itemsCatalog.addProduct("Orange", 20);	
	}

	// clearing the market account before each test
	@BeforeEach
	void setUp() throws Exception {
		
		marketAccount = new MarketAccount(itemsCatalog);	
	}

	
	@Test
	void compareTwoProductsTotalPrice() {
		
		marketAccount.addItem("Apple", 2);
		marketAccount.addItem("Banana", 2);
		
		assertEquals(50, marketAccount.getTotalPrice());
	}
	
	
	@Test
	void emptyAccountGivesPriceOfZero() {
		
		assertEquals(0, marketAccount.getTotalPrice());
	}
	
	@Test
	void addThenRemoveGivesPriceOfZero() {
		
		marketAccount.addItem("Apple", 2);
		marketAccount.removeProductFromCart("Apple");
		
		assertEquals(0, marketAccount.getTotalPrice());
	}
	
	@Test
	void addTwoProductsGivesTwoDefProducts() {
		
		marketAccount.addItem("Apple", 2);
		marketAccount.addItem("Banana", 2);
		
		assertEquals(2, marketAccount.getNumOfDifProducts());
	}
	
	@Test
	void priceAferDiscount() {
		
		marketAccount.addItem("Apple", 2);
		marketAccount.addItem("Banana", 2);
		
		marketAccount.setDiscountPersentage(0.5);
		
		assertEquals(25, marketAccount.getTotalPrice());
	}
	
	@Test
	void twoTimesQuantityChange() {
		
		marketAccount.addItem("Apple", 2);
		
		marketAccount.changeProductQuantity("Apple", 5);
		marketAccount.changeProductQuantity("Apple", 6);
		
		assertEquals(6, marketAccount.getProductQuantity("Apple"));
	}
	
	
	@Test
	void getQuantitiyOfNotExistsProductInMarketAccount() {
		
		marketAccount.addItem("Apple", 2);
		
		assertEquals(-1, marketAccount.getProductQuantity("Orange"));
	}
	
}
