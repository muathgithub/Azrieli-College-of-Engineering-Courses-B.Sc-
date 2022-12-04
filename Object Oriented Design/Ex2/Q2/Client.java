package Ex3_Question3;

//Client
public class Client {

	public static void main(String args[]) {
		AbstractFactory absFactoryOne = FactoryMaker.getFactory("a"); // get factory

		AbstractProductA product1 = (AbstractProductA) absFactoryOne.getProduct("ProductA1");
		product1.operationAOP1(); // this function buy the product
		product1.operationAOP2(); // this function return the product
		System.out.println();
		
		AbstractProductA product2 = (AbstractProductA) absFactoryOne.getProduct("ProductA2");
		product2.operationAOP1(); // this function buy the product
		product2.operationAOP2(); // this function return the product
		System.out.println();
		
		AbstractFactory absFactoryTwo = FactoryMaker.getFactory("b");

		AbstractProductB product3 = (AbstractProductB) absFactoryTwo.getProduct("ProductB1");
		product3.operationBOP1(); // this function buy the product
		product3.operationBOP2(); // this function return the product
		System.out.println();
		
		AbstractProductB product4 = (AbstractProductB) absFactoryTwo.getProduct("ProductB2");
		product4.operationBOP1();// this function buy the product
		product4.operationBOP2();// this function return the product
		System.out.println();
		
	}

}
