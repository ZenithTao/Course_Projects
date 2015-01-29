/***********************************************************************************
 *  THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
 *  CODE WRITTEN BY OTHERS. LIANG TAO
 *
 *  DATE SUBMIITED: 10/08/2013
 *  Compilation:  javac -cp :./stdlib.jar OnlineQuery.java
 *  Execution:    java  -cp :./stdlib.jar OnlineQuery http://www.emory.edu a  a  10
 *														
 ***********************************************************************************/

import java.util.regex.Matcher;
import java.util.*;

public class OnlineQuery { 

    public static void main(String[] args) { 

	boolean DEBUG = false;

	if (args.length < 3) {
	    System.out.println("Usage: java OnlineQuery URL domain queryString [M] (Maximum)");
	    System.exit(0);
	}

        // timeout connection after 500 miliseconds
        System.setProperty("sun.net.client.defaultConnectTimeout", "500");
        System.setProperty("sun.net.client.defaultReadTimeout",    "800");

	// maximum number of pages to crawl
	int max = ((args.length == 4) ? Integer.parseInt(args[3]) : 25);
	int pageCount = 1; // for the initial page

	// query, may be another URL
	Page query;
	if (args[2].length() > 4 && args[2].substring(0,4).equals("http"))
	    query = new Page(args[2]);
	else {
	    query = new Page();
	    query.setContent(args[2]);
	}

        // initial web page
        Page start = new Page(args[0]);
	String domainName = args[1];

	System.out.println("domain: " + domainName);

	// resulting pages
	Page[] results = new Page[max]; 

        // list of web pages to be examined
        Queue<Page> q = new Queue<Page>();
        q.enqueue(start);

        // Linkedlist to control the repetition
		LinkedList<String> str = new LinkedList<String>();
        str.insert(args[0]);

        // breadth first search crawl of web
	System.out.println("+++++++ Crawling and Comparing ");
        while (!q.isEmpty()) {
            Page v = q.dequeue();

	    if (! v.response()) continue;

	    	results[pageCount - 1] = v;
	    		v.findSimilarity(query);
           	 	System.out.println(v);

	    		pageCount++;

	    if (pageCount > max) // limit reached
		break;


            // find all outgoing links
	    Matcher vMatcher = v.getMatcher();
            while (vMatcher.find()) {
                String w = vMatcher.group();

        if (str.insert(w)) {
		// look only in similar domain names
			if(w.contains(domainName)) {
			    q.enqueue(new Page(w));
			} else {
		  	  	System.out.println("skipping " + w);
			}
     	} else;
     	}
    }

	// create an instance of the MergeSort data type

	int numCollected = Math.min(pageCount - 1,max);
	Page[] collected = new Page[numCollected];

	for (int i = 0; i < numCollected; i++) {
	    collected[i] = results[i];
	}

	System.out.println("\n+++++++ Search Results:");
	System.out.println("# of results: " + numCollected);
	MergeSort myMerge = new MergeSort(collected);

	myMerge.sort();  // based on similarities with the query
	myMerge.show();

   }
}
