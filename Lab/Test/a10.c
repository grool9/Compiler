int main() 
{
	int a[10][10], b[10][10]; 
	int i, j, N = 10; 
	while(i < N)
	{
		while(j < N) 
		{
			                a[i][j] = b[i][j] + i * j;
							                a[i][j-1] = a[i][j][0];
											                j = j + 1;
		}
		i = i + 1;
	}
	return 0;
}
