struct Food {
	int num, weight;
	float price; 
	float time;
};

int main() 
{
	struct Food food; 
	struct Food2{
		int weight1 = 2;
	}food2;
	food.weight = food2.weight1; 
	return food.weight;
}
