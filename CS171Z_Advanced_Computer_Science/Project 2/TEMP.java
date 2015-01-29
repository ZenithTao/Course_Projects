/**************************************************************************************************
 *  THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
 *  CODE WRITTEN BY OTHERS. LIANG TAO
 *
 *  DATE SUBMIITED: 12/10/2013
 *  Compilation:  javac -cp :./DrawRoute.jar TEMP.java
 *  Execution:     
 *  java -cp :./stdlib.jar TEMP <source> <target> < MapData.txt	|java -jar DrawRoute.jar -b usa.jpg
 **************************************************************************************************/
import java.util.*;
import java.math.*;

// City class providing the name & longitude and latitude
class City {

	// using two hash tables to store the information
	HashTable<String, Double> cityLongHashTable = new HashTable<String, Double>();
	HashTable<String, Double> cityLatHashTable = new HashTable<String, Double>();

	// read information from the file
	public City() {
		int NUMBER_OF_CITIES = StdIn.readInt();
		for(int i = 1; i <= NUMBER_OF_CITIES; i++) {
			String citiname = StdIn.readString();
			Double lo = StdIn.readDouble();
			Double lat = StdIn.readDouble();
			this.cityLongHashTable.put(citiname, lo);
			this.cityLatHashTable.put(citiname, lat);
		}
	}

	public Double getLongitude(String name) {
		return cityLongHashTable.get(name);
	}

	public Double getLatitude(String name) {
		return cityLatHashTable.get(name);
	}
}

class Connection {

	// using three hashtables to store the information
	HashTable<ArrayList, String> roadHashTable = new HashTable<ArrayList, String>();
	HashTable<ArrayList, Integer> distanceHashTable = new HashTable<ArrayList, Integer>();

	// an arraylist including the cities
	public ArrayList<String> cities = new ArrayList<String>();

	public Connection() {
		// start to read the information
		String cocity;		// the connected city
		String citiname;
		String road;		// the road between the connected cities
		int distance;		// distance between two cities
		int t;				// how many cities are connected with the given city, read from file

		while(!StdIn.isEmpty()) {
			citiname = StdIn.readString();
			cities.add(citiname);
			t = StdIn.readInt();

			for(int i = 1; i <= t; i++) {
				ArrayList<String> two_cities = new ArrayList<String>();
				cocity = StdIn.readString();
				distance = StdIn.readInt();
				two_cities.add(citiname);
				two_cities.add(cocity);
				road = StdIn.readString();
				roadHashTable.put(two_cities, road);
				distanceHashTable.put(two_cities, distance);
			}
		}
	}

	public int getDistance(String citya, String cityb) throws NullPointerException {
		ArrayList<String> cityset = new ArrayList<String>();
		cityset.add(citya);
		cityset.add(cityb);
		try {
		return distanceHashTable.get(cityset);
		} catch(NullPointerException e) {
			return 0;
		}
	}

	public String getRoad(String citya, String cityb) {
		ArrayList<String> cityset = new ArrayList<String>();
		cityset.add(citya);
		cityset.add(cityb);
		return roadHashTable.get(cityset);
	}

	public boolean isConnected(String citya, String cityb) {
		ArrayList<String> cityset = new ArrayList<String>();
		cityset.add(citya);
		cityset.add(cityb);
		if(distanceHashTable.get(cityset) == null)
			return false;
		else 
			return true;
	}
	public ArrayList<String> getCitiesList() {
		return cities;
	}

}

// the class to show the path between cities
class Path {
	private String endpoint;
	private String source;
	public int actcost; // actual cost of distance
	private Double estcost;
	private String target;

	// array to store the path information
	ArrayList<String> citylist = new ArrayList<String>();

	private City city;
	private Connection con;

	public Path(ArrayList<String> newlist, Path path) {
		this.city = path.city;
		this.con = path.con;
		this.source = path.source;
		this.target = path.target;
		this.estcost = path.estcost;
		this.citylist = newlist;
		this.endpoint = newlist.get(newlist.size() - 1);
		this.actcost = path.actcost + path.con.getDistance(
							newlist.get(newlist.size() - 2), 
							newlist.get(newlist.size() - 1));
	}

	public Path(String source, String target) {
		this.source = source;
		this.target = target;
		citylist.add(source);
		endpoint = source;
		actcost = 0;
		this.city = new City();
		this.con = new Connection();
	}

