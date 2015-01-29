import java.util.*;

class Student implements Comparable<Student>{
	private String name;
	private int age;
	private String sex;

	public Student(String name, int age, String sex){
		this.name = name;
		this.age = age;
		this.sex = sex;
	}

	@Override 
	public int compareTo(Student x){
		if (this.age > x.age){
			return 1;
		}else if(this.age < x.age){
			return -1;
		}else { 
		    return 0;
		}
	}

	@Override 
	public String toString(){
		return " Person :   name:" + name + "  sex: " +  sex + " age: "+ age;
	}


}


class Com {
	public static void main(String[] args) {
		Student[] stu = {new Student("Liang", 25, "M"),
						 new Student("Lei", 24, "F"),
						 new Student("Jingkai", 26, "M")
						};

		System.out.println(Arrays.toString(stu));
		Arrays.sort(stu);
		System.out.println(Arrays.toString(stu));

	}
}