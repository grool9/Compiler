struct A{
	int a;
	int a_array[10][5]; 
	struct innerA{
		int innera[10]; 
	} innerA_node;
};

struct B{
	int b;
	int b_array[5][2]; 
	struct innerB{
		int innerb[20]; 
	} innerB_node;
};

struct C{
		int c;
		float cc;
};

struct D{
	int d;
	float dd;
};

int main(){
	struct A tempA, tempA2;
	struct B tempB;
	struct C tempC, tempC2;
	struct D tempD;
	tempA = tempB;
	tempC = tempD;
	tempA = tempA2;
	tempC = tempC2;
	return 0;
}
