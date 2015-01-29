/*************************************************************************
 *  THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
 *  CODE WRITTEN BY OTHERS. LIANG TAO
 *
 *  DATE SUBMIITED: 10/07/2013
 *  Compilation:  javac -cp :./stdlib.jar DocCompare.java
 *  Execution:    java  -cp :./stdlib.jar DocCompare a.txt b.txt
 *************************************************************************/

import java.util.*;

public class DocCompare {
// three methods for calculating the similarity of documents:

	public static double nestedLoop(String[] doc1, String[] doc2) {
// nested loop: sort doc1, for each unique word in doc1, find all
// occurrences in doc2 using a sequential search;
	Arrays.sort(doc1);
	TreeSet<String> doc1words = new TreeSet<String>();
	for(int i = 0; i <= doc1.length - 1; i++) doc1words.add(doc1[i]);
	String[] doc11 = new String[doc1words.size()];
	doc1words.toArray(doc11);

	//finding the occurrence in doc2
	int sum = 0;
	for(int i = 0; i <= doc11.length - 1; i++){
		for(int j = 0; j <= doc2.length - 1; j++){
			if(doc11[i].equals(doc2[j])) sum++;
		}
	}
	//return the similarity
	return sum / (double)doc2.length;
}


	public static double indexNestedLoop(String[] doc1, String[] doc2) {
// index nested loop: sort doc1 and doc2, for each unique word in doc1,
// find all occurrences in doc2 using a binary search
	Arrays.sort(doc1);
	Arrays.sort(doc2);

	//returning an array only containing the words occur once
	TreeSet<String> doc1words = new TreeSet<String>();
	for(int i = 0; i <= doc1.length - 1; i++) doc1words.add(doc1[i]);
	String[] doc11 = new String[doc1words.size()];
	doc1words.toArray(doc11);

	//binary search
	int sum = 0;

	for(int i = 0; i <= doc11.length - 1; i++){

		int low = 0;
		int high = doc2.length - 1;
		int mid = (low + high) / 2;
		String doc1word = doc11[i];

		while (low <= high) {
			mid = (low + high) / 2;
			if(doc1word.compareTo(doc2[mid]) == 0) {
				sum++;
				if(mid > 0 && mid < doc2.length - 1) {
					int midplus = mid + 1;
					int midminus = mid - 1;
					while(doc1word.equals(doc2[midplus])) {
						if(midplus == doc2.length - 1) break;
						else {
							sum++;
							midplus++;
						}
					}

					while(doc1word.equals(doc2[midminus])) {
						if(midminus == 0) break;
						else {
							sum++;
							midminus--;
						}
					}
					break;
				} else if(mid == 0) {
					int midplus = mid + 1;
					while(doc1word.equals(doc2[midplus])) {
						if(midplus == doc2.length - 1) break;
						else {
							sum++;
							midplus++;
						}
					}
					break;
				} else {
					int midminus = mid - 1;
					while(doc1word.equals(doc2[midminus])) {
						if(midminus == 0) break;
						else {
							sum++;
							midminus--;
						}
					}
					break;
				}
			} else if(doc1word.compareTo(doc2[mid]) < 0) {
				high = mid - 1;
			} else {
				low = mid + 1;
			}
		}
	} 
	
	//return the similarity
	return sum / (double)doc2.length;
}


	public static double sortMerge(String[] doc1, String[] doc2) {
// sort merge: sort doc1 and doc2, for each unique word in doc1, find
// all occurrences in doc2 by "merging"
	Arrays.sort(doc1);
	Arrays.sort(doc2);

	//returning an array only containing the words occur once
	TreeSet<String> doc1words = new TreeSet<String>();
	for(int i = 0; i <= doc1.length - 1; i++) doc1words.add(doc1[i]);
	String[] doc11 = new String[doc1words.size()];
	doc1words.toArray(doc11);

	//the merging array
	String[] theArray = new String[doc11.length + doc2.length];
	//pointer for the doc11 and doc22
	int i = 0;
	int j = 0;
	int k = 0;

	//sum
	int sum = 0;

	while(i < doc11.length && j < doc2.length){
		if(doc11[i].compareTo(doc2[j]) < 0) {
			theArray[k++] = doc11[i];
			i++;
		} else if(doc11[i].compareTo(doc2[j]) > 0) {
			theArray[k++] = doc2[j];
			j++;
		} else {
			theArray[k++] = doc2[j];
			sum++;
			j++;
		}
 	}

 	//return the similarity
 	return sum/ (double)doc2.length;

}

public static void main(String[] args) {
	if (args.length < 2) {
		System.out.println("java DocCompare file1 file2");
		System.exit(0);
	}

	In in1 = new In(args[0]); // open file 1
	In in2 = new In(args[1]); // open file 2

	String s; // a string used to hold the content of the two files
	
	s = in1.readAll();
	s = s.toLowerCase();
	s = s.replaceAll("[\",!.:;?()']", "");
	String[] firstDoc = s.split("\\s+");

	s = in2.readAll();
	s = s.toLowerCase();
	s = s.replaceAll("[\",!.:;?()']", "");
	String[] secondDoc = s.split("\\s+");

	long start = System.currentTimeMillis();
	System.out.println("Similarity:" + nestedLoop(firstDoc,secondDoc));
	long stop = System.currentTimeMillis();
	double elapsed = (stop - start) / 1000.0;
	System.out.println("Nested Loop: " + elapsed + " seconds");

	start = System.currentTimeMillis();
	System.out.println("Similarity:" + indexNestedLoop(firstDoc,secondDoc));
	stop = System.currentTimeMillis();
	elapsed = (stop - start) / 1000.0;
	System.out.println("Index Nested Loop: " + elapsed + " seconds");

	start = System.currentTimeMillis();
	System.out.println("Similarity:" + sortMerge(firstDoc,secondDoc));
	stop = System.currentTimeMillis();
	elapsed = (stop - start) / 1000.0;
	System.out.println("Sort Merge: " + elapsed + " seconds");

	}
}

