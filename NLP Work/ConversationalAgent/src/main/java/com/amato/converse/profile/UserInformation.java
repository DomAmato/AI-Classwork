package com.amato.converse.profile;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

public class UserInformation {

	private List<String> queries = new ArrayList<>();
	private JSONArray selectedPlaces = new JSONArray();
	private JSONArray outputArray = new JSONArray();
	private UUID userID;

	public UserInformation() {
		userID = UUID.randomUUID();
		File inFile = new File("user_information.json");

		if (inFile.exists()) {
			JSONParser parser = new JSONParser();
			try {
				outputArray = (JSONArray) parser.parse(new FileReader(inFile));
			} catch (IOException | ParseException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}

		for (Object user : outputArray) {
			if (UUID.fromString((String) ((JSONObject) user).get("user")) == userID) {
				return;
			}
		}

		JSONObject outputJson = new JSONObject();
		outputJson.put("user", userID.toString());
		outputJson.put("queries", new JSONArray());
		outputJson.put("places", new JSONArray());

		outputArray.add(outputJson);
	}

	public UserInformation(UUID uuid) {
		userID = uuid;
		File inFile = new File("user_information.json");

		if (inFile.exists()) {
			JSONParser parser = new JSONParser();
			try {
				outputArray = (JSONArray) parser.parse(new FileReader(inFile));
			} catch (IOException | ParseException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}

		for (Object user : outputArray) {
			if (UUID.fromString((String) ((JSONObject) user).get("user")).equals(userID)) {
				return;
			}
		}

		JSONObject outputJson = new JSONObject();
		outputJson.put("user", userID.toString());
		outputJson.put("queries", new JSONArray());
		outputJson.put("places", new JSONArray());

		outputArray.add(outputJson);
	}

	public void addSelectedPlace(JSONObject place) {
		for (int i = 0; i < selectedPlaces.size(); i++) {
			if (((JSONObject) selectedPlaces.get(i)).get("id") == place.get("id")) {
				((JSONObject) selectedPlaces.get(i)).replace("total",
						((int) ((JSONObject) selectedPlaces.get(i)).get("total")) + 1);
				return;
			}
		}
		place.put("total", 1);
		selectedPlaces.add(place);
	}

	public void addUserQuery(String query) {
		queries.add(query);
	}

	public void saveUserInfoToDisk() {
		File outFile = new File("user_information.json");

		for (Object user : outputArray) {
			if (UUID.fromString((String) ((JSONObject) user).get("user")).equals(userID)) {
				((JSONArray) ((JSONObject) user).get("queries")).addAll(queries);
				((JSONArray) ((JSONObject) user).get("places")).addAll(selectedPlaces);
			}
		}

		FileOutputStream fOut;
		try {
			fOut = new FileOutputStream(outFile);
			OutputStreamWriter myOutWriter = new OutputStreamWriter(fOut);
			outputArray.writeJSONString(myOutWriter);
			myOutWriter.close();
			fOut.close();
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
