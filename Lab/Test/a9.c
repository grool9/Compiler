struct INTPair{ 
	int a; 
	int b;
};
struct INT{
	int a2; 
	int b2; 
	int c2;
}q;
int split(struct INTPair pair) 
{
	int first = pair.a; 
	int last = pair.b; 
	int x[10];
	int pivot = first;
	int split_point = first;
	int i=first+ 1;
	int temp,temp2;
	while(i<=last)
	{
		if(x[i]<x[pivot])
		{
			split_point = split_point + 1;
			temp = x[i];
			x[i] = x[split_point];
			x[split_point] = temp;
		}
		i = i + 1;
	}
	temp2 = x[pivot];
	x[pivot] = x[split_point];
	x[split_point] = temp2;
	return split_point;
}

	int main()
	{
		return split(q);
	}
