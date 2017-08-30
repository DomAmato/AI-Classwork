package com.amato.converse.yelp;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

//better yet use this: https://www.yelp.com/developers/documentation/v2/all_category_list

public enum RestaurantTypes {

	Afghan("Afgan", "Afganistan", "Middle Eastern"), African("African"), AmericanNew("American", "New",
			"Modern"), AmericanTraditional("American", "Traditional"), Arabian("Arabian", "Arabic",
					"Middle Eastern"), Argentine("Argentine", "Argentina", "South American"), Armenian(
							"Armenian"), AsianFusion("Asian Fusion", "Asian", "Fusion"), Australian("Australian",
									"Australia"), Austrian("Austrian", "Austria", "European"), Bangladeshi(
											"Bangladeshi", "Bangladesh",
											"Halal"), Barbeque("Barbeque", "BBQ", "Grill"), Basque("Basque", "Spanish",
													"European"), Belgian("Belgian", "Belgium", "European"), Brasseries(
															"Brasseries", "French", "Bistro",
															"European"), Brazilian("Brazilian", "South American",
																	"Latin"), BreakfastBrunch("Breakfast", "Brunch"), // breakfasst
																														// and
																														// brunch
																														// is
																														// one
																														// category
	British("British", "European"), Buffets("Buffets"), Burgers("Burgers", "American"), Burmese("Burmese",
			"Burma"), Cafes(
					"Cafes"), Cafeteria("Cafeteria", "Fast Food"), Cajun("Cajun", "Creole", "American", "Southern"), // cajun
																														// and
																														// creole
																														// are
																														// the
																														// same
																														// category
	Cambodian("Cambodian", "Cambodia", "Asian"), Caribbean("Caribbean", "Latin"), Catalan("Catalan", "Spanish",
			"European"), Cheesesteaks("Cheesesteaks", "Sandwiches", "American"), ChickenShop("Chicken Shop", "Chicken",
					"American"), Chinese("Chinese", "China", "Asian"), ComfortFood("Comfort Food"), Creperies(
							"Creperies", "Crepe",
							"French"), Cuban("Cuban", "Cuba", "Caribbean", "Latin"), Czech("Czech", "Czech Republic",
									"European"), Delis("Delis", "Deli", "Sandwich", "Soup"), Diners("Diner",
											"Comfort Food"), DinnerTheater("Dinner Theater", "Theater", "Theatre",
													"Show"), Ethiopian("Ethiopian", "Ethiopia", "African"), FastFood(
															"Fast Food",
															"Chain"), Filipino("Filipino", "Phillipines"), FishChips(
																	"Fish and Chips", "British",
																	"European"), Fondue("Fondue", "French",
																			"European"), FoodCourt("Food Court",
																					"Court", "Food Hall", "Cafeteria",
																					"Fast Food"), FoodStands(
																							"Food Stand", "Food Cart",
																							"Food Truck", "Stand",
																							"Truck",
																							"Fast Food"), French(
																									"French", "France",
																									"European"), GameMeat(
																											"Game Meat",
																											"Meat"), Gastropubs(
																													"Gastropubs",
																													"Pub",
																													"Brewpub"), German(
																															"German",
																															"European"), GlutenFree(
																																	"Gluten Free",
																																	"Gluten"), Greek(
																																			"Greek",
																																			"Greecian",
																																			"Greece",
																																			"Mediterranean"), Guamanian(
																																					"Guamanian",
																																					"Guam"), Halal(
																																							"Halal",
																																							"Hallal",
																																							"Halaal",
																																							"Middle Eastern"), Hawaiian(
																																									"Hawaiian",
																																									"American"), Nepalese(
																																											"Himalayan",
																																											"Nepalese"), // himilayan
																																															// nepalese
																																															// is
																																															// the
																																															// same
																																															// category
	Honduran("Honduran", "Honduras", "Latin", "Central America"), HongKong("Hong Kong", "Chinese", "Cafe"), HotDogs(
			"Hot Dog", "American", "Fast Food"), HotPot("Hot Pot", "Chinese"), Hungarian("Hungarian", "Hungary",
					"European"), Iberian("Iberian", "Iberia"), Indian("India", "Indian"), Indonesian("Indonesian",
							"Indonesia"), Irish("Irish", "Ireland", "European"), Italian("Italian", "Italy",
									"European"), Japanese("Japanese", "Japan", "Asian"), Kebab("Kebab",
											"Middle Eastern"), Korean("Korean", "Asian", "Korea"), Kosher("Kosher",
													"Jewish"), Laotian("Laos", "Laotian",
															"Asian"), Latin("Latin"), LiveRaw("Live", "Raw"), // Live/Raw
																												// is
																												// one
																												// category
	Malaysian("Malaysian", "Maylasia", "Asian"), Mediterranean("Mediterranean", "European", "African"), Mexican(
			"Mexican", "Mexico", "Latin",
			"Central America"), MiddleEastern("Middle Eastern"), ModernEuropean("Modern European", "Modern",
					"European"), Mongolian("Mongolian", "Mongolia", "Asian"), Moroccan("Moroccan", "Morocco",
							"African"), NewMexican("New Mexican", "American", "Southern"), Nicaraguan("Nicaraguan",
									"Nicaragua", "South American"), Noodles("Noodles", "Pasta"), Pakistani("Pakistani",
											"Pakistan", "Middle Eastern"), PanAsian("Pan Asian", "Asian"), Persian(
													"Persian", "Iranian", "Middle Eastern"), Peruvian("Peruvian",
															"South American"), Pizza("Pizza", "Italian"), Polish(
																	"Polish", "Poland",
																	"European"), PopUp("Pop Up"), Portuguese(
																			"Portugese", "European"), Poutineries(
																					"Poutine", "Poutineries",
																					"Canadian"), Russian("Russian",
																							"Russia",
																							"European"), Salad("Salad",
																									"Vegetarian"), Sandwiches(
																											"Sandwiches",
																											"Sandwich"), Scandinavian(
																													"Scandinavian",
																													"Scandinavia",
																													""), Scottish(
																															"Scottish",
																															"European"), Seafood(
																																	"Seafood",
																																	"Fish",
																																	"Shrimp"), Singaporean(
																																			"Singaporean",
																																			"Singapore",
																																			"Asian"), Slovakian(
																																					"Slovakian",
																																					"Slovakia",
																																					"Slavic",
																																					"Eurpoean"), SoulFood(
																																							"Soul",
																																							"Soul Food"), Soup(
																																									"Soup"), Southern(
																																											"Southern",
																																											"American"), Spanish(
																																													"Spanish",
																																													"European"), SriLankan(
																																															"Sri Lanka",
																																															"Sri Lankan",
																																															"Indian",
																																															"India"), Steakhouses(
																																																	"Steakhouse",
																																																	"Steak",
																																																	"Beef"), SupperClubs(
																																																			"Supper",
																																																			"Dinner",
																																																			"Supper Club"), Sushi(
																																																					"Sushi",
																																																					"Japanese"), Syrian(
																																																							"Syria",
																																																							"Syrian",
																																																							"Middle Eastern"), Taiwanese(
																																																									"Taiwan",
																																																									"Taiwanese",
																																																									"Asian"), TapasBars(
																																																											"Tapas",
																																																											"Tapas Bar"), TapasSmallPlates(
																																																													"Tapas",
																																																													"Small Plate",
																																																													"Shared"), TexMex(
																																																															"Tex Mex",
																																																															"Texas",
																																																															"Mexican",
																																																															"Southern"), Thai(
																																																																	"Thai",
																																																																	"Asian"), Turkish(
																																																																			"Turkish"), Ukrainian(
																																																																					"Ukrainian",
																																																																					"Ukraine",
																																																																					"European"), Uzbek(
																																																																							"Uzbek",
																																																																							"Uzbekistan",
																																																																							"Uzbekistani"), Vegan(
																																																																									"Vegan"), Vegetarian(
																																																																											"Vegetarian",
																																																																											"Vegan"), // all
																																																																														// vegan
																																																																														// is
																																																																														// vegetarain
	Vietnamese("Vietnamese", "Asian"), Waffles("Waffles", "Breakfast"), Wings("Wings", "Chicken"), Wraps("Wraps",
			"Sandwiches");

	static Map<String, List<RestaurantTypes>> wordMaps;

	private final List<String> associatedWords = new ArrayList<>();

	RestaurantTypes(String... words) {
		for (String word : words) {
			associatedWords.add(word);
		}
	}

	public List<String> getAssociatedWords() {
		return associatedWords;
	}

	public List<RestaurantTypes> getTypesFromKeyword(String word) {
		if (wordMaps == null) {
			initEnumMap();
		}
		return wordMaps.get(word);
	}

	void initEnumMap() {
		wordMaps = new HashMap<>();
		for (RestaurantTypes rest : RestaurantTypes.values()) {
			for (String word : rest.getAssociatedWords()) {
				if (wordMaps.containsKey(word)) {
					wordMaps.get(word).add(this);
				} else {
					wordMaps.put(word, new ArrayList<>(Arrays.asList(this)));
				}
			}
		}

	}
}
