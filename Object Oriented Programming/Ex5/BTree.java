
public class BTree<E> {

	//----------------------- Properties ----------------------------//
	private Node<E> root;
	private int size;

	//----------------------- Constructors --------------------------//
	
	//This constructor initializes the properties with the default values root = null & size = 0
	public BTree() {
		this.root = null;
		this.size = 0;
	}

	
	//This function receives data, and path and according to the path it creates a new node with given data
	// and it adds it in to the tree in the given path
	//also it throws an exception IncvalidPathException 
	public void addByPath(E data, String path) throws InvalidPathException {

		//These if conditions checking the special cases of the wrong paths
		
		//checks if the path is null and if true it throws InvalidPathException with the message of Reason: Path String Is Null
		if (path == null) {
			throw new InvalidPathException("Reason: Path String Is Null");
		}

		//checks if the path is not in the correct format which is composed just of R,L or empty string and if true it throws InvalidPathException with the message of Reason: Path Contains Illegal Characters
		//"[RL]*" this is a regular expression means the string composed just of R or L or R and L one time or zero times (empty string) 
		if (!path.matches("[RL]*")) {
			throw new InvalidPathException("Reason: Path Contains Illegal Characters");
		}

		//checks if the path equals an empty string "" (which means the root) and the root != null which means that the root is already exists and if true it throws InvalidPathException with the message of Reason: Root Is Not Empty
		if (path.equals("") && this.root != null) {
			throw new InvalidPathException("Reason: Root Is Not Empty");
		}

		//checks if the path.length > 0, which means that the path leads to a node under the root and if at the same time the root is empty so we can't follow the path and if true it throws InvalidPathException with the message of Reason: Root Is Empty
		if (path.length() > 0 && this.root == null) {
			throw new InvalidPathException("Reason: Root Is Empty");
		}

		
		//creating a new node with the data that given in data argument
		Node<E> newNode = new Node<>(data, null, null);

		//checking if the path is "" which means root node and it adds the newNode to this.root and it increases the value of size with one and it returns
		if (path.equals("")) {
			this.root = newNode;
			this.size++;
			return;
		}

		//else we create a new pointer that points at the root of the tree(this.root)
		Node<E> nodePointer = this.root;

		//This for runs path.length() - 1 times and each time it calls moveTo() with the pointer and the direction 
		// so at the end we reach to the node which is the father of the node that we want to add
		for (int i = 0; i < path.length() - 1; i++) {

			nodePointer = moveTo(nodePointer, path.charAt(i));

			//This if checks if the pointer is null which means that there is no father for that node according to the given path and if true it throws InvalidPathException with the message of Reason: No Father (Father Is NUll) 
			if (nodePointer == null) {
				throw new InvalidPathException("Reason: No Father (Father Is NUll)");
			}
		}

		//These if else determine in which son pointer to set the newNode according to the last char at the path
		if (path.charAt(path.length() - 1) == 'R') {
			nodePointer.setRightSon(newNode);
		} else {
			nodePointer.setLeftSon(newNode);
		}

		//increasing the size with one after adding a node to the tree
		this.size++;
	}

	//This method pre() prints the tree n preOrder format by creating a a pointer that points to the root and using the help Recursion function by sending the pointer to it
	public String pre() {
		Node<E> nodePointer = this.root;
		String result = preRecursion(nodePointer);

		//it returns the result after cutting the last two chars which is ", " if there is a need
		return result.length() > 2 ? result.substring(0, result.length() - 2) : result;
	}

	// This recursion function returns a string with the tree values splitted by ", " 
	private String preRecursion(Node<E> nodePointer) {

		if (nodePointer == null)
			return "";
		
		return nodePointer.getData().toString() + ", " 
		+ preRecursion(nodePointer.getLeftSon()) 
		+  preRecursion(nodePointer.getRightSon());

	}

	public E findByPath(String path) throws InvalidPathException {

		//checks if the root is null and if true it throws InvalidPathException with the message of Reason: Null Root (Empty Tree)
		if (this.root == null) {
			throw new InvalidPathException("Reason: Null Root (Empty Tree)");
		}

		//checks if the path is null and if true it throws InvalidPathException with the message of Reason: Path String Is Null
		if (path == null) {
			throw new InvalidPathException("Reason: Path String Is Null");
		}

		//checks if the path is not in the correct format which is composed just of R,L or empty string and if true it throws InvalidPathException with the message of Reason: Path Contains Illegal Characters
		//"[RL]*" this is a regular expression means the string composed just of R or L or R and L one time or zero times (empty string)
		if (!path.matches("[RL]*")) {
			throw new InvalidPathException("Reason: Path Contains Illegal Characters");
		}

		//else we create a new pointer that points at the root of the tree(this.root)
		Node<E> nodePointer = this.root;

		//This for runs path.length() times and each time it calls moveTo() with the pointer and the direction 
		// so at the end we reach node which is the node that we want to find
		for (int i = 0; i < path.length(); i++) {

			nodePointer = moveTo(nodePointer, path.charAt(i));

			//This if checks if the pointer is null which means that there is no father for that node according to the given path and if true it throws InvalidPathException with the message of Reason: No Father (Father Is NUll) 
			if (nodePointer == null) {
				throw new InvalidPathException("Reason: No Father (Father Is NUll)");
			}
		}

		return nodePointer.getData();
	}

	//This method returns a pointer that points to the left son or to the right son of the given pointer nodePointer
	private Node<E> moveTo(Node<E> nodePointer, char diriction) {

		if (diriction == 'R')
			return nodePointer.getRightSon();

		return nodePointer.getLeftSon();
	}

	//----------------------- Getters ----------------------------//
	public int getSize() {
		return this.size;
	}

}
