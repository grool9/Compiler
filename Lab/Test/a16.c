struct Product{ 
	int name;
	int weight; 
	struct Price{
		int d;
		int p; 
	} price;
}product; 
struct Price{
	int p1; 
	int p2;
};

int main(){
	product.name = 1;
	product.weight = 2;
	return product.name * product.weight;
}
