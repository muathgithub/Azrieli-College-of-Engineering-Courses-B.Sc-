package Ex3_Question3;

//product B2 constructor and operations
class ProductB2 extends AbstractProductB{
	ProductB2(String arg){
		System.out.println("Hello "+arg);
	}

	@Override
	public void operationBOP1() {

		System.out.println("Operation 1, Product B2");	
	}

	@Override
	public void operationBOP2() {

		System.out.println("Operation 2, Product B2");	
	} 
}

