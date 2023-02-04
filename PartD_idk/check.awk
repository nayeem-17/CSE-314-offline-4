BEGIN {
	crossing=0;
	direction=-1;
	reached=0;
}

{
	id=$2;
	act=$3;
	dir=$4; 
	dir = (dir=="LEFT"?0:1) ;

	if ( !(crossing == 0 || direction == -1 || direction== dir ) ){
		printf("id=%d, act=%s, dir=%d\n",id,act,dir);
		printf("Error! baboons fight and ZzZ \n");
		exit ;
	}
	else direction=dir; 
	if( act == "going" ) crossing++ ;
	else if( act == "reached" ) {
		crossing--;
		reached++;
	}

	if( crossing > capacity ) {
		printf("id=%d, act=%s, dir=%d\n",id,act,dir);
		printf("capacity=%d\n",capacity);
		printf("Error! capacity exceeded\n") ;
		exit ;
	}
    
}

END {
	if (reached == (baboons_left+baboons_right)) {
		printf("OK. All test cases passed!\n");
	}
	else {
		printf("reached=%d, baboons_left=%d, baboons_right=%d\n",reached,baboons_left,baboons_right);
		printf("Error! some baboons didn't make it to the end!!\n", MAX - count, MAX);
	}
}
