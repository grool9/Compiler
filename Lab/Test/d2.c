struct Node{ 
	int no;
	int array[5]; 
	float value; 
	int nextno;
}node[100];

int initial_Node(struct Node temp1){ 
	int i = 0;
	while(i < 5){
		temp1.array[i] = i + i * (2 * i + 1); 
		i = i + 1;
	}
	        temp1.value = 0.0;
	temp1.no = temp1.nextno = -1; 
	return 1;
}

int link_Node(struct Node former, struct Node later){ 
	int result = 0;
if(former.nextno!=-1 && later.nextno == -1){ 
	int result = 0;
	later.nextno = former.nextno; 
	former.nextno = later.no; 
	return result;
}
else if(former.nextno == -1 && later.nextno != -1){
	int result = 1; 
	former.nextno = later.no; 
	return result;
}
return -1;
}

int main(){
	struct Node a, b;
	int i,N=100; 
	initial_Node(a); 
	initial_Node(b); 
	link_Node(a,b); 
	while(i < N){
		    initial_Node(node[i]);
			i = i + 1;
	}
	i=0; 
	while(i < N){
		link_Node(node[i],node[i+1]);
		                i = i + 1;
	}
	return 0;

}
