struct A{
	int a;
	int a_array[10][5];
	struct innerA{
		int innera[10];
	} innerA_node[2];
};

struct B{
	int b;
	int b_array[5][2];
	struct innerB{
		float innerb[20];
	} innerB_node[4];
};

struct C{
	int c;
	float cc[10][2];
};

struct D{
	int d;
	float dd[3][4];
};

struct E{
	int e;
	      int e_array[5][2]; 
		  struct innerE{
			  float innere[25][5]; 
		  } innerE_node[6];
};

int main(){
	struct A tempA;
	struct B tempB; 
	struct C tempC; 
	struct D tempD; 
	struct E tempE; 
	tempA = tempB; 
	tempC = tempD; 
	tempB = tempE; 
	return 0;
}
