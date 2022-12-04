package Ex1_Question4;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;

// This class makes the serialization for the MathematicalSerializable shapes
public class GeoSerialization {

	public String createJSONFileSerialization(ArrayList<MathematicalSerializable> arrayList, String filePath) {

		String JSONString = "";

		for (MathematicalSerializable mathematicalSerializable : arrayList) {
			JSONString += mathematicalSerializable.getJSONSerialization() + "\n";
		}

		File file = new File(filePath);

		try {

			file.createNewFile();

			FileWriter fileWriter = new FileWriter(filePath, false);

			fileWriter.write(JSONString);
			fileWriter.close();

		} catch (IOException e) {

			e.printStackTrace();
		}

		return JSONString;
	}

}
