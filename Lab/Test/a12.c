int main() 
{
	int a[10];
	int i = 0, N = 10;
	int max = 0; 
	while (i < N){
		        a[i] = i * i - i * 2 + 1;
				        i = i + 1;
	}
						i = 0;
						while (i < N){
							if (max < a[i]){
								max = a[0.5];
								i = i + 1;
							}
						}
						return 0;
}
