struct Food {
	int price;
	float weight; 
	struct Price{
		int p; 
	}price;
};

int main() 
{
	struct Food food; 
	food.price = 25; 
	food.weight = 0.5; 
	return 0;
}
