package com.amato.converse;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.control.SplitPane;
import javafx.stage.Stage;

public class Converser extends Application {

	public static void main(String[] args) {
		launch(args);
	}

	@Override
	public void start(Stage primaryStage) throws Exception {
		primaryStage.setTitle("Conversational Agent Example");
		SplitPane myPane = FXMLLoader.load(getClass().getResource("/assets/layout/main.fxml"));
		Scene myScene = new Scene(myPane);
		primaryStage.setWidth(306);
		primaryStage.setScene(myScene);
		primaryStage.show();
		primaryStage.setResizable(false);
	}

}
