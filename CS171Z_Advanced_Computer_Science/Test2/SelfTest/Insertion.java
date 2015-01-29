public class Insertion {
	public static void main(String[] args) {
		int[] mmm = {1,2,3,37,23,46,23,46,2};
		sort(mmm);
		for(int m : mmm) System.out.println(m);
	}

	public static void sort(int[] arsort) {
		for (int i = 1; i < arsort.length; i++) {
			int j = i;
			int next = arsort[i];
			while (j > 0 && arsort[j - 1] > next) {
				arsort[j] = arsort[j - 1];
				j--;
			}
			arsort[j] = next;
		}
	}
}