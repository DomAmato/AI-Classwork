package com.amato.converse.agent;

import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

import org.apache.commons.math3.util.Pair;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import com.amato.converse.nlp.NLPAnalysis;
import com.amato.converse.nlp.Word2Num;
import com.amato.converse.profile.UserInformation;
import com.amato.converse.yelp.YelpAPI;
import com.amato.converse.yelp.YelpAPI.Settings;

import javafx.beans.property.StringProperty;

public class ConversationalAgent {

	public enum AgentResponses {

		Starter("What can I help you with?"), 
		SimilarType("Would you like to look for more places like this?"), 
		SimilarPrice("Do you want to find places of similar prices?"), 
		Now("Are you looking for something open now?"), 
		WhatTime("What time should I check to see if they are open?"), 
		MoreResults("Want to see more options?"), 
		Reset("Should we start over?"), 
		NoResults("I found no matching restaurants, should we start over?"), 
		Proximity("How close to your position in miles should I filter by?"), 
		Hot("What to see whats new and rising?"), 
		Deals("Should I filter by places offering deals?");

		private final String response;

		AgentResponses(String response) {
			this.response = response;
		}

		public String getResponse() {
			return response;
		}
	}

	private StringProperty outputProp;
	private UserInformation userInfo;
	private YelpAPI yelp;
	private AgentResponses currentResponse;
	private JSONObject recentBusiness;

	private List<Settings> prevStates = new ArrayList<>();

	public ConversationalAgent(StringProperty stringProperty) {
		outputProp = stringProperty;
		userInfo = new UserInformation(UUID.fromString("7a5c1de5-3e67-4e58-be9f-948aaa240545"));
		yelp = new YelpAPI();
		setCurrentAgentResponse(AgentResponses.Starter);
	}

	public void cleanUpAgent() {
		userInfo.saveUserInfoToDisk();
	}

	public void generateAgentHint() {
		int hint = (int) (Math.random() * AgentResponses.values().length);
		boolean validHint = false;
		int attempts = 0;
		while (!validHint) {
			if (attempts > 35) {
				// eventually we run out of filters to apply, if they can't find
				// a result lets try resetting
				setCurrentAgentResponse(AgentResponses.Reset);
				return;
			}
			switch (AgentResponses.values()[hint]) {
			case Deals:
				if (!yelp.settings.DEALS) {
					validHint = true;
				} else {
					hint = (int) (Math.random() * AgentResponses.values().length);
				}
				break;
			case Hot:
				if (!yelp.settings.HOT_NEW) {
					validHint = true;
				} else {
					hint = (int) (Math.random() * AgentResponses.values().length);
				}
				break;
			case MoreResults:
				validHint = true;
				break;
			case Now:
				if (!yelp.settings.OPEN_NOW && !(yelp.settings.OPEN_AT > 0)) {
					validHint = true;
				} else {
					hint = (int) (Math.random() * AgentResponses.values().length);
				}
				break;
			case Proximity:
				if (!(yelp.settings.RADIUS > 0)) {
					validHint = true;
				} else {
					hint = (int) (Math.random() * AgentResponses.values().length);
				}
				break;
			case WhatTime:
				if (!(yelp.settings.OPEN_AT > 0) && !yelp.settings.OPEN_NOW) {
					validHint = true;
				} else {
					hint = (int) (Math.random() * AgentResponses.values().length);
				}
				break;
			case SimilarType:
			case SimilarPrice:
			case Starter:
			case Reset:
			case NoResults:
				hint = (int) (Math.random() * AgentResponses.values().length);
				break;
			default:
				break;
			}

		}
		setCurrentAgentResponse(AgentResponses.values()[hint]);
	}

	public JSONObject getBusinessInfo(String id) {
		return yelp.getBusinessById(id);
	}

	public JSONArray getResults() {
		JSONArray results = yelp.searchForBusinesses();
		if ((results != null) && (results.size() == 0)) {
			setCurrentAgentResponse(AgentResponses.NoResults);
		}
		return results;
	}

