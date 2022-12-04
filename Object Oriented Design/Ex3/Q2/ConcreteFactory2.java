package Ex3_Question3;

class ConcreteFactory2 extends AbstractFactoryB {

	// function for creating product type B1
	@Override
	AbstractProductB createProductB1() {
		return new ProductB1("ProductB1");
	}

	// function for creating product type B2
	@Override
	AbstractProductB createProductB2() {

		return new ProductB2("ProductB2");
	}

	// function for creating product type A1 OR A2 according to the user need
	@Override
	Product getProduct(String productName) {

		Product newProduct = null;

		switch (productName) {

		case "ProductB1":
			newProduct = createProductB1();
			break;

		case "ProductB2":
			newProduct = createProductB2();
			break;

		default:
			System.out.println("Invalid Product Type");
			break;
		}

		return newProduct;
	}
}
