
public class MorseDecoder {

	// ----------------------- Properties ----------------------------//
	private BTree<String> morseTree;
	private int index;

	// This constructor initializes the BTree<String> morseTree with the values
	// which in the array lettersArr
	// That will fit in the array in a Morse Tree Format by calling the Recursive
	// prepareMorseTree()
	// which will go through the array and fills the tree in preOrder DFS technique
	public MorseDecoder() {
		String[] lettersArr = { "Start", "E", "I", "S", "H", "V", "U", "F", null, "A", "R", "L", null, "W", "P", "J", "T",
				"N", "D", "B", "X", "K", "C", "Y", "M", "G", "Z", "Q", "O" };

		this.index = 0;

		morseTree = new BTree<>();

		prepareMorseTree(lettersArr, 0, "");

	}

	// This Recursive function will fill the Tree with the values of morse tree from
	// given the array lettersArr
	// by using preOrder DFS technique
	public void prepareMorseTree(String[] lettersArr, int treeLevel, String path) {

		// stop condition
		if (treeLevel == 5 || this.index >= lettersArr.length)
			return;

		// adding the elements to the tree by calling addByPath() method from the calss
		// of BTree surrounded by try and catch because it throws InvalidPathException
		if (lettersArr[index] != null) {
			try {
				this.morseTree.addByPath(lettersArr[index], path);
			} catch (InvalidPathException e) {
				e.printStackTrace();
			}
		}
		// increasing the index counter with one so we step to the next element in the
		// lettersArr (Array)
		index++;

		// calling the function Recursively first with the path + "L" second with the
		// path + "R" (preOrder DFS technique)
		prepareMorseTree(lettersArr, treeLevel + 1, path + "L");
		prepareMorseTree(lettersArr, treeLevel + 1, path + "R");

	}

	// This method prints the morseTree in preOrder DFS technique by calling the
	// function pre() from the calss BTree
	public String toString() {

		return this.morseTree.pre();
	}

	// This decode function decodes the given string morseStr to the intended letter
	// by calling the function convertToRLFormat() that convert the Morse Format to
	// RL format
	// Then it calls findByPath on the morseTree and it returns the result
	public String decode(String morseStr) throws InvalidMorseCodeException {

		//This condition checks if the path is null and if true it throws InvalidMorseCodeException with the message of Reason: Path String Is Null
		if (morseStr == null)
			throw new InvalidMorseCodeException("Reason: Path String Is Null");

		String result;

		//else after it declare String result it tries to get the result of the converted bath by convertToRLFormat() by calling findByPath() function 
		//if it fails that means that there is a problem in the path according to the checks in BTree class
		//so it throws InvalidMorseCodeException with the message from the thrown InvalidPathException from BTree class
		try {

			result = this.morseTree.findByPath(convertToRLFormat(morseStr));

		} catch (InvalidPathException e) {

			throw new InvalidMorseCodeException(e.getMessage());
		}

		return result;
	}

	// This function converts the Morse Format to RL format by calling the
	// String.replace() function
	// to replace '.' with 'L' AND '-' with 'R' in the hole string the it return the
	// result
	private String convertToRLFormat(String str) {

		str = str.replace('.', 'L').replace('-', 'R');

		return str;
	}

}
