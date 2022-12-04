package Ex3_Question3;

//product A2 constructor and operations
class ProductA2 extends AbstractProductA{
	ProductA2(String arg){
		System.out.println("Hello "+arg);
	}

	@Override
	public void operationAOP1() {

		System.out.println("Operation 1, Product A2");	
	}

	@Override
	public void operationAOP2() {
		
		System.out.println("Operation 2, Product A2");	
	} 
}

