struct Food {
	int price;
	float weight;
}food;

int set_Item(struct Food temp){ 
	temp.price = 10;
	temp.weight = 0.5; 
	return 0;
}
int main() 
{
	int temp1 = 20; 
	struct Food food2; 
	food2.price = temp1; 
	food2.weight = 0.5;
	set_Item(food, food2); 
	return 0;
}
