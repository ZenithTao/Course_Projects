public class Bubble {
	public static void main(String[] args) {
		int[] x ={1,2,3,1,2,66,2,135,1,4};
		sort(x);
		for(int xx : x) System.out.println(xx);
	}

	public static void sort(int[] m) {
		int n = m.length;
		for(int i = 0; i < n; i++) {
			for(int j = 1; j < n - i; j++) {
				if(m[j] < m[j - 1]) {
					int temp = m[j];
					m[j] = m[j - 1];
					m[j - 1] = temp;
				}
			}
		}
	}
}