	private void handleNaturalText(String text) {
		List<String> dates = new ArrayList<>();
		List<String> times = new ArrayList<>();
		List<String> numbers = new ArrayList<>();
		List<String> distances = new ArrayList<>();
		List<String> categories = new ArrayList<>();
		List<Pair<String, String>> results = NLPAnalysis.getInstance().analyzeText(text);
		for (Pair<String, String> result : results) {
			if ("LOCATION MISC".contains(result.getSecond())) {
				if(YelpAPI.isACategory(result.getFirst().toLowerCase())){
					categories.add(result.getFirst().toLowerCase());
				}
			} else if (result.getSecond().equals("DATE")) {
				dates.add(result.getFirst());
			} else if (result.getSecond().equals("TIME")) {
				times.add(result.getFirst());
			} else if (result.getSecond().equals("NUMBER")) {
				numbers.add(result.getFirst());
			} else if (result.getSecond().equals("DISTANCE")) {
				distances.add(result.getFirst());
			}
		}
		if (categories.size() > 0) {
			yelp.settings.clearCategories();
			for(String category : categories){
				yelp.settings.addCategory(category);
			}
			generateAgentHint();
		}
		// will probably hit a lot of false flags
		yelp.settings.verifyCategories();

		switch (currentResponse) {
		case WhatTime:
			if ((dates.size() > 0) || (times.size() > 0)) {
				long timeOffset = 0;
				if ((dates.size() > 0) && (numbers.size() > 0)) {
					// the problem here is if the number isnt followed by a
					// colon or an AM/PM than it is not included...
					// the other problem is a date can also have a time...
					List<Long> possibleTimes = NLPAnalysis.getInstance()
							.extractTimes(NLPAnalysis.getInstance().replaceNumbersWithTimes(text));
					if (possibleTimes.size() > 0) {
						timeOffset = possibleTimes.get(0);
					}
				}
				if (timeOffset == 0) {
					timeOffset = NLPAnalysis.getInstance().extractTime(text);
				}
				if (timeOffset > 0) {
					yelp.settings.OPEN_AT = timeOffset;
					yelp.settings.OPEN_NOW = false;
					generateAgentHint();
				}
			}
			break;
		case Proximity:
			if (numbers.size() > 0) {
				int distance = 0; // NLPAnalysis.getInstance().extractDistance(text)
				for (String numStr : numbers) {
					try {
						if (Integer.parseInt(numStr) > distance) {
							distance = Integer.parseInt(numStr);
						}
					} catch (NumberFormatException e) {
						int num = Word2Num.convertToNumber(numStr);
						if (num > distance) {
							distance = num;
						}
					}
				}
				if (distances.size() > 0) {
					for (String dist : distances) {
						if (NLPAnalysis.getInstance().isImperial(dist)) {
							if (sanitizeImperialDistance(distance) > distance) {
								distance = sanitizeImperialDistance(distance);
							}
						} else {
							distance = sanitizeMetricDistance(distance);
						}
					}
				} else {
					distance = verifyDistance(distance);
				}
				if (distance > 0) {
					yelp.settings.RADIUS = distance;
					yelp.settings.OFFSET = 0;
					generateAgentHint();
				}
			}
			break;
		default:
			if ((distances.size() > 0) && (numbers.size() > 0)) {
				int distance = 0; // NLPAnalysis.getInstance().extractDistance(text)
				for (String numStr : numbers) {
					try {
						if (Integer.parseInt(numStr) > distance) {
							distance = Integer.parseInt(numStr);
						}
					} catch (NumberFormatException e) {
						int num = Word2Num.convertToNumber(numStr);
						if (num > distance) {
							distance = num;
						}
					}
				}
				for (String dist : distances) {
					if (NLPAnalysis.getInstance().isImperial(dist)) {
						if (sanitizeImperialDistance(distance) > distance) {
							distance = sanitizeImperialDistance(distance);
						}
					} else {
						distance = sanitizeMetricDistance(distance);
					}
				}

				if (distance > 0) {
					yelp.settings.RADIUS = distance;
					yelp.settings.OFFSET = 0;
					generateAgentHint();
				}
			}
			if ((dates.size() > 0) || (times.size() > 0)) {
				long timeOffset = 0;
				if ((dates.size() > 0) && (numbers.size() > 0)) {
					// the problem here is if the number isnt followed by a
					// colon or an AM/PM than it is not included...
					// the other problem is a date can also have a time...
					List<Long> possibleTimes = NLPAnalysis.getInstance()
							.extractTimes(NLPAnalysis.getInstance().replaceNumbersWithTimes(text));
					if (possibleTimes.size() > 0) {
						timeOffset = possibleTimes.get(0);
					}
				}
				if (timeOffset == 0) {
					timeOffset = NLPAnalysis.getInstance().extractTime(text);
				}
				if (timeOffset > 0) {
					yelp.settings.OPEN_AT = timeOffset;
					yelp.settings.OPEN_NOW = false;
					generateAgentHint();
				}
			}
			break;
		}
	}

	public void notifyDeselect() {
		recentBusiness = null;

	}

