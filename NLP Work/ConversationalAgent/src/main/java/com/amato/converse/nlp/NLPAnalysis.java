package com.amato.converse.nlp;

import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.LocalTime;
import java.time.ZoneId;
import java.time.format.DateTimeParseException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Properties;
import java.util.regex.Pattern;

import org.apache.commons.math3.util.Pair;

import com.amato.converse.yelp.YelpAPI;

import edu.stanford.nlp.ling.CoreAnnotations;
import edu.stanford.nlp.ling.CoreAnnotations.NamedEntityTagAnnotation;
import edu.stanford.nlp.ling.CoreAnnotations.SentencesAnnotation;
import edu.stanford.nlp.ling.CoreAnnotations.TextAnnotation;
import edu.stanford.nlp.ling.CoreAnnotations.TokensAnnotation;
import edu.stanford.nlp.ling.CoreLabel;
import edu.stanford.nlp.pipeline.Annotation;
import edu.stanford.nlp.pipeline.StanfordCoreNLP;
import edu.stanford.nlp.time.SUTime.Temporal;
import edu.stanford.nlp.time.TimeAnnotations;
import edu.stanford.nlp.time.TimeAnnotator;
import edu.stanford.nlp.time.TimeExpression;
import edu.stanford.nlp.util.CoreMap;

/**
 * A simple corenlp example ripped directly from the Stanford CoreNLP website
 * using text from wikinews.
 */
public class NLPAnalysis {

	private final static NLPAnalysis INSTANCE = new NLPAnalysis();

	private static StanfordCoreNLP pipeline;

	public static NLPAnalysis getInstance() {
		return INSTANCE;
	}

	List<String> affirmatives = new ArrayList<>(
			Arrays.asList("yes", "all right", "very well", "of course", "by all means", "sure", "certainly",
					"absolutely", "indeed", "right", "affirmative", "in the affirmative", "agreed", "roger", "aye aye",
					"yeah", "yep", "yup", "ya", "uh-huh", "okay", "ok", "okey-dokey", "okey-doke", "yea", "aye"));

	List<String> negations = new ArrayList<>(Arrays.asList("no", "absolutely not", "most certainly not",
			"of course not", "under no circumstances", "by no means", "not at all", "negative", "never", "not really",
			"nope", "uh-uh", "nah", "not on your life", "no way", "no way José", "ixnay", "nay"));

	Pattern distanceRgx = Pattern.compile("([Mm]ile[s]?|[Mm][Ii]|([Kk]ilo)?[Mm]eter[s]?|[Kk][Mm]|[Ff]eet|[Ff][Tt])");

	Pattern imprlDistanceRgx = Pattern.compile("([Mm]ile[s]?|[Mm][Ii]|[Ff]eet|[Ff][Tt])");

	private NLPAnalysis() {
		// creates a StanfordCoreNLP object, with POS tagging, lemmatization,
		// NER, parsing, and coreference resolution
		Properties props = new Properties();

		props.put("annotators", "tokenize, ssplit, pos, lemma, ner, parse"); // ,
																				// depparse,
																				// natlog,
																				// openie
		pipeline = new StanfordCoreNLP(props);
		pipeline.addAnnotator(new TimeAnnotator("sutime", new Properties()));

		System.out.println("NLP Initialized");
	}

	public List<Pair<String, String>> analyzeText(String text) {
		Annotation document = new Annotation(text);

		// run all Annotators on this text
		pipeline.annotate(document);

		List<Pair<String, String>> foundEntities = new ArrayList<>();

		for (CoreMap sentence : document.get(SentencesAnnotation.class)) {
			// traversing the words in the current sentence
			// a CoreLabel is a CoreMap with additional token-specific methods
			for (CoreLabel token : sentence.get(TokensAnnotation.class)) {
				// this is the text of the token
				String word = token.get(TextAnnotation.class);
				String ne = token.get(NamedEntityTagAnnotation.class);
				if (!ne.equals("O")) {
					foundEntities.add(new Pair<>(word, ne));
				} else if (determineDistance(word)) {
					foundEntities.add(new Pair<>(word, "DISTANCE"));
				} else if (YelpAPI.isACategory(word)) {
					foundEntities.add(new Pair<>(word, "MISC"));
				}
			}

		}
		return foundEntities;
	}

	public boolean determineAffirmative(String text) {
		return affirmatives.contains(text.toLowerCase());
	}

	public boolean determineDistance(String text) {
		return distanceRgx.matcher(text.toLowerCase()).find();
	}

	public boolean determineNegation(String text) {
		return negations.contains(text.toLowerCase());
	}

