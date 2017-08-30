package com.amato.converse.yelp;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;
import org.scribe.model.OAuthRequest;
import org.scribe.model.Response;
import org.scribe.model.Token;
import org.scribe.model.Verb;

public class YelpAPI {

	public class Settings {
		public String LOCATION = DEFAULT_LOCATION;
		public String SEARCH_TERM = "";
		public int RADIUS = -1;
		// adding more categories does not refine a list rather it will return
		// all
		// restaurants that contain any of the tags
		private List<String> CATEGORIES = new ArrayList<>(Arrays.asList("food", "restaurants"));
		public byte PRICE = 0;
		public boolean OPEN_NOW;
		public long OPEN_AT = -1;
		public boolean HOT_NEW;
		public boolean DEALS;
		public byte SORT_STYLE = 8;
		public int SEARCH_LIMIT = 5;
		public int OFFSET = 0;

		public Settings() {

		}

		public Settings(Settings copy) {
			LOCATION = copy.LOCATION;
			SEARCH_TERM = copy.SEARCH_TERM;
			RADIUS = copy.RADIUS;
			setCategories(copy.CATEGORIES);
			PRICE = copy.PRICE;
			OPEN_NOW = copy.OPEN_NOW;
			OPEN_AT = copy.OPEN_AT;
			HOT_NEW = copy.HOT_NEW;
			DEALS = copy.DEALS;
			SORT_STYLE = copy.SORT_STYLE;
			SEARCH_LIMIT = copy.SEARCH_LIMIT;
		}

		public void addCategory(String category) {
			if (!validCategories.contains(category)) {
				String alias = getAliasFromTerm(category);
				if (alias != null) {
					CATEGORIES.add(alias);
				}
			} else {
				CATEGORIES.add(category);
			}
		}

		public void clearCategories() {
			CATEGORIES.clear();
		}

		public List<String> getCategories() {
			return CATEGORIES;
		}

		public int getTotalCategories() {
			return CATEGORIES.size();
		}

		public boolean isAlreadyFilteredByCategory(String category) {
			return CATEGORIES.contains(category);
		}

		public void setCategories(List<String> cATEGORIES) {
			CATEGORIES = cATEGORIES;
		}

		public void verifyCategories() {
			if (CATEGORIES.size() > 2) {
				CATEGORIES.remove("restaurants");
				CATEGORIES.remove("food");
			} else if (CATEGORIES.size() == 0) {
				CATEGORIES.add("restaurants");
				CATEGORIES.add("food");
			}
		}
	}

	private static List<String> validCategories = new ArrayList<>();

	private static Map<String, List<String>> terms2categories = new HashMap<>();

	public static boolean isACategory(String word) {
		if (validCategories.contains(word.toLowerCase())) {
			return true;
		}
		for (Entry<String, List<String>> category : terms2categories.entrySet()) {
			for (String term : category.getValue()) {
				if (term.toLowerCase().equals(word.toLowerCase())) {
					return true;
				}
			}
		}
		return false;
	}

	// OAuth credential placeholders that must be filled in by users.
	// You can find them on
	// https://www.yelp.com/developers/v3/manage_app
	private final String CLIENT_ID = "J_PV1dywGIrywlq0CXxstA";
	private final String CLIENT_SECRET = "We29JUKPsld23sDVDld6byBeNotkTLCfa0SDR9D2t3GBOO7L5TiRqNA5HRaQadKT";
	// API constants, you shouldn"t have to change these.
	private final String API_HOST = "api.yelp.com";

	private final String SEARCH_PATH = "/v3/businesses/search";
	private final String BUSINESS_PATH = "/v3/businesses";

	// Business ID will come after slash.
	private final String TOKEN_PATH = "/oauth2/token";
	private final String DEFAULT_LOCATION = "Chicago, IL";

	private Token bearerToken;

	public Settings settings = new Settings();