	public void notifyRestaurantExpanded(JSONObject restaurant) {
		userInfo.addSelectedPlace(restaurant);
		recentBusiness = restaurant;
		if (!yelp.settings.isAlreadyFilteredByCategory(
				((String) ((JSONObject) ((JSONArray) restaurant.get("categories")).get(0)).get("alias")))) {
			// only ask if the filter hasn't already been applied
			setCurrentAgentResponse(AgentResponses.SimilarType);
		} else {
			switch ((String) (restaurant.get("price"))) {
			case "$":
				if (yelp.settings.PRICE != 1) {
					setCurrentAgentResponse(AgentResponses.SimilarPrice);
				}
				break;
			case "$$":
				if (yelp.settings.PRICE != 2) {
					setCurrentAgentResponse(AgentResponses.SimilarPrice);
				}
				break;
			case "$$$":
				if (yelp.settings.PRICE != 4) {
					setCurrentAgentResponse(AgentResponses.SimilarPrice);
				}
				break;
			case "$$$$":
				if (yelp.settings.PRICE != 8) {
					setCurrentAgentResponse(AgentResponses.SimilarPrice);
				}
				break;
			}
		}

	}

	public boolean notifyUserQuery(String query) {
		System.out.println("Analyzing Query: " + query);
		userInfo.addUserQuery(query);
		boolean affirmative = NLPAnalysis.getInstance().determineAffirmative(query);
		boolean negation = NLPAnalysis.getInstance().determineNegation(query);
		if (!affirmative && !negation) {
			handleNaturalText(query);
			return true;
		}
		switch (currentResponse) {
		case Now: {
			if (affirmative) {
				yelp.settings.OPEN_NOW = true;
				yelp.settings.OFFSET = 0;
				generateAgentHint();
			} else {
				yelp.settings.OPEN_NOW = false;
				yelp.settings.OFFSET = 0;
				setCurrentAgentResponse(AgentResponses.WhatTime);
			}
			break;
		}
		case MoreResults: {
			if (affirmative) {
				yelp.settings.OFFSET += 5;
			} else {
				generateAgentHint();
			}
			break;
		}
		case SimilarPrice: {
			if (affirmative) {
				if (recentBusiness != null) {
					prevStates.add(yelp.settings);
					yelp.settings.PRICE = 0;
					yelp.settings.OFFSET = 0;
					switch ((String) recentBusiness.get("price")) {
					case "$":
						yelp.settings.PRICE = 1;
						break;
					case "$$":
						yelp.settings.PRICE = 2;
						break;
					case "$$$":
						yelp.settings.PRICE = 4;
						break;
					case "$$$$":
						yelp.settings.PRICE = 8;
						break;
					}
				}
			} else {
				generateAgentHint();
			}
			break;
		}
		case SimilarType: {
			if (affirmative) {
				if (recentBusiness != null) {
					prevStates.add(yelp.settings);
					yelp.settings.clearCategories();
					for (Object cat : ((JSONArray) recentBusiness.get("categories"))) {
						yelp.settings.addCategory((String) ((JSONObject) cat).get("alias"));
					}
					yelp.settings.verifyCategories();
					yelp.settings.OFFSET = 0;
				}
			}
			generateAgentHint();
			break;
		}
		case Deals:
			if (affirmative) {
				prevStates.add(yelp.settings);
				yelp.settings.DEALS = true;
				yelp.settings.OFFSET = 0;
			}
			generateAgentHint();
			break;
		case Hot:
			if (affirmative) {
				yelp.settings.HOT_NEW = true;
				yelp.settings.OFFSET = 0;
			}
			generateAgentHint();
			break;
		case Reset:
		case NoResults:
			if (affirmative) {
				prevStates.add(yelp.settings);
				yelp.settings = yelp.new Settings();
				setCurrentAgentResponse(AgentResponses.Starter);
			}
			break;
		default:
			break;
		}
		return affirmative;
	}

	private int sanitizeImperialDistance(int distance) {
		if ((distance > 0) && (distance < 25)) {
			// convert miles to meters
			distance *= 1609.344;
		} else {
			// convert feet to meters
			distance *= 0.3048;
		}
		return distance;
	}

	private int sanitizeMetricDistance(int distance) {
		if ((distance > 0) && (distance < 40)) {
			// convert miles to meters
			distance *= 1000;
		}
		return distance;
	}

	private void setCurrentAgentResponse(AgentResponses response) {
		currentResponse = response;
		outputProp.set(response.getResponse());
	}

	private int verifyDistance(int dist) {
		if ((dist > 0) && (dist < 25)) {
			// convert miles to meters
			dist *= 1609.344;
		}
		return dist;
	}
}