	// estcost cost of distance
	public Double getEstcost() {
		Double alat = city.getLatitude(source);
		Double blat = city.getLatitude(target);
		Double along = city.getLongitude(source);
		Double blong = city.getLongitude(target);
		
		Double dlat = Math.toRadians((blat - alat));
		Double dlon = Math.toRadians((blong - along));
		Double da = Math.toRadians(alat);
		Double db = Math.toRadians(blat);

		Double a = Math.sin(dlat / 2) * Math.sin(dlat / 2) + 
				   Math.sin(dlon / 2) * Math.sin(dlon / 2) *
				   Math.cos(da) * Math.cos(db);

		Double c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));
		Double estcost = 3959 * c;
		this.estcost = estcost;

		return estcost;
	}

	public ArrayList<String> extend(String city) {
			ArrayList<String> newlist = new ArrayList<String>();
			for(String e : this.citylist) newlist.add(e);
			newlist.add(city);
			return newlist;
	}

	public String getRoad(String citya, String cityb) {
		return con.getRoad(citya, cityb);
	}

	public boolean connected(String citya, String cityb) {
		return con.isConnected(citya, cityb);
	}

	public ArrayList<String> connectedList() {
		ArrayList<String> connectedlist = new ArrayList<String>();
		for(String city : con.getCitiesList()) {
			if(connected(city, endpoint)) {
				connectedlist.add(city);
			}
		}
		return connectedlist;
	}

	public ArrayList<String> getCities() {
		return con.getCitiesList();
	}

	public String getEndPoint() {
		return citylist.get(citylist.size() - 1);
	}

	public int getActcost() {
		return this.actcost;
	}
}


// this is the priority class
class PQ {
	private Path[] itarray;
	private int N;
	private String source;
	private String target;
	private Double estimate;

	public PQ(int capacity) {
		N = 0;
		itarray = new Path[capacity];
	}

	public boolean isEmpty() {
		return N == 0;
	}

	public void insert(Path path) {
		if(N == itarray.length - 1)
			resize(2 * itarray.length);
		itarray[N] = path;
		trikleUp(N++);
		this.estimate = path.getEstcost();
	}

	// a helper function to help insert an element
	public void trikleUp(int index) {
		int parent = (index - 1) / 2;
		Path last = itarray[index];

		while (index > 0 && (estimate + (double)(itarray[parent].getActcost())) 
					> (estimate + last.getActcost())) {
			itarray[index] = itarray[parent];
			index = parent;
			parent = (parent - 1) / 2;
		}
		itarray[index] = last;
	}


	public Path remove() {
		Path root = itarray[0];
		if(N > 0 && N == itarray.length / 4)
			resize(itarray.length / 2);
		itarray[0] = itarray[--N];
		trikleDown(0);
		return root;
	}

	// a helper functino to help to remove an element
	public void trikleDown(int index) {
		int largerChild;
		Path top = itarray[index];

        while (index < (N / 2)) {
            int leftChild = 2 * index + 1;
            int rightChild = leftChild + 1;
            
            if (rightChild < N && (estimate + (double)itarray[leftChild].getActcost()) > 
                			(estimate + (double)itarray[rightChild].getActcost())) 
                largerChild = rightChild;
            else 
            	largerChild = leftChild;

            if((estimate + (double)top.getActcost()) <= 
            				(estimate + (double)itarray[largerChild].getActcost()))
            	break;
    
            itarray[index] = itarray[largerChild];
            index = largerChild;
        }        
        itarray[index] = top;
    }

	// a helper function to resize the array
	public void resize(int capacity) {
		Path[] copy = new Path[capacity];
		for(int i = 0; i < N; i++) {
			copy[i] = itarray[i];
		}
		itarray = copy;
	}
}


// this is the main class
public class TEMP {
	public static void main(String[] args) {

		String source = args[0];
		String target = args[1];
		// echo back the information from the file
		In read = new In("MapData.txt");
		String all = read.readAll();
		System.out.println(all);

		// initialize a priority queue
		PQ pq = new PQ(16);
		ArrayList<String> newlist = new ArrayList<String>();
		// initialize the first path including only the source
		Path path = new Path(source, target);
		pq.insert(path);

		// main while loop
		while(!pq.isEmpty()) {
			Path guess = pq.remove();  
			// whether the last stop is target?
			String endpoint = guess.getEndPoint();
			if(target.equals(endpoint)) {
				path = guess;
				break;
			} else {
				for(String city : guess.connectedList()) {
					if(!guess.citylist.contains(city)){
					newlist = guess.extend(city);
					Path newpath = new Path(newlist, guess);
					pq.insert(newpath); 
					}
				}
			}
		}

		// start to print the route information on the Std output
		System.out.println("Route:  " + path.getActcost());
			for(int i = 0; i < path.citylist.size() - 1; i++) {
				System.out.println(path.citylist.get(i) + "  " + 
							   path.citylist.get(i + 1) + "  " +
							   path.getRoad(path.citylist.get(i),
							   path.citylist.get(i + 1)));
			}
		}
}