	public YelpAPI() {
		try {
			OAuthRequest request = createOAuthPostRequest(TOKEN_PATH);
			request.addQuerystringParameter("client_id", CLIENT_ID);
			request.addQuerystringParameter("client_secret", CLIENT_SECRET);
			// request.addQuerystringParameter("grant_type", GRANT_TYPE);
			JSONParser parser = new JSONParser();

			JSONObject parseResult = (JSONObject) parser.parse(request.send().getBody());

			bearerToken = new Token((String) parseResult.get("token_type"), (String) parseResult.get("access_token"));

			InputStream jsonFile = getClass().getResourceAsStream("/assets/json/final_filtered_categories.json");

			if (jsonFile != null) {
				try {
					JSONArray catJson = (JSONArray) parser.parse(new InputStreamReader(jsonFile));
					for (Object cat : catJson) {
						validCategories.add((String) ((JSONObject) cat).get("alias"));
						terms2categories.put((String) ((JSONObject) cat).get("alias"),
								Collections.unmodifiableList(((JSONArray) ((JSONObject) cat).get("matching_terms"))));
					}
				} catch (IOException | ParseException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		} catch (ParseException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	/**
	 * Creates and returns an {@link OAuthRequest} based on the API endpoint
	 * specified.
	 *
	 * @param path
	 *            API endpoint to be queried
	 * @return <tt>OAuthRequest</tt>
	 */
	private OAuthRequest createOAuthGetRequest(String path) {
		OAuthRequest request = new OAuthRequest(Verb.GET, "https://" + API_HOST + path);
		return request;
	}

	private OAuthRequest createOAuthPostRequest(String path) {
		OAuthRequest request = new OAuthRequest(Verb.POST, "https://" + API_HOST + path);
		return request;
	}

	private String getAliasFromTerm(String word) {
		for (Entry<String, List<String>> category : terms2categories.entrySet()) {
			for (String term : category.getValue()) {
				if (term.toLowerCase().equals(word.toLowerCase())) {
					return category.getKey();
				}
			}
		}
		return null;
	}

	/**
	 * Creates and sends a request to the Business API by business ID.
	 * <p>
	 * See
	 * <a href="http://www.yelp.com/developers/documentation/v2/business">Yelp
	 * Business API V2</a> for more info.
	 *
	 * @param businessID
	 *            <tt>String</tt> business ID of the requested business
	 * @return <tt>String</tt> JSON Response
	 */
	public JSONObject getBusinessById(String businessID) {
		OAuthRequest request = createOAuthGetRequest(BUSINESS_PATH + "/" + businessID);
		JSONParser parser = new JSONParser();
		JSONObject response = null;
		try {
			response = (JSONObject) parser.parse(sendRequestAndGetResponse(request));
		} catch (ParseException pe) {
			System.out.println("Error: could not parse JSON response:");
			pe.printStackTrace();
			// System.exit(1);
		}
		return response;
	}

	public JSONArray searchForBusinesses() {
		OAuthRequest request = createOAuthGetRequest(SEARCH_PATH);
		if (!settings.SEARCH_TERM.isEmpty()) {
			request.addQuerystringParameter("term", settings.SEARCH_TERM);
		}
		if (settings.RADIUS != -1) {
			request.addQuerystringParameter("radius", "" + settings.RADIUS);
		}
		if (!settings.CATEGORIES.isEmpty()) {
			String catParam = "";
			for (String cat : settings.CATEGORIES) {
				catParam += cat + ",";
			}
			request.addQuerystringParameter("categories", catParam.substring(0, catParam.length() - 1));
		}
		if (settings.PRICE != 0) {
			String priceParam = "";
			if ((settings.PRICE & 1) > 0) {
				priceParam += "1";
			}
			if ((settings.PRICE & 2) > 0) {
				priceParam += "2";
			}
			if ((settings.PRICE & 4) > 0) {
				priceParam += "3";
			}
			if ((settings.PRICE & 8) > 0) {
				priceParam += "4";
			}
			request.addQuerystringParameter("price", priceParam);
		}

		String sortParam = "";
		if ((settings.SORT_STYLE & 1) > 0) {
			sortParam += "best_match";
		}
		if ((settings.SORT_STYLE & 2) > 0) {
			sortParam += "rating";
		}
		if ((settings.SORT_STYLE & 4) > 0) {
			sortParam += "review_count ";
		}
		if ((settings.SORT_STYLE & 8) > 0) {
			sortParam += "distance";
		}
		request.addQuerystringParameter("sort_by", sortParam);

		if (settings.OPEN_AT != -1) {
			request.addQuerystringParameter("open_at", "" + settings.OPEN_AT);
		} else if (settings.OPEN_NOW) {
			request.addQuerystringParameter("open_now", "" + settings.OPEN_NOW);
		}
		if (settings.HOT_NEW || settings.DEALS) {
			String attribPrarm = "";
			if (settings.HOT_NEW) {
				attribPrarm += "hot_and_new,";
			}
			if (settings.DEALS) {
				attribPrarm += "deals,";
			}
			request.addQuerystringParameter("attributes", attribPrarm.substring(0, attribPrarm.length() - 1));
		}

		request.addQuerystringParameter("offset", "" + settings.OFFSET);

		request.addQuerystringParameter("location", settings.LOCATION);
		request.addQuerystringParameter("limit", String.valueOf(settings.SEARCH_LIMIT));
		;

		JSONParser parser = new JSONParser();
		JSONObject response = null;
		try {
			response = (JSONObject) parser.parse(sendRequestAndGetResponse(request));
		} catch (ParseException pe) {
			System.out.println("Error: could not parse JSON response:");
			pe.printStackTrace();
			System.exit(1);
		}
		return (JSONArray) response.get("businesses");
	}

	/**
	 * Sends an {@link OAuthRequest} and returns the {@link Response} body.
	 *
	 * @param request
	 *            {@link OAuthRequest} corresponding to the API request
	 * @return <tt>String</tt> body of API response
	 */
	private String sendRequestAndGetResponse(OAuthRequest request) {
		System.out.println("Querying " + request.getCompleteUrl() + " ...");
		request.addHeader("Authorization", bearerToken.getToken() + " " + bearerToken.getSecret());
		Response response = request.send();
		return response.getBody();
	}
}
