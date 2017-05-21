struct Product{
	int name;
	float weight;
	float price;
	int date_from;
	int date_end;
};
struct salesList{
	float sale_no[100];
	int date;
};

struct Product set_ProductSingle(int name1, float weight1, float price1, int date_from1, int date_end1){
	struct Product result;
	result.name = name1;
	result.weight = weight1;
	result.price = price1;
	result.date_from = date_from1;
	result.date_end = date_end1;
	return result;
}

struct salesList set_SalesList(float sale_no1[100], int date1){ 
	struct salesList result2;
	int i, N = 100; 
	while(i < N){
	        result2.sale_no[i] = sale_no1[i];
			        i = i + 1;
}
result2.date = date1; 
return result2;
}

float calculateAll(struct Product p, struct salesList s){ 
	int resultAll;
float sum;
int i1, N1 = 100; 
while(i1 < N1){
	        sum = s.sale_no[i1] + sum;
			        i1 = i1 + 1;
}
return sum * p.price * p.weight;
}

int main(){
	struct Product p1;
	struct salesList s1;
	float list[100]; 
	calculateAll(set_ProductSingle(1,2.0,3.0,4,5),set_SalesList(list,6)); 
	calculateAll(p1,s1); 
	return 0;
}
