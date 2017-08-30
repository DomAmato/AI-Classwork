package com.amato.converse;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import com.amato.converse.agent.ConversationalAgent;
import com.amato.converse.nlp.NLPAnalysis;

import javafx.application.Platform;
import javafx.fxml.FXML;
import javafx.scene.control.Accordion;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.control.TitledPane;
import javafx.scene.image.ImageView;
import javafx.scene.input.KeyCode;
import javafx.scene.layout.GridPane;

public class FXMLViewController {
	@FXML
	private Button search_btn;
	@FXML
	private TextField input;
	@FXML
	private Label agent_lbl;
	@FXML
	private Accordion content;

	@FXML
	protected void initialize() {
		ConversationalAgent agent = new ConversationalAgent(agent_lbl.textProperty());

		new Thread() {
			@Override
			public void run() {
				NLPAnalysis.getInstance();
			}
		}.start();

		content.getPanes().clear();

		for (Object business : agent.getResults()) {
			TitledPane tp = new TitledPane();
			tp.setText("\t" + ((JSONObject) business).get("name").toString() + "\n\t"
					+ ((JSONObject) ((JSONArray) ((JSONObject) business).get("categories")).get(0)).get("title")
							.toString());
			GridPane contentPane = new GridPane();
			ImageView iv = new ImageView(((JSONObject) business).get("image_url").toString());
			iv.setFitWidth(100);
			iv.setPreserveRatio(true);
			iv.setSmooth(true);
			iv.setCache(true);
			contentPane.add(iv, 0, 0, 1, 4);
			contentPane.add(new Label("Price: " + ((JSONObject) business).get("price").toString()), 2, 0);
			contentPane.add(new Label("Rating: " + ((JSONObject) business).get("rating").toString()), 2, 1);
			contentPane.add(new Label("Number of Reviews: " + ((JSONObject) business).get("review_count").toString()),
					2, 2);
			contentPane.add(new Label(
					"Address: " + ((JSONObject) ((JSONObject) business).get("location")).get("address1").toString()), 2,
					3);
			tp.setContent(contentPane);
			tp.setMaxHeight(content.getMaxHeight());
			tp.setPrefHeight(100);
			tp.getStylesheets().add("assets/layout/titledLayout.css");
			tp.setOnMouseClicked(btn -> {
				if (tp.isExpanded()) {
					agent.notifyRestaurantExpanded((JSONObject) business);
					new Thread() {
						@Override
						public void run() {
							agent.getBusinessInfo(((JSONObject) business).get("id").toString());
						}
					}.start();
				} else {
					agent.generateAgentHint();
				}
			});

			content.getPanes().add(tp);
		}

		ImageView iv = new ImageView("assets/images/search.png");
		iv.setFitWidth(search_btn.getPrefWidth() - 10);
		iv.setPreserveRatio(true);
		iv.setSmooth(true);
		iv.setCache(true);

		search_btn.setGraphic(iv);

		input.setOnKeyPressed(key -> {
			if (key.getCode() == KeyCode.ENTER) {

				if (agent.notifyUserQuery(input.getText())) {
					content.getPanes().clear();

					Platform.runLater(() -> {
						for (Object business : agent.getResults()) {
							TitledPane tp = new TitledPane();
							tp.setText("\t" + ((JSONObject) business).get("name").toString() + "\n\t"
									+ ((JSONObject) ((JSONArray) ((JSONObject) business).get("categories")).get(0))
											.get("title").toString());
							GridPane contentPane = new GridPane();
							ImageView iv2 = new ImageView(((JSONObject) business).get("image_url").toString());
							iv2.setFitWidth(100);
							iv2.setPreserveRatio(true);
							iv2.setSmooth(true);
							iv2.setCache(true);
							contentPane.add(iv2, 0, 0, 1, 4);
							contentPane.add(new Label("Price: " + ((JSONObject) business).get("price").toString()), 2,
									0);
							contentPane.add(new Label("Rating: " + ((JSONObject) business).get("rating").toString()), 2,
									1);
							contentPane.add(new Label(
									"Number of Reviews: " + ((JSONObject) business).get("review_count").toString()), 2,
									2);
							contentPane
									.add(new Label("Address: " + ((JSONObject) ((JSONObject) business).get("location"))
											.get("address1").toString()), 2, 3);
							tp.setContent(contentPane);
							tp.setMaxHeight(content.getMaxHeight());
							tp.setPrefHeight(100);
							tp.getStylesheets().add("assets/layout/titledLayout.css");
							tp.setOnMouseClicked(pane -> {
								if (tp.isExpanded()) {
									agent.notifyRestaurantExpanded((JSONObject) business);
									new Thread() {
										@Override
										public void run() {
											agent.getBusinessInfo(((JSONObject) business).get("id").toString());
										}
									}.start();
								} else {
									agent.generateAgentHint();
								}
							});

							content.getPanes().add(tp);
						}
					});
				}
				input.setText("");
			}
		});

		search_btn.setOnMouseClicked(btn -> {
			if (agent.notifyUserQuery(input.getText())) {
				content.getPanes().clear();
				Platform.runLater(() -> {
					for (Object business : agent.getResults()) {
						TitledPane tp = new TitledPane();
						tp.setText("\t" + ((JSONObject) business).get("name").toString() + "\n\t"
								+ ((JSONObject) ((JSONArray) ((JSONObject) business).get("categories")).get(0))
										.get("title").toString());
						GridPane contentPane = new GridPane();
						ImageView iv2 = new ImageView(((JSONObject) business).get("image_url").toString());
						iv2.setFitWidth(100);
						iv2.setPreserveRatio(true);
						iv2.setSmooth(true);
						iv2.setCache(true);
						contentPane.add(iv2, 0, 0, 1, 4);
						contentPane.add(new Label("Price: " + ((JSONObject) business).get("price").toString()), 2, 0);
						contentPane.add(new Label("Rating: " + ((JSONObject) business).get("rating").toString()), 2, 1);
						contentPane.add(
								new Label(
										"Number of Reviews: " + ((JSONObject) business).get("review_count").toString()),
								2, 2);
						contentPane.add(new Label("Address: "
								+ ((JSONObject) ((JSONObject) business).get("location")).get("address1").toString()), 2,
								3);
						tp.setContent(contentPane);
						tp.setMaxHeight(content.getMaxHeight());
						tp.setPrefHeight(100);
						tp.getStylesheets().add("assets/layout/titledLayout.css");
						tp.setOnMouseClicked(pane -> {
							if (tp.isExpanded()) {
								agent.notifyRestaurantExpanded((JSONObject) business);
								new Thread() {
									@Override
									public void run() {
										agent.getBusinessInfo(((JSONObject) business).get("id").toString());
									}
								}.start();
							} else {
								agent.generateAgentHint();
							}
						});

						content.getPanes().add(tp);
					}
				});
			}
			input.setText("");
		});

		Runtime.getRuntime().addShutdownHook(new Thread() {
			@Override
			public void run() {
				agent.cleanUpAgent();
			}
		});
	}
}
