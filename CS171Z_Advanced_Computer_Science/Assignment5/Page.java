/*************************************************************************
 *  THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
 *  CODE WRITTEN BY OTHERS. LIANG TAO
 *
 *  DATE SUBMIITED: 10/07/2013
 *  Compilation:  javac -cp :./stdlib.jar Page.java
 *  Execution:    java  -cp :./stdlib.jar Page
 *************************************************************************/

import java.util.regex.Pattern;
import java.util.regex.Matcher;

public class Page implements Comparable {
    private String theURL;          // the URL of the page
    private double theSimilarity;   // the similarity wrt the given query
    private boolean theResponse;    // whether or not the page can be viewed
    private String theContent;      // the HTML code of the entire page
    private Matcher theMatcher;     // for matching against the query

    public Page() {   // when we only want the content
	theURL = "";
	theSimilarity = 0;
	theResponse = true;
	theContent = "";
    }	

    public Page(String url) {
	theURL = url;
	theSimilarity = 0;    // no query provided yet
	theResponse = true;   

	In in = new In(theURL);  // try to open the page

	if (!in.exists()) {   // if no response, skip the rest
	    theResponse = false;
	    return;
	}
	
	theContent = in.readAll();

	if (theContent == null) {
	    theResponse = false;
	    return;
	}
	/*************************************************************
	 *  Find links of the form: http://xxx.yyy.zzz
	 *  \\w+ for one or more alpha-numeric characters
	 *  \\. for dot
	 *************************************************************/
	String  regexp  = "http://(\\w+\\.)*(\\w+)";
	Pattern pattern = Pattern.compile(regexp);

	theMatcher = pattern.matcher(theContent);
    }

    public boolean response()      { return theResponse;      }
    public String  getURL()        { return theURL;           }
    public String  getContent()    { return theContent;       }
    public Matcher getMatcher()    { return theMatcher;       }
    public double  getSimilarity() { return theSimilarity;    }

    public void    setContent(String content) {
	theContent = content;
    }
    
    public String toString() { 
	String content = "";

	// print the similarity, the url, and the content from about
	// halfway point to the larger of 200 characters and the end
	// of the page

// 	if (theContent != null) {
// 	    int half = theContent.length() / 2;
// 	    int upper = (theContent.length() < half + 200 ? 
// 			 theContent.length() : half + 200 - 1);
// 	    content = theContent.substring(half,upper);
// 	}
	return "- (" + theSimilarity + ") " + theURL + "\n" + content;
    }

    public void findSimilarity(Page other) {
	// replace this method with a more meaningful one
   		DocCompare comp = new DocCompare();
   		String s1 = this.theContent;
   		s1 = s1.toLowerCase();
   		s1 = s1.replaceAll("[\",!.:;?()']", "");
   		String[] thisdoc = s1.split("\\s+");

   		String s2 = other.theContent;
   		s2 = s2.toLowerCase();
   		s2 = s2.replaceAll("[\",!.:;?()']", "");
   		String[] otherdoc = s2.split("\\s+");

   		this.theSimilarity = comp.sortMerge(otherdoc, thisdoc);
    }

    public int compareTo(Object other) {
	if (this.theSimilarity < ((Page) other).theSimilarity) 
	    return 1;
	else if (this.theSimilarity > ((Page) other).theSimilarity) 
	    return -1;
	else 
	    return 0;
    }
}

