
public class Node <E> {
	
	//----------------------- Properties ----------------------------//
	private E data;
	private Node<E> leftSon;
	private Node<E> rightSon;
	
	
	//----------------------- Constructors --------------------------//
	
	//This constructor receives the values (E data, Node<E> left, Node<E> right) and it initializes
	// the private properties with them
	public Node(E data, Node<E> left, Node<E> right) {
		
		this.data = data;
		this.leftSon = left;
		this.rightSon = right;
	}

	
	//----------------------- Setters && Getters ----------------------------//
	// setters and getters for all the properties
	
	public E getData() {
		return data;
	}

	public void setData(E data) {
		this.data = data;
	}

	public Node<E> getLeftSon() {
		return leftSon;
	}

	public void setLeftSon(Node<E> leftSon) {
		this.leftSon = leftSon;
	}

	public Node<E> getRightSon() {
		return rightSon;
	}

	public void setRightSon(Node<E> rightSon) {
		this.rightSon = rightSon;
	}

}
