package Ex3_Question3;

class ConcreteFactory1 extends AbstractFactoryA {

	// function for creating product type A1
	@Override
	AbstractProductA createProductA1() {
		return new ProductA1("ProductA1");
	}

	// function for creating product type A2
	@Override
	AbstractProductA createProductA2() {

		return new ProductA2("ProductA2");
	}

	// function for creating product type A1 OR A2 according to the user need
	@Override
	Product getProduct(String productName) {

		Product newProduct = null;

		switch (productName) {

		case "ProductA1":
			newProduct = createProductA1();
			break;

		case "ProductA2":
			newProduct = createProductA2();
			break;

		default:
			System.out.println("Invalid Product Type");
			break;
		}

		return newProduct;
	}
}