	public int extractDistance(String text) {
		Annotation document = new Annotation(text);
		pipeline.annotate(document);
		for (CoreMap sentence : document.get(SentencesAnnotation.class)) {
			for (CoreLabel token : sentence.get(TokensAnnotation.class)) {
				String word = token.get(TextAnnotation.class);
				String ne = token.get(NamedEntityTagAnnotation.class);
				// we need to check the dependencies better here
				if (ne.equals("NUMBER")) {
					return Integer.valueOf(word);
				}
			}
		}
		return -1;
	}

	public long extractTime(String text) {
		Annotation annotation = new Annotation(text);
		annotation.set(CoreAnnotations.DocDateAnnotation.class, LocalDate.now().toString());
		pipeline.annotate(annotation);
		List<Temporal> possibleTimes = new ArrayList<>();
		for (CoreMap cm : annotation.get(TimeAnnotations.TimexAnnotations.class)) {
			possibleTimes.add(cm.get(TimeExpression.Annotation.class).getTemporal());
		}

		long unixTime;
		for (Temporal time : possibleTimes) {
			if (time.isApprox() || time.isRef()) {
				return -1;
			} else {
				try {
					LocalDate date = LocalDate.parse(time.toString().split(Pattern.quote("T"))[0]);
					LocalTime pTime = LocalTime.now();
					if (time.toString().contains("T")) {
						pTime = LocalTime.parse(time.toString().split(Pattern.quote("T"))[1]);
					}

					unixTime = LocalDateTime.of(date, pTime).atZone(ZoneId.systemDefault()).toEpochSecond();
					if (unixTime > (System.currentTimeMillis() / 1000L)) {
						return unixTime;
					}
				} catch (DateTimeParseException e) {
					String timeString = time.getRange().begin().toISOString();
					LocalDate date = LocalDate.parse(timeString.split(Pattern.quote("T"))[0]);
					LocalTime pTime = LocalTime.now();
					if (timeString.contains("T")) {
						pTime = LocalTime.parse(timeString.split(Pattern.quote("T"))[1]);
					}

					unixTime = LocalDateTime.of(date, pTime).atZone(ZoneId.systemDefault()).toEpochSecond();
					if (unixTime > (System.currentTimeMillis() / 1000L)) {
						return unixTime;
					}
				}
			}
		}
		return -1;
	}

	public List<Long> extractTimes(String text) {
		List<Long> times = new ArrayList();
		Annotation annotation = new Annotation(text);
		annotation.set(CoreAnnotations.DocDateAnnotation.class, LocalDate.now().toString());
		pipeline.annotate(annotation);
		List<Temporal> foundTimes = new ArrayList<>();
		for (CoreMap cm : annotation.get(TimeAnnotations.TimexAnnotations.class)) {
			foundTimes.add(cm.get(TimeExpression.Annotation.class).getTemporal());
		}

		List<LocalDate> dates = new ArrayList();
		LocalTime pTime = null;
		for (Temporal time : foundTimes) {
			if (time.isApprox() || time.isRef()) {
				continue;
			} else {
				try {
					dates.add(LocalDate.parse(time.toString().split(Pattern.quote("T"))[0]));
					if (time.toString().contains("T")) {
						pTime = LocalTime.parse(time.toString().split(Pattern.quote("T"))[1]);
					}
				} catch (DateTimeParseException e) {
					String timeString = time.getRange().begin().toISOString();
					dates.add(LocalDate.parse(timeString.split(Pattern.quote("T"))[0]));
					if (timeString.contains("T")) {
						pTime = LocalTime.parse(timeString.split(Pattern.quote("T"))[1]);
					}
				}
			}
			if ((dates.size() > 0) && (pTime != null)) {
				for (LocalDate date : dates) {
					long unixTime = LocalDateTime.of(date, pTime).atZone(ZoneId.systemDefault()).toEpochSecond();
					if (unixTime > (System.currentTimeMillis() / 1000L)) {
						times.add(unixTime);
					}
				}
				dates.clear();
				pTime = null;
			}
		}
		return times;
	}

	public boolean isImperial(String text) {
		return imprlDistanceRgx.matcher(text.toLowerCase()).find();
	}

	// this is definitely not the way to do it but lets add a colon to the
	// number
	public String replaceNumbersWithTimes(String text) {
		String fixedText = "";
		Annotation document = new Annotation(text);
		pipeline.annotate(document);
		for (CoreMap sentence : document.get(SentencesAnnotation.class)) {
			for (CoreLabel token : sentence.get(TokensAnnotation.class)) {
				String word = token.get(TextAnnotation.class);
				String ne = token.get(NamedEntityTagAnnotation.class);
				// we need to check the dependencies better here
				if (ne.equals("NUMBER")) {
					fixedText += word + ":00 ";
				} else {
					fixedText += word + " ";
				}
			}
		}
		return fixedText;
	}
}