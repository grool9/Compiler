struct Circle{ 
	int x; 
	int y; 
	int r;
};

struct Square{ 
	int a; 
	int b;
};

int Calculation1(struct Square square){ 
	return square.a * square.b;
}

int Calculation2(struct Circle circle1);
int Calculation2(struct Circle circle){ 
	return 3 * circle.r * circle.r;
}

int Compare(struct Square s, struct Circle c); 
int Compare(struct Square s, struct Circle c){
	if(Calculation1(s)>Calculation2(c)) 
		return 1;
	else return -1;
}
