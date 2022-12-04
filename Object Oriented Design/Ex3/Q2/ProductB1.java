package Ex3_Question3;

//product B1 constructor and operations
class ProductB1 extends AbstractProductB{
	ProductB1(String arg){
		System.out.println("Hello "+arg);
	}

	@Override
	public void operationBOP1() {
		
		System.out.println("Operation 1, Product B1");	
	}

	@Override
	public void operationBOP2() {

		System.out.println("Operation 2, Product B1");
	}
	
	
}
