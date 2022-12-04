package Ex3_Question3;

// product A1 constructor and operations
class ProductA1 extends AbstractProductA {
	ProductA1(String arg) {
		System.out.println("Hello " + arg);
	}

	@Override
	public void operationAOP1() {

		System.out.println("Operation 1, Product A1");	
	}

	@Override
	public void operationAOP2() {
		
		System.out.println("Operation 2, Product A1");	
	}
}
